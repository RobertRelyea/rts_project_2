#include "tim4.h"

void timer4_init()
{
	
	
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	
	
	
	GPIOB-> MODER &= ~(0x03 << (2*6));  //clearing out 0 and 1 bit for pb6  
	GPIOB-> MODER |= 0x02 << (2*6);     //selecting AF mode for PB6
	
	GPIOB-> MODER &= ~(0x03 << (2*7));  //clearing out 0 and 1 bit for pb7
	GPIOB-> MODER |= 0x02 << (2*7);      //selecting AF mode for PB7
	
	GPIOB->AFR[0] &= ~(0x0F << (4*6));  // clearing afr for timer 4 PB6
	GPIOB->AFR[0] |= (0x02 << (4*6));   //setting AF2 for PB6
	
	GPIOB->AFR[0] &= ~(0x0F << (4*7));  // clearing afr for timer 4 PB7
	GPIOB->AFR[0] |= (0x02 << (4*7));   //setting AF2 for PB7
	
	
	// Enable TIM4 in the APB1 clock enable register 1
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;   
	
	
	TIM4->PSC = 8000;                 // Set prescaler value for TIM4
	TIM4->EGR |= TIM_EGR_UG;
	
	TIM4->CCER &= ~(TIM_CCER_CC1E);     // Clear output enable for capture input channel 1
  TIM4->CCER &=	~(TIM_CCER_CC2E);     // Clear output enable for capture input channel 2
	
	TIM4->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;   //Select PWM as mode 1 for PB6 CH1
	TIM4->CCMR1 &= ~(TIM_CCMR1_OC1M_0);
	TIM4->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;   //Select PWM as mode 1 for PB7 CH2
	TIM4->CCMR1 &= ~(TIM_CCMR1_OC2M_0);
	
	
	TIM4->CCMR1 |= TIM_CCMR1_OC1PE;  //enable the corresponding preload register by setting the OC1PE bit in the TIM4_CCMR1 register
	TIM4->CCMR1 |= TIM_CCMR1_OC2PE;  //enable the corresponding preload register by setting the OC2PE bit in the TIM4_CCMR1 register
	
	TIM4->CR1 |= TIM_CR1_ARPE;       //auto-reload preload register
	
	TIM4->CCER |= TIM_CCER_CC1E;    // Turn on output enable for capture input channel 1
	TIM4->CCER |= TIM_CCER_CC2E;    // Turn on output enable for capture input channel 2
	
	TIM4->CCER &= ~(TIM_CCER_CC1P);  //set to active high
	TIM4->CCER &= ~(TIM_CCER_CC2P);  //set to active high
	
	TIM4->ARR = 199;                 // Upcounting from 0 to 199
	TIM4->EGR |= TIM_EGR_UG;         //set UG bit in EGR register for force update
	
	
	
	
}


  void set_duty_CH1(int duty_cycle_1)
	{ 
	  
		TIM4->CCR1 = duty_cycle_1;               //setting counter compare value for channel 1
	}
		
	void set_duty_CH2(int duty_cycle_2)
	{
		TIM4->CCR2 = duty_cycle_2;                //setting counter compare value for channel 2
	}
	
	
	
	void timer4_start()
	{
		TIM4->CR1 |= TIM_CR1_CEN;
	}

  void timer4_stop()
 {
	 TIM4->CR1 &= ~(0X1);
	
 }
	
  uint32_t timer4_count()
 {
	return TIM4->CNT; 
 }
	
 uint32_t timer4_event()
 {
	return (TIM4->SR & 0x4);
 }
 
 uint32_t get_timer4_elapsed(uint32_t start_time)
 {
	// Get current timer count
	uint32_t current_time = timer4_count();
	// Check if the timer has wrapped around
	if (current_time < start_time)
		current_time += 200;
 
	// Return time elapsed between start and current times.
	return current_time - start_time;
 }