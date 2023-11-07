/*
 * Copyright (C) 2021 ETH Zurich and University of Bologna
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

#ifndef __CLIC2_H__
#define __CLIC2_H__

// Register width
#define CLICINT_PARAM_REG_WIDTH 32

// Machine mode CLIC Configuration
#define MCLIC_CLICCFG_REG_OFFSET	0x0

// CLIC configuration
#define CLIC_CLICCFG_MNLBITS_MASK   0xf
#define CLIC_CLICCFG_MNLBITS_OFFSET 0x0
#define CLIC_CLICCFG_NMBITS_MASK    0x3
#define CLIC_CLICCFG_NMBITS_OFFSET  5
#define CLIC_CLICCFG_SNLBITS_MASK   0xf
#define CLIC_CLICCFG_SNLITS_OFFSET  16
#define CLIC_CLICCFG_UNLBITS_MASK   0xf
#define CLIC_CLICCFG_UNLBITS_OFFSET 24

#define MCLIC_MNXTICFG_REG_OFFSET	 (MCLIC_CLICCFG_REG_OFFSET + 0x4)
#define CLIC_MNXTI_BIT 0

// CLIC interrupt pending, enable, attribute and control
#define CLICINT_REG_OFFSET(id) (0x1000 + 0x4 * id)

#define CLICINT_IP_BIT 0
#define CLICINT_IE_BIT 8
#define CLICINT_ATTR_SHV_BIT 16
#define CLICINT_ATTR_TRIG_MASK 0x3
#define CLICINT_ATTR_TRIG_OFFSET 17
#define CLICINT_ATTR_MODE_MASK 0x3
#define CLICINT_ATTR_MODE_OFFSET 22
#define CLICINT_CTL_MASK 0xff
#define CLICINT_CTL_OFFSET 24


// Supervisor mode CLIC Configuration
#define SCLIC_CLICCFG_REG_OFFSET	0x8000


#endif /* __CLIC2_H__ */