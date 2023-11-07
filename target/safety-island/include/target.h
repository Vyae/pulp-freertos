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
 */

/* Author: Robert Balas */

#ifndef __TARGET_H__
#define __TARGET_H__

#include "riscv.h"

static inline void hal_compiler_barrier()
{
	asm volatile("" : : : "memory");
}

static inline void hal_write32(volatile void *addr, uint32_t value)
{
	asm volatile("" : : : "memory");
	*((volatile uint32_t *)addr) = value;
	asm volatile("" : : : "memory");
}

static inline void hal_write8(volatile void *addr, uint8_t value)
{
	asm volatile("" : : : "memory");
	*((volatile uint8_t *)addr) = value;
	asm volatile("" : : : "memory");
}

static inline void hal_or32(volatile void *addr, uint32_t value)
{
	asm volatile("" : : : "memory");
	*((volatile uint32_t *)addr) |= value;
	asm volatile("" : : : "memory");
}

static inline void hal_and32(volatile void *addr, uint32_t value)
{
	asm volatile("" : : : "memory");
	*((volatile uint32_t *)addr) &= value;
	asm volatile("" : : : "memory");
}

static inline uint32_t hal_read32(volatile void *addr)
{
	asm volatile("" : : : "memory");
	uint32_t ret = *((volatile uint32_t *)addr);
	asm volatile("" : : : "memory");
	return ret;
}

static inline uint8_t hal_read8(volatile void *addr)
{
	asm volatile("" : : : "memory");
	uint8_t ret = *((volatile uint8_t *)addr);
	asm volatile("" : : : "memory");
	return ret;
}


#endif
