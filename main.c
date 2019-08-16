/**
 * @file main.c
 * @brief 整个工程的主函数写在该文件中
 * @mainpage 主函数文件
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


float V = 0;               ///<实际线速度
float SetV = 50;           ///<目标速度
uint32_t Pulse_num = 0;    ///<脉冲个数
float Duty = 0;            ///<占空比


uint16_t x_1=30,x_2=0,y_1=0,y_2=0,backup=300;///<绘制波形所要用到的变量


_Pid Pid;                  //新建一个pid结构体
_dissbuff dissbuff;        //新建一个dissbuff结构体
/**
* @brief Pid计算初始化函数
* @detail 用于初始化Pid计算所需的参数
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
* @brief Pid计算函数
* @detail 根据实时反馈的速度值，计算出来调整量
* 
* @param setspeed 速度的目标值
* @return Adjust
* @retval 根据Adjust即可调整占空比
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
* @brief 主函数
* @details 程序唯一入口
*
* @param argc 命令参数个数
* @param argv 命令参数指针数组
* @return 程序执行成功与否
*     @retval 0 程序执行成功
*     @retval 1 程序执行失败
* @note Null
*/
int main (void)
{
	/*准备好LED小灯用于指示运行状态*/
	LED_GPIO_Config();
	/*准备好LCD显示相关*/
	LCD_Ready();
	DrawOrdinate();
	
	/*准备好脉冲捕获中断*/
	PulseCap_EXTI_Config();
	
	/*准备好PWM生成定时器*/
	TIM_PWM_GPIO_Congig();
	TIM_PWM_Config();
	PWM_Enable();
	
	/*准备好基本定时器6开启中断进行PID调速*/
	BASIC_TIM_Config ();
	BASIC_TIM_NVIC_Config ();
	BASE_TIM_APBxClockCmd(BASE_TIM_Clock,ENABLE);
	/*准备好基本定时器7开启中断进行信息显示*/
	DISPLAYTIM_Config ();
	DISPLAYTIM_NVIC_Config ();
	/*启动电机*/
	PWM_SetDuty(100);
	/*Pid调速初始化*/
	Pid_Init();

    while (1);
}


 