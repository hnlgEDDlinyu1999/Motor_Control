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
#include "./delay/delay.h"
#include "./bsp_key/bsp_key.h"
#include "./bsp_coder/bsp_coder.h"

/*������Ҫ��ȫ�ֱ���*/
uint8_t mode = 1;          ///<����ģʽ��0��1��������ģʽ
uint8_t status_index = 0;  ///<����ʱ����������ʱ������
uint8_t i = 0;             ///<������ʱ������������
uint8_t s;                 //<������¼��ֵ

uint32_t Pulse_num = 0;    ///<�������
volatile float Duty = 0;   ///<ռ�ձ�


uint16_t x_1 = 30, x_2 = 0, y_1 = 0, y_2 = 0, backup = 300; ///<���Ʋ�����Ҫ�õ��ı���


_Pid Pid;                   //�½�һ��pid�ṹ��,����ߵ��������ڵ�ǰģʽ�ļ���
_Pid PidV_Buff;             //�½�һ��pid�ṹ��,����߱��������׵���Ӧ��һ��ģʽ�Ĳ���
_Pid PidW_Buff;             //�½�һ��pid�ṹ��,����߱��������׵���Ӧ��һ��ģʽ�Ĳ���

_dissbuff parambuff;        //�½�һ��dissbuff�ṹ�壬��������ṹ���뿴main.h

/**
* @brief Pid�����ʼ������
* @detail ���ڳ�ʼ��Pid��������Ĳ���
*
* @param None
* @return None
*/
void Pid_Init(void)         //�ϵ��ʼ��һ��ģʽ���ɣ����ڿ����ٸ�
{
    Pid.SetSpeed = 150;			//150��
    Pid.Err = 0;
    Pid.Err1 = 0;
    Pid.Err2 = 0;
    Pid.Kp = 0.5; //�ʵ�����Ҫ��΢��һ��
    Pid.Ki = 0.1;
    Pid.Kd = 0.05;
}
/**
* @brief Pidbuff��ʼ������
* @detail ���ڳ�ʼ��pidbuff
*
* @param None
* @return None
*/
void PidBuff_Init()
{
    PidV_Buff.SetSpeed = 50;
    PidV_Buff.Kp = 0.004;
    PidV_Buff.Ki = 0.1;
    PidV_Buff.Kd = 0.05;

    PidW_Buff.SetSpeed = 150;
    PidW_Buff.Kp = 0.5;
    PidW_Buff.Ki = 0.1;
    PidW_Buff.Kd = 0.05;

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
    Pid.Err = Pid.SetSpeed - Pid.ActualSpeed;
    Adjust = Pid.Kp * (Pid.Err - Pid.Err1) + Pid.Ki * Pid.Err + Pid.Kd * (Pid.Err - 2 * Pid.Err1 + Pid.Err2);
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
int main(void)
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
    GPIO_SetBits(GPIOB, GPIO_Pin_6);
    PWM_SetDuty(50);
    /*Pid���ٳ�ʼ��*/
    Pid_Init();
    PidBuff_Init();
    /*׼���ð���*/
    Key_Init();
    /*��ʾ�����ı���*/
    Param_NameShow();
    Param_Refresh();

    while(1) {
        Key_Handle();
        if(mode) {
            Dir_Control();
        }
        if(status_index) {  //���ڵ���״̬ʱɲ��
            GPIO_ResetBits(GPIOB, GPIO_Pin_6);
            GPIO_ResetBits(GPIOB, GPIO_Pin_7);
        }
    }
}
/**
* @brief ����������ܼ�
* @param None
* @return None
*/
void Motor_Start()
{
    GPIO_SetBits(GPIOB, GPIO_Pin_6);
    PWM_SetDuty(50);
}

