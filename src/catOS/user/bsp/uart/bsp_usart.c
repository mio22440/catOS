/**
 * @file bsp_usart.c
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

#include "bsp_usart.h"

UART_HandleTypeDef UartHandle;


/**************** catos interfaces *******************/
uint32_t cat_bsp_uart_init(void)
{
    uint32_t ret = 0;
    cat_f103vet6_usart1_init();

    return ret;
}

uint32_t cat_bsp_uart_transmit(uint8_t *pData, uint16_t Size)
{
	uint32_t ret = 0;
	
	ret = HAL_UART_Transmit(&UartHandle, pData, Size, 0xffff);
	
  //没成功就跑到天荒地老
	/*
	if(ret)
	{
		while(1);
	}
	*/

  return ret;
}

uint32_t cat_bsp_uart_receive(uint8_t *pData, uint16_t Size)
{
	uint32_t ret = 0;
	
	ret = HAL_UART_Receive(&UartHandle, pData, Size, 0x1000);
	
  //没成功就跑到天荒地老
	/*
	if(ret)
	{
		while(1);
	}
*/
  return ret;
}

uint8_t cat_bsp_uart_transmit_byte(uint8_t *ch)
{
  HAL_UART_Transmit(&UartHandle, (uint8_t *)ch, 1, 1000);
  return *ch;
}

uint8_t cat_bsp_uart_receive_byte(uint8_t *ch)
{
  HAL_UART_Receive(&UartHandle, (uint8_t *)ch, 1, 1000);	
  return *ch;
}


/******************* local functions ************************/
void cat_f103vet6_usart1_init(void)
{
    UartHandle.Instance          = USART_1_NAME;
  
  UartHandle.Init.BaudRate     = USART_1_BAUDRATE;
  UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits     = UART_STOPBITS_1;
  UartHandle.Init.Parity       = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode         = UART_MODE_TX_RX;
  
  HAL_UART_Init(&UartHandle);
   
  //__HAL_UART_ENABLE_IT(&UartHandle,UART_IT_RXNE);  
}

//会在HAL_UART_Init中被调用
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  USART_1_CLK_ENABLE();
  USART_1_RX_GPIO_CLK_ENABLE();
  USART_1_TX_GPIO_CLK_ENABLE();
  
/**USART1 GPIO Configuration    
  PA9     ------> USART1_TX
  PA10    ------> USART1_RX 
  */
  /* 设置tx引脚为复用  */
  GPIO_InitStruct.Pin = USART_1_TX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed =  GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(USART_1_TX_GPIO_PORT, &GPIO_InitStruct);
  
  /* 设置rx引脚为复用 */
  GPIO_InitStruct.Pin = USART_1_RX_PIN;
  GPIO_InitStruct.Mode=GPIO_MODE_AF_INPUT;
  HAL_GPIO_Init(USART_1_RX_GPIO_PORT, &GPIO_InitStruct); 
 
  //HAL_NVIC_SetPriority(USART_1_IRQ ,0,1);	
  //HAL_NVIC_EnableIRQ(USART_1_IRQ );		   
}


#if 0
  #define CATOS_REDEFINE_PRINTF 1
  #if (CATOS_REDEFINE_PRINTF == 1)
  #if 1
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
      
      HAL_UART_Transmit(&UartHandle, pBuffer, size, 0xffff);
    
      cat_task_exit_critical(status);	
    
    return 0;
  }

  #endif //#ifdef (__CC_ARM)
  #endif //#if 0
  #endif //#if (CATOS_REDEFINE_PRINTF == 1)
#endif
