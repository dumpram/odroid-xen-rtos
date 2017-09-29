/**
 * @ brief:
 *
 * Console API for applications. Wrapper for console drivers.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#ifndef _CONSOLE_H
#define _CONSOLE_H

#include <types.h>

typedef enum {
    CONSOLE_ERR_OK,
    CONSOLE_ERR_BAD
} console_err_t;

typedef enum {
    CONSOLE_PARITY_ODD,
    CONSOLE_PARITY_EVEN,
    CONSOLE_PARITY_NONE
} console_parity_t;

typedef struct console_params {
    uint32_t baud;
    uint32_t data_bits;
    console_parity_t parity;
    uint32_t stop_bits;
    
    uint32_t addr;
    uint32_t clock_hz;
} console_params_t;



typedef struct _console_driver_t {
    console_err_t (*console_drv_init)(console_params_t *params);
    int (*console_drv_putc)(console_params_t *params, char c);
    int (*console_drv_block_write)(console_params_t *params, char *ptr, 
        int len);
    int (*console_drv_getc)(void); 
} console_driver_t;

console_err_t console_api_init(console_driver_t *drv, console_params_t *par);
int console_putc(char c);
int console_block_write(char *ptr, int len);
int console_getc(void);


#endif /*----------  End of header file console.h  ----------*/
