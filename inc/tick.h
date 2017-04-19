/**
 * @ brief:
 *
 * Header file contains declarations of manipulation functions for getting 
 * ARM generic timer registers.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#ifndef _TICK_H
#define _TICK_H


int gt_get_cntfrq();
int gt_get_cntkctl();

int gt_get_cntp_tval();
int gt_get_cntp_ctl();

int gt_get_cntv_tval();
int gt_get_cntv_ctl();

int gt_get_cntvct();
int gt_get_cntp_cval();
int gt_get_cntv_cval();

int gt_get_cntvoff();


#endif