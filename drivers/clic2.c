/*
 * Copyright (C) 2023 ETH Zurich and University of Bologna
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
 * Authors: Robert Balas (balasr@iis.ee.ethz.ch)
 * 			Nicole Narr  (narrn@student.ethz.ch)
 */

/* Spec-conform Core-local interrupt controller for PULP. */

#include <stdint.h>
#include <assert.h>

#include "pulp_mem_map.h"
#include "properties.h"
#include "io.h"
#include "csr.h"
#include "clic2.h"

extern void (*isr_table[ISR_TABLE_SIZE])(void);

/* set interrupt handler for given interrupt id */
void irq_set_handler(int id, void (*handler)(void))
{
	assert(0 <= id && id < ISR_TABLE_SIZE);
	isr_table[id] = handler;
}

void irq_disable(int id)
{
	assert(0 <= id && id < CLIC_PARAM_NUM_SRC);
	// Clear shv and ie bit
	writew(0xFFFEFEFF & readw(PULP_CLIC_ADDR + CLICINT_REG_OFFSET(id)),
		(uintptr_t)(PULP_CLIC_ADDR + CLICINT_REG_OFFSET(id)));
}

void irq_pend(int id)
{
	assert(0 <= id && id < CLIC_PARAM_NUM_SRC);
	// Set ip bit
	writew(1ul << CLICINT_IP_BIT | readw(PULP_CLIC_ADDR + CLICINT_REG_OFFSET(id)),
		(uintptr_t)(PULP_CLIC_ADDR + CLICINT_REG_OFFSET(id)));
}

void irq_clear(int id)
{
	assert(0 <= id && id < CLIC_PARAM_NUM_SRC);
	// Clear ip bit
	writew(0xFFFFFFFE & readw(PULP_CLIC_ADDR + CLICINT_REG_OFFSET(id)), 
		(uintptr_t)(PULP_CLIC_ADDR + CLICINT_REG_OFFSET(id)));
}

void irq_set_lvl_and_prio(int id, int lvl, int prio)
{
	// Read out number of implemented level bits
	uint32_t mnlbits = readw((uintptr_t)(PULP_CLIC_ADDR + MCLIC_CLICCFG_REG_OFFSET)) >>
				  CLIC_CLICCFG_MNLBITS_OFFSET & CLIC_CLICCFG_MNLBITS_MASK;

	// Concatenate lvl and prio depending on mnlbits
	uint32_t shift = 8 - mnlbits;
	uint32_t val = ((((uint32_t)lvl & BIT_MASK(mnlbits)) << shift |
			((uint32_t)prio & BIT_MASK(shift))) & CLICINT_CTL_MASK);
	
	// Write value
	writew((val << CLICINT_CTL_OFFSET) | 
		(readw(PULP_CLIC_ADDR + CLICINT_REG_OFFSET(id)) & ~(CLICINT_CTL_MASK << CLICINT_CTL_OFFSET)),
        PULP_CLIC_ADDR + CLICINT_REG_OFFSET(id));
}

void irq_set_trigger_type(int id, int flags)
{
	writew((flags << CLICINT_ATTR_TRIG_OFFSET) |
        (readw(PULP_CLIC_ADDR + CLICINT_REG_OFFSET(id)) & ~(CLICINT_ATTR_TRIG_MASK << CLICINT_ATTR_TRIG_OFFSET)),
        PULP_CLIC_ADDR + CLICINT_REG_OFFSET(id));
}

/* utility functions for PULPs external interrupt controller */
void irq_enable(int id)
{
	assert(0 <= id && id < CLIC_PARAM_NUM_SRC);
	// Enable selective hardware vectoring for interrupt. We might later make this configurable
    writew((0x1 << CLICINT_ATTR_SHV_BIT) | (readw(PULP_CLIC_ADDR + CLICINT_REG_OFFSET(id))),
        PULP_CLIC_ADDR + CLICINT_REG_OFFSET(id));

	// Enable interrupt id
	writew(1 << CLICINT_IE_BIT, (uintptr_t)(PULP_CLIC_ADDR + CLICINT_REG_OFFSET(id)));

	/* TODO: fix this quick hack which is there just to get going */
	// Set level and prio
	irq_set_lvl_and_prio(id, 1, 1);

	/* TODO: assume edge triggered interrupt by default */
	//irq_set_trigger_type(id, CLIC_TRIG_EDGE | CLIC_TRIG_POSITIVE);
	irq_set_trigger_type(id, 0x19);
}

void pulp_irq_init()
{
	/* min threshold, thereby propagating all interrupts */
	csr_write(CSR_MINTTHRESH, 0x0);
	/* set mnlbits to four which gives 4 bits for level and priority */
	/* TODO: implement freertos level interrupts */
	writew((0x4 << CLIC_CLICCFG_MNLBITS_OFFSET) | 
		(readw(PULP_CLIC_ADDR + MCLIC_CLICCFG_REG_OFFSET) & ~(CLIC_CLICCFG_MNLBITS_MASK << CLIC_CLICCFG_MNLBITS_OFFSET)), 
		PULP_CLIC_ADDR + MCLIC_CLICCFG_REG_OFFSET);

	// enable mnxti extension by writing to custom memory-mapped reg
    // enabling mnxti in core
    writew((0x0 << CLIC_MNXTI_BIT), PULP_CLIC_ADDR + MCLIC_MNXTICFG_REG_OFFSET);
    csr_write(CSR_MNXTICFG, 0);
}
