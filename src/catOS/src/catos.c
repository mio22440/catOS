/**
 * @file catos.c
 * @brief 
 * @author amoigus (648137125@qq.com)
 * @version 0.1
 * @date 2021-03-29
 * 
 * @copyright Copyright (c) 2021
 * 
 * @par 修改日志：
 * Date              Version Author      Description
 * 2021-03-29 1.0    amoigus             内容
 */

#include "catos.h"
#include "app.h"

#include "strategy.h"

extern struct _cat_list_t task_tbl[CATOS_MAX_TASK_PRIO];//任务表    from cat_task.c
extern struct _cat_list_t cat_task_delayed_list;//延时链表          from cat_time.c

extern uint32_t tick_count;    /**< 时钟节拍次数*/                 //from cat_idle.c
extern uint32_t catos_ticks;


/**
 * @brief 系统初始化
 */
void catos_init(void)
{
    cat_task_sched_init();
    cat_task_delayed_init();

//定时器模块初始化
#if (CATOS_ENABLE_TIMER == 1)
    cat_timer_module_init();
#endif
    
    time_tick_init();

#if (CATOS_ENABLE_CPUUSAGE_STAT == 1)
    init_cpu_usage_stat();
#endif

    cat_idle_task_init();
}

/**
 * @brief 系统时钟中断处理函数
 *        处理tick等
 */
void cat_task_systemtick_handler(void)
{
    struct _cat_node_t *node;
    uint32_t status = cat_task_enter_critical();

    //处理延时队列
    for(node = cat_task_delayed_list.head_node.next_node;
        node != &(cat_task_delayed_list.head_node);
        node = node->next_node)
    {
        struct _cat_TCB_t *task = NODE_PARENT(node, struct _cat_TCB_t, delay_node);
        
        if(--task->task_delay == 0)
        {
            //处理等待事件超时的情况
            if(NULL != task->wait_event)
            {
                cat_event_remove_task(task, NULL, cat_event_err_timeout);
            }

            cat_task_time_wakeup(task);//从延时表取出
            cat_task_sched_rdy(task);//挂到就绪表
        }
    }

    //处理时间片
    if(--cat_current_task->slice == 0)
    {
        if(cat_list_count(&(task_tbl[cat_current_task->prio])))
        {
            //将当前任务节点从任务链表首位去掉
            cat_list_remove_first(&(task_tbl[cat_current_task->prio]));
            //将当前任务节点加到任务链表末尾
            cat_list_add_last(&(task_tbl[cat_current_task->prio]), &(cat_current_task->link_node));

            //重置时间片
            cat_current_task->slice = CATOS_MAX_SLICE;
        }
    }

	tick_count++;
    catos_ticks++;
#if (CATOS_ENABLE_CPUUSAGE_STAT == 1)
    cpu_usage_stat();
#endif

    cat_task_exit_critical(status);

#if (CATOS_ENABLE_TIMER == 1)
    //通知定时器任务tick
    cat_timer_module_tick_come_notify();
#endif

#if (CATOS_ENABLE_HOOKS == 1)
    cat_hooks_systick();
#endif
	

#ifdef USE_EDF_SCHED
    uint8_t edf_got_task = 0;
    edf_got_task = edf_sched();

    /* 如果edf没有需要执行的任务则调度优先级任务 */
    if(0 != edf_got_task)
    {
#endif
        //进行一次调度
        cat_task_sched();
#ifdef USE_EDF_SCHED
    }
#endif

}
