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
#include <gpio/exynos5422-gpio.h>

#include <gpio.h>
#include <interrupt.h>
#include <memory.h>

extern uint32_t ulICCIAR;
extern uint32_t ulICCEOIR;
extern uint32_t ulICCPMR;
extern struct gic gic;

void free_rtos_init()
{
    ulICCIAR  = (uint32_t)gic.gicc_base + GICC_IAR;
    ulICCEOIR = (uint32_t)gic.gicc_base + GICC_EOIR;
    ulICCPMR  = (uint32_t)gic.gicc_base + GICC_PMR;

    print_register("ulICCIAR",  ulICCIAR);
    print_register("ulICCEOIR", ulICCIAR);
    print_register("ulICCPMR",  ulICCIAR);
}

void arch_early_init(uint32_t offset)
{
    print_simple("Arch early init!\n");

    // initialize memory first with physical to virtual offset 
    memory_api_init(offset);
    interrupt_api_init(&gic_driver);
    gpio_api_init(&exynos5422_gpio_driver);

    // FIX ME: this shouldn't be here, create os wrapper
    free_rtos_init();
}
