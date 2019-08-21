#ifndef _MAIN_H
#define _MAIN_H

#define PIDV_BUFF_ADDR      10
#define PIDW_BUFF_ADDR      60

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
extern uint32_t Pulse_num;//�������
extern _Pid Pid;
extern _Pid PidV_Buff;             //�½�һ��pid�ṹ��,����߱��������׵���Ӧ��һ��ģʽ�Ĳ���
extern _Pid PidW_Buff;             //�½�һ��pid�ṹ��,����߱��������׵���Ӧ��һ��ģʽ�Ĳ���
extern _dissbuff parambuff;
extern uint16_t x_1, x_2, y_1, y_2, backup;
extern float EE_Buff[4];

extern uint8_t mode;               ///<����ģʽ��0��1��������ģʽ
extern uint8_t status_index;       ///<����ʱ����������ʱ������
extern uint8_t i, j, k, recv_flag;          ///<������ʱ������������
extern uint8_t s;                  ///<������¼��ֵ
extern char recv_buff[10],tempbuff[10];         ///<���ڽ��ջ�����
#endif /*_MAIN_C*/

extern volatile float Duty;        //ռ�ձ�


float Pid_Cal(void);//�ɹ��ⲿ����
void Motor_Start(void);

#endif /*_MAIN_H*/
/***********************************THE END************************************/