#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#include "device.h"
#include "usart.h"

//for 485
#define UART_TX     Bit_SET
#define UART_RX     Bit_RESET

#define UART_TXERR_TIME     10
#define UART_RXOVER_TIME    3

void UartCom_Initialize(struct serial_ *baseCfg);
void UartCom_IRQ(struct serial_ *serial);
unsigned int UartCom_GetMsg(struct serial_ *serial, void *pRecvBuf, unsigned int len);
void UartCom_SendMsg(struct serial_ *serial, void *pSendBuf, unsigned int len);
void UartCom_RtCtrl(struct serial_ *serial,u8 rtStatus);
void UartCom_SetParmas(struct serial_ *serial,serial_params_t *params);
void UartCom_CheckRxOver(serial_t *serial);



/****** inner ********/
struct UartCom
{
    USART_TypeDef *USARTx;
    IRQn_Type irq;
};
static struct UartCom systemUsart[UART_COMNUM];

typedef void (*rcc_func_ptr)(int,int);
struct rs485_ctrl_pin
{
    rcc_func_ptr rcc_ptr;
    unsigned int rcc;
    GPIO_TypeDef *gpio;
    unsigned int pin;
};
#define RS485_PIN_CONFIG(rcc,gpio,gpio_index) { RCC_##rcc##PeriphClockCmd,RCC_##rcc##Periph_GPIO##gpio, GPIO##gpio, GPIO_Pin_##gpio_index}

static serial_ops_t uart_ops =
{
    UartCom_Initialize,
    UartCom_SetParmas,
    UartCom_GetMsg,
    UartCom_SendMsg,
    UartCom_RtCtrl,
    UartCom_IRQ,
    UartCom_CheckRxOver
};

/***** COM3 *****/
#define UART_COM3_UART          USART3
#define UART_COM3_CLK           RCC_APB1Periph_USART3

#define UART_COM3_TX_PIN        GPIO_Pin_10
#define UART_COM3_TX_GPIO       GPIOB
#define UART_COM3_TX_GPIO_CLK   RCC_APB2Periph_GPIOB

#define UART_COM3_RX_PIN        GPIO_Pin_11
#define UART_COM3_RX_GPIO       GPIOB
#define UART_COM3_RX_GPIO_CLK   RCC_APB2Periph_GPIOB

#define UART_COM3_IRQn          USART3_IRQn


#define COM3_TX_LEN     256
#define COM3_RX_LEN     256
static unsigned char UartCom3Txbuf[COM3_TX_LEN];
static unsigned char UartCom3Rxbuf[COM3_RX_LEN];


serial_t UartCom3 =
{
    {
        UART_COM3,
        115200,
        USART_WordLength_8b,
        USART_StopBits_1,
        USART_Parity_No,
        USART_Mode_Rx | USART_Mode_Tx
    },
    {0},
    0,0,
        {0},{0},
    NULL,
    &uart_ops
};


void USART3_IRQHandler(void)
{
    UartCom3.ops->irq(&UartCom3);
}

void USART3_configuration(serial_params_t *param)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStructure;

    /* enable gpio clock */
    RCC_APB2PeriphClockCmd(UART_COM3_TX_GPIO_CLK | UART_COM3_RX_GPIO_CLK,ENABLE);




    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//????
    GPIO_InitStruct.GPIO_Pin = UART_COM3_TX_PIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(UART_COM3_TX_GPIO,&GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Pin = UART_COM3_RX_PIN;

    GPIO_Init(UART_COM3_RX_GPIO,&GPIO_InitStruct);

    /* enable usart clock */
    RCC_APB1PeriphClockCmd(UART_COM3_CLK,ENABLE);

    USART_InitStructure.USART_BaudRate = param->baudRate;
    USART_InitStructure.USART_WordLength = param->WordLength;
    USART_InitStructure.USART_StopBits = param->stopBits;
    USART_InitStructure.USART_Parity = param->parity;
    USART_InitStructure.USART_Mode = param->mode;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

    USART_Init(UART_COM3_UART,&USART_InitStructure);
    USART_Cmd(UART_COM3_UART,ENABLE);

    USART_ITConfig(UART_COM3_UART,USART_IT_RXNE,ENABLE);
    USART_ITConfig(UART_COM3_UART,USART_IT_TC,ENABLE);
    USART_ClearFlag(UART_COM3_UART,USART_FLAG_TC);
}

