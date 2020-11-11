#ifndef __FIFO_H
#define __FIFO_H

#include <stdint.h>


typedef struct fifo__ fifo_t;

void fifo_init(fifo_t *fifo,uint8_t *buffer,uint16_t size);

void fifo_push(fifo_t *fifo,uint8_t data);

uint8_t fifo_pop(fifo_t *fifo);

void fifo_flush(fifo_t *fifo);

uint8_t fifo_isempty(fifo_t *fifo);

uint8_t fifo_isfull(fifo_t *fifo);

#endif