/**
* @brief ����ȷ����֮����»����������ݵ�pidbuff
* @detail �ٵ��εĹ����У������Ὣ�����ֵ�������µ����������Ƿ��ڻ�������
*         ������ȷ����֮�����µ�����
* @param None
* @return None
*/
void ParamBuff_Update(void)    //�����ݸ��µ�pidbuff
{
    switch(status_index) {
    case 1:
        mode = ((uint8_t)(my_atof(parambuff.Mode_Disbuff)));
        break;
    default:
        break;
    }
    if(!mode) {
        switch(status_index) {
        case 2:
            PidV_Buff.SetSpeed = my_atof(parambuff.SetV_Disbuff);
            break;
        case 3:
            PidV_Buff.Kp = my_atof(parambuff.Kp_Disbuff);
            break;
        case 4:
            PidV_Buff.Ki = my_atof(parambuff.Ki_Disbuff);
            break;
        case 5:
            PidV_Buff.Kd = my_atof(parambuff.Kd_Disbuff);
            break;
        default:
            break;
        }
    } else {
        switch(status_index) {
        case 2:
            PidW_Buff.SetSpeed = my_atof(parambuff.SetV_Disbuff);
            break;
        case 3:
            PidW_Buff.Kp = my_atof(parambuff.Kp_Disbuff);
            break;
        case 4:
            PidW_Buff.Ki = my_atof(parambuff.Ki_Disbuff);
            break;
        case 5:
            PidW_Buff.Kd = my_atof(parambuff.Kd_Disbuff);
            break;
        default:
            break;
        }
    }
}
/**
* @brief ����ȷ����֮�����pidbuff�����ݵ�����
* @detail ������ȷ����֮�����µ����� 
*    ��
* @param None
* @return None
*/
void  Param_Update(void)
{
    if(!mode) {
        Pid.SetSpeed = PidV_Buff.SetSpeed;
        Pid.Kp = PidV_Buff.Kp;
        Pid.Ki = PidV_Buff.Ki;
        Pid.Kd = PidV_Buff.Kd;
    } else {
        Pid.SetSpeed = PidW_Buff.SetSpeed;
        Pid.Kp = PidW_Buff.Kp;
        Pid.Ki = PidW_Buff.Ki;
        Pid.Kd = PidW_Buff.Kd;
    }
}
/**
* @brief ���������ʾˢ��
* @param None
* @return None
*/
void Param_Refresh(void)      //�����������ʾ����Ļ,Ҫд��handle֮ǰ
{
    my_printf(48, 16, "%d", mode);
    my_printf(168, 16, "%f", Pid.SetSpeed);
    my_printf(24, 32, "%f", Pid.Kp);
    my_printf(104, 32, "%f", Pid.Ki);
    my_printf(184, 32, "%f", Pid.Kd);
}
/**
* @brief ��ʾ���������������Ϣ����Ļ
* @param None
* @return None
*/
void Param_NameShow(void)
{
    my_printf(8,   0, "value:");
    my_printf(128, 0, "duty:");
    my_printf(8,  16, "mode:");
    my_printf(128, 16, "goal:");
    my_printf(8,  32, "P:");
    my_printf(88, 32, "I:");
    my_printf(168, 32, "D:");
}
/**
* @brief ���ι����и��������̬ˢ��
* @detail �ڵ��ι����У�ÿ��һ�ΰ�����Ļ����ʵʱ��������������֣��������Ի�
* @param None
* @return None
*/
void Param_DynaRefresh(void)     //�˺���Ӧ��ֻ�ڵ���״̬�¹��� if����status_index)
{
    switch(status_index) {
    case 1:
        my_printf(48, 16, "%f", my_atof(parambuff.Mode_Disbuff));
        break;
    case 2:
        my_printf(168, 16, "%f", my_atof(parambuff.SetV_Disbuff));
        break;
    case 3:
        my_printf(24, 32, "%f", my_atof(parambuff.Kp_Disbuff));
        break;
    case 4:
        my_printf(104, 32, "%f", my_atof(parambuff.Ki_Disbuff));
        break;
    case 5:
        my_printf(184, 32, "%f", my_atof(parambuff.Kd_Disbuff));
        break;
    default :
        break;
    }

}
/**
* @brief ���ָʾ�����ľ�������һ������
* @detail ��*��������ʾ�����ľ�������һ������
*
* @param None
* @return None
*/
void Cusor_Show(void)      //�˺�������ָʾ�������ĸ�����,���ж����ˢ��
{
    switch(status_index) {
    case 1:
        my_printf(0, 16, "%c", '*');
        my_printf(120, 16, "%c", ' ');
        my_printf(0, 32, "%c", ' ');
        my_printf(80, 32, "%c", ' ');
        my_printf(160, 32, "%c", ' ');
        break;
    case 2:
        my_printf(0, 16, "%c", ' ');
        my_printf(120, 16, "%c", '*');
        my_printf(0, 32, "%c", ' ');
        my_printf(80, 32, "%c", ' ');
        my_printf(160, 32, "%c", ' ');
        break;
    case 3:
        my_printf(0, 16, "%c", ' ');
        my_printf(120, 16, "%c", ' ');
        my_printf(0, 32, "%c", '*');
        my_printf(80, 32, "%c", ' ');
        my_printf(160, 32, "%c", ' ');
        break;
    case 4:
        my_printf(0, 16, "%c", ' ');
        my_printf(120, 16, "%c", ' ');
        my_printf(0, 32, "%c", ' ');
        my_printf(80, 32, "%c", '*');
        my_printf(160, 32, "%c", ' ');
        break;
    case 5:
        my_printf(0, 16, "%c", ' ');
        my_printf(120, 16, "%c", ' ');
        my_printf(0, 32, "%c", ' ');
        my_printf(80, 32, "%c", ' ');
        my_printf(160, 32, "%c", '*');
        break;
    case 0:
        my_printf(0, 16, "%c", ' ');
        my_printf(120, 16, "%c", ' ');
        my_printf(0, 32, "%c", ' ');
        my_printf(80, 32, "%c", ' ');
        my_printf(160, 32, "%c", ' ');
        break;
    default :
        break;
    }
}


