#include <tick.h>
#include <string.h>
#include <hypercall.h>

void gic_init(void);

static char debug_buffer[100];

void dump_register(char *out, char *reg_name, unsigned value);

void print_register(char *reg_name, unsigned int value)
{
    dump_register(debug_buffer, reg_name, value);  
    HYPERVISOR_console_io(HYPERCALL_WRITE, strlen(debug_buffer), debug_buffer);
    memset(debug_buffer, 0, sizeof(debug_buffer));
}

void print_simple(char *buf)
{
    HYPERVISOR_console_io(HYPERCALL_WRITE, strlen(buf), buf);   
    memset(debug_buffer, 0, sizeof(debug_buffer));
}

static inline long long read_virtual_count(void)
{
    unsigned int  c_lo, c_hi;
    __asm__ __volatile__("mrrc p15, 3, %0, %1, c14":"=r"(c_lo), "=r"(c_hi));
    return (((long long) c_hi) << 32) + c_lo;
}

void test_gt_timer()
{
    print_simple("Testing generic timer!\n");

    int value = 0;

    gt_set_cntv_ctl(2);

    value = gt_get_cntv_ctl();

    
    if (value == 2)
    {
        print_simple(
            "Write and read virtual timer control register successful!\n");
        print_register("cntv_ctl", gt_get_cntv_ctl());
    }
    else 
    {
        print_simple("Error... setting register!\n");
        print_register("cntv_ctl", gt_get_cntv_ctl());
        print_register("value", value);
    }

    long long cvalue = 0;

    gt_set_cntv_cval(2400);

    cvalue = gt_get_cntv_cval();

    if (cvalue == (long long)2400)
    {
        print_simple(
            "Write and read virual compare value register successful!\n");
    }
    else 
    {
        print_simple("Error... setting register!\n");
        print_register("cntv_cval", read_virtual_count());
        print_register("cvalue", cvalue);
    }


    print_simple("Test ended!\n");
}

void test_1second_timer()
{
    int ctl_val;
    print_simple("Testing 1 second timer!\n");
    //gt_set_cntvct(0);
    print_simple("Setting cntvct to 0!\n");
    gt_set_cntv_cval(-1);
    gt_set_cntv_tval(24000000);
    print_simple("Setting tval to 24000000!\n");
    gt_set_cntv_ctl(1);
    print_simple("Enabling timer!\n");

    long long loop_cnt = 0;
    int sec = 0;

    while(1)
    {
        ctl_val = gt_get_cntv_ctl();

        print_register("ctl_val", ctl_val);
        while (!(ctl_val & 0x4)) {
            ctl_val = gt_get_cntv_ctl();
            loop_cnt++;
        }
        sec++;
        print_register("Seconds", sec);
        print_register("loop_cnt", loop_cnt);
        loop_cnt = 0;
        gt_set_cntv_tval(24000000);        
    }
}

void print_gt_registers()
{
    print_register("cntfrq", gt_get_cntfrq());
    print_register("cntkctl", gt_get_cntkctl());
    print_register("cntp_tval", gt_get_cntp_tval());
    print_register("cntp_ctl", gt_get_cntp_ctl());
    print_register("cntv_tval", gt_get_cntv_tval());
    print_register("cntv_ctl", gt_get_cntv_ctl());
    print_register("cntvct", gt_get_cntvct());
}

void setup_timer_interrupt()
{
    gt_set_cntv_cval(-1); // 0xffffffffffffffff in cval register so it will 
                          // never give interrupt
    gt_set_cntv_tval(GT_RATE); // overflow after 1 second

    print_simple("Initializing GIC!\n");
    gic_init(); // initialise GIC and enable virtual interrupt 

    print_simple("Enabling timer!\n");
    gt_set_cntv_ctl(1); // enable timer

}

int flag = 0;

int pv_offset;

int main(int argc, char **args)
{

    pv_offset = argc;

    print_register("pv_offset:", pv_offset);

    print_gt_registers();
    //test_gt_timer();
    //test_1second_timer();

    setup_timer_interrupt();

    while (!flag);

    if (flag)
    {
        print_simple("Congrats! Interrupt occured!\n");
    }

    while (1);
}
