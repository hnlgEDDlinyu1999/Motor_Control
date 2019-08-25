/*学长，我在调stm32的i2c读写eeprom遇到了一系列的问题，所以使用软件i2c.
这套程序移植自51单片机平台，软件i2c使用位带操作不受硬件引脚的限制，
我准备发一个帖子，因而注释是以指导的口吻来写的，忽略即可*/
/**
 * @file i2c.c
 * @brief 软件模拟ic通信，可以使用任何两个GPIO口，但是，要做好引脚初始化工作（ICInit（void）函数）。
 *		  该功能的实现用的全是位带操作，一定要包含config.h（里面做了位带操作的准备）。
 *        关于延时，请提供如下格式的接口函数：delay_us(uint32_t)。
 *        再者，本i2c通信协议可兼容任何速度的器件，因而速度限制在100Kb/s
 *        如需提高速率，请自行更改i2c延时时间。
 * @mainpage 软件模拟IC通信
 * @author hnlgEDDLinYu
 * @email 2970658553@qq.com
 * @version 1.0.0
 * @date 2019/8/22
 * @copyright    Copyright (c) hnlgEDDLinYu
 */
#include "./bsp_I2c/i2c.h"



/**
* @brief I2C引脚初始化函数
* @detail 用于初始化I2C通信所使用的两个引脚
*
* @param None
* @return None
*/
void I2CInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //使能GPIOB时钟       
    GPIO_SetBits(GPIOC, GPIO_Pin_2|GPIO_Pin_3);           //SCL和SDA初始输出高电平
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;  //选择SCL和SDA引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;      //选择开漏输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;     //输出速率10MHz（I2C足够了）
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}
/**
* @brief 总线起始信号产生函数
* @detail 根据I2C通信协议的要求，产生一个起始信号
*
* @param None
* @return None
*/
void I2CStart(void)
{
    I2C_SDA_OUT = 1; //首先确保SDA、SCL都是高电平
    I2C_SCL_OUT = 1;
    delay_us(5);
    I2C_SDA_OUT = 0; //先拉低SDA
    delay_us(5);
    I2C_SCL_OUT = 0; //再拉低SCL
}

/**
* @brief 总线停止信号产生函数
* @detail 根据I2C通信协议的要求，产生一个停止信号
*
* @param None
* @return None
*/
void I2CStop(void)
{
    I2C_SCL_OUT = 0; //首先确保SDA、SCL都是低电平
    I2C_SDA_OUT = 0;
    delay_us(5);
    I2C_SCL_OUT = 1; //先拉高SCL
    delay_us(5);
    I2C_SDA_OUT = 1; //再拉高SDA
    delay_us(5);
}

/**
* @brief I2C总线写操作函数
* @detail 根据I2C通信协议的要求，写入一个字节
*
* @param dat 要写入的字节
* @return (!ack)
* @retval 0 不存在或忙或写入失败
*         1存在且空闲或写入成功
*/
uint8_t I2CWrite(uint8_t dat)
{
    int i;
    uint8_t ack;   //用于暂存应答位的值
    
    for (i=0; i<8; i++)  //循环将8bit数据输出到总线上
    {
        I2C_SDA_OUT = (dat&0x80) ? 1 : 0; //将最高位的值输出到SDA上
        delay_us(5);
        I2C_SCL_OUT = 1; //拉高SCL
        delay_us(5);
        I2C_SCL_OUT = 0; //再拉低SCL，完成一个位周期
        dat <<= 1;       //左移将次高位变为最高位，实现高位在先低位在后的发送顺序
    }
    I2C_SDA_OUT = 1;  //8位数据发送完后，主机释放SDA，以检测从机应答
    delay_us(5);
    I2C_SCL_OUT = 1;  //拉高SCL
    ack = I2C_SDA_IN; //读取此时的SDA值，即为从机的应答值
    delay_us(5);
    I2C_SCL_OUT = 0;  //再拉低SCL完成应答位，并保钳制总线        
    delay_us(5);
    
    return (!ack); 
}

/**
* @brief I2C总线读取8位数据
* @detail 根据I2C通信协议的要求，读取一个字节
*
* @param None
* @return dat
* @retval 读到的数据，一个字节
*/
uint8_t I2CRead(void)
{
    int i;
    uint8_t dat = 0; //数据接收变量赋初值0
    
    I2C_SDA_OUT = 1;    //首先确保主机释放SDA
    for (i=0; i<8; i++) //循环将总线上的8bit数据读入dat中
    {
        delay_us(5);
        I2C_SCL_OUT = 1;    //拉高SCL
        dat <<= 1;          //左移将己读到的位向高位移动，实现高位在先低位在后的接收顺序
        if(I2C_SDA_IN != 0) //读取SDA的值到dat最低位上
        {
            dat |= 0x01;    //SDA为1时设置dat最低位为1，SDA为0时无操作，即仍为初始值的0
        }
        delay_us(5);
        I2C_SCL_OUT = 0;    //再拉低SCL，以使从机发送出下一位
    }
    return dat;
}


/**
* @brief I2C总线读操作，并发送非应答信号
* @detail 根据I2C通信协议的要求，读取一个字节，并发送非应答信号
*
* @param None
* @return dat
* @retval 读到的数据，一个字节
*/
uint8_t I2CReadNAK(void)
{
    uint8_t dat;
    
    dat = I2CRead();  //读取8位数据
    I2C_SDA_OUT = 1;  //8位数据读取完后，拉高SDA，发送非应答信号
    delay_us(5);
    I2C_SCL_OUT = 1;  //拉高SCL
    delay_us(5);
    I2C_SCL_OUT = 0;  //再拉低SCL完成非应答位，并保持住总线
    delay_us(5);
    
    return dat;
}

/**
* @brief I2C总线读操作，并发送应答信号
* @detail 根据I2C通信协议的要求，读取一个字节，并发送应答信号
*
* @param None
* @return dat
* @retval 读到的数据，一个字节
*/
uint8_t I2CReadACK(void)
{
    uint8_t dat;
    
    dat = I2CRead();  //读取8位数据
    I2C_SDA_OUT = 0;  //8位数据读取完后，拉低SDA，发送应答信号
    delay_us(5);
    I2C_SCL_OUT = 1;  //拉高SCL
    delay_us(5);
    I2C_SCL_OUT = 0;  //再拉低SCL完成应答位，并保持住总线
    delay_us(5);
    
    return dat;
}
