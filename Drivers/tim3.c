#include "tim3.h"

void timer3_init()
{
	init_pa6_pwm();

	// Enable TIM3 in the APB1 clock enable register 1
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM3EN; 
	
	TIM3->PSC = 800;    // Set prescaler value for TIM3
	TIM3->EGR |= TIM_EGR_UG; 	  // Trigger an update event for TIM3
	
	TIM3->CCER &= ~(TIM_CCER_CC2E); // Disable channel 2 output
	TIM3->CCER &= 0xA0; // Set CC2NP and CC2P to rising edge trigger
	
	// Set up CCMRx
	TIM3->CCMR1 &= ~(0xF000);
	TIM3->CCMR1 |= 0x0100; // Set CC2 channel as input, IC2 mapped on TI2
	TIM3->CCER |= TIM_CCER_CC2E; // Turn on output enable for capture input
}

void timer3_start()
{
	TIM3->CR1 |= 0x1;
}

void timer3_stop()
{
	TIM3->CR1 |= 0x0;
}

uint32_t timer3_count()
{
	return TIM3->CNT; // Timer 3
}

uint32_t timer3_event()
{
	return (TIM3->SR & 0x4);
}
