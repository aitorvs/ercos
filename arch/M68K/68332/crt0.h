
#ifndef _CRT0_H_
#define _CRT0_H_

/** System Protection Control Register value
 * 0x00 -> disable watchdog
 * 0xc0 -> 8s watchdog period
 * 0xd0 -> 32s watchdog period
 */
#define M68K_SYPCR			0x00

#ifndef ASM

void startup (void);
void m68k_reset_entry (void);
void m68k_sup_disable_irqs (void);
void m68k_sup_enable_irqs (void);

void m68k_lpstop (void);
void m68k_trap0 (void);
void m68k_trap1 (void);
void m68k_trap2 (void);
void m68k_trap3 (void);
void m68k_atomic_inc (volatile long *i);
void m68k_atomic_dec (volatile long *i);

unsigned m68k_get_sr (void);

/* defined in the linke script */
extern unsigned CODE_START;
extern unsigned CODE_END;
extern unsigned DATA_START;
extern unsigned DATA_END;
extern unsigned bss_start;
extern unsigned end;

#endif

#endif
