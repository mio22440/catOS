/**
 * @file cat_event.h
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

#ifndef CAT_EVENT_H
#define CAT_EVENT_H

#include "cat_list.h"
#include "cat_task.h"

#include "catos_declares.h"

typedef enum _cat_event_type_t cat_event_type_t;

struct _task_TCB;//defined in cat_task.h

enum _cat_event_type_t
{
    cat_event_type_unkown = 0,
    cat_event_type_sem,//信号量
    cat_event_type_mailbox,//邮箱
    cat_event_type_memblock,//内存块
    cat_event_type_flaggrp,//标志组
    cat_event_type_mutex,//互斥量

};

struct _cat_event_t
{
    cat_event_type_t type;
    struct _cat_list_t wait_list;
};


//初始化事件控制块(设置事件类型)
void cat_event_init(struct _cat_event_t *event, cat_event_type_t type);

//等待事件
void cat_event_wait(struct _cat_event_t *event, struct _cat_TCB_t *task, void *msg, uint32_t state, uint32_t timeout);

//将第一个任务从等待事件中唤醒
struct _cat_TCB_t *cat_event_wakeup_first(struct _cat_event_t *event, void *msg, uint32_t result); 

//将指定的任务从等待中唤醒
struct _cat_TCB_t *cat_event_wakeup(struct _cat_event_t *event,struct _cat_TCB_t *task, void *msg, uint32_t result);

//将任务从时间控制块等待队列中移除
void cat_event_remove_task(struct _cat_TCB_t *task, void *msg, uint32_t result);

//将事件等待队列中的任务全部取出(并挂到就绪队列)
uint32_t cat_event_remove_all(struct _cat_event_t *event, void *msg, uint32_t result);

//事件的等待队列中任务个数
uint32_t cat_event_wait_count(struct _cat_event_t *event);

#endif
