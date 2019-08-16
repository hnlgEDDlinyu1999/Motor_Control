#ifndef _BSP_PWM_C
#define _BSP_PWM_C

#include "stm32f10x.h"

//#define ADVANCE_TIM                    TIM1
//#define ADVANCE_TIM_APBxClockCmd       RCC_APB2PeriphClockCmd
//#define ADVANCE_TIM_CLK                RCC_APB2Periph_TIM1
void TIM_PWM_GPIO_Congig(void);
void TIM_PWM_Config(void);
void PWM_Enable(void);
void PWM_SetDuty(float duty);
#endif /*_BSP_PWM_C*/