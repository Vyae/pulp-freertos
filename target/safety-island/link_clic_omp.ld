OUTPUT_ARCH(riscv)
ENTRY( _start )
MEMORY
{
  LOCAL        : ORIGIN = 0x60000000, LENGTH = 0x00020000
  L2           : ORIGIN = 0x78200000, LENGTH = 0x00080000
  /* L2           : ORIGIN = 0x78200000, LENGTH = 0x00100000 */
  
}

/*
 * This linker script try to put core data in LOCAL private bank0 and core code
 * in LOCAL private bank1 to avoid contention between core code and data
 * as the core has no instruction cache and is so often accessing LOCAL to
 * get instructions. Everything can be shifted in case one bank is full.
 *
 * Cluster code and initialized data are put in L2 to not polute
 * private banks which are quite small, and also avoid contentions between
 * cluster cache refill and FC.
 */

SECTIONS
{
  PROVIDE(__boot_address = 0x60000080);
  /* stack and heap related settings */
  __stack_size = 0x1000;
  PROVIDE(__stack_size = __stack_size);
  /* heap_size is define in FreeRTOSConfig.h */
  __heap_size = 0x9000;
  PROVIDE(__heap_size = __heap_size);

  /*
   * LOCAL PRIVATE BANK0
   *
   * Contains Code
   */

  /* text */
  .init : ALIGN(4)
  {
    KEEP( *(.init) )
  } > LOCAL

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
  } > LOCAL

  .vectors ALIGN(1024) : /* lets leave the first 1k free for now "quarter zero page" */
  {
    __irq_vector_base = .;
    __vector_start = .;
    KEEP(*(.vectors))
  } > LOCAL

  .fini : ALIGN(4)
  {
    KEEP( *(.fini) )
  } > LOCAL


  .preinit_array : {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > LOCAL


  .init_array : ALIGN(4)
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
    KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > LOCAL

  .fini_array : ALIGN(4)
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
    KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > LOCAL

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
  } > LOCAL

  .dtors          :
  {
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
  } > LOCAL

  .boot : {
    . = ALIGN(4);
    *(.boot)
    *(.boot.data)
  } > LOCAL


  .talias : {
  } > LOCAL


  .gnu.offload_funcs : {
    . = ALIGN(4);
    KEEP(*(.gnu.offload_funcs))
  } > LOCAL


  .gnu.offload_vars : {
    . = ALIGN(4);
    KEEP(*(.gnu.offload_vars))
  } > LOCAL


  .heap : ALIGN(16)
  {
    __freertos_heap_start = .;

    KEEP(*(.heap))

    __freertos_heap_end = .;

    . = ALIGN(16);

    __heap_start = .;

    /*. += 23*1024;
     . = ALIGN(16);*/

    __heap_end = .;
  } > LOCAL


  .stack : {
    . = ALIGN(4);
    . = ALIGN(16);
    stack_start = .;
    __stack_bottom = .;
    . += __stack_size;
    __stack_top = .;
    __freertos_irq_stack_top = .; /* sytem stack */
    stack = .;
  } > LOCAL



  /*
   * L2 SHARED BANKS
   *
   * Contains other data such as peripheral data and cluster code and data
   */

  .l2_data :
  {
    . = ALIGN(4);
    *(.l2_data)
    *(.l2_data.*)
    *(.data_fc_shared)
    *(.data_fc_shared.*)
    . = ALIGN(4);
  } > L2

  .data : {
    . = ALIGN(4);
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
    _edata  =  .;
  } > L2


  .rodata : {
    . = ALIGN(4);
    *(.rodata);
    *(.rodata.*)
    *(.srodata);
    *(.srodata.*)
    *(.eh_frame*)
  } > L2


  .shbss : {
    . = ALIGN(4);
    *(.shbss)
  } > L2

  .bss : {
    . = ALIGN(8);
    _bss_start = .;
    __bss_start = .;
    *(.bss)
    *(.bss.*)
    *(.sbss)
    *(.sbss.*)
    *(COMMON)
    . = ALIGN(8);
    __bss_end = .;
    _bss_end = .;
    _end = .; /* man 3 end: first addr after bss */
  } > L2

  /* The compiler uses this to access data in the .sdata, .data, .sbss and .bss
     sections with fewer instructions (relaxation). This reduces code size. */
  PROVIDE(__global_pointer$ = ADDR(.data) + (SIZEOF(.data) + SIZEOF(.rodata) + SIZEOF(.shbss) +SIZEOF(.bss)) / 2);


  __l2_shared_end = ALIGN(4);

}
