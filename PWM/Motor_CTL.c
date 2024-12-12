/**
 * @file Motor_CTL.c
 *
 * @brief Source file for the DRV8833 driver.
 *
 * This file contains the function definitions for the DRV8833 driver.
 * PWM signals with a DRV8833 breackout board	& Two DC GearMotors 
 *				- Left motor controlled  PB6 (PWM0_0)FWD    PB7 (PWM0_1) REV
 *				- Right motor controlled PD0 (PWM0_3)FWD    PD1 (PWM1_3) REV
 *
 * @note This driver assumes that the PWM_Clock_Init, PWM0_0, PWM0_1, PWM0_3, and PWM1_3 
 * functions has been called
 * 
 *
 * @author Lenny Marron
 */

#include "Motor_CTL.h"
#include "PWM_Clock.h"
#include "PWM0_0.h"
#include "PWM0_1.h"
#include "PWM1_3.h"
#include "PWM1_1.h"


void Move_FWD (int Speed)
{
	
	// Left motor controlled  PB6 (PWM0_0)FWD    PB4 (PWM0_1) REV
  // Right motor controlled PF2 (PWM1_3)FWD    PA6 (PWM1_1) REV
	PWM0_1_Update_Duty_Cycle (0); // needs to hold logic 0
	PWM1_1_Update_Duty_Cycle (0);	// needs to hold logic 0
	
	// Calculated value based on Sensor input
	PWM0_0_Update_Duty_Cycle (500); 
	PWM1_3_Update_Duty_Cycle (500); // 
}



void Move_REV (int Speed)
{

	PWM0_0_Update_Duty_Cycle (0); // needs to hold logic 1
	PWM1_3_Update_Duty_Cycle (0);	// needs to hold logic 1
	
	// Calculated value based on Sensor input
	PWM0_1_Update_Duty_Cycle (62500 * ( Speed *.1)); 
	PWM1_1_Update_Duty_Cycle (62500 * ( Speed *.1)); // 
	
}

void BREAK (void)
{

	PWM0_0_Update_Duty_Cycle (0); // needs to hold logic 0
	PWM0_1_Update_Duty_Cycle (0);	// needs to hold logic 0
	PWM1_1_Update_Duty_Cycle (0); // needs to hold logic 0
	PWM1_3_Update_Duty_Cycle (0); // needs to hold logic 0
}