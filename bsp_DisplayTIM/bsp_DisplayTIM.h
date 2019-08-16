#ifndef _BSP_DISPLATYTIM_H
#define _BSP_DISPLATYTIM_H

#include "stm32f10x.h"

#define DISPLAYTIM                     TIM7
#define DISPLAYTIM_APBxClockCmd        RCC_APB1PeriphClockCmd
#define DISPLAYTIM_Clock               RCC_APB1Periph_TIM7

#define DISPLAYTIM_IRQ                 TIM7_IRQn
#define DISPLAYTIM_IRQHandler          TIM7_IRQHandler

void DISPLAYTIM_Config (void);
void DISPLAYTIM_NVIC_Config (void);
#endif /*_BSP_DISPLATYTIM_H*/