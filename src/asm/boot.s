#include "asm/stack.s"
#include "asm/bss.s"

    .section ".start", "x"
    .align
    .globl    _start

_start:
    @ zImage header
.rept   8
    mov     r0, r0
.endr
    b       setup
    .word   0x016f2818      @ Magic numbers to help the loader
    .word   0               @ relocatable load/run zImage address
    .word   _end - _start   @ zImage size
    @ end of zImage header

setup:
    @ Linker script doesn't assume start position of image in RAM because it is 
    @ bad practice to hardcode RAM position.
    adr r1, _start          @ Gets address of start using relative offset
    ldr r3, =_start         @ Gets desired (virtual) address
    sub r9, r1, r3          @ R9 = virtual to physical offset

    ldr r7, =_page_dir @ Virtual address of page table
    add r1, r7, r9          @ Physical address of page table

    @ Sharable, Inner/Outer Write-Back Write-Allocate Cacheable 
    orr r0, r1, #0b0001011     
    
    @ set TTBR0
    mcr p15, 0, r0, c2, c0, 0

    @ set domains (deprecated) 
    mov r0, #1                  @ CLient mode
    mcr p15, 0, r0, c3, c0, 0   @ Update domain access control register

    @ Creating template for 1MB entries in L1 page table
    @ r8 = page table entry template
    ldr r8, =(0x2 + \
              0xc + \
              (3 << 10) + \
              (1 << 12) + \
              (1 << 16) + \
              (1 << 19))

    @ Add 1MB entry for current section
    mov r0, pc, lsr#20          @ Discard lower bits 
    mov r0, r0, lsl#20          @ They are irrelevant in 1 MB mappings?

    orr r3, r0, r8              @ r3 = entry for this section
    ldr r4, =_start             @ r4 = virtual address of this section

    str r3, [r1, r4, lsr#18]    @ Entry for first 1MB is in page table
    str r3, [r1, r0, lsr#18]    @ Entry for first 1MB is in page table
    
    dsb                         @ Wait for all transactions to end
    mcr p15, 0, r1, c8, c7, 0   @ Invalidate TLB

    @ Enable MMU / SCTLR
    mrc p15, 0, r1, c1, c0, 0   @ SCTLR
    orr r1, r1, #3 << 11        @ enable icache, branch prediction
    orr r1, r1, #4 + 1          @ enable dcache, MMU
    mcr p15, 0, r1, c1, c0, 0   @ SCTLR
    isb

    ldr r1, =stage2
    bx r1

stage2:

    zero_bss
    setup_stacks

    @ Install exception vector table address
    ldr r0, =exception_vector_table
    mcr p15, 0, r0, c12, c0, 0

    cpsie  i            @ Enable interrupts

    @ Greeting message from boot.s
    mov r0, #0
    mov r1, #45
    ldr r2, =msg
    bl HYPERVISOR_console_io    

__main:
    ldr lr,=__exit      @ go to exit after main
    b main

__exit:
    b   .

.align 4
msg:
    .asciz ">> Booting bare metal Xen application... <<\n"
