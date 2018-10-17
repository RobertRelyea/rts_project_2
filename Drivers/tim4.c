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
	
	
	TIM4->PSC = 0x1F3F;                 // Set prescaler value for TIM4
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
	
	TIM4->ARR = 200;                 // Upcounting from 0 to 199
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