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
#define GPX1_BASE_ADDR  ((unsigned int)  0x13400000)
#define GPX2_BASE_ADDR  ((unsigned int)  0x13400000)

#define GPA2_CTRL_OFFS 0x0040
#define GPA2_DATA_OFFS 0x0044
#define GPA2_PUPD_OFFS 0x0048

#define GPX1_CTRL_OFFS 0x0C20
#define GPX1_DATA_OFFS 0x0C24
#define GPX1_PUPD_OFFS 0x0C28

#define GPX2_CTRL_OFFS 0x0C40
#define GPX2_DATA_OFFS 0x0C44
#define GPX2_PUPD_OFFS 0x0C48




#endif /*----------  End of header file exynos5422-gpio.h  ----------*/
