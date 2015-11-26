
#include <syscall.h>
#include <autoconf.h>
#include <kernel/traps.h>
#include <kernel/task.h>

#include <M68K/cpu/cputypes.h>
#include <M68K/cpu/cpu.h>
#include <M68K/68332/platform.h>

#include "mess.h"
#include "crt0.h"

#include <lib/string.h> /* DEBUG -- memset */
#include <kernel/time.h> /* DEBUG -- OS_TimeGetTicks */
#include <kernel/debug.h> /* DEBUG -- DEBUG2 */

/* external symbols */
extern volatile uint32 schedule_active;

/* ** global variables ** */
int m68k_timer_interrupted = 0;
uint32 m68k_timer_countdown = 0;
uint32 m68k_timer_reminder = 0;
int m68k_timer_status = 0;
/* ** end of global variables ** */

/* ** internal variables ** */
static volatile long _m68k_intr_nesting = 0;
static unsigned _m68k_sup_ipl_cache = 0;
static unsigned _m68k_usr_ipl_cache = 0;
/* ** end of internal variables ** */

/* ** forward headers ** */
void m68k_sim_init (void);
void m68k_context_init (CPU_context_t * ctx, uint8 * stackbase,
		uint32 stacksize, uint32 level, void * entry);
void m68k_timer_init (void);
void m68k_timer_handler (unsigned long * ident, struct m68k_context * ctx);
void m68k_clock_init (uint32 clicks);
void m68k_serial_init (void);
void m68k_serial_tx (char ch);
void m68k_serial_rx (char * ch);
static void m68k_disable_irqs (struct m68k_context * ctx);
static void m68k_enable_irqs (struct m68k_context * ctx);
static void m68k_power_down (struct m68k_context * ctx);

void m68k_interrupt_handler2 (unsigned long nr, struct m68k_context * ctx);
void m68k_task_block (void);
void m68k_task_block_handler (struct m68k_context * ctx);
/* ** end of forward headers ** */

/** Timer status */
#define M68K_CLOCK_OFF			0x0
#define M68K_CLOCK_WITHIN		0x1
#define M68K_CLOCK_BEYOND		0x2
#define M68K_CLOCK_ON			(M68K_CLOCK_BEYOND | M68K_CLOCK_WITHIN)

#define _HACK_SIZE	(1024 * 64) // 64K @ 80k offset
#define _HACK_BASE	(char *) 0x14000

/* check that timer interrupt level and vector are coherent */
#if (M68K_TIMER_IRQL + 24 != M68K_TIMER_INTERRUPT)
#error Invalid value for M68K_TIMER_IRQL; check include/M68K/68332/platform.h
#endif

void m68k_cstart (void) {

	/* initialize system integration module */
	m68k_sim_init ();

	/* FIXME -- initialize serial port */
	memset ((void *) _HACK_BASE, 'C', _HACK_SIZE); /* DEBUG */
	m68k_serial_init ();

	/* call arch-independent initialization stuff */
	OS_Start ();
}

