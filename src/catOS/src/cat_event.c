/**
 * @file cat_event.c
 * @brief 事件
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

#include "cat_event.h"
#include "cat_time.h"


void cat_event_init(struct _cat_event_t *event, cat_event_type_t type)
{
    event->type = type;
    cat_list_init(&(event->wait_list));
}

//等待事件
void cat_event_wait(struct _cat_event_t *event, struct _cat_TCB_t *task, void *msg, uint32_t state, uint32_t timeout)
{
    uint32_t status = cat_task_enter_critical();

    //task->state |= state;//会把task->state低位表示任务状态的覆盖掉
    task->state |= (state << 16);//这样才是正确的，只占用给event_type预定的高16位
    task->wait_event = event;
    task->event_msg = msg;
    task->wait_event_result = cat_event_err_noerror;

    //从就序队列取下
    cat_task_sched_unrdy(task);

    //将任务挂到事件的等待队列
    cat_list_add_last(&(event->wait_list), &(task->link_node));

    //如果设置了超时时间(为零表示未设置)
    if(timeout)
    {
        cat_task_time_wait(task, timeout);
    }

    cat_task_exit_critical(status);
}

//将第一个等待事件的任务从等待中唤醒
struct _cat_TCB_t *cat_event_wakeup_first(struct _cat_event_t *event, void *msg, uint32_t result)
{
    struct _cat_node_t *node;
		struct _cat_TCB_t *task = NULL;
    uint32_t status = cat_task_enter_critical();

    //当等待队列中有任务时
    if((node = cat_list_remove_first((&event->wait_list))) != NULL)
    {
        //获取任务起始地址并处理事件相关的成员
        task = (struct _cat_TCB_t *)NODE_PARENT(node, struct _cat_TCB_t, link_node);
        task->wait_event = NULL;//清空
        task->event_msg = msg;
        task->wait_event_result = result;
        task->state &= ~CATOS_TASK_EVENT_MASK;//清除掉设置的事件相关状态

        //如果处在延时状态则唤醒
        if(task->task_delay != 0)
        {
            cat_task_time_wakeup(task);
            //此时还没在就绪列表中,且延时未结束，所以cat_task_systemtick_handler(void)
            //不会将其挂到就绪队列，需要手动挂
        }
        cat_task_sched_rdy(task);//挂就绪队列

    }

    cat_task_exit_critical(status);
		
		return task;
}

//将指定的任务从等待中唤醒
struct _cat_TCB_t *cat_event_wakeup(struct _cat_event_t *event,struct _cat_TCB_t *task, void *msg, uint32_t result)
{
    //struct _cat_node_t *node;
    uint32_t status = cat_task_enter_critical();

    //直接将任务从等待队列中取出
    cat_list_remove(&(event->wait_list), &(task->link_node));

    //获取任务起始地址并处理事件相关的成员
	//task = (struct _cat_TCB_t *)NODE_PARENT(node, struct _cat_TCB_t, link_node);这里已知task，故不需要推导
    task->wait_event = NULL;//清空
    task->event_msg = msg;
    task->wait_event_result = result;
    task->state &= ~CATOS_TASK_EVENT_MASK;//清除掉设置的事件相关状态
    
    //如果处在延时状态则唤醒
    if(task->task_delay != 0)
    {
        cat_task_time_wakeup(task);
        //此时还没在就绪列表中,且延时未结束，所以cat_task_systemtick_handler(void)
        //不会将其挂到就绪队列，需要手动挂
    }
    cat_task_sched_rdy(task);//挂就绪队列

    cat_task_exit_critical(status);
		
	return task;
}

//将任务从时间控制块等待队列中移除
void cat_event_remove_task(struct _cat_TCB_t *task, void *msg, uint32_t result)
{
    uint32_t status = cat_task_enter_critical();

    cat_list_remove(&(task->wait_event->wait_list), &(task->link_node));
    task->wait_event = NULL;//清空
    task->event_msg = msg;
    task->wait_event_result = result;
    task->state &= ~CATOS_TASK_EVENT_MASK;//清除掉设置的事件相关状态
    
    //这里没有对延时进行处理，因为此函数会在cat_task_systemtick_handler(void)中
    //并且在其中对延时处理之前被调用，所以延时相关处理在这里不必管

    cat_task_exit_critical(status);
}


uint32_t cat_event_remove_all(struct _cat_event_t *event, void *msg, uint32_t result)
{
    struct _cat_node_t *node;
    uint32_t count = 0;

    uint32_t status = cat_task_enter_critical();

    count = cat_list_count(&(event->wait_list));

    while((node = cat_list_remove_first(&(event->wait_list))) != NULL)
    {
        struct _cat_TCB_t *task = (struct _cat_TCB_t *)NODE_PARENT(node, struct _cat_TCB_t, link_node);
        
        task->wait_event = NULL;
        task->event_msg = msg;
        task->wait_event_result = result;
        task->state &= ~(CATOS_TASK_EVENT_MASK);

        //如果还没超时，就从延时队列取出
        if(task->task_delay != 0)
        {
            cat_task_time_wakeup(task);
        }
/*********************************************************/
/*********************************************************/
        //cat_task_sched();致命错误！！！！！这里应该是挂就绪队列而不是调度
/*********************************************************/
/*********************************************************/
        cat_task_sched_rdy(task);//挂就绪队列
    }

    cat_task_exit_critical(status);

    return count;
}

uint32_t cat_event_wait_count(struct _cat_event_t *event)
{
    uint32_t count;

    uint32_t status = cat_task_enter_critical();
    count = cat_list_count(&(event->wait_list));
    cat_task_exit_critical(status);

    return count;
}

