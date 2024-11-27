/**
 * @file IR_Tracking_Sensor_Interrupt.h
 *
 * @brief Header file for the IR_Tracking_Sensor_Interrupt driver.
 *
 * This file contains the function definitions for the IR_Sensor_Interrupt driver.
 * It interfaces with the IR_Tracking_Sensor. The following pins are used:
 * 	- IR1 (PA2)
 *	- IR2 (PA3)
 *	- IR3 (PA4)
 *	- IR4 (PA5)
 *  - IR5 (PA6)
 *
 * It configures the pins to trigger interrupts on rising edges. The IR Sensor
 * push buttons operate in an active high configuration.
 *
 * @author Aaron Nanas
 */

#include "TM4C123GH6PM.h"

// Declare pointer to the user-defined task
extern void (*IR_Sensor_Task)(uint8_t ir_sensor_state);

/**
 * @brief Initializes interrupts for the PMOD BTN module using Port A.
 *
 * This function initializes interrupts for the PMOD BTN module
 * connected to the following pins:
 * 	- IR1 (PA2)
 *	- IR2 (PA3)
 *	- IR3 (PA4)
 *	- IR4 (PA5)
 *  - IR5 (PA6)
 *
 * It configures the specified pins to trigger interrupts on rising edges.
 * When an interrupt occurs, the provided task function is executed with the current button status.
 * Interrupt priority is set to 3 for GPIO Port A.
 *
 * @param task A pointer to the user-defined function to be executed upon button interrupts.
 *
 * @return None
 */
void IR_Sensor_Interrupt_Init(void(*task)(uint8_t));

/**
 * @brief Reads the current status of the PMOD BTN module.
 *
 * This function reads the current status of the PMOD BTN module connected to Port A.
 * It returns the button status as an 8-bit unsigned integer, where each bit represents
 * the state of a specific button.
 *
 * @return An 8-bit unsigned integer representing the status of the PMOD BTN module.
 */
uint8_t IR_Sensor_Read(void);

/**
 * @brief The interrupt service routine (ISR) for GPIO Port A.
 *
 * This function is the interrupt service routine (ISR) GPIO Port A.
 * It checks if an interrupt has been triggered by the PA5, PA4, PA3, or PA2 pins,
 * and if so, it executes the user-defined task function with the current button status.
 * After executing the task function, it acknowledges and clears the interrupt.
 *
 * @param None
 *
 * @return None
 */
void GPIOA_Handler(void);
