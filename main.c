#include "stm32l476xx.h"
#include "Drivers/SysClock.h"
#include "Drivers/UART.h"
#include "Drivers/tim2.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


int main(void)
{
	int dc = 4;
	System_Clock_Init(); // Switch System Clock = 80 MHz
	UART2_Init();
	timer2_pwm();
	timer_start();
	dutycycle(dc);
	
	while(1)
	{}
  
	
	
}

