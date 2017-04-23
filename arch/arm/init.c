/**
 * @ brief:
 *
 * ARM initalization routines.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#include <stdint.h>

uint32_t p2v_offset;

void arch_init(uint32_t offset)
{
    p2v_offset = offset;
}
