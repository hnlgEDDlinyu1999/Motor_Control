#include "./bsp_usart/bsp_usart.h"
#include "./bsp_printf/bsp_printf.h"
#include "stdio.h"
#include "./main.h"
#include "./bsp_param/bsp_param.h"

static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd =  ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}

void USART_Config (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK,ENABLE);
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN ;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN ;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT,&GPIO_InitStruct);
	
	USART_InitStruct.USART_BaudRate = DEBUG_USART_BAUDRATE;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b ;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_Mode = (USART_Mode_Rx|USART_Mode_Tx);
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init (DEBUG_USARTx,&USART_InitStruct);
	
	NVIC_Configuration();
	
	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);
	USART_Cmd(DEBUG_USARTx,ENABLE);
	
}
void PC_CommandHandle(void)
{
	/*先更新数据到对应的缓存区*/
	if (!mode){
		switch(recv_buff[0]) {
    case 's':
        PidV_Buff.SetSpeed = my_atof((recv_buff+2));
        break;
    case 'p':
        PidV_Buff.Kp = my_atof((recv_buff+2));
        break;
    case 'i':
        PidV_Buff.Ki = my_atof((recv_buff+2));
        break;
    case 'd':
        PidV_Buff.Kd = my_atof((recv_buff+2));
        break;
    default:
        break;
    }	
	}else{
		switch(recv_buff[0]) {
    case 's':
        PidW_Buff.SetSpeed = my_atof((recv_buff+2));
        break;
    case 'p':
        PidW_Buff.Kp = my_atof((recv_buff+2));
        break;
    case 'i':
        PidW_Buff.Ki = my_atof((recv_buff+2));
        break;
    case 'd':
        PidW_Buff.Kd = my_atof((recv_buff+2));
        break;
    default:
        break;
    }	
	}
    Param_Update();
	Param_Refresh();
}

