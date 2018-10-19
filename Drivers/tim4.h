#ifndef TIM4_H
#define TIM4_H

#include "stm32l476xx.h"
#include "pa6_config.h"

// Sets up the TIM3 registers
void timer4_init(void);

void timer3_start(void);

void timer3_stop(void);

uint32_t timer3_count(void);

uint32_t timer3_capture(void);

uint32_t timer3_event(void);
void movs(unsigned char z, int servop);
void delay(int s);
void Timer2_Init(void);
#endif
