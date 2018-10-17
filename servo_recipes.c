#include "servo_recipes.h"


// Define a "global" state value that is only accessible in one .c module (static makes it "private").
// Define the initial state as paused.
static enum servo_states servo_one_state = state_unknown;

// Code to start the move (adjust PWM) and start the timing delay based on the
// current position.
static void start_move( enum servo_states new_state )
{
	// TBD
}



// This section should be in a separate .c file such as state_machine.c.
// In this code you add code to each case to process the 
/*void process_event( enum events one_event )
{
	switch ( servo_one_state )
	{
		case state_at_position:		// servo is stationary at a known position
			// Left movement requested
			if ( one_event == move_left && servo_position < 5 )
			{
				servo_position++;
				start_move( servo_position );
				current_servo_state = state_moving;		// when the move ends (enough time has elapsed) new state will be state_position_1
			}

			// Right movement requested
			if (one_event == move_right && servo_position > 0)
			{
				servo_position--;
				start_move(servo_position);
				current_servo_state = state_moving;
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

void print_recipe()
{
	int i = 0 ;

	while ( recipes[ i ] )
	{
		printf( "%02x\n", *recipes[ i++ ] ) ;	// printf first hex entry of each recipe

	}
}

*/