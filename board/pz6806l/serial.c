#include "default.h"
#include "serial.h"

#define SERIAL_PORT_3_USED

/* stm32 uart */
struct stm32_uart
{
    USART_TypeDef *uart_device;
    IRQn_Type irq;
};


#if defined(SERIAL_PORT_3_USED)
struct stm32_uart uart3 =
{
    USART3,
    USART3_IRQn
};

struct serial_device serial3;

void USART3_IRQHandler(void)
{

}
#endif // defined

static void USART_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHZ;
#if defined(SERIAL_PORT_3_USED)
    /* action AFIO */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB1PeriphClcokCmd(RCC_APB1Periph_USART3,ENABLE);
    /* rx */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;

    GPIO_Init(GPIOB,&GPIO_InitStructure);

    /* tx */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;

    GPIO_Init(GPIOB,&GPIO_InitStructure);

    /* if rs485 */
#endif // defined
}

static void NVIC_Configuration(struct stm32_uart* uart)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the USART1 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = uart->irq;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

int stm32_configure(struct serial_device *dev,struct serial_config *cfg)
{
    struct stm32_uart *uart;
    USART_InitTypeDef USART_InitStructure;

    uart = (struct stm32_uart *)dev->priv_data;

    USART_InitStructure.USART_BaudRate = cfg->baud_rate;
    if (cfg->data_bits == DATA_BITS_8){
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    } else if (cfg->data_bits == DATA_BITS_9) {
        USART_InitStructure.USART_WordLength = USART_WordLength_9b;
    }

    if (cfg->stop_bits == STOP_BITS_1){
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
    } else if (cfg->stop_bits == STOP_BITS_2){
        USART_InitStructure.USART_StopBits = USART_StopBits_2;
    }

    if (cfg->parity == PARITY_NONE){
        USART_InitStructure.USART_Parity = USART_Parity_No;
    } else if (cfg->parity == PARITY_ODD) {
        USART_InitStructure.USART_Parity = USART_Parity_Odd;
    } else if (cfg->parity == PARITY_EVEN) {
        USART_InitStructure.USART_Parity = USART_Parity_Even;
    }

    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(uart->uart_device, &USART_InitStructure);

    /* Enable USART */
    USART_Cmd(uart->uart_device, ENABLE);

    return 0;
}
int stm32_ioctl(struct serial_device *dev,int cmd,void *args)
{
    struct stm32_uart* uart;
    uart = (struct stm32_uart *)dev->priv_data;

    switch(cmd)
    {
    case SERIAL_IOCTL_DISABLE_RX_IRQ:
        NVIC_DisableIRQ(uart->irq);
        USART_ITConfig(uart->uart_device,USART_IT_RXNE,DISABLE);
        break;
    case SERIAL_IOCTL_ENABLE_RX_IRQ:
        NVIC_EnableIRQ(uart->irq);
        USART_ITConfig(uart->uart_device,USART_IT_RXNE,ENABLE);
        break;
    case SERIAL_IOCTL_ENABLE_RX_DMA:
        break;
    }
    return 0;
}
int stm32_putc(struct serial_device *dev,int ch)
{
    struct stm32_uart *uart;
    uart = (struct stm32_uart *)dev->priv_data;

    while(USART_GetFlagStatus(uart->uart_device,USART_FLAG_TXE) == RESET) ;
    USART_SendData(uart->uart_device,ch);
    while(USART_GetFlagStatus(uart->uart_device,USART_FLAG_TC) == RESET) ;
    return 0;
}
int stm32_getc(struct serial_device *dev)
{
    struct stm32_uart *uart;
    uart = (struct stm32_uart *)dev->priv_data;
    int ch = -1;

    if(USART_GetFlagStatus(uart->uart_device,USART_FLAG_RXNE) == SET)
    {
        ch = USART_ReceiveData(uart->uart_device);
    }
    return ch;
}

int stm32_async_puts(struct serial_device *dev,const char *fmt,...)
{

}
int stm32_async_gets(struct serial_device *dev,char *data,int len)
{

}


int stm32_dma_transmit(struct serial_device *dev,uint8_t *buf,uint32_t size,int direction)
{

}


void hardware_usart_init(void)
{
    struct serial_config config = SERIAL_CONFIG_DEFAULT;

    USART_Configuration();
#ifdef SERIAL_PORT_3_USED
    serial3.config = config;
    serial3.configure = stm32_configure;
    serial3.ioctl = stm32_ioctl;
    serial3.getc = stm32_getc;
    serial3.putc = stm32_putc;
    serial3.async_gets = stm32_async_gets;
    serial3.async_puts = stm32_async_puts;
    serial3.dma_transmit = stm32_dma_transmit;
    serial3.priv_data = &uart3;
    NVIC_Configuration(uart);
#endif // SERIAL_PORT_3_USED
}
