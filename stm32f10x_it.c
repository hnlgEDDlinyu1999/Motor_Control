/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "./main.h"
#include "./bsp_PulseCap_EXTI/bsp_PulseCap_EXTI.h"
#include "./bsp_basetim/bsp_basetim.h"
#include "./bsp_DisplayTIM/bsp_DisplayTIM.h"
#include "./bsp_pwm/bsp_pwm.h"
#include "./bsp_drawgraph/bsp_drawgraph.h"
#include "stdio.h"
#include "./bsp_led/bsp_led.h"
#include "./bsp_printf/bsp_printf.h"
#include "./bsp_key/bsp_key.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/*以下为自己配置的中断服务函数*/
void PulseCap_IRQHandler()
{
	if (EXTI_GetITStatus(PulseCap_EXTI_LINE)!=RESET)
	{
		/*外部中断只管脉冲计数*/
		Pulse_num++;
		EXTI_ClearITPendingBit(PulseCap_EXTI_LINE);
	}
}
void BASIC_TIM_IRQHandler(void)
{
	if (TIM_GetITStatus(BASE_TIMx,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(BASE_TIMx,TIM_FLAG_Update);
		/*进入中断，先计算实际速度*/
		Pid.ActualSpeed = Pulse_num/0.668;
		Pulse_num = 0;       //将脉冲数清零，下一轮计数自动开始
		/*进行PID调整*/
		Duty += Pid_Cal();

//		if(Duty<101)
//		{
			PWM_SetDuty(Duty);//
//		}
		
	}
	
}
void DISPLAYTIM_IRQHandler(void)
{
	if (TIM_GetITStatus(DISPLAYTIM,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(DISPLAYTIM,TIM_FLAG_Update);
		my_printf(56,0,"%f",Pid.ActualSpeed);
		my_printf(168,0,"%f",Duty);
		if (status_index)Param_DynaRefresh();
		DrawGraph();
		LED_G_TOGGLE;
	}
}

void KEYPAD4x4_INTHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line12)!=RESET){//判断某个线上的中断是否发生 
		key_int_flag=1;//标志位置1，表示有按键中断
		EXTI_ClearITPendingBit(EXTI_Line12);   //清除 LINE 上的中断标志位
	}     
	if(EXTI_GetITStatus(EXTI_Line13)!=RESET){//判断某个线上的中断是否发生 
		key_int_flag=2;//标志位置1，表示有按键中断
		EXTI_ClearITPendingBit(EXTI_Line13);   //清除 LINE 上的中断标志位
	}     
	if(EXTI_GetITStatus(EXTI_Line14)!=RESET){//判断某个线上的中断是否发生 
		key_int_flag=3;//标志位置1，表示有按键中断
		EXTI_ClearITPendingBit(EXTI_Line14);   //清除 LINE 上的中断标志位
	}     
	if(EXTI_GetITStatus(EXTI_Line15)!=RESET){//判断某个线上的中断是否发生 
		key_int_flag=4;//标志位置1，表示有按键中断
		EXTI_ClearITPendingBit(EXTI_Line15);   //清除 LINE 上的中断标志位
	}     
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
