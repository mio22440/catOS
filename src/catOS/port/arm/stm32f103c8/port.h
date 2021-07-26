/**
 * @file port.h
 * @brief 
 * @author amoigus (648137125@qq.com)
 * @version 0.1
 * @date 2021-03-22
 * 
 * @copyright Copyright (c) 2021
 * 
 * @par 修改日志：
 * Date              Version Author      Description
 * 2021-03-22 1.0    amoigus             内容
 */

#ifndef CAT_PORT_H
#define CAT_PORT_H

#include "stm32f10x.h"

#include "catos_config.h"
#include "catos_defs.h"

#define NVIC_INT_CTRL   0xE000ED04
#define NVIC_PENDSVSET  0x10000000
#define NVIC_SYSPRI2    0xE000ED22
#define NVIC_PENDSV_PRI 0x000000FF

#define MEM32(addr)     *(volatile uint32_t *)(addr)
#define MEM8(addr)      *(volatile uint8_t  *)(addr)


/**
 * @brief 硬件初始化
 */
void cat_port_hardware_init(void);

/**
 * @brief 设定系统时钟中断周期
 * @param ms 
 */
void cat_set_systick_period(uint32_t ms);//初始化中断

void cat_task_sched(void);
void cat_start_first(void);
void cat_task_switch(void);
uint32_t cat_task_enter_critical(void);
void cat_task_exit_critical(uint32_t status);



#endif
