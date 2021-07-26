


#include "cat_sys_print.h"
#include "bsp_usart.h"




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
    #if 0
        int _write(int fd, char *pBuffer, int size)
        {
            uint32_t status = cat_task_enter_critical();
            
            cat_bsp_uart_transmit((uint8_t*)pBuffer, (uint16_t)(size | 0xffff));
            
            cat_task_exit_critical(status);	
            
            return 0;
        }
    #else
        int _write(int fd, char *pBuffer, int size)
        {
            uint32_t status = cat_task_enter_critical();
            
            cat_bsp_uart_transmit(pBuffer, size);
            
            cat_task_exit_critical(status);	
            
            return 0;
        }

        int32_t cat_sys_scanf(uint8_t fmt, ...)
        {

        }

        int32_t cat_sys_printf(uint8_t fmt, ...)
        {

        }

        uint8_t cat_sys_getchar(void)
        {
            uint8_t ch;

            //uint32_t status = cat_task_enter_critical();
            cat_bsp_uart_receive_byte((uint8_t *)&ch);
            //cat_task_exit_critical(status);	

            return (ch);
        }

        int32_t cat_sys_putchar(uint8_t ch)
        {
            uint32_t status = cat_task_enter_critical();
            /* 发送一个字节数据到串口DEBUG_USART */
            cat_bsp_uart_transmit_byte(&ch);
            cat_task_exit_critical(status);	
            
            return (ch);
        }

    #endif

#endif //#ifdef (__CC_ARM)
#endif //#if 0
#endif //#if (CATOS_REDEFINE_PRINTF == 1)