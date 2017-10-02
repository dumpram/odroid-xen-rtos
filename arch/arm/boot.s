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
    mov r9, #0

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
