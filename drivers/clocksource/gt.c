/**
 * @ brief:
 *
 * ARM generic timer functions. For details about registers ARMv7-A TRM.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#include <stdint.h>


// getters  
uint64_t gt_get_cntv_cval(void)
{
    unsigned int  c_lo, c_hi;
    __asm__ __volatile__("mrrc p15, 3, %0, %1, c14":"=r"(c_lo), "=r"(c_hi));
    return (((long long) c_hi) << 32) + c_lo;
}

uint64_t gt_get_cntvct(void)
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