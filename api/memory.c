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
#include <memory.h>
#include <stdio.h>

uint32_t phy_to_virt_offset;
extern uint32_t _page_dir;

void memory_api_init(uint32_t p2v_offset)
{
    phy_to_virt_offset = p2v_offset;
}

void memory_dump_page_table()
{
    // uint32_t *L1_table = &_page_dir;

    // printf("%x\n", (unsigned int)L1_table);

    // for (int i = 0; i < 3512; i++)
    // {
    //     printf("L1_table[%d] = %x\n", i, (unsigned int)L1_table[i]);
    // }
}

/**
 * Set memory section attributes for write back. Devices shouldn't be 
 * cacheable.
 */
void memory_set_device(uint32_t phy_addr)
{
       
 
}
