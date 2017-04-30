/**
 * @ brief:
 *
 * Exynos 5422 GPIO driver.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#include <gpio.h>
#include <stdint.h>

#include "exynos5422-gpio.h"

static inline uint32_t REG_READ32(volatile uint32_t *addr)
{
    uint32_t value;
    __asm__ __volatile__("ldr %0, [%1]":"=&r"(value):"r"(addr));
    __asm("dsb");
    return value;
}

static inline void REG_WRITE32(volatile uint32_t *addr, unsigned int value)
{
    __asm__ __volatile__("str %0, [%1]"::"r"(value), "r"(addr));
    __asm("dsb");
}

static void exynos5422_gpio_init(int pin, int direction)
{

}

static void exynos5422_gpio_set_direction(int pin, int direction)
{

}

static void exynos5422_gpio_set_value(int pin, int value)
{

}

static void exynos5422_gpio_set_pupd(int pin, int flags)
{

}

static int exynos5422_gpio_get_value(int pin)
{
    return 0;
}

gpio_driver_t exynos5422_gpio_driver = {
    .gpio_drv_init = exynos5422_gpio_init,
    .gpio_drv_set_direction = exynos5422_gpio_set_direction,
    .gpio_drv_set_value = exynos5422_gpio_set_value,
    .gpio_drv_set_pupd = exynos5422_gpio_set_pupd,
    .gpio_drv_get_value = exynos5422_gpio_get_value
};
