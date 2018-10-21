#include "stm32l476xx.h"
#include "Drivers/SysClock.h"
#include "Drivers/UART.h"
#include "Drivers/tim4.h"
#include "Drivers/LED.h"
#include "servo_recipes.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
	
	/* TODO: 
	 * Move all pwm to PA1 and PA2 (TIM2_CH2, TIM2_CH3)
	 * Using pwm outputs on port B prevents the LEDs from functioning.
	 *
	 *
	 */

void wait()
{
	uint32_t start_time = timer2_count();
	while(get_timer2_elapsed(start_time) < 100){}
}

void waitFor(uint32_t ms)
{
	for(int i = 0; i < ms / 10; ++i)
		wait();
}

enum events parseInput(char input)
{
	// Handle user input
	if (input == 'C' || input == 'c') // Continue
		return resume;
	else if (input == 'P' || input == 'p') // Pause
		return pause;
	else if (input == 'L' || input == 'l') // Pause
		return move_left;
	else if (input == 'R' || input == 'r') // Pause
		return move_right;
	else if (input == 'B' || input == 'b') // Pause
		return begin;
	return nop;
}

int main(){
	
	System_Clock_Init();
	
	UART2_Init();
	
  timer2_init();
	timer2_start();
	LED_Init();
	Red_LED_Off();
	Green_LED_Off();

	
	initServos();
	// Place new command indicator
	newLine(USART2);
	putLine(USART2, ">");
	
	// Space to store user input chars
	uint8_t user_input[2];
	uint8_t input_idx = 0;
	
	while(1)
	{
		
		uint32_t start_time = timer2_count();
		// Check for user input
		if(USART_Received(USART2))
		{
			if (input_idx < 2) // Append to input command
				user_input[input_idx++] = getChar(USART2);
			
			else // Input command fully constructed, wait for CR
			{
				// Check for carraige return
				if (getChar(USART2) == '\r')
				{
					// Determine events input by user
					enum events event1 = parseInput(user_input[0]);
					enum events event2 = parseInput(user_input[1]);
					processInput(event1, event2);
				}
				// Reset input buffer index
				input_idx = 0;
				
				// Place new command indicator
				newLine(USART2);
				putLine(USART2, ">");
			}
		}
		
		
		
	  switch (servo1.recipe.status)
		{
			case status_running:
				Green_LED_On();
			  Red_LED_Off();
				break;
			case status_paused:
			  Red_LED_Off();
			  Green_LED_Off();
			  break;
			case status_command_error:
			  Red_LED_On();
			  Green_LED_Off();
			  break;
			case status_nested_error:
			  Green_LED_On();
			  Red_LED_On();
			  break;
		}			
		recipeStep();
		
		uint32_t time_elapsed = get_timer2_elapsed(start_time);
		
		waitFor(100 - time_elapsed);
	} 
}

