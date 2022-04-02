/**
 * @file edf.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef EDF_H
#define EDF_H

#include "catos_defs.h"
#include "catos_config.h"

#include "cat_task.h"
#include "strategy.h"

typedef struct _cat_TCB_t edf_task_t;
typedef void (*edf_user_func)(void *);

// struct _edf_task_t
// {
//     char *name;
//     int period;
//     int next_arrive;
//     int deadline;
//     int sched_times;

//     int exec_time;
//     int left_exec_time;

//     struct _edf_task_t *pre;
//     struct _edf_task_t *next;
// };

void edf_init(void);

void edf_create_task(
    const uint8_t *task_name,
    edf_task_t *task, 
    edf_user_func func, 
    void *arg, 
//    uint8_t prio, 
    cat_stack_type_t *stack_start_addr,
    uint32_t stack_size,
//    uint32_t sched_strategy,
//    int exec_time,
    int period
);

uint8_t edf_sched(void);



#endif


