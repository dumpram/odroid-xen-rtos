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
#include <gpio.h>
#include <types.h>
#include <utils/print.h>
static gpio_driver_t *gpio_drv;
gpio_err_t gpio_api_init(gpio_driver_t *drv)
{
    gpio_err_t err = GPIO_ERR_OK;
    if (drv == NULL)
    {
        err = GPIO_ERR_BAD;
    }
    else
    {
        gpio_drv = drv;
    }
    return err;
}
void gpio_init(int pin, gpio_mode_t mode)
{
    gpio_drv->gpio_drv_init(pin, mode);
}
void gpio_set_value(int pin, int value)
{
    gpio_drv->gpio_drv_set_value(pin, value);
}
void gpio_set_direction(int pin, gpio_mode_t mode)
{
    gpio_drv->gpio_drv_set_direction(pin, mode);
}
int gpio_get_value(int pin)
{
    return gpio_drv->gpio_drv_get_value(pin);
}
