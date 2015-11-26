/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT on ERC-32.
*/

/*
	***** Window underflow and window overflow trap handlers *****
	%l0 -> PSR register
	%l1 -> PC
	%l2 -> nPC
	%l3 -> WIM
*/

.section ".text"
    .align 4

	.globl wof_trap
    /*	window overflow low level handler	*/
wof_trap:

	mov %g1, %l5
	mov %l1, %g1
	
	/*	Back to the valid window	*/
	restore

	/*	Store the window	*/
	std %l0, [%sp + 0x00]
	std %l2, [%sp + 0x08]
	std %l4, [%sp + 0x10]
	std %l6, [%sp + 0x18]
	std %i0, [%sp + 0x20]
	std %i2, [%sp + 0x28]
	std %i4, [%sp + 0x30]
	std %i6, [%sp + 0x38]
	
	mov %g0, %wim
	nop;nop;nop

	set save_inst, %l0
	ld [%g1], %l1
	st %l1, [%l0]
	
save_inst:	nop 		! save the frame pointer in the next valid window

	mov %l5, %g1
	
	mov %l2, %o0
		
	mov %psr, %o1
	and %o1, 0x7, %o1
	set 0x1, %o2
	sll %o2, %o1, %o1
	not %o1
	
	save
	
	mov %i1, %wim
	nop;nop;nop
	

	jmp  %i0                        ! Re-execute restore.
    rett  %i0 + 4
	

	.globl wuf_trap
/*	window underflow low level handler	*/
wuf_trap:

	mov %g0, %wim
	nop;nop;nop
	
	st  %l2, [%fp]

	set restore_inst, %l4
	ld [%l1], %l5
	st %l5, [%l4]

	restore
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

restore_inst:	nop

	save

	mov %psr, %o0
	add %o0, 0x1, %o0
	and %o0, 0x7, %o0
	set 0x1, %o1
	sll %o1, %o0, %o0
	not %o0
	
	mov %o0, %wim
	nop;nop;nop

	ld [%sp], %o0
	
	// <-------------

	jmp %o0
	rett %o0 + 4
	    
