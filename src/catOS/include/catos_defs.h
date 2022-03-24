/**
 * @file catos_defs.h
 * @brief 
 * @author amoigus (648137125@qq.com)
 * @version 0.1
 * @date 2021-03-17
 * 
 * @copyright Copyright (c) 2021
 * 
 * @par 修改日志：
 * Date              Version Author      Description
 * 2021-03-17 1.0    amoigus             基本类型定义
 */

#ifndef CATOS_DEF_H
#define CATOS_DEF_H

#include <stdio.h>
#include "catos_declares.h"

#define U32_MAX_VAL 0xffffffff

typedef signed char     int8_t;
typedef short int       int16_t;

#if defined(__CC_ARM)
    typedef int             int32_t;
#endif

typedef unsigned char           uint8_t;
typedef unsigned short int      uint16_t;
typedef unsigned int            uint32_t;

typedef uint32_t cat_stack_type_t;       //stack type(the default stack wide of stm32 is 32)
//typedef void (*cat_task_entry_t)(void *);   //pointer type of user defined task entry function

/************** struct type*********************/



#endif


