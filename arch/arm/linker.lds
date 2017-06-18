OUTPUT_ARCH(arm)
ENTRY(_start)

STACK_SIZE = 16K;
PAGE_TABLE_SIZE = 16K;
HEAP_SIZE = 16K;

SECTIONS
{

  _page_dir      = 0x404000;  /* 16 KB translation table */
  .    = 0x408000;

  .text : {
    *(.start);
    *(.text);
  }

  . = ALIGN(4);

  .rodata : { *(.rodata); }

  /* newlib initialization functions */
  . = ALIGN(32 / 8);
  PROVIDE (__preinit_array_start = .);
  .preinit_array     : { *(.preinit_array) }
  PROVIDE (__preinit_array_end = .);
  PROVIDE (__init_array_start = .);
  .init_array     : { *(.init_array) }
  PROVIDE (__init_array_end = .);
  PROVIDE (__fini_array_start = .);
  .fini_array     : { *(.fini_array) }
  PROVIDE (__fini_array_end = .);

  .ctors : {
        __CTOR_LIST__ = .;
        *(.ctors)
  CONSTRUCTORS
        LONG(0)
        __CTOR_END__ = .;
        }

  .dtors : {
        __DTOR_LIST__ = .;
        *(.dtors)
        LONG(0)
        __DTOR_END__ = .;
        }

  .data : { *(.data); }

  . = ALIGN(4);

  /* NOTE: The space specified in the zImage header (_end - _start) MUST be no
   * bigger than the binary image size of the entire kernel. */
  _end = .;

  . = ALIGN(8);
  _start_stacks = .;
  svc_stack = _start_stacks + STACK_SIZE;
  irq_stack = svc_stack + STACK_SIZE;
  firq_stack = irq_stack + STACK_SIZE;
  abt_stack = firq_stack + STACK_SIZE;
  und_stack = abt_stack + STACK_SIZE;

  . = und_stack;  

  . = ALIGN(4);
  .bss : { 
    _sbss = .;
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
    __bss_end__ = _ebss;
  }
  
  . = ALIGN(8);
  .heap : {
    . = . + HEAP_SIZE;
    end = .;
    _end = end;
  }
}
