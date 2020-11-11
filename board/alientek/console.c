#include "device.h"

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"

void alientek_usart1_init(void)
{
    USART_InitTypeDef USART_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);

    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;

    GPIO_Init(GPIOA,&GPIO_InitStruct);

    USART_InitStruct.USART_BaudRate = 115200;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx |USART_Mode_Tx;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;

    USART_Init(USART1,&USART_InitStruct);

    USART_Cmd(USART1,ENABLE);
}

void alientek_putc(char ch)
{
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET) ;
    USART_SendData(USART1,ch);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET) ;
}


void alientek_console_init(void)
{
    alientek_usart1_init();
    console_init(alientek_putc);
}

stm32_board_init(alientek_console_init);
