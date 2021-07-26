/**
 * @file cat_shell_port.c
 * @brief 
 * @author amoigus (648137125@qq.com)
 * @version 0.1
 * @date 2021-06-10
 * 
 * @copyright Copyright (c) 2021
 * 
 * @par 修改日志：
 * Date              Version Author      Description
 * 2021-06-10 1.0    amoigus             内容
 */

#include "cat_task.h"
#include "cat_shell_port.h"
#include "cat_shell.h"
#include "cat_uart.h"
#include "cat_sys_print.h"
#include "port.h"

struct _cat_TCB_t shell_task;
cat_stack_type_t shell_task_env[CATOS_SHELL_STACK_SIZE];

cat_shell_instance_t port_shell_inst_1 = {0};
cat_shell_config_t shell_cfg = {0};
uint8_t shell_space[512];



void cat_shell_task_init(void)
{
    int32_t ret = 0;

    /* 将shellspace分配到各个成员 */
    shell_cfg.buffer = shell_space;
    shell_cfg.buf_size = 512;
    if(sizeof(shell_space) < CAT_BUF_SIZE * (CAT_MAX_HISTORY + 1))
    {
        CAT_SYS_PRINTF("[cat_shell_port:%d] shell_space is not enough !\r\n", __LINE__);
        while(1);
    }

    // for(i=0; i<CAT_MAX_HISTORY; i++)
    // {
    //     shell_cfg.historys[i] = shell_space + CAT_BUF_SIZE * i;
    // }

    ret = cat_shell_init(&port_shell_inst_1, &shell_cfg);
    if(ret)
    {
        CAT_SYS_PRINTF("[cat_shell_port:%d] cat_shell_init fail!\r\n", __LINE__);
        while(1);
    }

    task_init("shell_task", &shell_task, cat_shell_task_entry, NULL, CATOS_SHELL_TASK_PRIO, shell_task_env, CATOS_SHELL_STACK_SIZE);
    CAT_SYS_PRINTF("[cat_shell_port] cat_shell init success \r\n");
}


int16_t cat_shell_port_getc(uint8_t *data)
{
    int16_t ret = 0;
    *data = CAT_SYS_GETCHAR();
    return ret;
}

int16_t cat_shell_port_putc(uint8_t data)
{
    int16_t ret = 0;
#if 0
    (void)putchar(data);
#else
    CAT_SYS_PUTCHAR(data);
#endif
    return ret;
}

#if (CATOS_USE_SW_RESET == 1)
void cat_reset_chip(void)
{
    cat_port_reset_chip();
}
#endif

