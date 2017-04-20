/**
 * @ brief:
 *
 * Module contains set of assembly functions for manipulating generic timer.
 * For more information consult ARMv7-A TRM. 
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */

.globl gt_get_cntfrq
.globl gt_get_cntkctl
.globl gt_get_cntvct
.globl gt_get_cntvoff
.globl gt_get_cntp_tval
.globl gt_get_cntp_ctl
.globl gt_get_cntv_tval
.globl gt_get_cntv_cval
.globl gt_get_cntp_cval
.globl gt_get_cntv_ctl

.globl gt_set_cntkctl
.globl gt_set_cntv_cval
.globl gt_set_cntv_ctl
.globl gt_set_cntv_tval
.globl gt_set_cntp_tval

gt_get_cntfrq:
    mrc p15, 0, r0, c14, c0, 0  // return value over r0
    mov pc, lr                  // return

gt_get_cntkctl:
    mrc p15, 0, r0, c14, c1, 0  // return value over r0
    mov pc, lr                  // return    

gt_get_cntp_tval:
    mrc p15, 0, r0, c14, c2, 0  // return value over r0
    mov pc, lr                  // return    

gt_get_cntp_ctl:
    mrc p15, 0, r0, c14, c2, 1  // return value over r0
    mov pc, lr                  // return    

gt_get_cntv_tval:
    mrc p15, 0, r0, c14, c3, 0  // return value over r0
    mov pc, lr                  // return   

gt_get_cntv_ctl:
    mrc p15, 0, r0, c14, c3, 1  // return value over r0
    mov pc, lr                  // return   



// setters
gt_set_cntkctl:

gt_set_cntv_ctl:
    mcr p15, 0, r0, c14, c3, 1  // param over r0
    mov pc, lr                  // return   

gt_set_cntv_tval:
    mcr p15, 0, r0, c14, c3, 0  // param over r0
    mov pc, lr                  // return

gt_set_cntp_tval:
    mcr p15, 0, r0, c14, c2, 0  // return value over r0
    mov pc, lr                  // return    
