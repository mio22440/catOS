/**
 * @file cat_timer.h
 * @brief 软件定时器
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

#ifndef CAT_TIMER_H
#define CAT_TIMER_H

//#include "cat_event.h"
#include "cat_list.h"
#include "catos_defs.h"

enum _cat_timer_state
{
    cat_timer_state_created,      /**< 已经创建完成*/
    cat_timer_state_started,      /**< 已经启动*/
    cat_timer_state_running,      /**< 定时器回调函数正在运行*/
    cat_timer_state_stopped,      /**< 已经停止运行*/
    cat_timer_state_destroyed,    /**< 已经被销毁*/
};
typedef enum _cat_timer_state cat_timer_state;

struct _cat_timer_t
{
    struct _cat_node_t  link_node;                  /**< 链表节点*/
    uint32_t            start_delay_ticks;          /**< 在软定时器启动后延时多久开始执行回调函数*/
    uint32_t            duration_ticks;             /**< 软定时器一个周期(以tick为单位)*/
    uint32_t            delay_ticks;                /**< 当前的递减计数值*/
    void                (* timer_cb)(void *arg);    /**< 软定时器回调*/
    void                *timer_arg;                 /**< 回调函数的参数*/
    uint32_t            config;                     /**< 软定时器的配置(按位)*/
    cat_timer_state     state;                      /**< 定时器状态*/
};

struct _cat_timer_info_t
{
    uint32_t            start_delay_ticks;          /**< 在软定时器启动后延时多久开始执行回调函数*/
    uint32_t            duration_ticks;             /**< 软定时器一个周期(以tick为单位)*/
    void                (* timer_cb)(void *arg);    /**< 软定时器回调*/
    void                *timer_arg;                 /**< 回调函数的参数*/
    uint32_t            config;                     /**< 软定时器的配置(按位)*/
    cat_timer_state     state;                      /**< 定时器状态*/
};


#define CAT_TIMER_CONFIG_TYPE_HARD      (1 << 0)    //在硬件定时器中断服务函数中
#define CAT_TIMER_CONFIG_TYPE_SOFT      (0 << 0)    //在软件定时任务重处理


/**
 * @brief 软定时器初始化
 * @param timer 
 * @param delay_ticks 
 * @param duration_ticks 
 * @param timer_cb 
 * @param timer_arg 
 * @param config 
 */
void cat_timer_init(
    struct _cat_timer_t *timer,
    uint32_t            delay_ticks,
    uint32_t            duration_ticks,
    void                (* timer_cb)(void *arg),
    void                *timer_arg,
    uint32_t            config
);

/**
 * @brief 初始化定时器任务
 */
void cat_timer_module_init(void);

/**
 * @brief 初始化定时器任务
 */
void cat_timer_task_init(void);

/**
 * @brief 启动定时器
 */
void cat_timer_start(struct _cat_timer_t *timer);

/**
 * @brief 停止定时器
 */
void cat_timer_stop(struct _cat_timer_t *timer);

/**
 * @brief 用于在系统时钟中断中的tick通知
 */
void cat_timer_module_tick_come_notify(void);

/**
 * @brief 删除定时器(从list移除)
 * @param timer 
 */
void cat_timer_destroy_timer(struct _cat_timer_t *timer);

/**
 * @brief 获取软定时器状态信息
 * @param timer 
 * @param info 
 */
void cat_timer_get_info(struct _cat_timer_t *timer, struct _cat_timer_info_t *info);

#endif
