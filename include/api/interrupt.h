/**
 * @ brief:
 *
 * Interrupt API declarations and data types.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#ifndef _INTERRUPT_H
#define _INTERRUPT_H

typedef enum _interrupt_err_t {
    INTERRUPT_ERR_OK,
    INTERRUPT_ERR_BAD
} interrupt_err_t;

typedef struct _irq_chip_t {
    interrupt_err_t (*init)(void);
    interrupt_err_t (*disable_irq)(int irq_num, int params);
    interrupt_err_t (*enable_irq)(int irq_num, int params);
    interrupt_err_t (*deinit)(void);
} irq_chip_t;


interrupt_err_t interrupt_api_init(irq_chip_t *chip);
interrupt_err_t interrupt_disable_irq(int irq_num, int flags);
interrupt_err_t interrupt_enable_irq(int irq_num, int flags);
interrupt_err_t interrupt_register_handler(int irq_num, void (*hdlr)(void));




#endif /*----------  End of header file interrupt.h  ----------*/
