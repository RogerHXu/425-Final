/**
 * @file PWM1_1.c
 *
 * completed 11/17/24
 *
 * @brief Source file for the PWM1_1 driver.
 *
 * This file contains the function definitions for the PWM1_1 driver.
 * It uses the Module 1 PWM Generator 1 to generate a PWM signal with the PA7 pin.
 *
 * @note This driver assumes that the system clock's frequency is 50 MHz.
 *
 * @note This driver assumes that the PWM_Clock_Init function has been called
 * before calling the PWM1_1_Init function.
 *
 * @author Lenny Marron
 */
 
#include "PWM1_1.h"
 
void PWM1_1_Init(uint16_t period_constant, uint16_t duty_cycle)
{	
	
	if (duty_cycle >= period_constant) return;
	
	//Enable the clock to PWM Module 1 (Bit 2) in the RCGCPWM
	SYSCTL -> RCGCPWM |= 0x02; 
	
	//Enable the clock to GPIO Port D  (Bit 3) in the RCGCGPIO
	SYSCTL -> RCGCGPIO |= 0x04;
	
	//Configure the PD1 pin (M1PWM6) by setting (Bit 0)
  GPIOD-> AFSEL |= 0x02;
	
	//Clear the PMC2 field (Bits 4 to 7) for Mux0 based on PD0 in the PCTL register.
 	GPIOD -> PCTL &=~ 0x000000F0;
	
	//Configure the PD1 pin writing 0x5 ( in the PCTL register. 5 is from the AF coding.
	GPIOD -> PCTL |= 0x00000050;
	
	
	//Digital Enabled for the PD1 (Bit 1) in the DEN register
	GPIOD -> DEN |= 0x02;
	
	//Disable the Module 1 PWM 0 Generator block (PWM0_3) before 
	//configuration by clearing the ENABLE bit (Bit 0) in the PWM3CTL register.
	PWM1 -> _0_CTL &=~ 0x01;
	
	//Configure the counter for the PWM0_3 block to use Count-Down mode by clearing
  //the MODE bit (Bit 1) in the PWM3CTL register. The counter will count from the load
  //value to 0, and then wrap back to the load value.
	PWM1 -> _0_CTL &=~ 0x02;
	
	
	//Set the ACTCMPAD field (Bits 7 to 6) to 0x3 in the PWM3GENA register to drive the
  //PWM signal high when the counter matches the comparator (i.e. the value in the
  //PWM3CMPA register) while it is counting down.
	PWM1 -> _0_GENA |= 0xC0;
	
	
	//Set the ACTLOAD field (Bits 3 to 2) to 0x2 in the PWM3GENA register to drive the
  //PWM signal low when the counter matches the value in the PWM3LOAD register. 
	PWM1 -> _0_GENA |= 0x08;
	
	//Set the period of the PWM signal by writing to the LOAD field (Bits 15 to 0) in the
  //PWM3LOAD register. This determines the number of clock cycles needed to count
  //down to zero.
	PWM1 -> _0_LOAD = (period_constant - 1);
	
	
	//Set the duty cycle of the PWM signal by writing to the COMPA field (Bits 15 to 0) in
  //the PWM3CMPA register. When the counter matches the value in this register, the
  //PWM signal will be driven high. 
	PWM1 -> _0_CMPA = (duty_cycle-1);
	
	
	//Enable the PWM1_3 block after configuration by setting the ENABLE bit (Bit 0) in
  //the PWM3CTL register. 
	PWM1 -> _0_CTL |= 0x01;
	
	// Enable the PWM1_3 signal to be passed to the PD0 pin (M0PWM6) 
	//by setting the PWM6EN bit (Bit 6) in the PWMENABLE register. 
	PWM1 -> ENABLE |= 0x04;
	
}

void PWM1_1_Update_Duty_Cycle(uint16_t duty_cycle)
{
	
	PWM1-> _1_CMPA = (duty_cycle - 1);
	}