void m68k_sim_init (void) {
	/* Module Configuration Register:
	 * Internal clock, disable watchdog on FREEZE assertion, disable bus
	 * monitor on FREEZE assertion, IMB not available to external master,
	 * Show Cycle Enable = 0, register with access controlled only
	 * available in supervisor mode, module mapping = 1, (0xfff000),
	 * Interrupt Arbitration = 0xf */
	M68K_SIM_WRITE16 (M68K_SIM_MCR, 0x60cf);

	/* Clock Synthesize Control Register:
	 * Internal clock at 16.77 Mhz; external clock at 16.77/8 Mhz */
	M68K_SIM_WRITE16 (M68K_SIM_SYNCR, 0x7f08);

	/* System Protection Control Register has been initialized in crt0.S */
	/* M68K_SIM_WRITE8 (M68K_SIM_SYPCR, 0xc0); // disable watchdog */

	/* Chip Select Pin Assignment Register 0 and 1*/
	M68K_SIM_WRITE16 (M68K_SIM_CSPAR0, 0x3fff);
	M68K_SIM_WRITE16 (M68K_SIM_CSPAR1, 0x00fd);

	/* Chip Select Base Address Register Boot ROM */
	/* M68K_SIM_WRITE16 (M68K_SIM_CSBARBT, 0x0005); */

	/* Chip Select Option Register Boot ROM */
	/* M68K_SIM_WRITE16 (M68K_SIM_CSORBT, 0x68f0); */

	/* FIXME -- ??? */
	M68K_SIM_WRITE16 (M68K_SIM_CSBAR0, 0x2007);

	/* FIXME -- ??? */
	M68K_SIM_WRITE16 (M68K_SIM_CSOR0, 0x7830);
	/* M68K_SIM_WRITE16 (M68K_SIM_CSOR0, 0x78f0); */

	/* FIXME -- ??? */
	M68K_SIM_WRITE16 (M68K_SIM_CSBAR1, 0x3007);

	/* FIXME -- ??? */
	M68K_SIM_WRITE16 (M68K_SIM_CSOR1, 0x7830);
	/* M68K_SIM_WRITE16 (M68K_SIM_CSOR1, 0x78f0); */

	/* FIXME -- ??? */
	M68K_SIM_WRITE16 (M68K_SIM_CSBAR2, 0x4007);

	/* FIXME -- ??? */
	M68K_SIM_WRITE16 (M68K_SIM_CSOR2, 0x7830);
	/* M68K_SIM_WRITE16 (M68K_SIM_CSOR1, 0x78f0); */

	/* FIXME -- ??? */
	M68K_SIM_WRITE16 (M68K_SIM_CSBAR3, 0x5007);

	/* FIXME -- ??? */
	M68K_SIM_WRITE16 (M68K_SIM_CSOR3, 0x7830);
	/* M68K_SIM_WRITE16 (M68K_SIM_CSOR1, 0x78f0); */

	/* FIXME -- ??? */
	M68K_SIM_WRITE16 (M68K_SIM_CSBAR4, 0x0405);

	/* FIXME -- ??? */
	M68K_SIM_WRITE16 (M68K_SIM_CSOR4, 0x78f0);

	/* FIXME -- ??? */
	M68K_SIM_WRITE16 (M68K_SIM_CSBAR7, 0x1007);

	/* FIXME -- ??? */
	M68K_SIM_WRITE16 (M68K_SIM_CSOR7, 0x3830);
	/* M68K_SIM_WRITE16 (M68K_SIM_CSOR7, 0x3870); */

	/* FIXME -- ??? */
	M68K_SIM_WRITE16 (M68K_SIM_CSBAR8, 0x1007);

	/* FIXME -- ??? */
	M68K_SIM_WRITE16 (M68K_SIM_CSOR7, 0x5830);
	/* M68K_SIM_WRITE16 (M68K_SIM_CSOR7, 0x5870); */
}

