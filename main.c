#include "stm32l476xx.h"
#include "Drivers/SysClock.h"
#include "Drivers/UART.h"
#include "Drivers/tim4.h"
#include "Drivers/recipe.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#define MOV (0x20)
#define WAIT (0x40)
#define LOOP (0x80)
#define END_LOOP (0xA0)
#define RECIPE_END (0)

unsigned char recipe1[] = { MOV + 0, MOV | 5, MOV + 0,MOV + 3, LOOP +0, MOV + 1, MOV + 4, END_LOOP, MOV + 0, MOV + 2, WAIT +0, MOV + 3, WAIT +0, MOV + 2, MOV + 3, WAIT +31, WAIT +31, WAIT +31, MOV +4} ;
unsigned char recipe2[] = { MOV + 0, MOV | 5, MOV + 0,MOV + 3, LOOP +0, MOV + 1, MOV + 4, END_LOOP, MOV + 0, MOV + 2, WAIT +0, MOV + 3, WAIT +0, MOV + 2, MOV + 3, WAIT +31, WAIT +31, WAIT +31, MOV +4} ;
uint8_t buffer[1024];

int main(){
	System_Clock_Init();
	UART2_Init();
  //timer5_init();
  timer4_init();
	Timer2_Init();
	while(1)
	{
		sprintf((char *)buffer, "FAIZAL IS CHUTIYAPA\r\n");
	  USART_Write(USART2,(uint8_t *)buffer, strlen((char *)buffer));
		delay(1);
	}
	/*int i=0;
	while (i<=20)
	{
		//int i =0;
		unsigned char a = recipe1[i];
		opcode (a, 0);
		unsigned char b = recipe2[i];
		opcode (b, 1);
		i=i+1;
	}*/

}

