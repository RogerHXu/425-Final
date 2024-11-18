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
	
	//clocks PWM Module 1 (Bit 1)
	SYSCTL -> RCGCPWM |= 0x02;
	
	//Clocks GPIO Port A (Bit 0)
	SYSCTL -> RCGCGPIO |= 0x01;
	
	//Sets PA7 Alt Function  (bit 7)
	GPIOA-> AFSEL |= 0x80;
	
	//Clears (Bits 31-28)  in PMC reg Port A
	GPIOA -> PCTL &= ~ 0xF0000000;
	
	//Sets Pin Assignment in (Bits 31-28)  in PMC reg
	GPIOA -> PCTL |= 0x50000000;
	
	//Set Pin 7 as digital enabled
  GPIOA -> DEN |= 0x80;

  //Clears ENABLE bit (Bit 0) from Gen Block 1
	PWM1-> _1_CTL &= ~ 0x01;
	
  //Clear MODE bit (Bit 1) from Gen Block 1
	PWM1-> _1_CTL &= ~ 0x02;
	
	PWM1-> _1_CTL &= ~ 0x02;
	
	// Sets ACTCMPAD (Bit 7 & 6) high to drive PWM High when Count matches CompA
	PWM1-> _1_GENA |= 0xC0;
	
	//Sets ACTLOAD (Bit 3 & 2) as 10 to drive PWM LOW when Count = LOAD
	PWM1-> _1_GENA |= 0x08;
	
	// Sets PWM LOAD as a variable to be adjusted by the period max vale of 65,535
	PWM1-> _1_LOAD = (period_constant -1) ;
	
	//Sets PWM COMPARATOR A
	PWM1-> _1_CMPA = (duty_cycle-1);
	
	//Enables PWM (bit 0) from Gen Block
	PWM1-> _1_CTL |=  0x01;
	
	// PWM signal to Pin PA7 M1 PWM3 Gen1  (bit 3)
	PWM1-> ENABLE |=  0x08;

}

void PWM1_1_Update_Duty_Cycle(uint16_t duty_cycle)
{
	
	PWM1-> _1_CMPA = (duty_cycle - 1);
	}
