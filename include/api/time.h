/**
 * @ brief:
 *
 * Time API header. Declarations of time management functions.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#ifndef _TIME_H
#define _TIME_H

#define NOW() gettimeofday()

uint64_t gettimeofday();

#endif /*----------  End of header file time.h  ----------*/