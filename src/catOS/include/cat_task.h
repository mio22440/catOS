/**
 * @file cat_task.h
 * @brief task 定义
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

#ifndef CAT_TASK_H
#define CAT_TASK_H

#include "cat_list.h"

#include "catos_defs.h"
#include "catos_config.h"

#include "strategy.h"
#include "edf.h"

#define CATOS_TASK_STATE_RDY        0               //目前不具备判断功能
#define CATOS_TASK_STATE_DESTROYED  (1 << 1)        //删除
#define CATOS_TASK_STATE_DELAYED    (1 << 2)        //延时
#define CATOS_TASK_STATE_SUSPEND    (1 << 3)        //挂起

#define CATOS_TASK_EVENT_MASK       (0xff << 16)    //高16位用作事件相关的状态


extern void cat_set_systick_period(uint32_t ms);//初始化中断

extern void cat_task_sched(void);
extern void cat_start_first(void);
extern void cat_task_switch(void);
extern uint32_t cat_task_enter_critical(void);
extern void cat_task_exit_critical(uint32_t status);

extern void cat_task_systemtick_handler(void);

/*************** type defines *****************/


struct _cat_TCB_t
{
    cat_stack_type_t    *stack_top;                     /**< 栈顶(堆栈指针)*/
    uint32_t            *stack_start_addr;              /**< 堆栈起始地址*/
    uint32_t            stack_size;                     /**< 堆栈大小*/

    struct _cat_node_t  link_node;                      /**< 任务表中的链表节点*/
    uint32_t            task_delay;                     /**< soft delay timer(number of systemticks)*/
    struct _cat_node_t  delay_node;                     /**< */

    uint32_t            state;                          /**< */

    uint8_t             prio;                           /**< priority of task*/
    uint32_t            slice;                          /**< 时间片(剩余时间)*/
    uint32_t            suspend_cnt;                    /**< 被挂起的次数*/
    
    void                (*clean_cbk)(void *);        /**< 资源清理函数*/
    void                *clean_arg;                     /**< 资源清理函数的参数*/
    uint8_t             req_delete_flag;                /**< 请求删除该任务标志*/

    struct _cat_event_t *wait_event;                    /**< 等待的事件*/
    void                *event_msg;                     /**< 消息(等待的事件传回的消息)*/
    uint32_t            wait_event_result;              /**< 消息等待的结果*/

    uint32_t            wait_flags_type;                /**< 等待标志的类型*/
    uint32_t            wait_event_flags;               /**< 等待的标志*/

    uint8_t             *task_name;                     /**< 任务名称*/
    uint32_t            sched_times;                    /**< 调度次数*/

    uint32_t            sched_strategy;                 /**< 调度策略*/

 #ifdef USE_EDF_SCHED
    /** EDF 相关 */
    void                (*user_func)(void *);           /**< 用户函数*/
    uint32_t            period;                         /**< 周期*/
    uint32_t            next_arrive;                    /**< 距离下一次到达剩余的tick数*/
    uint32_t            deadline;                       /**< 距离本次截止时间的tick数*/

    uint32_t            exec_time;                      /**< 任务每周期执行需要的tick数*/
    uint32_t            left_exec_time;                 /**< 距离本周期执行结束剩余的tick数*/
#endif
    
    //uint8_t              task_name[CATOS_MAX_TASK_NAME_LEN];  /**< name of stack*/
};

struct _task_info_t
{
    uint8_t             *task_name;
    uint32_t            sched_times;
    uint32_t            task_delay;
    uint32_t            prio;
    uint32_t            state;
    uint32_t            slice;
    uint32_t            suspend_cnt;

    cat_stack_type_t    *stack_top;                     /**< 栈顶(堆栈指针)*/
    uint32_t            *stack_start_addr;              /**< 堆栈起始地址*/
    uint32_t            stack_size;
    uint32_t            free_stack_size;
};

//事件的结果
enum _cat_event_err_t
{
    cat_event_err_noerror = 0,              /**< 没有错误*/
    cat_event_err_timeout,                  /**< 超时*/
    cat_event_err_resource_unabaliable,     /**< 资源不可用*/
    cat_event_err_del,                      /**< 删除(销毁)*/
    cat_event_err_resource_full,            /**< 资源已满*/
    cat_event_err_owner,                    /**< 不是拥有者*/

};

// extern var
extern struct _cat_TCB_t *cat_current_task;
extern struct _cat_TCB_t *cat_ready_task;

/**
 * @brief 
 * @param task                  任务控制块指针
 * @param entry                 入口函数指针
 * @param arg                   入口函数的参数
 * @param prio                  任务优先级
 * @param stack_start_addr      堆栈起始地址
 * @param stack_size            堆栈大小
 */
void task_init(
    const uint8_t *task_name,
    struct _cat_TCB_t *task, 
    void (*entry)(void *), 
    void *arg, 
    uint8_t prio, 
    cat_stack_type_t *stack_start_addr,
    uint32_t stack_size,
    uint32_t sched_strategy
);


//得到最高优先级任务
struct _cat_TCB_t *cat_task_highest_ready(void);

//初始化调度相关
void cat_task_sched_init(void);

//禁止调度
void cat_task_sched_disable(void);

//允许调度
void cat_task_sched_enable(void);

void cat_task_sched(void);

//将任务放进就绪表
void cat_task_sched_rdy(struct _cat_TCB_t *task);

//将任务从就绪表取出
void cat_task_sched_unrdy(struct _cat_TCB_t *task);

//挂起
void cat_task_suspend(struct _cat_TCB_t *task);

//唤醒
void cat_task_wakeup(struct _cat_TCB_t *task);


//删除任务(目前和取出就绪表的函数实现一样)
void cat_task_sched_remove(struct _cat_TCB_t *task);

//为任务设置清理函数
void cat_task_set_clean_cbk(struct _cat_TCB_t *task, void (*clean_cbk)(void *clean_arg), void *clean_arg);

//强制删除任务
void cat_task_force_delete(struct _cat_TCB_t *task);

//请求删除任务
void cat_task_request_delete(struct _cat_TCB_t *task);

//检查是否被请求删除
uint8_t cat_task_is_requested_delete(void);

//删除自己
void cat_task_delete_self(void);



//获取任务信息
void cat_task_get_info(struct _cat_TCB_t *task, struct _task_info_t *info);

#endif

// /****************** 调度相关 **************************/
// //创建任务
// void task_init(struct _cat_TCB_t *task, void (*entry)(void *), void *arg, uint8_t prio, cat_stack_type_t *stack);

// //初始化中断
// void cat_set_systick_period(uint32_t ms);

// void cat_task_systemtick_handler(void);

// void cat_task_sched_init(void);//初始化调度锁

// void cat_task_sched_disable(void);//禁止调度

// void cat_task_sched_enable(void);

// void cat_task_sched_rdy(struct _cat_TCB_t *task);//挂就绪表

// void cat_task_sched_unrdy(struct _cat_TCB_t *task);//从就绪表取出

// //初始化延时列表
// void cat_task_delayed_init(void);

// //插入延时列表
// void cat_task_time_wait(struct _cat_TCB_t *task, uint32_t ticks);

// //从延时列表取下
// void cat_task_time_wakeup(struct _cat_TCB_t *task);

