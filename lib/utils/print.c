/**
 * @ brief:
 *
 * Print function for debugging.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#include <string.h>
#include <xen/hypercall.h>

#define BARE_METAL 1

char debug_buffer[100];

static int number_to_buffer(char *p, unsigned int num);

void print_simple(char *buf)
{
#if BARE_METAL == 0
    HYPERVISOR_console_io(HYPERCALL_WRITE, strlen(buf), buf);   
#endif
}

void dump_register(char *out, char *reg_name, unsigned value)
{
    strcpy(out, reg_name);
    out = out + strlen(reg_name);
    *out = ':';
    out = out + 1;

    int len = number_to_buffer(out, value);
    out = out + len;
    *out = '\n';
    out++;
    out = '\0';
}

void print_register(char *reg_name, unsigned int value)
{
#if BARE_METAL == 0
    dump_register(debug_buffer, reg_name, value);  
    HYPERVISOR_console_io(HYPERCALL_WRITE, strlen(debug_buffer), debug_buffer);
    memset(debug_buffer, 0, sizeof(debug_buffer));
#endif
}

static void shift_array(char *p, int len)
{
    int i;
    for (i = len; i >= 1; i--)
    {
        p[i] = p[i - 1];       
    }
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
        *p = num % 10 + '0';
        num /= 10;
        len++;
        if (num > 0)
        {
            shift_array(p, len);
        }
    } while (num > 0);
    p[len] = 0;

    return len;
}
