/**
 * @ brief:
 *
 * ARM traps definitions and structures.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#ifndef _TRAPS_H_
#define _TRAPS_H_

struct pt_regs {
    unsigned long r0;
    unsigned long r1;
    unsigned long r2;
    unsigned long r3;
    unsigned long r4;
    unsigned long r5;
    unsigned long r6;
    unsigned long r7;
    unsigned long r8;
    unsigned long r9;
    unsigned long r10;
    unsigned long r11;
    unsigned long r12;
};

#endif /*----------  End of header file memory.h  ----------*/
