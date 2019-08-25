/**
 * @file eeprom.c
 * @brief  eeprom读写驱动文件
 * @mainpage EEPROM读写
 * @author hnlgEDDLinYu
 * @email 2970658553@qq.com
 * @version 1.0.0
 * @date 2019/8/22
 * @copyright    Copyright (c) hnlgEDDLinYu
 */
#include "./bsp_eeprom/eeprom.h"

/**
* @brief 读取E2中的一个字节
* @detail 根据EEPROM器件手册的时序要求，读取一个字节
*
* @param addr 字节地址
* @return None
*/
uint8_t E2ReadByte(uint8_t addr)
{
    uint8_t dat;
    
    do { //用寻址操作查询当前是否可进行读写
        I2CStart();
        if (I2CWrite(0x50<<1)) //寻址器件，应答则跳出循环，否则继续查询
        {
            break;
        }
        I2CStop();
    } while(1);
    I2CWrite(addr);           //写入存储地址
    I2CStart();               //发送重复启动信号
    I2CWrite((0x50<<1)|0x01); //寻址器件，后续为读操作
    dat = I2CReadNAK();       //读取一个字节数据
    I2CStop();
    
    return dat;
}

/**
* @brief 写入E2中的一个字节
* @detail 根据EEPROM器件手册的时序要求，写入一个字节
*
* @param addr 字节地址
* @return None
*/
void E2WriteByte(uint8_t addr, uint8_t dat)
{
    do { //用寻址操作查询当前是否可进行读写
        I2CStart();
        if (I2CWrite(0x50<<1)) //寻址器件，应答则跳出循环，否则继续查询
        {
            break;
        }
        I2CStop();
    } while(1);
    I2CWrite(addr);  //写入存储地址
    I2CWrite(dat);   //写入一个字节数据
    I2CStop();
}

/* E2连续读取函数，addr-E2中的起始地址，buf-数据接收指针，len-读取长度 */
/**
* @brief 连续读取函数
* @detail 根据EEPROM器件手册的时序要求，一次性读取多个字节
*
* @param addr E2中的起始地址
* @param *buf 数据接收指针
* @param len 读取长度 
* @return None
*/
void E2Read(uint8_t *buf, uint8_t addr, int len)
{
    do { //用寻址操作查询当前是否可进行读写
        I2CStart();
        if (I2CWrite(0x50<<1)) //寻址器件，应答则跳出循环，否则继续查询
        {
            break;
        }
        I2CStop();
    } while(1);
    I2CWrite(addr);            //写入起始地址
    I2CStart();                //发送重复启动信号
    I2CWrite((0x50<<1)|0x01);  //寻址器件，后续为读操作
    while (len > 1)            //连续读取len-1个字节
    {
        *buf++ = I2CReadACK(); //最后字节之前为读取操作+应答
        len--;
    }
    *buf = I2CReadNAK();       //最后一个字节为读取操作+非应答
    I2CStop();
}


/**
* @brief 连续写入函数
* @detail 根据EEPROM器件手册的时序要求，一次性写入多个字节
*
* @param addr E2中的起始地址
* @param *buf 源数据指针
* @param len 读取长度 
* @return None
*/
void E2Write(const uint8_t *buf, uint8_t addr, int len)
{
    while (len > 0)
    {
        do { //用寻址操作查询当前是否可进行读写
            I2CStart();
            if (I2CWrite(0x50<<1)) //寻址器件，应答则跳出循环，否则继续查询
            {
                break;
            }
            I2CStop();
        } while(1);
        //按页写模式连续写入字节
        I2CWrite(addr);        //写入起始地址
        while (len > 0)
        {
            I2CWrite(*buf++);  //写入一个字节数据
            len--;             //待写入长度计数递减
            addr++;            //E2地址递增
            if ((addr%8) == 0) //检查地址是否到达页边界，24C02每页8字节，
            {                  //所以检测低3位是否为零即可
                break;         //到达页边界时，跳出循环，结束本次写操作
            }
        }
        I2CStop();
    }
}
