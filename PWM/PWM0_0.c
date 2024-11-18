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
	
	//Enable the clock to PWM Module 0 (Bit 1) in the RCGCPWM
	SYSCTL -> RCGCPWM |= 0x01; 
	//Enable the clock to GPIO Port F  (Bit 5) in the RCGCGPIO
	SYSCTL -> RCGCGPIO |= 0x02;
	
	//Configure the PB6 pin (M1PWM6) by setting Bit 6
  GPIOB-> AFSEL |= 0x40;
	//Clear the PMC2 field (Bits 27 to 24) in the PCTL register for Port B.
 	GPIOB -> PCTL &=~ 0x0F000000;
	//Configure the PB6 pin writing 0x4 (Bits 27 to 24) in the PCTL register.
	GPIOB -> PCTL |= 0x04000000;
	
	//Digital Enabled for the PB6 (Bit 6) in the DEN register
	GPIOB -> DEN |= 0x40;
	//Disable the Module 0 PWM 0 Generator block (PWM0_0) before configuration by clearing the ENABLE bit (Bit 0) in the PWM3CTL register.
	PWM0 -> _0_CTL &=~ 0x01;
	
	PWM0 -> _0_CTL &=~ 0x02;
	
	PWM0 -> _0_CTL &=~ 0x02;
	
	PWM0 -> _0_GENA |= 0xC0;
	
	PWM0 -> _0_GENA |= 0x08;
	
	PWM0 -> _0_LOAD = (period_constant - 1);
	
	PWM0 -> _0_CMPA = (duty_cycle-1);
	
	PWM0 -> _0_CTL |= 0x01;
	
	// Enable the PWM1_3 signal to be passed to the PF2 pin (M1PWM6) by setting the PWM6EN bit (Bit 6) in the PWMENABLE register. 
	PWM0 -> ENABLE |= 0x01;
	
}

void PWM0_0_Update_Duty_Cycle(uint16_t duty_cycle)
{
PWM0-> _0_CMPA = (duty_cycle - 1);
}
