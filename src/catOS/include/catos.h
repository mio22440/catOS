/**
 * @file catos.h
 * @brief 包含所有头文件
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

#ifndef CATOS_H
#define CATOS_H

#include "catos_defs.h"
#include "catos_config.h"

#include "cat_list.h"
#include "cat_task.h"
#include "cat_time.h"
#include "cat_lib.h"
#include "cat_event.h"
#include "cat_sem.h"
#include "cat_mailbox.h"
#include "cat_mem.h"
#include "cat_flag_group.h"
#include "cat_mutex.h"
#include "cat_timer.h"
#include "cat_hooks.h"
#include "cat_idle.h"

#include "port.h"


/**
 * @brief 系统初始化
 */
void catos_init(void);

/**
 * @brief 系统时钟中断处理函数
 *        处理tick等
 */
void cat_task_systemtick_handler(void);

#endif
