#include <autoconf.h>


    .data
    .align        4

in_trap_handler:
    .word        0

    .text
    .align 4


#ifdef CONFIG_GDB_MONITOR

    .globl trap_low
trap_low:
    mov        %psr, %l0
    mov        %wim, %l3

    srl        %l3, %l0, %l4	/* wim >> cwp	*/
    cmp        %l4, 1
    bne        window_fine      /* Branch if not in the invalid window */
    nop

/* Handle window overflow */

    mov        %g1, %l4         /* Save g1, we use it to hold the wim */
    srl        %l3, 1, %g1      /* Rotate wim right */
    tst        %g1
    bg        good_wim          /* Branch if new wim is non-zero */
    nop

/*
 At this point, we need to bring a 1 into the high order bit of the wim.
 Since we don't want to make any assumptions about the number of register
 windows, we figure it out dynamically so as to setup the wim correctly.
 */

    not        %g1                 /* Fill g1 with ones */
    mov        %g1, %wim           /* Fill the wim with ones */
    nop
    nop
    nop
    mov        %wim, %g1           /* Read back the wim */
    inc        %g1                 /* Now g1 has 1 just to left of wim */
    srl        %g1, 1, %g1         /* Now put 1 at top of wim */
    mov        %g0, %wim           /* Clear wim so that subsequent save */
    nop                            /* won't trap */
    nop
    nop

good_wim:
    save        %g0, %g0, %g0       /* Slip into next window */
    mov        %g1, %wim            /* Install the new wim */

    std        %l0, [%sp + 0 * 4]   /* save L & I registers */
    std        %l2, [%sp + 2 * 4]
    std        %l4, [%sp + 4 * 4]
    std        %l6, [%sp + 6 * 4]

    std        %i0, [%sp + 8 * 4]
    std        %i2, [%sp + 10 * 4]
    std        %i4, [%sp + 12 * 4]
    std        %i6, [%sp + 14 * 4]

    restore                          /* Go back to trap window. */
    mov        %l4, %g1              /* Restore %g1 */

window_fine:
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

    ldd        [%sp + (24 + 64) * 4], %l0  /* Y & PSR */
    ldd        [%sp + (24 + 68) * 4], %l2   /* PC & NPC */

    restore                          /* Ensure that previous window is valid */
    save        %g0, %g0, %g0        /*  by causing a window_underflow trap */

    mov        %l0, %y
    mov        %l1, %psr             /* Make sure that traps are disabled */
                                     /* for rett */

    sethi     	%hi(in_trap_handler), %l4
    ld        	[%lo(in_trap_handler) + %l4], %l5
    dec        	%l5
    st        	%l5, [%lo(in_trap_handler) + %l4]

    jmpl        %l2, %g0              /* Restore old PC */
    rett        %l3                   /* Restore old nPC */

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
   
#endif
