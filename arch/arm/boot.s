#include "arm/stack.s"
#include "arm/bss.s"

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

    ldr r7, =_page_dir      @ Virtual address of page table
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
    mov r0, r0, lsl#20          @ They are irrelevant in 1 MB mappings

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

    @ Map whole 4 GB of memory space
    mov r1, r7                  @ Start of L1 page table
    add r3, r1, #16 * 1024      @ Fill 16 kB of L1 page table
    orr r0, r8, r9              @ r8 = template, r9 = address of RAM
1:
    str r0, [r1],#4             @ Store at current position and increment
    add r0, r0, #1<<20          @ Next 1 MB
    cmp r3, r1                  @ Until everything is mapped
    bne 1b

    zero_bss
    setup_stacks

    @ Install exception vector table address
    ldr r0, =exception_vector_table
    mcr p15, 0, r0, c12, c0, 0

    mrc p15, #0, r1, c1, c0, #2           @ r1 = Access Control Register
    orr r1, r1, #(0xf << 20)              @ enable full access for p10,11
    mcr p15, #0, r1, c1, c0, #2           @ Access Control Register = r1
    mov r1, #0
    mcr p15, #0, r1, c7, c5, #4           @ flush prefetch buffer
    mov r0,#0x40000000
    fmxr FPEXC, r0                        @ Set Neon/VFP Enable bit

    cpsie  i                              @ Enable interrupts

    ldr lr,=__main
    mov r0, r9                  @ Pass physical to virtual offset to arch_init
    b arch_early_init

__main:
    ldr lr,=__exit              @ go to exit after main
    b main

__exit:
    b   .
