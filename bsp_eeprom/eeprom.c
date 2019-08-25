/**
 * @file eeprom.c
 * @brief  eeprom��д�����ļ�
 * @mainpage EEPROM��д
 * @author hnlgEDDLinYu
 * @email 2970658553@qq.com
 * @version 1.0.0
 * @date 2019/8/22
 * @copyright    Copyright (c) hnlgEDDLinYu
 */
#include "./bsp_eeprom/eeprom.h"

/**
* @brief ��ȡE2�е�һ���ֽ�
* @detail ����EEPROM�����ֲ��ʱ��Ҫ�󣬶�ȡһ���ֽ�
*
* @param addr �ֽڵ�ַ
* @return None
*/
uint8_t E2ReadByte(uint8_t addr)
{
    uint8_t dat;
    
    do { //��Ѱַ������ѯ��ǰ�Ƿ�ɽ��ж�д
        I2CStart();
        if (I2CWrite(0x50<<1)) //Ѱַ������Ӧ��������ѭ�������������ѯ
        {
            break;
        }
        I2CStop();
    } while(1);
    I2CWrite(addr);           //д��洢��ַ
    I2CStart();               //�����ظ������ź�
    I2CWrite((0x50<<1)|0x01); //Ѱַ����������Ϊ������
    dat = I2CReadNAK();       //��ȡһ���ֽ�����
    I2CStop();
    
    return dat;
}

/**
* @brief д��E2�е�һ���ֽ�
* @detail ����EEPROM�����ֲ��ʱ��Ҫ��д��һ���ֽ�
*
* @param addr �ֽڵ�ַ
* @return None
*/
void E2WriteByte(uint8_t addr, uint8_t dat)
{
    do { //��Ѱַ������ѯ��ǰ�Ƿ�ɽ��ж�д
        I2CStart();
        if (I2CWrite(0x50<<1)) //Ѱַ������Ӧ��������ѭ�������������ѯ
        {
            break;
        }
        I2CStop();
    } while(1);
    I2CWrite(addr);  //д��洢��ַ
    I2CWrite(dat);   //д��һ���ֽ�����
    I2CStop();
}

/* E2������ȡ������addr-E2�е���ʼ��ַ��buf-���ݽ���ָ�룬len-��ȡ���� */
/**
* @brief ������ȡ����
* @detail ����EEPROM�����ֲ��ʱ��Ҫ��һ���Զ�ȡ����ֽ�
*
* @param addr E2�е���ʼ��ַ
* @param *buf ���ݽ���ָ��
* @param len ��ȡ���� 
* @return None
*/
void E2Read(uint8_t *buf, uint8_t addr, int len)
{
    do { //��Ѱַ������ѯ��ǰ�Ƿ�ɽ��ж�д
        I2CStart();
        if (I2CWrite(0x50<<1)) //Ѱַ������Ӧ��������ѭ�������������ѯ
        {
            break;
        }
        I2CStop();
    } while(1);
    I2CWrite(addr);            //д����ʼ��ַ
    I2CStart();                //�����ظ������ź�
    I2CWrite((0x50<<1)|0x01);  //Ѱַ����������Ϊ������
    while (len > 1)            //������ȡlen-1���ֽ�
    {
        *buf++ = I2CReadACK(); //����ֽ�֮ǰΪ��ȡ����+Ӧ��
        len--;
    }
    *buf = I2CReadNAK();       //���һ���ֽ�Ϊ��ȡ����+��Ӧ��
    I2CStop();
}


/**
* @brief ����д�뺯��
* @detail ����EEPROM�����ֲ��ʱ��Ҫ��һ����д�����ֽ�
*
* @param addr E2�е���ʼ��ַ
* @param *buf Դ����ָ��
* @param len ��ȡ���� 
* @return None
*/
void E2Write(const uint8_t *buf, uint8_t addr, int len)
{
    while (len > 0)
    {
        do { //��Ѱַ������ѯ��ǰ�Ƿ�ɽ��ж�д
            I2CStart();
            if (I2CWrite(0x50<<1)) //Ѱַ������Ӧ��������ѭ�������������ѯ
            {
                break;
            }
            I2CStop();
        } while(1);
        //��ҳдģʽ����д���ֽ�
        I2CWrite(addr);        //д����ʼ��ַ
        while (len > 0)
        {
            I2CWrite(*buf++);  //д��һ���ֽ�����
            len--;             //��д�볤�ȼ����ݼ�
            addr++;            //E2��ַ����
            if ((addr%8) == 0) //����ַ�Ƿ񵽴�ҳ�߽磬24C02ÿҳ8�ֽڣ�
            {                  //���Լ���3λ�Ƿ�Ϊ�㼴��
                break;         //����ҳ�߽�ʱ������ѭ������������д����
            }
        }
        I2CStop();
    }
}
