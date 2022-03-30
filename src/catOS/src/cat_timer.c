/**
 * @file cat_timer.c
 * @brief 软定时器
 * @author amoigus (648137125@qq.com)
 * @version 0.1
 * @date 2021-03-27
 * 
 * @copyright Copyright (c) 2021
 * 
 * @par 修改日志：
 * Date              Version Author      Description
 * 2021-03-27 1.0    amoigus             内容
 */

#include "cat_timer.h"
#include "cat_sem.h"

#if (CATOS_ENABLE_TIMER == 1)


/****************** global vars start ***********************/
static struct _cat_list_t cat_timer_hard_list;         /**<*/
static struct _cat_list_t cat_timer_soft_list;         /**<*/
static struct _cat_sem_t  cat_timer_access_sem;        /**< 定时器任务和其他任务访问softlist */
static struct _cat_sem_t  cat_timer_tick_come_sem;     /**< 在中断服务函数中提醒软定时器任务"有中断发生了"*/


static struct _cat_TCB_t cat_timer_task;                                           /**< 定时器任务任务控制块*/
static cat_stack_type_t cat_timer_task_stack[CATOS_TIMERTASK_STACK_SIZE];   /**< 定时器任务堆栈*/
/****************** global vars end ***********************/


/****************** static funcs start ***********************/

static void cat_timer_soft_task(void *arg);
static void cat_timer_call_func_list(struct _cat_list_t *timer_list);

/**
 * @brief 软定时器的任务入口函数
 * @param arg 
 */
static void cat_timer_soft_task(void *arg)
{
    for(;;)
    {
        //等待tick到来
        cat_sem_wait(&cat_timer_tick_come_sem, 0);

        cat_sem_wait(&cat_timer_access_sem, 0);

        cat_timer_call_func_list(&cat_timer_soft_list);

        cat_sem_notify(&cat_timer_access_sem);

    }
}

/**
 * @brief 依次处理队列中的定时器
 * @param timer_list 
 */
static void cat_timer_call_func_list(struct _cat_list_t *timer_list)
{
    struct _cat_node_t *node;
    for(node = timer_list->head_node.next_node; node != &(timer_list->head_node); node = node->next_node)
    {
        struct _cat_timer_t *timer = NODE_PARENT(node, struct _cat_timer_t, link_node);
        
        //如果延时时间到达了或者即将到达,则调用处理函数
        if((timer->delay_ticks == 0) || (--timer->delay_ticks == 0))
        {
            timer->state = cat_timer_state_running;
            timer->timer_cb(timer->timer_arg);
            timer->state = cat_timer_state_started;

            if(timer->duration_ticks > 0)
            {
                timer->delay_ticks = timer->duration_ticks;
            }
            else
            {
                cat_list_remove(timer_list, &(timer->link_node));
                timer->state = cat_timer_state_stopped;
            }
        }
    }
}

/****************** static funcs end ***********************/

//初始化定时器任务
void cat_timer_module_init(void)
{
    cat_list_init(&cat_timer_hard_list);
    cat_list_init(&cat_timer_soft_list);
    cat_sem_init(&cat_timer_access_sem, 1, 1);
    cat_sem_init(&cat_timer_tick_come_sem, 0, 0);//不限制最大值
}

/**
 * @brief 初始化定时器任务
 */
void cat_timer_task_init(void)
{
//保证时钟任务优先级比空闲任务优先级高
#if CATOS_TIMERTASK_PRIO >= (CATOS_MAX_TASK_PRIO - 1)
    #error "priority of timer task must be higher than idle task"
#endif
    task_init(
        "timer_task", 
        &cat_timer_task, 
        cat_timer_soft_task, 
        NULL, 
        CATOS_TIMERTASK_PRIO, 
        cat_timer_task_stack, 
        CATOS_TIMERTASK_STACK_SIZE,
        SCHED_STRATEGY_PRIO    
    );
}

void cat_timer_init(
    struct _cat_timer_t *timer,
    uint32_t            delay_ticks,
    uint32_t            duration_ticks,
    void                (* timer_cb)(void *arg),
    void                *timer_arg,
    uint32_t            config
)
{
    cat_node_init(&(timer->link_node));

    timer->start_delay_ticks = delay_ticks;
    timer->duration_ticks = duration_ticks;
    timer->timer_cb = timer_cb;
    timer->timer_arg = timer_arg;
    timer->config = config;

    if(delay_ticks == 0)
    {
        timer->delay_ticks = duration_ticks;
    }
    else
    {
        timer->delay_ticks = timer->start_delay_ticks;
    }

    timer->state = cat_timer_state_created;
}

void cat_timer_start(struct _cat_timer_t *timer)
{
    switch(timer->state)
    {
        case cat_timer_state_created:
        case cat_timer_state_stopped:
            timer->delay_ticks = timer->start_delay_ticks ? timer->start_delay_ticks : timer->duration_ticks;
            timer->state = cat_timer_state_started;

            if(timer->config & CAT_TIMER_CONFIG_TYPE_HARD)
            {
                uint32_t status = cat_task_enter_critical();
                cat_list_add_first(&cat_timer_hard_list, &(timer->link_node));
                cat_task_exit_critical(status);
            }
            else
            {
                //这里只用信号量而不用关中断，要思考
                //不用互斥量是因为用不到那么多特性
                cat_sem_wait(&cat_timer_access_sem, 0);
                cat_list_add_last(&cat_timer_soft_list, &(timer->link_node));
                cat_sem_notify(&cat_timer_access_sem);
            }

            break;
        default:
            break;
    }
}

void cat_timer_stop(struct _cat_timer_t *timer)
{
    switch(timer->state)
    {
        case cat_timer_state_started:
        case cat_timer_state_running:
            timer->state = cat_timer_state_stopped;

            if(timer->config & CAT_TIMER_CONFIG_TYPE_HARD)
            {
                uint32_t status = cat_task_enter_critical();
                cat_list_remove(&cat_timer_hard_list, &(timer->link_node));
                cat_task_exit_critical(status);
            }
            else
            {
                /****************************************/
                //这里只用信号量而不用关中断，要思考
                /****************************************/
                //不用互斥量是因为用不到那么多特性
                cat_sem_wait(&cat_timer_access_sem, 0);
                cat_list_remove(&cat_timer_soft_list, &(timer->link_node));
                cat_sem_notify(&cat_timer_access_sem);
            }

            break;
        default:
            break;
    }
}

void cat_timer_module_tick_come_notify(void)
{
    //处理hardlist上的定时器
    uint32_t status = cat_task_enter_critical();
    cat_timer_call_func_list(&cat_timer_hard_list);
    cat_task_exit_critical(status);

    //通知定时器任务tick来了
    cat_sem_notify(&cat_timer_tick_come_sem);
}

void cat_timer_destroy_timer(struct _cat_timer_t *timer)
{
    cat_timer_stop(timer);

    //一定要先调用stop然后再设置状态，否则stop中不满足条件就不会执行stop
    timer->state = cat_timer_state_destroyed;
}

void cat_timer_get_info(struct _cat_timer_t *timer, struct _cat_timer_info_t *info)
{
    uint32_t status = cat_task_enter_critical();

    info->start_delay_ticks = timer->start_delay_ticks;
    info->duration_ticks = timer->duration_ticks;
    info->timer_cb = timer->timer_cb;
    info->timer_arg = timer->timer_arg;
    info->config = timer->config;
    info->state = timer->state;

    cat_task_exit_critical(status);
}

#endif
