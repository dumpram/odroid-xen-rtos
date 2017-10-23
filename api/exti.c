/**
 * @ brief:
 *
 * EXTI peripheral API for applications.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#include <types.h>
#include <exti.h>
static exti_driver_t *exti_drv;
exti_err_t exti_api_init(exti_driver_t *drv)
{
    exti_err_t err = EXTI_ERR_OK;
    if (drv == NULL)
    {
        err = EXTI_ERR_BAD;
    }
    else
    {
        exti_drv = drv;
    }
    return err;
}
exti_err_t exti_mask_irq(int exti_line_num, int mask)
{
    exti_err_t err = EXTI_ERR_OK;
    if (exti_drv != NULL && exti_drv->exti_drv_mask_irq != NULL)
    {
        exti_drv->exti_drv_mask_irq(exti_line_num, mask);
    }
    else
    {
        err = EXTI_ERR_BAD;
    }
    return err;
}
exti_err_t exti_set_filt(int exti_line_num, int filt_mask)
{
    exti_err_t err = EXTI_ERR_OK;
    if (exti_drv != NULL && exti_drv->exti_drv_set_filt != NULL)
    {
        exti_drv->exti_drv_set_filt(exti_line_num, filt_mask);
    }
    else
    {
        err = EXTI_ERR_BAD;
    }
    return err;
}
exti_err_t exti_set_trigger(int exti_line_num, exti_mode_t mode)
{
    exti_err_t err = EXTI_ERR_OK;
    if (exti_drv != NULL && exti_drv->exti_drv_set_trigger != NULL)
    {
        exti_drv->exti_drv_set_trigger(exti_line_num, mode);
    }
    else
    {
        err = EXTI_ERR_BAD;
    }
    return err;
}
int exti_get_pend(int exti_line_num)
{
    int pend_status;
    if (exti_drv != NULL && exti_drv->exti_drv_get_pend != NULL)
    {
       pend_status = exti_drv->exti_drv_get_pend(exti_line_num);
    }
    else
    {
        pend_status = 0xdeadbeef; // debugging purposes
    }
    return pend_status;
}
exti_err_t exti_clear_pend(int exti_line_num)
{
    exti_err_t err = EXTI_ERR_OK;
    if (exti_drv != NULL && exti_drv->exti_drv_set_trigger != NULL)
    {
        exti_drv->exti_drv_clear_pend(exti_line_num);
    }
    else
    {
        err = EXTI_ERR_BAD;
    }
    return err;
}