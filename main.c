#include "stm32l476xx.h"
#include "Drivers/SysClock.h"
#include "Drivers/UART.h"
#include "Drivers/tim4.h"
#include "Drivers/LED.h"
#include "servo_recipes.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

uint8_t buffer[BufferSize];
uint8_t user_input[2];
uint8_t input_idx = 0;
uint8_t pos_idx = 0;
	
	/* TODO: 
	 * Move all pwm to PA1 and PA2 (TIM2_CH2, TIM2_CH3)
	 * Using pwm outputs on port B prevents the LEDs from functioning.
	 *
	 *
	 */

void wait()
{
	uint32_t start_time = timer4_count();
	while(get_timer4_elapsed(start_time) < 100){}
}

void waitFor(uint32_t ms)
{
	for(int i = 0; i < ms / 10; ++i)
		wait();
}

int main(){
	
	System_Clock_Init();
	
	UART2_Init();
	
  timer4_init();
	timer4_start();
	
	while(1)
	{
		// Check for user input
		if(USART_Received(USART2))
		{
			if (input_idx < 2) // Append to input command
				user_input[input_idx++] = getChar(USART2);
			
			else // Input command fully constructed, wait for CR
			{
				if (getChar(USART2) == '\r')
				{
					newLine(USART2);
					// Reset input buffer index
					input_idx = 0;
					// Handle user input
					if (user_input[0] == 'C' || user_input[0] == 'c') // Continue
						process_event(resume);
					else if (user_input[0] == 'P' || user_input[0] == 'p') // Pause
						process_event(pause);
					else if (user_input[0] == 'L' || user_input[0] == 'l') // Pause
						process_event(move_left);
					else if (user_input[0] == 'R' || user_input[0] == 'r') // Pause
						process_event(move_right);
					else if (user_input[0] == 'B' || user_input[0] == 'b') // Pause
						process_event(begin);
				}
			}
		}
		
		/*
		set_duty_CH1(servo1_positions[pos_idx]);
		set_duty_CH2(servo2_positions[pos_idx++]);
		if(pos_idx > 5)
			pos_idx = 0;
		*/
		
		recipe1Step();
		
		waitFor(100);
	} 
}
