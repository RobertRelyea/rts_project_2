#ifndef SERVO_RECIPE_H
#define SERVO_RECIPE_H

#include <stdlib.h>
#include "Drivers/tim4.h"
#include "Drivers/UART.h"

extern int servo1_positions[6];
extern int servo2_positions[6];




// Define all of the commands that are valid
#define MOV         (0x20)
#define WAIT        (0x40)
#define LOOP        (0x80)
#define END_LOOP    (0xA0)
#define SKIP        (0x30)
#define CUST_TWO    (0xC0) // TODO
#define RECIPE_END 	(0)

#define NUM_SERVO_POS (6)

// Examples of simple recipes
// Note that, because the bottom 5 bits are zeros adding or bitwise or'ing
// in the values for the bottom 5 bits are equivalent. However, using a bitwise
// or is better at communicating your purpose.
extern unsigned char recipe1[];
extern unsigned char recipe2[];

// If you set up an array like this then you can easily switch recipes
// using an additional user input command.
extern unsigned char *recipes[];

// This is a good way to define the status of the display.
// This should be in a header (.h) file.
enum recipe_status 
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
	move_left,
	move_right,
	pause,
	resume,
	begin,
	recipe_ended
};

// Struct for managing recipe information
// r_idx 				->  Index of current recipe step
// r_move				->  Number of steps required to complete current MOV
// r_wait 			->  Number of steps required to complete current WAIT
// r_loop				->  Starting step of current loop
// r_loop_iter	->  Number of iterations left in current LOOP
// r_status     ->  Current status of recipe
typedef struct recipe_t
{
	int idx;
	int move;
	int wait;
	int loop;
	int loop_iter;
	enum recipe_status status;
	unsigned char *recipe;
}recipe_type;

// Struct for managining servo and recipe status
// servo_positions -> Measured positions for servo
// servo_position  -> Current position of servo
// state 					 -> Current state of servo
// recipe					 -> Recipe struct associated with servo
typedef struct servo_t
{
	int *positions;
	int position;
	enum servo_states state;
	recipe_type recipe;
	int channel;
}servo_type;

extern servo_type servo1;
extern servo_type servo2;

void recipeStep();
servo_type recipeStepHelper();
void processInput(enum events one_event, enum events two_event);
servo_type processEvent( enum events one_event, servo_type servo);
void initServos();

#endif
