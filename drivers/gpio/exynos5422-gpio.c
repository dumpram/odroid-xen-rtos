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
    GPIO_PORT_A2,
    GPIO_PORT_B2,
    GPIO_PORT_B3,
    GPIO_PORT_X1,
    GPIO_PORT_X2
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
    int port;


    // portmapping of ODROID XU3 header
    switch(pinnum)
    {
        case 0  : port = GPIO_PORT_B2; break; // onboard led
        case 4  : port = GPIO_PORT_A0; break;
        case 5  : port = GPIO_PORT_A0; break;
        case 6  : port = GPIO_PORT_A0; break;
        case 7  : port = GPIO_PORT_A2; break;
        case 8  : port = GPIO_PORT_A0; break;
        case 9  : port = GPIO_PORT_A2; break;
        case 10 : port = GPIO_PORT_A2; break;
        case 11 : port = GPIO_PORT_A2; break;
        case 13 : port = GPIO_PORT_X1; break;
        case 14 : port = GPIO_PORT_B3; break;
        case 15 : port = GPIO_PORT_X1; break;
        case 16 : port = GPIO_PORT_B3; break;
        case 17 : port = GPIO_PORT_X1; break;
        case 18 : port = GPIO_PORT_X1; break;
        case 19 : port = GPIO_PORT_X2; break;
        case 20 : port = GPIO_PORT_X2; break;
        case 21 : port = GPIO_PORT_X2; break;
        case 22 : port = GPIO_PORT_X2; break;
        case 24 : port = GPIO_PORT_X2; break;
        case 25 : port = GPIO_PORT_X1; break;
        case 26 : port = GPIO_PORT_X2; break;
        case 27 : port = GPIO_PORT_X1; break;
        
        default : port = -1; print_simple("Specified pin isn't supported!\n"); 
    }

    return (gpio_port_t) port;

}

static int pinnum_to_pin(int pinnum)
{
    int pin;

    // pinmapping of ODROID-XU3 header
    switch(pinnum)
    {
        case 0  : pin = 2; break; // onboard led
        case 4  : pin = 2; break;
        case 5  : pin = 3; break;
        case 6  : pin = 0; break;
        case 7  : pin = 7; break;
        case 8  : pin = 1; break;
        case 9  : pin = 6; break;
        case 10 : pin = 4; break;
        case 11 : pin = 5; break;
        case 13 : pin = 5; break;
        case 14 : pin = 3; break;
        case 15 : pin = 2; break;
        case 16 : pin = 2; break;
        case 17 : pin = 6; break;
        case 18 : pin = 3; break;
        case 19 : pin = 6; break;
        case 20 : pin = 4; break;
        case 21 : pin = 5; break;
        case 22 : pin = 7; break;
        case 24 : pin = 1; break;
        case 25 : pin = 7; break;
        case 26 : pin = 0; break;
        case 27 : pin = 1; break;

        default: pin = -1; print_simple("Specified pin isn't supported!\n");
    }


    return pin;
}

static void exynos5422_gpio_init(int pinnum, gpio_mode_t mode)
{
    gpio_port_t port = pinnum_to_port(pinnum);
    int pin = pinnum_to_pin(pinnum);
    volatile uint32_t *base_address;
    uint32_t ctrl_offset;
    int direction_bit;

    switch (mode)
    {
        case GPIO_MODE_IN :
            direction_bit = 0;
        break;
        case GPIO_MODE_OUT :
            direction_bit = 1;
        break;
        case GPIO_MODE_EXTINT :
            direction_bit = 0xF;
        break;
        default : 
            print_simple("Specified pin mode isn't supported!\n"); 
            return;
    }

    switch (port)
    {
        case GPIO_PORT_A2 : 
            base_address = p2v_translate(GPA2_BASE_ADDR);
            ctrl_offset = GPA2_CTRL_OFFS;
        break;

        case GPIO_PORT_B2 : 
            base_address = p2v_translate(GPB2_BASE_ADDR);
            ctrl_offset = GPB2_CTRL_OFFS;
        break;

        case GPIO_PORT_X1 :
            base_address = p2v_translate(GPX1_BASE_ADDR);
            ctrl_offset = GPX1_CTRL_OFFS;
        break;

        case GPIO_PORT_X2 :
            base_address = p2v_translate(GPX2_BASE_ADDR);
            ctrl_offset = GPX2_CTRL_OFFS;
        break;


        default : print_simple("Specified pin isn't supported!\n"); return;
    }

    // reset old configuration
    *(base_address + (ctrl_offset >> 2)) &= ~(0xF << (pin * 4));

    // set direction
    *(base_address + (ctrl_offset >> 2)) |= (direction_bit << (pin * 4));
}

