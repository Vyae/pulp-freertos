/*
 * Copyright (C) 2019 ETH Zurich and University of Bologna
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

#ifndef __PULP_MEM_MAP_H
#define __PULP_MEM_MAP_H


/*
 * SOC PERIPHERALS
 */

// for simulation using only safety-island
// #define SAFETY_ISLAND_BASE_ADDRESS 0x00000000u
// for simulation of safety island in carfield
#define SAFETY_ISLAND_BASE_ADDRESS 0x60000000u

#define PULP_SOC_PERIPHERALS_ADDR  0x00200000u

#define PULP_TIMER_OFFSET    0x00008000u
#define PULP_STDOUT_OFFSET   0x00006000u
#define PULP_CLIC_OFFSET     0x00010000u

#define PULP_TIMER_ADDR    (SAFETY_ISLAND_BASE_ADDRESS + PULP_SOC_PERIPHERALS_ADDR + PULP_TIMER_OFFSET)
#define PULP_CLIC_ADDR     (SAFETY_ISLAND_BASE_ADDRESS + PULP_SOC_PERIPHERALS_ADDR + PULP_CLIC_OFFSET)

// for simulation using safety-island's fs_handler uart (need to have CONFIG_STDIO == STDIO_FAKE)
// #define PULP_STDOUT_ADDR   (SAFETY_ISLAND_BASE_ADDRESS + PULP_SOC_PERIPHERALS_ADDR + PULP_STDOUT_OFFSET)
// for simulation using carfield's uart module (in cheshire) (need to have CONFIG_STDIO == STDIO_UART)
#define PULP_STDOUT_ADDR     0x03002000u

//Unused addresses (needed for compilation right now)
#define SOC_EU_ADDR 0x00000000
#define UDMA_CTRL_ADDR 0x00000000
#define FC_IRQ_ADDR	0x00000000

#endif
