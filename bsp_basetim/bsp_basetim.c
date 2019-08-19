#include "./bsp_basetim/bsp_basetim.h"

void BASIC_TIM_Config()
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

    BASE_TIM_APBxClockCmd(BASE_TIM_Clock, ENABLE);

    TIM_TimeBaseInitStruct.TIM_Period = 2000;      //2ms更新一次
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 71;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(BASE_TIMx,  &TIM_TimeBaseInitStruct);


    TIM_ClearFlag(BASE_TIMx, TIM_FLAG_Update);
    TIM_ITConfig(BASE_TIMx, TIM_IT_Update, ENABLE);
    TIM_Cmd(BASE_TIMx, ENABLE);

    BASE_TIM_APBxClockCmd(BASE_TIM_Clock, ENABLE);
}




void BASIC_TIM_NVIC_Config()
{
    NVIC_InitTypeDef NVIC_InitStruct;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    NVIC_InitStruct.NVIC_IRQChannel = BASIC_TIM_IRQ;
    NVIC_InitStruct.NVIC_IRQChannelCmd =  ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority  = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
    NVIC_Init(&NVIC_InitStruct);

}