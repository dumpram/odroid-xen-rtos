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

exti_err_t exynos5422_exti_mask_irq(int exti_line_num, int mask)
{
    exti_err_t err = EXTI_ERR_OK;



    return err;
}

exti_err_t exynos5422_exti_set_filt(int exti_line_num, int filt_mask)
{
    exti_err_t err = EXTI_ERR_OK;



    return err;
}

exti_err_t exynos5422_exti_set_trigger(int exti_line_num, exti_mode_t mode)
{
    exti_err_t err = EXTI_ERR_OK;


    return err;
}

int exynos5422_exti_get_pend(int exti_line_num)
{
    int pend_status = 0;


    return pend_status;
}


exti_driver_t exynos5422_exti_driver = {
    .exti_drv_mask_irq = exynos5422_exti_mask_irq,
    .exti_drv_set_filt = exynos5422_exti_set_filt,
    .exti_drv_get_pend = exynos5422_exti_get_pend,
    .exti_drv_set_trigger = exynos5422_exti_set_trigger
};