void m68k_interrupt_handler2 (unsigned long ident, struct m68k_context * ctx) {
	ident &= 0x0fff;
	ident >>= 2;

	switch (ident) {
	case 32 :
		/* if this is trap 0, execute the context switch handler */
		m68k_task_block_handler (ctx);
		break;

	case 33 :
		/* if this is trap 1, enable interrupts for the current task */
		m68k_enable_irqs (ctx);
		break;

	case 34 :
		/* if this is trap 2, disable interrupts for the current task */
		m68k_disable_irqs (ctx);
		break;

	case 35 :
		/* if this is trap 3, inter into low-power stop operation */
		m68k_power_down (ctx);
		break;

	default :
		M68K_DEBUG ("*** m68k interrupt handler; ident=%p; "
			"ctx=%p; ctx->pc=%p; ctx->sr=%p; sr=%p; "
			"cnd=%d; tintr=%d", ident, ctx,
			ctx->pc, ctx->sr, m68k_get_sr (), m68k_timer_countdown,
			m68k_timer_interrupted);

		/* if this is the timer interrupt, call the timer handler */
		if (ident == M68K_TIMER_INTERRUPT) {
			m68k_timer_handler (&ident, ctx);
		}

		/* increment interrupt nesting level */
		m68k_atomic_inc (&_m68k_intr_nesting);
		/* M68K_DEBUG ("Incremented _m68k_intr_nesting to %d", _m68k_intr_nesting); */

		/* execute the architecture-independent handler */
		/* FIXME -- OS_TrapEntry have a clear a dependency with SPARC
		 * architecture */
		if (ident >= 0) OS_TrapEntry (ident, ctx->pc, ctx->pc);

		/* disable interrupts and decrement interrupt nesting level */
		m68k_sup_disable_irqs ();
		/* M68K_DEBUG ("Interrupts now disabled; sr=%p; "
			"decrementing _m68k_intr_nesting to %d",
			m68k_get_sr (), _m68k_intr_nesting - 1); */
		_m68k_intr_nesting--;

		/* are we going to dispatch a task or the context of another
		 * interrupt? In the latter case, we just return, stating that,
		 * stating that the timer interrupt handler may have been
		 * interrupted */
		if ((ctx->sr & 0x0700) != 0x0000) {
			if ((ctx->sr & 0x0700) == 0x0300) m68k_timer_interrupted = 1;
			/* M68K_DEBUG ("Another interrupt was interrupted. Au revoire!\n"); */
			break;
		}
#ifdef _M68K_DEBUG
		else {
			// M68K_DEBUG ("This is the first level interrupt");
		}
#endif

		/* At this point, we will return to an interrupted task. Was it
		 * performing a system call? In that case we cannot flush
		 * events and we will return, clearing the timer_interrupted
		 * flag */
		if (schedule_active != 0) {
			m68k_timer_interrupted = 0;
			/* M68K_DEBUG ("Scheduler is active: a system call is "
				"going on. Au revoire!\n"); */
			break;
		}
#ifdef _M68K_DEBUG
		else {
			// M68K_DEBUG ("There is not a system call going on");
		}
#endif

		/* Ok, we have to flush pending events, with interrupts enabled
		 */
		/* M68K_DEBUG ("Flushing pending events (with interrupts "
				"enabled)!"); */
		schedule_active = 1;
		m68k_sup_enable_irqs ();
		OS_SchedCurrentReady ();
		OS_IRQHandleEvent ();

		OS_Schedule ();

		/* OS_Schedule returns with interrupts disabled; we may not
		 * need to disable it */
		// m68k_sup_disable_irqs ();
		m68k_timer_interrupted = 0;

		/* if we have to dispatch the same task that was interrupted,
		 * we return from interrupt */
		if (current == old_current) {
			/* M68K_DEBUG ("Dispatching the same task (%d). Au "
				"revoire!\n", current - task_table); */
		}
		if (current == old_current) break;

		/* in the opposite case, we block the old_current task (the
		 * interrupted task) and dispatch the current task, by
		 * switching its contexts */
		m68k_task_block_handler (ctx);

		/* have a good day */
		/* M68K_DEBUG ("Dispatching task (%d). Au revoire!\n",
				current - task_table); */
		break;
	}

	return;
}

void m68k_task_block (void) {
	/* if the current task is the same as the old current task, then
	 * SYS_Schedule picked the same task to be dispatched: we shall return
	 * y aqui paz y despues gloria */
	if (current == old_current) return;

	/* Ok, we have to dispatch the task pointed by current, storing the
	 * hardware state in the old_current tcb. We invoke
	 * m68k_task_block_handler via the trap 0 (see crt0.S and
	 * m68k_interrupt_handler2) and get this task blocked in the following
	 * call. Dispatching this task will resume the execution just after the
	 * next statement */
	m68k_trap0 ();

	/* At this point, this task is now ready again: we will return as if
	 * nothing had happened */
	return;
}

void m68k_task_block_handler (struct m68k_context * ctx) {
	/* store the context of the (deliberately) interrupted task in
	 * old_current tcb */
	/* M68K_DEBUG ("Blocking task %d in favour of %d. Let's go!",
		old_current - task_table, current - task_table); */
	/* DEBUG2 ("#%d", current - task_table); */
	old_current->context = *ctx;

	/* dispatch the task pointed by current */
	/* M68K_DEBUG ("Dispatching task from current context (at slot %d)",
		current - task_table);
	M68K_TRACE (current->context.pc, "p");
	M68K_TRACE (current->context.sr, "p"); */
	*ctx = current->context;
}

void m68k_timer_init (void) {
	uint16 val;

	/* 10ms real time clock tick configuration */
	M68K_DEBUG ("Initializing timer on irq %d; vector %#x; %dus period",
		M68K_TIMER_IRQL, M68K_TIMER_INTERRUPT, M68K_TIMER_PERIOD);

	m68k_timer_countdown = M68K_TIMER_RELOAD;
	m68k_timer_status = M68K_CLOCK_OFF;

	val = (M68K_TIMER_IRQL << 8) | (M68K_TIMER_INTERRUPT & 0xff);
	M68K_SIM_WRITE16 (M68K_SIM_PICR, val);

	/* Periodic Interrupt Timer Register:
	 * For a 32.768kHz EXTAL freq. (no prescaler): Modulus / 8192 = period
	 * Some interesting values:
	 * 0x0040 -> 7812.5 us
	 * 0x0047 -> 8666.9921875 us
	 * 0x0052 -> 10009.765625 us
	 * 0x0055 -> 10375.9765625 us
	 * 0x0080 -> 15625 us
	 */
	M68K_SIM_WRITE16 (M68K_SIM_PITR, 0x0080);
}

