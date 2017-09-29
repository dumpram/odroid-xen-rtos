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
#include <exti/exynos5422-exti.h>
#include <console/exynos5422-console.h>
#include <console/xen-console.h>

#include <gpio.h>
#include <interrupt.h>
#include <memory.h>
#include <exti.h>
#include <console.h>

console_params_t default_console_params = {
    .baud = 115200,
    .data_bits = 8,
    .parity = CONSOLE_PARITY_NONE,
    .stop_bits = 1
};

// FIX me: this can be in separate layer...
#include <xen/xen.h>

extern uint32_t ulICCIAR;
extern uint32_t ulICCEOIR;
extern uint32_t ulICCPMR;
extern struct gic gic;

/*
 * Shared page for communicating with the hypervisor.
 * Events flags go here, for example.
 */
shared_info_t *HYPERVISOR_shared_info;


void free_rtos_init()
{
    ulICCIAR  = (uint32_t)gic.gicc_base + GICC_IAR;
    ulICCEOIR = (uint32_t)gic.gicc_base + GICC_EOIR;
    ulICCPMR  = (uint32_t)gic.gicc_base + GICC_PMR;

    print_register("ulICCIAR",  ulICCIAR);
    print_register("ulICCEOIR", ulICCEOIR);
    print_register("ulICCPMR",  ulICCPMR);
}


void arch_early_init(uint32_t offset)
{
    print_simple("Arch early init!\n");
    print_register("offset", offset);

    // initialize memory first with physical to virtual offset 
    memory_api_init(offset);
    interrupt_api_init(&gic_driver);
    gpio_api_init(&exynos5422_gpio_driver);
    exti_api_init(&exynos5422_exti_driver);

#if BARE_METAL == 1
    console_api_init(&exynos5422_console_driver, &default_console_params);
#elif XEN_EMERGENCY_CONSOLE == 1
    console_api_init(&xen_emerg_console, &default_console_params);
#else 
    console_api_init(&xen_console, &default_console_params);
#endif
    // FIX ME: this shouldn't be here, create os wrapper
    free_rtos_init();
}
