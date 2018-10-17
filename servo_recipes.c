#include "servo_recipes.h"

// Code to start the move (adjust PWM) and start the timing delay based on the
// current position.
static void start_move( enum servo_states new_state )
{
	// TBD
}
// This section should be in a separate .c file such as state_machine.c.
// In this code you add code to each case to process the 
void process_event( enum events one_event )
{
	/*
	switch ( current_servo_state )
	{
		case state_at_position :		// servo is stationary at a known position
			if ( one_event == user_entered_left && servo_position < 5 ) // prevent moving too far left
			{
				servo_position++ ;
				start_move( servo_position ) ;
				current_servo_state = state_moving ;		// when the move ends (enough time has elapsed) new state will be state_position_1
			}
			break ;
		case state_unknown :
			break ;
		case state_recipe_ended :
			break ;
	}*/
}

