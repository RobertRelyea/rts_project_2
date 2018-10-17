#ifndef SERVO_RECIPE_H
#define SERVO_RECIPE_H

#include <stdlib.h>


// Define all of the commands that are valid
#define MOV 				(0x20)
#define WAIT 				(0x40)
#define LOOP				(0x80)
#define END_LOOP 		(0xA0)
#define RECIPE_END 	(0)

// Examples of simple recipes
// Note that, because the bottom 5 bits are zeros adding or bitwise or'ing
// in the values for the bottom 5 bits are equivalent. However, using a bitwise
// or is better at communicating your purpose.
unsigned char recipe1[] = { MOV + 3, MOV | 5, RECIPE_END } ;
unsigned char recipe2[] = { MOV | 5, MOV | 2, RECIPE_END } ;

// If you set up an array like this then you can easily switch recipes
// using an additional user input command.
unsigned char *recipes[] = { recipe1, recipe2, NULL } ;

// This is a good way to define the status of the display.
// This should be in a header (.h) file.
enum status 
{
	status_running,
	status_paused,
	status_command_error,
	status_nested_error 
} ;

// This is a good way to define the state of a servo motor.
// This should be in a header (.h) file.
enum servo_states
{
	state_at_position,		// use a separate integer to record the current position (0 through 5) for each servo.
	state_unknown,
	state_moving,
	state_recipe_ended
};

// This is a good way to define the event transitions between states.
// This also should be in a header (.h) file.
// More events are needed.
enum events
{
	user_entered_left,
	recipe_ended
};

// Define a "global" state value that is only accessible in one .c module (static makes it "private").
// Define the initial state as paused.
static enum servo_states current_servo_state = state_unknown;


#endif