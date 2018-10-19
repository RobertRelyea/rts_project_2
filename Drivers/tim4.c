#include "tim4.h"
#include "tim2.h"
#define servo_0 (20)
#define servo_1 (40)
#define servo_2 (60)
#define servo_3 (80)
#define servo_4 (100)
#define servo_5 (120)

int myticks = 0;

uint8_t buffer[1024];
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
	
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;   //Enable clock
	TIM4->PSC = 0x1F3F; 
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
	
	TIM4->ARR = 200; 
	TIM4->EGR |= TIM_EGR_UG;         //set UG bit in EGR register for force update
	
	//TIM4->CCR1 = 150;               //setting counter compare value for channel 1
	//TIM4->CCR2 = 150;                //setting counter compare value for channel 2
	
	TIM4->CR1 |= TIM_CR1_CEN;
	
}


void Timer2_Init()
{
	//Enable Timer 2 through the RCC
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	//RCC->AHB2ENR |=   RCC_AHB2ENR_GPIOAEN; //Enable clk for port A GPIO as well
	
	//Set the prescaler to 7999 (80MHz / 1MHz) to get 0.1 ms per clock cnt
	TIM2->PSC = 0; //0x50 = 80 decimal

	//Set the ARR so that interrupts occur every 100 ms
	TIM2->ARR = 80; 
	
	//Create an update event via the Update Generator bit
	TIM2->CR1 |= TIM_CR1_URS;
	TIM2->DIER |= TIM_DIER_UIE;
	TIM2->EGR |= TIM_EGR_UG;
	
	//TIM2->CR1 |= TIM_CR1_CEN; //Enable counting!

	
	TIM2->EGR |= TIM_EGR_UG; 
	
	NVIC_EnableIRQ(TIM2_IRQn);
}

void TIM2_IRQHandler(void)
{
	myticks ++;
	TIM2->SR &= ~(TIM_SR_UIF);
}

void delay(int s)
{
	TIM2->CR1 |= TIM_CR1_CEN;
	myticks = 0;
	while(myticks<(s*1000000));
	TIM2->CR1 &= ~(TIM_CR1_CEN);
		
	
}





 void servo_position(int servo, int position)
 {
	 if(servo == 0)
	 {
		 if(position == 0)
		 {
			 TIM4->CCR1 = servo_0;
		 }
		 else if (position == 1)
		 {
			 TIM4->CCR1 = servo_1;
		 }
		  else if (position == 2)
		 {
			 TIM4->CCR1 = servo_2;
		 }
		  else if (position == 3)
		 {
			 TIM4->CCR1 = servo_3;
		 }
		  else if (position == 4)
		 {
			 TIM4->CCR1 = servo_4;
		 }
		  else if (position == 5)
		 {
			 TIM4->CCR1 = servo_5;
		 }
	 }
	 else if (servo == 1)
	 {
		 if(position == 0)
		 {
			 TIM4->CCR2 = servo_0;
		 }
		 else if (position == 1)
		 {
			 TIM4->CCR2 = servo_1;
		 }
		  else if (position == 2)
		 {
			 TIM4->CCR2 = servo_2;
		 }
		  else if (position == 3)
		 {
			 TIM4->CCR2 = servo_3;
		 }
		  else if (position == 4)
		 {
			 TIM4->CCR2 = servo_4;
		 }
		  else if (position == 5)
		 {
			 TIM4->CCR2 = servo_5;
		 }
		 
	 }
	 TIM4->EGR |= TIM_EGR_UG;
 }
	
void opcode( unsigned char x , int servo)
{
	 unsigned char y = (x >>5);
	 if (y== 0x01)
	 {
		 sprintf((char *)buffer, "MOV\r\n");
		 USART_Write(USART2,(uint8_t *)buffer, strlen((char *)buffer));
		 movs(x, servo);
	 }
	 else if (y==0x02)
	 {
		  sprintf((char *)buffer, "WAIT\r\n");
		 USART_Write(USART2,(uint8_t *)buffer, strlen((char *)buffer));
		 //wait(&x);
	 }
	 else if (y==0x04)
	 {
		  sprintf((char *)buffer, "LOOP\r\n");
		 USART_Write(USART2,(uint8_t *)buffer, strlen((char *)buffer));
		 //loop(&x);
	 }
	 else if (y==0x05)
	 {
		  sprintf((char *)buffer, "END_LOOP\r\n");
		 USART_Write(USART2,(uint8_t *)buffer, strlen((char *)buffer));
		 //endloop(&x);
	 }
	 else if (y==0x00)
	 {
		  sprintf((char *)buffer, "RECIPE_END\r\n");
		 USART_Write(USART2,(uint8_t *)buffer, strlen((char *)buffer));
		 //recipeend(&x);
	 }
}


void movs(unsigned char z, int servop)
{
	
	if (z==0x20)
	{
	servo_position( servop,0);
	}
	if (z==0x21)
	{
	servo_position( servop,1);
	}
	if (z==0x22)
	{
	servo_position( servop,2);
	}
	if (z==0x23)
	{
	servo_position( servop,3);
	}if (z==0x24)
	{
	servo_position( servop,4);
	}
	if (z==0x25)
	{
	servo_position( servop,5);
	}
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