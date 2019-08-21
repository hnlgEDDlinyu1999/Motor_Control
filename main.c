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
#include "string.h"
#include "stm32f10x.h"
#include "./main.h"
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
#include "./bsp_i2c_ee/bsp_i2c_ee.h"

/*几个重要的全局变量*/
uint8_t mode = 1;          ///<运行模式，0，1代表两种模式
uint8_t status_index = 0;  ///<调参时与正常运行时的索引
uint8_t i=0,j=0,k=0;       ///<for循环，等某些需要变量指引的场合使用
uint8_t s;                 //<用来记录键值
char recv_flag=0,recv_buff[10],tempbuff[10];          ///<接收到上位机命令的标志
float EE_Buff[4];

uint32_t Pulse_num = 0;    ///<脉冲个数
volatile float Duty = 0;   ///<占空比


uint16_t x_1 = 30, x_2 = 0, y_1 = 0, y_2 = 0, backup = 300; ///<绘制波形所要用到的变量


_Pid Pid;                   //新建一个pid结构体,这里边的数据用于当前模式的计算
_Pid PidV_Buff;             //新建一个pid结构体,这里边保存着整套的适应于一种模式的参数
_Pid PidW_Buff;             //新建一个pid结构体,这里边保存着整套的适应于一种模式的参数

_dissbuff parambuff;        //新建一个dissbuff结构体，关于这个结构体请看main.h

/**
* @brief Pid计算初始化函数
* @detail 用于初始化Pid计算所需的参数
*
* @param None
* @return None
*/
void Pid_Init(void)         //上电初始化一种模式即可，后期可以再改
{
    Pid.SetSpeed = 150;			//150度
    Pid.Err = 0;
    Pid.Err1 = 0;
    Pid.Err2 = 0;
    Pid.Kp = 0.5; //适当的需要稍微大一点
    Pid.Ki = 0.1;
    Pid.Kd = 0.05;
}
/**
* @brief Pidbuff初始化函数
* @detail 用于初始化pidbuff
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
    Pid.Err = Pid.SetSpeed - Pid.ActualSpeed;
    Adjust = Pid.Kp * (Pid.Err - Pid.Err1) + Pid.Ki * Pid.Err + Pid.Kd * (Pid.Err - 2 * Pid.Err1 + Pid.Err2);
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
int main(void)
{
    /*准备好LED小灯用于指示运行状态*/
    LED_GPIO_Config();
    /*准备好LCD显示相关*/
    LCD_Ready();
    DrawOrdinate();
	
	/*准备好串口接收*/
	USART_Config();
	/*准备好e2prom*/
	I2C_EE_Init();

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
    GPIO_SetBits(GPIOB, GPIO_Pin_6);
    PWM_SetDuty(50);
    /*Pid调速初始化*/
    Pid_Init();
    PidBuff_Init();
    /*准备好按键*/
    Key_Init();
    /*显示变量的标题*/
    Param_NameShow();
    //Param_Refresh();
	Param_EE_Update();
	/**************************/

    while(1) {
        Key_Handle();
        if(mode) {
            Dir_Control();
        }
        if(status_index) {  //处于调整状态时刹车
            GPIO_ResetBits(GPIOB, GPIO_Pin_6);
            GPIO_ResetBits(GPIOB, GPIO_Pin_7);
        }
		if(recv_flag) {
            recv_flag = 0;
			my_printf(8,LINE(3),"PC_Cmd:%c:%f",recv_buff[0],my_atof((recv_buff+2)));
            PC_CommandHandle();
        }
    }
}

/***********************************THE END************************************/

