/**
 * @ brief:
 *
 * EXTI API for applications. Wrapper for external interrupt drivers.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#ifndef _EXTI_H
#define _EXTI_H

typedef enum {
    OS_ERR_OK,
    OS_ERR_BAD
} os_err_t;


os_err_t os_sched_start();
os_err_t os_thread_create();
os_err_t os_thread_destroy();
os_err_t os_thread_delay();
os_err_t os_thread_suspend();

#endif /*----------  End of header file exti.h  ----------*/
