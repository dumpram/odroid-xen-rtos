/**
 * @ brief:
 *
 * Exynos 5422 console driver.
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

#include "exynos5422-console_regs.h"

#define PARITY_NONE  (0)
#define PARITY_ODD   (0x4)
#define PARITY_EVEN  (0x5)
#define FORCED_CHECKED_AS_ONE (0x6)
#define FORCED_CHECKED_AS_ZERO (0x7)

#define exynos5422_read(uart, off)  readl((volatile uint32_t *)(uart + off))
#define exynos5422_write(uart, off, val) \
    writel(val, (volatile uint32_t *)(uart + off))

static console_err_t exynos5422_console_init(console_params_t *params)
{
    console_err_t err = CONSOLE_ERR_OK;

    //unsigned int divisor;
    uint32_t ulcon = 0;
    uint32_t uart = params->addr;

    /* reset, TX/RX disables */
    exynos5422_write(uart, UCON, 0);

    /* No Interrupt, auto flow control */
    exynos5422_write(uart, UMCON, 0);

    /* Baud rate specified: program it into the divisor latch. */
    //divisor = ((params->clock_hz) / (params->baud)) - 1;
    /* FIXME: will use a hacked divisor, assuming the src clock and bauds */
    exynos5422_write(uart, UFRACVAL, 53);
    exynos5422_write(uart, UBRDIV, 4);

    /*
     * Number of bits per character
     * 0 => 5 bits
     * 1 => 6 bits
     * 2 => 7 bits
     * 3 => 8 bits
     */
    if (!err && params->data_bits >= 5 && params->data_bits <= 8) 
    {
        ulcon = (params->data_bits - 5);
    }
    else
    {
        err = CONSOLE_ERR_BAD;
    }
    /*
     * Stop bits
     * 0 => 1 stop bit per frame
     * 1 => 2 stop bit per frame
     */
    if (!err && params->stop_bits >= 1 && params->stop_bits <= 2)
    {
        ulcon |= (params->stop_bits - 1) << ULCON_STOPB_SHIFT;    
    }
    else 
    {
        err = CONSOLE_ERR_BAD;
    }
    if (!err)
    {
        /* Parity */
        ulcon |= params->parity << ULCON_PARITY_SHIFT;
    }
    exynos5422_write(uart, ULCON, ulcon);

    /* Mask and clear the interrupts */
    exynos5422_write(uart, UINTM, UINTM_ALLI);
    exynos5422_write(uart, UINTP, UINTM_ALLI);

    /* reset FIFO */
    exynos5422_write(uart, UFCON, UFCON_FIFO_RESET);

    /* TODO: Add timeout to avoid infinite loop */
    while ( exynos5422_read(uart, UFCON) & UFCON_FIFO_RESET )
        ;

    /*
     * Enable FIFO and set the trigger level of Tx FIFO
     * The trigger level is always set to b101, an interrupt will be
     * generated when data count of Tx FIFO is less than or equal to the
     * following value:
     * UART0 => 160 bytes
     * UART1 => 40 bytes
     * UART2 => 10 bytes
     * UART3 => 10 bytes
     */
    exynos5422_write(uart, UFCON, UFCON_FIFO_TX_TRIGGER | UFCON_FIFO_EN);

    /*
     * Enable the UART for Rx and Tx
     *   - Use only interrupt request
     *   - Interrupts are level trigger
     *   - Enable Rx timeout
     */
    exynos5422_write(uart, UCON,
                     UCON_RX_IRQ_LEVEL | UCON_TX_IRQ_LEVEL | UCON_RX_IRQ |
                     UCON_TX_IRQ | UCON_RX_TIMEOUT);

    return err;
}

static int exynos5422_console_putc(console_params_t *params, char c)
{
   exynos5422_write(params->addr, UTXH, (uint32_t)(unsigned char)c);

   return c;
}

static int exynos5422_console_getc()
{
    return 0;
}

console_driver_t exynos5422_console_driver = {
    .console_drv_init = exynos5422_console_init,
    .console_drv_putc = exynos5422_console_putc,
    .console_drv_getc = exynos5422_console_getc,
    .console_drv_block_write = NULL
};
