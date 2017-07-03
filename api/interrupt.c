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

// GPIOA2.4
#define GPA2_BASE_ADDR  ((unsigned int)  0x14010000)
#define GPA2_DATA_OFFS 0x0044

#define GPX2_BASE_ADDR  ((unsigned int)  0x13400000)
#define GPX2_DATA_OFFS 0x0C44

#define ADDRG (volatile uint32_t *)( \
    p2v_translate(GPA2_BASE_ADDR + GPA2_DATA_OFFS))

#define ADDRX (volatile uint32_t *)( \
    p2v_translate(GPX2_BASE_ADDR + GPX2_DATA_OFFS)) 

static inline void gpio_toggle()
{
    *(ADDRG) ^= (1 << (1 * 4));
}

static inline int gpio_read_in()
{
    return (*(ADDRX) & (1 << (0 * 4)));
}

static inline int gpio_read_out()
{
    return (*(ADDRG) & (1 << (1 * 4)));
}

static inline void gpio_down()
{
    if (gpio_read_out())
    {
        gpio_toggle();
    }
}

static inline void gpio_up()
{
    if (!gpio_read_out())
    {
        gpio_toggle();
    }
}
void irq_handler(int curr_active_irq) 
{

    if (gpio_read_in())
    {
        gpio_up();   
    } else
    {
        gpio_down();
    }
    //gpio_toggle();
    //int i = 0;
    //while(i--);
    //gpio_toggle();

    if (handler[curr_active_irq] != NULL)
    {
        handler[curr_active_irq]();
    }
}
