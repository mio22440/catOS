/**
 * @file cat_config.h
 * @brief the config MACROs
 * @author amoigus (648137125@qq.com)
 * @version 0.1
 * @date 2021-03-19
 * 
 * @copyright Copyright (c) 2021
 * 
 * @par 修改日志：
 * Date              Version Author      Description
 * 2021-03-19 1.0    amoigus             内容
 */

#ifndef CATOS_CONFIG_H
#define CATOS_CONFIG_H


/** 调试相关 **/
#define DEBUG_PRINT_ENABLE          (1)     /**< 使能调试打印功能 */

/** 系统相关 **/
#define CATOS_SYSTICK_FRQ           (100)   /**< 系统时钟频率，每秒的tick数(周期的倒数)*/
#define CATOS_SYSTICK_MS            ((1000 * 1) / CATOS_SYSTICK_FRQ) /**< 每个tick经过的毫秒数*/

/** task 相关 **/
#define CATOS_MAX_TASK_NAME_LEN     (50u)   /**<*/
//#define CATOS_MAX_TASK_PRIO         (64u)//must be 64 if use unmap_tbl
#define CATOS_MAX_TASK_PRIO         (32)    /**< 任务最大优先级*/
#define CATOS_MAX_SLICE             (10)    /**<*///最大时间片

#define CATOS_IDLE_STACK_SIZE       (1024)  /**< 空闲任务栈大小*/

#define CATOS_TIMERTASK_STACK_SIZE  (1024)  /**< 软定时器任务栈大小*/
#define CATOS_TIMERTASK_PRIO        (1)     /**< 软定时器任务优先级*/

#define CATOS_RDY_TBL_LEN           ((CATOS_MAX_TASK_PRIO + 7) >> 3) /**< raw number of ready table(align 8)*/
#define CATOS_RDY_GRP_LEN           ((CATOS_RDY_TBL_LEN + 7) >> 3)

/** 裁剪 **/

#define CATOS_ENABLE_SEM            1
#define CATOS_ENABLE_MUTEX          1
#define CATOS_ENABLE_FLAGGRP        0
#define CATOS_ENABLE_MAILBOX        0
#define CATOS_ENABLE_MEMBLOCK       0
#define CATOS_ENABLE_TIMER          1
#define CATOS_ENABLE_CPUUSAGE_STAT  0   /**< 暂时别用！！！idle和shell任务的优先级导致idle不会执行*/

#define CATOS_ENABLE_HOOKS          0   /**< 钩子函数*/

#define CATOS_ENABLE_SHELL          1
#define CATOS_ENABLE_SYS_PRINT      1  /**< 定义内核的输出(通常为终端输出)*/

/* 任务统计 */
#define CATOS_TASK_STAT_MAX_TASK    (20)
#define CATOS_ENABLE_TASK_STAT      1  /**< 是否使用task信息统计功能 */

#define CATOS_USE_SW_RESET          1 /**< 软复位*/

#endif
