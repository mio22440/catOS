/**
 * @file cat_idle.h
 * @brief 空闲任务相关
 * @author amoigus (648137125@qq.com)
 * @version 0.1
 * @date 2021-03-29
 * 
 * @copyright Copyright (c) 2021
 * 
 * @par 修改日志：
 * Date              Version Author      Description
 * 2021-03-29 1.0    amoigus             内容
 */

#ifndef CAT_IDLE_H
#define CAT_IDLE_H

#include "catos_config.h"
#include "catos_defs.h"

void cat_idle_task_init(void);

/**
 * @brief 初始化系统tick数(tick_count清零)
 */
void time_tick_init(void);

void cat_idle_entry(void *arg);

#if (CATOS_ENABLE_CPUUSAGE_STAT == 1)
/**
 * @brief 初始化cpu统计的变量
 */
void init_cpu_usage_stat(void);

#if 0
    float cat_cpu_usage_get(void);
#else
    int32_t cat_cpu_usage_get(uint32_t *integer, uint32_t *decimal);
#endif

void cpu_usage_stat(void);

void cpu_usage_sync_with_systick(void);
#endif

#endif
