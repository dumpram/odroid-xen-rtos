/**
 * @ brief:
 *
 * Register definitions and memory mappings for EXTI peripheral of Exynos5422.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#ifndef _EXYNOS_5422_EXTI_REGS_H
#define _EXYNOS_5422_EXTI_REGS_H


#define EXT_INT_16_31_BASE_ADDR ((unsigned int)  0x13400000)

// External interrupt 42
#define EXT_INT42_BASE_ADDR ((unsigned int)  0x13400000)
#define EXT_INT42_MASK_OFFS ((unsigned int)  0xF08)
#define EXT_INT42_PEND_OFFS ((unsigned int)  0xF48)
#define EXT_INT42_CONT_OFFS ((unsigned int)  0xE08)


#endif