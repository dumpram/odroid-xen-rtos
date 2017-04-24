/**
 * @ brief:
 *
 * ARM initalization routines.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#include <stdint.h>
#include <irqchip/gic.h>
#include <utils/print.h>

uint32_t p2v_offset;

extern uint32_t ulICCIAR;
extern uint32_t ulICCEOIR;
extern uint32_t ulICCPMR;

extern struct gic gic;

void free_rtos_init()
{
    ulICCIAR = (uint32_t)gic.gicc_base + GICC_IAR;
    ulICCEOIR = (uint32_t)gic.gicc_base + GICC_EOIR;
    ulICCPMR = (uint32_t)gic.gicc_base + GICC_PMR;

    print_register("ulICCIAR", ulICCIAR);
}

void arch_init(uint32_t offset)
{
    p2v_offset = offset;

    gic_init();

    free_rtos_init();
}
