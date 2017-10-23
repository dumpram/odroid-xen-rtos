/* 
 ****************************************************************************
 * (C) 2006 - Cambridge University
 ****************************************************************************
 *
 *        File: gnttab.c
 *      Author: Steven Smith (sos22@cam.ac.uk) 
 *     Changes: Grzegorz Milos (gm281@cam.ac.uk)
 *              
 *        Date: July 2006
 * 
 * Environment: Xen Minimal OS
 * Description: Simple grant tables implementation. About as stupid as it's
 *  possible to be and still work.
 *
 ****************************************************************************
 */
//#include <mini-os/os.h>
//#include <mini-os/mm.h>
#include <xen/gnttab.h>
//#include <xen/semaphore.h>
#include <stdio.h>
#include <string.h>
#include <xmalloc.h>
#include <memory.h>
#include <os.h>
#include <xen/hypercall.h>
#include <xen/memory.h>
#define PAGE_SIZE 4096
#define NR_RESERVED_ENTRIES 8
/* NR_GRANT_FRAMES must be less than or equal to that configured in Xen */
#define NR_GRANT_FRAMES 4
#define NR_GRANT_ENTRIES (NR_GRANT_FRAMES * PAGE_SIZE / sizeof(grant_entry_v1_t))
static grant_entry_v1_t *gnttab_table;
static grant_ref_t gnttab_list[NR_GRANT_ENTRIES];
#ifdef GNT_DEBUG
static char inuse[NR_GRANT_ENTRIES];
#endif
//static __DECLARE_SEMAPHORE_GENERIC(gnttab_sem, 0);
//
#define PAGE_SHIFT 12
typedef uint64_t paddr_t;
grant_entry_v1_t *arch_init_gnttab(int nr_grant_frames)
{
    struct xen_add_to_physmap xatp;
    struct gnttab_setup_table setup;
    xen_pfn_t frames[nr_grant_frames];
    paddr_t gnttab_table;
    int i;//, rc;
    gnttab_table = 0x80000000;//get_gnttab_base();
    for (i = 0; i < nr_grant_frames; i++)
    {
        xatp.domid = DOMID_SELF;
        xatp.size = 0;      /* Seems to be unused */
        xatp.space = XENMAPSPACE_grant_table;
        xatp.idx = i;
        xatp.gpfn = (gnttab_table >> PAGE_SHIFT) + i;
        HYPERVISOR_memory_op(XENMEM_add_to_physmap, &xatp);
        //BUG_ON(rc != 0);
    }
    setup.dom = DOMID_SELF;
    setup.nr_frames = nr_grant_frames;
    set_xen_guest_handle(setup.frame_list, frames);
    HYPERVISOR_grant_table_op(GNTTABOP_setup_table, &setup, 1);
    if (setup.status != 0)
    {
        printf("GNTTABOP_setup_table failed; status = %d\n", setup.status);
        //BUG();
    }
    return p2v_translate((uint32_t)gnttab_table);
}
static void
put_free_entry(grant_ref_t ref)
{
    //unsigned long flags;
    //local_irq_save(flags);
#ifdef GNT_DEBUG
    BUG_ON(!inuse[ref]);
    inuse[ref] = 0;
#endif
    gnttab_list[ref] = gnttab_list[0];
    gnttab_list[0]  = ref;
    //local_irq_restore(flags);
    //up(&gnttab_sem);
}
static grant_ref_t
get_free_entry(void)
{
    unsigned int ref;
    //unsigned long flags;
    //down(&gnttab_sem);
    //local_irq_save(flags);
    ref = gnttab_list[0];
#ifdef GNT_DEBUG
    BUG_ON(ref < NR_RESERVED_ENTRIES || ref >= NR_GRANT_ENTRIES);
#endif
    gnttab_list[0] = gnttab_list[ref];
#ifdef GNT_DEBUG
    BUG_ON(inuse[ref]);
    inuse[ref] = 1;
#endif
    //local_irq_restore(flags);
    return ref;
}
grant_ref_t
gnttab_grant_access(domid_t domid, unsigned long frame, int readonly)
{
    grant_ref_t ref;
    ref = get_free_entry();
    gnttab_table[ref].frame = frame;
    gnttab_table[ref].domid = domid;
    wmb();
    readonly *= GTF_readonly;
    gnttab_table[ref].flags = GTF_permit_access | readonly;
    return ref;
}
grant_ref_t
gnttab_grant_transfer(domid_t domid, unsigned long pfn)
{
    grant_ref_t ref;
    ref = get_free_entry();
    gnttab_table[ref].frame = pfn;
    gnttab_table[ref].domid = domid;
    wmb();
    gnttab_table[ref].flags = GTF_accept_transfer;
    return ref;
}
int
gnttab_end_access(grant_ref_t ref)
{
    uint16_t flags, nflags;
#ifdef GNT_DEBUG 
    BUG_ON(ref >= NR_GRANT_ENTRIES || ref < NR_RESERVED_ENTRIES);
#endif 
    nflags = gnttab_table[ref].flags;
    do {
        if ((flags = nflags) & (GTF_reading|GTF_writing)) {
            printf("WARNING: g.e. still in use! (%x)\n", flags);
            return 0;
        }
    } while ((nflags = synch_cmpxchg(&gnttab_table[ref].flags, flags, 0)) !=
            flags);
    put_free_entry(ref);
    return 1;
}
unsigned long
gnttab_end_transfer(grant_ref_t ref)
{
    unsigned long frame;
    uint16_t flags;
#ifdef GNT_DEBUG
    BUG_ON(ref >= NR_GRANT_ENTRIES || ref < NR_RESERVED_ENTRIES);
#endif
    while (!((flags = gnttab_table[ref].flags) & GTF_transfer_committed)) {
        if (synch_cmpxchg(&gnttab_table[ref].flags, flags, 0) == flags) {
            printf("Release unused transfer grant.\n");
            put_free_entry(ref);
            return 0;
        }
    }
    /* If a transfer is in progress then wait until it is completed. */
    while (!(flags & GTF_transfer_completed)) {
        flags = gnttab_table[ref].flags;
    }
    /* Read the frame number /after/ reading completion status. */
    rmb();
    frame = gnttab_table[ref].frame;
    put_free_entry(ref);
    return frame;
}
#define alloc_page() _xmalloc(4096, 4096);
#define virt_to_mfn(a) v2p_translate(a)
grant_ref_t
gnttab_alloc_and_grant(void **map)
{
    unsigned long mfn;
    grant_ref_t gref;
    *map = (void *)alloc_page();
    mfn = (unsigned long)virt_to_mfn(*map);
    gref = gnttab_grant_access(0, mfn, 0);
    return gref;
}
static const char * const gnttabop_error_msgs[] = GNTTABOP_error_msgs;
const char *
gnttabop_error(int16_t status)
{
    status = -status;
    if (status < 0)// || status >= ARRAY_SIZE(gnttabop_error_msgs))
        return "bad status";
    else
        return gnttabop_error_msgs[status];
}
void
init_gnttab(void)
{
    int i;
#ifdef GNT_DEBUG
    memset(inuse, 1, sizeof(inuse));
#endif
    for (i = NR_RESERVED_ENTRIES; i < NR_GRANT_ENTRIES; i++)
        put_free_entry(i);
    gnttab_table = arch_init_gnttab(NR_GRANT_FRAMES);
    printf("gnttab_table mapped at %p.\n", gnttab_table);
}
void
fini_gnttab(void)
{
    struct gnttab_setup_table setup;
    setup.dom = DOMID_SELF;
    setup.nr_frames = 0;
    HYPERVISOR_grant_table_op(GNTTABOP_setup_table, &setup, 1);
}
