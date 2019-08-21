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
/*main文件重要的全局变量*/
extern uint32_t Pulse_num;//脉冲个数
extern _Pid Pid;
extern _Pid PidV_Buff;             //新建一个pid结构体,这里边保存着整套的适应于一种模式的参数
extern _Pid PidW_Buff;             //新建一个pid结构体,这里边保存着整套的适应于一种模式的参数
extern _dissbuff parambuff;
extern uint16_t x_1, x_2, y_1, y_2, backup;
extern float EE_Buff[4];

extern uint8_t mode;               ///<运行模式，0，1代表两种模式
extern uint8_t status_index;       ///<调参时与正常运行时的索引
extern uint8_t i, j, k, recv_flag;          ///<调参数时，缓冲区索引
extern uint8_t s;                  ///<用来记录键值
extern char recv_buff[10],tempbuff[10];         ///<串口接收缓存区
#endif /*_MAIN_C*/

extern volatile float Duty;        //占空比


float Pid_Cal(void);//可供外部调用
void Motor_Start(void);

#endif /*_MAIN_H*/
/***********************************THE END************************************/