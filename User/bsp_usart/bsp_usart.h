#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "misc.h"


//����
#define DEBUG_USARTx       	    USART1   //USART1��USART1_Typedef���͵�ָ�룬ָ���˸�������׵�ַ
#define DEBUG_USART_CLK    		RCC_APB2Periph_USART1//RCC_APB2Periph_USART1��RCC�Ĵ�����ĳ����Щ��λ
#define DEBUG_USART_APBxClkCmd  RCC_APB2PeriphClockCmd//��Ӧ����һ��������
#define DEBUG_USART_BAUDRATE    115200

// USART GPIO
#define DEBUG_USART_GPIO_CLK    (RCC_APB2Periph_GPIOA)
#define DEBUG_USART_GPIO_APBxClkCmd  RCC_APB2PeriphClockCmd

#define DEBUG_USART_TX_GPIO_PORT     GPIOA
#define DEBUG_USART_TX_GPIO_PIN      GPIO_Pin_9
#define DEBUG_USART_RX_GPIO_PORT     GPIOA
#define DEBUG_USART_RX_GPIO_PIN      GPIO_Pin_10

#define DEBUG_USART_IRQ              USART1_IRQn
#define DEBUG_USART_IRQHandler       USART1_IRQHandler

extern char recv_flag, recv_buff[10];      ///<���յ���λ������ı�־

void Usart_SendByte(USART_TypeDef * pUSARTx,uint8_t ch);
void UsartSendString(USART_TypeDef *pUSARTx,char *str);
void USART_Config (void);
void PC_CommandHandle(void);

#endif  /*__BSP_USART_H*/
