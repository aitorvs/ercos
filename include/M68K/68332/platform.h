/*
    Copyright 2004-2005 (c) by Óscar García Población,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT on ERC-32.
*/

/**
 * $Revision$ $Date$
 * Last CVS commit by $Author$
 * $Log$
 */

/**
 * This file is MANDATORY. From this file are included all other required
 * files 
 */

#ifndef _M68K_68332_PLATFORM_H_
#define _M68K_68332_PLATFORM_H_

#include <M68K/cpu/cpu.h>

/** Timer interrupt period, in microseconds: 15,625ms */
#define M68K_TIMER_PERIOD		15625

/** Timer countdown reload value (see timer stuff in mess.c) */
#define M68K_TIMER_RELOAD		0xffffffff

/** Timer interrupt vector */
#define M68K_TIMER_INTERRUPT		0x1b

/** Timer interrupt level */
#define M68K_TIMER_IRQL			3

/** Clock (pseudo-)interrupt vector */
#define M68K_CLOCK_INTERRUPT		0x40

/** System Protection Control Register value (see arch/M68K/68332/crt0.h) */

/** Timer interrupt vector number */
#define CPU_TIMER_TRAP			M68K_TIMER_INTERRUPT

/** Real time clock interrupt vector number. Clock timer is emulated with the
 * help of the timer interrupt and some internal variables in the architecture
 * dependent code */
#define CPU_CLOCK_TRAP			M68K_CLOCK_INTERRUPT

/**
 * This is the number of interrupts defined by the architecture: 256
 */
#define CPU_NUMBER_OF_INTERRUPTS	M68K_INTERRUPT_NR

/**
 * Number of the lowest and highest interrupt vector
 */
#define CPU_TRAP_MIN_NUMBER		2
#define CPU_TRAP_MAX_NUMBER		255

/**
 * Reload value for the general purpose timer. This is the maximum amount of
 * ticks before reaching a timing overflow for the general purpose timer, buut
 * supposing that each tick happens in 1 micro-second. As our real ticks occurs
 * in 10 ms (see M68K_TIMER_PERIOD), this is the amount of microseconds that we
 * have between timer interrupts */
//#define CPU_TIMER_COUNTER		(M68K_TIMER_RELOAD * M68K_TIMER_PERIOD - 1)
#define CPU_TIMER_COUNTER		(M68K_TIMER_RELOAD * M68K_TIMER_PERIOD)

/* This macro serves to the architecture-independent code to know if a system
 * call is performed while in an interrupt or not. In fact, it is only used in
 * posix/semaphore.c:sem_post to append an event to the interrupt event queue
 * instead of calling SYS_SemPost */
#define CPU_IS_IN_TRAP_HANDLER()	(m68k_is_interrupted ())

void CPU_ContextInit (CPU_context_t * ctx, uint8 * stackbase, uint32 stacksize,
		uint32 level, void * entry);
void CPU_Init (void);
void CPU_PowerDown (void);

void _CPU_EnableIRQ (void);
void _CPU_DisableIRQ (void);

void CPU_TimerInit (void);
void CPU_TimerReset (void);
void CPU_ClockInit (uint32 _clicks);
void CPU_ClockReset (void);

void CPU_MaskInterrupt (uint32 vector);
void CPU_MaskAllInterrupts (void);
bool CPU_IsInterrupt (uint32 vector);
void CPU_ClearInterrupt (uint32 vector);
void CPU_EnableInterrupt (uint32 vector);
void CPU_ForceInterrupt (uint32 vector);
void CPU_TraceTraps (unsigned long vec, int l1, int l2);
void CPU_SerialTx (char ch);
void CPU_SerialRx (char * ch);
void CPU_SerialRxDebug (uint8 ch);

int m68k_is_interrupted (void);
OS_Time_t m68k_get_timer (void);

void m68k_addrerr (void);

#endif

