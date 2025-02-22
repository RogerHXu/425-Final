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
 *  - IR5 (PA6)
 *
 * The sensor works as follows: 
 * black is active at LOW level and white is active at HIGH.
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
	
	// Configure the PA6, PA5, PA4, PA3, and PA2 pins as input
	// by clearing Bits 6 to 2 in the DIR register
	GPIOA->DIR &= ~0x7C;
	
	// Configure the PA6, PA5, PA4, PA3, and PA2 pins to function as
	// GPIO pins by clearing Bits 6 to 2 in the AFSEL register
	GPIOA->AFSEL &= ~0x7C;
	
	// Enable the digital functionality for the PA6, PA5, PA4, PA3, and PA2 pins
	// by setting Bits 6 to 2 in the DEN register
	GPIOA->DEN |= 0x7C;
	
	// Enable the weak pull-up resistor for the PA6, PA5, PA4, PA3, and PA2 pins
	// by setting Bits 6 to 2 in the PUR register
	GPIOA->PUR |= 0x7C;
	
	// Configure the PA6,PA5, PA4, PA3, and PA2 pins to detect edges
	// by clearing Bits 6 to 2 in the IS register
	GPIOA->IS &= ~0x7C;
	
	// Allow the GPIOIEV register to handle interrupt generation
	// and determine which edge to check for the PA6, PA5, PA4, PA3, and PA2 pins 
	// by clearing Bits 6 to 2 in the IBE register
	GPIOA->IBE &= ~0x7C;
	
	// Configure the PA6, PA5, PA4, PA3, and PA2 pins to detect
	// Falling edges by clearing Bits 6 to 2 in the IEV register
	// Falling edges on the corresponding pins will trigger interrupts
	GPIOA->IEV &=~ 0x7C;
	
	// Clear any existing interrupt flags on the PA6, PA5, PA4, PA3, and PA2 pins
	// by setting Bits 6 to 2 in the ICR register
	GPIOA->ICR |= 0x7C;
	
	// Allow the interrupts that are generated by the PA6, PA5, PA4, PA3, and PA2 pins
	// to be sent to the interrupt controller by setting
	// Bits 6 to 2 in the IM register
	GPIOA->IM |= 0x7C;
	

	// Clear the INTA field is for PORT A (Bits 7 to 5) of the IPR[0] register (PRI0)
	NVIC->IPR[0] &= ~0x000000E0;
	
	// Set the priority level of the interrupts to 3. 
	//Port A has an Interrupt Request (IRQ) number of 0
	NVIC->IPR[0] |= (3 << 5);
	
	// Enable IRQ 0 for GPIO Port A by setting Bit 0 in the ISER[0] register
	NVIC->ISER[0] |= (1 << 0);
}

uint8_t IR_Sensor_Read(void)
{
	// Declare a local variable to store the status of the IR_Sensor
	// Then, read the DATA register for Port A
	// A "0x7C" bit mask is used to capture only the pins used the IR_Sensor
	uint8_t ir_sensor_state = GPIOA->DATA & 0x7C;
	
	// Return the status of the PMOD BTN module
	return ir_sensor_state;
}

void GPIOA_Handler(void)
{
	// Check if an interrupt has been triggered by any of
	// the following pins: PA6, PA5, PA4, PA3, and PA2
	if (GPIOA->MIS & 0x7C)
	{
		// Execute the user-defined function
		(*IR_Sensor_Task)(IR_Sensor_Read());
		
		// Acknowledge the interrupt from any of the following pins: 
		// PA6, PA5, PA4, PA3, and PA2
		// Then clear by setting (bits 2-6)
		GPIOA->ICR |= 0x7C;
	}
}
