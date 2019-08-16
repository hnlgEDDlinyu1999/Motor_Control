/**
 * @file main.c
 * @brief �������̵�������д�ڸ��ļ���
 * @mainpage �������ļ�
 * @author LinYu
 * @email 2970658553@qq.com
 * @version 1.0.0
 * @date 2019/8/16
 * @copyright    Copyright (c) LinYu  
 */
#define _MAIN_C
#include "stdio.h"
#include "stm32f10x.h"
#include "./main.h"
#include "./bsp_drawgraph/bsp_drawgraph.h"
#include "./bsp_PulseCap_EXTI/bsp_PulseCap_EXTI.h"
#include "./bsp_basetim/bsp_basetim.h"
#include "./bsp_DisplayTIM/bsp_DisplayTIM.h"
#include "./bsp_pwm/bsp_pwm.h"
#include "./bsp_led/bsp_led.h"
#include "./bsp_printf/bsp_printf.h"


float V = 0;               ///<ʵ�����ٶ�
float SetV = 50;           ///<Ŀ���ٶ�
uint32_t Pulse_num = 0;    ///<�������
float Duty = 0;            ///<ռ�ձ�


uint16_t x_1=30,x_2=0,y_1=0,y_2=0,backup=300;///<���Ʋ�����Ҫ�õ��ı���


_Pid Pid;                  //�½�һ��pid�ṹ��
_dissbuff dissbuff;        //�½�һ��dissbuff�ṹ��
/**
* @brief Pid�����ʼ������
* @detail ���ڳ�ʼ��Pid��������Ĳ���
* 
* @param None
* @return None
*/
void Pid_Init(void)
{	
	Pid.SetSpeed = 0;
	Pid.ActualSpeed = 0;
	Pid.Err = 0;
	Pid.Err1 =0;
	Pid.Err2 =0;
	Pid.Kp = 0.004;
	Pid.Ki = 0.1;
	Pid.Kd = 0.05;
}
/**
* @brief Pid���㺯��
* @detail ����ʵʱ�������ٶ�ֵ���������������
* 
* @param setspeed �ٶȵ�Ŀ��ֵ
* @return Adjust
* @retval ����Adjust���ɵ���ռ�ձ�
*/
float Pid_Cal(float setspeed)
{
	float Adjust = 0;
	
	Pid.ActualSpeed = V;
	Pid.SetSpeed = setspeed;
	Pid.Err = Pid.SetSpeed - Pid.ActualSpeed;
	
	Adjust = Pid.Kp*(Pid.Err-Pid.Err1) + Pid.Ki*Pid.Err + Pid.Kd*(Pid.Err-2*Pid.Err1+Pid.Err2);
	
	Pid.Err2 = Pid.Err1;
	Pid.Err1 = Pid.Err;
	
	return Adjust;
}

/**
* @brief ������
* @details ����Ψһ���
*
* @param argc �����������
* @param argv �������ָ������
* @return ����ִ�гɹ����
*     @retval 0 ����ִ�гɹ�
*     @retval 1 ����ִ��ʧ��
* @note Null
*/
int main (void)
{
	/*׼����LEDС������ָʾ����״̬*/
	LED_GPIO_Config();
	/*׼����LCD��ʾ���*/
	LCD_Ready();
	DrawOrdinate();
	
	/*׼�������岶���ж�*/
	PulseCap_EXTI_Config();
	
	/*׼����PWM���ɶ�ʱ��*/
	TIM_PWM_GPIO_Congig();
	TIM_PWM_Config();
	PWM_Enable();
	
	/*׼���û�����ʱ��6�����жϽ���PID����*/
	BASIC_TIM_Config ();
	BASIC_TIM_NVIC_Config ();
	BASE_TIM_APBxClockCmd(BASE_TIM_Clock,ENABLE);
	/*׼���û�����ʱ��7�����жϽ�����Ϣ��ʾ*/
	DISPLAYTIM_Config ();
	DISPLAYTIM_NVIC_Config ();
	/*�������*/
	PWM_SetDuty(100);
	/*Pid���ٳ�ʼ��*/
	Pid_Init();

    while (1);
}


 