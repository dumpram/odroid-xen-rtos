#ifndef _HYPERCALL_H_
#define _HYPERCALL_H_

#define HYPERCALL_WRITE 0

inline int
HYPERVISOR_console_io(
	int cmd, int count, char *str);


#endif /* end of include guard: _HYPERCALL_H_ */
