/**
 * @file PWM0_0.c
 *
 * @brief Source file for the PWM0_0 driver.
 *
 * This file contains the function definitions for the PWM0_0 driver.
 * It uses the Module 0 PWM Generator 0 to generate a PWM signal using the PB6 pin.
 *
 * @note This driver assumes that the system clock's frequency is 50 MHz.
 *
 * @note This driver assumes that the PWM_Clock_Init function has been called
 * before calling the PWM0_0_Init function.
 *
 * @author Aaron Nanas
 */

#include "PWM0_0.h"
 
void PWM0_0_Init(uint16_t period_constant, uint16_t duty_cycle)
{	
	if (duty_cycle >= period_constant) return;
	
	SYSCTL -> RCGCPWM |= 0x01; 

	SYSCTL -> RCGCGPIO |= 0x02;

  GPIOB-> AFSEL |= 0x40;

 	GPIOB -> PCTL &=~ 0x0F000000;
	
	GPIOB -> PCTL |= 0x04000000;
	
	GPIOB -> DEN |= 0x40;
	
	PWM0 -> _0_CTL &=~ 0x01;
	
	PWM0 -> _0_CTL &=~ 0x02;
	
	PWM0 -> _0_CTL &=~ 0x02;
	
	PWM0 -> _0_GENA |= 0xC0;
	
	PWM0 -> _0_GENA |= 0x08;
	
	PWM0 -> _0_LOAD = (period_constant - 1);
	
	PWM0 -> _0_CMPA = (duty_cycle-1);
	
	PWM0 -> _0_CTL |= 0x01;
	
	PWM0 -> ENABLE |= 0x01;
	
}

void PWM0_0_Update_Duty_Cycle(uint16_t duty_cycle)
{
PWM0-> _0_CMPA = (duty_cycle - 1);
}