void m68k_timer_handler (unsigned long * ident, struct m68k_context * ctx) {
	
	/* if the countdown is not zero, we only have to execute the interrupt
	 * prologue */
	if (m68k_timer_countdown != 0) {
		*ident = -1;
		M68K_DEBUG ("** Spurious timer interrupt! (m68k_timer_countdown=%d)", m68k_timer_countdown);
		return;
	}

	/* the clock has ended its countdown */
	if (m68k_timer_status & M68K_CLOCK_WITHIN) {
		/* M68K_DEBUG ("** Simulating clock interrupt"); */
		m68k_sup_disable_irqs ();
		m68k_timer_countdown = m68k_timer_reminder;
		m68k_timer_status = M68K_CLOCK_OFF;
		m68k_sup_enable_irqs ();
		*ident = M68K_CLOCK_INTERRUPT;
		return;
	}

	/* the timer has ended its countdown and the clock has to be
	 * reprogrammed */
	if (m68k_timer_status & M68K_CLOCK_BEYOND) {
		/* M68K_DEBUG ("** Timer interrupt"); */
		m68k_sup_disable_irqs ();
		m68k_timer_countdown = m68k_timer_reminder;
		m68k_timer_status = M68K_CLOCK_WITHIN;
		m68k_timer_reminder = M68K_TIMER_RELOAD - m68k_timer_reminder;
		m68k_sup_enable_irqs ();
		return;
	}

	/* timer has ended its countdown */
	/* M68K_DEBUG ("** Reloading countdown to M68K_TIMER_RELOAD = %d", M68K_TIMER_RELOAD); */
	m68k_sup_disable_irqs ();
	m68k_timer_countdown = M68K_TIMER_RELOAD;
	m68k_sup_enable_irqs ();
	return;
}

OS_Time_t m68k_get_timer (void) {
	uint32 cd;

	/* disable irqs in user mode */
	m68k_trap2 ();

	if (m68k_timer_status & M68K_CLOCK_WITHIN) {
		cd =  m68k_timer_countdown + m68k_timer_reminder;

		/* M68K_DEBUG ("-> WITHIN; reminder=%d; countdown=%d; cd = %d",
			m68k_timer_reminder, m68k_timer_countdown, cd); */

	} else if (m68k_timer_status & M68K_CLOCK_BEYOND) {
		cd = m68k_timer_countdown;
		/* M68K_DEBUG ("-> BEYOND; cd = %p", cd); */

	} else {
		cd = m68k_timer_countdown;
		/* M68K_DEBUG ("-> OFF; cd = %p", cd); */
	}

	/* enable irqs in user mode */
	m68k_trap1 ();

	/* M68K_DEBUG ("Returning %d", (M68K_TIMER_RELOAD - cd) * M68K_TIMER_PERIOD); */
	return (uint64) (M68K_TIMER_RELOAD - cd) * M68K_TIMER_PERIOD;
}

void m68k_clock_init (uint32 clicks) {
	register uint32 cd;

	/* M68K_DEBUG ("(cliks=%d); cndw=%d; remdr=%d; status=%d;",
		clicks, m68k_timer_countdown, m68k_timer_reminder,
		m68k_timer_status); */

	/* calculate the countdown required to wait for the requested clicks */
	cd = clicks / M68K_TIMER_PERIOD;
	/* M68K_DEBUG ("clicks=%d; cd=%d", clicks, cd); */
	if (cd == 0) cd = 1;

	/* disable interrupts */
	m68k_trap2 ();

	/* if timer was already programmed we have to reset it: if it was
	 * on WITHIN mode, we just add the reminder to the countdown and now
	 * the timer is reset. If it was in BEYOND mode, nothing has to be
	 * done! */
	if (m68k_timer_status == M68K_CLOCK_WITHIN) {
		m68k_timer_countdown += m68k_timer_reminder;
	}

	/* if the clock countdown is equal to the timer countdown, we have to
	 * modify the clock countdown */
	if (cd == m68k_timer_countdown) {
		if (cd != 0xffffffff) cd++;
		else cd--;
	}

	if (cd < m68k_timer_countdown) {
		/* the clock countdown will end before the next timer interrupt
		 */
		m68k_timer_reminder = m68k_timer_countdown - cd;
		m68k_timer_countdown = cd;
		m68k_timer_status = M68K_CLOCK_WITHIN;
		/* M68K_DEBUG ("status=WITHIN, countdown=%d, reminder=%d",
				m68k_timer_countdown, m68k_timer_reminder); */

	} else {
		/* the  clock countdown will end after the next timer interrupt
		 */
		m68k_timer_reminder = cd - m68k_timer_countdown;
		m68k_timer_status = M68K_CLOCK_BEYOND;
		/* M68K_DEBUG ("status=BEYOND, countdown=%d, reminder=%d",
				m68k_timer_countdown, m68k_timer_reminder); */
	}
	m68k_trap1 ();
}

