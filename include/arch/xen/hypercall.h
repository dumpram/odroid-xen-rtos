#ifndef _HYPERCALL_H_
#define _HYPERCALL_H_

#define HYPERCALL_WRITE 0

int HYPERVISOR_console_io(int cmd, int count, char *str);
int HYPERVISOR_grant_table_op(int cmd, void *uop, int count);
int HYPERVISOR_memory_op(int cmd, void *uop);
int HYPERVISOR_event_channel_op(int cmd, void *uop);
int HYPERVISOR_xsm_op(void *uop);
int HYPERVISOR_hvm_op(int cmd, void *uop);


#endif /* end of include guard: _HYPERCALL_H_ */
