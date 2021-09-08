/**
 * @file cat_mem.h
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

#ifndef CAT_MEM_H
#define CAT_MEM_H

#include "cat_event.h"
#include "cat_list.h"

//内存块组
struct _cat_mem_blocks_t
{
    struct _cat_event_t     event;//事件
    void                    *mem_start_addr;//内存开始地址
    uint32_t                block_size;//每个内存块大小
    uint32_t                max_blk_cnt;//最大内存块个数
    struct _cat_list_t      block_list;//内存块链表
};

struct _cat_mem_blocks_info_t
{
    uint32_t                block_cnt;//当前内存块个数
    uint32_t                max_blk_cnt;//最大内存块个数
    uint32_t                block_size;//每个内存块大小
    uint32_t                task_cnt;//当前等待内存块的任务数量
};

//初始化内存块组
void cat_mem_blocks_init(struct _cat_mem_blocks_t *blocks, uint8_t *mem_start, uint32_t block_size, uint32_t block_cnt);

//获取内存块
uint32_t cat_mem_block_wait(struct _cat_mem_blocks_t *blocks, uint8_t **mem, uint32_t wait_ticks);

//非阻塞获取内存块
uint32_t cat_mem_block_no_wait_get(struct _cat_mem_blocks_t *blocks, uint8_t **mem, uint32_t wait_ticks);

//释放内存块
void cat_mem_block_notify(struct _cat_mem_blocks_t *blocks, uint8_t *mem);

//获取内存块组信息
void cat_mem_blocks_get_info(struct _cat_mem_blocks_t *blocks, struct _cat_mem_blocks_info_t *info);

//删除内存块组
uint32_t cat_mem_blocks_destroy(struct _cat_mem_blocks_t *blocks);


#endif
