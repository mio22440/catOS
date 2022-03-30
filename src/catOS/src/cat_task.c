/**
 * @file task.c
 * @brief 任务相关函数实现
 * @author amoigus (648137125@qq.com)
 * @version 0.1
 * @date 2021-03-19
 * 
 * @copyright Copyright (c) 2021
 * 
 * @par 修改日志：
 * Date              Version Author      Description
 * 2021-03-19 1.0    amoigus             内容
 */

#include "cat_task.h"
#include "cat_time.h"
#include "cat_lib.h"
#include "cat_hooks.h"
#include "string.h"

struct _cat_TCB_t *cat_current_task;//当前任务的指针
struct _cat_TCB_t *cat_ready_task;//要切换到的任务的指针

struct _cat_TCB_t *cat_idle_task;//空闲任务指针 from cat_idle.c

uint8_t sched_lock_cnt;//调度锁

cat_bitmap cat_task_prio_bitmap;//就绪位图
struct _cat_list_t task_tbl[CATOS_MAX_TASK_PRIO];//就绪表

#if (CATOS_ENABLE_TASK_STAT == 1)
    struct _cat_TCB_t *cat_task_stat_list[CATOS_TASK_STAT_MAX_TASK] = {0};
    uint32_t cat_task_num = 0;
#endif //#if (CATOS_ENABLE_TASK_STAT == 1)

//任务初始化
void task_init(
    const uint8_t *task_name,
    struct _cat_TCB_t *task, 
    void (*entry)(void *), 
    void *arg, 
    uint8_t prio, 
    cat_stack_type_t *stack_start_addr,
    uint32_t stack_size,
    uint32_t sched_strategy
)
{
    uint32_t *stack_top;

    task->stack_start_addr = stack_start_addr;
    task->stack_size = stack_size;
    
    memset(stack_start_addr, 0, stack_size);

    stack_top = stack_start_addr + (stack_size / sizeof(cat_stack_type_t));

    //pensv自动保存的部分
    *(--stack_top) = (uint32_t)(1 << 24);//spsr
    *(--stack_top) = (uint32_t)entry;//pc
    *(--stack_top) = (uint32_t)0x14;//lr(r14)
    *(--stack_top) = (uint32_t)0x12;//r12
    *(--stack_top) = (uint32_t)0x3;//r3
    *(--stack_top) = (uint32_t)0x2;//r2
    *(--stack_top) = (uint32_t)0x1;//r1
    *(--stack_top) = (uint32_t)arg;

    *(--stack_top) = (uint32_t)0x11;
    *(--stack_top) = (uint32_t)0x10;
    *(--stack_top) = (uint32_t)0x9;
    *(--stack_top) = (uint32_t)0x8;
    *(--stack_top) = (uint32_t)0x7;
    *(--stack_top) = (uint32_t)0x6;
    *(--stack_top) = (uint32_t)0x5;
    *(--stack_top) = (uint32_t)0x4;

    task->stack_top = stack_top;
    task->task_delay = 0;
    task->prio = prio;
    task->state = CATOS_TASK_STATE_RDY;
    task->slice = CATOS_MAX_SLICE;
    task->suspend_cnt = 0;
    task->clean_cbk = NULL;//现在没有清理函数
    task->clean_arg = NULL;
    task->req_delete_flag = 0;

    task->wait_event = NULL;
    task->event_msg  = NULL;

    task->task_name = task_name;
    task->sched_times = 0;

    task->sched_strategy = sched_strategy;

    cat_node_init(&(task->delay_node));
    cat_node_init(&(task->link_node));

    if(SCHED_STRATEGY_PRIO == task->sched_strategy)
    {
        cat_task_sched_rdy(task);//将任务挂到就绪表
    }
    

#if (CATOS_ENABLE_TASK_STAT == 1)
    if(cat_task_num < CATOS_TASK_STAT_MAX_TASK)
    {
        cat_task_stat_list[cat_task_num++] = task;
    }
#endif //#if (CATOS_ENABLE_TASK_STAT == 1)

#if (CATOS_ENABLE_HOOKS == 1)
    //执行钩子函数
    cat_hooks_task_init(task);
#endif

}



