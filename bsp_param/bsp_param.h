#ifndef _BSP_PARAM_H
#define _BSP_PARAM_H

#include "stm32f10x.h"

void ParamBuff_Update(void);    //仅仅将数据更新到pidbuff
void Param_Update(void);
void Param_Refresh(void);      //将各项参数显示到屏幕,要写在handle之前
void Param_NameShow(void);
void Param_DynaRefresh(void);     //此函数应当只在调参状态下工作 if（！status_index)
void Cusor_Show(void);      //此函数用于指示调的是哪个参数,在中断里边刷新
void Motor_Start(void);
void Data_BackUp(void);
void Data_Fetch(void);
void Param_EE_Update(void);
#endif /*_BSP_PARAM_H*/
/***********************************THE END************************************/