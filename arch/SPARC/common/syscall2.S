/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/

#include <cpu/asm.h>
#include <arch/asm.h>
#include <syscall.h>

.section ".text"

/*
 *  system call
 *
 *  On entry:
 *
 *    l0 = psr (from trap table)
 *    l1 = pc
 *    l2 = npc
 *    g1 = system call id
 */

.text
.align 4

PUBLIC(disable_interrupts)
disable_interrupts:
	mov	%l2, %l1		! we came from a syscall
	add	%l2, 4, %l2		! L1 = L2 ; L2 = L2 + 4

	subcc   %g1, _NR_disable_irq, %g0		! syscall disable interrupts
	bne 	return
	or    	%l0, 0x0f00, %l4	! set PIL=15
	mov   	%l4, %psr
	nop;nop;nop
	ba,a	return

PUBLIC(enable_interrupts)
enable_interrupts:
	mov	%l2, %l1		! we came from a syscall
	add	%l2, 4, %l2		! L1 = L2 ; L2 = L2 + 4

	subcc   %g1, _NR_enable_irq, %g0		! syscall enable interrupts
	bne 	return
	andn	%l0, 0xf00, %l4
	mov   	%l4, %psr
	nop;nop;nop
	ba,a	return


PUBLIC(syscall)	
syscall:

	subcc	%g1, _NR_schedule, %g0		! schedule syscall
	be		schedule
	nop

	ba,a return

/*	schedule syscall implementation	*/
schedule:
	mov	%l2, %l1		! we came from a syscall
	add	%l2, 4, %l2		! L1 = L2 ; L2 = L2 + 4

	/* Clear the wim register so that we can move
	 * between windows without any incoveniences */

	mov %g0, %wim
	nop;nop;nop
	
	/*	Back to the valid window	*/
	restore

	/*	Store the window	*/
    std     %l0, [%sp + CPU_STACK_FRAME_L0_OFFSET]
    std     %l2, [%sp + CPU_STACK_FRAME_L2_OFFSET]
    std     %l4, [%sp + CPU_STACK_FRAME_L4_OFFSET]
    std     %l6, [%sp + CPU_STACK_FRAME_L6_OFFSET]
 
    std     %i0, [%sp + CPU_STACK_FRAME_I0_OFFSET]
    std     %i2, [%sp + CPU_STACK_FRAME_I2_OFFSET]
    std     %i4, [%sp + CPU_STACK_FRAME_I4_OFFSET]
    std     %i6, [%sp + CPU_STACK_FRAME_I6_FP_OFFSET]
								
	save

	/* Now we should set the current window as the
	 * valid one */

	mov %psr, %l5
	and %l5, 0x7, %l5
	set 0x1, %l6
	sll %l6, %l5, %l5
	not %l5

	mov %l5, %wim
	nop;nop;nop

	/*	
		ENABLE the traps and PIL = 15 (only the watchdog could interrupt us.
		This is done because the call OS_Schedule instruction could provoke
		a window overflow and the trap must be triggered
	*/
	wr	%l0, PSR_ET | PSR_PIL, %psr
	nop;nop;nop

//	call	OS_Schedule		! yes, then invoke the dispatcher
//	nop	

	/* DISABLE THE INTERRUTPS - Resore the PIL to zero vlaue */
	mov	%l0, %psr
	nop;nop;nop
	
	sethi	%hi(old_current), %l4
	ld 		[%l4 + %lo(old_current)], %o0
	
	clr %l4
	
	sethi	%hi(current), %l4
	ld 		[%l4 + %lo(current)], %o1
	
	xor %o0, %o1, %g1
	cmp %g1, 0
	bne _CPU_Context_Switch
	nop

	mov %g5, %l5
	mov %g6, %l6

	mov %g0, %wim
	nop;nop;nop

	restore

        ldd     [%sp + 0x00], %l0
        ldd     [%sp + 0x08], %l2
        ldd     [%sp + 0x10], %l4
        ldd     [%sp + 0x18], %l6
        ldd     [%sp + 0x20], %i0
        ldd     [%sp + 0x28], %i2
        ldd     [%sp + 0x30], %i4
        ldd     [%sp + 0x38], %i6

        mov %psr, %g5
        and %g5, 0x7, %g5
        set 0x1, %g6
        sll %g6, %g5, %g5
        not %g5

	save

        mov %g5, %wim
        nop;nop;nop
	
	mov %l5, %g5
	mov %l6, %g6

return:
	jmp	%l1
	rett	%l2
	
/**	AQUI IRIA EL CODIGO DEL DISPATCHER EN CASO DE PROBLEMAS	****/

/*
 ************	SYSTEM CALLS ***************
 */

/*	Sparc schedule syscall	*/ 
PUBLIC(_SYS_Schedule)
_SYS_Schedule:

	mov	_NR_schedule, %g1
	ta	0
	retl
	nop

PUBLIC(_CPU_DisableIRQ)
_CPU_DisableIRQ:
	mov	_NR_disable_irq, %g1
	ta	_NR_disable_irq
	retl
	nop

PUBLIC(_CPU_EnableIRQ)

_CPU_EnableIRQ:

	mov	_NR_enable_irq, %g1
	ta	_NR_enable_irq
	retl
	nop