/*********** public *****************/
void UartCom_Initialize(struct serial_ *baseCfg)
{
    unsigned char No = baseCfg->param.No % UART_COMNUM;
    struct UartCom *curUsart = &systemUsart[No];

    switch(No)
    {
    case UART_COM3:
        {
            curUsart->USARTx = UART_COM3_UART;
            curUsart->irq = UART_COM3_IRQn;

            fifo_init(&UartCom3.TxList,UartCom3Rxbuf,COM3_TX_LEN);
            fifo_init(&UartCom3.RxList,UartCom3Txbuf,COM3_RX_LEN);

            USART3_configuration(&baseCfg->param);
        }
        break;
    default:
        break;
    }
}
void UartCom_IRQ(struct serial_ *serial)
{
    unsigned char dat;
    struct UartCom *curUsart = &systemUsart[serial->param.No % UART_COMNUM];

    if(USART_GetITStatus(curUsart->USARTx,USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(curUsart->USARTx,USART_IT_RXNE);

        dat = (unsigned char)USART_ReceiveData(curUsart->USARTx);
        fifo_push(&serial->RxList,dat);
        serial->RxOverTime = UART_RXOVER_TIME;
    }

    if(USART_GetITStatus(curUsart->USARTx,USART_IT_TC) != RESET)
    {
        USART_ClearITPendingBit(curUsart->USARTx,USART_IT_TC);

        if(fifo_isempty(&serial->TxList) != True)
        {
            fifo_pop(&serial->TxList,&dat);
            USART_SendData(curUsart->USARTx,dat);
            serial->TxErrTime = UART_TXERR_TIME;
        }
        else
        {
            serial->TxErrTime = 0;
            serial->flg.b0 = 0;
            serial->ops->rt_ctrl(serial,UART_TX);
        }
    }
    USART_ClearITPendingBit(curUsart->USARTx,USART_IT_LBD);
}
unsigned int UartCom_GetMsg(struct serial_ *serial, void *pRecvBuf, unsigned int len)
{
    unsigned int n=0;

    if(NULL == pRecvBuf)
        return 0;
    while((fifo_isempty(&serial->RxList) != True)&&(len--)&&(serial->RxOverTime==0))
    {
        fifo_pop(&serial->RxList,(((unsigned char *)pRecvBuf)+n));
        n++;
    }
    return n;
}
void UartCom_SendMsg(struct serial_ *serial, void *pSendBuf, unsigned int len)
{
    unsigned char *ptr = (unsigned char *)pSendBuf + 1;
    unsigned char dat = *(uint8_t*)pSendBuf;
    struct UartCom *curUsart = &systemUsart[serial->param.No % UART_COMNUM];

    if(NULL == pSendBuf || len == 0)
        return;
    if(serial->flg.b1)
    {
        serial->flg.b1 = 0;
        serial->flg.b0 = 0;

    }

    if(len == 1 && (0==serial->flg.b0))
    {
        serial->ops->rt_ctrl(serial,UART_TX);
        serial->flg.b0 = 1;
        serial->flg.b1 = 0;
        serial->TxErrTime = UART_TXERR_TIME;
        USART_SendData(curUsart->USARTx, dat);

        return ;
    }
    len --;
    while(len--)
    {
        fifo_push(&serial->TxList,*ptr++);
    }

    if(0==serial->flg.b0)
    {
            serial->ops->rt_ctrl(serial,UART_TX);
            serial->flg.b0 = 1;
            serial->flg.b1 = 0;
            serial->TxErrTime = UART_TXERR_TIME;
            USART_SendData(curUsart->USARTx, dat);
    }
}
void UartCom_RtCtrl(struct serial_ *serial,u8 rtStatus)
{
    if(serial->private_user)
    {
        struct rs485_ctrl_pin *ctrl_pin = (struct rs485_ctrl_pin *)serial->private_user;

        GPIO_WriteBit(ctrl_pin->gpio,ctrl_pin->pin,(BitAction)rtStatus);
    }
}

void UartCom_SetParmas(struct serial_ *serial,serial_params_t *params)
{
    unsigned char no = serial->param.No % UART_COMNUM;
    switch(no)
    {
    case UART_COM3:
        memcpy(&serial->param,params,sizeof(serial->param));
        USART3_configuration(&serial->param);
        break;
    default:
        break;
    }
}
void UartCom_CheckRxOver(serial_t *serial)
{
    /** 为了确定何时算接收完一帧。要么每次读的时候 禁止中断。要么定时计数 **/
    if(NULL == serial)
        return;
    if(serial->RxOverTime > 0)
    {
        serial->RxOverTime--;
    }
    if(serial->flg.b0)
    {
        if(serial->TxErrTime--==0)
        {
            serial->flg.b1 = 1;
        }
    }

}

static void NVIC_Configuration(struct serial_ *uart,unsigned char sub_pri)
{
    unsigned char no = uart->param.No % UART_COMNUM;
    struct UartCom *curUsart = &systemUsart[no];

    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the USART1 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = curUsart->irq;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub_pri;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void usart_checkover(void)
{
    /* com1 */
    UartCom3.ops->checkover(&UartCom3);
}

serial_t *usart_find(unsigned int port)
{
    serial_t *dev = NULL;

    if(port >= UART_COMNUM)
        return NULL;

    switch(port % UART_COMNUM)
    {
    case UART_COM3:
        dev = &UartCom3;
        break;
    default:
        break;
    }
    return dev;
}


void stm32f1_usart_init(void)
{
    /* com3 */
    UartCom3.ops->init(&UartCom3);
    NVIC_Configuration(&UartCom3,1);


}

stm32_board_init(stm32f1_usart_init);
