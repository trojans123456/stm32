#ifndef __FIFO_H
#define __FIFO_H

#include <stdint.h>


typedef struct fifo__
{
    uint16_t Begin;
    uint16_t End;
    uint8_t *Data;
    uint16_t Size;
}fifo_t;

void fifo_init(fifo_t *fifo,uint8_t *buffer,uint16_t size);

void fifo_push(fifo_t *fifo,uint8_t data);

void fifo_pop(fifo_t *fifo, uint8_t *data);

void fifo_flush(fifo_t *fifo);

uint8_t fifo_isempty(fifo_t *fifo);

uint8_t fifo_isfull(fifo_t *fifo);

#endif
