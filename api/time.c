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

// return ns
uint64_t gettimeofday()
{
    return gt_get_cntvct() * 41;
}