/**
 * @file bsp_printf.c
 * @brief ������printf��������ʾ������Ϣ
 * @mainpage printf����ʵ��
 * @author hnlgEDDLinYu
 * @email 2970658553@qq.com
 * @version 1.0.0
 * @date 2019/8/17
 * @copyright    Copyright (c) hnlgEDDLinYu
 */
#include "./bsp_printf/bsp_printf.h"

#include "stdarg.h"
#include "./bsp_drawgraph/bsp_drawgraph.h"

/**
* @brief �˷�����
* @detail ��������������ĳ������ĳ�η����˴�������my_printf����ʹ��
*
* @param x ����
* @param y ����
* @return None
*/
static unsigned long my_pow(int x, int y)
{
    unsigned long sum = 1;
    while(y--) {
        sum *= x;
    }
    return sum;
}
/**
* @brief ������C�⺯��printf�ĸ�ʽ���������
* @detail �ú�������ض��Ŀ������д��ģ����C�⺯��printf�Ĺ��ܣ�����ʵ����������������
*          �ַ����ַ��������������Ϊ�˾��򣬲�֧�־���ָ�����Ѿ��������������
* @param X_Coordinate ָ����ʾλ�õ�X����
* @param Y_Coordinate ָ����ʾλ�õ�X����
* @param *str ��ʽ�����ַ���
* @param ... ���
* @return res
* @retval res���������Ļ�ϵ��ַ��ĸ�����\r��\n��ֻ����һ���ַ�
*/
int my_printf(uint16_t X_Coordinate, uint16_t Y_Coordinate, const char* str, ...)
{
    uint16_t x = X_Coordinate;      //����ָʾһ���������λ��
    uint16_t y = Y_Coordinate;      //����ָʾһ���������λ��

    uint16_t* X;                    //����ָʾһ���������λ��
    uint16_t* Y;                    //����ָʾһ���������λ��
    X = &x;
    Y = &y;

    va_list ap;
    int val_backup_int;       //�����õ��ı������ͣ�����ֵ������ת������
    int val_used_int;         //�����õ��ı������ͣ�����ֵ����̬��

    float val_backup_flt;     //�����õ��ı������ͣ�����ֵ������ת������
    float val_used_flt;       //�����õ��ı������ͣ�����ֵ����̬��

    char count = 0, ch;     //count���ڼ�������ַ��ĸ�����ch���м����
    char* s = " ";
    int res;                //�������printf���з���ֵ��


    va_start(ap, str);    //��ʼ��ap,��ʱapӦ��ָ�����б�ĵ�һ������

    while('\0' != *str) {
        switch(*str) {
        case '%':
            str++;
            switch(*str) {
            case 'd':
                val_backup_int = va_arg(ap, int);
                val_used_int = val_backup_int;
                if(val_used_int == 0) {
                    my_putchar(X, Y, '0');  //����ֻ������
                    //���
                    res += 1;
                } else {
                    if(val_used_int > 0) {                   //����
                        count = 0;
                        while(val_used_int) {
                            count++;                    //�����ĳ���
                            val_used_int /= 10;
                        }
                        res += count;
                        val_used_int = val_backup_int;
                        while(count) {
                            ch = val_used_int / my_pow(10, count - 1);
                            val_used_int %= my_pow(10, count - 1);
                            my_putchar(X, Y, ch + '0');
                            count--;
                        }
                    } else {
                        my_putchar(X, Y, '-');    //����
                        val_used_int = -val_backup_int;
                        count = 0;
                        while(val_used_int) {
                            count++;           //�����ĳ���
                            val_used_int /= 10;
                        }
                        res += count;
                        val_used_int = -val_backup_int;
                        while(count) {
                            ch = val_used_int / my_pow(10, count - 1);
                            val_used_int %= my_pow(10, count - 1);
                            my_putchar(X, Y, ch + '0');
                            count--;
                        }
                    }
                }
                break;


            case 'f':
                val_backup_flt  = va_arg(ap, float);
                val_used_flt = val_backup_flt;
                if(val_used_flt == 0) {
                    res += 1;
                    my_putchar(X, Y, '0');
                } else {
                    if(val_used_flt > 0) {
                        if(val_used_flt >= 1) { //����1���ֿ�Ϊ���������
                            val_used_int = (int)val_used_flt;
                            count = 0;
                            while(val_used_int) {
                                count++;
                                val_used_int /= 10;

                            }
                            res += count;
                            val_used_int = (int)val_used_flt;
                            while(count) {
                                ch = val_used_int / my_pow(10, count - 1);
                                val_used_int %= my_pow(10, count - 1);
                                my_putchar(X, Y, ch + '0');
                                count--;
                            }                 //���ˣ���������������
                            count = 1;
                            my_putchar(X, Y, '.');
                            val_used_flt = val_backup_flt;
                            while(count <= FLOATLEN) {
                                val_used_int = (int)(val_used_flt * my_pow(10, count));
                                ch = val_used_int % 10;
                                my_putchar(X, Y, ch + '0');
                                count++;
                                res += 1;
                            }

                        } else {  //С��1����ȫ��С��
                            val_used_flt = val_backup_flt;
                            my_putchar(X, Y, '0');
                            my_putchar(X, Y, '.');
                            res += 2;
                            count = 1;
                            while(count <= FLOATLEN) {
                                val_used_int = (int)(val_backup_flt * my_pow(10, count));
                                ch = val_used_int % 10;
                                my_putchar(X, Y, ch + '0');
                                count++;
                                res += 1;
                            }

                        }

                    } else {  //С��0
                        if(val_used_flt <= (-1)) { //
                            my_putchar(X, Y, '-');
                            res += 1;
                            val_used_flt = -val_backup_flt;
                            val_used_int = (int)val_used_flt;
                            while(val_used_int) {
                                count++;
                                val_used_int /= 10;
                            }
                            res += count;
                            val_used_int = ((int)(-val_backup_flt));
                            while(count) {
                                ch = val_used_int / my_pow(10, count - 1);
                                val_used_int %= my_pow(10, count - 1);
                                my_putchar(X, Y, ch + '0');
                                count--;
                            }                 //���ˣ���������������
                            count = 1;
                            my_putchar(X, Y, '.');
                            val_used_flt = -val_backup_flt;
                            while(count <= FLOATLEN) {
                                val_used_int = ((int)(val_used_flt * my_pow(10, count)));
                                ch = val_used_int % 10;
                                my_putchar(X, Y, ch + '0');
                                count++;
                                res += 1;
                            }
                        } else {  //��ȫ�Ǹ�С��
                            my_putchar(X, Y, '-');
                            my_putchar(X, Y, '0');
                            my_putchar(X, Y, '.');
                            res += 3;
                            val_used_flt = -val_backup_flt;
                            count = 1;
                            while(count <= FLOATLEN) {
                                val_used_int = ((int)(val_used_flt * my_pow(10, count)));
                                ch = val_used_int % 10;
                                my_putchar(X, Y, ch + '0');
                                count++;
                                res += 1;
                            }
                        }

                    }
                }
                break;      //���ˣ������õ��Ѿ���д���
            case 's':
                s = va_arg(ap, char*);
                my_putstr(X, Y, s);
                res += strlen(s);     //����ֵ��������
                break;
            case 'x':
                val_backup_int = va_arg(ap, int);
                val_used_int = val_backup_int;
                count = 0;
                while(val_used_int) {
                    count++;     //�����ĳ���
                    val_used_int /= 16;
                }
                res += count;
                val_used_int = val_backup_int;
                while(count) {
                    ch = val_used_int / my_pow(16, count - 1);
                    val_used_int %= my_pow(16, count - 1);
                    if(ch <= 9)
                        my_putchar(X, Y, ch + '0'); 	//���ֵ��ַ���ת��
                    else
                        my_putchar(X, Y, ch - 10 + 'a');
                    count--;
                }
                break;

            case 'c':
                my_putchar(X, Y, (char)va_arg(ap, int));
                res += 1;
                break;
            default :
                break;
            }
            break;
        case '\n':
            my_putchar(X, Y, '\n');
            res += 1;
            break;
        case '\r':
            my_putchar(X, Y, '\r');
            res += 1;
            break;
        default :
            my_putchar(X, Y, *str);
            res += 1;
        }
        str++;

    }
    va_end(ap);
    return res;
}

/**
* @brief ���ڽ�һ���ַ���ת��Ϊ��ֵ��֧�ָ�����
* @param *str ��Ҫ��ת�����ַ���
* @return s*flag
* @retval s����ֵ�ľ���ֵ��flag�Ƿ��ű�־��s*flag��Ϊת�����ֵ
*/
float my_atof(const char* str)
{
    float s = 0.0;
    float d = 10.0;
    float flag = 1.0;

    if(*str == '-') {
        flag = -1.0 ;
        str ++;
    }
    while(*str >= '0' && *str <= '9' && *str != '.') {
        s = s * 10.0 + *str - '0';
        str ++;
    }
    if(*str == '.')str ++;
    while(*str >= '0' && *str <= '9') {
        s = s + (*str - '0') / d;
        d *= 10.0;
        str++;
    }
    return s * flag;
}
/***********************************THE END************************************/

















































































