OUTPUT_ARCH(arm)
ENTRY(_start)
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
  .data : { *(.data); }

  . = ALIGN(4);

  /* NOTE: The space specified in the zImage header (_end - _start) MUST be no
   * bigger than the binary image size of the entire kernel. */
  _end = .;

  . = ALIGN(8);
  _start_stacks = .;
  svc_stack = _start_stacks + 16384;
  irq_stack = _start_stacks + 32768;
  firq_stack = _start_stacks + 49152;
  abt_stack = _start_stacks + 65536;
  und_stack = _start_stacks + 81920;

  . = und_stack;  

  . = ALIGN(4);
  _bss_start = .;
  .bss : { *(.bss); }
  . = ALIGN(4);
  _bss_end = .;

  . = ALIGN(16K);
  _l1_page_table = .;
  _l1_page_table_end = _l1_page_table + 16 * 1024;
  . = _l1_page_table_end;
}
