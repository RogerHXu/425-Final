/**

 * @file SysTick_Interrupt.h
 *
 * @brief Header file for the SysTick_Interrupt driver
 *
 
 * @author Lenny Marron
 
 */
 
 #include "TM4C123GH6PM.h"
 
 
/**

 * @brief The SysTick_Interrupt_Init fucntion initializes the SysTick timer with interrupts enabled. 
 *
 * @brief Source code for the SysTick_Interrupt driver
 *
 
 * This function configures the SysTick timer and its interrupt with a specified reload value to
 * generate interrupts every 1 ms. It uses the PIOSC as the clock source.
 * The PIOSC provides 16 MHz whihc is then divided by 4. The timer used to generate periodic interrupts
 * every 1ms
 
 * @author Lenny Marron
 
 */
 
 
 void SysTick_Interrupt_Init (void);