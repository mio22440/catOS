/**
 * @file cat_sem.c
 * @brief 
 * @author amoigus (648137125@qq.com)
 * @version 0.1
 * @date 2021-03-25
 * 
 * @copyright Copyright (c) 2021
 * 
 * @par 修改日志：
 * Date              Version Author      Description
 * 2021-03-25 1.0    amoigus             内容
 */

#include "cat_sem.h"

//裁剪控制
#if (CATOS_ENABLE_SEM == 1)

void cat_sem_init(struct _cat_sem_t *sem, uint32_t start_cnt, uint32_t max_cnt)
{
    cat_event_init(&(sem->event), cat_event_type_sem);

    sem->max_cnt = max_cnt;

    if(max_cnt == 0)
    {
        sem->cur_cnt = start_cnt;
    }
    else
    {
        sem->cur_cnt = (start_cnt > max_cnt) ? max_cnt : start_cnt;
    }

}

uint32_t cat_sem_wait(struct _cat_sem_t *sem, uint32_t wait_ticks)
{
    uint32_t ret = cat_event_err_noerror;
    uint32_t status = cat_task_enter_critical();

    if(sem->cur_cnt > 0)
    {
        --(sem->cur_cnt);//减少计数信号量的值
        cat_task_exit_critical(status);
        ret = cat_event_err_noerror;
    }
    else
    {
        cat_event_wait(&(sem->event), cat_current_task, NULL, cat_event_type_sem, wait_ticks);
        cat_task_exit_critical(status);
        
        cat_task_sched();

        //等任务切换回来时取出结果
        ret = cat_current_task->wait_event_result;//这个值由cat_event_wait()设置
    }

    return ret;
}

uint32_t cat_sem_no_wait_get(struct _cat_sem_t *sem)
{
		uint32_t ret = cat_event_err_noerror;
    uint32_t status = cat_task_enter_critical();

    if(sem->cur_cnt > 0)
    {
        --(sem->cur_cnt);
        cat_task_exit_critical(status);
        ret = cat_event_err_noerror;
    }
    else
    {
        cat_task_exit_critical(status);
        ret = cat_event_err_resource_unabaliable;
    }
    
    return ret;
}

void cat_sem_notify(struct _cat_sem_t *sem)
{
    uint32_t status = cat_task_enter_critical();

    //有正在等待信号量的任务
    if(cat_event_wait_count(&(sem->event)) > 0)
    {
        struct _cat_TCB_t *task = cat_event_wakeup_first(&(sem->event), NULL, cat_event_err_noerror);
        
        //当取出的任务优先级比当前任务的优先级高
        if(task->prio < cat_current_task->prio)
        {
            //则进行一次调度，让高优先级任务先运行
            cat_task_sched();
        }
    }
    else
    {
        ++(sem->cur_cnt);

        //防止当前计数值超过最大计数值
        if((sem->max_cnt != 0) && (sem->cur_cnt > sem->max_cnt))
        {
            sem->cur_cnt = sem->max_cnt;
        }
    }

    cat_task_exit_critical(status);
}

//获取信号量状态信息
void cat_sem_get_info(struct _cat_sem_t *sem, struct _cat_sem_info_t *info)
{
    uint32_t status = cat_task_enter_critical();

    info->cur_cnt = sem->cur_cnt;
    info->max_cnt = sem->max_cnt;
    info->task_cnt = cat_event_wait_count(&(sem->event));

    cat_task_exit_critical(status);
}

//删除信号量
uint32_t cat_sem_destroy(struct _cat_sem_t *sem)
{
    uint32_t status = cat_task_enter_critical();

    uint32_t count = cat_event_remove_all(&(sem->event), NULL, cat_event_err_del);
    sem->cur_cnt = 0;

    cat_task_exit_critical(status);

    //如果被删除的信号量中有任务，则现在就绪队列中多了些任务，故进行一次调度
    if(count > 0)
    {
        cat_task_sched();
    }

    return count;
}

#endif
