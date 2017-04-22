/**
 * @ brief:
 *
 * Header file for simple architecture related memory management operations.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#ifndef _MM_H
#define _MM_H

extern unsigned int pv_offset;

#define to_virt(adr)          ((void *)adr - pv_offset)

#endif 