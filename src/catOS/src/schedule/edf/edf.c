/**
 * @file edf.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "edf.h"

//edf_task_t *edf_pre_task = NULL, *edf_cur_task = NULL;

edf_task_t edf_rdy_head = {0};
edf_task_t edf_wait_head = {0};

/* pri funcs start ***/
void        edf_task_insert_tail(edf_task_t *head, edf_task_t *task);
edf_task_t *edf_task_get_first(edf_task_t *head);
void        edf_task_remove_from_list(edf_task_t *task);
edf_task_t *edf_get_highest(void);
void        edf_wait_next_arrive(edf_task_t *task);
void        edf_deal_wait_list(void);

void        edf_infinite_call(void);
/* pri funcs end ***/

void edf_init(void)
{
    edf_rdy_head.link_node.pre_node    = &(edf_rdy_head.link_node);
    edf_rdy_head.link_node.next_node   = &(edf_rdy_head.link_node);

    edf_wait_head.link_node.pre_node   = &(edf_wait_head.link_node);
    edf_wait_head.link_node.next_node  = &(edf_wait_head.link_node);
}

void edf_create_task(
    const uint8_t *task_name,
    edf_task_t *tcb, 
    edf_user_func func, 
    void *arg, 
    cat_stack_type_t *stack_start_addr,
    uint32_t stack_size,
    int period
)
{
    tcb->period         = period;
    tcb->next_arrive    = period;
    tcb->deadline       = period;

    tcb->exec_time      = 0;        //执行时间在第一次执行时获取
    tcb->left_exec_time = 0;        //剩余时间未知

    tcb->user_func      = func;

    task_init(
        task_name,
        tcb,
        edf_infinite_call,
        NULL,
        5,
        stack_start_addr,
        stack_size,
        SCHED_STRATEGY_EDF
    );

    edf_task_insert_tail(&edf_rdy_head, tcb);
}



uint8_t edf_sched(void)
{
    uint8_t ret = -1;
    edf_task_t *temp_task;
    uint32_t status = cat_task_enter_critical();

    edf_deal_wait_list();
    temp_task = edf_get_highest();
    if(NULL != temp_task)
    {

        ret = 0;
        
        if(temp_task != cat_current_task)
        {
            cat_ready_task = temp_task;
            cat_ready_task->sched_times++; /* 增加调度次数信息 */
        }
        
        cat_task_switch();
    }

    cat_task_exit_critical(status);

    return ret;
}

/* pri funcs start ***/
void edf_task_insert_tail(edf_task_t *head, edf_task_t *task)
{
    task->link_node.pre_node  = head->link_node.pre_node;
    task->link_node.next_node = &(head->link_node);

    head->link_node.pre_node->next_node = &(task->link_node);
    head->link_node.pre_node = &(task->link_node);
}

edf_task_t *edf_task_get_first(edf_task_t *head)
{
    edf_task_t *ret = NULL;
    struct _cat_node_t *tmp_node = NULL;

    if(head->link_node.next_node != &(head->link_node))
    {
        //如果链表不为空
        tmp_node = head->link_node.next_node;
        head->link_node.next_node = tmp_node->next_node;
        tmp_node->next_node->pre_node = &(head->link_node);
    }

    ret = NODE_PARENT(tmp_node, edf_task_t, link_node);
    return ret;
}

void edf_task_remove_from_list(edf_task_t *task)
{
    task->link_node.pre_node->next_node = task->link_node.next_node;
    task->link_node.next_node->pre_node = task->link_node.pre_node;
}

/**
 * @brief 得到最高优先级(最早截止时间)的任务
 * 
 * @return edf_task_t* 
 */
edf_task_t *edf_get_highest(void)
{
    edf_task_t *ret        = NULL;
    struct _cat_node_t *tmp_node   = edf_rdy_head.link_node.next_node;
    struct _cat_node_t *next_rdy   = NULL;//下一个就绪的任务

    

    //找到截止时间最早的任务
    while(tmp_node != &(edf_rdy_head.link_node))
    {
        //截止时间和下次到达时间均去掉一个tick
        NODE_PARENT(tmp_node, edf_task_t, link_node)->deadline--;
        NODE_PARENT(tmp_node, edf_task_t, link_node)->next_arrive--;
        
        //如果剩余执行时间为零
        if(0 == NODE_PARENT(tmp_node, edf_task_t, link_node)->left_exec_time)
        {
            next_rdy = NODE_PARENT(tmp_node, edf_task_t, link_node)->link_node.next_node;

            NODE_PARENT(tmp_node, edf_task_t, link_node)->deadline = NODE_PARENT(tmp_node, edf_task_t, link_node)->period;
            edf_wait_next_arrive(NODE_PARENT(tmp_node, edf_task_t, link_node));

            tmp_node = next_rdy;//将tmp_task移动到下一个就绪的任务上
        }
        else
        {
            if(
                (NULL == ret) ||
                (NODE_PARENT(tmp_node, edf_task_t, link_node)->deadline < ret->deadline)
                )
            {
                ret = NODE_PARENT(tmp_node, edf_task_t, link_node);
            }

            tmp_node = tmp_node->next_node;
        }
    }

    return ret;
}

/**
 * @brief 等待下一次到达
 * 
 * @param task 
 */
void edf_wait_next_arrive(edf_task_t *task)
{
    edf_task_remove_from_list(task);

    //todo: 计算任务距离下次到达还有多少时间，赋值给next_arrive
    //因为在每次调度都处理了下次到达时间，这里就不用处理了
    edf_task_insert_tail(&edf_wait_head, task);
}

/**
 * @brief 处理正在等待到达的链表
 * 
 */
void edf_deal_wait_list(void)
{
    edf_task_t *task = NULL;
    struct _cat_node_t *tmp_node = edf_wait_head.link_node.next_node;//用于遍历链表
    struct _cat_node_t *next_wait = NULL;             //用于存储即将到达的等待任务的下一个任务

    while(tmp_node != &(edf_wait_head.link_node))
    {
        task = NODE_PARENT(tmp_node, edf_task_t, link_node);

        //如果任务到了到达时间，就从等待链表取出，放入就绪链表
        if(0 == task->next_arrive)
        {
            next_wait = tmp_node->next_node;//如果这里不用额外指针rdy_task, tmp_task指针会跑到就绪链表中去
            edf_task_remove_from_list(task);

            task->deadline    = task->period;
            task->next_arrive = task->period;

            task->left_exec_time = task->exec_time;

            edf_task_insert_tail(&edf_rdy_head, task);

            tmp_node = next_wait;
        }
        else
        {
            task->next_arrive--;
            //printf("**%s next_arrive:%d\n", tmp_task->name, tmp_task->next_arrive);
            tmp_node = tmp_node->next_node;
        }
    }

}

/**
 * @brief 无限循环，每个edf任务执行时实际的入口
 * 
 */
void edf_infinite_call(void)
{
    cat_current_task->user_func(NULL);      //调用用户函数
    edf_wait_next_arrive(cat_current_task); //等待下一次到达
}

/* pri funcs end ***/