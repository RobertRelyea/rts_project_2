#include "servo_recipes.h"

// Examples of simple recipes
// Note that, because the bottom 5 bits are zeros adding or bitwise or'ing
// in the values for the bottom 5 bits are equivalent. However, using a bitwise
// or is better at communicating your purpose.
unsigned char recipe1[] = {  
	LOOP+3,
	MOV+3, 
	MOV+5, 
//	WAIT+4,
	END_LOOP,
	MOV+4,
	WAIT+3,
	MOV+1, 
	RECIPE_END };

unsigned char recipe2[] = { MOV | 5, MOV | 2, RECIPE_END } ;

int servo1_positions[] = {5, 6, 10, 13, 17, 21};
int servo2_positions[] = {5, 6, 10, 13, 17, 21};

// Declare servo structs
servo_type servo1;
servo_type servo2;
recipe_type recipe_t1;
recipe_type recipe_t2;

void initServos()
{
	// Set servo positions
	servo1.positions = servo1_positions;
	servo2.positions = servo2_positions;
	
	// Set current servo position
	servo1.position = 0;
	servo2.position = 0;
	
	// Set current servo status
	servo1.state = state_unknown;
  servo2.state = state_unknown;
	
	// Set servo channels
	servo1.channel = 1;
	servo2.channel = 2;
	
	// Initialize servo recipes
	recipe_t1.idx = 0;
	recipe_t1.move = 0;
	recipe_t1.wait = 0;
	recipe_t1.loop = 0;
	recipe_t1.loop_iter = 0;
	recipe_t1.status = status_paused;
	recipe_t1.recipe = recipe1;
	servo1.recipe = recipe_t1;
	
	recipe_t2.idx = 0;
	recipe_t2.move = 0;
	recipe_t2.wait = 0;
	recipe_t2.loop = 0;
	recipe_t2.loop_iter = 0;
	recipe_t2.status = status_paused;
	recipe_t2.recipe = recipe2;
	servo2.recipe = recipe_t2;
}

// Code to start the move (adjust PWM) and start the timing delay based on the
// current position.
static servo_type startMove( enum servo_states new_state, servo_type servo, unsigned char position )
{
	servo.state = new_state;
	servo.recipe.move = (servo.position > position) ? 
		(servo.position - position) : (position - servo.position);
	servo.position = position;
	setDuty(servo.channel, servo.positions[position]);
	return servo;
}


void recipeStep()
{	
	servo1 = recipeStepHelper(servo1);
	servo2 = recipeStepHelper(servo2);
}

servo_type recipeStepHelper(servo_type servo)
{
	if (servo.recipe.status != status_running)
	{
		servo.state = state_at_position;
		return servo;
	}
	
	// Grab the current recipe step
	unsigned char step = servo.recipe.recipe[servo.recipe.idx];
	
	// Extract the command
	unsigned char command = (step & 0xE0); 
	// Extract the param
	unsigned char param = (step & 0x1F);
	
	switch(command)
	{
		case MOV:
			//putLine(USART2, "Moving");
			if(param > NUM_SERVO_POS - 1)
			{
				// Out of range error
				servo.recipe.status = status_command_error;
			}
			else if (servo.state != state_moving)
			{
				// Move servo
				servo = startMove(state_moving, servo, param);
			}
			else if (servo.recipe.move > 0) // Still moving...
				servo.recipe.move--;
			else
			{
				servo.state = state_at_position;
				servo.recipe.idx++;
			}
			break;
			
		case WAIT:
			//putLine(USART2, "Waiting");
			if (servo.recipe.wait > 1) // WAIT has commenced
				servo.recipe.wait--;
			else if (servo.recipe.wait == 1) // WAIT will complete
			{
				servo.recipe.wait--;
				servo.recipe.idx++;
			}
			else // New WAIT command
				servo.recipe.wait = param;
			break;
			
		case LOOP:
			//putLine(USART2, "Loopin");
			// Check for nested loop
			if (servo.recipe.loop != 0)
			{
				// Nested loop error
				servo.recipe.status = status_nested_error;
			}
			else
			{
				// Set loop flag, loop block index and loop iter count
				servo.recipe.loop = servo.recipe.idx + 1;
				servo.recipe.loop_iter = param;
			}
			servo.recipe.idx++;
			break;
		
		case END_LOOP:
			//putLine(USART2, "End loopin");
			if (servo.recipe.loop_iter > 0) // 
			{
				servo.recipe.loop_iter--;
				servo.recipe.idx = servo.recipe.loop;
			}
			if (servo.recipe.loop_iter <=0) // This will catch the end
			{	
				servo.recipe.loop = 0;
				servo.recipe.idx++;
			}
			break;
		
		case RECIPE_END:
			//putLine(USART2, "Ending");
			servo.state = state_recipe_ended;
			break;	
	}
	return servo;
}

void processInput(enum events one_event, enum events two_event)
{
	servo1 = processEvent(one_event, servo1);
	servo2 = processEvent(two_event, servo2);
}

servo_type processEvent( enum events event, servo_type servo)
{
	if (event == begin)
	{
		servo.recipe.idx = 0;
		servo.recipe.loop = 0;
		servo.recipe.loop_iter = 0;
		servo.recipe.wait = 0;
		servo.recipe.status = status_running;
	}
	
	if (event == pause)
	{
		servo.recipe.status = status_paused;
	}
	
	if (event == resume)
	{
		servo.recipe.status = status_running;
	}
	
	// User servo position commands are valid only when paused
	if (servo.recipe.status == status_paused)
	{
		switch ( servo.state )
		{
			case state_at_position:		// servo is stationary at a known position
				// Left movement requested
				if ( event == move_left && servo.position < 5 )
					servo = startMove(state_moving, servo, servo.position + 1 );

				// Right movement requested
				if (event == move_right && servo.position > 0)
					servo = startMove(state_moving, servo, servo.position - 1);
				break;
			case state_unknown :
				break;
			case state_recipe_ended :
				break;
		}
	}	
	return servo;
}