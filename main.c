#include "stm32l476xx.h"
#include "Drivers/SysClock.h"
#include "Drivers/UART.h"
#include "Drivers/tim4.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


int main(){
	System_Clock_Init();
	UART2_Init();
	//timer5_init();
  timer4_init();

}

