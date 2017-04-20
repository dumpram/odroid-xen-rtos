#include <tick.h>
#include <string.h>
#include <hypercall.h>

static char debug_buffer[100];

void dump_register(char *out, char *reg_name, unsigned int value);

void print_register(char *reg_name, unsigned int value)
{
    dump_register(debug_buffer, reg_name, value);
    if (!strncmp(debug_buffer, "cntkctl", 7))
    {
        __asm("mov r6, #102");
    }   
    HYPERVISOR_console_io(HYPERCALL_WRITE, strlen(debug_buffer), debug_buffer);
}

int main()
{
    print_register("cntfrq", gt_get_cntfrq());
    print_register("cntkctl", gt_get_cntkctl());
    print_register("cntp_tval", gt_get_cntp_tval());
    print_register("cntp_ctl", gt_get_cntp_ctl());
    print_register("cntv_tval", gt_get_cntv_tval());
    print_register("cntv_ctl", gt_get_cntv_ctl());

    while (1);
}
