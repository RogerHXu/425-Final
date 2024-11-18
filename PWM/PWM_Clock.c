/**
 * @file PWM_Clock.c
 *
 * @brief Source file used to configure the PWM clock source.
 *
 * This file contains the function definitions for configuring the PWM clock.
 * It updates the RCC register from SYSCTL to update the PWM clock and use
 * the PWM clock divider as the source.
 *
 * When the PWM divisor is used, it is applied to the clock for both PWM modules.
 *
 * @note This driver assumes that the system clock's frequency is 50 MHz.
 *
 * @author Aaron Nanas
 */
 
#include "PWM_Clock.h"
 
void PWM_Clock_Init(void)
{
  SYSCTL-> RCC |=  0x00100000;
	SYSCTL-> RCC &= ~0x000E0000;
	
	SYSCTL-> RCC &= ~0x00060000;
	
}
