/**
 * @file cat_uart.c
 * @brief 
 * @author amoigus (648137125@qq.com)
 * @version 0.1
 * @date 2021-06-05
 * 
 * @copyright Copyright (c) 2021
 * 
 * @par 修改日志：
 * Date              Version Author      Description
 * 2021-06-05 1.0    amoigus             内容
 */

#include "cat_uart.h"
#include "bsp_usart.h"
#include "port.h"

uint32_t cat_uart_init(void)
{
    uint32_t ret = 0;
    (void)cat_bsp_uart_init();
    cat_uart_transmit("[bsp] uart init succeed!!\r\n", 27);
    return ret;
}

uint32_t cat_uart_transmit(uint8_t *data, uint32_t size)
{
    uint32_t ret = 0;

    (void)cat_bsp_uart_transmit(data, size);

    return ret;
}

uint32_t cat_uart_receive(uint8_t *data, uint32_t size)
{
    uint32_t ret = 0;

    (void)cat_bsp_uart_receive(data, size);

    return ret;
}

uint8_t cat_uart_transmit_byte(uint8_t *ch)
{
    return cat_bsp_uart_transmit_byte(ch);
}

uint8_t cat_uart_receive_byte(uint8_t *ch)
{
    return cat_bsp_uart_receive_byte(ch);
}

#if (CATOS_REDEFINE_PRINTF == 1)
#if 0
#if defined(__CC_ARM)
int fputc(int ch, FILE *f)
{
	uint32_t status = cat_task_enter_critical();
	/* 发送一个字节数据到串口DEBUG_USART */
	cat_bsp_uart_transmit_byte((uint8_t *)&ch);
  cat_task_exit_critical(status);	
	
	return (ch);
}

//重定向c库函数scanf到串口DEBUG_USART，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{		
	int ch;

    //uint32_t status = cat_task_enter_critical();
	cat_bsp_uart_receive_byte((uint8_t *)&ch);
    //cat_task_exit_critical(status);	

	return (ch);
}
#elif defined(__GNUC__)
int _write(int fd, char *pBuffer, int size)
{
	uint32_t status = cat_task_enter_critical();
    
    cat_bsp_uart_transmit((uint8_t *)pBuffer, size);
	
    cat_task_exit_critical(status);	
	
	return 0;
}

//重定向c库函数scanf到串口DEBUG_USART，重写向后可使用scanf、getchar等函数
// int fgetc(FILE *f)
// {		
// 	int ch;

//     //uint32_t status = cat_task_enter_critical();
// 	cat_uart_receive_byte((uint8_t *)&ch);
//     //cat_task_exit_critical(status);	

// 	return (ch);
// }
#endif //#ifdef (__CC_ARM)
#endif //#if 0

#endif //#if (CATOS_REDEFINE_PRINTF == 1)
