/**
 * @file Motor_CTL.h
 *
 * @brief Header file for the DRV8833 breakout board driver
 *
 * This file contains the function definitions for the DRV8833 driver.
 * PWM signals with a DRV8833 breackout board	& Two DC GearMotors 
 *				- Left motor controlled  PB6 (PWM0_0)FWD    PB4 (PWM0_1) REV
 *				- Right motor controlled PF2 (PWM1_3)FWD    PA6 (PWM1_1) REV
 *
 * @note This driver assumes that the PWM_Clock_Init, PWM0_0_Init, PWM0_1_Init, PWM1_1_Init, and PWM1_3_Init 
 * functions have been called
 * 
 *
 * @author Lenny Marron
 */

 
#include "TM4C123GH6PM.h"
#include "PWM_Clock.h"
#include "PWM0_0.h"
#include "PWM0_1.h"
#include "PWM1_1.h"
#include "PWM1_3.h"


/**
 * @brief  Adjusts PWM signals to allow FWD drive direction. 
 *
 * @param  PB6 (PWM0_0) PF2 pin (M1PWM6) are set to the minimum forward drive speed
 *				 While PB4 (PWM0_1) and PA6 (PWM1_1) are set to logic level LOW.
 *
 * @return None
 */
void Move_FWD (float power);



/**
 * @brief  Adjusts PWM signals to allow Right drive direction. 
 *
 * @param  Stops right motor and continues with Left Motor speed to turn right.
 *				 PB6 (PWM0_0) is set to the minimum forward drive speed
 *				 PF2 pin (M1PWM6) PB4 (PWM0_1) and PA6 (PWM1_1) are set to logic level LOW.
 *
 * @return None
 */
void Move_Right (float power);

/**
 * @brief  Adjusts PWM signals to allow Left drive direction. 
 *
 * @param  Stops Left motor and continues with Right Motor speed to turn Left.
 *				 PF2 pin (PWM1_3) is set to the minimum forward drive speed
 *				 PB6 (PWM0_0)PB4 (PWM0_1) and PA6 (PWM1_1) are set to logic level LOW.
 *
 * @return None
*/
void Move_Left (float power);


/**
 * @brief  Adjusts PWM signals to allow REV drive direction. 
 *
 * @param  PB4 (PWM0_1) and PA6 (PWM1_1) are set to the minimum forward drive speed
 *				 While PB6 (PWM0_0) PF2 pin (M1PWM6) are set to logic level LOW.
 *
 * @return None
 */
void Move_REV (float power);

/**
 * @brief  Adjusts PWM signals to allow motors to stop. 
 *
 * @param  All PWM signals are set to are set to logic level LOW.
 *					 PB4 (PWM0_1)  PA6 (PWM1_1) PB6 (PWM0_0) PF2 pin (M1PWM6) 
 *
 * @return None
 */
void BREAK (void);
