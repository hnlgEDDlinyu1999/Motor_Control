/**
 * @file config.h
 * @brief Ϊ��֧��λ�������������д�˹���λ��������һЩ׼������
 * @mainpage CONFIG_H
 * @author hnlgEDDLinYu
 * @email 2970658553@qq.com
 * @version 1.0.0
 * @date 2019/8/16
 * @copyright    Copyright (c) hnlgEDDLinYu
 */
#ifndef _CONFIG_H
#define _CONFIG_H

#include "stm32f10x.h"


//λ���궨��
#define BITBAND(addr, bitnum)   ((addr&0xF0000000) + 0x2000000 + ((addr&0xFFFFF)<<5) + (bitnum<<2))
#define MEM_ADDR(addr)          *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)  MEM_ADDR(BITBAND(addr, bitnum))

//IO�ڵ�ַλ��ӳ��
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //GPIOA������ݼĴ�����ַ0x4001080C
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //GPIOB������ݼĴ�����ַ0x40010C0C
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //GPIOC������ݼĴ�����ַ0x4001100C
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //GPIOD������ݼĴ�����ַ0x4001140C
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //GPIOE������ݼĴ�����ַ0x4001180C
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //GPIOF������ݼĴ�����ַ0x40011A0C
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //GPIOG������ݼĴ�����ַ0x40011E0C

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //GPIOA�������ݼĴ�����ַ0x40010808
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //GPIOB�������ݼĴ�����ַ0x40010C08
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //GPIOC�������ݼĴ�����ַ0x40011008
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //GPIOD�������ݼĴ�����ַ0x40011408
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //GPIOE�������ݼĴ�����ַ0x40011808
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //GPIOF�������ݼĴ�����ַ0x40011A08
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //GPIOG�������ݼĴ�����ַ0x40011E08

//����IO��λ������
#define PA_OUT(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //PAx���
#define PA_IN(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //PAx����

#define PB_OUT(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //PBx���
#define PB_IN(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //PBx����

#define PC_OUT(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //PCx���
#define PC_IN(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //PCx����

#define PD_OUT(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //PDx���
#define PD_IN(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //PDx����

#define PE_OUT(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //PEx���
#define PE_IN(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //PEx����

#define PF_OUT(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //PFx���
#define PF_IN(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //PFx����

#define PG_OUT(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //PGx���
#define PG_IN(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //PGx����


#define I2C_SCL_OUT   PC_OUT(2)
#define I2C_SDA_OUT   PC_OUT(3)
#define I2C_SDA_IN    PC_IN(3)

#endif
/***********************************THE END************************************/