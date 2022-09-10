#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "stm32f1xx.h"
#include <stdio.h>

//���ڽ��ջ��������С
#define UART_RX_BUFFER_SIZE 256 

extern unsigned char UART_RxBuffer[UART_RX_BUFFER_SIZE];
extern uint8_t receive_cmd;

#define  DEBUG_USART_BAUDRATE                   115200

// USART1
#define  DEBUG_USARTx                           USART1
#define  DEBUG_USART_CLK_ENABLE()               __HAL_RCC_USART1_CLK_ENABLE();

#define  RCC_PERIPHCLK_UARTx                    RCC_PERIPHCLK_USART1
#define  RCC_UARTxCLKSOURCE_SYSCLK              RCC_USART1CLKSOURCE_SYSCLK


// USART GPIO macro define
#define  DEBUG_USART_TX_GPIO_PORT               GPIOA   
#define  DEBUG_USART_TX_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define  DEBUG_USART_TX_GPIO_PIN                GPIO_PIN_9

#define  DEBUG_USART_RX_GPIO_PORT               GPIOA
#define  DEBUG_USART_RX_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define  DEBUG_USART_RX_GPIO_PIN                GPIO_PIN_10

#define  DEBUG_USART_IRQ                        USART1_IRQn
#define  DEBUG_USART_IRQHandler                 USART1_IRQHandler

void uart_FlushRxBuffer(void);
void Usart_SendByte(uint8_t str);
void Usart_SendString(uint8_t *str);
void DEBUG_USART_Config(void);
//int fputc(int ch, FILE *f);
extern UART_HandleTypeDef UartHandle;

#endif /*__BSP_USART_H*/
