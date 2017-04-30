/**
 * @ brief:
 *
 * Memory API functions for memory management.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#include <types.h>

uint32_t phy_to_virt_offset;

void memory_api_init(uint32_t p2v_offset)
{
    phy_to_virt_offset = p2v_offset;
}