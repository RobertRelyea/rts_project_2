#ifndef TIM2_H
#define TIM2_H

#include "stm32l476xx.h"
#include "pa6_config.h"

// Sets up the TIM3 registers
void timer3_init(void);

void timer3_start(void);

void timer3_stop(void);

uint32_t timer3_count(void);

uint32_t timer3_capture(void);

uint32_t timer3_event(void);


#endif
