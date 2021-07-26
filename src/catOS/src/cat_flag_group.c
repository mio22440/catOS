/**
 * @file cat_flag_group.c
 * @brief 
 * @author amoigus (648137125@qq.com)
 * @version 0.1
 * @date 2021-03-26
 * 
 * @copyright Copyright (c) 2021
 * 
 * @par 修改日志：
 * Date              Version Author      Description
 * 2021-03-26 1.0    amoigus             内容
 */

#include "cat_flag_group.h"

#if (CATOS_ENABLE_FLAGGRP == 1)

//初始化事件标志组
void cat_flag_group_init(struct _cat_flag_group_t *fg, uint32_t flags)
{
    cat_event_init(&(fg->event), cat_event_type_flaggrp);
    fg->flag = flags;
}

/**
 * @brief 辅助函数，查看并按需求置零标志组的相应位
 * @param fg 
 * @param type      事件等待的标志位类型
 * @param flags     事件期待的标志,如果符合则不变，否则只留下符合预期的位
 * @return uint32_t cat_event_err_noerror：             符合
 *                  cat_event_err_resource_unabaliable：不符合
 */
static uint32_t private_flggrp_check_and_consume(struct _cat_flag_group_t *fg,uint32_t type, uint32_t *flags)
{
    uint32_t ret = 0;

    uint32_t flag_src = *flags;//拷贝

    //获取类型中的不同位
    uint32_t is_set     = type & _BASE_CAT_FLAG_GRP_SET;
    uint32_t is_all     = type & _BASE_CAT_FLAG_GRP_ALL;
    uint32_t is_consume = type & CAT_FLAG_GRP_CONSUME;

    //用与的方式来得知是否相应位处于期待的状态
    uint32_t flag_cal = is_set ? (fg->flag & flag_src) : (~fg->flag & flag_src);

    if(((is_all != 0) && (flag_cal == flag_src)) || ((is_all == 0) && (flag_cal != 0)))
    {
        if(is_consume)
        {
            if(is_set)
            {
                //将相应标志位置零
                fg->flag &= ~flag_src;
            }
            else
            {
                //将相应标志位置位
                fg->flag |= flag_src;
            }//else
        }//is_consume

        //*flags = flag_cal;
        ret = cat_event_err_noerror;
    }
    else
    {
        ret = cat_event_err_resource_unabaliable;
    }

    *flags = flag_cal;
    return ret;
}

uint32_t cat_flag_group_wait(struct _cat_flag_group_t *fg, uint32_t wait_type, uint32_t request_flag, uint32_t *result_flag, uint32_t wait_ticks)
{
    uint32_t ret = 0;
    uint32_t flags = request_flag;
    uint32_t status = cat_task_enter_critical();

    ret = private_flggrp_check_and_consume(fg, wait_type, &flags);

    //如果目前标志位不符合，则等待
    if(ret != cat_event_err_noerror)
    {
        cat_current_task->wait_flags_type = wait_type;
        cat_current_task->wait_event_flags = request_flag;
        cat_event_wait(&(fg->event), cat_current_task, NULL, cat_event_type_flaggrp, wait_ticks);

        cat_task_exit_critical(status);

        cat_task_sched();

        //切回来后取出相应的值
        *result_flag = cat_current_task->wait_event_flags;//操作后的flags
        ret = cat_current_task->wait_event_result;
    }
    else
    {
        //否则取得操作后的flags后直接返回
        *result_flag = flags;
        cat_task_exit_critical(status);
    }

    return ret;
}


uint32_t cat_flag_group_no_wait_get(struct _cat_flag_group_t *fg, uint32_t wait_type, uint32_t request_flag, uint32_t *result_flag)
{
    uint32_t flags = request_flag;

    uint32_t status = cat_task_enter_critical();
    uint32_t result = private_flggrp_check_and_consume(fg, wait_type, &flags);
    cat_task_exit_critical(status);

    *result_flag = flags;

    return cat_event_err_noerror;
}


void cat_flag_group_notify(struct _cat_flag_group_t *fg, uint32_t is_set, uint32_t flag)
{
    struct _cat_list_t *wait_list;//指向标志组等待队列的指针
    struct _cat_node_t *node;//两个用来遍历等待任务队列的指针
    struct _cat_node_t *next_node;

    uint8_t need_sched_flg = 0;//是否需要调度 0：不需要 1：需要

    uint32_t status = cat_task_enter_critical();

    //设置相应标志位
    if(is_set)
    {
        fg->flag |= flag;
    }
    else
    {
        fg->flag &= ~flag;
    }

    //根据设置后的标志位检查等待的任务
    wait_list = &(fg->event.wait_list);
    for(node = wait_list->head_node.next_node; node != &(wait_list->head_node); node = next_node)
    {
        uint32_t ret;
        struct _cat_TCB_t *task = (struct _cat_TCB_t *)NODE_PARENT(node, struct _cat_TCB_t, link_node);
        uint32_t flags = task->wait_event_flags;
        next_node = node->next_node;

        ret = private_flggrp_check_and_consume(fg, task->wait_flags_type, &flags);

        //如果符合预期
        if(ret == cat_event_err_noerror)
        {
            task->wait_event_flags = flags;//和预期标志位一样
            cat_event_wakeup(&(fg->event), task, NULL, cat_event_err_noerror);

            //置位调度标志
            need_sched_flg = 1;
        }
    }

    if(need_sched_flg)
    {
        cat_task_sched();
    }

    cat_task_exit_critical(status);
}


//获取当前事件标志组状态信息
void cat_flag_group_get_info(struct _cat_flag_group_t *fg, struct _cat_flag_group_info_t *info)
{
    uint32_t status = cat_task_enter_critical();

    info->flag = fg->flag;
    info->task_cnt = cat_event_wait_count(&(fg->event));

    cat_task_exit_critical(status);
}

//删除事件标志组
uint32_t cat_flag_group_destroy(struct _cat_flag_group_t *fg)
{
    uint32_t count = 0;

    uint32_t status = cat_task_enter_critical();
    count = cat_event_remove_all(&(fg->event), NULL, cat_event_err_del);
    cat_task_exit_critical(status);

    if(count > 0)
    {
        cat_task_sched();
    }

    return count;
}


#endif
