/**
 * @file PWM0_1.c
 *
 * @brief Source file for the PWM0_1 driver.
 *
 * This file contains the function definitions for the PWM0_1 driver.
 * It uses the Module 0 PWM Generator 1 to generate a PWM signal using the PB4 pin.
 *
 * @note This driver assumes that the system clock's frequency is 50 MHz.
 *
 * @note This driver assumes that the PWM_Clock_Init function has been called
 * before calling the PWM0_1_Init function.
 *
 * @author Lenny Marron
 */

#include "PWM0_1.h"
 
void PWM0_1_Init(uint16_t period_constant, uint16_t duty_cycle)
{	
	if (duty_cycle >= period_constant) return;
	
	//Enable the clock to PWM Module 0 (Bit 0) in the RCGCPWM
	SYSCTL -> RCGCPWM |= 0x01; 
	
	//Enable the clock to GPIO Port B  (Bit 1) in the RCGCGPIO
	SYSCTL -> RCGCGPIO |= 0x02;
	
	//Configure the PB4 (M0PWM2) by setting Bit 4 and PB5 on pin by setting Bit 5
  GPIOB-> AFSEL |= 0x30;
	
	//Clear the PMC4 & PMC5 field (Bits 23 to 20)(Bits 19 to 16) in the PCTL register for Port B.
 	GPIOB -> PCTL &=~ 0x00FF0000;
	
	//Configure the PB4 & PB5 pin writing 0x4 (Bits 23 to 20)(Bits 19 to 16) in the PCTL register.
	GPIOB -> PCTL |= 0x00440000;
	
	//Digital Enabled for the PB4 (Bit 4) PB5 (Bit 5) in the DEN register
	GPIOB -> DEN |= 0x30;
	
	//Disable the Module 0 PWM 1 Generator block (PWM0_1) before config 
	//by clearing the ENABLE bit (Bit 0) in the PWM1CTL register.
	PWM0 -> _1_CTL &=~ 0x01;
	
	
	//Configure the counter for the PWM0_1 block to use Count-Down mode by clearing (Bit 1)
	//The counter will count from the load value to 0, and then wrap back to the load value. 
	PWM0 -> _1_CTL &=~ 0x02;
	
	//drive high when equal to compA
	PWM0 -> _1_GENA |= 0xC0;
	
	//drive high when equal to Load
	PWM0 -> _1_GENA |= 0x08;
	
	//sets a load value
	PWM0 -> _1_LOAD = (period_constant - 1);
	
	//sets a value for compA
	PWM0 -> _1_CMPA = (duty_cycle-1);
	
	//Enables the PWM Module
	PWM0 -> _1_CTL |= 0x01;
	
	// Enable the PWM0_1 (pwm1A) signal to be passed to the PB4 pin (M0PWM2) bit 2  
	// Enable the PWM0_1 (pwm1B) signal to be passed to the PB5 pin (M0PWM3) bit 3  
	PWM0 -> ENABLE |= 0x0C;
	
}

void PWM0_1_Update_Duty_Cycle(uint16_t duty_cycle)
{
PWM0-> _1_CMPA = (duty_cycle -1);
}
