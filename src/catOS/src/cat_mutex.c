/**
 * @file cat_mutex.c
 * @brief 
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

#include "cat_mutex.h"

#if (CATOS_ENABLE_MUTEX == 1)

//初始化互斥量
void cat_mutex_init(struct _cat_mutex_t *mutex)
{
    cat_event_init(&(mutex->event), cat_event_type_mutex);

    mutex->locked_cnt = 0;
    mutex->owner = NULL;
    mutex->owner_original_prio = CATOS_MAX_TASK_PRIO;//比最大优先级还要大的无效值
}

uint32_t cat_mutex_wait(struct _cat_mutex_t *mutex, uint32_t wait_ticks)
{
    uint32_t ret = 0;
    uint32_t status = cat_task_enter_critical();

    //如果互斥量还未被占有
    if(mutex->locked_cnt <= 0)
    {
        mutex->owner = cat_current_task;
        mutex->owner_original_prio = cat_current_task->prio;
        mutex->locked_cnt++;

        cat_task_exit_critical(status);
        ret = cat_event_err_noerror;
    }
    else
    {
        //如果是当前任务嵌套
        if(mutex->owner == cat_current_task)
        {
            mutex->locked_cnt++;
            cat_task_exit_critical(status);
            ret = cat_event_err_noerror;
        }
        else
        {
            //如果请求互斥量的不是当前任务
            //1 检查请求任务和已经获取互斥量任务的原始优先级
            if(cat_current_task->prio < mutex->owner->prio)
            {
                //如果当前任务优先级更高，则让已经获取的任务继承优先级
                struct _cat_TCB_t *owner = mutex->owner;
                
                if(owner->state == CATOS_TASK_STATE_RDY)
                {
                    //如果已获取的任务(owner)状态只有就绪态，说明其在就绪队列中，
                    //则先从就绪队列取下再更改其优先级，因为其优先级决定了在哪个就绪队列
                    cat_task_sched_unrdy(owner);
                    owner->prio = cat_current_task->prio;
                    cat_task_sched_rdy(owner);
                }
                else
                {
                    //否则说明owner任务不在就绪队列中，可以直接更改优先级
                    owner->prio = cat_current_task->prio;
                }
            }

            //2 把请求互斥量的任务挂到互斥量的等待队列中
            cat_event_wait(&(mutex->event), cat_current_task, NULL, cat_event_type_mutex, wait_ticks);
            cat_task_exit_critical(status);

            cat_task_sched();

            ret = cat_current_task->wait_event_result;
        }
    }

    return ret;
}

uint32_t cat_mutex_no_wait_get(struct _cat_mutex_t *mutex)
{
    uint32_t ret = 0;
    uint32_t status = cat_task_enter_critical();

    if(mutex->locked_cnt <= 0)
    {
        mutex->owner = cat_current_task;
        mutex->owner_original_prio = cat_current_task->prio;
        mutex->locked_cnt++;

        cat_task_exit_critical(status);
        ret = cat_event_err_noerror;
    }
    else
    {
        if(mutex->owner == cat_current_task)
        {
            mutex->locked_cnt++;
            cat_task_exit_critical(status);
            ret = cat_event_err_noerror;
        }
        else
        {
            cat_task_exit_critical(status);
            ret = cat_event_err_resource_unabaliable;
        }
    }

    return ret;
}

uint32_t cat_mutex_notify(struct _cat_mutex_t *mutex)
{
    uint32_t ret = 0;
    uint32_t status = cat_task_enter_critical();

    if(mutex->locked_cnt <= 0)
    {
        //当没有任务获取该互斥量，通知不起实际作用
        cat_task_exit_critical(status);
        ret = cat_event_err_noerror;
    }
    else if(mutex->owner != cat_current_task)
    {
        //不是拥有者任务时拒绝通知
        cat_task_exit_critical(status);
        ret = cat_event_err_owner;//不是拥有者
    }
    else
    {
        if(--mutex->locked_cnt > 0)
        {
            //说明拥有者退出了一层嵌套
            cat_task_exit_critical(status);
            ret = cat_event_err_noerror;
        }
        else
        {
            //(--mutex->locked_cnt == 0)
            //此时拥有者应该释放该互斥量
            //1 先检查所有者的优先级
            if(mutex->owner->prio != mutex->owner_original_prio)
            {
                struct _cat_TCB_t *owner = mutex->owner;

                //若有优先级继承，则将优先级改回
                if(owner->state == CATOS_TASK_STATE_RDY)
                {
                    cat_task_sched_unrdy(owner);
                    cat_current_task->prio = mutex->owner_original_prio;//改回
                    cat_task_sched_rdy(owner);
                }
                else
                {
                    //否则说明owner任务不在就绪队列中，可以直接更改优先级
                    cat_current_task->prio = mutex->owner_original_prio;
                }
            }

            //2 检查是否有任务在等待获取互斥量
            if(cat_event_wait_count(&(mutex->event)) > 0)
            {
                //若有任务等待则唤醒第一个等待的任务**********
                /*************需要改进***********************/
                struct _cat_TCB_t *task = cat_event_wakeup_first(&(mutex->event), NULL, cat_event_err_noerror);
                /********************************************/
                /********************************************/

                //将互斥量的拥有者设置为该任务
                mutex->owner = task;
                mutex->owner_original_prio = task->prio;
                mutex->locked_cnt++;

                if(task->prio < cat_current_task->prio)
                {
                    cat_task_sched();
                }
            }

            cat_task_exit_critical(status);
            ret = cat_event_err_noerror;
        }//else
    }//else
    
    return ret;
}

//删除互斥量
uint32_t cat_mutex_destroy(struct _cat_mutex_t *mutex)
{
    uint32_t count = 0;
    uint32_t status = cat_task_enter_critical();

    //如果有上锁
    if(mutex->locked_cnt > 0)
    {
        //1 如果有优先级继承，则取回优先级
        if(mutex->owner_original_prio != mutex->owner->prio)
        {
            //把优先级取回
            if(mutex->owner->state == CATOS_TASK_STATE_RDY)
            {
                cat_task_sched_unrdy(mutex->owner);
                mutex->owner->prio = mutex->owner_original_prio;
                cat_task_sched_rdy(mutex->owner);
            }
            else
            {
                mutex->owner->prio = mutex->owner_original_prio;
            }
        }

        //2 移除所有等待的任务并放入就绪队列
        count = cat_event_remove_all(&(mutex->event), NULL, cat_event_err_del);

        if(count > 0)
        {
            cat_task_sched();
        }
    }

    cat_task_exit_critical(status);
    return count;
}

//获取互斥量状态信息
void cat_mutex_get_info(struct _cat_mutex_t *mutex, struct _cat_mutex_info_t *info)
{
    uint32_t status = cat_task_enter_critical();

    info->task_cnt = cat_event_wait_count(&(mutex->event));
    info->owner_prio = mutex->owner_original_prio;
    
    if(NULL != mutex->owner)
    {
        //如果有拥有者，才获取继承优先级
        info->inherit_prio = mutex->owner->prio;
    }
    else
    {
        //否则给一个非法值
        info->inherit_prio = CATOS_MAX_TASK_PRIO;
    }

    info->owner = mutex->owner;
    info->locked_cnt = mutex->locked_cnt;

    cat_task_exit_critical(status);
}

#endif
