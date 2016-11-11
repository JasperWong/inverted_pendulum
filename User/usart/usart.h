#ifndef __USART_H
#define	__USART_H


#include "stm32f10x.h"
#include <stdio.h>

#define USART1_DR_Base  0x40013804	
#define SENDBUFF_SIZE   53

/**************************USART参数定义********************************/
#define             macUSART_BAUD_RATE                       115200

#define             macUSARTx                                USART1
#define             macUSART_APBxClock_FUN                   RCC_APB2PeriphClockCmd
#define             macUSART_CLK                             RCC_APB2Periph_USART1
#define             macUSART_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             macUSART_GPIO_CLK                        RCC_APB2Periph_GPIOA     
#define             macUSART_TX_PORT                         GPIOA   
#define             macUSART_TX_PIN                          GPIO_Pin_9
#define             macUSART_RX_PORT                         GPIOA 
#define             macUSART_RX_PIN                          GPIO_Pin_10
#define             macUSART_IRQ                             USART1_IRQn
#define             macUSART_INT_FUN                         USART1_IRQHandler



void USARTx_Config( void );
void USART1_DMA_Config(void);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendStr( USART_TypeDef * pUSARTx, uint8_t *str );
void Usart_SendStr_length( USART_TypeDef * pUSARTx, uint8_t *str,uint32_t strlen );

#endif /* __USART1_H */
