/**
 * @file cat_mem.c
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

#include "cat_mem.h"

#if (CATOS_ENABLE_MEMBLOCK == 1)

/**
 * block:
 * |<-block start addr                   |<-block end addr
 * | node_size |  block_size - node_size |
 * 
 */

//初始化内存块组
void cat_mem_blocks_init(struct _cat_mem_blocks_t *blocks, uint8_t *mem_start, uint32_t block_size, uint32_t max_blk_cnt)
{
    uint8_t *start_index = (uint8_t *)mem_start;//内存组内存开始地址
    uint8_t *blocks_end   = mem_start + (block_size * max_blk_cnt);//内存组内存结束地址

    if(block_size < sizeof(struct _cat_node_t))
    {
        //可以返回初始化失败,但是目前用不着
        return;
    }

    cat_event_init(&(blocks->event), cat_event_type_memblock);
    blocks->mem_start_addr = start_index;
    blocks->block_size = block_size;
    blocks->max_blk_cnt = max_blk_cnt;

    cat_list_init(&(blocks->block_list));
    while(start_index < blocks_end)
    {
        cat_node_init((struct _cat_node_t *)start_index);
        cat_list_add_last(&(blocks->block_list), (struct _cat_node_t *)start_index);

        start_index += block_size;
    }

}

uint32_t cat_mem_block_wait(struct _cat_mem_blocks_t *blocks, uint8_t **mem, uint32_t wait_ticks)
{
    uint32_t ret = 0;
    uint32_t status = cat_task_enter_critical();

    if(cat_list_count(&(blocks->block_list)) > 0)
    {
        *mem = (uint8_t *)cat_list_remove_first(&(blocks->block_list));
        cat_task_exit_critical(status);
        ret = cat_event_err_noerror;
    }
    else
    {
        cat_event_wait(&(blocks->event), cat_current_task, NULL, cat_event_type_memblock, wait_ticks);
        cat_task_exit_critical(status);

        cat_task_sched();

        *mem = cat_current_task->event_msg;
        ret = cat_current_task->wait_event_result;
    }

    //cat_task_exit_critical(status);
    return ret;
}

uint32_t cat_mem_block_no_wait_get(struct _cat_mem_blocks_t *blocks, uint8_t **mem, uint32_t wait_ticks)
{
    uint32_t ret = 0;
    uint32_t status = cat_task_enter_critical();

    if(cat_list_count(&(blocks->block_list)) > 0)
    {
        *mem = (uint8_t *)cat_list_remove_first(&(blocks->block_list));
        cat_task_exit_critical(status);
        ret = cat_event_err_noerror;
    }
    else
    {
        cat_task_exit_critical(status);
        ret = cat_event_err_resource_unabaliable;
    }

    //cat_task_exit_critical(status);
    return ret;
}

void cat_mem_block_notify(struct _cat_mem_blocks_t *blocks, uint8_t *mem)
{
    uint32_t status = cat_task_enter_critical();

    //如果有任务正在等待内存块
    if(cat_event_wait_count(&(blocks->event)) > 0)
    {
        //把内存块直接给等待队列的第一个任务
        struct _cat_TCB_t *task = cat_event_wakeup_first(&(blocks->event), (void *)mem, cat_event_err_noerror);
        if(task->prio < cat_current_task->prio)
        {
            cat_task_sched();
        }
    }
    else
    {
        //否则就把内存块挂到内存组的链表上
        cat_list_add_last(&(blocks->block_list), (struct _cat_node_t *)mem);
    }

    cat_task_exit_critical(status);
}

//获取内存块组信息
void cat_mem_blocks_get_info(struct _cat_mem_blocks_t *blocks, struct _cat_mem_blocks_info_t *info)
{
    uint32_t status = cat_task_enter_critical();

    info->block_cnt = cat_list_count(&(blocks->block_list));
    info->max_blk_cnt = blocks->max_blk_cnt;
    info->block_size = blocks->block_size;
    info->task_cnt = cat_event_wait_count(&(blocks->event));

    cat_task_exit_critical(status);
}

//删除内存块组
uint32_t cat_mem_blocks_destroy(struct _cat_mem_blocks_t *blocks)
{
    uint32_t status = cat_task_enter_critical();
    uint32_t count = cat_event_remove_all(&(blocks->event), NULL, cat_event_err_del);
    cat_task_exit_critical(status);

    if(count > 0)
    {
        cat_task_sched();
    }

    return count;
}

#endif
