#include "stm32l476xx.h"
#include "Drivers/SysClock.h"
#include "Drivers/UART.h"
#include "Drivers/tim4.h"
#include "Drivers/LED.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

uint8_t buffer[BufferSize];
	
	
int main(){
	
	System_Clock_Init();
	
	UART2_Init();
	
  timer4_init();
	
	set_duty_CH1(100);
	
	set_duty_CH2(50);
	
	timer4_start();
	
	LED_Init();
	Red_LED_On();
	while(1)
	{
	uint32_t count = timer4_count();
  sprintf((char *)buffer, "Timer count is %d\r\n", count);
	USART_Write(USART2,(uint8_t *)buffer, strlen((char *)buffer));
	}
	
	
	
	
  
}

