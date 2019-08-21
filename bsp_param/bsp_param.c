/**
 * @file bsp_param.c
 * @brief 在调参的过程中会有几个动作，比如更新参数刷新显示等，由于功能
 *        相对集中，不便放在其他文件当中，因而单独形成一个文件
 * @mainpage 调参刷新动作
 * @author hnlgEDDLinYu
 * @email 2970658553@qq.com
 * @version 1.0.0
 * @date 2019/8/19
 * @copyright    Copyright (c) hnlgEDDLinYu
 */
#include "./bsp_param/bsp_param.h"
#include "./main.h"
#include "./bsp_printf/bsp_printf.h"
#include "./bsp_pwm/bsp_pwm.h"
#include "./bsp_i2c_ee/bsp_i2c_ee.h"

/**
* @brief 按下确定键之后更新缓冲区的数据到pidbuff
* @detail 再调参的过程中，并不会将输入的值立即更新到参数，而是放在缓冲区里
*         当按下确定键之后会更新到参数
* @param None
* @return None
*/
void ParamBuff_Update(void)    //仅仅将数据更新到pidbuff
{
    switch(status_index) {
    case 1:
        mode = ((uint8_t)(my_atof(parambuff.Mode_Disbuff)));
        break;
    default:
        break;
    }
    if(!mode) {
        switch(status_index) {
        case 2:
            PidV_Buff.SetSpeed = my_atof(parambuff.SetV_Disbuff);
            break;
        case 3:
            PidV_Buff.Kp = my_atof(parambuff.Kp_Disbuff);
            break;
        case 4:
            PidV_Buff.Ki = my_atof(parambuff.Ki_Disbuff);
            break;
        case 5:
            PidV_Buff.Kd = my_atof(parambuff.Kd_Disbuff);
            break;
        default:
            break;
        }
    } else {
        switch(status_index) {
        case 2:
            PidW_Buff.SetSpeed = my_atof(parambuff.SetV_Disbuff);
            break;
        case 3:
            PidW_Buff.Kp = my_atof(parambuff.Kp_Disbuff);
            break;
        case 4:
            PidW_Buff.Ki = my_atof(parambuff.Ki_Disbuff);
            break;
        case 5:
            PidW_Buff.Kd = my_atof(parambuff.Kd_Disbuff);
            break;
        default:
            break;
        }
    }
}
/**
* @brief 按下确定键之后更新pidbuff的数据到参数
* @detail 当按下确定键之后会更新到参数
*    当
* @param None
* @return None
*/
void  Param_Update(void)
{
    if(!mode) {
        Pid.SetSpeed = PidV_Buff.SetSpeed;
        Pid.Kp = PidV_Buff.Kp;
        Pid.Ki = PidV_Buff.Ki;
        Pid.Kd = PidV_Buff.Kd;
    } else {
        Pid.SetSpeed = PidW_Buff.SetSpeed;
        Pid.Kp = PidW_Buff.Kp;
        Pid.Ki = PidW_Buff.Ki;
        Pid.Kd = PidW_Buff.Kd;
    }
}
/**
* @brief 各项参数显示刷新
* @param None
* @return None
*/
void Param_Refresh(void)      //将各项参数显示到屏幕,要写在handle之前
{
    my_printf(48, 16, "%d", mode);
    my_printf(168, 16, "%f", Pid.SetSpeed);
    my_printf(24, 32, "%f", Pid.Kp);
    my_printf(104, 32, "%f", Pid.Ki);
    my_printf(184, 32, "%f", Pid.Kd);
}
/**
* @brief 显示各项参数的名字信息到屏幕
* @param None
* @return None
*/
void Param_NameShow(void)
{
    my_printf(8,   0, "value:");
    my_printf(128, 0, "duty:");
    my_printf(8,  16, "mode:");
    my_printf(128, 16, "goal:");
    my_printf(8,  32, "P:");
    my_printf(88, 32, "I:");
    my_printf(168, 32, "D:");
}
/**
* @brief 调参过程中各项参数动态刷新
* @detail 在调参过程中，每按一次按键屏幕都会实时更新所输入的数字，更加人性化
* @param None
* @return None
*/
void Param_DynaRefresh(void)     //此函数应当只在调参状态下工作 if（！status_index)
{
    switch(status_index) {
    case 1:
        my_printf(48, 16, "%f", my_atof(parambuff.Mode_Disbuff));
        break;
    case 2:
        my_printf(168, 16, "%f", my_atof(parambuff.SetV_Disbuff));
        break;
    case 3:
        my_printf(24, 32, "%f", my_atof(parambuff.Kp_Disbuff));
        break;
    case 4:
        my_printf(104, 32, "%f", my_atof(parambuff.Ki_Disbuff));
        break;
    case 5:
        my_printf(184, 32, "%f", my_atof(parambuff.Kd_Disbuff));
        break;
    default :
        break;
    }

}
/**
* @brief 光标指示所调的具体是哪一个参数
* @detail 用*符号来标示所调的具体是哪一个参数
*
* @param None
* @return None
*/
void Cusor_Show(void)      //此函数用于指示调的是哪个参数,在中断里边刷新
{
    switch(status_index) {
    case 1:
        my_printf(0, 16, "%c", '*');
        my_printf(120, 16, "%c", ' ');
        my_printf(0, 32, "%c", ' ');
        my_printf(80, 32, "%c", ' ');
        my_printf(160, 32, "%c", ' ');
        break;
    case 2:
        my_printf(0, 16, "%c", ' ');
        my_printf(120, 16, "%c", '*');
        my_printf(0, 32, "%c", ' ');
        my_printf(80, 32, "%c", ' ');
        my_printf(160, 32, "%c", ' ');
        break;
    case 3:
        my_printf(0, 16, "%c", ' ');
        my_printf(120, 16, "%c", ' ');
        my_printf(0, 32, "%c", '*');
        my_printf(80, 32, "%c", ' ');
        my_printf(160, 32, "%c", ' ');
        break;
    case 4:
        my_printf(0, 16, "%c", ' ');
        my_printf(120, 16, "%c", ' ');
        my_printf(0, 32, "%c", ' ');
        my_printf(80, 32, "%c", '*');
        my_printf(160, 32, "%c", ' ');
        break;
    case 5:
        my_printf(0, 16, "%c", ' ');
        my_printf(120, 16, "%c", ' ');
        my_printf(0, 32, "%c", ' ');
        my_printf(80, 32, "%c", ' ');
        my_printf(160, 32, "%c", '*');
        break;
    case 0:
        my_printf(0, 16, "%c", ' ');
        my_printf(120, 16, "%c", ' ');
        my_printf(0, 32, "%c", ' ');
        my_printf(80, 32, "%c", ' ');
        my_printf(160, 32, "%c", ' ');
        break;
    default :
        break;
    }
}
/**
* @brief 根据mode所指示的模式将对应的整套参数存储到EEPROM
* @detail 只有在改参动作发生后，该函数才会被调用
*
* @param None
* @return None
*/
void Data_BackUp(void)
{
    if(!mode) {
        EE_Buff[0] = PidV_Buff.SetSpeed;
        EE_Buff[1] = PidV_Buff.Kp;
        EE_Buff[2] = PidV_Buff.Ki;
        EE_Buff[3] = PidV_Buff.Kd;
        I2C_EE_BufferWrite((void*)EE_Buff, PIDV_BUFF_ADDR, sizeof(EE_Buff));
    } else {
        EE_Buff[0] = PidW_Buff.SetSpeed;
        EE_Buff[1] = PidW_Buff.Kp;
        EE_Buff[2] = PidW_Buff.Ki;
        EE_Buff[3] = PidW_Buff.Kd;
        I2C_EE_BufferWrite((void*)EE_Buff, PIDW_BUFF_ADDR, sizeof(EE_Buff));
    }
}
/**
* @brief 根据mode所指示的模式将对应的整套参数从EEPROM里边取出
* @detail 只有在上电初始化准备时，该函数才会被调用
*
* @param None
* @return None
*/
void Data_Fetch(void)
{
    if(!mode) {
        I2C_EE_BufferRead((void*)EE_Buff, PIDV_BUFF_ADDR, sizeof(EE_Buff));
        PidV_Buff.SetSpeed = EE_Buff[0];
        PidV_Buff.Kp = EE_Buff[1];
        PidV_Buff.Ki = EE_Buff[2];
        PidV_Buff.Kd = EE_Buff[3];
    } else {
        I2C_EE_BufferRead((void*)EE_Buff, PIDW_BUFF_ADDR, sizeof(EE_Buff));
        PidW_Buff.SetSpeed = EE_Buff[0];
        PidW_Buff.Kp = EE_Buff[1];
        PidW_Buff.Ki = EE_Buff[2];
        PidW_Buff.Kd = EE_Buff[3];
    }
}
/**
* @brief 与EEPROM相关的参数更新动作
* @param None
* @return None
*/
void Param_EE_Update(void)
{
    Data_Fetch();
    Param_Update();
    Param_Refresh();
}
/**
* @brief 电机启动，很简单
* @param None
* @return None
*/
void Motor_Start()
{
    GPIO_SetBits(GPIOB, GPIO_Pin_6);
    PWM_SetDuty(50);
}
/***********************************THE END************************************/







































