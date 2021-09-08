/**
 * @file bsp_usart.h
 * @brief 
 * @author amoigus (648137125@qq.com)
 * @version 0.1
 * @date 2021-05-14
 * 
 * @copyright Copyright (c) 2021
 * 
 * @par 修改日志：
 * Date              Version Author      Description
 * 2021-05-14 1.0    amoigus             内容
 */

#ifndef USART_1_H
#define USART_1_H

#include "stm32f1xx.h"
#include <stdio.h>

//串口1波特率
#define USART_1_BAUDRATE 115200

//名称
#define USART_1_NAME                    USART1

//引脚
#define USART_1_RX_GPIO_PORT            GPIOA
#define USART_1_RX_PIN                  GPIO_PIN_10
#define USART_1_TX_GPIO_PORT            GPIOA
#define USART_1_TX_PIN                  GPIO_PIN_9

//中断
#define USART_1_IRQHandler              USART1_IRQHandler
#define USART_1_IRQ                     USART1_IRQn

//使能宏定义
#define USART_1_CLK_ENABLE()            __HAL_RCC_USART1_CLK_ENABLE()
#define USART_1_RX_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()
#define USART_1_TX_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()


/**
 * @brief 和catos标准接口相同
 */
uint32_t cat_bsp_uart_init(void);
uint32_t cat_bsp_uart_transmit(uint8_t *pData, uint16_t Size);
uint32_t cat_bsp_uart_receive(uint8_t *pData, uint16_t Size);
uint8_t cat_bsp_uart_transmit_byte(uint8_t *ch);
uint8_t cat_bsp_uart_receive_byte(uint8_t *ch);

void cat_f103vet6_usart1_init(void);




#endif
