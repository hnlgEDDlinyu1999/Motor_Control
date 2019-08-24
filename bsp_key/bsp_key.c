/**
 * @file bsp_key.c
 * @brief 整个工程的按键部分写在该文件当中
 * @mainpage 按键驱动
 * @author hnlgEDDLinYu
 * @email 2970658553@qq.com
 * @version 1.0.0
 * @date 2019/8/17
 * @copyright    Copyright (c) hnlgEDDLinYu
 */
#include "./bsp_key/bsp_key.h"
#include "./delay/delay.h"
#include "./bsp_led/bsp_led.h"
#include <stdio.h>
#include "./bsp_drawgraph/bsp_drawgraph.h"
#include "./main.h"
#include "./bsp_param/bsp_param.h"

uint8_t key_int_flag = 0;

void KEYPAD4x4_Init(void)  //微动开关的接口初始化
{
    GPIO_InitTypeDef  GPIO_InitStruct; //定义GPIO的初始化枚举结构
    GPIO_InitStruct.GPIO_Pin = KEYa | KEYb | KEYc | KEYd; //选择端口号（0~15或all）
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; //选择IO接口工作方式 //上拉电阻
    GPIO_Init(KEYPAD4x4PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = KEY1 | KEY2 | KEY3 | KEY4; //选择端口号（0~15或all）
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; //选择IO接口工作方式 //上拉电阻
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //设置IO接口速度（2/10/50MHz）
    GPIO_Init(KEYPAD4x4PORT, &GPIO_InitStruct);

}

void KEYPAD4x4_Init_Toggle(void)  //微动开关的接口初始化2（用于IO工作方式反转）
{
    GPIO_InitTypeDef  GPIO_InitStruct; //定义GPIO的初始化枚举结构
    GPIO_InitStruct.GPIO_Pin = KEY1 | KEY2 | KEY3 | KEY4; //选择端口号（0~15或all）
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; //选择IO接口工作方式 //上拉电阻
    GPIO_Init(KEYPAD4x4PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = KEYa | KEYb | KEYc | KEYd; //选择端口号（0~15或all）
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; //选择IO接口工作方式 //上拉电阻
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //设置IO接口速度（2/10/50MHz）
    GPIO_Init(KEYPAD4x4PORT, &GPIO_InitStruct);

}

/**
* @brief 按键读值，有按键被按下时才会被调用
* @param None
* @return b
* @retval b是读到的键值
*/
uint8_t KEYPAD4x4_Read(void)
{
    uint8_t a = 0, b = 0; //定义变量
    KEYPAD4x4_Init();//初始化IO
    GPIO_ResetBits(KEYPAD4x4PORT, KEY1 | KEY2 | KEY3 | KEY4);
    GPIO_SetBits(KEYPAD4x4PORT, KEYa | KEYb | KEYc | KEYd);
    if(!GPIO_ReadInputDataBit(KEYPAD4x4PORT, KEYa) ||  	//查寻键盘口的值是否变化
       !GPIO_ReadInputDataBit(KEYPAD4x4PORT, KEYb) ||
       !GPIO_ReadInputDataBit(KEYPAD4x4PORT, KEYc) ||
       !GPIO_ReadInputDataBit(KEYPAD4x4PORT, KEYd)) {
        delay_ms(20); //延时20毫秒
        if(!GPIO_ReadInputDataBit(KEYPAD4x4PORT, KEYa) ||  	//查寻键盘口的值是否变化
           !GPIO_ReadInputDataBit(KEYPAD4x4PORT, KEYb) ||
           !GPIO_ReadInputDataBit(KEYPAD4x4PORT, KEYc) ||
           !GPIO_ReadInputDataBit(KEYPAD4x4PORT, KEYd)) {
            a = (GPIO_ReadInputData(KEYPAD4x4PORT) >> 8) & 0xff; //键值放入寄存器a
        }

        KEYPAD4x4_Init_Toggle();//IO工作方式反转
        GPIO_SetBits(KEYPAD4x4PORT, KEY1 | KEY2 | KEY3 | KEY4);
        GPIO_ResetBits(KEYPAD4x4PORT, KEYa | KEYb | KEYc | KEYd);
        b = (GPIO_ReadInputData(KEYPAD4x4PORT) >> 8) & 0xff; //将第二次取得值放入寄存器b
        a = a | b; //将两个数据相或
        switch(a) { //对比数据值
        case 0xee:
            b =  1;
            break;//对比得到的键值给b一个应用数据
        case 0xde:
            b =  2;
            break;
        case 0xbe:
            b =  3;
            break;
        case 0x7e:
            b =  4;
            break;
        case 0xed:
            b =  5;
            break;
        case 0xdd:
            b =  6;
            break;
        case 0xbd:
            b =  7;
            break;
        case 0x7d:
            b =  8;
            break;
        case 0xeb:
            b =  9;
            break;
        case 0xdb:
            b = 10;
            break;
        case 0xbb:
            b = 11;
            break;
        case 0x7b:
            b = 12;
            break;
        case 0xe7:
            b = 13;
            break;
        case 0xd7:
            b = 14;
            break;
        case 0xb7:
            b = 15;
            break;
        case 0x77:
            b = 16;
            break;
        default:
            b = 0;
            break;
        }
        while(!GPIO_ReadInputDataBit(KEYPAD4x4PORT, KEY1) ||  	//等待按键放开
              !GPIO_ReadInputDataBit(KEYPAD4x4PORT, KEY2) ||
              !GPIO_ReadInputDataBit(KEYPAD4x4PORT, KEY3) ||
              !GPIO_ReadInputDataBit(KEYPAD4x4PORT, KEY4));
        delay_ms(20); //延时20毫秒
    }
    return (b);//将b作为返回值
}
/**
* @brief 按键中断初始化
* @detail 本次完全使用中断处理按键，按下任何一个键中断就会被唤醒，没有按下时也不做
*         任何扫描，非常节省资源，只是用了四个中断
* @param None
* @return None
*/
void KEYPAD4x4_INT_INIT(void)
{
    NVIC_InitTypeDef  NVIC_InitStruct;	//定义结构体变量
    EXTI_InitTypeDef  EXTI_InitStruct;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    KEYPAD4x4_APBxPeriphClockCmd(KEYPAD4x4_GPIO_CLK, ENABLE); //启动GPIO时钟 （需要与复用时钟一同启动）
    KEYPAD4x4_APBxPeriphClockCmd(KEYPAD4x4_INT_AFIO_CLK, ENABLE); //配置端口中断需要启用复用时钟

    GPIO_EXTILineConfig(KEYPAD4x4_INT_PORTSOURCE, GPIO_PinSource12);
    EXTI_InitStruct.EXTI_Line = EXTI_Line12;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(& EXTI_InitStruct);
    NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 5;
    NVIC_Init(& NVIC_InitStruct);

    GPIO_EXTILineConfig(KEYPAD4x4_INT_PORTSOURCE, GPIO_PinSource13);
    EXTI_InitStruct.EXTI_Line = EXTI_Line13;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(& EXTI_InitStruct);
    NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 6;
    NVIC_Init(& NVIC_InitStruct);

    GPIO_EXTILineConfig(KEYPAD4x4_INT_PORTSOURCE, GPIO_PinSource14);
    EXTI_InitStruct.EXTI_Line = EXTI_Line14;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(& EXTI_InitStruct);
    NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 7;
    NVIC_Init(& NVIC_InitStruct);

    GPIO_EXTILineConfig(KEYPAD4x4_INT_PORTSOURCE, GPIO_PinSource15);
    EXTI_InitStruct.EXTI_Line = EXTI_Line15;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(& EXTI_InitStruct);
    NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 7;
    NVIC_Init(& NVIC_InitStruct);

}
/**
* @brief 按键初始化
* @detail 一方面初始化引脚，一方面会设置好按键输入中断
* @param None
* @return None
*/
void Key_Init(void)
{
    KEYPAD4x4_Init();     //引脚初始化
    KEYPAD4x4_INT_INIT(); //按键中断初始化
}
/**
* @brief 按键按下时的响应函数
* @detail 按键按下时，该函数会判断按键的类型，并做出具体的反应
* @param None
* @return None
*/
void Key_Handle(void)
{
    if(key_int_flag) {
        key_int_flag = 0;
        s = KEYPAD4x4_Read();
        if(s != 0) {
            switch(s) {
            case 15 :
                if(!status_index) {
                    status_index  = 1;
                    Cusor_Show();
                    break;   //必须要先按，否则其他的部件都不运行
                } else {
                    ParamBuff_Update();//按下确定键，先更新到pidbuff
                    Param_Update();
                    Param_Refresh();
                    if(!mode) {
                        Motor_Start();
                    }
                    status_index  = 0;
                    i = 0;
                    Cusor_Show();
                    break;
                }
            case 4  :
                if(status_index) {
                    status_index += 1;
                    if(status_index > 5)status_index = 5;
                    i = 0;
                    Cusor_Show();
                    break;
                } else {
                    break;
                }
            case 8  :

                if(status_index) {
                    status_index -= 1;
                    if(status_index < 1)status_index = 1;
                    i = 0;
                    Cusor_Show();
                    break;
                } else {
                    break;
                }
            default:
                switch(status_index) { //index确定之后才让进入改参
                case 0:
                    break;
                case 1:
                    switch(s) {
                    case 14:
                        parambuff.Mode_Disbuff[i] = '0';
                        parambuff.Mode_Disbuff[i + 1] = '\0';
                        break;
                    case  1:
                        parambuff.Mode_Disbuff[i] = '1';
                        parambuff.Mode_Disbuff[i + 1] = '\0';
                        break;
                    default:
                        break;
                    }
                    break;
                case 2:
                    switch(s) {
                    case  1:
                        parambuff.SetV_Disbuff[i] = '1';
                        parambuff.SetV_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case  2:
                        parambuff.SetV_Disbuff[i] = '2';
                        parambuff.SetV_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case  3:
                        parambuff.SetV_Disbuff[i] = '3';
                        parambuff.SetV_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case  5:
                        parambuff.SetV_Disbuff[i] = '4';
                        parambuff.SetV_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case  6:
                        parambuff.SetV_Disbuff[i] = '5';
                        parambuff.SetV_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case  7:
                        parambuff.SetV_Disbuff[i] = '6';
                        parambuff.SetV_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case  9:
                        parambuff.SetV_Disbuff[i] = '7';
                        parambuff.SetV_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case 10:
                        parambuff.SetV_Disbuff[i] = '8';
                        parambuff.SetV_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case 11:
                        parambuff.SetV_Disbuff[i] = '9';
                        parambuff.SetV_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case 13:
                        parambuff.SetV_Disbuff[i] = '.';
                        parambuff.SetV_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case 14:
                        parambuff.SetV_Disbuff[i] = '0';
                        parambuff.SetV_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
					default:
                        break;
                    }
                    break;
                case 3:
                    switch(s) {
                    case  1:
                        parambuff.Kp_Disbuff[i] = '1';
                        parambuff.Kp_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case  2:
                        parambuff.Kp_Disbuff[i] = '2';
                        parambuff.Kp_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case  3:
                        parambuff.Kp_Disbuff[i] = '3';
                        parambuff.Kp_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case  5:
                        parambuff.Kp_Disbuff[i] = '4';
                        parambuff.Kp_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case  6:
                        parambuff.Kp_Disbuff[i] = '5';
                        parambuff.Kp_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case  7:
                        parambuff.Kp_Disbuff[i] = '6';
                        parambuff.Kp_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case  9:
                        parambuff.Kp_Disbuff[i] = '7';
                        parambuff.Kp_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case 10:
                        parambuff.Kp_Disbuff[i] = '8';
                        parambuff.Kp_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case 11:
                        parambuff.Kp_Disbuff[i] = '9';
                        parambuff.Kp_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case 13:
                        parambuff.Kp_Disbuff[i] = '.';
                        parambuff.Kp_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case 14:
                        parambuff.Kp_Disbuff[i] = '0';
                        parambuff.Kp_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
					default:
                        break;
                    }
                    break;
                case 4:
                    switch(s) {
                    case  1:
                        parambuff.Ki_Disbuff[i] = '1';
                        parambuff.Ki_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case  2:
                        parambuff.Ki_Disbuff[i] = '2';
                        parambuff.Ki_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case  3:
                        parambuff.Ki_Disbuff[i] = '3';
                        parambuff.Ki_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case  5:
                        parambuff.Ki_Disbuff[i] = '4';
                        parambuff.Ki_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case  6:
                        parambuff.Ki_Disbuff[i] = '5';
                        parambuff.Ki_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case  7:
                        parambuff.Ki_Disbuff[i] = '6';
                        parambuff.Ki_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case  9:
                        parambuff.Ki_Disbuff[i] = '7';
                        parambuff.Ki_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case 10:
                        parambuff.Ki_Disbuff[i] = '8';
                        parambuff.Ki_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case 11:
                        parambuff.Ki_Disbuff[i] = '9';
                        parambuff.Ki_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case 13:
                        parambuff.Ki_Disbuff[i] = '.';
                        parambuff.Ki_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case 14:
                        parambuff.Ki_Disbuff[i] = '0';
                        parambuff.Ki_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
					default:
                        break;
                    }
                    break;
                case 5:
                    switch(s) {
                    case  1:
                        parambuff.Kd_Disbuff[i] = '1';
                        parambuff.Kd_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case  2:
                        parambuff.Kd_Disbuff[i] = '2';
                        parambuff.Kd_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case  3:
                        parambuff.Kd_Disbuff[i] = '3';
                        parambuff.Kd_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case  5:
                        parambuff.Kd_Disbuff[i] = '4';
                        parambuff.Kd_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case  6:
                        parambuff.Kd_Disbuff[i] = '5';
                        parambuff.Kd_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case  7:
                        parambuff.Kd_Disbuff[i] = '6';
                        parambuff.Kd_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case  9:
                        parambuff.Kd_Disbuff[i] = '7';
                        parambuff.Kd_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case 10:
                        parambuff.Kd_Disbuff[i] = '8';
                        parambuff.Kd_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case 11:
                        parambuff.Kd_Disbuff[i] = '9';
                        parambuff.Kd_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case 13:
                        parambuff.Kd_Disbuff[i] = '.';
                        parambuff.Kd_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
                    case 14:
                        parambuff.Kd_Disbuff[i] = '0';
                        parambuff.Kd_Disbuff[i + 1] = '\0';
                        i += 1;
                        break;
					default:
                        break;
                    }
                    break;
                default :
                    break;

                }


            }
        }
    }
}
/***********************************THE END************************************/