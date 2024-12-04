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
 *	-	Two DC Motors Gearboxes 
 *				- Left motor controlled  PB6 (PWM0_0) PB7 (PWM0_1)
 *				- Right motor controlled PD0 (PWM0_3) PD1 (PWM1_3)
 *				
 *			
 * For PC communication using:
 *  PA0 = UART0 RX
 *  PA1 = UART0 TX
 *
 * The US-100 Ultrasonic Distance Sensor uses the following pinout:
 *  - US-100 Pin 1 (VCC)  <-->  Tiva LaunchPad 3.3V
 *  - US-100 Pin 2 (TX)   <-->  Tiva LaunchPad Pin PB1 (U1TX)
 *  - US-100 Pin 3 (RX)   <-->  Tiva LaunchPad Pin PB0 (U1RX)
 *  - US-100 Pin 4 (GND)  <-->  Tiva LaunchPad GND
 *  - US-100 Pin 5 (GND)  <-->  Tiva LaunchPad GND
 *
 * It uses Timer 0A to generate periodic interrupts every 1 ms. 
 *
 *
 * @author Lenny Marron
 */
 
 
#include "TM4C123GH6PM.h"
#include "SysTick_Delay.h" 
#include "PWM_Clock.h"
#include "PWM0_0.h"
#include "PWM0_1.h"
#include "PWM1_3.h"
#include "UART0.h"
#include "UART1.h"
 
#include "Timer_0A_Interrupt.h"
#include "IR_Tracking_Sensor_Interrupt.h"

#define BUFFER_SIZE   64
#define READ_DISTANCE 0x55

void Timer_0A_Periodic_Task (void);
extern void IR_Sensor_Handler (uint8_t ir_sensor_status);
static uint32_t Timer_0A_ms_elapsed= 0;
//static uint16_t RGB_LED_duty_cycle = 0;
static uint8_t increment_duty_cycle = 1;



int main(void)
{
	//Used to Initialize Systick Timer blocking delay functions
	SysTick_Delay_Init();
	
	//PWM clock divisor used to update clock to 3.125 MHz
	PWM_Clock_Init();
	
	
	// Period_Constant = 62500    Duty Cycle = (62500 * 5%)= 3125 
	PWM0_1_Init(62500,3125);
	
		// Period_Constant = 62500    Duty Cycle = (62500 * 5%)= 3125 
  PWM0_0_Init(62500,3125);
	
	
	IR_Sensor_Interrupt_Init(&IR_Sensor_Handler);
	
		// Initialize an array to store the measured distance values from the US-100 Ultrasonic Distance Sensor
	char US_100_UART_Buffer[BUFFER_SIZE] = {0};

	// Initialize the UART0 module which will be used to print characters on the serial terminal
	UART0_Init();
	
	// Initialize the UART1 module which will be used to communicate with the US-100 Ultrasonic Distance Sensor
	UART1_Init();
	
	//Timer_0A_Interrupt_Init (&Timer_0A_Periodic_Task);
	
	while(1)
	{						
		
		UART1_Output_Character(READ_DISTANCE);
		
		US_100_UART_Buffer[0] = UART1_Input_Character();
		US_100_UART_Buffer[1] = UART1_Input_Character();
		
		uint16_t distance_value = (US_100_UART_Buffer[1] | (US_100_UART_Buffer[0] << 8));
		
		UART0_Output_String("Distance (mm): ");
		UART0_Output_Unsigned_Decimal(distance_value);
		UART0_Output_Newline();
		
		SysTick_Delay1ms(100);
	}
}
		
		/*
		
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
		*/
	


// will decide where to shift the robot based off of IR input
void IR_Sensor_Handler (uint8_t ir_sensor_status)
{   //62,500 = 100% duty
  switch (ir_sensor_status)
    { 
			//All Black Continue straight!!!!
	    case 0x00:
	    { 
	     PWM0_0_Update_Duty_Cycle (62500 * .2); //20%
			 PWM0_1_Update_Duty_Cycle (62500 * .2);	//20%
		   break;
	    }
			//Not Reading Black STOP both motors!!!!
	    case 0x7C:
	    { 
	     PWM0_0_Update_Duty_Cycle (0);
			 PWM0_1_Update_Duty_Cycle (0);	
		   break;
	    }
			//IR1 (PA2) increase left side by %15 
	    case 0x78:
				
	    { 
	     PWM0_0_Update_Duty_Cycle (62500 * .35); //35%
			 PWM0_1_Update_Duty_Cycle (62500 * .2);	//20%
		   break;
	    }
			
			//IR1 (PA2) + IR2 (PA3) seeing black 
			// increase right side by % 10
	    case 0x70:
	    { 
	     PWM0_0_Update_Duty_Cycle (62500 * .3); //30%
			 PWM0_1_Update_Duty_Cycle (62500 * .2); //20%
		   break;
	    }
	
	    
			//IR1 (PA2) + IR2 (PA3) + IR3 (PA4) seeing black 
			// increase right side by % 
	    case 0x60:
	    {
	     PWM0_0_Update_Duty_Cycle (47187);
		   break;
	    }
	
	   	//IR1 (PA2) + IR2 (PA3) + IR3 (PA4) + IR4 (PA5) seeing black 
			// increase right side by % 
	    case 0x40:
	    {
			PWM0_0_Update_Duty_Cycle (7187);
			break;
	    }
	
	   //IR1 (PA2) + IR2 (PA3) + IR3 (PA4) + IR4 (PA5) seeing black 
		 //increase right side by % 
	    case 0x20:
	    {
				PWM0_0_Update_Duty_Cycle (17187);
	    	break;
	    }
			
			 //IR5 (PA6) increase left side by 10%
	    case 0x42:
	    {
				PWM0_0_Update_Duty_Cycle (57187);
	    	break;
	    }
	
	default:
	{
	break;
	}
 }
}



void Timer_0A_periodic_Task (void)
{
	Timer_0A_ms_elapsed ++;
  if ((Timer_0A_ms_elapsed %5 ) ==0)
	{IR_Sensor_Handler (increment_duty_cycle);
  }
}
