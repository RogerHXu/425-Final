/**
 * @file PWM0_0.h
 *
 * @brief Header file for the PWM0_0 driver.
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
 
#include "TM4C123GH6PM.h"
#include "PWM0_0.h"
#include "PWM_Clock.h"
#include "PWM0_1.h"
#include "PWM0_3.h"
#include "PWM1_3.h"



void Move_FWD (int Speed);


void Move_REV (int Speed);


void BREAK (void);
