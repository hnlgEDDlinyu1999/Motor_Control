#ifndef _BSP_BASETIM_H
#define _BSP_BASETIM_H

#include "stm32f10x.h"

#define BASE_TIMx                      TIM6
#define BASE_TIM_APBxClockCmd          RCC_APB1PeriphClockCmd
#define BASE_TIM_Clock                 RCC_APB1Periph_TIM6

#define BASIC_TIM_IRQ                  TIM6_IRQn
#define BASIC_TIM_IRQHandler           TIM6_IRQHandler




void BASIC_TIM_Config (void);
void BASIC_TIM_NVIC_Config (void);
#endif /*_BSP_BASETIM_H*/
/***********************************THE END************************************/