//得到最高优先级任务
struct _cat_TCB_t *cat_task_highest_ready(void)
{
    uint32_t highest_prio = cat_bitmap_get_first_set(&cat_task_prio_bitmap);
    struct _cat_node_t *node = cat_list_first(&(task_tbl[highest_prio]));//获取链表的第一个节点
    return NODE_PARENT(node, struct _cat_TCB_t, link_node);//获取任务结构指针
}

//初始化调度相关
void cat_task_sched_init(void)
{
    int i;

	sched_lock_cnt = 0;
    cat_bitmap_init(&cat_task_prio_bitmap);

    for(i=0; i < CATOS_MAX_TASK_PRIO; i++)
    {
        cat_list_init(&(task_tbl[i]));
    }

}

//禁止调度
void cat_task_sched_disable(void)
{
	uint32_t status = cat_task_enter_critical();
    if(sched_lock_cnt < 255)
    {
        sched_lock_cnt++;
    }

    cat_task_exit_critical(status);
}

//允许调度
void cat_task_sched_enable(void)
{
    uint32_t status = cat_task_enter_critical();
    if(sched_lock_cnt > 0)
    {
        if(--sched_lock_cnt == 0)
        {
            cat_task_sched();
        }
    }

    cat_task_exit_critical(status);
}

//进行一次调度
void cat_task_sched(void)
{
    struct _cat_TCB_t *temp_task;
    uint32_t status = cat_task_enter_critical();

    if(sched_lock_cnt > 0)//如果调度被上锁就直接返回，不调度
    {
        cat_task_exit_critical(status);
        return;
    }

    temp_task = cat_task_highest_ready();
    if(temp_task != cat_current_task)
    {
        cat_ready_task = temp_task;
        cat_ready_task->sched_times++; /* 增加调度次数信息 */

#if (CATOS_ENABLE_HOOKS == 1)
        cat_hooks_task_before_switch(cat_current_task, cat_ready_task);
#endif

        cat_task_switch();
    }

    cat_task_exit_critical(status);
}

//将任务放进就绪表
void cat_task_sched_rdy(struct _cat_TCB_t *task)
{
    //task_tbl[task->prio] = task;
    cat_list_add_first(&(task_tbl[task->prio]), &(task->link_node));
    cat_bitmap_set(&cat_task_prio_bitmap, task->prio);
}

//将任务从就绪表取出
void cat_task_sched_unrdy(struct _cat_TCB_t *task)
{
    //task_tbl[task->prio] = NULL;
    cat_list_remove(&(task_tbl[task->prio]), &(task->link_node));
    if(cat_list_count(&(task_tbl[task->prio])) == 0)//如果没有任务才清除就绪位
    {
        cat_bitmap_clr(&cat_task_prio_bitmap, task->prio);
    }
    
}

//挂起
void cat_task_suspend(struct _cat_TCB_t *task)
{
    uint32_t status = cat_task_enter_critical();

    //只有不在延时状态时可以被挂起
    if(!(task->state & CATOS_TASK_STATE_DELAYED))
    {
        if(++task->suspend_cnt <= 1)//增加被阻塞次数
        {                           //若当前未被阻塞，则阻塞(是否可用后缀简化？)
            task->state |= CATOS_TASK_STATE_SUSPEND;//置位阻塞状态位
            cat_task_sched_unrdy(task);//从就绪表取出
            
            //如果被阻塞的是当前任务，则需要执行下一个任务，即进行一次调度
            if(task == cat_current_task)
            {
                cat_task_sched();
            }
        }
    }

    cat_task_exit_critical(status);
}

void cat_task_wakeup(struct _cat_TCB_t *task)
{
    uint32_t status = cat_task_enter_critical();

    //只有已经被挂起至少一次的任务才能被唤醒
    if(task->state & CATOS_TASK_STATE_SUSPEND)
    {
        if(--task->suspend_cnt == 0)//减少被阻塞次数
        {                           //若被阻塞次数为零，则退出阻塞状态
            task->state &= ~(CATOS_TASK_STATE_SUSPEND);
            cat_task_sched_rdy(task);//挂到就绪表
            cat_task_sched();//进行一次调度
        }
    }

    cat_task_exit_critical(status);
}

void cat_task_sched_remove(struct _cat_TCB_t *task)
{
    cat_list_remove(&(task_tbl[task->prio]), &(task->link_node));
    if(cat_list_count(&(task_tbl[task->prio])) == 0)
    {
        cat_bitmap_clr(&cat_task_prio_bitmap, task->prio);
    }
}

