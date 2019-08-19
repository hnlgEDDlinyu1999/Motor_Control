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
/*main文件重要的全局变量*/
//extern float V;           //实际线速度
//extern float SetV;        //目标速度
extern uint32_t Pulse_num;//脉冲个数
extern _Pid Pid;
extern _dissbuff parambuff;
extern uint16_t x_1, x_2, y_1, y_2, backup;

extern uint8_t mode;          ///<运行模式，0，1代表两种模式
extern uint8_t status_index;  ///<调参时与正常运行时的索引
extern uint8_t i;             ///<调参数时，缓冲区索引
extern uint8_t s;                 ///<用来记录键值
#endif /*_MAIN_C*/

extern volatile float Duty;      //占空比
float Pid_Cal();//可供外部调用
void Motor_Start();

////////////////////////////////////////////////////////////////////
/*显示与刷新相关的几个函数，暂时放在这里*/
void ParamBuff_Update(void);
void Param_Update(void);
void Param_Refresh(void);
void Param_NameShow(void);
void Param_DynaRefresh(void);
void Param_Change(void);    //根据模式，成套的更换参数
void Cusor_Show(void);
/////////////////////////////////////////////////////////////////
#endif /*_MAIN_H*/