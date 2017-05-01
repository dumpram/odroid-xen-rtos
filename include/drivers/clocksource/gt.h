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
#include <types.h>

#define GT_VIRTUAL_TIMER_IRQ_NUM 27

/*----------  Common timer settings  ----------*/

uint32_t gt_get_cntfrq();
uint32_t gt_get_cntkctl();
void gt_set_cntkctl(uint32_t val);

/*----------  Physical timer  ----------*/

uint32_t gt_get_cntp_ctl();
uint32_t gt_get_cntp_tval();
void gt_set_cntp_tval(uint32_t val);
uint64_t gt_get_cntp_cval();


/*----------  Virtual timer  ----------*/

uint32_t gt_get_cntv_ctl();
void gt_set_cntv_ctl(uint32_t val);

// timer value 
uint32_t gt_get_cntv_tval();
void gt_set_cntv_tval(uint32_t val);

// compare value
uint64_t gt_get_cntv_cval();
void gt_set_cntv_cval(uint64_t val);

// timer count and offset
uint64_t gt_get_cntvct();
uint64_t gt_get_cntvoff();
