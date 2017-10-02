/**
 * @ brief:
 *
 * ARM Generic interrupt controller support.
 *
 * @ author: Ivan Pavic (@dumpram)
 * 
 * @ year: 2017
 *
 */
#include <stdint.h>
#include <xen/arch-arm.h>

#include <interrupt.h>
#include <memory.h>
#include <types.h>

// Base addresses
#define GICC_BASE 0x10482000
#define GICD_BASE 0x10481000

// Virtual timer IRQ
#define VIRTUALTIMER_IRQ 27

struct gic {
    volatile char *gicd_base;
    volatile char *gicc_base;
};

struct gic gic;

extern unsigned int p2v_offset;

// Distributor Interface
#define GICD_CTLR         0x0
#define GICD_ISENABLER    0x100
#define GICD_IPRIORITYR   0x400
#define GICD_ITARGETSR    0x800
#define GICD_ICFGR        0xC00

// CPU Interface
#define GICC_CTLR         0x0
#define GICC_PMR          0x4
#define GICC_IAR          0xc
#define GICC_BPR          0x08
#define GICC_EOIR         0x10
#define GICC_RPR          0x14
#define GICC_HPPIR        0x18

#define gicd(gic, offset) ((gic)->gicd_base + (offset))
#define gicc(gic, offset) ((gic)->gicc_base + (offset))

#define g_gicd(gic, offset) ((gic).gicd_base + (offset))
#define g_gicc(gic, offset) ((gic).gicc_base + (offset))

#define REG(addr) ((uint32_t *)(addr))

static inline uint32_t REG_READ32(volatile uint32_t *addr)
{
    uint32_t value;
    __asm__ __volatile__("ldr %0, [%1]":"=&r"(value):"r"(addr));
    __asm("dsb");
    return value;
}

static inline void REG_WRITE32(volatile uint32_t *addr, unsigned int value)
{
    __asm__ __volatile__("str %0, [%1]"::"r"(value), "r"(addr));
    __asm("dsb");
}

void gic_set_priority(struct gic *gic, int irq_number, unsigned char priority)
{
    uint32_t value;
    uint32_t *addr = REG(gicd(gic, GICD_IPRIORITYR)) + (irq_number >> 2);
    value = REG_READ32(addr);
    value &= ~(0xff << (8 * (irq_number & 0x3))); // clear old priority
    value |= priority << (8 * (irq_number & 0x3)); // set new priority
    REG_WRITE32(addr, value);
}

void gic_route_interrupt(struct gic *gic, int irq_number, unsigned char cpu_set)
{
    uint32_t value;
    uint32_t *addr = REG(gicd(gic, GICD_ITARGETSR)) + (irq_number >> 2);
    value = REG_READ32(addr);
    value &= ~(0xff << (8 * (irq_number & 0x3))); // clear old target
    value |= cpu_set << (8 * (irq_number & 0x3)); // set new target
    REG_WRITE32(addr, value);
}

/* When accessing the GIC registers, we can't use LDREX/STREX because it's not 
regular memory. */
static __inline__ void clear_bit_non_atomic(int nr, volatile void *base)
{
    volatile uint32_t *tmp = base;
    tmp[nr >> 5] &= (unsigned long)~(1 << (nr & 0x1f));
}

static __inline__ void set_bit_non_atomic(int nr, volatile void *base)
{
    volatile uint32_t *tmp = base;
    tmp[nr >> 5] |= (1 << (nr & 0x1f));
}

/* Note: not thread safe (but we only support one CPU for now anyway) */
void gic_enable_interrupt(struct gic *gic, int irq_number,
        unsigned char cpu_set, unsigned char level_sensitive)
{
    int *set_enable_reg;
    void *cfg_reg;

    // set priority
    // gic_set_priority(gic, irq_number, 0x0);

    // set target cpus for this interrupt
    gic_route_interrupt(gic, irq_number, cpu_set);

    // set level/edge triggered
    cfg_reg = (void *)gicd(gic, GICD_ICFGR);
    if (level_sensitive) {
        clear_bit_non_atomic((irq_number * 2) + 1, cfg_reg);
    } else {
        set_bit_non_atomic((irq_number * 2) + 1, cfg_reg);
    }

    __asm("dsb");

    // enable forwarding interrupt from distributor to cpu interface
    set_enable_reg = (int *)gicd(gic, GICD_ISENABLER);
    set_enable_reg[irq_number >> 5] = 1 << (irq_number & 0x1f);
    __asm("dsb");
}

