#ifndef TIM4_H
#define TIM4_H

#include "stm32l476xx.h"
#include "pa6_config.h"

// Sets up the TIM2 registers
void timer2_init(void);

void timer2_start(void);

void timer2_stop(void);

uint32_t timer2_count(void);

uint32_t timer2_capture(void);

uint32_t timer2_event(void);

void setDuty(int channel, int duty_cycle);

uint32_t get_timer2_elapsed(uint32_t start_time);

 

#endif
