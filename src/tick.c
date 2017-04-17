#include <hypercall.h>
#include <string.h>

#define INTERRUPT_MESSAGE "Message from IRQ handler"

void vSetupTickInterrupt()
{
	
}

void vApplicationFPUSafeIRQHandler()
{
    HYPERVISOR_console_io(HYPERCALL_WRITE, 
        strlen(INTERRUPT_MESSAGE), INTERRUPT_MESSAGE);
}
