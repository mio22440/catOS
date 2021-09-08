/**
 * @file cat_hooks.c
 * @brief 
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

#include "cat_hooks.h"

#if (CATOS_ENABLE_HOOKS == 1)
void cat_hooks_idle(void)
{

}

void cat_hooks_systick(void){

}

void cat_hooks_task_before_switch(struct _cat_TCB_t *from, struct _cat_TCB_t *to)
{

}


void cat_hooks_task_init(struct _cat_TCB_t *task)
{

}

#endif
