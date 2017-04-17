/**
 * @ brief:
 *
 * Module contains set of assembly functions for cache maintenance. For more
 * information consult ARM Cortex-A programmer's guide. 
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */

.macro invalidate_data_cache level
    dsb
    isb
    

.endm


clear_cache:
    /*----------  Disable MMU and caching  ----------*/
    
    mrc p15, 0, r1, c1, c0, 0   // Read System Control Register (SCTLR)
    bic r1, r1, #1              // mmu off
    bic r1, r1, #(1 << 12)      // i-cache off
    bic r1, r1, #(1 << 2)       // d-cache & L2-$ off
    mcr p15, 0, r1, c1, c0, 0   // Write System Control Register (SCTLR)

    /*----------  Invalidate caches  ----------*/
    
    mov r0, #0
    mcr p15, 0, r0, c7, c5, 0   // Invalidate Instruction Cache
    mcr p15, 0, r0, c7, c5, 6   // Invalidate branch prediction array
    mcr p15, 0, r0, c8, c7, 0   // Invalidate entire Unified Main TLB
    invalidate_data_cache 1     // Invalidate L1 cache
    invalidate_data_cache 2     // Invalidate L2 cache
    isb                         // instr sync barrier

    /*----------  Enable instruction cache and branch predictor  ----------*/
            
    mrc p15, 0, r0, c1, c0, 0   // System control register (SCTLR)
    orr r0, r0, #1 << 12        // Instruction cache enable
    orr r0, r0, #1 << 11        // Program flow prediction
    mcr p15, 0, r0, c1, c0, 0   // System control register (SCTLR)
