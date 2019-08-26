#ifndef _BSP_CODER_H
#define _BSP_CODER_H

#include "stm32f10x.h"

#define ENCODER_TIM                TIM3

#define ENCDODER_GPIO              GPIOA
#define ENCDODER_GPIO_PINA         GPIO_Pin_6
#define ENCDODER_GPIO_PINB         GPIO_Pin_7

#define TIM3_ECODER_PeriphClockCmd RCC_APB1PeriphClockCmd
#define ENCODER_TIM_Clk            RCC_APB1Periph_TIM3

extern float Angle;

void TIM3_Encoder_GPIO_Config(void);
void TIM3_Encoder_Config(void);



void Dir_Control(void);
void Dir_Control_GPIO_Config(void);
#endif /*_BSP_CODER_H*/