static void exynos5422_gpio_set_direction(int pinnum, gpio_mode_t mode)
{
    gpio_port_t port = pinnum_to_port(pinnum);
    int pin = pinnum_to_pin(pinnum);
    volatile uint32_t *base_address;
    uint32_t ctrl_offset;
    int direction_bit;

    switch (mode)
    {
        case GPIO_MODE_IN :
            direction_bit = 0;
        break;
        case GPIO_MODE_OUT :
            direction_bit = 1;
        break;
        case GPIO_MODE_EXTINT :
            direction_bit = 0xF;
        break;
        default : 
            print_simple("Specified pin mode isn't supported!\n"); 
            return;
    }

    switch (port)
    {
        case GPIO_PORT_A2 : 
            base_address = p2v_translate(GPA2_BASE_ADDR);
            ctrl_offset = GPA2_CTRL_OFFS; 
        break;

        case GPIO_PORT_B2 : 
            base_address = p2v_translate(GPB2_BASE_ADDR);
            ctrl_offset = GPB2_CTRL_OFFS;
        break;

        case GPIO_PORT_X1 :
            base_address = p2v_translate(GPX1_BASE_ADDR);
            ctrl_offset = GPX1_CTRL_OFFS;
        break;

        case GPIO_PORT_X2 :
            base_address = p2v_translate(GPX2_BASE_ADDR);
            ctrl_offset = GPX2_CTRL_OFFS;
        break;

        default : print_simple("Specified pin isn't supported!\n"); return;
    }
    // clear direction
    *(base_address + (ctrl_offset >> 2)) &= ~(1 << (pin * 4));
    
    // set direction
    *(base_address + (ctrl_offset >> 2)) |= (direction_bit << (pin * 4));
}

static void exynos5422_gpio_set_value(int pinnum, int value)
{
    gpio_port_t port = pinnum_to_port(pinnum);
    int pin = pinnum_to_pin(pinnum);
    volatile uint32_t *base_address;
    uint32_t data_offset;
    int value_bit = (value) ? 1 : 0;

    switch (port)
    {
        case GPIO_PORT_A2 : 
            base_address = p2v_translate(GPA2_BASE_ADDR);
            data_offset = GPA2_DATA_OFFS; 
        break;

        case GPIO_PORT_B2 : 
            base_address = p2v_translate(GPB2_BASE_ADDR);
            data_offset = GPB2_DATA_OFFS;
        break;

        case GPIO_PORT_X1 :
            base_address = p2v_translate(GPX1_BASE_ADDR);
            data_offset = GPX1_DATA_OFFS;
        break;

        case GPIO_PORT_X2 :
            base_address = p2v_translate(GPX2_BASE_ADDR);
            data_offset = GPX2_DATA_OFFS;
        break;

        default : print_simple("Specified pin isn't supported!\n"); return;
    }
    // clear value
    *(base_address + (data_offset >> 2)) &= ~(1 << (pin));

    // set value
    *(base_address + (data_offset >> 2)) |= (value_bit << (pin));  
}

static void exynos5422_gpio_set_pupd(int pinnum, int flags)
{

}

static int exynos5422_gpio_get_value(int pinnum)
{
    gpio_port_t port = pinnum_to_port(pinnum);
    int pin = pinnum_to_pin(pinnum);
    volatile uint32_t *base_address;
    uint32_t data_offset;

    switch (port)
    {
        case GPIO_PORT_A2 : 
            base_address = p2v_translate(GPA2_BASE_ADDR);
            data_offset = GPA2_DATA_OFFS; 
        break;

        case GPIO_PORT_B2 : 
            base_address = p2v_translate(GPB2_BASE_ADDR);
            data_offset = GPB2_DATA_OFFS;
        break;

        case GPIO_PORT_X1 :
            base_address = p2v_translate(GPX1_BASE_ADDR);
            data_offset = GPX1_DATA_OFFS;
        break;

        case GPIO_PORT_X2 :
            base_address = p2v_translate(GPX2_BASE_ADDR);
            data_offset = GPX2_DATA_OFFS;
        break;

        default : print_simple("Specified pin isn't supported!\n"); return -1;
    }
    // get value
    int value = *(base_address + (data_offset >> 2)) & (1 << (pin));

    return !!value;  
}



gpio_driver_t exynos5422_gpio_driver = {
    .gpio_drv_init = exynos5422_gpio_init,
    .gpio_drv_set_direction = exynos5422_gpio_set_direction,
    .gpio_drv_set_value = exynos5422_gpio_set_value,
    .gpio_drv_set_pupd = exynos5422_gpio_set_pupd,
    .gpio_drv_get_value = exynos5422_gpio_get_value
};
