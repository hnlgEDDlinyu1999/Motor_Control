#ifndef _BSP_KEY_H
#define _BSP_KEY_H
#include "stm32f10x.h"
#include "./delay/delay.h"

#define KEYPAD4x4PORT	GPIOB	//����IO�ӿ���
#define KEY1	GPIO_Pin_8	//����IO�ӿ�
#define KEY2	GPIO_Pin_9	//����IO�ӿ�
#define KEY3	GPIO_Pin_10	//����IO�ӿ�
#define KEY4	GPIO_Pin_11	//����IO�ӿ�
#define KEYa	GPIO_Pin_12	//����IO�ӿ�
#define KEYb	GPIO_Pin_13	//����IO�ӿ�
#define KEYc	GPIO_Pin_14	//����IO�ӿ�
#define KEYd	GPIO_Pin_15	//����IO�ӿ�

#define KEYPAD4x4_APBxPeriphClockCmd   RCC_APB2PeriphClockCmd
#define KEYPAD4x4_GPIO_CLK             RCC_APB2Periph_GPIOB

#define KEYPAD4x4_INT_AFIO_CLK         RCC_APB2Periph_AFIO

#define KEYPAD4x4_INT_PORTSOURCE       GPIO_PortSourceGPIOB

#define KEYPAD4x4_INTHandler           EXTI15_10_IRQHandler


extern uint8_t key_int_flag;
extern uint8_t status_index;  ///<����ʱ����������ʱ������
extern uint8_t s;                 //<������¼��ֵ

void KEYPAD4x4_Init(void);//��ʼ��
void KEYPAD4x4_Init_Toggle(void);//����IO������ʽ��ת
uint8_t KEYPAD4x4_Read (void);//�����м���
void KEYPAD4x4_INT_INIT (void);
void Key_Init(void);
void Key_Handle(void);
#endif /*_BSP_KEY_H*/
/***********************************THE END************************************/