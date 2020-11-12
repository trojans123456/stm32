/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE. 
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/

/*调度模式配置：0时间片		1抢占*/
#define configUSE_PREEMPTION					1
/*某些运行freertos的硬件有两种方法选择下一个要执行的任务，通用方法和特定于硬件的方法
 1-特定硬件方法 依赖一个汇编指令 CLZ*/
#define configUSE_PORT_OPTIMISED_TASK_SELECTION	1
/*是否使用消息队列"SET"功能 0-不使用 1-使用*/
#define configUSE_QUEUE_SETS					1
/*1-使用空闲钩子  0忽略 空闲任务钩子是一个函数，由用户实现 vApplicationIdleHook*/
#define configUSE_IDLE_HOOK						0
/*1-使用时间片钩子，0-忽略vApplicationTickHook*/
#define configUSE_TICK_HOOK						0
/*cpu时钟主频 SystemCoreClock=168M*/
#define configCPU_CLOCK_HZ						( SystemCoreClock )
/*系统滴答*/
#define configTICK_RATE_HZ						( 1000 )
/*系统最大优先级 配置的优先级不能超过该值 数值越大优先级越高*/
#define configMAX_PRIORITIES					( 10 )
/*最小堆栈值，用于空闲定时等任务*/
#define configMINIMAL_STACK_SIZE				( ( unsigned short ) 130 )
/*系统总共堆栈大小 prvHeapInit使用*/
#define configTOTAL_HEAP_SIZE					( ( size_t ) ( 46 * 1024 ) ) // 46
/* 支持动态分配*/
#define configSUPPORT_DYNAMIC_ALLOCATION	1 // ??

/*任务名称最大长度xTaskCreate参数中限制*/
#define configMAX_TASK_NAME_LEN					( 10 )
/*1-启动可视化跟踪调试，会几乎一些附加的结构体和函数*/
#define configUSE_TRACE_FACILITY				0
/*是否使用16位滴答计数值 0-32位的滴答计数值  1-16位的滴答计数值*/
#define configUSE_16_BIT_TICKS					0
/*是否让空闲任务放弃抢占  0-不放弃  1-放弃抢占*/
#define configIDLE_SHOULD_YIELD					1


/*是否使用互斥锁  0-不使用 1-使用*/
#define configUSE_MUTEXES						1
/*可添加或登记队列名的数量 vQueueAddToRegistry
主要目的是给队列取名，方便调试*/
#define configQUEUE_REGISTRY_SIZE				20
/*用户任务堆栈溢出钩子函数，检查 vApplicationStackOverflowHook*/
#define configCHECK_FOR_STACK_OVERFLOW			0 
/*是否使用递归互斥锁 0-不适用 1-使用*/
#define configUSE_RECURSIVE_MUTEXES				1
/* malloc 失败的钩子函数 为1 vApplicationMallocFailedHook 自实现*/
#define configUSE_MALLOC_FAILED_HOOK			0
#define configUSE_APPLICATION_TASK_TAG			0
/*是否使用信号量计数功能 0-不使用 1-使用*/
#define configUSE_COUNTING_SEMAPHORES			1

/* The full demo always has tasks to run so the tick will never be turned off.
The blinky demo will use the default tickless idle implementation to turn the
tick off. */
/*低功耗tickless模式 0保持系统节拍tick中断一直运行，1-idle任务时不进行tick中断*/
#define configUSE_TICKLESS_IDLE					0

/* Run time stats gathering definitions. */
/*使能运行时间信息统计*/
#define configGENERATE_RUN_TIME_STATS	0

/* This demo makes use of one or more example stats formatting functions.  These
format the raw data provided by the uxTaskGetSystemState() function in to human
readable ASCII form.  See the notes in the implementation of vTaskList() within
FreeRTOS/Source/tasks.c for limitations. */
/*vTaskList vTaskGetRunTimeStats 获取任务运行时间信息 统计每个任务使用cpu的时间*/
#define configUSE_STATS_FORMATTING_FUNCTIONS	0

/* Co-routine definitions. */
/*1-使用协程，必须包含croutine.c 用于资源受限RAM很少*/
#define configUSE_CO_ROUTINES 			0
/*应用程序协程的有效优先级数目*/
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )

/* Software timer definitions. */
/*软件定时器*/
#define configUSE_TIMERS				0 //1
/*软件定时器优先级*/
#define configTIMER_TASK_PRIORITY		( configMAX_PRIORITIES - 1 )
#define configTIMER_QUEUE_LENGTH		5
/*软件定时器服务任务的堆栈深度*/
#define configTIMER_TASK_STACK_DEPTH	( configMINIMAL_STACK_SIZE * 2 )

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
/*1-表示 _vTaskxxx 函数才能使用*/
/*设置优先级任务*/
#define INCLUDE_vTaskPrioritySet		1
#define INCLUDE_uxTaskPriorityGet		1
#define INCLUDE_vTaskDelete				1
#define INCLUDE_vTaskCleanUpResources	1
#define INCLUDE_vTaskSuspend			1
/*任务延迟指定时间*/
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay				1
#define INCLUDE_eTaskGetState			1
#define INCLUDE_xTimerPendFunctionCall	1
#define INCLUDE_xTaskGetSchedulerState		1

/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS
	/* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
	#define configPRIO_BITS       		__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       		4        /* 15 priority levels */
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			15

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	2

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names. */
#define xPortPendSVHandler PendSV_Handler
#define vPortSVCHandler SVC_Handler
//#define xPortSysTickHandler SysTick_Handler

/* Prevent the inclusion of items the assembler will not understand in assembly
files. */
#ifndef __IAR_SYSTEMS_ASM__

	/* Library includes. */
	#include "FreeRTOS.h"

	extern uint32_t SystemCoreClock;

#ifdef DEBUG
	/* Normal assert() semantics without relying on the provision of an assert.h
	header file. */
	extern void vAssertCalled( uint32_t ulLine, const char *pcFile );
	#define configASSERT( x ) if( ( x ) == 0 ) vAssertCalled( __LINE__, __FILE__ )
#endif
	
#endif /* __IAR_SYSTEMS_ASM__ */
#endif /* FREERTOS_CONFIG_H */

