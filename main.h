#ifndef _MAIN_H
#define _MAIN_H

typedef struct {
    char Mode_Disbuff[2];
    char Kp_Disbuff[10];
    char Ki_Disbuff[10];
    char Kd_Disbuff[10];
    char V_Disbuff[10];
    char SetV_Disbuff[10];
} _dissbuff;

typedef struct {
    volatile float SetSpeed;
    volatile float ActualSpeed;
    volatile float Err;
    volatile float Err1;
    volatile float Err2;
    volatile float Kp, Ki, Kd;
} _Pid;


#ifndef _MAIN_C
/*main�ļ���Ҫ��ȫ�ֱ���*/
//extern float V;           //ʵ�����ٶ�
//extern float SetV;        //Ŀ���ٶ�
extern uint32_t Pulse_num;//�������
extern _Pid Pid;
extern _dissbuff parambuff;
extern uint16_t x_1, x_2, y_1, y_2, backup;

extern uint8_t mode;          ///<����ģʽ��0��1��������ģʽ
extern uint8_t status_index;  ///<����ʱ����������ʱ������
extern uint8_t i;             ///<������ʱ������������
extern uint8_t s;                 ///<������¼��ֵ
#endif /*_MAIN_C*/

extern volatile float Duty;      //ռ�ձ�
float Pid_Cal();//�ɹ��ⲿ����
void Motor_Start();

////////////////////////////////////////////////////////////////////
/*��ʾ��ˢ����صļ�����������ʱ��������*/
void ParamBuff_Update(void);
void Param_Update(void);
void Param_Refresh(void);
void Param_NameShow(void);
void Param_DynaRefresh(void);
void Param_Change(void);    //����ģʽ�����׵ĸ�������
void Cusor_Show(void);
/////////////////////////////////////////////////////////////////
#endif /*_MAIN_H*/