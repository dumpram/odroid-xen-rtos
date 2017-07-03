/**
 * @ brief:
 *
 * Time API for applications.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#include <clocksource/gt.h>

uint64_t gettimeofday()
{
    return gt_get_cntvct();
}