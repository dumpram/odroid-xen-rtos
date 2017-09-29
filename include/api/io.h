/**
 * @ brief:
 *
 * ARM I/O macros.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */

#include <memory.h>


#define readl(c) (*c)
#define writel(v,c) (*c = v);

