#ifndef _BSP_PRINTF_H
#define _BSP_PRINTF_H
#include "./bsp_printf/bsp_printf.h"
#include "stdarg.h"
#include "string.h"
#include "./bsp_drawgraph/bsp_drawgraph.h"

/*宏定义：一个字符的宽度和高度*/
#define WORD_WITH             8
#define WORD_HIGH             16

#define FLOATLEN   4


int my_printf(uint16_t X_Coordinate, uint16_t Y_Coordinate, const char* str, ...);
float my_atof(const char* str);
#endif /*_BSP_PRINTF_H*/