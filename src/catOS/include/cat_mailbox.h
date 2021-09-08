/**
 * @file cat_mailbox.h
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

#ifndef CAT_MAILBOX_H
#define CAT_MAILBOX_H

#include "cat_event.h"

#define CAT_MAILBOX_OPT_SEND_NORMAL     0x00 //正常放入消息
#define CAT_MAILBOX_OPT_SEND_FRONT      0x01 //在当前读指针的前一位放入消息

struct _cat_mailbox_t
{
    struct _cat_event_t event;//事件控制块
    uint32_t            msg_cnt;//当前消息数量
    uint32_t            read_index;//读消息索引
    uint32_t            write_index;//写消息索引
    uint32_t            max_msg_cnt;//最大消息数
    void                **msg_buffer;//消息缓冲区
};

struct _cat_mailbox_info_t
{
    uint32_t            msg_cnt;//当前消息数量
    uint32_t            max_msg_cnt;//最大消息数
    uint32_t            task_cnt;//正在等待的任务数
};

//初始化邮箱
void cat_mailbox_init(struct _cat_mailbox_t *mailbox, void **msg_buffer, uint32_t max_msg_cnt);

//从邮箱取出消息
uint32_t cat_mailbox_wait(struct _cat_mailbox_t *mailbox, void **msg, uint32_t wait_ticks);

//非阻塞取出消息
uint32_t cat_mailbox_no_wait_get(struct _cat_mailbox_t *mailbox, void **msg);

//把消息放入邮箱
uint32_t cat_mailbox_notify(struct _cat_mailbox_t *mailbox, void *msg, uint32_t notify_option);

//清空邮箱消息
void cat_mailbox_clear_msg(struct _cat_mailbox_t *mailbox);

//删除邮箱(清空邮箱等待队列)
uint32_t cat_mailbox_destroy(struct _cat_mailbox_t * mailbox);

//获取邮箱当前状态信息
void cat_mailbox_get_info(struct _cat_mailbox_t *mailbox, struct _cat_mailbox_info_t *info);

#endif
