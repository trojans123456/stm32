
#include "fifo.h"


static uint16_t fifo_next(fifo_t *fifo,uint16_t index)
{
    return (index + 1) % fifo->Size;
}


void fifo_init(fifo_t *fifo, uint8_t *buffer, uint16_t size)
{
    fifo->Begin = 0;
    fifo->End = 0;
    fifo->Data = buffer;
    fifo->Size = size;
}

void fifo_push(fifo_t *fifo, uint8_t data)
{
    fifo->End = fifo_next(fifo,fifo->End);
    fifo->Data[fifo->End] = data;
}

void fifo_pop(fifo_t *fifo,uint8_t *data)
{
    *data= fifo->Data[fifo_next(fifo,fifo->Begin)];
    fifo->Begin = fifo_next(fifo,fifo->Begin);
}

void fifo_flush(fifo_t *fifo)
{
    fifo->Begin = 0;
    fifo->End = 0;
}

uint8_t fifo_isempty(fifo_t *fifo)
{
    return (fifo->Begin == fifo->End) ? 1 : 0;
}

uint8_t fifo_isfull(fifo_t *fifo)
{
    return (fifo_next(fifo,fifo->End) == fifo->Begin) ? 1 : 0;
}
