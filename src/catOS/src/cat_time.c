/**
 * @file cat_time.c
 * @brief 
 * @author amoigus (648137125@qq.com)
 * @version 0.1
 * @date 2021-03-24
 * 
 * @copyright Copyright (c) 2021
 * 
 * @par 修改日志：
 * Date              Version Author      Description
 * 2021-03-24 1.0    amoigus             内容
 */

#include "cat_time.h"

extern uint32_t catos_ticks;

struct _cat_list_t cat_task_delayed_list;//延时链表

void cat_get_ticks(uint32_t *ticks)
{
    *ticks = catos_ticks;
}

void cat_task_delay(uint32_t delay)
{
    uint32_t status = cat_task_enter_critical(); 

    //cat_current_task->task_delay = delay;
    cat_task_time_wait(cat_current_task, delay);

    //cat_bitmap_clr(&cat_task_prio_bitmap, cat_current_task->prio);//从就绪表取出（清掉标志位即可）
    cat_task_sched_unrdy(cat_current_task);

    cat_task_exit_critical(status);

    cat_task_sched();
}

//初始化延时列表
void cat_task_delayed_init(void)
{
    cat_list_init(&cat_task_delayed_list);
}

//插入延时列表
void cat_task_time_wait(struct _cat_TCB_t *task, uint32_t ticks)
{
    task->task_delay = ticks;
    cat_list_add_last(&cat_task_delayed_list, &(task->delay_node));
    task->state |= CATOS_TASK_STATE_DELAYED;
}

//从延时唤醒(从延时表取出并改变状态)
void cat_task_time_wakeup(struct _cat_TCB_t *task)
{
    cat_list_remove(&cat_task_delayed_list, &(task->delay_node));
    task->state &= ~CATOS_TASK_STATE_DELAYED;
}

void cat_task_time_remove(struct _cat_TCB_t *task)
{
    cat_list_remove(&cat_task_delayed_list, &(task->delay_node));
}

#if (CATOS_ENABLE_SHELL == 1)
#include "cat_shell.h"
#if (CATOS_ENABLE_SYS_PRINT == 1)
    #include "cat_sys_print.h"
#endif //#if (CATOS_ENABLE_SYS_PRINT == 1)
void *do_get_ticks(void *arg)
{
    (void)arg;
    uint32_t ticks = 0;

    cat_get_ticks(&ticks);
    CAT_SYS_PRINTF("current ticks: %d\r\n", ticks);

    return NULL;
}
CAT_DECLARE_CMD(get_ticks, print ticks, do_get_ticks);
#endif //#if (CATOS_ENABLE_SHELL == 1)
