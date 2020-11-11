#ifndef __USART_QUEUE_H
#define __USART_QUEUE_H

#include "define.h"

typedef struct queue
{
    /** 队列属性域 **/
    void            *base;		/** 初始化的动态分配空间 **/
    uint32_t		front;		/**头指针，若队列不空，则指向队列头元素 **/
    uint32_t		rear;		/**尾指针，若队列不空，则指向队列尾元素的下一个位置 **/
    uint32_t		maxLen;		/**队列长度，即单元个数 **/
    uint32_t		used;		/**队列中含有数据个数 **/
    uint32_t		typeSize;	/**队列所支持数据类型的大小 **/
    // 队列函数操作域
    bool_t (*queue_push)(struct queue *Q, void *e);	/**插入e为队列的新元素 **/
    bool_t (*queue_pop)(struct queue *Q, void *e, uint32_t loc);	/**删除队列头元素 **/
    bool_t (*queue_is_empty)(struct queue *Q);			/**测试队列是否为空 **/
    bool_t (*queue_is_full) (struct queue *Q);			/**测试队列是否为满 **/
    bool_t (*queue_destroy)(struct queue *Q);			/**销毁队列 **/
    bool_t (*queue_clear)(struct queue *Q);				/**清除队列 **/
    uint32_t (*queue_traverse)(struct queue *Q, void *buf ,uint32_t length);/**遍历队列 **/
    bool_t (*queue_get_index)(struct queue *Q, void *e, uint32_t loc);	/**获取队列中数据 **/
}queue_t;

void usart_queue_init(queue_t *q,uint32_t typesize,void *buf,uint32_t buf_len);

#endif
