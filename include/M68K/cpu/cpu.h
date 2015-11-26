
#ifndef _M68K_CPU_CPU_H_
#define _M68K_CPU_CPU_H_

#include <M68K/cpu/cputypes.h>
#include <sys/types.h>

/** Number of architecture defined interrupts: 256 */
#define M68K_INTERRUPT_NR		0x100

extern OS_Time_t m68k_global_time;

#define CPU_GetTimer(counter)		(counter = m68k_get_timer ())

void _SYS_Schedule (void);

#endif
