#ifndef _BSP_PARAM_H
#define _BSP_PARAM_H

#include "stm32f10x.h"

void ParamBuff_Update(void);    //���������ݸ��µ�pidbuff
void Param_Update(void);
void Param_Refresh(void);      //�����������ʾ����Ļ,Ҫд��handle֮ǰ
void Param_NameShow(void);
void Param_DynaRefresh(void);     //�˺���Ӧ��ֻ�ڵ���״̬�¹��� if����status_index)
void Cusor_Show(void);      //�˺�������ָʾ�������ĸ�����,���ж����ˢ��
void Motor_Start(void);
void Data_BackUp(void);
void Data_Fetch(void);
void Param_EE_Update(void);
#endif /*_BSP_PARAM_H*/
/***********************************THE END************************************/