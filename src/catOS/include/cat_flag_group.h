/**
 * @file cat_flag_group.h
 * @brief 
 * @author amoigus (648137125@qq.com)
 * @version 0.1
 * @date 2021-03-26
 * 
 * @copyright Copyright (c) 2021
 * 
 * @par 修改日志：
 * Date              Version Author      Description
 * 2021-03-26 1.0    amoigus             内容
 */

#ifndef CAT_FLAG_GROUP_H
#define CAT_FLAG_GROUP_H

#include "cat_event.h"

struct _cat_flag_group_t
{
    struct _cat_event_t event;
    uint32_t            flag;
};

struct _cat_flag_group_info_t
{
    uint32_t            flag;
    uint32_t            task_cnt;
};

/**
 * 事件等待的标志事件
 * 一共2 * 2 = 4种组合情况
 * 
 * opt[0]: 0：标志位被置零      1：标志位被置位
 * opt[1]：0：任意标志位被置位  2：所有标志位被置位
 * 
 */

//基础事件只在flag_group模块内部使用，只允许出现四种组合事件
#define _BASE_CAT_FLAG_GRP_CLR           (0x0 << 0) /**< 标志位被置零*/
#define _BASE_CAT_FLAG_GRP_SET           (0x1 << 0) /**< 标志位被置位*/
#define _BASE_CAT_FLAG_GRP_ANY           (0x0 << 1) /**< 任意标志位*/
#define _BASE_CAT_FLAG_GRP_ALL           (0x1 << 1) /**< 所有标志位*/

#define CAT_FLAG_GRP_SET_ALL       (_BASE_CAT_FLAG_GRP_SET | _BASE_CAT_FLAG_GRP_ALL)    /**< */
#define CAT_FLAG_GRP_CLR_ALL       (_BASE_CAT_FLAG_GRP_CLR | _BASE_CAT_FLAG_GRP_ALL)    /**< */
#define CAT_FLAG_GRP_SET_ANY       (_BASE_CAT_FLAG_GRP_SET | _BASE_CAT_FLAG_GRP_ANY)    /**< */
#define CAT_FLAG_GRP_CLR_ANY       (_BASE_CAT_FLAG_GRP_CLR | _BASE_CAT_FLAG_GRP_ANY)    /**< */


#define CAT_FLAG_GRP_CONSUME       (1 << 7) /**< 置零标志位*/

//初始化事件标志组
void cat_flag_group_init(struct _cat_flag_group_t *fg, uint32_t flags);

/**
 * @brief 等待标志
 * @param fg            事件标志组对象指针
 * @param wait_type     所期待的标志的条件(0,1，全部，至少一个)
 * @param request_flag  等待的标志位
 * @param result_flag   进行操作后的标志
 * @param wait_ticks    超时时间
 * @return uint32_t 
 */
uint32_t cat_flag_group_wait(struct _cat_flag_group_t *fg, uint32_t wait_type, uint32_t request_flag, uint32_t *result_flag, uint32_t wait_ticks);

/**
 * @brief 非阻塞查询标志组
 * @param fg 
 * @param wait_type 
 * @param request_flag 
 * @param result_flag 
 * @return uint32_t cat_event_err_noerror:成功
 */
uint32_t cat_flag_group_no_wait_get(struct _cat_flag_group_t *fg, uint32_t wait_type, uint32_t request_flag, uint32_t *result_flag);

/**
 * @brief 设置标志组
 * @param fg 
 * @param is_set 
 * @param flag 
 */
void cat_flag_group_notify(struct _cat_flag_group_t *fg, uint32_t is_set, uint32_t flag);

//获取当前事件标志组状态信息
void cat_flag_group_get_info(struct _cat_flag_group_t *fg, struct _cat_flag_group_info_t *info);

//删除事件标志组
uint32_t cat_flag_group_destroy(struct _cat_flag_group_t *fg);

#endif
