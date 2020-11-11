#include "device.h"

#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"

#include "misc.h"

#include "FreeRTOS.h"
#include "task.h"

void pz6806l_systick_init(void)
{
    uint32_t reload = 0;
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);

    reload = 72 / 8; /* 9MHZ = 9000000 = 1s */
    reload *= 1000 * 5; /* 5ms */

    SysTick->LOAD = (reload * 1) - 1;

    /* enable */
    SysTick->CTRL |= (1UL << 1);
    SysTick->CTRL |= (1UL << 0);
}

uint32_t Tick;
void SysTick_Handler(void)
{
    Tick++;
    if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        xPortSysTickHandler();
    }
}

uint32_t get_tick(void)
{
    return Tick;
}

void pz6806l_usart3_init(void)
{
    USART_InitTypeDef USART_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);


    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;

    /* tx */
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;

    GPIO_Init(GPIOB,&GPIO_InitStruct);

    /* rx */
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;

    GPIO_Init(GPIOB,&GPIO_InitStruct);

    USART_InitStruct.USART_BaudRate = 115200;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx |USART_Mode_Tx;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;

    USART_Init(USART3,&USART_InitStruct);

    USART_Cmd(USART3,ENABLE);
}

void pz6806l_putc(char ch)
{
    while(USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET) ;
    USART_SendData(USART3,ch);
    while(USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET) ;
}

void pz6806l_console_init(void)
{
    pz6806l_usart3_init();
    console_init(pz6806l_putc);
}

stm32_board_init(pz6806l_console_init);
