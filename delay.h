/**************************** Documentation ******************************/
/*
 * delay.h
 *
 * Created: 19/02/2018 09:28:48 م
 *  Author: Amr Mostafa
 */

/**************************** PreProcessors ******************************/
#ifndef DELAY_H_
#define DELAY_H_

#ifndef F_CPU						// if CPU clock not defined
#define F_CPU	16000000UL			// then define it as  16MHZ
#endif

/******************* Prototypes and Global variables *********************/

// wrapper function for the built-in delay function
// used to make delay in units of milli seconds
// INPUT: number of milli seconds
// OUTPUT: ...
// NOTES: - you must define/edit the F_CPU in this delay.h file with the current CPU clock
//		  - this function wrap the built-in _delay_xx() functions
//			to make precise busy-wait loop delay with different delay
//			values passed in run-time instead of constant values
//			passed in compile-time to the _delay_xx() built-in functions
void delay_ms(unsigned int num_1ms);

#endif /* DELAY_H_ */