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
