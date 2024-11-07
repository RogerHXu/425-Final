/**

 * @file SysTick_Interrupt.c
 *
 * @brief Source code for the SysTick_Interrupt driver
 *
 
 This function must configure the SysTick timer to
generate an interrupt every 1 millisecond. Assume that the SysTick timer will have a
clock of 4 MHz from the Precision Internal Oscillator (PIOSC). 
 
 * @author Lenny Marron
 
 */
 
 #include "TM4C123GH6PM.h"
 #include "SysTick_Interrupt.h"
 
 void SysTick_Interrupt_Init (void)
 {
 
  //SysTick timer reload value for 1 ms intervals
	// Each clock cycle is (1 / 4 MHz) = 0.25 us
	SysTick->LOAD = 3999;
	
	// Clearing the VAL register 
	SysTick->VAL = 0;
	
	// Bit 1 & 0 Enables the SysTick timer and its interrupt
	// Bit 2 Peripheral Internal Oscillator (PIOSC) as the clock source
	 // 011
	SysTick->CTRL |= 0x03;
 
 }
	 