#ifndef _BSP_KEY_H
#define _BSP_KEY_H
#include "stm32f10x.h"
#include "./delay/delay.h"

#define KEYPAD4x4PORT	GPIOB	//定义IO接口组
#define KEY1	GPIO_Pin_8	//定义IO接口
#define KEY2	GPIO_Pin_9	//定义IO接口
#define KEY3	GPIO_Pin_10	//定义IO接口
#define KEY4	GPIO_Pin_11	//定义IO接口
#define KEYa	GPIO_Pin_12	//定义IO接口
#define KEYb	GPIO_Pin_13	//定义IO接口
#define KEYc	GPIO_Pin_14	//定义IO接口
#define KEYd	GPIO_Pin_15	//定义IO接口

#define KEYPAD4x4_APBxPeriphClockCmd   RCC_APB2PeriphClockCmd
#define KEYPAD4x4_GPIO_CLK             RCC_APB2Periph_GPIOB

#define KEYPAD4x4_INT_AFIO_CLK         RCC_APB2Periph_AFIO

#define KEYPAD4x4_INT_PORTSOURCE       GPIO_PortSourceGPIOB

#define KEYPAD4x4_INTHandler           EXTI15_10_IRQHandler


extern uint8_t key_int_flag;
extern uint8_t status_index;  ///<调参时与正常运行时的索引
extern uint8_t s;                 //<用来记录键值

void KEYPAD4x4_Init(void);//初始化
void KEYPAD4x4_Init_Toggle(void);//用于IO工作方式反转
uint8_t KEYPAD4x4_Read (void);//读阵列键盘
void KEYPAD4x4_INT_INIT (void);
void Key_Init(void);
void Key_Handle(void);
#endif /*_BSP_KEY_H*/
/***********************************THE END************************************/