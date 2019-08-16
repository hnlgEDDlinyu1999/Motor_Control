#ifndef _BSP_PULSECAP_EXTI_H
#define _BSP_PULSECAP_EXTI_H
#include "stm32f10x.h"

#define PulseCap_GPIO_PORT       GPIOA
#define PulseCap_GPIO_PIN        GPIO_Pin_2
#define PulseCap_GPIO_CLK        (RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA)

#define PulseCap_EXTI_PORTSOURCE GPIO_PortSourceGPIOA
#define PulseCap_EXTI_PINSOURCE  GPIO_PinSource2
#define PulseCap_EXTI_LINE       EXTI_Line2
#define PulseCap_EXTI_IRQ        EXTI2_IRQn

#define PulseCap_IRQHandler      EXTI2_IRQHandler


void PulseCap_EXTI_Config(void);

#endif /*_BSP_PULSECAP_EXTI_H*/