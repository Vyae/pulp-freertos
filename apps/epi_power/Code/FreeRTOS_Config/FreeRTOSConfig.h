/*
 * FreeRTOS Kernel V10.0.1
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
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

#define DEFAULT_SYSTEM_CLOCK 50000000u /* Default System clock value */

/* Ensure stdint is only used by the compiler, and not the assembler. */
#if defined( __ICCARM__ ) || defined( __CC_ARM ) || defined( __GNUC__ )
    #include <stdint.h>
#endif

#ifdef __PULP__
#include "pmsis/targets/target.h"
#else
/* GAP8 specifics. */
#include "pmsis_targets.h"
#endif

/* extern volatile uint32_t SystemCoreClock; */
extern void vPrvAssertFailed( const char *filename, uint32_t line, const char *expr );

#ifdef __PULP__
/* There is no CLINT so the base address must be set to 0. */
#define configCLINT_BASE_ADDRESS 0
/* make newlib routines re-entrant */
/* TODO: disabled this because of reent bug when calling printf before scheduler start */
/* #define configUSE_NEWLIB_REENTRANT 1 */
#endif

/*
 * Use standard malloc instead of those offered(heap_X).
 * To use other malloc implementations, include heap_X file,
 * and define heap regions, see FreeRTOS_util files.
 * Call vPortDefineHeapRegions( xHeapRegions ) in the main function in order to
 * allocate memeory.
 */
//#define malloc                                    ( pmsis_malloc )
//#define free                                      ( pmsis_malloc_free )
#define pvPortMalloc                              ( pi_malloc )
#define vPortFree                                 ( pi_free )

/* Thread local storage. */
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS   ( 0 )

/* To disable notfications set to 0. */
#define configUSE_TASK_NOTIFICATIONS              ( 1 )
/* To get MPU working set to 1. */
#define INCLUDE_xQueueGetMutexHolder              ( 1 )

/* Disable when not necessary -> Save 25KB. Used by uxTimerStack. */
#define configSUPPORT_STATIC_ALLOCATION           ( 1 )
#define configSUPPORT_DYNAMIC_ALLOCATION          ( 1 )

#ifdef PREEMPTION
#define configUSE_PREEMPTION                      ( 1 )
#else
#define configUSE_PREEMPTION                      ( 0 )
#endif
#define configUSE_TIME_SLICING                    ( 1 )

/* Time slice. Quantum a task can execute on CPU. */
#define configPREEMPTION_QUANTUM                  ( 50 )

#define configIDLE_SHOULD_YIELD                   ( 1 )
#define configUSE_IDLE_HOOK                       ( 1 )
#define configUSE_TICK_HOOK                       ( 0 )
#define configUSE_TICKLESS_IDLE                   ( 0 )
#define configUSE_DAEMON_TASK_STARTUP_HOOK        ( 0 )
#define configCPU_CLOCK_HZ                        ( DEFAULT_SYSTEM_CLOCK )
#ifndef configTICK_RATE_HZ
#define configTICK_RATE_HZ                        ( ( TickType_t ) 1000 )
#endif
#define configMAX_PRIORITIES                      ( 16 )
#define configMINIMAL_STACK_SIZE                  ( ( uint16_t ) ( 128 ) )
#define configSTACK_DEPTH_TYPE                    uint32_t

/* PULP: we want to put the heap into special section */
#define configAPPLICATION_ALLOCATED_HEAP 1
#define configTOTAL_HEAP_SIZE		 ((size_t)(32 * 1024))

#define configMAX_TASK_NAME_LEN                   ( 16 )
#define configUSE_TRACE_FACILITY                  ( 1 )
#define configUSE_16_BIT_TICKS                    ( 0 )

#define configUSE_MUTEXES                         ( 1 )
#define configQUEUE_REGISTRY_SIZE                 ( 8 )
/* disable stack overflow checking in release builds */
#ifndef NDEBUG
#define configCHECK_FOR_STACK_OVERFLOW            ( 2 )
#else
#define configCHECK_FOR_STACK_OVERFLOW            ( 0 )
#endif
#define configUSE_RECURSIVE_MUTEXES               ( 1 )
#define configUSE_MALLOC_FAILED_HOOK              ( 0 )
#define configUSE_APPLICATION_TASK_TAG            ( 0 )
#define configUSE_COUNTING_SEMAPHORES             ( 1 )
#define configGENERATE_RUN_TIME_STATS             ( 0 )
#define configOVERRIDE_DEFAULT_TICK_CONFIGURATION ( 1 )
#define configRECORD_STACK_HIGH_ADDRESS           ( 1 )

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES                     ( 0 )
#define configMAX_CO_ROUTINE_PRIORITIES           ( 2 )

/* Software timer definitions. */
#ifndef configUSE_TIMERS
#define configUSE_TIMERS                          ( 0 )
#else
#define configTIMER_TASK_PRIORITY                 ( configMAX_PRIORITIES - 1 )
#define configTIMER_QUEUE_LENGTH                  ( 10 )
#define configTIMER_TASK_STACK_DEPTH              ( configMINIMAL_STACK_SIZE * 2 )
#endif

/* Set the following definitions to 1 to include the API function, or zero
 * to exclude the API function. */
#define INCLUDE_vTaskPrioritySet                  ( 1 )
#define INCLUDE_uxTaskPriorityGet                 ( 1 )
#define INCLUDE_vTaskDelete                       ( 1 )
#define INCLUDE_vTaskCleanUpResources             ( 0 )
#define INCLUDE_vTaskSuspend                      ( 1 )
#define INCLUDE_vTaskDelayUntil                   ( 1 )
#define INCLUDE_vTaskDelay                        ( 1 )
#define INCLUDE_xTaskGetSchedulerState            ( 1 )
#define INCLUDE_xTaskGetIdleTaskHandle            ( 1 )
#define INCLUDE_eTaskGetState                     ( 1 )

/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS
    /* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
    #define configPRIO_BITS    __NVIC_PRIO_BITS
#else
    #define configPRIO_BITS    4                                 /* 15 priority levels. */
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority"
 * function. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY         0xf

/* The highest interrupt priority that can be used by any interrupt service
 * routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
 * INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
 * PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY    5

/* Interrupt priorities used by the kernel port layer itself.  These are generic
* to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY \
    ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << ( 8 - configPRIO_BITS ) )

/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
 * See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY \
    ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << ( 8 - configPRIO_BITS ) )

/* Normal assert() semantics without relying on the provision of an assert.h
 * header file. */
#ifndef NDEBUG
#define configASSERT( x )                                               \
    if( ( x ) == 0 ) { vPrvAssertFailed( __FILE__, __LINE__, #x ); }
#else
#define configASSERT( x ) ( void ) ( x )
#endif

/* Map the FreeRTOS printf() to the logging task printf. */
#define configPRINTF( x )          printf( x )

/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
 * standard names. */
#define vPortSVCHandler               ecall_handler
#define xPortPendSVHandler            pendSV_handler
#define vHardFault_Handler            HardFault_Handler

/* IMPORTANT: This define MUST be commented when used with STM32Cube firmware,
 *            to prevent overwriting SysTick_Handler defined within STM32Cube HAL. */
#define xPortSysTickHandler systick_handler

#endif /* FREERTOS_CONFIG_H */
