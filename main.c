/**
 * @file main.c
 * @brief 整个工程的主函数写在该文件中
 * @mainpage 主函数文件
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

/*几个重要的全局变量*/
uint8_t mode = 0;          ///<运行模式，0，1代表两种模式
uint8_t i = 0, j = 0, k = 0; ///<for循环，等某些需要变量指引的场合使用

float EE_Buff[4];
uint32_t Pulse_num = 0;    ///<脉冲个数
volatile float Duty = 0;   ///<占空比

_Pid Pid;                   //新建一个pid结构体,这里边的数据用于当前模式的计算
_Pid PidV_Buff;             //新建一个pid结构体,这里边保存着整套的适应于一种模式的参数
_Pid PidW_Buff;             //新建一个pid结构体,这里边保存着整套的适应于一种模式的参数
_dissbuff parambuff;        //新建一个dissbuff结构体，关于这个结构体请看main.h

float Pid_Cal(void);
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
int main(void)
{
	delay_ms(100); //上电时等待器件就绪
	/*准备好软件模拟I2C*/
    I2CInit();
	/*准备好LED小灯用于指示运行状态*/
    LED_GPIO_Config();
	/*准备好LCD显示相关*/
    LCD_Ready();
	/*初始化坐标系*/
    DrawOrdinate();
	/*准备好串口接收*/
    USART_Config();
	/*准备好脉冲捕获中断*/
    PulseCap_EXTI_Config();
	/*准备好PWM生成定时器*/
	TIM_PWM_GPIO_Congig();
    TIM_PWM_Config();
    PWM_Enable();
	/*准备好基本定时器6开启中断进行PID调速*/
    BASIC_TIM_Config();
    BASIC_TIM_NVIC_Config();
    BASE_TIM_APBxClockCmd(BASE_TIM_Clock, ENABLE);
	/*准备好基本定时器7开启中断进行信息显示*/
    DISPLAYTIM_Config();
    DISPLAYTIM_NVIC_Config();
	/*准备好角度模式下电机方向控制的GPIO*/
    Dir_Control_GPIO_Config();
	/*准备好角度模式下编码器相关工作*/
    TIM3_Encoder_GPIO_Config();
    TIM3_Encoder_Config();
	/*启动电机*/    
	GPIO_SetBits(GPIOA, GPIO_Pin_12);
    PWM_SetDuty(50);
	/*准备好按键*/
    Key_Init(); 
	/*取回备份的数据*/
    Data_Fetch();
	/*更新缓冲区的数据*/
    Param_Update();
	/*显示变量的标题*/
    Param_NameShow();
	/*参数显示刷新*/
    Param_Refresh();
	
    while(1) {
        Key_Handle();//按键处理函数需要一直运行
        if(mode) {
            Dir_Control();//在角度模式下，要时刻纠正电机的转向
        }
        if(status_index) {  //处于调整状态时刹车
            GPIO_ResetBits(GPIOA, GPIO_Pin_11);
            GPIO_ResetBits(GPIOA, GPIO_Pin_12);
        }
        if(recv_flag) {  //接收到PC发来的命令，并进行处理
            recv_flag = 0;
            my_printf(8, LINE(3), "PC_Cmd:%c:%f", recv_buff[0], my_atof((recv_buff + 2)));
            Pid.SetSpeed = my_atof((recv_buff + 2));
            Param_Refresh();
            PC_CommandHandle();
        }
    }
}
/**
* @brief Pid计算函数
* @detail 根据实时反馈的速度值，计算出来调整量
*
* @param setspeed 速度的目标值
* @return Adjust
* @retval 根据Adjust即可调整占空比
*/
float Pid_Cal()     //这个函数应当适应于两种模式，因为原理都一样
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
