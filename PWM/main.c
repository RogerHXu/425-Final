/*
 * @file main.c
 *
 * @brief Main source code for the PWM program.
 *
 * This file contains the main entry point and function definitions for the PWM program.
 * It Generates PWM signals using the two PWM modules: M0PWM and M1PWM.
 *
 * It interfaces with the following:
 *  - User LED (RGB) Tiva C Series TM4C123G LaunchPad
 *	- DRV8833 DC Motor Driver
 *	-	DC Motor 200RPM Gearbox   
 *
 * It uses Timer 0A to generate periodic interrupts every 1 ms. 
 * It will update the duty cycle every 5 ms to demonstrate a fading LED effect.
 *
 * @author Lenny Marron
 */
 
 
#include "TM4C123GH6PM.h"
#include "SysTick_Delay.h" 
#include "PWM_Clock.h"
#include "PWM0_1.h"

#include "PWM0_0.h" 
#include "Timer_0A_Interrupt.h"
#include "PMOD_BTN_Interrupt.h"

void Timer_0A_Periodic_Task (void);

void PMOD_BTN_Handler (uint8_t pmod_btn_status);

static uint32_t Timer_0A_ms_elapsed= 0;

static uint16_t RGB_LED_duty_cycle = 0;

static uint8_t increment_duty_cycle = 1;



int main(void)
{
	//Used to Initialize Systick Timer blocking delay functions
	SysTick_Delay_Init();
	
	//PWM clock divisor used to update clock to 3.125 MHz
	PWM_Clock_Init();
	
	
	// Period_Constant = 62500    Duty Cycle = (62500 * 5%)= 3125 
	PWM0_1_Init(62500,3125);
	
	while(1)
	{						
		//TT motor requires min 0.5V Equivalent PWM Voltage  to turn on
		
		
		//20% duty cycle
PWM0_1_Update_Duty_Cycle (12500);
		SysTick_Delay1ms (2000);
		
		//40% duty cycle
PWM0_1_Update_Duty_Cycle (25000);
		SysTick_Delay1ms (2000);
	

		//50% duty cycle
PWM0_1_Update_Duty_Cycle (31250);
		SysTick_Delay1ms (2000);
		
		
		//75% duty cycle
PWM0_1_Update_Duty_Cycle (46875);
		SysTick_Delay1ms (2000);
		
		
	//95% duty cycle
PWM0_1_Update_Duty_Cycle (59375);
		SysTick_Delay1ms (2000);
	}
}

/*// WILL NOT USE FOR DEV

void PMOD_BTN_Handler (uint8_t pmod_btn_status)
{
  switch (pmod_btn_status)
    { //BTN0 (PA2)
	    case 0x04:
	    { //3% -> 600us
	     PWM0_0_Update_Duty_Cycle (1875);
		   break;
	    }
	
	    //BTN1 (PA3)
	    case 0x08:
	    {
	     PWM0_0_Update_Duty_Cycle (7187);
		   break;
	    }
	
	   //BTN2 (PA4)
	    case 0x10:
	    {
			break;
	    }
	
	    //BTN3 (PA5)
	    case 0x20:
	    {
	
	    	break;
	    }
	
	default:
	{
	break;
	}
}

}	
*/

void Timer_0A_periodic_Task (void)
{
	Timer_0A_ms_elapsed ++;
	
  if ((Timer_0A_ms_elapsed %5 ) ==0)
  {
  }
}

