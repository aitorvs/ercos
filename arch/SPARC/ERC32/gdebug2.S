#include <autoconf.h>
#include <cpu/cpu.h>
#include <cpu/asm.h>
#include <cpu/psr.h>


    .data
    .align        4

in_trap_handler:
    .word        0

    .text
    .align 4

/*
 The function receives:
 	l0 = PSR
 	l1 = PC
 	l2 = nPC
 	l3 = WIM
*/

    .globl trap_low
trap_low:
    mov     %l2, %l1                ! we came from a synchronous trap
    add     %l2, 4, %l2             ! L1 = L2 ; L2 = L2 + 4

		
    mov        %psr, %l0
    mov        %wim, %l3 /* WIM to %l3 */

    not %l3
    and %l3, 0xff, %l3
    srl %l3, 1, %l5
    sll %l3, SPARC_NUMBER_OF_REGISTER_WINDOWS - 1, %l6
    or  %l5, %l6, %l5
    not %l3
    andn %l3, %l5, %l5
    mov %l5, %wim			! clear this window in the WIM register
    nop; nop; nop

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

    not %l3
    or %l5, %l3, %l3
    mov %l3, %wim
    nop; nop; nop


    /* Now we must check the value of in_trap_handler to
       allow recursive traps: we only use the temporal stack
       for the first time */
    
    sethi        %hi(in_trap_handler), %l4
    ld        [%lo(in_trap_handler) + %l4], %l5
    tst        %l5
    bg        recursive_trap
    inc        %l5

    set        trapstack+1000*4, %sp      /* Switch to trap stack */

recursive_trap:
    st         %l5, [%lo(in_trap_handler) + %l4]
    sub        %sp,(16+1+6+1+72)*4,%sp        /* Make room for input & locals */
                                    /* + hidden arg + arg spill */
                                    /* + doubleword alignment */
                                    /* + registers[72] local var */

    /*	
     * Store the Global, Output, Local, Input, Floating point and special
     * registers into the stack in order to pass the registers value to the
     * handle_exception rutine
     */
    std        %g0, [%sp + (24 + 0) * 4] /* registers[Gx] */
    std        %g2, [%sp + (24 + 2) * 4]
    std        %g4, [%sp + (24 + 4) * 4]
    std        %g6, [%sp + (24 + 6) * 4]

    std        %i0, [%sp + (24 + 8) * 4] /* registers[Ox] */
    std        %i2, [%sp + (24 + 10) * 4]
    std        %i4, [%sp + (24 + 12) * 4]
    std        %i6, [%sp + (24 + 14) * 4]
    
    std        %l0, [%sp + (24 + 16) * 4] /* registers[Lx] */
    std        %l2, [%sp + (24 + 18) * 4]
    std        %l4, [%sp + (24 + 20) * 4]
    std        %l6, [%sp + (24 + 22) * 4]
    
	/* I0->I7 not implemented */
	/* F0->F31 not implemented */
	
    mov        %y, %l4
    mov        %tbr, %l5
    st        %l4, [%sp + (24 + 64) * 4] ! Y
    st        %l0, [%sp + (24 + 65) * 4] ! PSR
    st        %l3, [%sp + (24 + 66) * 4] ! WIM
    st        %l5, [%sp + (24 + 67) * 4] ! TBR
    st        %l1, [%sp + (24 + 68) * 4] ! PC
    st        %l2, [%sp + (24 + 69) * 4] ! NPC

	/* CPSR and FPSR not impl */

    or        %l0, 0xf20, %l4
    mov        %l4, %psr                /* Turn on traps, disable interrupts */
    nop; nop; nop

    call       handle_exception
    add        %sp, 24 * 4, %o0        /* Pass address of registers */

/* Reload all of the registers that aren't on the stack */

    ld         [%sp + (24 + 1) * 4], %g1   /* registers[Gx] */
    ldd        [%sp + (24 + 2) * 4], %g2
    ldd        [%sp + (24 + 4) * 4], %g4
    ldd        [%sp + (24 + 6) * 4], %g6

    ldd         [%sp + (24 + 8) * 4], %i0   /* registers[Ox] */
    ldd        [%sp + (24 + 10) * 4], %i2
    ldd        [%sp + (24 + 12) * 4], %i4
    ldd        [%sp + (24 + 14) * 4], %i6

    /* After reloading, we must go back to where we started, saving the
       correct value in the WIM register */

    mov %wim, %l0
    not %l0
    and %l0, 0xff, %l0
    sll %l0, 1, %l3			! Caltulates the new invalid window WIM = WIM << 1
    srl %l0, 7, %l4		
    or %l3, %l4, %l3
    not %l3
    mov %l3, %wim
    nop; nop; nop

    
    ld [%sp + (24 + 64) * 4], %l3 ! Y
    ld [%sp + (24 + 65) * 4], %l0 ! PSR

    mov %l3, %y
    mov %l0, %psr
    nop;nop;nop
    
    sethi     	%hi(in_trap_handler), %l3
    ld        	[%lo(in_trap_handler) + %l3], %l5
    dec        	%l5
    st        	%l5, [%lo(in_trap_handler) + %l3]

    jmp %l1
    rett %l2

	.text
	.align 4
	.globl fltr_set_mem_err

    
fltr_set_mem_err:

	sethi %hi(mem_err), %l0
	st %l1, [%l0 + %lo(mem_err)]
	jmpl %l2, %g0
	rett %l2+4
	
	.text
	.align
	.globl rdtbr
rdtbr:
	save %sp, -104, %sp
	mov %tbr, %i0
	ret
	restore
   
