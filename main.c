/********************************  Documentation ************************/
/*
 * main.c
 *
 * Created: 29/01/2018 11:46:42 ص
 * Author : Amr Mostafa
 */ 
/**************************** Preprocessors ******************************/
#define F_CPU	16000000UL			// define the CPU clock crystal used as 16MHZ
#include <avr/interrupt.h>
#include <avr/io.h>
#include "delay.h"					// include my own delay function prototype
#include "timers.h"					// include my own timers functions prototypes
#include "debounce.h"				// include my own debounce functions prototypes

// macro for players I/O registers
#define PLAYER1_DDR		DDRA
#define PLAYER1_PORT	PORTA
#define PLAYER2_DDR		DDRC
#define PLAYER2_PORT	PORTC

// macros for state output pattern in HEX
#define	READY_OUTPUT	0x1f	// 0x1f after & ~ operations will be 0x00
#define	LED1_OUTPUT		0x01
#define	LED2_OUTPUT		0x03
#define	LED3_OUTPUT		0x07
#define	LED4_OUTPUT		0x0f
#define	LED5_OUTPUT		0x1f
#define	WIN1_OUTPUT		0x1f	// 0x1f after & ~ operations will be 0x00
#define	WIN2_OUTPUT		0x1f

// macros for states names in index
//		name	  number
#define READY		0
#define LED1		1
#define LED2		2
#define LED3		3
#define LED4		4
#define LED5		5
#define WIN1		6
#define WIN2		7

// macro for states delay value in milli sec
#define FSM_DELAY	200
/******************* Prototypes and Global variables *********************/
typedef struct state
{
	unsigned char output;
	unsigned int delay;
	unsigned char next[2];		// array of 2 element, each refer to the next state
}state_type;					// declare new data type of the FSM struct

// define and initialize only 1 FSM for both 2 players with 8 states
// const: to save the FSM in ROM as no need to exhaust the RAM with non-changeable structure
state_type const FSM[8]=
{
//		output		delay		input0 input1
	{ READY_OUTPUT, 0 ,			 {READY,LED1} },	// state 0: READY
	{ LED1_OUTPUT , 0		   , {LED1,LED2} },		// state 1: LED1
	{ LED2_OUTPUT , 0		   , {LED2,LED3} },		// state 2: LED2
	{ LED3_OUTPUT , 0		   , {LED3,LED4} },		// state 3: LED3
	{ LED4_OUTPUT , 0		   , {LED4,LED5} },		// state 4: LED4
	{ LED5_OUTPUT , FSM_DELAY ,	 {WIN1,WIN1} },		// state 5: LED5
	{ WIN1_OUTPUT , FSM_DELAY ,  {WIN2,WIN2} },		// state 6: WIN1
	{ WIN2_OUTPUT , 200 ,		 {READY,READY} }	// state 7: WIN2
};

unsigned char player1_fsm_current_state = READY;	// variable to hold the current state of player1
unsigned char player2_fsm_current_state = READY;	// variable to hold the current state of player2
unsigned char player1_input;						// variable to hold the input of the player1
unsigned char player1_output;						// variable to hold the output pattern of the player1
unsigned char player2_input;						// variable to hold the input of the player2
unsigned char player2_output;						// variable to hold the output pattern of the player2
/******************************* Functions *********************************/
int main(void)
{
	PLAYER1_DDR |=0x1f;							// set player1 LEDS port pins as output
	PLAYER2_DDR |=0x1f;							// set player2 LEDS port pins as output
	Debounce_Init();							// initialize button deboune (actually this initialize the buttons I/O registers)
	Timer0_CTC_Interrupt_DELAY_Init();			// initialize Timer0 with CTC mode to perform buttons debounce in the ISR
	sei();										// enable the global interrupt flag to activate the Timer0 CTC ISR
	while(1)
	{	
		// FSM controller implementation: 1)OUTPUT >> 2)WAIT >> 3)INPUT >> 4)CHANGE TO NEXT STATE
		
		// if any player reached the the LED5 state"get a winner" .. then make the other player in the ready state
		if (player1_fsm_current_state == LED5) {
			player2_fsm_current_state = READY;
		}
		if (player2_fsm_current_state == LED5) {
			player1_fsm_current_state = READY;
		}
		
		// 1) OUTPUT
		player1_output = FSM[player1_fsm_current_state].output;		// get player1 output pattern from his FSM current state
		player2_output = FSM[player2_fsm_current_state].output; 	// get player2 output pattern from his FSM current state
		
		// check which state involves SET and  which CLEAR for player1
		if (player1_fsm_current_state == READY || player1_fsm_current_state == WIN1){
			PLAYER1_PORT &= ~player1_output;			// CLEAR the 5 LEDS when being at state READY OR WIN1
		} else{
			PLAYER1_PORT |= player1_output;				// all other states require SET operation
		}
		// check which state involves SET and  which CLEAR for player2
		if (player2_fsm_current_state == READY || player2_fsm_current_state == WIN1){
			PLAYER2_PORT &= ~player2_output;			// CLEAR the 5 LEDS when being at state READY OR WIN1
		} else{
			PLAYER2_PORT |= player2_output;				// all other states require SET operation
		}
		
		// 2) wait
		delay_ms(FSM[player1_fsm_current_state].delay);
		delay_ms(FSM[player2_fsm_current_state].delay);
		
		// Before reading players"buttons" input and during winning states "LED,WIN1,WIN2" FSM must not
		//		listen to any press from the other loser player "who is in the READY state" or his press will
		//		cause his FSM to detect the input of '1' and goes to the state after "READY" which is "LED1"
		//		in general the loser must stay in the "READY state" till the winning states of the winner are finished
		// This check is due to the button press keeping feature of the debounce library as it keeps buttons press
		//		even when main is busy with other code lines "as debounce reads buttons continuously each 10ms via interrupt"
		if (player1_fsm_current_state == LED5 || player1_fsm_current_state == WIN1 || player1_fsm_current_state == WIN2
			 || player2_fsm_current_state == LED5 || player2_fsm_current_state == WIN1 || player2_fsm_current_state == WIN2){
			buttons_down &= ~(1<<KEY0 | 1<<KEY1);		// clear all 2 buttons press to avoid keeping any press while FSM is at any winning state
		}
		
		// 3) INPUT
		player1_input = Get_Keys_Pressed(1<<KEY0);					// 0000 0001 OR 0000 0000 >> 0000 0001 OR 0000 0000
		player2_input = (Get_Keys_Pressed(1<<KEY1) ) >> KEY1;		// 0000 0010 OR 0000 0000 >> 0000 0001 OR 0000 0000
		
		// 4) CHANGE STATE ACCORDING TO CURRENT STATE AND INPUT
		player1_fsm_current_state = FSM[player1_fsm_current_state].next[player1_input];		// change state of player1 FSM  to the next[0] state OR next[1] state
		player2_fsm_current_state = FSM[player2_fsm_current_state].next[player2_input];		// change state of player2 FSM  to the next[0] state OR next[1] state
	}
}


