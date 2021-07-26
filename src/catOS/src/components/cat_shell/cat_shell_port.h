/**
 * @file cat_shell_port.h
 * @brief 
 * @author amoigus (648137125@qq.com)
 * @version 0.1
 * @date 2021-06-10
 * 
 * @copyright Copyright (c) 2021
 * 
 * @par 修改日志：
 * Date              Version Author      Description
 * 2021-06-10 1.0    amoigus             内容
 */

#ifndef CAT_SHELL_PORT_H
#define CAT_SHELL_PORT_H

#include "catos_config.h"
#include "catos_defs.h"

void cat_shell_task_init(void);

#if (CATOS_USE_SW_RESET == 1)
void cat_reset_chip(void);
#endif



#endif
