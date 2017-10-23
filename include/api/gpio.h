/**
 * @ brief:
 *
 * GPIO API for applications. Wrapper for GPIO chip drivers.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#ifndef _GPIO_H
#define _GPIO_H

typedef enum {
    GPIO_ERR_OK,
    GPIO_ERR_BAD
} gpio_err_t;

typedef enum {
    GPIO_MODE_IN,
    GPIO_MODE_OUT,
    GPIO_MODE_EXTINT
} gpio_mode_t;

typedef struct _gpio_driver_t {
    void (*gpio_drv_init)(int pin, gpio_mode_t mode);
    void (*gpio_drv_set_value)(int pin, int value);
    int  (*gpio_drv_get_value)(int pin);
    void (*gpio_drv_set_direction)(int pin, gpio_mode_t mode);
    void (*gpio_drv_set_pupd)(int pin, int flags);
} gpio_driver_t;

gpio_err_t gpio_api_init(gpio_driver_t *drv);
void gpio_init(int pin, gpio_mode_t mode);
void gpio_set_value(int pin, int direction);
void gpio_set_direction(int pin, gpio_mode_t mode);
int gpio_get_value(int pin)


#endif /*----------  End of header file gpio.h  ----------*/
