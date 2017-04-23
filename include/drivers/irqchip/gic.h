/**
 * @ brief:
 *
 * ARM generic interrupt controller public function declarations.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#ifndef _GIC_H
#define _GIC_H

#include <stdint.h>


uint32_t gic_pmr();
uint32_t gic_bpr();
uint32_t gic_rpr();


#endif /*----------  End of header file gic.h  ----------*/
