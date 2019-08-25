/*ѧ�������ڵ�stm32��i2c��дeeprom������һϵ�е����⣬����ʹ�����i2c.
���׳�����ֲ��51��Ƭ��ƽ̨�����i2cʹ��λ����������Ӳ�����ŵ����ƣ�
��׼����һ�����ӣ����ע������ָ���Ŀ�����д�ģ����Լ���*/
/**
 * @file i2c.c
 * @brief ���ģ��icͨ�ţ�����ʹ���κ�����GPIO�ڣ����ǣ�Ҫ�������ų�ʼ��������ICInit��void����������
 *		  �ù��ܵ�ʵ���õ�ȫ��λ��������һ��Ҫ����config.h����������λ��������׼������
 *        ������ʱ�����ṩ���¸�ʽ�Ľӿں�����delay_us(uint32_t)��
 *        ���ߣ���i2cͨ��Э��ɼ����κ��ٶȵ�����������ٶ�������100Kb/s
 *        ����������ʣ������и���i2c��ʱʱ�䡣
 * @mainpage ���ģ��ICͨ��
 * @author hnlgEDDLinYu
 * @email 2970658553@qq.com
 * @version 1.0.0
 * @date 2019/8/22
 * @copyright    Copyright (c) hnlgEDDLinYu
 */
#include "./bsp_I2c/i2c.h"



/**
* @brief I2C���ų�ʼ������
* @detail ���ڳ�ʼ��I2Cͨ����ʹ�õ���������
*
* @param None
* @return None
*/
void I2CInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //ʹ��GPIOBʱ��       
    GPIO_SetBits(GPIOC, GPIO_Pin_2|GPIO_Pin_3);           //SCL��SDA��ʼ����ߵ�ƽ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;  //ѡ��SCL��SDA����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;      //ѡ��©���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;     //�������10MHz��I2C�㹻�ˣ�
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}
/**
* @brief ������ʼ�źŲ�������
* @detail ����I2Cͨ��Э���Ҫ�󣬲���һ����ʼ�ź�
*
* @param None
* @return None
*/
void I2CStart(void)
{
    I2C_SDA_OUT = 1; //����ȷ��SDA��SCL���Ǹߵ�ƽ
    I2C_SCL_OUT = 1;
    delay_us(5);
    I2C_SDA_OUT = 0; //������SDA
    delay_us(5);
    I2C_SCL_OUT = 0; //������SCL
}

/**
* @brief ����ֹͣ�źŲ�������
* @detail ����I2Cͨ��Э���Ҫ�󣬲���һ��ֹͣ�ź�
*
* @param None
* @return None
*/
void I2CStop(void)
{
    I2C_SCL_OUT = 0; //����ȷ��SDA��SCL���ǵ͵�ƽ
    I2C_SDA_OUT = 0;
    delay_us(5);
    I2C_SCL_OUT = 1; //������SCL
    delay_us(5);
    I2C_SDA_OUT = 1; //������SDA
    delay_us(5);
}

/**
* @brief I2C����д��������
* @detail ����I2Cͨ��Э���Ҫ��д��һ���ֽ�
*
* @param dat Ҫд����ֽ�
* @return (!ack)
* @retval 0 �����ڻ�æ��д��ʧ��
*         1�����ҿ��л�д��ɹ�
*/
uint8_t I2CWrite(uint8_t dat)
{
    int i;
    uint8_t ack;   //�����ݴ�Ӧ��λ��ֵ
    
    for (i=0; i<8; i++)  //ѭ����8bit���������������
    {
        I2C_SDA_OUT = (dat&0x80) ? 1 : 0; //�����λ��ֵ�����SDA��
        delay_us(5);
        I2C_SCL_OUT = 1; //����SCL
        delay_us(5);
        I2C_SCL_OUT = 0; //������SCL�����һ��λ����
        dat <<= 1;       //���ƽ��θ�λ��Ϊ���λ��ʵ�ָ�λ���ȵ�λ�ں�ķ���˳��
    }
    I2C_SDA_OUT = 1;  //8λ���ݷ�����������ͷ�SDA���Լ��ӻ�Ӧ��
    delay_us(5);
    I2C_SCL_OUT = 1;  //����SCL
    ack = I2C_SDA_IN; //��ȡ��ʱ��SDAֵ����Ϊ�ӻ���Ӧ��ֵ
    delay_us(5);
    I2C_SCL_OUT = 0;  //������SCL���Ӧ��λ������ǯ������        
    delay_us(5);
    
    return (!ack); 
}

/**
* @brief I2C���߶�ȡ8λ����
* @detail ����I2Cͨ��Э���Ҫ�󣬶�ȡһ���ֽ�
*
* @param None
* @return dat
* @retval ���������ݣ�һ���ֽ�
*/
uint8_t I2CRead(void)
{
    int i;
    uint8_t dat = 0; //���ݽ��ձ�������ֵ0
    
    I2C_SDA_OUT = 1;    //����ȷ�������ͷ�SDA
    for (i=0; i<8; i++) //ѭ���������ϵ�8bit���ݶ���dat��
    {
        delay_us(5);
        I2C_SCL_OUT = 1;    //����SCL
        dat <<= 1;          //���ƽ���������λ���λ�ƶ���ʵ�ָ�λ���ȵ�λ�ں�Ľ���˳��
        if(I2C_SDA_IN != 0) //��ȡSDA��ֵ��dat���λ��
        {
            dat |= 0x01;    //SDAΪ1ʱ����dat���λΪ1��SDAΪ0ʱ�޲���������Ϊ��ʼֵ��0
        }
        delay_us(5);
        I2C_SCL_OUT = 0;    //������SCL����ʹ�ӻ����ͳ���һλ
    }
    return dat;
}


/**
* @brief I2C���߶������������ͷ�Ӧ���ź�
* @detail ����I2Cͨ��Э���Ҫ�󣬶�ȡһ���ֽڣ������ͷ�Ӧ���ź�
*
* @param None
* @return dat
* @retval ���������ݣ�һ���ֽ�
*/
uint8_t I2CReadNAK(void)
{
    uint8_t dat;
    
    dat = I2CRead();  //��ȡ8λ����
    I2C_SDA_OUT = 1;  //8λ���ݶ�ȡ�������SDA�����ͷ�Ӧ���ź�
    delay_us(5);
    I2C_SCL_OUT = 1;  //����SCL
    delay_us(5);
    I2C_SCL_OUT = 0;  //������SCL��ɷ�Ӧ��λ��������ס����
    delay_us(5);
    
    return dat;
}

/**
* @brief I2C���߶�������������Ӧ���ź�
* @detail ����I2Cͨ��Э���Ҫ�󣬶�ȡһ���ֽڣ�������Ӧ���ź�
*
* @param None
* @return dat
* @retval ���������ݣ�һ���ֽ�
*/
uint8_t I2CReadACK(void)
{
    uint8_t dat;
    
    dat = I2CRead();  //��ȡ8λ����
    I2C_SDA_OUT = 0;  //8λ���ݶ�ȡ�������SDA������Ӧ���ź�
    delay_us(5);
    I2C_SCL_OUT = 1;  //����SCL
    delay_us(5);
    I2C_SCL_OUT = 0;  //������SCL���Ӧ��λ��������ס����
    delay_us(5);
    
    return dat;
}
