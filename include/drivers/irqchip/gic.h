/**
 * @ brief:
 *
 * ARM generic interrupt controller public function declarations.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#ifndef _GIC_H
#define _GIC_H

#include <stdint.h>

// Distributor Interface
#define GICD_CTLR         0x0
#define GICD_ISENABLER    0x100
#define GICD_IPRIORITYR   0x400
#define GICD_ITARGETSR    0x800
#define GICD_ICFGR        0xC00

// CPU Interface
#define GICC_CTLR         0x0
#define GICC_PMR          0x4
#define GICC_IAR          0xc
#define GICC_BPR          0x08
#define GICC_EOIR         0x10
#define GICC_RPR          0x14
#define GICC_HPPIR        0x18

struct gic {
    volatile char *gicd_base;
    volatile char *gicc_base;
};

void gic_init();

uint32_t gic_pmr();
uint32_t gic_bpr();
uint32_t gic_rpr();


#endif /*----------  End of header file gic.h  ----------*/
