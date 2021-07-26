/**
 * @file cat_lib.h
 * @brief 
 * @author amoigus (648137125@qq.com)
 * @version 0.1
 * @date 2021-03-23
 * 
 * @copyright Copyright (c) 2021
 * 
 * @par 修改日志：
 * Date              Version Author      Description
 * 2021-03-23 1.0    amoigus             位图
 */

#ifndef CAT_LIB_H
#define CAT_LIB_H

#include "catos_defs.h"
#include "catos_config.h"

struct _cat_bitmap
{
    uint32_t bitmap;
};
typedef struct _cat_bitmap cat_bitmap;

/**
 * @brief 获取位图位数
 */
uint32_t cat_bitmap_get_wide(void);

void cat_bitmap_init(cat_bitmap *bitmap);
void cat_bitmap_set(cat_bitmap *bitmap, uint32_t pos);
void cat_bitmap_clr(cat_bitmap *bitmap, uint32_t pos);
uint32_t cat_bitmap_get_first_set(cat_bitmap *bitmap);

#endif
