/**
 * @file cat_idle.c
 * @brief 
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

#include "cat_idle.h"
#include "cat_task.h"

#if (CATOS_ENABLE_TIMER == 1)
#include "cat_timer.h"
#endif

#if (CATOS_ENABLE_HOOKS == 1)
#include "cat_hooks.h"
#endif

#if (CATOS_ENABLE_SHELL == 1)
#include "cat_shell.h"
#endif

#include "app.h"

#include "catos_config.h"
#include "catos_defs.h"

uint32_t tick_count;    /**< 时钟节拍次数*/
uint32_t catos_ticks;   /**< 系统总节拍数（*********以后代替tick_count*/

struct _cat_TCB_t idle_task;
cat_stack_type_t idle_task_env[CATOS_IDLE_STACK_SIZE];
uint32_t idle_cnt;      /**< 空闲任务时钟节拍计数*/
uint32_t idle_max_cnt;  /**< 最大节拍输(现在是一秒内的)*/

extern struct _cat_TCB_t *cat_idle_task;

void cat_idle_task_init(void)
{
    task_init("idle_task", &idle_task, cat_idle_entry, NULL, CATOS_MAX_TASK_PRIO - 1, idle_task_env, CATOS_IDLE_STACK_SIZE);
    cat_idle_task = &idle_task;
}

void time_tick_init(void)
{
    tick_count = 0;
    catos_ticks = 0;
}

void cat_idle_entry(void *arg)
{
    //禁止调度
    cat_task_sched_disable();

#if (CATOS_ENABLE_TIMER == 1)
    //初始化定时器任务
    cat_timer_task_init();
#endif

    //设置系统tick频率
    cat_set_systick_period(CATOS_SYSTICK_MS);

#if (CATOS_ENABLE_CPUUSAGE_STAT == 1)
    //测量最大空闲计数值
    cpu_usage_sync_with_systick();
#endif

#if 1
//串口和backtrace初始化暂时放在这
    cat_uart_init();
#ifdef USER_USE_CM_BACKTRACE
    cm_backtrace_init("CmBacktrace", "0.0.0", "0.0.0");
#endif //#ifdef USER_USE_CM_BACKTRACE
#endif

#if (CATOS_ENABLE_SHELL == 1)
    cat_shell_task_init();
#endif

    //初始化用户定义的任务
    cat_user_tasks_init();

#if (CATOS_ENABLE_CPUUSAGE_STAT != 1)
    //如果没打开cpu_usage，则手动允许调度
    cat_task_sched_enable();
#endif
    for(;;)
    {
#if (CATOS_ENABLE_CPUUSAGE_STAT == 1)
        uint32_t status = cat_task_enter_critical();
        idle_cnt++;
        cat_task_exit_critical(status);
#endif

#if (CATOS_ENABLE_HOOKS == 1)
        cat_hooks_idle();
#endif
    }
}


//cpu统计相关

#if (CATOS_ENABLE_CPUUSAGE_STAT == 1)
uint32_t enable_cpu_usage_stat;

#if 0
    float    cpu_usage;
#else
    uint32_t cpu_usage_integer; /**< 整数部分*/
    uint32_t cpu_usage_decimal; /**< 小数部分*/
#endif


/**
 * @brief 初始化cpu统计的变量
 */
void init_cpu_usage_stat(void)
{
    idle_cnt = 0;
    idle_max_cnt = 0;

#if 0
    cpu_usage = 0.0f;
#else
    cpu_usage_integer = 0;
    cpu_usage_decimal = 0;
#endif

    enable_cpu_usage_stat = 0;
}

//之后改写成初始化和统计两部分
void cpu_usage_stat(void)
{
    if(enable_cpu_usage_stat == 0)
    {
        //第一次被调用时，将enable_cpu_usage_stat置为1，让cat_idle_entry中
        //的同步函数返回，开始空闲计数
        enable_cpu_usage_stat = 1;
        tick_count = 0;
    }
    else if(tick_count == CATOS_SYSTICK_FRQ)
    {
        //如果从启动开始到了一秒，此时的空闲计数即为最大空闲计数
        idle_max_cnt = idle_cnt;
        idle_cnt = 0;

        //从这里开始就初始化完成，允许其他任务运行
        cat_task_sched_enable();
    }
    else if(tick_count % CATOS_SYSTICK_FRQ == 0)
    {
#if 0
        cpu_usage = 100 - (idle_cnt * 100.0 / idle_max_cnt);
#else
        cpu_usage_integer = 10000 - (idle_cnt * 10000 / idle_max_cnt);
        cpu_usage_decimal = cpu_usage_integer % 100;
        cpu_usage_integer = cpu_usage_integer / 100;
#endif

        idle_cnt = 0;
    }
}

/**
 * @brief 将空闲任务时钟与cpu同步，即等待调用这个函数后第一个时钟节拍发生
 */
void cpu_usage_sync_with_systick(void)
{
    while(enable_cpu_usage_stat == 0){
        ;;
    }
}

#if 0
float cat_cpu_usage_get(void)
{
    float usage = 0;

    uint32_t status = cat_task_enter_critical();
    usage = cpu_usage;
    cat_task_exit_critical(status);

    return usage;
}
#else
int32_t cat_cpu_usage_get(uint32_t *integer, uint32_t *decimal)
{
    float usage = 0;
    int32_t len = 0;

    uint32_t status = cat_task_enter_critical();
    *integer = cpu_usage_integer;
    *decimal = cpu_usage_decimal;
    cat_task_exit_critical(status);

    return 0;
}
#endif


#endif
