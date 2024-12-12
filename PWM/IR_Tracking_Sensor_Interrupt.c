/**
 * @file IR_Tracking_Sensor_Interrupt.h
 *
 * @brief Header file for the IR_Tracking_Sensor_Interrupt driver.
 *
 * This file contains the function definitions for the IR_Sensor_Interrupt driver.
 * It interfaces with the IR Tracking Sensor module. The following pins are used:
 * 	- IR1 (PA2)
 *	- IR2 (PA3)
 *	- IR3 (PA4)
 *	- IR4 (PA5)
 *  - IR5 (PA7)
 *
 * The sensor works as follows: 
 * black is active at LOW level (zero) and white is active at HIGH (one) .
 * It configures the pins to trigger interrupts on rising edges. 
 * Each individual IR sensor operates in an active high configuration.
 *
 * @author Lenny Marron
 */
 
#include "IR_Tracking_Sensor_Interrupt.h"
 
// Declare pointer to the user-defined task
void (*IR_Sensor_Task)(uint8_t ir_sensor_state);

void IR_Sensor_Interrupt_Init(void(*task)(uint8_t))
{
	// Store the user-defined task function for use during interrupt handling
	IR_Sensor_Task = task;
	
	// Enable the clock to Port A by setting the
	// R0 bit (Bit 0) in the RCGCGPIO register
	SYSCTL->RCGCGPIO |= 0x01;
	
	// Configure the PA5, PA4, PA3, PA2, and PA7 pins as input
	// by clearing Bits 5 to 1 in the DIR register
	GPIOA->DIR &= ~0xBC;  // 7C
	
	// Configure the PA5, PA4, PA3, PA2, and PA1 pins to function as
	// GPIO pins by clearing Bits 5 to 1 in the AFSEL register
	GPIOA->AFSEL &= ~0xBC;
	
	// Enable the digital functionality for the PA5, PA4, PA3, PA2, and PA1 pins
	// by setting Bits 5 to 1 in the DEN register
	GPIOA->DEN |= 0xBC;
	
	// Enable the weak pull-up resistor for the PA5, PA4, PA3, PA2, and PA1 pins
	// by setting Bits 5 to 1 in the PUR register
	GPIOA->PUR |= 0xBC;
	
	// Configure the PA5, PA4, PA3, PA2, and PA1 pins to detect edges
	// by clearing Bits 5 to 1 in the IS register
	GPIOA->IS &= ~0xBC;
	
	// Allow the GPIOIEV register to handle interrupt generation
	// and determine which edge to check for the PA5, PA4, PA3, PA2, and PA1 pins 
	// by clearing Bits 5 to 1 in the IBE register
	GPIOA->IBE &= ~0xBC;
	
	// Configure the PA5, PA4, PA3, PA2, and PA1 pins to detect
	// Falling edges by clearing Bits 5 to 1 in the IEV register
	// Falling edges on the corresponding pins will trigger interrupts
	GPIOA->IEV &=~ 0xBC;
	
	// Clear any existing interrupt flags on the PA5, PA4, PA3, PA2, and PA1 pins
	// by setting Bits 5 to 1 in the ICR register
	GPIOA->ICR |= 0xBC;
	
	// Allow the interrupts that are generated by the PA5, PA4, PA3, PA2, and PA1 pins
	// to be sent to the interrupt controller by setting
	// Bits 5 to 1 in the IM register
	GPIOA->IM |= 0xBC;
	
	// Clear the INTA field is for PORT A (Bits 7 to 5) of the IPR[0] register (PRI0)
	NVIC->IPR[0] &= ~0x000000E0;
	
	// Set the priority level of the interrupts to 3. 
	//Port A has an Interrupt Request (IRQ) number of 0
	NVIC->IPR[0] |= (3 << 5);
		
	// Enable IRQ 0 for GPIO Port A by setting Bit 0 in the ISER[0] register
	// Bit 0 is from Interrupts Table 2-9 Port  = Int # 0 (Bit in Int Register)
	NVIC->ISER[0] |= (1 << 0);
}

uint8_t IR_Sensor_Read(void)
{
	// Declare a local variable to store the status of the IR_Sensor
	// Then, read the DATA register for Port A
	// A "0x7C" bit mask is used to capture only the pins used the IR_Sensor
	uint8_t ir_sensor_state = GPIOA->DATA & 0xBC;
	
	// Return the status of the PMOD BTN module
	return ir_sensor_state;
}

void GPIOA_Handler(void)
{
	// Check if an interrupt has been triggered by any of
	// the following pins: PA5, PA4, PA3, PA2, and PA1
	if (GPIOA->MIS & 0xBC)
	{
		// Execute the user-defined function
		(*IR_Sensor_Task)(IR_Sensor_Read());
		
		// Acknowledge the interrupt from any of the following pins: 
		// PA5, PA4, PA3, PA2, and PA1
		// Then clear by setting (bits 1-5)
		GPIOA->ICR |= 0xBC;
	}
}
