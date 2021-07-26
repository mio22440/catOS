/**
 * @file cat_hooks.h
 * @brief 钩子函数
 * @author amoigus (648137125@qq.com)
 * @version 0.1
 * @date 2021-03-28
 * 
 * @copyright Copyright (c) 2021
 * 
 * @par 修改日志：
 * Date              Version Author      Description
 * 2021-03-28 1.0    amoigus             内容
 */

#ifndef CAT_HOOKS_H
#define CAT_HOOKS_H

#include "catos_declares.h"
#include "catos_config.h"

void cat_hooks_idle(void);

void cat_hooks_systick(void);

void cat_hooks_task_before_switch(struct _cat_TCB_t *from, struct _cat_TCB_t *to);

void cat_hooks_task_init(struct _cat_TCB_t *task);

#endif

