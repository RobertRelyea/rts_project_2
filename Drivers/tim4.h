#ifndef TIM4_H
#define TIM4_H

#include "stm32l476xx.h"
#include "pa6_config.h"

// Sets up the TIM4 registers
void timer4_init(void);

void timer4_start(void);

void timer4_stop(void);

uint32_t timer4_count(void);

uint32_t timer4_capture(void);

uint32_t timer4_event(void);

void set_duty_CH1(int duty_cycle_1);

void set_duty_CH2(int duty_cycle_2);

uint32_t get_timer4_elapsed(uint32_t start_time);

 

#endif
