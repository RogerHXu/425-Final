/**
 * @file PWM0_3.c
 *
 * @brief Source file for the PWM0_3 driver.
 *
 * This file contains the function definitions for the PWM0_3 driver.
 * It uses the Module 0 PWM 3 Generator  to generate a PWM signal using the PD0 pin.
 *
 * @note This driver assumes that the system clock's frequency is 50 MHz.
 *
 * @note This driver assumes that the PWM_Clock_Init function has been called
 * before calling the PWM0_3_Init function.
 *
 * @author Lenny Marron
 */

#include "PWM0_3.h"
 
void PWM0_3_Init(uint16_t period_constant, uint16_t duty_cycle)
{	
	if (duty_cycle >= period_constant) return;
	
	//Enable the clock to PWM Module 0 (Bit 0) in the RCGCPWM
	SYSCTL -> RCGCPWM |= 0x01; 
	
	//Enable the clock to GPIO Port D  (Bit 3) in the RCGCGPIO
	SYSCTL -> RCGCGPIO |= 0x04;
	
	//Configure the PD0 pin (M1PWM6) by setting (Bit 0)
  GPIOD-> AFSEL |= 0x01;
	
	//Clear the PMC2 field (Bits 3 to 0) for Mux0 based on PD0 in the PCTL register.
 	GPIOD -> PCTL &=~ 0x0000000F;
	
	//Configure the PD0 pin writing 0x4 (Bits 27 to 24) in the PCTL register. 4 is from the AF coding.
	GPIOD -> PCTL |= 0x00000004;
	
	
	//Digital Enabled for the PD0 (Bit 0) in the DEN register
	GPIOD -> DEN |= 0x01;
	
	//Disable the Module 0 PWM 3 Generator block (PWM0_3) before 
	//configuration by clearing the ENABLE bit (Bit 0) in the PWM3CTL register.
	PWM0 -> _3_CTL &=~ 0x01;
	
	//Configure the counter for the PWM0_3 block to use Count-Down mode by clearing
  //the MODE bit (Bit 1) in the PWM3CTL register. The counter will count from the load
  //value to 0, and then wrap back to the load value.
	PWM0 -> _3_CTL &=~ 0x02;
	
	
	//Set the ACTCMPAD field (Bits 7 to 6) to 0x3 in the PWM3GENA register to drive the
  //PWM signal high when the counter matches the comparator (i.e. the value in the
  //PWM3CMPA register) while it is counting down.
	PWM0 -> _3_GENA |= 0xC0;
	
	
	//Set the ACTLOAD field (Bits 3 to 2) to 0x2 in the PWM3GENA register to drive the
  //PWM signal low when the counter matches the value in the PWM3LOAD register. 
	PWM0 -> _3_GENA |= 0x08;
	
	//Set the period of the PWM signal by writing to the LOAD field (Bits 15 to 0) in the
  //PWM3LOAD register. This determines the number of clock cycles needed to count
  //down to zero.
	PWM0 -> _3_LOAD = (period_constant - 1);
	
	
	//Set the duty cycle of the PWM signal by writing to the COMPA field (Bits 15 to 0) in
  //the PWM3CMPA register. When the counter matches the value in this register, the
  //PWM signal will be driven high. 
	PWM0 -> _3_CMPA = (duty_cycle-1);
	
	
	//Enable the PWM1_3 block after configuration by setting the ENABLE bit (Bit 0) in
  //the PWM3CTL register. 
	PWM0 -> _3_CTL |= 0x01;
	
	// Enable the PWM1_3 signal to be passed to the PD0 pin (M0PWM6) 
	//by setting the PWM6EN bit (Bit 6) in the PWMENABLE register. 
	PWM0 -> ENABLE |= 0x04;
	
}

void PWM0_3_Update_Duty_Cycle(uint16_t duty_cycle)
{
PWM0-> _3_CMPA = (duty_cycle - 1);
}