static char * _m68k_serial_hack;

void m68k_serial_init (void) {
	uint16 val;
	
	/* FIXME -- For the moment I have no physical serial connector */
	_m68k_serial_hack = _HACK_BASE;
	
	/* install irq handler 
	Pr_IRQManager::Install(DataInIRQHandler,1,0x40); */

	/* baud rate: 19200 baud */
	M68K_QSM_WRITE16 (M68K_QSM_SCCR0, 27);

	/* 8 bits, no parity, REC, irq enabled */
	M68K_QSM_WRITE16 (M68K_QSM_SCCR1, 0x002c);

	/* enable transmitter and receiver; set TDRE flag */
	val = M68K_QSM_READ16 (M68K_QSM_SCSR);
	M68K_QSM_WRITE16 (M68K_QSM_SCSR, val | 0x0100);

	M68K_DEBUG ("Serial port initialized: 19200 baud, 8bit, no parity, "
		"interrupt enabled");
}

void m68k_serial_tx (char ch) {
	uint16 scsr;

	/* a circular buffer on 0x12000, 0x1200 + 512 stores send chars */
	* _m68k_serial_hack = ch;

	if (_m68k_serial_hack >= _HACK_BASE + _HACK_SIZE - 1) {
		_m68k_serial_hack = _HACK_BASE;
	} else {
		_m68k_serial_hack++;
	}

	/* FIXME -- For the moment, I have no physical serial connector :( */
	return;
	
	/* wait for TDRE bit to be set */
	do {
		scsr = M68K_QSM_READ16 (M68K_QSM_SCSR);
	} while (! (scsr & 0x0100));
	
	/* now clear TDRE bit */
	M68K_QSM_WRITE16 (M68K_QSM_SCSR, scsr & ~0x0100);

	/* write data */
	M68K_QSM_WRITE16 (M68K_QSM_SCDR, ch);
}

void m68k_serial_rx (char * ch) {
	uint16 scsr;

	/* FIXME -- For the moment, I have no physical serial connector :/ */
	*ch = 'C';
	return;

	/* wait for data: RDFR bit to be set */
	do {
		scsr = M68K_QSM_READ16 (M68K_QSM_SCSR);
	} while (! (scsr & 0x0040));

	/* read data */
	*ch = M68K_QSM_READ16 (M68K_QSM_SCDR) & 0xff;

	/* clear the RDFR bit */
	M68K_QSM_WRITE16 (M68K_QSM_SCSR, scsr & ~0x0040);

	// Old code (!!):
	// while (! *QSM_SCSR & 0x0040 ) {		// test for character recieved
	// 	*car = *QSM_SCDR & 0xFF;	// get character and place at pointer
	// 	*QSM_SCSR &= ~0x0040;		// Clear reciever flag
	// }
}

static void m68k_disable_irqs (struct m68k_context * ctx) {
	
	/* if irqs are already disabled, nothing to do */
	if ((ctx->sr & 0x0700) == 0x0700) {
		/* M68K_DEBUG ("Interrupts already disabled. Nothing to do");
		 */
		return;
	}

	/* in the opposite case, we store the current value of sr in
	 * _m68k_{usr|sup}_ipl_cache and switch to interrupt level 7 on the
	 * next return of interrupt */
	if (ctx->sr & 0x2000) {
		_m68k_sup_ipl_cache = ctx->sr & 0x0700;
		ctx->sr |= 0x0700;
		/* M68K_DEBUG ("Restoring sr to 0x%p, supervisor ipl cache is "
			"now 0x%p", ctx->sr, _m68k_sup_ipl_cache); */
	} else {
		_m68k_usr_ipl_cache = ctx->sr & 0x0700;
		ctx->sr |= 0x0700;
		/* M68K_DEBUG ("Restoring sr to 0x%p, user ipl cache is now 0x%p",
			ctx->sr, _m68k_usr_ipl_cache); */
	}
}

