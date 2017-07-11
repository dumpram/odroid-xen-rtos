#ifndef _HYPERCALL_H_
#define _HYPERCALL_H_

#define HYPERCALL_WRITE 0

int HYPERVISOR_console_io(int cmd, int count, char *str);
int HYPERVISOR_grant_table_op();
int HYPERVISOR_memory_op();


#endif /* end of include guard: _HYPERCALL_H_ */
