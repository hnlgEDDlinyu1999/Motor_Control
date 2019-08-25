/**
 * @file bsp_printf.c
 * @brief 类似于printf，用于显示参数信息
 * @mainpage printf函数实现
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
* @brief 乘方函数
* @detail 这个函数方便计算某个数的某次方，此处仅限于my_printf（）使用
*
* @param x 底数
* @param y 次数
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
* @brief 类似于C库函数printf的格式化输出函数
* @detail 该函数针对特定的开发板而写，模拟了C库函数printf的功能，可以实现整数、浮点数、
*          字符，字符串的输出，但是为了精简，不支持精度指定，已经可满足调试需求
* @param X_Coordinate 指定显示位置的X坐标
* @param Y_Coordinate 指定显示位置的X坐标
* @param *str 格式控制字符串
* @param ... 变参
* @return res
* @retval res是输出到屏幕上的字符的个数，\r与\n都只算做一个字符
*/
int my_printf(uint16_t X_Coordinate, uint16_t Y_Coordinate, const char* str, ...)
{
    uint16_t x = X_Coordinate;      //用于指示一连串输出的位置
    uint16_t y = Y_Coordinate;      //用于指示一连串输出的位置

    uint16_t* X;                    //用于指示一连串输出的位置
    uint16_t* Y;                    //用于指示一连串输出的位置
    X = &x;
    Y = &y;

    va_list ap;
    int val_backup_int;       //可能用到的变量类型，备份值，用于转换过程
    int val_used_int;         //可能用到的变量类型，常用值（动态）

    float val_backup_flt;     //可能用到的变量类型，备份值，用于转换过程
    float val_used_flt;       //可能用到的变量类型，常用值（动态）

    char count = 0, ch;     //count用于计算输出字符的个数，ch是中间变量
    char* s = " ";
    int res;                //正常情况printf是有返回值的


    va_start(ap, str);    //初始化ap,此时ap应当指向变参列表的第一个变量

    while('\0' != *str) {
        switch(*str) {
        case '%':
            str++;
            switch(*str) {
            case 'd':
                val_backup_int = va_arg(ap, int);
                val_used_int = val_backup_int;
                if(val_used_int == 0) {
                    my_putchar(X, Y, '0');  //整数只有三种
                    //情况
                    res += 1;
                } else {
                    if(val_used_int > 0) {                   //正数
                        count = 0;
                        while(val_used_int) {
                            count++;                    //整数的长度
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
                        my_putchar(X, Y, '-');    //负数
                        val_used_int = -val_backup_int;
                        count = 0;
                        while(val_used_int) {
                            count++;           //整数的长度
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
                        if(val_used_flt >= 1) { //大于1，分开为两部分输出
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
                            }                 //至此，整数部分输出完毕
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

                        } else {  //小于1，完全是小数
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

                    } else {  //小于0
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
                            }                 //至此，整数部分输出完毕
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
                        } else {  //完全是负小数
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
                break;      //至此，最有用的已经编写完毕
            case 's':
                s = va_arg(ap, char*);
                my_putstr(X, Y, s);
                res += strlen(s);     //返回值长度增加
                break;
            case 'x':
                val_backup_int = va_arg(ap, int);
                val_used_int = val_backup_int;
                count = 0;
                while(val_used_int) {
                    count++;     //整数的长度
                    val_used_int /= 16;
                }
                res += count;
                val_used_int = val_backup_int;
                while(count) {
                    ch = val_used_int / my_pow(16, count - 1);
                    val_used_int %= my_pow(16, count - 1);
                    if(ch <= 9)
                        my_putchar(X, Y, ch + '0'); 	//数字到字符的转换
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
* @brief 用于将一个字符串转化为数值，支持浮点数
* @param *str 将要被转化的字符串
* @return s*flag
* @retval s是数值的绝对值，flag是符号标志，s*flag即为转换后的值
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

















































