static void m68k_enable_irqs (struct m68k_context * ctx) {
	ctx->sr &= 0xf8ff;
	if (ctx->sr & 0x2000) {
		ctx->sr |= _m68k_sup_ipl_cache & 0x0700;
	} else {
		ctx->sr |= _m68k_usr_ipl_cache & 0x0700;
	}
	/* M68K_DEBUG ("Restoring sr to 0x%p", ctx->sr); */
}

static void m68k_power_down (struct m68k_context * ctx) {
	/*
	 * Interesting manual sections:
	 * 4.3.4 Low-Power Operation
	 * 5.8.2.1 Low-Power Stop
	 * 6.2.1.1 Low-Power Stop Operation from QSM
	 * 8.7 Low-Power Stop Operation
	 */
	while (1) m68k_lpstop ();
}

void m68k_context_init (CPU_context_t * ctx, uint8 * stackbase,
		uint32 stacksize, uint32 level, void * entry) {
    
	/* M68K_DEBUG ("Initializing hardware context: entry at 0x%p, stack at "
		"0x%p, 0x%p bytes", entry - (void *) task_table, stackbase, stacksize); */

	/* now simulate a trap from which we want to return */

	ctx->usp = (uint32) stackbase + stacksize;

	ctx->a6 = ctx->a5 = 0;
	ctx->a4 = ctx->a3 = 0;
	ctx->a2 = ctx->a1 = 0;
	ctx->a0 = 0;

	ctx->d7 = ctx->d6 = 0;
	ctx->d5 = ctx->d4 = 0;
	ctx->d3 = ctx->d2 = 0;
	ctx->d1 = ctx->d0 = 0;

	ctx->sr = 0x0000;
	ctx->pc = (uint32) entry;
}

int m68k_is_interrupted (void) {
	return _m68k_intr_nesting;
}

/* ** The Red Line: Interface with ERCOS *************************************/

void CPU_ContextInit (CPU_context_t * ctx, uint8 * stackbase, uint32 stacksize,
		uint32 level, void * entry) {
	m68k_context_init (ctx, stackbase, stacksize, level, entry);
}

void CPU_Init (void) {
	/* Noting to do */
	return;
}

void CPU_PowerDown (void) {
	m68k_trap3 ();
}

void _SYS_Schedule (void) {
	m68k_task_block ();
}

void _CPU_EnableIRQ (void) {
	m68k_trap1 ();
}

void _CPU_DisableIRQ (void) {
	m68k_trap2 ();
}

void CPU_TimerInit (void) {
	m68k_timer_init ();
}

void CPU_TimerReset (void) {
	M68K_DEBUG ("FIXME - this code may be only a stub (API not used)");
}

void CPU_ClockInit (uint32 _clicks) {
	m68k_clock_init (_clicks);
}

void CPU_ClockReset (void) {
	M68K_DEBUG ("FIXME - this code may be only a stub (API not used)");
}

void CPU_MaskInterrupt (uint32 vector) {
	/* nothing can be done here! */
}

void CPU_EnableInterrupt (uint32 vector) {
	/* noting can be done here! */
}

void CPU_MaskAllInterrupts (void) {
	m68k_trap2 ();
}

bool CPU_IsInterrupt (uint32 vector) {
	/* 24 - 31 */
	if (vector >= 24 && vector <= 31) return TRUE;

	/* 64 - 255 */
	if (vector >= 64 && vector <= 255) return TRUE;

	return FALSE;
}

void CPU_ClearInterrupt (uint32 vector) {
	/* nothing to do in this platform */
}

void CPU_ForceInterrupt (uint32 vector) {
	/* nothing can be done here! */
}

void CPU_TraceTraps (unsigned long vec, int l1, int l2) {
	/* trap is alrady traced in m68k_interrupt_handler2 */
}

void CPU_SerialTx (char ch) {
	m68k_serial_tx (ch);
}

void CPU_SerialRx (char * ch) {
	m68k_serial_rx (ch);
}

void CPU_SerialRxDebug (uint8 ch) {
	m68k_serial_tx ((char) ch);
}

