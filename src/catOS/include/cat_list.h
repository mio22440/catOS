/**
 * @file cat_list.h
 * @brief 循环链表
 * @author amoigus (648137125@qq.com)
 * @version 0.1
 * @date 2021-03-17
 * 
 * @copyright Copyright (c) 2021
 * 
 * @par 修改日志：
 * Date              Version Author      Description
 * 2021-03-17 1.0    amoigus             类型和函数声明
 */

#ifndef LIST_H
#define LIST_H

#include "catos_defs.h"
#include "catos_config.h"


#define FIRST_NODE head_node.next_node
#define LAST_NODE  head_node.pre_node

#define NODE_PARENT(node, parent, name) (parent *)((uint32_t)node - (uint32_t)&((parent *)0)->name)


struct _cat_node_t
{
    struct _cat_node_t *pre_node;
    struct _cat_node_t *next_node;
};

struct _cat_list_t
{
    struct _cat_node_t head_node;
    uint32_t node_cnt;
};



void cat_node_init(struct _cat_node_t *node);

void cat_list_init(struct _cat_list_t *list);

uint32_t cat_list_count(struct _cat_list_t *list);

struct _cat_node_t *cat_list_first(struct _cat_list_t *list);

struct _cat_node_t *cat_list_last(struct _cat_list_t *list);

//指定节点的前一个节点
struct _cat_node_t *cat_list_pre(struct _cat_list_t *list, struct _cat_node_t *node);

struct _cat_node_t *cat_list_next(struct _cat_list_t *list, struct _cat_node_t *node);

void cat_list_remove_all(struct _cat_list_t *list);

//头插法
void cat_list_add_first(struct _cat_list_t *list, struct _cat_node_t *node);

void cat_list_add_last(struct _cat_list_t *list, struct _cat_node_t *node);

struct _cat_node_t *cat_list_remove_first(struct _cat_list_t *list);

void cat_list_insert_after(struct _cat_list_t *list, struct _cat_node_t *node_after, struct _cat_node_t *node_to_insert);

void cat_list_remove(struct _cat_list_t *list, struct _cat_node_t *node);

#endif

