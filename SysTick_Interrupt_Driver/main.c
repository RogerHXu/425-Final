/**
 * @file GPIO.c
 *
 * @brief Source code for the GPIO driver.
 *
 * This file contains the function definitions for the GPIO driver.
 * It interfaces with the following:
 *  - User LED (RGB) Tiva C Series TM4C123G LaunchPad
 *
 * @author Lenny Marron
 */

#include "TM4C123GH6PM.h"
#include "SysTick_Interrupt.h"
#include "GPIO.h"

static uint32_t ms_elapsed = 0;

void SysTick_Handler (void)
{
ms_elapsed = ms_elapsed +1;
	
	if (ms_elapsed == 500)
	{
	  ms_elapsed = 0;
		RGB_LED_Toggle (RGB_LED_GREEN);
	}
}

int main (void)
{
SysTick_Interrupt_Init ();
	
	RGB_LED_Init();
	
	while (1)
		
	{
	
	}
}