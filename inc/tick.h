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


inline int gt_get_cntfrq();
inline int gt_get_cntkctl();

inline int gt_get_cntp_tval();
inline int gt_get_cntp_ctl();

inline int gt_get_cntv_tval();
inline int gt_get_cntv_ctl();

inline int gt_get_cntvct();
inline int gt_get_cntp_cval();
inline int gt_get_cntv_cval();

inline int gt_get_cntvoff();


#endif