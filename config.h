/*
*******************************************************************************
*                     《ARM嵌入式编程与实战应用--STM32F1系列》
*                      配套Kingst-32F1单片机开发板 示例源代码
*
*          (c) 版权所有 2018 金沙滩工作室/清华大学出版社 保留所有权利
*          官方网站：www.qdkingst.com
*          淘宝网店：kstmcu.taobao.com
*          技术支持QQ：415942827
*          关注微信公众号"金沙滩工作室"，在线学习51、STM32单片机教程
*
*  文件名：config.h
*  描  述：
*  备  注：
*******************************************************************************
*/
#ifndef _CONFIG_H
#define _CONFIG_H

#include "stm32f10x.h"


//位带宏定义
#define BITBAND(addr, bitnum)   ((addr&0xF0000000) + 0x2000000 + ((addr&0xFFFFF)<<5) + (bitnum<<2))
#define MEM_ADDR(addr)          *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)  MEM_ADDR(BITBAND(addr, bitnum))

//IO口地址位带映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //GPIOA输出数据寄存器地址0x4001080C
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //GPIOB输出数据寄存器地址0x40010C0C
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //GPIOC输出数据寄存器地址0x4001100C
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //GPIOD输出数据寄存器地址0x4001140C
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //GPIOE输出数据寄存器地址0x4001180C
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //GPIOF输出数据寄存器地址0x40011A0C
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //GPIOG输出数据寄存器地址0x40011E0C

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //GPIOA输入数据寄存器地址0x40010808
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //GPIOB输入数据寄存器地址0x40010C08
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //GPIOC输入数据寄存器地址0x40011008
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //GPIOD输入数据寄存器地址0x40011408
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //GPIOE输入数据寄存器地址0x40011808
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //GPIOF输入数据寄存器地址0x40011A08
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //GPIOG输入数据寄存器地址0x40011E08

//单个IO口位带操作
#define PA_OUT(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //PAx输出
#define PA_IN(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //PAx输入

#define PB_OUT(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //PBx输出
#define PB_IN(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //PBx输入

#define PC_OUT(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //PCx输出
#define PC_IN(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //PCx输入

#define PD_OUT(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //PDx输出
#define PD_IN(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //PDx输入

#define PE_OUT(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //PEx输出
#define PE_IN(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //PEx输入

#define PF_OUT(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //PFx输出
#define PF_IN(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //PFx输入

#define PG_OUT(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //PGx输出
#define PG_IN(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //PGx输入


#define I2C_SCL_OUT   PC_OUT(2)
#define I2C_SDA_OUT   PC_OUT(3)
#define I2C_SDA_IN    PC_IN(3)




#endif
