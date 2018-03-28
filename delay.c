/**************************** Documentation ******************************/
/*
 * delay.c
 *
 * Created: 19/02/2018 09:29:08 م
 *  Author: Amr Mostafa
 */ 

/**************************** PreProcessors ******************************/
#include "delay.h"
#include <util/delay.h>

/************************ Functions Definitions **************************/

// wrapper function for the built-in delay function
// used to make delay in units of milli seconds
// INPUT: number of milli seconds
// OUTPUT: ...
// NOTES: - you must define/edit the F_CPU in the delay.h file with the current CPU clock
//		  - this function wrap the built-in _delay_xx() functions
//			to make precise busy-wait loop delay with different delay
//			values passed in run-time instead of constant values
//			passed in compile-time to the _delay_xx() built-in functions     
void delay_ms(unsigned int num_1ms)
{
	while (num_1ms)			// busy-wait loop until there no more milli seconds to wait
	{
		_delay_ms(1);		// wait 1 milli second using the built-in delay function
		num_1ms--;			// decrease the number of milli seconds passed by 1
	}
}