void gic_enable_interrupts(struct gic *gic)
{
    // Global enable forwarding interrupts from distributor to cpu interface
    REG_WRITE32(REG(gicd(gic, GICD_CTLR)), 0x00000001);

    // Global enable signalling of interrupt from the cpu interface
    REG_WRITE32(REG(gicc(gic, GICC_CTLR)), 0x00000001);
}

void gic_disable_interrupts(struct gic *gic)
{
    // Global disable signalling of interrupt from the cpu interface
    REG_WRITE32(REG(gicc(gic, GICC_CTLR)), 0x00000000);

    // Global disable forwarding interrupts from distributor to cpu interface
    REG_WRITE32(REG(gicd(gic, GICD_CTLR)), 0x00000000);
}

void gic_cpu_set_priority(struct gic *gic, char priority)
{
    REG_WRITE32(REG(gicc(gic, GICC_PMR)), priority & 0x000000FF);
}

unsigned long gic_readiar(struct gic *gic) 
{
    return REG_READ32(REG(gicc(gic, GICC_IAR))) & 0x000003FF; // Interrupt ID
}

void gic_eoir(struct gic *gic, uint32_t irq) {
    REG_WRITE32(REG(gicc(gic, GICC_EOIR)), irq & 0x000003FF);
}

void gic_init() 
{
    gic.gicd_base = p2v_translate(GICD_BASE); //(char *)((void *)GICD_BASE - p2v_offset);
    gic.gicc_base = p2v_translate(GICC_BASE); //(char *)((void *)GICC_BASE - p2v_offset);

    gic_disable_interrupts(&gic);
    gic_cpu_set_priority(&gic, 0x0);

    gic_enable_interrupts(&gic);

    gic_set_priority(&gic, VIRTUALTIMER_IRQ, 65);

    gic_enable_interrupt(&gic, 
        VIRTUALTIMER_IRQ /* interrupt number */, 
        0x1 /*cpu_set*/, 
        1 /*level_sensitive*/);
}

uint32_t gic_pmr()
{
    return REG_READ32(REG(g_gicc(gic, GICC_IAR)));
}

uint32_t gic_bpr()
{
    return REG_READ32(REG(g_gicc(gic, GICC_IAR)));
}

uint32_t gic_rpr()
{
    return REG_READ32(REG(g_gicc(gic, GICC_IAR)));
}

// not implemented
static interrupt_err_t gic_drv_disable_irq(int irq_num, int flags)
{
    interrupt_err_t err = INTERRUPT_ERR_OK;

    return err;
}


static interrupt_err_t gic_drv_enable_irq(int irq_num, int flags)
{
    interrupt_err_t err = INTERRUPT_ERR_OK;

    gic_enable_interrupt(&gic, irq_num, 0x1, flags);

    return err;
}

static interrupt_err_t gic_drv_init(void)
{
    interrupt_err_t err = INTERRUPT_ERR_OK;

    gic_init();

    return err;
}

static interrupt_err_t gic_drv_set_priority(int irq_num, int priority)
{
    interrupt_err_t err = INTERRUPT_ERR_OK;

    gic_set_priority(&gic, irq_num, priority);

    return err;
}

irq_chip_t gic_driver = 
{
    .init = gic_drv_init,
    .disable_irq = gic_drv_disable_irq,
    .enable_irq = gic_drv_enable_irq,
    .disable_irq = gic_drv_disable_irq,
    .set_priority = gic_drv_set_priority,
    .deinit = NULL
};