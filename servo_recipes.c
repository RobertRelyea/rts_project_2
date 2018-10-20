#include "servo_recipes.h"
#include "Drivers/tim4.h"
#include "Drivers/UART.h"

// Examples of simple recipes
// Note that, because the bottom 5 bits are zeros adding or bitwise or'ing
// in the values for the bottom 5 bits are equivalent. However, using a bitwise
// or is better at communicating your purpose.
unsigned char recipe1[] = {  
	LOOP+3,
	MOV+3, 
	MOV+5, 
	WAIT+4,
	END_LOOP,
	MOV+4,
	WAIT+3,
	MOV+1, 
	RECIPE_END };

unsigned char recipe2[] = { MOV | 5, MOV | 2, RECIPE_END } ;

// If you set up an array like this then you can easily switch recipes
// using an additional user input command.
unsigned char *recipes[] = { recipe1, recipe2, NULL } ;

int servo1_positions[] = {5, 6, 10, 13, 17, 21};
int servo2_positions[] = {5, 6, 10, 13, 17, 21};
int servo1_position = 0;
int servo2_position = 0;

// Indexes for each recipe
int r1_idx = 0;
int r2_idx = 0;

// Wait variables
unsigned char r1_wait = 0;
unsigned char r2_wait = 0;

// Loop variables
unsigned char r1_loop = 0;
unsigned char r1_loop_iter = 0;
unsigned char r2_loop = 0;
unsigned char r2_loop_iter = 0;

enum status r1_status = status_running;
enum status r2_status = status_running;

// Define a "global" state value that is only accessible in one .c module (static makes it "private").
// Define the initial state as paused.
static enum servo_states servo1_state = state_unknown;

// Code to start the move (adjust PWM) and start the timing delay based on the
// current position.
static void startMove1( enum servo_states new_state, unsigned char position )
{
	servo1_state = new_state;
	set_duty_CH1(servo1_positions[position]);
}


void recipe1Step()
{	
	if (r1_status != status_running)
		return;
	
	// Grab the current recipe step
	unsigned char step = recipe1[r1_idx];
	
	// Extract the command
	unsigned char command = (step & 0xE0); 
	// Extract the param
	unsigned char param = (step & 0x1F);
	
	switch(command)
	{
		case MOV:
			putLine(USART2, "Moving");
			if(param > NUM_SERVO_POS - 1)
			{
				// Out of range error
				r1_status = status_command_error;
			}
			else if (servo1_state != state_moving)
			{
				// Move servo
				startMove1(state_moving, param);
			}
			else
			{
				servo1_state = state_at_position;
				r1_idx++;
			}
			break;
			
		case WAIT:
			putLine(USART2, "Waiting");
			if (r1_wait > 1) // WAIT has commenced
				r1_wait--;
			else if (r1_wait == 1) // WAIT will complete
			{
				r1_wait--;
				r1_idx++;
			}
			else // New WAIT command
				r1_wait = param;
			break;
			
		case LOOP:
			putLine(USART2, "Loopin");
			// Check for nested loop
			if (r1_loop != 0)
			{
				// Nested loop error
				r1_status = status_nested_error;
			}
			else
			{
				// Set loop flag, loop block index and loop iter count
				r1_loop = r1_idx + 1;
				r1_loop_iter = param;
			}
			r1_idx++;
			break;
		
		case END_LOOP:
			putLine(USART2, "End loopin");
			if (r1_loop_iter > 0) // 
			{
				r1_loop_iter--;
				r1_idx = r1_loop;
			}
			if (r1_loop_iter <=0) // This will catch the end
			{	
				r1_loop = 0;
				r1_idx++;
			}
			break;
		
		case RECIPE_END:
			putLine(USART2, "Ending");
			servo1_state = state_recipe_ended;
			break;	
	}
}





// This section should be in a separate .c file such as state_machine.c.
// In this code you add code to each case to process the 

void process_event( enum events one_event )
{
	if (one_event == pause)
	{
		r1_status = status_paused;
	}
	
	if (one_event == resume)
	{
		r1_status = status_running;
	}
	
	// User servo position commands are valid only when paused
	if (r1_status == status_paused)
	{
		switch ( servo1_state )
		{
			case state_at_position:		// servo is stationary at a known position
				// Left movement requested
				if ( one_event == move_left && servo1_position < 6 )
				{
					servo1_position++;
					startMove1(state_moving, servo1_position );
				}

				// Right movement requested
				if (one_event == move_right && servo1_position > 0)
				{
					servo1_position--;
					startMove1(state_moving, servo1_position );
				}
				break;
			case state_moving: // Servo is moving, wait for some time
					break;
			case state_unknown :
				break;
			case state_recipe_ended :
				break;
		}
	}	
}