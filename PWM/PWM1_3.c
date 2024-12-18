/**
 * @file PWM1_3.c
 *
 * @brief Source file for the PWM1_3 driver.
 *
 * This file contains the function definitions for the PWM1_3 driver.
 * It uses the Module 1 PWM Generator 3 to generate a PWM signal with the PF2 pin.
 *
 * @note This driver assumes that the system clock's frequency is 50 MHz.
 *
 * @note This driver assumes that the PWM_Clock_Init function has been called
 * before calling the PWM1_3_Init function.
 *
 * @author Aaron Nanas
 */
 
#include "PWM1_3.h"
 
void PWM1_3_Init(uint16_t period_constant, uint16_t duty_cycle)
{	
	
	if (duty_cycle >= period_constant) return;
	

	//Enable PWM Clock Module 1
	SYSCTL -> RCGCPWM |= 0x02;
	
		//Enable GPIO clock port F
	SYSCTL -> RCGCGPIO |= 0x20;
	
		//Alt Func to bits 2 & 3
	GPIOF-> AFSEL|= 0x04;
	
	GPIOF -> PCTL &= ~ 0x00000F00;
	
	GPIOF -> PCTL |= 0x00000500;
	
	GPIOF -> DEN |= 0x04;

	PWM1-> _3_CTL &= ~ 0x01;
	
	PWM1-> _3_CTL &= ~ 0x02;
	
	PWM1-> _3_GENA |= 0xC0;
	
	PWM1-> _3_GENA |= 0x08;
	
	PWM1-> _3_LOAD = (period_constant -1) ;
	
	PWM1-> _3_CMPA = (duty_cycle-1);
	
	PWM1-> _3_CTL |=  0x01;
	
	// Enable bit 6 for M1PWM6 and bit 7 for M1PWM7
	PWM1-> ENABLE |=  0x40;

}

void PWM1_3_Update_Duty_Cycle(uint16_t duty_cycle)
{
	
	PWM1-> _3_CMPA = (duty_cycle -1);
	}
