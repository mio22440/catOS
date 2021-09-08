/**
 * @file cat_mutex.h
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

#ifndef CAT_MUTEX_H
#define CAT_MUTEX_H

#include "cat_event.h"
#include "catos_declares.h"

struct _cat_mutex_t
{
    struct _cat_event_t event;                  /**< 事件控制块*/
    uint32_t            locked_cnt;             /**< 被锁定次数*/
    struct _cat_TCB_t   *owner;                 /**< 拥有者*/
    uint32_t            owner_original_prio;    /**< 拥有者继承优先级之前的原始优先级*/
};

struct _cat_mutex_info_t
{
    uint32_t            task_cnt;
    uint32_t            owner_prio;
    uint32_t            inherit_prio;
    struct _cat_TCB_t   *owner;
    uint32_t            locked_cnt;
};

//初始化互斥量
void cat_mutex_init(struct _cat_mutex_t *mutex);

//等待互斥量(给互斥量上锁)
uint32_t cat_mutex_wait(struct _cat_mutex_t *mutex, uint32_t wait_ticks);

//非阻塞(尝试给互斥量上锁)
uint32_t cat_mutex_no_wait_get(struct _cat_mutex_t *mutex);

//通知互斥量(开锁)
uint32_t cat_mutex_notify(struct _cat_mutex_t *mutex);

//删除互斥量
uint32_t cat_mutex_destroy(struct _cat_mutex_t *mutex);

//获取互斥量状态信息
void cat_mutex_get_info(struct _cat_mutex_t *mutex, struct _cat_mutex_info_t *info);

#endif
