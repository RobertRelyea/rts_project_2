#include "tim4.h"

void timer2_init()
{
	
	
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	
	
	GPIOA-> MODER &= ~(0x03 << (2*1));  //clearing out 0 and 1 bit for PA1 
	GPIOA-> MODER |= 0x02 << (2*1);     //selecting AF mode for PA1
	
	GPIOA-> MODER &= ~(0x03 << (2*2));  //clearing out 0 and 1 bit for PA2
	GPIOA-> MODER |= 0x02 << (2*2);      //selecting AF mode for PA2
	
	GPIOA->AFR[0] &= ~(0x0F << (4*1));  // clearing afr for timer 2 PA1
	GPIOA->AFR[0] |= (0x01 << (4*1));   //setting AF1 for PA1
	
	GPIOA->AFR[0] &= ~(0x0F << (4*2));  // clearing afr for timer 2 PA2
	GPIOA->AFR[0] |= (0x01 << (4*2));   //setting AF1 for PA2
	
	
	// Enable TIM2 in the APB1 clock enable register 1
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;   
	
	
	TIM2->PSC = 8000;                 // Set prescaler value for TIM2
	TIM2->EGR |= TIM_EGR_UG;
	
	TIM2->CCER &= ~(TIM_CCER_CC2E);     // Clear output enable for capture input channel 2
  TIM2->CCER &=	~(TIM_CCER_CC3E);     // Clear output enable for capture input channel 3
	
	TIM2->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;   //Select PWM as mode 1 for PB6 CH1
	TIM2->CCMR1 &= ~(TIM_CCMR1_OC2M_0);
	TIM2->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2;   //Select PWM as mode 1 for PB7 CH2
	TIM2->CCMR2 &= ~(TIM_CCMR2_OC3M_0);
	
	
	TIM2->CCMR1 |= TIM_CCMR1_OC2PE;  //enable the corresponding preload register by setting the OC1PE bit in the TIM4_CCMR1 register
	TIM2->CCMR2 |= TIM_CCMR2_OC3PE;  //enable the corresponding preload register by setting the OC2PE bit in the TIM4_CCMR1 register
	
	TIM2->CR1 |= TIM_CR1_ARPE;       //auto-reload preload register
	
	TIM2->CCER |= TIM_CCER_CC2E;    // Turn on output enable for capture input channel 1
	TIM2->CCER |= TIM_CCER_CC3E;    // Turn on output enable for capture input channel 2
	
	TIM2->CCER &= ~(TIM_CCER_CC2P);  //set to active high
	TIM2->CCER &= ~(TIM_CCER_CC3P);  //set to active high
	
	TIM2->ARR = 199;                 // Upcounting from 0 to 199
	//TIM2->CCR2 = 50;
	//TIM2->CCR3 = 100;
	TIM2->EGR |= TIM_EGR_UG;         //set UG bit in EGR register for force update
}


void setDuty(int channel, int duty_cycle)
{ 
	switch(channel)
	{
		case 1: // Channel 1
			TIM2->CCR1 = duty_cycle;
			break;
		case 2: // Channel 2
			TIM2->CCR2 = duty_cycle;
			break;
		case 3: // Channel 3
			TIM2->CCR3 = duty_cycle;
			break;
		case 4: // Channel 4
			TIM2->CCR4 = duty_cycle;
			break;
	}		
}


void timer2_start()
{
	TIM2->CR1 |= TIM_CR1_CEN;
}

void timer2_stop()
{
  TIM2->CR1 &= ~(0X1);
}

uint32_t timer2_count()
{
	return TIM2->CNT; 
}

uint32_t timer2_event()
{
	return (TIM2->SR & 0x4);
}

uint32_t get_timer2_elapsed(uint32_t start_time)
{
	// Get current timer count
	uint32_t current_time = timer2_count();
	// Check if the timer has wrapped around
	if (current_time < start_time)
		current_time += 200;

	// Return time elapsed between start and current times.
	return current_time - start_time;
}