/*
 * Copyright 2020 ETH Zurich
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 * Author: Robert Balas (balasr@iis.ee.ethz.ch)
 */

/* Description: Platform system level functions */

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <stdint.h>
#include <stdbool.h>

#include "FreeRTOSConfig.h"

#ifndef DEFAULT_SYSTEM_CLOCK
/* Default System clock value */
#define DEFAULT_SYSTEM_CLOCK ASIC_SYSTEM_CLOCK
#endif

extern volatile uint32_t system_core_clock;

void system_init(void);
void system_core_clock_update(void);
uint32_t system_core_clock_get(void);


/* todo, right now keep exactly 32khz */
#define ARCHI_REF_CLOCK	     (32000)

/* interrupt configuration */
#ifdef CONFIG_CLIC
#define ISR_TABLE_SIZE 256 /* needs to be power of two */
#define CLIC_PARAM_NUM_SRC 256
#define CLIC_PARAM_CLIC_INT_CTL_BITS 8
#else
#define ISR_TABLE_SIZE 32 /* needs to be power of two */
#endif


/* Interrupt line masks: these interrupts directly go to the core (after being
 * serialized as reqest + id). We refer to these interrupts with the prefix IRQ.
 */
//#define IRQ_SW0	    (0)
//#define IRQ_SW1	    (1)
//#define IRQ_SW2	    (2)
#define IRQ_MSIP	    (3)
//#define IRQ_SW4	    (4)
//#define IRQ_SW5	    (5)
//#define IRQ_SW6	    (6)
#define IRQ_MTIMER      (7) // Currently Timer Low (so same as 16)
//#define IRQ_SW8       (8)
#define IRQ_SEIP        (9)
//#define IRQ_SW10	   (10)
#define IRQ_MEIP       (11)
//#define IRQ_SW12	   (12)
//#define IRQ_SW13	   (13)
//#define IRQ_SW14	   (14)
//#define IRQ_SW15	   (15)

#define IRQ_TIMER_LO   (16)
#define IRQ_TIMER_HI   (17)

#define IRQ_BUS_ERR1   (18)
#define IRQ_BUS_ERR2   (19)
#define IRQ_BUS_ERR3   (20)

#define IRQ_RESYNCH    (21)

#endif /* __SYSTEM_H_ */
