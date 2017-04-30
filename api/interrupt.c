/**
 * @ brief:
 *
 * Interrupt API for applications.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#include <FreeRTOS.h>
#include <task.h>
#include <clocksource/gt.h>

#define GT_RATE 24000000

extern int ulPortYieldRequired;


void setup_timer_interrupt()
{
    gt_set_cntv_cval(-1); // 0xffffffffffffffff for no overflow
    gt_set_cntv_tval(GT_RATE/1000); // overflow after 1 ms
    gt_set_cntv_ctl(1); // enable timer
}

void reload_timer()
{
    gt_set_cntv_tval(GT_RATE/1000);
}

void vSetupTickInterrupt()
{
    setup_timer_interrupt();
}

void vApplicationFPUSafeIRQHandler()
{
    gt_set_cntv_ctl(2);

    if (xTaskIncrementTick() != pdFALSE)
    {
        ulPortYieldRequired = pdTRUE;
    }

    reload_timer();

    gt_set_cntv_ctl(1);
}


// 
// interrupt API
//
#define INTERRUPT_IRQ_NUM 1024

#include <interrupt.h>

static void (*handler[INTERRUPT_IRQ_NUM])();

static irq_chip_t *irq_chip; 


interrupt_err_t interrupt_api_init(irq_chip_t *chip)
{
    interrupt_err_t err = INTERRUPT_ERR_OK;

    if (chip == NULL)
    {
        err = INTERRUPT_ERR_BAD;
    }

    if (!err)
    {
        irq_chip = chip;
        if (irq_chip->init != NULL)
        {
            irq_chip->init();    
        }
    }

    return err;
}

interrupt_err_t interrupt_disable_irq(int irq_num, int flags)
{
    interrupt_err_t err = INTERRUPT_ERR_OK;

    if (irq_chip == NULL || irq_chip->disable_irq == NULL)
    {
        err = INTERRUPT_ERR_BAD;
    }

    if (!err)
    {
        err = irq_chip->disable_irq(irq_num, flags);
    }

    return err; 
}

interrupt_err_t interrupt_enable_irq(int irq_num, int flags)
{
    interrupt_err_t err = INTERRUPT_ERR_OK;

    if (irq_chip == NULL || irq_chip->disable_irq == NULL)
    {
        err = INTERRUPT_ERR_BAD;
    }

    if (!err)
    {
        err = irq_chip->enable_irq(irq_num, flags);
    }

    return err;
}

interrupt_err_t interrupt_register_handler(int irq_num, void (*hdlr)(void))
{
    interrupt_err_t err = INTERRUPT_ERR_OK;

    if (irq_num < 0 || irq_num >= INTERRUPT_IRQ_NUM)
    {
        handler[irq_num] = hdlr;
    }
    else 
    {
        err = INTERRUPT_ERR_BAD;
    }

    return err;
}

void irq_handler(void)
{
    int i;

    for (i = 0; i < INTERRUPT_IRQ_NUM; i++)
    {
        if (handler[i] != NULL)
        {
            handler[i]();
        }
    }
}
