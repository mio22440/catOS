/**
 * @file cat_mailbox.c
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

#include "cat_mailbox.h"

#if (CATOS_ENABLE_MAILBOX == 1)

/**
    uint32_t status = cat_task_enter_critical();

    cat_task_exit_critical(status);
 */

//初始化邮箱
void cat_mailbox_init(struct _cat_mailbox_t *mailbox, void **msg_buffer, uint32_t max_msg_cnt)
{
    cat_event_init(&(mailbox->event), cat_event_type_mailbox);

    mailbox->msg_buffer = msg_buffer;
    mailbox->max_msg_cnt = max_msg_cnt;
    mailbox->read_index = 0;
    mailbox->write_index = 0;
    mailbox->msg_cnt = 0;
}

uint32_t cat_mailbox_wait(struct _cat_mailbox_t *mailbox, void **msg, uint32_t wait_ticks)
{
    uint32_t ret = 0;
    uint32_t status = cat_task_enter_critical();

    if(mailbox->msg_cnt > 0)
    {
        --(mailbox->msg_cnt);

        //先放再加
        *msg = mailbox->msg_buffer[mailbox->read_index++];

        //如果超出了就置零
        if(mailbox->read_index >= mailbox->max_msg_cnt)
        {
            mailbox->read_index = 0;
        }

        cat_task_exit_critical(status);
        ret = cat_event_err_noerror;
    }
    else
    {
        cat_event_wait(&(mailbox->event), cat_current_task, NULL, cat_event_type_mailbox, wait_ticks);
        cat_task_exit_critical(status);

        cat_task_sched();

        //切回该任务后取出消息和结果
        *msg = cat_current_task->event_msg;
        ret = cat_current_task->wait_event_result;
    }

    return ret;
}

uint32_t cat_mailbox_no_wait_get(struct _cat_mailbox_t *mailbox, void **msg)
{
    uint32_t ret = 0;
    uint32_t status = cat_task_enter_critical();

    if(mailbox->msg_cnt > 0)
    {
        --(mailbox->msg_cnt);
        *msg = mailbox->msg_buffer[mailbox->read_index++];
        if(mailbox->read_index >= mailbox->max_msg_cnt)
        {
            mailbox->read_index = 0;
        }
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

uint32_t cat_mailbox_notify(struct _cat_mailbox_t *mailbox, void *msg, uint32_t notify_option)
{
    uint32_t status = cat_task_enter_critical();

    if(cat_event_wait_count(&(mailbox->event)) > 0)
    {
        struct _cat_TCB_t *task = cat_event_wakeup_first(&(mailbox->event), (void *)msg, cat_event_err_noerror);
        if(task->prio < cat_current_task->prio)
        {
            cat_task_sched();
        }
    }
    else
    {
        //如果缓冲区满，就不放入，返回资源已满
        if(mailbox->msg_cnt >= mailbox->max_msg_cnt)
        {
            cat_task_exit_critical(status);
            return cat_event_err_resource_full;
        }

        if(notify_option & CAT_MAILBOX_OPT_SEND_FRONT)
        {
            //在现有的read_index指针前一位放入消息
            if(mailbox->read_index <= 0)
            {
                mailbox->read_index = mailbox->max_msg_cnt - 1;
            }
            else
            {
                --mailbox->read_index;
            }

            mailbox->msg_buffer[mailbox->read_index] = msg;
        }
        else
        {
            //正常放入消息
            mailbox->msg_buffer[mailbox->write_index++] = msg;
            if(mailbox->write_index >= mailbox->max_msg_cnt)
            {
                mailbox->write_index = 0;
            }
        }

        mailbox->msg_cnt++;
    }

    cat_task_exit_critical(status);
    return cat_event_err_noerror;

}

void cat_mailbox_clear_msg(struct _cat_mailbox_t *mailbox)
{
    uint32_t status = cat_task_enter_critical();

    if(cat_event_wait_count(&(mailbox->event)) == 0)
    {
        mailbox->read_index = 0;
        mailbox->write_index = 0;
        mailbox->msg_cnt = 0;
    }

    cat_task_exit_critical(status);
}

uint32_t cat_mailbox_destroy(struct _cat_mailbox_t * mailbox)
{
    uint32_t count = 0;

    uint32_t status = cat_task_enter_critical();
    count = cat_event_remove_all(&(mailbox->event), NULL, cat_event_err_del);
    cat_task_exit_critical(status);

    if(count > 0)
    {
        cat_task_sched();
    }

    return count;
}

//获取邮箱当前状态信息
void cat_mailbox_get_info(struct _cat_mailbox_t *mailbox, struct _cat_mailbox_info_t *info)
{
    uint32_t status = cat_task_enter_critical();

    info->msg_cnt = mailbox->msg_cnt;
    info->max_msg_cnt = mailbox->max_msg_cnt;
    info->task_cnt = cat_event_wait_count(&(mailbox->event));

    cat_task_exit_critical(status);
}


#endif
