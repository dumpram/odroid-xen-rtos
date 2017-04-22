/**
 * @ brief:
 *
 * ARM Generic interrupt controller support.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#ifndef _GIC_H
#define _GIC_H
#include <stdint.h>

/* Xen vGIC v2 mappings */
#define GUEST_GICD_BASE   0x03001000ULL
#define GUEST_GICD_SIZE   0x00001000ULL
#define GUEST_GICC_BASE   0x03002000ULL
#define GUEST_GICC_SIZE   0x00000100ULL


uint32_t gic_pmr();

uint32_t gic_bpr();

uint32_t gic_rpr();


#endif