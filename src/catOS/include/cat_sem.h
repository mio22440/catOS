/**
 * @file cat_sem.h
 * @brief 
 * @author amoigus (648137125@qq.com)
 * @version 0.1
 * @date 2021-03-25
 * 
 * @copyright Copyright (c) 2021
 * 
 * @par 修改日志：
 * Date              Version Author      Description
 * 2021-03-25 1.0    amoigus             计数信号量
 */

#ifndef CAT_SEM_H
#define CAT_SEM_H

#include "cat_event.h"

struct _cat_sem_t
{
    struct _cat_event_t event;//事件控制块
    uint32_t cur_cnt;//当前计数值
    uint32_t max_cnt;//最大计数值(为0表示不限制)
};

//信号量的状态信息
struct _cat_sem_info_t
{
    uint32_t cur_cnt;//当前计数值
    uint32_t max_cnt;//最大计数值
    uint32_t task_cnt;//在等待信号量的任务数量
};

//初始化计数信号量
void cat_sem_init(struct _cat_sem_t *sem, uint32_t start_cnt, uint32_t max_cnt);

//wait操作
uint32_t cat_sem_wait(struct _cat_sem_t *sem, uint32_t wait_ticks);

//非阻塞的wait操作
uint32_t cat_sem_no_wait_get(struct _cat_sem_t *sem);

//post操作
void cat_sem_notify(struct _cat_sem_t *sem);

//获取信号量状态信息
void cat_sem_get_info(struct _cat_sem_t *sem, struct _cat_sem_info_t *info);

//删除信号量
uint32_t cat_sem_destroy(struct _cat_sem_t *sem);

#endif