//为任务设置清理函数
void cat_task_set_clean_cbk(struct _cat_TCB_t *task, void (*clean_cbk)(void *clean_arg), void *clean_arg)
{
    task->clean_cbk = clean_cbk;
    task->clean_arg = clean_arg;
}


void cat_task_force_delete(struct _cat_TCB_t *task)
{
    uint32_t status = cat_task_enter_critical();

    if(task->state & CATOS_TASK_STATE_DELAYED)
    {
        //若处于延时状态则将任务从延时队列取出
        cat_task_time_remove(task);
    }
    else if(!(task->state & CATOS_TASK_STATE_SUSPEND))
    {
        //若未被阻塞则从就绪表取出
        cat_task_sched_remove(task);
    }

    if(NULL != task->clean_cbk)
    {
        task->clean_cbk(task->clean_arg);
    }

    if(cat_current_task == task)
    {
        cat_task_sched();
    }

    cat_task_exit_critical(status);
}

//请求删除任务
void cat_task_request_delete(struct _cat_TCB_t *task)
{
    uint32_t status = cat_task_enter_critical();

    task->req_delete_flag = 1;

    cat_task_exit_critical(status);
}

//检查是否被请求删除
uint8_t cat_task_is_requested_delete(void)
{
    uint8_t delete;


    uint32_t status = cat_task_enter_critical();

    delete = cat_current_task->req_delete_flag;

    cat_task_exit_critical(status);


    return delete;
}

//删除自己
void cat_task_delete_self(void)
{
    uint32_t status = cat_task_enter_critical();

    cat_task_sched_remove(cat_current_task);//此时current就是自己

    if(NULL != cat_current_task->clean_cbk)
    {
        cat_current_task->clean_cbk(cat_current_task->clean_arg);
    }

    cat_task_sched();

    cat_task_exit_critical(status); 
}


//获取任务信息
void cat_task_get_info(struct _cat_TCB_t *task, struct _task_info_t *info)
{
    uint32_t *stack_end;
    uint32_t status = cat_task_enter_critical();

    info->task_name     = task->task_name;
    info->sched_times   = task->sched_times;
    info->task_delay    = task->task_delay;
    info->prio          = task->prio;
    info->state         = task->state;
    info->slice         = task->slice;
    info->suspend_cnt   = task->suspend_cnt;

    info->stack_start_addr = task->stack_start_addr;
    info->stack_top        = task->stack_top;
    info->stack_size       = task->stack_size;

    cat_task_exit_critical(status);

    info->free_stack_size = 0;//初始化空闲堆栈长度
#if 0
    stack_end = task->stack_start_addr;//开始地址是低地址

    while((*stack_end++ == 0) && (stack_end <= task->stack_start_addr + (task->stack_size / sizeof(cat_stack_type_t))))
    {
        info->free_stack_size++;
    }
    info->free_stack_size *= sizeof(cat_stack_type_t);
#else //#if 0
    info->free_stack_size = info->stack_top - info->stack_start_addr;
#endif //#if 0
    
}

#if (CATOS_ENABLE_SHELL == 1)
#if (CATOS_ENABLE_TASK_STAT == 1)
#include "cat_shell.h"
#if (CATOS_ENABLE_SYS_PRINT == 1)
    #include "cat_sys_print.h"
#endif //#if (CATOS_ENABLE_SYS_PRINT == 1)
void *do_task_stat(void *arg)
{
    (void)arg;
    struct _task_info_t tmp_info = {0};
    uint32_t i = 0;

    CAT_SYS_PRINTF("no\tname\t\tsched\tdelay\tprio\tstate\tstack\r\n");

    for(i=0; i<cat_task_num; i++)
    {
        cat_task_get_info(cat_task_stat_list[i], &tmp_info);
        CAT_SYS_PRINTF("%d:\t%s\t%d\t%d\t%d\t%d\t%d/%d\r\n", 
            i,
            tmp_info.task_name,
            tmp_info.sched_times,
            tmp_info.task_delay,
            tmp_info.prio,
            tmp_info.state,
            tmp_info.free_stack_size,
            tmp_info.stack_size
        );
    }

    return NULL;
}
CAT_DECLARE_CMD(task_stat, print all task info, do_task_stat);
#endif //#if (CATOS_ENABLE_TASK_STAT == 1)
#endif //#if (CATOS_ENABLE_SHELL == 1)

/************* 任务删除相关 ******************/
