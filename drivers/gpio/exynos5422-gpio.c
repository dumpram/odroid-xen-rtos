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
#include <stdint.h>

#include <gpio.h>
#include <memory.h>

#include <utils/print.h>

#include "exynos5422-gpio.h"

typedef enum _gpio_port_t {
    GPIO_PORT_A0,
    GPIO_PORT_A1,
    GPIO_PORT_A2,
    GPIO_PORT_B0,
    GPIO_PORT_B1,
    GPIO_PORT_B2
} gpio_port_t;

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

static gpio_port_t pinnum_to_port(int pinnum)
{
    return GPIO_PORT_A2;
}

static int pinnum_to_pin(int pinnum)
{
    return 4;
}

static void exynos5422_gpio_init(int pinnum, int direction)
{
    gpio_port_t port = pinnum_to_port(pinnum);
    int pin = pinnum_to_pin(pinnum);
    volatile uint32_t *base_address;
    int direction_bit = (direction) ? 1 : 0;

    switch (port)
    {
        case GPIO_PORT_A2 : 
            base_address = p2v_translate(GPA2_BASE_ADDR); 
        break;


        default : base_address = 0x0;
    }

    // reset old configuration
    *(base_address + (GPIO_CTRL_OFFS >> 2)) &= ~(0xF << (pin * 4));

    // set direction
    *(base_address + (GPIO_CTRL_OFFS >> 2)) |= (direction_bit << (pin * 4));
}

static void exynos5422_gpio_set_direction(int pinnum, int direction)
{
    gpio_port_t port = pinnum_to_port(pinnum);
    int pin = pinnum_to_pin(pinnum);
    volatile uint32_t *base_address;
    int direction_bit = (direction) ? 1 : 0;

    switch (port)
    {
        case GPIO_PORT_A2 : 
            base_address = p2v_translate(GPA2_BASE_ADDR); 
        break;


        default : base_address = 0x0;
    }
    // clear direction
    *(base_address + (GPIO_CTRL_OFFS >> 2)) &= ~(1 << (pin * 4));
    
    // set direction
    *(base_address + (GPIO_CTRL_OFFS >> 2)) |= (direction_bit << (pin * 4));
}

static void exynos5422_gpio_set_value(int pinnum, int value)
{
    gpio_port_t port = pinnum_to_port(pinnum);
    int pin = pinnum_to_pin(pinnum);
    volatile uint32_t *base_address;
    int value_bit = (value) ? 1 : 0;

    switch (port)
    {
        case GPIO_PORT_A2 : 
            base_address = p2v_translate(GPA2_BASE_ADDR); 
        break;


        default : base_address = 0x0;
    }
    // clear value
    *(base_address + (GPIO_DATA_OFFS >> 2)) &= ~(1 << (pin));

    // set value
    *(base_address + (GPIO_DATA_OFFS >> 2)) |= (value_bit << (pin));  
}

static void exynos5422_gpio_set_pupd(int pinnum, int flags)
{

}

static int exynos5422_gpio_get_value(int pinnum)
{
    gpio_port_t port = pinnum_to_port(pinnum);
    int pin = pinnum_to_pin(pinnum);
    volatile uint32_t *base_address;

    switch (port)
    {
        case GPIO_PORT_A2 : 
            base_address = p2v_translate(GPA2_BASE_ADDR); 
        break;


        default : base_address = 0x0;
    }
    // get value
    int value = *(base_address + (GPIO_DATA_OFFS >> 2)) & (1 << (pin * 4));

    return !!value;  
}



gpio_driver_t exynos5422_gpio_driver = {
    .gpio_drv_init = exynos5422_gpio_init,
    .gpio_drv_set_direction = exynos5422_gpio_set_direction,
    .gpio_drv_set_value = exynos5422_gpio_set_value,
    .gpio_drv_set_pupd = exynos5422_gpio_set_pupd,
    .gpio_drv_get_value = exynos5422_gpio_get_value
};
