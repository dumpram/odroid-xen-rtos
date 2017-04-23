#include <hypercall.h>
#include <string.h>
#include <FreeRTOS.h>
#include <task.h>
#include <tick.h>

#define INTERRUPT_MESSAGE "Message from IRQ handler\n"

extern int main_flag;
extern int ulPortYieldRequired;

void setup_timer_interrupt();
void reload_timer();

void vSetupTickInterrupt()
{
	setup_timer_interrupt();
}

void vApplicationFPUSafeIRQHandler()
{
    gt_set_cntv_ctl(2);

    // main_flag = 1;
    // HYPERVISOR_console_io(HYPERCALL_WRITE, 
    //     strlen(INTERRUPT_MESSAGE), INTERRUPT_MESSAGE);

    if (xTaskIncrementTick() != pdFALSE)
    {
        ulPortYieldRequired = pdTRUE;
    }

    reload_timer();

    gt_set_cntv_ctl(1);

}

void handler()
{
    main_flag = 1;
}


// ARM Generic Timer related functions 


// getters

long long gt_get_cntv_cval(void)
{
    unsigned int  c_lo, c_hi;
    __asm__ __volatile__("mrrc p15, 3, %0, %1, c14":"=r"(c_lo), "=r"(c_hi));
    return (((long long) c_hi) << 32) + c_lo;
}

long long gt_get_cntvct(void)
{
    unsigned int  c_lo, c_hi;
    __asm__ __volatile__("mrrc p15, 1, %0, %1, c14":"=r"(c_lo), "=r"(c_hi));
    return (((long long) c_hi) << 32) + c_lo;
}

// setters

void gt_set_cntv_cval(long long value)
{
    __asm__ __volatile__("mcrr p15, 3, %0, %H0, c14"
        ::"r"(value));
}

void gt_set_cntvct(long long value)
{
    __asm__ __volatile__("mcrr p15, 1, %0, %H0, c14"
    ::"r"(value));
}
