/**
 * @file UART1.c
 *
 * @brief Source code for the UART1 driver.
 *
 * This file contains the function definitions for the UART1 driver.
 *
 * @note For more information regarding the UART module, refer to the
 * Universal Asynchronous Receivers / Transmitters (UARTs) section
 * of the TM4C123GH6PM Microcontroller Datasheet.
 * Link: https://www.ti.com/lit/gpn/TM4C123GH6PM
 *
 * @note Assumes that the system clock (50 MHz) is used.
 *
 * PB0  = UART RX
 * PB1  = UART TX
 *
 * @author Aaron Nanas
 */

#include "UART1.h"

void UART1_Init(void)
{
	// Enable the clock to UART1 by setting the 
	// R1 bit (Bit 1) in the RCGCUART register
	SYSCTL->RCGCUART |= 0x02;
	
	// Enable the clock to Port B by setting the
	// R10 bit (Bit 1) in the RCGCGPIO register
	SYSCTL->RCGCGPIO |= 0x02;
	
	// Disable the UART1 module before configuration by clearing
	// the UARTEN bit (Bit 0) in the CTL register
	UART1->CTL &= ~0x01;
	
	// Set the baud rate by writing to the DIVINT field (Bits 15 to 0)
	// and the DIVFRAC field (Bits 15 to 0) in the IBRD and FBRD registers, respectively.
	// The integer part of the calculated constant will be written to the IBRD register,
	// while the fractional part will be written to the FBRD register.
	// N = (System Clock Frequency) / (16 * Baud Rate)
	// N = (50,000,000) / (16 * 9600) = 325.5208333 (N = 325)
	// F = ((0.5208333 * 64) + 0.5) = 33.8333312 (F = 33)
	UART1->IBRD = 325;
	UART1->FBRD = 33;
	
	// Configure the data word length of the UART packet to be 8 bits by 
	// writing a value of 0x3 to the WLEN field (Bits 6 to 5) in the LCRH register
	UART1->LCRH |= 0x60;
	
	// Enable the transmit and receive FIFOs by setting the FEN bit (Bit 4) in the LCRH register
	UART1->LCRH |= 0x10;
	
	// Select one stop bit to be transmitted at the end of a UART frame by
	// clearing the STP2 bit (Bit 3) in the LCRH register
	UART1->LCRH &= ~0x08;
	
	// Disable the parity bit by clearing the PEN bit (Bit 1) in the LCRH register
	UART1->LCRH &= ~0x02;
	
	// Enable the UART1 module after configuration by setting
	// the UARTEN bit (Bit 0) in the CTL register
	UART1->CTL |= 0x01;
	
	// Configure the PB1 (U1TX) and PB0 (U1RX) pins to use the alternate function
	// by setting Bits 1 to 0 in the AFSEL register
	GPIOB->AFSEL |= 0x03;
	
	// Clear the PMC1 (Bits 7 to 4) and PMC0 (Bits 3 to 0) fields in the PCTL register before configuration
	GPIOB->PCTL &= ~0x000000FF;
	
	// Configure the PB1 pin to operate as a U1TX pin by writing 0x1 to the
	// PMC1 field (Bits 7 to 4) in the PCTL register
	// The 0x1 value is derived from Table 23-5 in the TM4C123G Microcontroller Datasheet
	GPIOB->PCTL |= 0x00000010;
	
	// Configure the PB0 pin to operate as a U1RX pin by writing 0x1 to the
	// PMC0 field (Bits 3 to 0) in the PCTL register
	// The 0x1 value is derived from Table 23-5 in the TM4C123G Microcontroller Datasheet
	GPIOB->PCTL |= 0x00000001;
	
	// Enable the digital functionality for the PB1 and PB0 pins
	// by setting Bits 1 to 0 in the DEN register
	GPIOB->DEN |= 0x03;
}

char UART1_Input_Character(void)
{
	while((UART1->FR & UART1_RECEIVE_FIFO_EMPTY_BIT_MASK) != 0);
	
	return (char)(UART1->DR & 0xFF);
}

void UART1_Output_Character(char data)
{
	while((UART1->FR & UART1_TRANSMIT_FIFO_FULL_BIT_MASK) != 0);
	UART1->DR = data;
}

uint32_t UART1_Input_String(char *buffer_pointer, uint16_t buffer_size) 
{
	int length = 0;
	uint32_t string_size = 0;
	
	// Read the last received data from the UART Receive Buffer
	char character = UART1_Input_Character();
	
	// Check if the received character is a carriage return. Otherwise,
	// for each valid character, increment the string_size variable which will
	// indicate how many characters have been detected from the input string
	while(character != UART1_CR)
	{
		// Remove the character from the buffer is the received character is a backspace character
		if (character == UART1_BS)
		{
			if (length)
			{
				buffer_pointer--;
				length--;
				UART1_Output_Character(UART1_BS);
			}
		}
		
		// Otherwise, if there are more characters to be read, store them in the buffer
		else if (length < buffer_size)
		{
			*buffer_pointer = character;
			buffer_pointer++;
			length++;
			string_size++;
		}
		character = UART1_Input_Character();
	}
	*buffer_pointer = 0;
	
	return string_size;
}

void UART1_Output_String(char *pt)
{
	while(*pt)
	{
		UART1_Output_Character(*pt);
		pt++;
	}
}