/*
 * @file main.c
 *
 * @brief Main source code for the Pathfinder Robot.
 *
 * This file contains the main entry point and function definitions for the Pathfinder Robot.
 * It Generates 4 PWM signals, a 1ms Timer interrupt, 9600 UART1 communication, SysTickTimer .
 *
 * Timer 0A generates periodic interrupts every 1 ms in order to count the 
 * amount of time needed to turn trigger the US-100 sensor and check obstacles in front of it. 
 *
 *
 * It interfaces with the following:
 *  - User LED (RGB) Tiva C Series TM4C123G LaunchPad
 *	- DRV8833 DC Motor Driver
 *	- SG90 Micro Servo Motor
 *	-	Two DC Motors Gearboxes 
 *				- Left motor controlled  PB6 (PWM0_0)FWD    PB4 (PWM0_1) REV
 *				- Right motor controlled PF2 (PWM1_3)FWD    PA6 (PWM1_1) REV
 *
 *
 *	DRV8833 Breakout Board
 *  - DRV8833   (AN1)  <-->  Tiva LaunchPad PB6
 *  - DRV8833   (AN2)  <-->  Tiva LaunchPad PB4
 *  - DRV8833   (SLP)  <-->  +3.3V (Active High)
 *  - DRV8833   (BN2)  <-->  Tiva LaunchPad PA6
 *  - DRV8833   (BN1)  <-->  Tiva LaunchPad PF2
 *
 *  - DRV8833   (AOut1)<-->  DC Motor (+)
 *  - DRV8833   (AOut2)<-->  DC Motor (-)
 *  - DRV8833   (BOut2)<-->  DC Motor (-)
 *  - DRV8833   (BOut1)<-->  DC Motor (+)
 *
 *  - DRV8833   (VM+)  <-->  +6V (Battery Pack)
 *  - DRV8833   (VM-)  <-->  GND (Battery Pack)
 *
 * UART0 was used during the debugging
 * For PC communication using:
 *  - PA0 = UART0 RX
 *  - PA1 = UART0 TX
 *
 * The US-100 Ultrasonic Distance Sensor uses the following pinout:
 *  - US-100 Pin 1 (VCC)  <-->  Tiva LaunchPad 3.3V
 *  - US-100 Pin 2 (TX)   <-->  Tiva LaunchPad Pin PB1 (U1TX)
 *  - US-100 Pin 3 (RX)   <-->  Tiva LaunchPad Pin PB0 (U1RX)
 *  - US-100 Pin 4 (GND)  <-->  Tiva LaunchPad GND
 *  - US-100 Pin 5 (GND)  <-->  Tiva LaunchPad GND
 *
 *
 * @author Lenny Marron
 */
 

#include "TM4C123GH6PM.h"
#include "SysTick_Delay.h" 
#include "PWM_Clock.h"
#include "PWM0_0.h"
#include "PWM0_1.h"
#include "PWM1_1.h"
#include "PWM1_3.h"
#include "Motor_CTL.h"
#include "UART0.h"
#include "UART1.h"
#include "Timer_0A_Interrupt.h"
#include "IR_Tracking_Sensor_Interrupt.h"

#define BUFFER_SIZE   64
#define READ_DISTANCE 0x55

void Timer_0A_periodic_Task (void);
void IR_Sensor_Handler (uint8_t ir_sensor_status); //extern
static uint32_t Timer_0A_ms_elapsed= 0;
int Space  = 0;


int Distance_cm (void)
{
	char US_100_UART_Buffer[BUFFER_SIZE] = {0};
	
		UART1_Output_Character(READ_DISTANCE);
		
		US_100_UART_Buffer[0] = UART1_Input_Character();  //buffer aka read
		US_100_UART_Buffer[1] = UART1_Input_Character();
		
		uint16_t distance_value = ((US_100_UART_Buffer[1] | (US_100_UART_Buffer[0] << 8))/10);

	return distance_value ;
}



int main(void)
{
	//Used to Initialize Systick Timer blocking delay functions
	   SysTick_Delay_Init();
	
	// PWM clock divisor used to update clock to 3.125 MHz
	   PWM_Clock_Init();
	
	// Initializes PB and sets Period_Constant = 62500  
	   PWM0_0_Init(62500,0);  // works PB6
	
	// Initializes PB and sets Period_Constant = 62500  
     PWM0_1_Init(62500,0);  // works PB4
	
	// Initializes PB and sets Period_Constant = 62500   
     PWM1_1_Init(62500,0); // works PA6
	
	// Initializes PB and sets Period_Constant = 62500  
	   PWM1_3_Init(62500,0); // works PF2
	
	// Initialize the IR Channel Interrupts (Port A)
	   IR_Sensor_Interrupt_Init(&IR_Sensor_Handler); // working

	// Initialize the UART0 module which will be used to print characters on the serial terminal
	// UART0_Init();
	
	// Initialize the UART1 module which will be used to communicate with the US-100 Ultrasonic Distance Sensor
	   UART1_Init();
	
	// Initializes the Timer A0 Interrupts 
	   Timer_0A_Interrupt_Init (&Timer_0A_periodic_Task); //working
	
	while(1)
	{						
	   //Distance_cm();
	}
}


// Timer will check distance every 1 seconds
void Timer_0A_periodic_Task (void)
{
	Timer_0A_ms_elapsed ++;

  if ((Timer_0A_ms_elapsed % 1000 ) == 0)
	{
	 Space = Distance_cm(); // measure distance infront
		 if (Space < 10)
		 { 
		//	 Move_REV ( .3 );
		
	     Move_Right (.3);  
		
		 }
		 else
		   Move_Left ( .3 );     
	 }	 
	 else
	   Move_FWD ( .3 );
}



// will decide where to shift the robot based off of IR input
void IR_Sensor_Handler (uint8_t ir_sensor_status)
{   //62,500 = 100% duty
  switch (ir_sensor_status)
    { 
			//Not Reading Black REverse motors!!!
	    case 0xBC:
	    { 
				Move_REV (.4);		
				break;
	    }
			
			//IR5 (PA7) change left side to %80
	    case 0x3C:
	    { 
			 BREAK ();

			 PWM0_0_Update_Duty_Cycle (62500 * .5); //	Left motor 80%
			 PWM0_1_Update_Duty_Cycle (62500 * .2);	// Right motor 20%		
		   break;
	    }
			
			//IR5 (PA7) + IR4 (PA5) change left side to 70
			// increase left side by % 10
	    case 0x1C:
	    {
			 BREAK ();
				
			 PWM0_0_Update_Duty_Cycle (62500 * .5);  // Left motor 50%
			 PWM0_1_Update_Duty_Cycle (62500 * .3);	// Right motor 30%			
			break;
	    }
			
			//IR1 (PA2) + IR2 (PA3) seeing black 
			// increase right side by % 10
	    case 0xB0:
	    { 
	     BREAK ();
				
			 PWM0_0_Update_Duty_Cycle (62500 * .3);  // Left motor 30%
			 PWM0_1_Update_Duty_Cycle (62500 * .5);	// Right motor 50%
		   break;
	    }
			
			//IR1 (PA2) seeing black 
			// increase right side by % 15
	    case 0xB8:
	    {
	    BREAK ();

		  PWM0_0_Update_Duty_Cycle (62500 * .5); //	Left motor 80%
		  PWM0_1_Update_Duty_Cycle (62500 * .2);	// Right motor 20%
		
		  break;
	    }
	default:
	{
	// Do Nothing
	break;
	}
 }
}
		






