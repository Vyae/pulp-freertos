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

OUTPUT_ARCH(riscv)
ENTRY(_start)
MEMORY
{
  SRAM           : ORIGIN = 0x60000000, LENGTH = 0x00020000
}

/*
 * TODO: fix this
 * This linker script try to put FC data in SRAM private bank0 and FC code
 * in SRAM private bank1 to avoid contention between FC code and data
 * as FC has no instruction cache and is so often accessing SRAM to
 * get instructions. Everything can be shifted in case one bank is full.
 *
 * Cluster code and initialized data are put in shared banks to not polute
 * private banks which are quite small, and also avoid contentions between
 * cluster cache refill and FC.
 */


SECTIONS
{

  PROVIDE(__boot_address = 0x60010080);
  /* stack and heap related settings */
  __stack_size = DEFINED(__stack_size) ? __stack_size : 0x800;
  PROVIDE(__stack_size = __stack_size);
  __heap_size = DEFINED(__heap_size) ? __heap_size : 0x400;
  PROVIDE(__heap_size = __heap_size);

  /*
   * SRAM PRIVATE BANK0
   *
   * Contains code
   */

  /* text */
  .init : ALIGN(4)
  {
    KEEP( *(.init) )
  } > SRAM

  .vectors MAX(0x60000400, ALIGN(1024)) : /* lets leave the first 1k free for now "quarter zero page" */
  {
    __irq_vector_base = .;
    __vector_start = .;
    KEEP(*(.vectors))
  } > SRAM

  .text __boot_address :
  {
    _stext = .;
    *(.text.start)
    *(.text)
    *(.text.*)
    _etext = .; /* man 3 end: first addr after text */
    *(.lit)
    *(.shdata)
    _endtext = .;
    . = ALIGN(4);
  } > SRAM

  .fini : ALIGN(4)
  {
    KEEP( *(.fini) )
  } > SRAM


  __SRAM_priv0_end = ALIGN(4);


  /*
   * SRAM PRIVATE BANK1
   *
   * Contains FC data
   */


  /* make sure we start filling fc data into the correct bank */
  /* read-only sections */
  .rodata MAX(0x60008000, ALIGN(4)) :
  {
    *(.rodata .rodata.* .gnu.linkonce.r.*)
  } > SRAM
  .rodata1        :
  {
    *(.rodata1)
  } > SRAM


  .boot : ALIGN(4)
  {
    *(.boot)
    *(.boot.data)
  } > SRAM

  /* .got : ALIGN(4) */
  /* { */
  /*   *(.got.plt) * (.igot.plt) *(.got) *(.igot) */
  /* } > SRAM */

  .talias :
  {
  } > SRAM


  /* gcc language agnostic exception related sections (try-catch-finally) */
  .eh_frame_hdr :
  {
    *(.eh_frame_hdr) *(.eh_frame_entry .eh_frame_entry.*)
  } > SRAM
  .eh_frame       : ONLY_IF_RO
  {
    KEEP (*(.eh_frame)) *(.eh_frame.*)
  } > SRAM
  .gcc_except_table   : ONLY_IF_RO
  {
    *(.gcc_except_table .gcc_except_table.*)
  } > SRAM
  .gnu_extab   : ONLY_IF_RO
  {
    *(.gnu_extab*)
  } > SRAM


  /* Exception handling  */
  .eh_frame       : ONLY_IF_RW
  {
    KEEP (*(.eh_frame)) *(.eh_frame.*)
  } > SRAM
  .gnu_extab      : ONLY_IF_RW
  {
    *(.gnu_extab)
  } > SRAM
  .gcc_except_table   : ONLY_IF_RW
  {
    *(.gcc_except_table .gcc_except_table.*)
  } > SRAM
  .exception_ranges   : ONLY_IF_RW
  {
    *(.exception_ranges .exception_ranges*)
  } > SRAM


  /* Thread Local Storage sections  */
  .tdata	  :
  {
    PROVIDE_HIDDEN (__tdata_start = .);
    *(.tdata .tdata.* .gnu.linkonce.td.*)
  } > SRAM
  .tbss		  :
  {
    *(.tbss .tbss.* .gnu.linkonce.tb.*) *(.tcommon)
  } > SRAM


  /* constructors / destructors */
  .preinit_array : ALIGN(4)
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > SRAM

  .init_array : ALIGN(4)
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
    KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > SRAM

  .fini_array : ALIGN(4)
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
    KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > SRAM

  .ctors          :
  {
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    /* We don't want to include the .ctor section from
       the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
  } > SRAM

  .dtors          :
  {
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
  } > SRAM


  /* Open MP */
  .gnu.offload_funcs : ALIGN(4)
  {
    KEEP(*(.gnu.offload_funcs))
  } > SRAM


  .gnu.offload_vars : ALIGN(4)
  {
    KEEP(*(.gnu.offload_vars))
  } > SRAM


  .data : ALIGN(4)
  {
    sdata  =  .;
    _sdata  =  .;
    __data_begin = .;
    *(.data_fc)
    *(.data_fc.*)
    *(.data);
    *(.data.*)
    __sdata_begin = .;
    *(.sdata);
    *(.sdata.*)
    *(.sdata2.*) /* FreeRTOS xISRStackTop, __global_impure_ptr */
    *(.heapl2ram)
    *(.fcTcdm)
    *(.fcTcdm.*)
    *(.fcTcdm_g)
    *(.fcTcdm_g.*)
    . = ALIGN(4);
    edata  =  .;
    _edata  =  .; /* man 3 end: first addr after data */
  } > SRAM


  .bss : ALIGN(8)
  {
    _bss_start = .;
    __bss_start = .;
    *(.shbss) /* don't even know where this is from */
    *(.bss)
    *(.bss.*)
    *(.sbss)
    *(.sbss.*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end = .;
    _bss_end = .;
    _end = .; /* man 3 end: first addr after bss */
  } > SRAM


  /* The compiler uses this to access data in the .sdata, .data, .sbss and .bss
     sections with fewer instructions (relaxation). This reduces code size. */
    PROVIDE(__global_pointer$ = MIN(__sdata_begin + 0x800,
			    MAX(__data_begin + 0x800, __bss_end - 0x800)));

  .heap : ALIGN(16)
  {
    __heap_start = .;
    /* . += __heap_size; */
    /* __heap_end = .; */ /* Will be automatically filled by the ucHeap array */
    /* . = ALIGN(16); */
    KEEP(*(.heap))
    __heap_end = .;
  } > SRAM


  .stack : ALIGN(16)
  {
    stack_start = .;
    __stack_bottom = .;
    . += __stack_size;
    __stack_top = .;
    __freertos_irq_stack_top = .; /* sytem stack */
    stack = .;
  } > SRAM


  /*
   * Contains other data such as peripheral data and cluster code and data
   */

  .SRAM_data ALIGN(8) :
  {
    __cluster_text_start = .;
    *(.cluster.text)
    *(.cluster.text.*)
    . = ALIGN(4);
    __cluster_text_end = .;
    *(.SRAM_data)
    *(.SRAM_data.*)
    *(.data_fc_shared)
    *(.data_fc_shared.*)
    . = ALIGN(4);
  } > SRAM

  __SRAM_priv1_end = ALIGN(4);

}
