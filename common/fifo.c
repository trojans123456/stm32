
#include "fifo.h"

struct fifo__
{
    uint16_t Begin;
    uint16_t End;
    uint8_t *Data;
    uint16_t Size;
};

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

uint8_t fifo_pop(fifo_t *fifo)
{
    uint8_t data= fifo->Data[fifo_next(fifo,fifo->Begin)];
    fifo->Begin = fifo_next(fifo,fifo->Begin);
    return data;
}

void fifo_flush(fifo_t *fifo)
{
    fifo->Begin = 0;
    fifo->End = 0;
}

uint8_t fifo_isempty(fifo_t *fifo)
{
    return (fifo->Begin == fifo->End);
}

uint8_t fifo_isfull(fifo_t *fifo)
{
    return (fifo_next(fifo,fifo->End) == fifo->Begin);
}
