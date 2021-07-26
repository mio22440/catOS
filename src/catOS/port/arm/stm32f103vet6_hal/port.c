/**
 * @file port.c
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

#include "port.h"
#include "cat_list.h"
#include "cat_task.h"
#include "cat_time.h"
#include "cat_event.h"
#include "cat_timer.h"

// void PendSV_Handler(void)
// {
//     while(1);
// }

#if defined(__GNUC__)

int __wrap_atexit(void __attribute__((unused)) (*function)(void)) {
    return -1;
}

#endif //#if defined(__GNUC__)

void SysTick_Handler(void)//stm32的时钟中断处理函数
{
    //cat_task_sched();
    cat_task_systemtick_handler();//调用自己的处理函数
}

void cat_set_systick_period(uint32_t ms)//初始化中断
{
    SysTick->LOAD = ms * SystemCoreClock / 1000;                    //重载计数器值
    NVIC_SetPriority(SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1);
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk |
                    SysTick_CTRL_ENABLE_Msk;
}

//开始第一个任务(pendsv)
void cat_start_first(void)
{
    __set_PSP(0);

    MEM8(NVIC_SYSPRI2)      = NVIC_PENDSV_PRI;
    MEM32(NVIC_INT_CTRL)    = NVIC_PENDSVSET;
}

//触发pendsv中断进行任务切换，pendsv中断处理函数定义在汇编port.s中
void cat_task_switch(void)
{
    MEM32(NVIC_INT_CTRL)    = NVIC_PENDSVSET;//pendsv的优先级在开始第一个任务时已经设置
}

//关中断方式进入临界区
uint32_t cat_task_enter_critical(void)
{
    uint32_t primask = __get_PRIMASK();//读取中断配置
    __disable_irq();
    return primask;
}

//开中断方式出临界区
void cat_task_exit_critical(uint32_t status)
{
    __set_PRIMASK(status);
}

#if (CATOS_USE_SW_RESET == 1)
void cat_port_reset_chip(void)
{
    __set_FAULTMASK(1);
    NVIC_SystemReset();
}
#endif



