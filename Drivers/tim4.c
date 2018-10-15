#include "tim4.h"

void timer4_init()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	//GPIOB-> MODER &= ~(0x300);  
	GPIOB-> MODER &= ~(0x03 << (2*6));         //clearing out 0 and 1 bit for pa0	
	
	//GPIOB-> MODER |= 0x01 << (2*6);
	GPIOB-> MODER |= 0x02 << (2*6);
	GPIOB->AFR[0] &= ~(0x0F << (4*6));
	GPIOB->AFR[0] |= (0x02 << (4*6));
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN; 
	TIM4->PSC = 0x1F3F; 
	TIM4->EGR |= TIM_EGR_UG;
	TIM4->CCER &= ~(TIM_CCER_CC1E);
	TIM4->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
	TIM4->CCMR1 &= ~(TIM_CCMR1_OC1M_0);
	TIM4->CCMR1 |= TIM_CCMR1_OC1PE;
	TIM4->CR1 |= TIM_CR1_ARPE;
	TIM4->CCER |= TIM_CCER_CC1E;
	TIM4->CCER &= ~(TIM_CCER_CC1P);
	TIM4->ARR = 200;
	TIM4->EGR |= TIM_EGR_UG;
	TIM4->CCR1 = 150;
	TIM4->CR1 |= TIM_CR1_CEN;
}

/*
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
*/