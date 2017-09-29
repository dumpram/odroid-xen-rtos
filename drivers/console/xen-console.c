/**
 * @ brief:
 *
 * Xen console drivers.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#include <console.h>
#include <types.h>
#include <memory.h>
#include <io.h>
#include <xen/hypercall.h>


static console_err_t xen_emerg_console_init(console_params_t *params)
{
    console_err_t err = CONSOLE_ERR_OK;

    return err;
}

int xen_emerg_console_block_write(console_params_t *params,
    char *ptr, int len)
{
    return HYPERVISOR_console_io(HYPERCALL_WRITE, len, ptr); 
}

int xen_emerg_console_putc(console_params_t *params, char c)
{
    return HYPERVISOR_console_io(HYPERCALL_WRITE, 1, &c); 
}

int xen_emerg_console_getc()
{
    return 0;
}

console_driver_t xen_emerg_console = {
    .console_drv_init = xen_emerg_console_init,
    .console_drv_putc = xen_emerg_console_putc,
    .console_drv_getc = xen_emerg_console_getc,
    .console_drv_block_write = xen_emerg_console_block_write
};

console_driver_t xen_console = {
    .console_drv_init = xen_emerg_console_init,
    .console_drv_putc = xen_emerg_console_putc,
    .console_drv_getc = xen_emerg_console_getc,
    .console_drv_block_write = xen_emerg_console_block_write
};
