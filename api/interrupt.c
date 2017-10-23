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
#define INTERRUPT_IRQ_NUM 1024
#include <interrupt.h>
#include <utils/print.h>
#include <types.h>
#include <memory.h>
#include <time.h>
#include <stdio.h>
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
    if (!(irq_num < 0 || irq_num >= INTERRUPT_IRQ_NUM))
    {
        handler[irq_num] = hdlr;
    }
    else 
    {
        err = INTERRUPT_ERR_BAD;
    }
    return err;
}
interrupt_err_t interrupt_set_priority(int irq_num, int priority)
{
    interrupt_err_t err = INTERRUPT_ERR_OK;
    if (irq_chip == NULL || irq_chip->set_priority == NULL)
    {
        err = INTERRUPT_ERR_BAD;
    }
    if (!err)
    {
        err = irq_chip->set_priority(irq_num, priority);
    }
    return err;
}
// static uint64_t profiling[20000] = {0};
// static int i = 0;
void irq_handler(int curr_active_irq) 
{
   // uint64_t start = NOW();
    if (handler[curr_active_irq] != NULL)
    {
       handler[curr_active_irq]();
    }
    //handler[64]();
    // if (i < 20000)
    // {
    //     profiling[i++] = NOW() - start;
    // } 
    // else if (i == 20000) 
    // {
    //     uint64_t max = profiling[0];
    //     uint64_t min = profiling[0];
    //     uint64_t sum = 0;
    //     for (i = 0 ; i < 20000; i++)
    //     {
    //         sum += profiling[i];
    //         if (profiling[i] > max)
    //         {
    //             max = profiling[i];
    //         }
    //         if (profiling[i] < min)
    //         {
    //             min = profiling[i];
    //         }
    //     }
    //     printf("XEN_LAT (max - avg - min): %u - %u - %u ns\n", 
    //         (unsigned int) max, 
    //         (unsigned int) sum / 20000,
    //         (unsigned int) min);
    //     i = 0;
    // }   
}
