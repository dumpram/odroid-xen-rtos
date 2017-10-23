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
    uint32_t *L1_table = &_page_dir;
    printf("%x\n", (unsigned int)L1_table);
    for (int i = 0; i < 3512; i++)
    {
        printf("L1_table[%d] = %x\n", i, (unsigned int)L1_table[i]);
    }
}
/**
 * Set memory section attributes for write back. Devices shouldn't be 
 * cacheable.
 */
void memory_set_device(uint32_t phy_addr)
{
    uint32_t entry_template =   (0x2 + \
                                 0x8 + \
                           (3 << 10) + \
                           (0 << 12) + \
                           (1 << 16) + \
                           (1 << 19)); 
    uint32_t section_from_addr = (phy_addr >> 20) << 20;
    uint32_t entry = entry_template | section_from_addr;
    uint32_t virtual_address_of_section = phy_addr - phy_to_virt_offset;
    uint32_t page_table_address = (uint32_t) &_page_dir;
    uint32_t entry_address = page_table_address + 
        (virtual_address_of_section >> 18);
    printf("section_from_addr: %x\n", (unsigned int)section_from_addr);
    printf("entry: %x\n", (unsigned int)entry);
    printf("virtual_address_of_section: %x\n", (unsigned int)virtual_address_of_section);
    printf("page_table_address: %x\n", (unsigned int)page_table_address);
    printf("entry_address: %x\n", (unsigned int)entry_address);
    printf("phy_to_virt_offset: %x\n", (unsigned int)phy_to_virt_offset);
    *((uint32_t *)entry_address) = entry; 
    __asm("mcr p15, 0, r1, c8, c7, 0   @ Invalidate TLB");
}
