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

#include "PWM0_1.h"
 
void PWM0_1_Init(uint16_t period_constant, uint16_t duty_cycle)
{	
	if (duty_cycle >= period_constant) return;
	
	//Enable the clock to PWM Module 0 (Bit 0) in the RCGCPWM
	SYSCTL -> RCGCPWM |= 0x01; 
	//Enable the clock to GPIO Port B  (Bit 1) in the RCGCGPIO
	SYSCTL -> RCGCGPIO |= 0x02;
	
	//Configure the PB7 pin (M1PWM6) by setting Bit 4
  GPIOB-> AFSEL |= 0x80;
	//Clear the PMC2 field (Bits 19 to 16) in the PCTL register for Port B.
 	GPIOB -> PCTL &=~ 0xF0000000;
	//Configure the PB7 pin writing 0x4 (Bits 31 to 28) in the PCTL register.
	GPIOB -> PCTL |= 0x40000000;
	
	//Digital Enabled for the PB4 (Bit 4) in the DEN register
	GPIOB -> DEN |= 0x80;
	//Disable the Module 0 PWM 1 Generator block (PWM0_1) before configuration by clearing the ENABLE bit (Bit 0) in the PWM3CTL register.
	PWM0 -> _1_CTL &=~ 0x01;
	
	
	
	PWM0 -> _1_CTL &=~ 0x02;
	
	//PWM0 -> _1_CTL &=~ 0x02;
	
	PWM0 -> _1_GENA |= 0xC0;
	
	PWM0 -> _1_GENA |= 0x08;
	
	PWM0 -> _1_LOAD = (period_constant - 1);
	
	PWM0 -> _1_CMPA = (duty_cycle-1);
	
	PWM0 -> _1_CTL |= 0x01;
	
	// Enable the PWM0_1 signal to be passed to the PB4 pin (M0PWM2) by setting the PWM2EN bit (Bit 2) in the PWMENABLE register. 
	PWM0 -> ENABLE |= 0x04;
	
}

void PWM0_1_Update_Duty_Cycle(uint16_t duty_cycle)
{
PWM0-> _1_CMPA = (duty_cycle - 1);
}
