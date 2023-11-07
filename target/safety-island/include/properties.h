/*
 * Copyright (C) 2019 ETH Zurich, University of Bologna and GreenWaves
 * Technologies
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
 */

#ifndef __PROPERTIES_H__
#define __PROPERTIES_H__


/* PULP defs */
#define PULP

/* variable system and fixed peripheral frequency */
#define ASIC_SYSTEM_CLOCK 600000000u /* 600 MhZ */
#define FPGA_SYSTEM_CLOCK 20000000u  /* 20 MhZ */

#define ASIC_PERIPH_CLOCK 100000000u /* 100 MhZ */
#define FPGA_PERIPH_CLOCK 10000000u  /* 10 MhZ */

/* TODO: fix this define name */
#define UDMA_NB_UART   (1)

/* interrupt configuration */
#ifdef CONFIG_CLIC
#define ISR_TABLE_SIZE 256 /* needs to be power of two */
#define CLIC_PARAM_NUM_SRC 256
#define CLIC_PARAM_CLIC_INT_CTL_BITS 8
#else
#define ISR_TABLE_SIZE 32 /* needs to be power of two */
#endif

#define PRINTF_BUFFER_SIZE              ( 128 )

#endif /* __PROPERTIES_H__ */
