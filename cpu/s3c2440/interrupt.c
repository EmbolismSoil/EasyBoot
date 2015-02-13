
#define CONFIG_USE_IRQ
#ifdef CONFIG_USE_IRQ
#include <printf.h>

/* enable IRQ interrupts */
struct pt_regs {
        long uregs[18];
};
void enable_interrupts (void)
{
	unsigned long temp;
	__asm__ __volatile__("mrs %0, cpsr\n"
			     "bic %0, %0, #0x80\n"
			     "msr cpsr_c, %0"
			     : "=r" (temp)
			     :
			     : "memory");
}


/*
 * disable IRQ/FIQ interrupts
 * returns true if interrupts had been enabled before we disabled them
 */
int disable_interrupts (void)
{
	unsigned long old,temp;
	__asm__ __volatile__("mrs %0, cpsr\n"
			     "orr %1, %0, #0xc0\n"
			     "msr cpsr_c, %1"
			     : "=r" (old), "=r" (temp)
			     :
			     : "memory");
	return (old & 0x80) == 0;
}
#else
void enable_interrupts (void)
{
	return;
}
int disable_interrupts (void)
{
	return 0;
}
#endif


void bad_mode (void)
{
   printf("error :  in bad mode!!!!\n\r");
   return;
}

void show_regs (struct pt_regs *regs)
{
    return;
}

void do_undefined_instruction (struct pt_regs *pt_regs)
{
    return;
}

void do_software_interrupt (struct pt_regs *pt_regs)
{
    return;
}

void do_prefetch_abort (struct pt_regs *pt_regs)
{
	return;
}

void do_data_abort (struct pt_regs *pt_regs)
{
        return;
}

void do_not_used (struct pt_regs *pt_regs)
{
	return;
}

void do_fiq (struct pt_regs *pt_regs)
{
	return;
}

void do_irq (struct pt_regs *pt_regs)
{
    return;
}
