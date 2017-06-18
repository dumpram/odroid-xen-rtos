#include <xen/hypercall.h>

void retarget_init()
{
  // Initialize UART
}

extern int _write (int fd, char *ptr, int len)
{
  return HYPERVISOR_console_io(HYPERCALL_WRITE, len, ptr);  
}

int _read (int fd, char *ptr, int len)
{
  /* Read "len" of char to "ptr" from file id "fd"
   * Return number of char read.
   * Need implementing with UART here. */
  return len;
}

void _ttywrch(int ch) {
  /* Write one char "ch" to the default console
   * Need implementing with UART here. */
}

/* SystemInit will be called before main */
void SystemInit()
{
    retarget_init();
}

