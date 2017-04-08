#include <drivers/gic.h>

#define CHIP_INTERRUPT_NUM 1024

#define INTERRUPT_NUM CHIP_INTERRUPT_NUM

typedef enum _interrupt_err_t {
	INTERRUPT_ERR_OK,
	INTERRUPT_ERR_BAD
} interrupt_err_t;

typedef struct _irq_chip_t {
	interrupt_err_t (*init)(void);
	interrupt_err_t (*disable_irq)(void);
	interrupt_err_t (*enable_irq)(void);
	interrupt_err_t (*deinit)(void);
} irq_chip_t;


static void (*handler[INTERRUPT_NUM])();

static irq_chip_t irq_chip; 


interrupt_err_t interrupt_init (void)
{
	interrupt_err_t err = INTERRUPT_ERR_OK;

	err = irq_chip.init();


	return err;
}

interrupt_err_t interrupt_disable_irq (void)
{
	interrupt_err_t err = INTERRUPT_ERR_OK;

	err = irq_chip.disable_irq();


	return err; 
}