/**
 * @file cat_time.h
 * @brief 与任务延时相关
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

#ifndef CAT_TIME_H
#define CAT_TIME_H

#include "cat_task.h"

#include "catos_defs.h"
#include "catos_config.h"

void cat_get_ticks(uint32_t *ticks);

//延时delay个系统时钟
void cat_task_delay(uint32_t delay);

//初始化延时列表
void cat_task_delayed_init(void);

//插入延时列表
void cat_task_time_wait(struct _cat_TCB_t *task, uint32_t ticks);

//从延时唤醒(从延时表取出并改变状态)
void cat_task_time_wakeup(struct _cat_TCB_t *task);

//从延时表取下，但不做其他操作(目前用于任务删除)
void cat_task_time_remove(struct _cat_TCB_t *task);



#endif
