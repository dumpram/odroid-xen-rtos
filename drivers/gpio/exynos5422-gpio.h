/**
 * @ brief:
 *
 * Register definitions and memory mappings.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#ifndef _EXYNOS_5422_GPIO_H
#define _EXYNOS_5422_GPIO_H


#define GPA2_BASE_ADDR  ((unsigned int)  0x14010000)
#define GPIOB_BASE_ADDR ((unsigned int *)0x41400000)
#define GPIOC_BASE_ADDR ((unsigned int *)0x41400000)
#define GPIOD_BASE_ADDR ((unsigned int *)0x41400000)
#define GPIOE_BASE_ADDR ((unsigned int *)0x41400000)
#define GPIOF_BASE_ADDR ((unsigned int *)0x41400000)
#define GPIOG_BASE_ADDR ((unsigned int *)0x41400000)
#define GPIOH_BASE_ADDR ((unsigned int *)0x41400000)



#define GPIO_CTRL_OFFS 0x0040
#define GPIO_DATA_OFFS 0x0044
#define GPIO_PUPD_OFFS 0x0048


#endif /*----------  End of header file exynos5422-gpio.h  ----------*/
