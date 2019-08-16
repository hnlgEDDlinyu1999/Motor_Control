#ifndef _MAIN_H
#define _MAIN_H

typedef struct{
	char Mode_Disbuff[2];
	char Kp_Disbuff[10];
	char Ki_Disbuff[10];
	char Kd_Disbuff[10];
	char V_Disbuff[10];
	char SetV_Disbuff[10];
} _dissbuff;

typedef struct
{
	float SetSpeed;
	float ActualSpeed;
	float Err;
	float Err1;
	float Err2;
	float Kp,Ki,Kd;
} _Pid;
	

#ifndef _MAIN_C
/*main文件重要的全局变量*/
extern float V;           //实际线速度
extern float SetV;        //目标速度
extern uint32_t Pulse_num;//脉冲个数
extern _Pid Pid;
extern _dissbuff dissbuff;
extern uint16_t x_1,x_2,y_1,y_2,backup;
#endif /*_MAIN_C*/

extern float Duty;      //占空比
float Pid_Cal(float setspeed);//可供外部调用


#endif /*_MAIN_H*/