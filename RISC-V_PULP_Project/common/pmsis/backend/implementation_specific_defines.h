#ifndef __IMPLEMENTATION_SPECIFIC_DEFINES_H__
#define __IMPLEMENTATION_SPECIFIC_DEFINES_H__

#include "stdlib.h"
#include "string.h"

#if defined(__GAP8__)
#include "system_gap8.h"
#elif defined(__GAP9__)
#include "system_gap9.h"
#elif defined(__PULP__)
#include "system_pulp_ri5cy.h"
#else
#error "Target specific macro is not set. Recommended to use __PULP__."
#endif

#define __INC_TO_STRING(x) #x

#define PMSIS_TASK_EVENT_KERNEL_PRIORITY     2
#define DEFAULT_MALLOC_INC                   __INC_TO_STRING(pmsis/rtos/malloc/l2_malloc.h)

// default malloc for driver structs etc (might not be compatible with udma!)
/* TODO: rereoute to newlib malloc (?)*/
#ifdef __PULP__
#define pi_default_malloc(x)  malloc(x)
#define pi_default_free(x,y)  free(x)
#define pi_data_malloc(x)     malloc(x)
#define pi_data_free(x,y)     free(x)
#else
#error "__PULP__ not set: malloc not supported"
#define pi_default_malloc(x)  pmsis_l2_malloc(x)
#define pi_default_free(x,y)  pmsis_l2_malloc_free(x,y)
#define pi_data_malloc(x)     pmsis_l2_malloc(x)
#define pi_data_free(x,y)     pmsis_l2_malloc_free(x,y)
#endif

#define PI_TASK_IMPLEM                          \
    uint8_t destroy;

#define CLUSTER_TASK_IMPLEM

#endif  /* __IMPLEMENTATION_SPECIFIC_DEFINES_H__ */
