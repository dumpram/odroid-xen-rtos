/**
 * @ brief:
 *
 * Console API and interface for console driver.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#include <console.h>


static console_driver_t *console_drv;
static console_params_t *console_par;

console_err_t console_api_init(console_driver_t *driver, console_params_t 
    *params)
{
    console_err_t err = CONSOLE_ERR_OK;

    if (driver == NULL || params == NULL) 
    {
        err = CONSOLE_ERR_BAD;
    }
    else
    {
        console_drv = driver;
        console_par = params;
    }
    if (!err && console_drv->console_drv_init != NULL)
    {
        err = console_drv->console_drv_init(console_par);
    }
    return err;
}

int console_block_write(char *ptr, int len)
{
    int i = 0;

    if (console_drv->console_drv_block_write != NULL && len > 1 && ptr != NULL)
    {
        i = console_drv->console_drv_block_write(console_par, ptr, len);
    }
    else if (len > 0 && ptr != NULL && console_drv->console_drv_putc != NULL)
    {
        for (i = 0; i < len; i++)
        {
            console_drv->console_drv_putc(console_par, ptr[i]);
        }
    }

    return i;
}

int console_putc(char c)
{
    if (console_drv->console_drv_putc != NULL)
    {
        return console_drv->console_drv_putc(console_par, c);
    }
    else
    {
        return 0;
    }
}  

int console_getc(void)
{
    return console_drv->console_drv_getc();
}
