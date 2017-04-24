/**
 * @ brief:
 *
 * Functions for printing to console.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#ifndef _PRINT_H
#define _PRINT_H

void print_simple(char *buf);
void print_register(char *reg_name, unsigned int value);
void dump_register(char *out, char *reg_name, unsigned value);

#endif /*----------  End of header file module.h  ----------*/
