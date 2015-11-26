/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/

/*	***** Default TRAP handler	******	*/


#include <cpu/asm.h>
#include <arch/asm.h>

/* irq_counter	*/
    .data
    .align        4

.globl irq_counter

irq_counter:
    .word        0

    .text
    .align 4

    
/*
	This function is called when any SPARC trap (except window overflow or
 underflow) occurs.  It makes sure that the invalid register window is still
 available before jumping into C code.  It will also restore the world if you
 return from handle_exception.
 
 The function receives:
 	l0 = PSR
 	l1 = PC
 	l2 = nPC
 	l3 = trap type
 	l4 = WIM
 */

/* Generic low level trap/irq entry point	*/

.section ".text"

PUBLIC(irq_entry)
irq_entry:

    /*
     * Firstly, we must clear the wim register and save the previous window
     */

	mov %g0, %wim
	nop;nop;nop

	/*	Save the previous window	*/
	restore

	std %l0, [%sp + 0x00]
	std %l2, [%sp + 0x08]
	std %l4, [%sp + 0x10]
	std %l6, [%sp + 0x18]
	std %i0, [%sp + 0x20]
	std %i2, [%sp + 0x28]
	std %i4, [%sp + 0x30]
	std %i6, [%sp + 0x38]
	
	save
	
        mov %psr, %l5
        and %l5, 0x7, %l5
        set 0x1, %l6
        sll %l6, %l5, %l5
        not %l5

	mov %l5, %wim
	nop; nop; nop

	/* We are now in the invalid window */

    /*
     *  Save the state of the interrupted task -- especially the global
     *  registers -- in the Interrupt Stack Frame.  Note that the ISF
     *  includes a regular minimum stack frame which will be used if
     *  needed by register window overflow and underflow handlers.
     */
	sethi   %hi(kstack), %l4
	ld	[%l4 + %lo(kstack)], %l5
	sub	%l5, CONTEXT_CONTROL_INTERRUPT_FRAME_SIZE, %l5
	st %l5, [%l4 + %lo(kstack)]
	
	add	%l5, 0x0, %sp

	/* Save registers used in trap handler	*/


/*	std	%l0, [%sp + 0x00]	
	std	%l2, [%sp + 0x08]*/
	st	%g1, [%sp + 0x40]
	rd	%y, %g1
	st	%g1, [%sp + 0x44]
	std	%g2, [%sp + 0x48]
	std	%g4, [%sp + 0x50]
	std	%g6, [%sp + 0x58]
	std	%i0, [%sp + 0x60]
	std	%i2, [%sp + 0x68]
	std	%i4, [%sp + 0x70]
	std	%i6, [%sp + 0x78]

	/*	Increment the interrupt counter irq_counter	*/
	INC_VARIABLE(irq_counter,l5)    ! irq_counter++
	
	/*	
	    %l3 = interrupt level
	    %l5 = output register where the PIL is saved
	*/
	CALCULATE_PIL(l3,l5)
	
	or %l5, PSR_ET, %l5

	/* ENABLE INTERRUTPS - ET = 1	and new PIL */
	wr	%l0, %l5, %psr
	WRITE_PAUSE

	mov %l1,%o1			/* Second parameter = PC */
	mov %l2,%o2			/* Third parameter = nPC */
	call OS_TrapEntry	/* Higer level trap handler	*/
	mov %l3,%o0			/* the trap_entry function receives the trap type */


