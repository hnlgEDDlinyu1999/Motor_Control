/**
 * @file main.c
 * @brief �������̵�������д�ڸ��ļ���
 * @mainpage �������ļ�
 * @author hnlgEDDLinYu
 * @email 2970658553@qq.com
 * @version 1.0.0
 * @date 2019/8/16
 * @copyright    Copyright (c) hnlgEDDLinYu
 */
#define _MAIN_C
#include "stdio.h"

#include "stm32f10x.h"
#include "./main.h"
#include <stdlib.h>
#include "./delay/delay.h"
#include "./bsp_drawgraph/bsp_drawgraph.h"
#include "./bsp_PulseCap_EXTI/bsp_PulseCap_EXTI.h"
#include "./bsp_basetim/bsp_basetim.h"
#include "./bsp_DisplayTIM/bsp_DisplayTIM.h"
#include "./bsp_pwm/bsp_pwm.h"
#include "./bsp_led/bsp_led.h"
#include "./bsp_printf/bsp_printf.h"
#include "./bsp_key/bsp_key.h"
#include "./bsp_coder/bsp_coder.h"
#include "./bsp_param/bsp_param.h"
#include "./bsp_usart/bsp_usart.h"
#include <string.h>
#include "./bsp_I2c/i2c.h"
#include "./bsp_eeprom/eeprom.h"
#include "./config.h"

/*������Ҫ��ȫ�ֱ���*/
uint8_t mode = 0;          ///<����ģʽ��0��1��������ģʽ
uint8_t i = 0, j = 0, k = 0; ///<forѭ������ĳЩ��Ҫ����ָ���ĳ���ʹ��

float EE_Buff[4];
uint32_t Pulse_num = 0;    ///<�������
volatile float Duty = 0;   ///<ռ�ձ�

_Pid Pid;                   //�½�һ��pid�ṹ��,����ߵ��������ڵ�ǰģʽ�ļ���
_Pid PidV_Buff;             //�½�һ��pid�ṹ��,����߱��������׵���Ӧ��һ��ģʽ�Ĳ���
_Pid PidW_Buff;             //�½�һ��pid�ṹ��,����߱��������׵���Ӧ��һ��ģʽ�Ĳ���
_dissbuff parambuff;        //�½�һ��dissbuff�ṹ�壬��������ṹ���뿴main.h

float Pid_Cal(void);
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
int main(void)
{
	delay_ms(100); //�ϵ�ʱ�ȴ���������
	/*׼�������ģ��I2C*/
    I2CInit();
	/*׼����LEDС������ָʾ����״̬*/
    LED_GPIO_Config();
	/*׼����LCD��ʾ���*/
    LCD_Ready();
	/*��ʼ������ϵ*/
    DrawOrdinate();
	/*׼���ô��ڽ���*/
    USART_Config();
	/*׼�������岶���ж�*/
    PulseCap_EXTI_Config();
	/*׼����PWM���ɶ�ʱ��*/
	TIM_PWM_GPIO_Congig();
    TIM_PWM_Config();
    PWM_Enable();
	/*׼���û�����ʱ��6�����жϽ���PID����*/
    BASIC_TIM_Config();
    BASIC_TIM_NVIC_Config();
    BASE_TIM_APBxClockCmd(BASE_TIM_Clock, ENABLE);
	/*׼���û�����ʱ��7�����жϽ�����Ϣ��ʾ*/
    DISPLAYTIM_Config();
    DISPLAYTIM_NVIC_Config();
	/*׼���ýǶ�ģʽ�µ��������Ƶ�GPIO*/
    Dir_Control_GPIO_Config();
	/*׼���ýǶ�ģʽ�±�������ع���*/
    TIM3_Encoder_GPIO_Config();
    TIM3_Encoder_Config();
	/*�������*/    
	GPIO_SetBits(GPIOA, GPIO_Pin_12);
    PWM_SetDuty(50);
	/*׼���ð���*/
    Key_Init(); 
	/*ȡ�ر��ݵ�����*/
    Data_Fetch();
	/*���»�����������*/
    Param_Update();
	/*��ʾ�����ı���*/
    Param_NameShow();
	/*������ʾˢ��*/
    Param_Refresh();
	
    while(1) {
        Key_Handle();//������������Ҫһֱ����
        if(mode) {
            Dir_Control();//�ڽǶ�ģʽ�£�Ҫʱ�̾��������ת��
        }
        if(status_index) {  //���ڵ���״̬ʱɲ��
            GPIO_ResetBits(GPIOA, GPIO_Pin_11);
            GPIO_ResetBits(GPIOA, GPIO_Pin_12);
        }
        if(recv_flag) {  //���յ�PC��������������д���
            recv_flag = 0;
            my_printf(8, LINE(3), "PC_Cmd:%c:%f", recv_buff[0], my_atof((recv_buff + 2)));
            Pid.SetSpeed = my_atof((recv_buff + 2));
            Param_Refresh();
            PC_CommandHandle();
        }
    }
}
/**
* @brief Pid���㺯��
* @detail ����ʵʱ�������ٶ�ֵ���������������
*
* @param setspeed �ٶȵ�Ŀ��ֵ
* @return Adjust
* @retval ����Adjust���ɵ���ռ�ձ�
*/
float Pid_Cal()     //�������Ӧ����Ӧ������ģʽ����Ϊԭ��һ��
{
    float Adjust = 0;
	if (mode)
    Pid.Err = abs(Pid.SetSpeed - Pid.ActualSpeed);
	else {
		Pid.Err = Pid.SetSpeed - Pid.ActualSpeed;
	}
    Adjust = Pid.Kp * (Pid.Err - Pid.Err1) + Pid.Ki * Pid.Err + Pid.Kd * (Pid.Err - 2 * Pid.Err1 + Pid.Err2);
    Pid.Err2 = Pid.Err1;
    Pid.Err1 = Pid.Err;

    return Adjust;
}
/***********************************THE END************************************/
