#include <stdio.h>
#include <string.h>

#include "usart_fifo.h"


/*******************************************************************************
  * @说明：测试队列是否为空
  * @参数：Q: 被测试队列指针
  * @返回：队列状态
*******************************************************************************/
bool_t inn_queue_is_empty(queue_t *Q)
{
        if(Q->front == Q->rear)
                return True;

        return False;
}

/*******************************************************************************
  * @说明：测试队列是否满
  * @参数：Q: 被测试队列指针
  * @返回：队列状态
*******************************************************************************/
bool_t inn_queue_is_full(queue_t *Q)
{
        if((Q->rear + 1) % Q->maxLen == Q->front)
                return True;

        return False;
}
/*******************************************************************************
  * @说明：向队列插入新元素,数据的插入，也即数据的拷贝过程，按照1字节逐个拷贝
  * @参数：Q: 被操作的队列指针
  *		   e: 新元素
  * @返回：队列状态
*******************************************************************************/
bool_t inn_queue_push(queue_t *Q, void *e)
{
        uint32_t size = Q->typeSize;
        char *s  = NULL;
        char *d  = NULL;
        uint32_t front;
        uint32_t rear = Q->rear;

        if(inn_queue_is_full(Q) == True)
                return False;

        s  = (char *)e;
        d  = (char *)((uint32_t)Q->base + rear * size);

        memcpy(d, s, size);

        s = d = NULL;
        Q->rear = (Q->rear+1) % Q->maxLen;
        //Q->used++;
#if 1
        rear = Q->rear;
        front = Q->front;
        if(rear >= front)
                Q->used = rear - front;
        else
                Q->used = rear + Q->maxLen - front;
#endif
        return True;
}
/*******************************************************************************
  * @说明：删除队列头元素,数据的出列，也即数据的拷走过程，按1字节逐个拷走
  * @参数：Q: 被操作的队列指针 loc 从哪个位置读
  *		   e: 带回头元素
  * @返回：队列状态
*******************************************************************************/
bool_t inn_queue_pop(queue_t *Q, void *e ,uint32_t loc)
{
        uint32_t size = Q->typeSize;
        char *d  = NULL;
        uint32_t pos = loc;
        uint32_t front = (Q->front+pos)%Q->maxLen;
        uint32_t rear;
        char *s  = NULL;
        char *s1 = NULL;

        if(inn_queue_is_empty(Q) == True)
                return False;

        if(pos > Q->used)
                return False;

        d = (char *)e;
        s = (char *)Q->base + front * size;
    memcpy(d, s, size);

        while(pos--)
        {
                s = (char *)Q->base + front * size;
                if(front>0)
                {
                        front--;
                }
                else
                {
                        front = Q->maxLen-1;
                }
                s1 = (char *)Q->base + front * size;


            memcpy(s, s1, size);
        }
        d = s = NULL;
        Q->front = (Q->front + 1) % Q->maxLen;
        //Q->used--;
        #if 1
        rear = Q->rear;
        front = Q->front;
        if(rear >= front)
                Q->used = rear - front;
        else
                Q->used = rear + Q->maxLen - front;
        #endif
        return True;
}

/*******************************************************************************
  * @说明：删除队列
  * @参数：Q: 队列
  * @返回：操作状态
*********************************************************************************/
bool_t inn_queue_destroy(queue_t *Q)
{
//	free(Q->base);
        Q->base = NULL;
        Q->front = Q->rear = 0;
        Q->maxLen = Q->used = Q->typeSize = 0;
        return True;
}
/*******************************************************************************
  * @说明：清除队列
  * @参数：Q: 队列
  * @返回：操作状态
*********************************************************************************/
bool_t inn_queue_clear(queue_t *Q)
{
        memset(Q->base, 0, Q->maxLen * Q->typeSize);
        Q->front = Q->rear = 0;
        Q->used = 0;
        return True;
}
/*******************************************************************************
  * @说明：遍历队列
  * @参数：Q: 队列
  * @返回：操作状态
*********************************************************************************/
uint32_t inn_queue_traverse(queue_t *Q, void *buf ,uint32_t length)
{
        uint32_t used = Q->used;
        uint32_t size = Q->typeSize;
        char *d  = (char *)buf;
        uint32_t front = Q->front;
        char *s  = NULL;
        uint32_t len = length;

        if(used>len)
                used = len;
        len = used;
        while(used--)
        {
                s  = (char *)Q->base + front * size;
            memcpy(d, s, size);

                d += size;
                front = (front + 1) % Q->maxLen;
        }
        d = s = NULL;
        return len;
}

/*******************************************************************************
  * @说明：取队列中任意一个数据
  * @参数：Q: 队列
  * @返回：操作状态
*******************************************************************************/
bool_t inn_queue_get_index(queue_t *Q, void *e ,uint32_t loc)
{
        uint32_t size = Q->typeSize;
        char *d  = NULL;
        uint32_t pos = loc;
        uint32_t front = (Q->front+pos)%Q->maxLen;
        char *s  = NULL;

        if(inn_queue_is_empty(Q) == True)
                return False;

        if(pos > Q->used)
                return False;

        d = (char *)e;
        s = (char *)Q->base + front * size;
        memcpy(d, s, size);
        d = s = NULL;
        return True;
}



void usart_queue_init(queue_t *q,uint32_t typesize,void *buf,uint32_t buf_len)
{
    if(q)
    {
        q->front  = q->rear = q->used	= 0;

        q->typeSize = typesize;//sizeof(type);

        q->base = buf;

        memset(q->base, 0, buf_len*q->typeSize);

        q->maxLen = buf_len;

        q->used	= 0;

        q->queue_push = inn_queue_push;

        q->queue_pop = inn_queue_pop;

        q->queue_is_empty = inn_queue_is_empty;

        q->queue_is_full  = inn_queue_is_full;

        q->queue_clear   = inn_queue_clear;

        q->queue_traverse = inn_queue_traverse;

        q->queue_get_index = inn_queue_get_index;

        q->queue_destroy = inn_queue_destroy;
    }
}
