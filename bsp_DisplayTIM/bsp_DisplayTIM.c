#include "./bsp_DisplayTIM/bsp_DisplayTIM.h"

void DISPLAYTIM_Config()
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

    DISPLAYTIM_APBxClockCmd(DISPLAYTIM_Clock, ENABLE);

    TIM_TimeBaseInitStruct.TIM_Period = 2500;//250ms进入一次
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 7199;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(DISPLAYTIM, &TIM_TimeBaseInitStruct);


    TIM_ClearFlag(DISPLAYTIM, TIM_FLAG_Update);
    TIM_ITConfig(DISPLAYTIM, TIM_IT_Update, ENABLE);
    TIM_Cmd(DISPLAYTIM, ENABLE);

    DISPLAYTIM_APBxClockCmd(DISPLAYTIM_Clock, ENABLE);
}




void DISPLAYTIM_NVIC_Config()
{
    NVIC_InitTypeDef NVIC_InitStruct;

    NVIC_InitStruct.NVIC_IRQChannel = DISPLAYTIM_IRQ;
    NVIC_InitStruct.NVIC_IRQChannelCmd =  ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority  = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 4;
    NVIC_Init(&NVIC_InitStruct);
}
/***********************************THE END************************************/