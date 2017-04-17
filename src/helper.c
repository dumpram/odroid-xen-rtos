/**
 * @ brief:
 *
 * Helper functions for debugging and printing strings and numbers without 
 * libc.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#include <string.h>

static int number_to_buffer(char *p, unsigned int num);

void dump_register(char *out, char *reg_name, unsigned int value)
{
    strcpy(out, reg_name);
    //__asm("mov r6, #100");
    out = out + strlen(reg_name);
    //__asm("mov r6, #101");
    //strcpy(out, " : ");
    //__asm("mov r6, #102");
    
    *out = ':';
    out = out + 1;

    int len = number_to_buffer(out, value);
    out = out + len;
    *out = '\n';
    out++;
    out = '\0';
}

/**
 * @param p pointer to string
 * 
 * @param int point to unsigned integer
 * 
 * @return size of string representation of number
 */
static int number_to_buffer(char *p, unsigned int num)
{
    int len = 0;
    do
    {
        *(p + 1) = *p;
        *p++ = num % 10 + '0';
        num /= 10;
        len++;
    } while (num > 0);
    p[len] = 0;
    return len;
}
