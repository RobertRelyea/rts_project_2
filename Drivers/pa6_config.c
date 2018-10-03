#include "pa6_config.h"


// Turn on the peripheral clock for GPIOA
// and set PA3 to be in Alternate Function 
// mode for use with TIM3_CH1 for PWM.
void init_pa6_pwm( void )
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN ;		// enable clock for A group of GPIO
	GPIOA->MODER &= ~0xC; // clear out bits 0 and 1 for PA6
	
	// Enable alternate function mode (binary 10) for PA6
	GPIOA->MODER |= 0x2000;
	GPIOA->AFR[0] &= ~0xF000000;	// Clear PA6 field in AFRL
	GPIOA->AFR[0] |= 0x2000000;		// Sets AF2 as the alternate function for PA6
														    // TIM3_CH1
}