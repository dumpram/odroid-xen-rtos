/**
 * @ brief:
 *
 * Retargeting.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#include <console.h>

extern int _write (int fd, char *ptr, int len)
{
    return console_block_write(ptr, len);  
}

int _read (int fd, char *ptr, int len)
{
    return len;
}

void _ttywrch(int ch) {
  /* Write one char "ch" to the default console
   * Need implementing with UART here. */
}

