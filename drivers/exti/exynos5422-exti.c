/**
 * @ brief:
 *
 * Exynos 5422 EXTI driver.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#include <exti.h>
#include <types.h>
#include <memory.h>
#include <utils/print.h>

#include "exynos5422-exti.h"

static exti_err_t exynos5422_exti_mask_irq(int exti_line_num, int mask)
{
    exti_err_t err = EXTI_ERR_OK;

    volatile uint32_t *addr;
    
    mask = !!mask;

    addr = p2v_translate(EXT_INT42_BASE_ADDR + EXT_INT42_MASK_OFFS);

    // clear
    *(addr) &= ~(1 << exti_line_num); 
    // set 
    *(addr) |= (mask << exti_line_num);

    return err;
}

static exti_err_t exynos5422_exti_set_filt(int exti_line_num, int filt_mask)
{
    exti_err_t err = EXTI_ERR_OK;



    return err;
}

static exti_err_t exynos5422_exti_set_trigger(int exti_line_num, 
    exti_mode_t mode)
{
    exti_err_t err = EXTI_ERR_OK;

    int real_mask;
    volatile uint32_t *addr;

    switch(mode)
    {
        case EXTI_MODE_TRIG_LOW:
            real_mask = 0x0;
        break;
        case EXTI_MODE_TRIG_HIGH:
            real_mask = 0x1;
        break;
        case EXTI_MODE_TRIG_RISING:
            real_mask = 0x3;
        break;
        case EXTI_MODE_TRIG_FALLING:
            real_mask = 0x2;
        break;
        case EXTI_MODE_TRIG_BOTH:
            real_mask = 0x4;
        break;

        default: real_mask = 0x3;
    }

    addr = p2v_translate(EXT_INT42_BASE_ADDR + EXT_INT42_CONT_OFFS);

    (void) real_mask;
    // clear old
    *(addr) &= ~(0x7 << (4 * exti_line_num));
    // set new
    *(addr) |= (real_mask << (4 * exti_line_num));

    return err;
}

int exynos5422_exti_get_pend(int exti_line_num)
{
    int pend_status = 0;
    volatile uint32_t *addr;

    addr = p2v_translate(EXT_INT42_BASE_ADDR + EXT_INT42_PEND_OFFS);

    pend_status = *(addr) & (1 << exti_line_num);

    return !!pend_status;
}

static exti_err_t exynos5422_exti_clear_pend(int exti_line_num)
{
    exti_err_t err = EXTI_ERR_OK;
    volatile uint32_t *addr;

    addr = p2v_translate(EXT_INT42_BASE_ADDR + EXT_INT42_PEND_OFFS);

    // write 1, not 0 to position of exti_line_num
    *(addr) &= (1 << exti_line_num);

    return err;
}

exti_driver_t exynos5422_exti_driver = {
    .exti_drv_mask_irq = exynos5422_exti_mask_irq,
    .exti_drv_set_filt = exynos5422_exti_set_filt,
    .exti_drv_get_pend = exynos5422_exti_get_pend,
    .exti_drv_set_trigger = exynos5422_exti_set_trigger,
    .exti_drv_clear_pend = exynos5422_exti_clear_pend
};
