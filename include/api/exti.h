/**
 * @ brief:
 *
 * EXTI API for applications. Wrapper for external interrupt drivers.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#ifndef _EXTI_H
#define _EXTI_H

typedef enum {
    EXTI_ERR_OK,
    EXTI_ERR_BAD
} exti_err_t;

typedef enum {
    EXTI_MODE_TRIG_LOW,
    EXTI_MODE_TRIG_HIGH,
    EXTI_MODE_TRIG_RISING,
    EXTI_MODE_TRIG_FALLING,
    EXTI_MODE_TRIG_NONE
} exti_mode_t;

typedef struct _exti_driver_t {
    exti_err_t (*exti_drv_mask_irq)(int exti_line_num, exti_mode_t mode);
    exti_err_t (*exti_drv_set_filt)(int exti_line_num, int mask);
    int  (*exti_drv_get_pend)(int exti_line_num);
    exti_err_t (*exti_drv_set_trigger)(int exti_line_num, exti_mode_t mode);
} exti_driver_t;

exti_err_t exti_api_init(exti_driver_t *drv);
exti_err_t exti_mask_irq(int exti_line_num, int mask);
exti_err_t exti_set_filt(int exti_line_num, int filt_mask);
exti_err_t exti_set_trigger(int exti_line_num, exti_mode_t mode);
int exti_get_pend(int exti_line_num);

#endif /*----------  End of header file exti.h  ----------*/