irq_exit:
	/*	In %l0 is stored the PSR with ET = 0. Disable the interrupts	*/
	mov %l0, %psr
	nop;nop;nop
	
	/*	
	   Decrement the interrupt counter irq_counter.
	   In the %l3 register is store the final irq_counter value.
	*/
	DEC_VARIABLE(irq_counter,l3)    ! irq_counter--
	
	cmp %l3, 0				! Check if irq_counter = 0
	bnz trap_entry_return	! irq_counter != 0 -> trap_entry_return
	nop

	/*	
		If we came from a Systel Call must not perform a schedule	
		if schedule_active == 1 then branch to trap_entry_return
		The macro use the %l5 and %l4 registers.
	*/
	BRANCH_IF_VARIABLE_SET(schedule_active,trap_entry_return)

	/*
		We set the schedule_active flag in order to avoid
		any scheduling action inside nested interrupts
	*/
	SET_VARIABLE(schedule_active)    ! schedule_active = 1
	
	/* ENABLE INTERRUTPS - ET = 1 PIL = 15 */
	or %l0, PSR_PIL, %l4
	wr %l4, %g0, %psr
	WRITE_PAUSE

	wr	%l4, PSR_ET, %psr
	WRITE_PAUSE
	
	/* Insert the current task in the ready queue */
	call OS_SchedCurrentReady		! add the current task to the sched queue
	nop
	
	/* Handle the pending events */
	call	OS_IRQHandleEvent
	nop
	
	/* The OS_Schedule function may turn off the traps */
	call	OS_Schedule		! yes, then invoke the dispatcher
	nop	

	/* DISABLE the interrupts - maybe it is not necessary (security) */
	wr %l0, PSR_PIL | PSR_ET, %psr
	WRITE_PAUSE

	/*	All this code is the dispatcher	*/
	clr %l4
	sethi	%hi(old_current), %l4
	ld 		[%l4 + %lo(old_current)], %o0

	clr %l4
	
	sethi	%hi(current), %l4
	ld 		[%l4 + %lo(current)], %o1

	xor %o0, %o1, %l5
	cmp %l5, 0
	bne restore_irq_context
	nop
	
trap_entry_return:

	/* Restore the psr because it stores the condition codes */
	mov %l0, %psr
	WRITE_PAUSE

	mov %g0, %wim
	nop;nop;nop

/*	ldd	[%sp + 0x00], %l0

	ldd	[%sp + 0x08], %l2*/
	ld	[%sp + 0x44], %g1
	wr	%g1, %y
	ld	[%sp + 0x40], %g1
	ldd	[%sp + 0x48], %g2
	ldd	[%sp + 0x50], %g4
	ldd	[%sp + 0x58], %g6
	ldd	[%sp + 0x60], %i0
	ldd	[%sp + 0x68], %i2
	ldd	[%sp + 0x70], %i4
	ldd	[%sp + 0x78], %i6

	restore

        ldd     [%sp + 0x00], %l0
        ldd     [%sp + 0x08], %l2
        ldd     [%sp + 0x10], %l4
        ldd     [%sp + 0x18], %l6
        ldd     [%sp + 0x20], %i0
        ldd     [%sp + 0x28], %i2
        ldd     [%sp + 0x30], %i4
        ldd     [%sp + 0x38], %i6

	save

        mov %psr, %l5
        and %l5, 0x7, %l5
	add %l5, 0x1, %l5
        set 0x1, %l6
        sll %l6, %l5, %l5
        not %l5

        mov %l5, %wim
        nop;nop;nop


    sethi   %hi(kstack), %l4
	ld	[%l4 + %lo(kstack)], %l5
	add	%l5, CONTEXT_CONTROL_INTERRUPT_FRAME_SIZE, %l5
	st %l5, [%l4 + %lo(kstack)]
	
ret_from_interrupt:	
	jmp	%l1
	rett	%l2

restore_irq_context:

	/* Restore the psr because it stores the condition codes */
	mov %l0, %psr
	WRITE_PAUSE

/*	ldd	[%sp + 0x00], %l0
	ldd	[%sp + 0x08], %l2*/
	ld	[%sp + 0x44], %g1
	wr	%g1, %y
	ld	[%sp + 0x40], %g1
	ldd	[%sp + 0x48], %g2
	ldd	[%sp + 0x50], %g4
	ldd	[%sp + 0x58], %g6
	ldd	[%sp + 0x60], %i0
	ldd	[%sp + 0x68], %i2
	ldd	[%sp + 0x70], %i4
	ldd	[%sp + 0x78], %i6

    sethi   %hi(kstack), %l4
	ld	[%l4 + %lo(kstack)], %l5
	add	%l5, CONTEXT_CONTROL_INTERRUPT_FRAME_SIZE, %l5
	st %l5, [%l4 + %lo(kstack)]

	ba _CPU_Context_Switch
	nop


/*	Bad trap handler	*/
PUBLIC(bad_trap_handler)
bad_trap_handler:
	mov %l7, %o0	/*	vec */
	mov %l1, %o1	/* 	Program counter PC  */ 
#ifdef CONFIG_DEBUG
	b,a CPU_TraceTraps
#endif
	SOFT_TRAP;
	nop

/* vim:set ts=4: */
