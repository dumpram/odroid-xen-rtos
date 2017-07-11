/**
 * @ brief:
 *
 * OS API for applications. Wrapper for operating system APIs.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#ifndef _OS_H
#define _OS_H

typedef enum {
    OS_ERR_OK,
    OS_ERR_BAD
} os_err_t;


os_err_t os_sched_start();
os_err_t os_thread_create();
os_err_t os_thread_destroy();
os_err_t os_thread_delay();
os_err_t os_thread_suspend();


#define synch_cmpxchg(ptr, old, new) \
({ __typeof__(*ptr) stored = old; \
   __atomic_compare_exchange_n(ptr, &stored, new, 0, __ATOMIC_SEQ_CST, \
    __ATOMIC_SEQ_CST) ? new : old; \
})

/* We probably only need "dmb" here, but we'll start by being paranoid. */
#define mb() __asm__("dsb":::"memory");
#define rmb() __asm__("dsb":::"memory");
#define wmb() __asm__("dsb":::"memory");


#endif /*----------  End of header file os.h  ----------*/
