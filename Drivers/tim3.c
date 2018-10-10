#include "tim3.h"

void timer3_init()
{
	init_pa6_pwm();

	// Enable TIM3 in the APB1 clock enable register 1
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM3EN; 
	
	TIM3->PSC = 800; // Set prescaler value for TIM3
	TIM3->ARR = 199; // Upcounting from 0 to 199
	TIM3->EGR |= TIM_EGR_UG; 	  // Trigger an update event for TIM3
	
	
	
	// Set up CCMRx
	TIM3->CCMR1 &= ~(0x0FFF);
	TIM3->CCMR1 |= 0x06; // Set PWM mode 1 in the OC1M bit in the CCMR1 register
	TIM3->CCMR1 |= TIM_CCMR1_OC1PE;   //Enable the preload register by setting OC1PE bit 
	TIM3->CR1 |= TIM_CR1_ARPE;       // Auto reload pre-load register
	TIM4->EGR |= TIM_EGR_UG;        //Force update
	TIM4->CCER |= TIM_CCER_CC1E;    //Enable output for channel 1
	
}
 
void timer3_start()
{
	TIM3->CR1 |= 0x1;
}

void timer3_stop()
{
	TIM3->CR1 &= ~(0x1);
}

uint32_t timer3_count()
{
	return TIM3->CNT; // Timer 3
}

uint32_t timer3_event()
{
	return (TIM3->SR & 0x4);
}
