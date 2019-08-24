/**
 * @file bsp_coder.c
 * @brief 该文件是对定时器编码器模式的配置，由于功能集中且较为独立
 *        故单独形成一个文件
 * @mainpage 编码器模式
 * @author hnlgEDDLinYu
 * @email 2970658553@qq.com
 * @version 1.0.0
 * @date 2019/8/18
 * @copyright    Copyright (c) hnlgEDDLinYu
 */
#include "./bsp_coder/bsp_coder.h"
#include "./main.h"

void TIM3_Encoder_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    TIM3_ECODER_PeriphClockCmd(ENCODER_TIM_Clk, ENABLE);

    GPIO_InitStruct.GPIO_Pin = ENCDODER_GPIO_PINA | ENCDODER_GPIO_PINB;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(ENCDODER_GPIO, &GPIO_InitStruct);
}
void TIM3_Encoder_Config(void)
{
    TIM_TimeBaseInitTypeDef   TIM_TimeBaseStruct;
    TIM_ICInitTypeDef         TIM_ICInitStruct;

    TIM_TimeBaseStruct.TIM_Prescaler = 0;
    TIM_TimeBaseStruct.TIM_Period = 1336;
    TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(ENCODER_TIM, &TIM_TimeBaseStruct);

    TIM_EncoderInterfaceConfig(ENCODER_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM_ICStructInit(&TIM_ICInitStruct);    // 初始化为默认值

    TIM_ICInitStruct.TIM_ICFilter = 0;
    TIM_ICInit(TIM5, &TIM_ICInitStruct);

    TIM3->CNT = 0;
    TIM_Cmd(ENCODER_TIM, ENABLE);
}


void Dir_Control_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA, &GPIO_InitStruct);
}
void Dir_Control(void)       //调电机的转动方向 ，接线：A11-白-IN8 A12-黑-IN7
//倒转 ：IN7低，IN8高
//正转 ：IN7高，IN8低
{
    if(Pid.ActualSpeed > Pid.SetSpeed) {  //超过就倒转，pin11高，pin12低	set高，reset低
        GPIO_SetBits(GPIOA, GPIO_Pin_11);
        GPIO_ResetBits(GPIOA, GPIO_Pin_12);
    } else {                              //否则正转，pin11低，pin12高
        GPIO_SetBits(GPIOA, GPIO_Pin_12);
        GPIO_ResetBits(GPIOA, GPIO_Pin_11);
    }
}
/***********************************THE END************************************/