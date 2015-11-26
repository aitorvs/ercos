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

/*
 *  void _CPU_Context_switch(
 *    Context_Control  *run,
 *    Context_Control  *heir
 *  )
 *
 *  This routine performs a normal non-FP context switch.
 */

.section ".text"

    .align 4
    PUBLIC(_CPU_Context_Switch)
_CPU_Context_Switch:
    ! skip g0
    st      %g1, [%o0 + G1_OFFSET]       ! save the global registers
    std     %g2, [%o0 + G2_OFFSET]
    std     %g4, [%o0 + G4_OFFSET]
    std     %g6, [%o0 + G6_OFFSET]

    std     %l0, [%o0 + L0_OFFSET]       ! save the local registers
    std     %l2, [%o0 + L2_OFFSET]
    std     %l4, [%o0 + L4_OFFSET]
    std     %l6, [%o0 + L6_OFFSET]

    std     %i0, [%o0 + I0_OFFSET]       ! save the input registers
    std     %i2, [%o0 + I2_OFFSET]
    std     %i4, [%o0 + I4_OFFSET]
    std     %i6, [%o0 + I6_FP_OFFSET]

    std     %o0, [%o0 + O0_OFFSET]       ! save the output registers
    std     %o2, [%o0 + O2_OFFSET]
    std     %o4, [%o0 + O4_OFFSET]
    std     %o6, [%o0 + O6_SP_OFFSET]

    andn %l0, PSR_PIL, %l0            ! clear the PIL field
    st   %l0, [%o0 + PSR_OFFSET]      ! save status register
    rd    %y, %g1
    st   %g1, [%o0 + Y_OFFSET]        ! save the Y register

    /*
     *  This is entered from _CPU_Context_restore with:
     *    o1 = context to restore
     *    o2 = psr
     */
/*
    PUBLIC(_CPU_Context_restore_heir)
_CPU_Context_restore_heir:*/
    /*
     *  Flush all windows with valid contents except the current one.
     *  In examining the set register windows, one may logically divide
     *  the windows into sets (some of which may be empty) based on their
     *  current status:  
     *
     *    + current (i.e. in use), 
     *    + used (i.e. a restore would not trap)
     *    + invalid (i.e. 1 in corresponding bit in WIM)
     *    + unused
     *
     *  Either the used or unused set of windows may be empty.
     *
     *  NOTE: We assume only one bit is set in the WIM at a time.
     *
     *  Given a CWP of 5 and a WIM of 0x1, the registers are divided
     *  into sets as follows:
     *
     *    + 0   - invalid
     *    + 1-4 - unused
     *    + 5   - current
     *    + 6-7 - used
     *
     *  In this case, we only would save the used windows -- 6 and 7.
     *
     *   Traps are disabled for the same logical period as in a 
     *     flush all windows trap handler.
     *   
     *    Register Usage while saving the windows:
     *      g1 = current PSR
     *      g2 = current wim
     *      g3 = CWP
     *      g4 = wim scratch
     *      g5 = scratch
     */

    ld      [%o1 + PSR_OFFSET], %g1       	! g1 = saved heir psr

    and     %l0, PSR_CWP, %g3  				! g3 = CWP
                                          	! g1 = psr w/o cwp
    andn    %g1, PSR_ET | PSR_CWP, %g1
    or      %g1, %g3, %g1                 	! g1 = heirs psr
    mov     %g1, %psr                     	! restore status register and
    nop;nop;nop                            	! **** DISABLE TRAPS ****

	/*	
	 *	This window restore is absolutely necessary to ensure the receiver
	 * window
	 */

	
    ! skip g0
        ld      [%o1 + Y_OFFSET], %g1		  ! restore the Y register
        wr		%g1, %y
    ld      [%o1 + G1_OFFSET], %g1        ! restore the global registers
    ldd     [%o1 + G2_OFFSET], %g2
    ldd     [%o1 + G4_OFFSET], %g4
    ldd     [%o1 + G6_OFFSET], %g6


    ldd     [%o1 + L0_OFFSET], %l0        ! restore the local registers
    ldd     [%o1 + L2_OFFSET], %l2
    ldd     [%o1 + L4_OFFSET], %l4
    ldd     [%o1 + L6_OFFSET], %l6

    ldd     [%o1 + I0_OFFSET], %i0        ! restore the output registers
    ldd     [%o1 + I2_OFFSET], %i2
    ldd     [%o1 + I4_OFFSET], %i4
    ldd     [%o1 + I6_FP_OFFSET], %i6

    ldd     [%o1 + O2_OFFSET], %o2        ! restore the output registers
    ldd     [%o1 + O4_OFFSET], %o4
    ldd     [%o1 + O6_SP_OFFSET], %o6

    ! do o0/o1 last to avoid destroying heir context pointer
    ldd     [%o1 + O0_OFFSET], %o0        ! overwrite heir pointer

	mov %g0, %wim
	nop;nop;nop

	restore
	
	ldd 	[%sp + 0x00], %l0
	ldd 	[%sp + 0x08], %l2
	ldd 	[%sp + 0x10], %l4
	ldd 	[%sp + 0x18], %l6
	ldd 	[%sp + 0x20], %i0
	ldd 	[%sp + 0x28], %i2
	ldd 	[%sp + 0x30], %i4
	ldd 	[%sp + 0x38], %i6

	save

	mov %psr, %l5
	add %l5, 0x1, %l5
        and %l5, 0x7, %l5
        set 0x1, %l6
        sll %l6, %l5, %l5
        not %l5

        mov %l5, %wim
        nop;nop;nop

	rd %psr, %l0
	andn %l0, PSR_PIL, %l0	!DISABLE TRAPS - PIL = 0
	wr %l0, %psr
	nop;nop;nop

	jmp %l1
	rett %l2

/*
 *  void _CPU_Context_restore(
 *    Context_Control *new_context
 *  )
 *
 *  This routine is generally used only to perform restart self.
 *
 *  NOTE: It is unnecessary to reload some registers.
 */
/*
        .align 4
        PUBLIC(_CPU_Context_Restore)
_CPU_Context_Restore:
        save    %sp, -CPU_MINIMUM_STACK_FRAME_SIZE, %sp
        rd      %psr, %o2
        ba      _CPU_Context_restore_heir
        mov     %i0, %o1                      ! in the delay slot
*/
