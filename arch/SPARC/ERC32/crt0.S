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


#define PSR_INIT    0x000010c0	/* CPU traps off, set s and ps, and FP unit  */
#define MEM_INIT    0x00081000	/* MEC config (4Mb RAM / 512Kb ROM) + EDAC */
#define WST_INIT    0x33330035	/* MEC waitstate configuration */
#define CTR_INIT    0x1988555f	/* MEC initial control register, 19200bps */
#ifdef CONFIG_FAST_MODE
#define WIM_INIT    0x00000002	/* Initial invalid window value	*/
#else
#define WIM_INIT    0x000000FE	/* Initial invalid window value	*/
#endif
#define SPP_INIT    0x02400000	/* Initial stack pointer	*/

//.section ".text"
.section ".romvec"
    .align 4
    .globl startup

/*	Reset entry point	*/
startup: 
trap_table:

    wr 	%g0, PSR_INIT, %psr	/* traps off, set s and ps	*/
    mov %g0, %tbr			/*	TBR at 0x00000000	*/
    b	system_reset		/*	branch to system reset	*/
    mov	%g0, %wim			/* 	Clears the window invalid register	*/
	
    TRAP_ENTRY(0x01)		/* 01 Instruction acess	*/
    TRAP_ENTRY(0x02)		/* 02 Ilegal instruction	*/
    TRAP_ENTRY(0x03)		/* 03 Priviledge instruction	*/
	
	TRAP(0x04, fpu_dis)		/* 04 Floating point disable	*/
	
    mov	%psr, %l0
    mov	%wim, %l3
    b	wof_trap			/*	05 Window overfow	*/
    nop
	
    mov	%psr, %l0
    mov	%wim, %l3
    b	wuf_trap			/*	06 Window underflow	*/
    nop
	
    TRAP_ENTRY(0x07)	/* 07 Memory access not aligment */
    TRAP_ENTRY(0x08)	/* 08 Flotating point exception */
    TRAP_ENTRY(0x09)	/* 09 Data access exception */
    TRAP_ENTRY(0x0a)	/* 10 Tag overflow */
    TRAP_ENTRY(0x0b)	/* 11 Not handled exception */
    TRAP_ENTRY(0x0c)	/* 12 Not handled exception */
    TRAP_ENTRY(0x0d)	/* 13 Not handled exception */
    TRAP_ENTRY(0x0e)	/* 14 Not handled exception */
    TRAP_ENTRY(0x0f)	/* 15 Not handled exception */
    TRAP_ENTRY(0x10)	/* 16 Not handled exception */
    TRAP_ENTRY(0x11)	/* 17 Masked hardware errors */
    TRAP_ENTRY(0x12)	/* 18 Ext INT0 */
    TRAP_ENTRY(0x13)	/* 19 Ext INT1 */
    TRAP_ENTRY(0x14)	/* 20 UART A data ready or trasnmitter ready */
    TRAP_ENTRY(0x15)	/* 21 UART B data ready or trasnmitter ready */
    TRAP_ENTRY(0x16)	/* 22 Correctable error in memory */
    TRAP_ENTRY(0x17)	/* 23 UART error */
    TRAP_ENTRY(0x18)	/* 24 DMA access error */
    TRAP_ENTRY(0x19)	/* 25 DMA time-out */
    TRAP_ENTRY(0x1a)	/* 26 EXT INT2 */
    TRAP_ENTRY(0x1b)	/* 27 EXT INT3 */
    TRAP_ENTRY(0x1c)	/* 28 General purpose timer */
    TRAP_ENTRY(0x1d)	/* 29 Real time clock */
    TRAP_ENTRY(0x1e)	/* 30 EXT INT4 (ethernet) */
    TRAP_ENTRY(0x1f)	/* 31 Watch dog time out */
    
  BAD_TRAP(0x20); BAD_TRAP(0x21); BAD_TRAP(0x22); BAD_TRAP(0x23);	! 20 - 23 undefined
  BAD_TRAP(0x24);                                     				! 24 cp_disabled
            BAD_TRAP(0x25); BAD_TRAP(0x26); BAD_TRAP(0x27);       	! 25 - 27 undefined
  BAD_TRAP(0x28);                                     				! 28 cp_exception
            BAD_TRAP(0x29); BAD_TRAP(0x2A); BAD_TRAP(0x2B);       	! 29 - 2B undefined
  BAD_TRAP(0x2C); BAD_TRAP(0x2D); BAD_TRAP(0x2E); BAD_TRAP(0x2F); 	! 2C - 2F undefined
  BAD_TRAP(0x30); BAD_TRAP(0x31); BAD_TRAP(0x32); BAD_TRAP(0x33); 	! 30 - 33 undefined
  BAD_TRAP(0x34); BAD_TRAP(0x35); BAD_TRAP(0x36); BAD_TRAP(0x37); 	! 34 - 37 undefined
  BAD_TRAP(0x38); BAD_TRAP(0x39); BAD_TRAP(0x3A); BAD_TRAP(0x3B); 	! 38 - 3B undefined
  BAD_TRAP(0x3C); BAD_TRAP(0x3D); BAD_TRAP(0x3E); BAD_TRAP(0x3F); 	! 3C - 3F undefined
  BAD_TRAP(0x40); BAD_TRAP(0x41); BAD_TRAP(0x42); BAD_TRAP(0x43); 	! 40 - 43 undefined
  BAD_TRAP(0x44); BAD_TRAP(0x45); BAD_TRAP(0x46); BAD_TRAP(0x47); 	! 44 - 47 undefined
  BAD_TRAP(0x48); BAD_TRAP(0x49); BAD_TRAP(0x4A); BAD_TRAP(0x4B); 	! 48 - 4B undefined
  BAD_TRAP(0x4C); BAD_TRAP(0x4D); BAD_TRAP(0x4E); BAD_TRAP(0x4F); 	! 4C - 4F undefined
  BAD_TRAP(0x50); BAD_TRAP(0x51); BAD_TRAP(0x52); BAD_TRAP(0x53); 	! 50 - 53 undefined
  BAD_TRAP(0x54); BAD_TRAP(0x55); BAD_TRAP(0x56); BAD_TRAP(0x57); 	! 54 - 57 undefined
  BAD_TRAP(0x58); BAD_TRAP(0x59); BAD_TRAP(0x5A); BAD_TRAP(0x5B); 	! 58 - 5B undefined
  BAD_TRAP(0x5C); BAD_TRAP(0x5D); BAD_TRAP(0x5E); BAD_TRAP(0x5F); 	! 5C - 5F undefined
  BAD_TRAP(0X60); BAD_TRAP(0X61); BAD_TRAP(0X62); BAD_TRAP(0X63); 	! 60 - 63 undefined
  BAD_TRAP(0X64); BAD_TRAP(0X65); BAD_TRAP(0X66); BAD_TRAP(0X67); 	! 64 - 67 undefined
  BAD_TRAP(0X68); BAD_TRAP(0X69); BAD_TRAP(0X6A); BAD_TRAP(0X6B); 	! 68 - 6B undefined
  BAD_TRAP(0X6C); BAD_TRAP(0X6D); BAD_TRAP(0X6E); BAD_TRAP(0X6F); 	! 6C - 6F undefined
  BAD_TRAP(0X70); BAD_TRAP(0X71); BAD_TRAP(0X72); BAD_TRAP(0X73); 	! 70 - 73 undefined
  BAD_TRAP(0X74); BAD_TRAP(0X75); BAD_TRAP(0X76); BAD_TRAP(0X77); 	! 74 - 77 undefined
  BAD_TRAP(0X78); BAD_TRAP(0X79); BAD_TRAP(0X7A); BAD_TRAP(0X7B); 	! 78 - 7B undefined

/* 
   This is a sad patch to make sure that we know where the
   MEC timer control register mirror is so we can stop the timers
   from an external debugger. It is needed because the control
   register is write-only. Trap 0x7C cannot occure in ERC32...
*/

	.global __ERC32_MEC_Timer_Control_Mirror

__ERC32_MEC_Timer_Control_Mirror:

  BAD_TRAP(0x7C); 	! 7C undefined 
  
  BAD_TRAP(0x7D); BAD_TRAP(0x7E); BAD_TRAP(0x7F); 	! 7D - 7F undefined 

  /* 
   *  Software traps
   *
   *  NOTE: At the risk of being redundant... this is not a full
   *        table.  The setjmp on the SPARC requires a window flush trap
   *        handler and RTEMS will preserve the entries that were
   *        installed before.
   */

 
  TRAP( 0x80, syscall ); 			! 80 syscall SW trap
  SOFT_TRAP;  ! 81
  SOFT_TRAP;  ! 82
  TRAP( 0x83, disable_interrupts ); ! 83 enable interrupts
  TRAP( 0x84, enable_interrupts );  ! 84 disable interrupts
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! 85 - 87
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! 88 - 8B
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! 8C - 8F
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! 90 - 93
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! 94 - 97
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! 98 - 9B
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! 9C - 9F
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! A0 - A3
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! A4 - A7
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! A8 - AB
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! AC - AF
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! B0 - B3
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! B4 - B7
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! B8 - BB
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! BC - BF
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! C0 - C3
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! C4 - C7
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! C8 - CB
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! CC - CF
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! D0 - D3
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! D4 - D7
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! D8 - DB
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! DC - DF
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! E0 - E3
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! E4 - E7
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! E8 - EB
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! EC - EF
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! F0 - F3
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! F4 - F7
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! F8 - FB
  SOFT_TRAP; SOFT_TRAP; SOFT_TRAP; SOFT_TRAP;   ! FC - FF
	
	.align 4
	
system_reset:
	
	/*	Clear the global registers	*/
	mov	%g0,%g1
	mov	%g0,%g2
	mov	%g0,%g3
	mov	%g0,%g4
	mov	%g0,%g5
	mov	%g0,%g6
	mov	%g0,%g7
	
	/*	Set the MEC registers	*/
	set	ERC32_MEC, %g3

	clr	[ %g3 + MEC_WATCHDOG_TRAP_DOOR ]

	set	0x1000, %g2

	st	%g2, [ %g3 + MEC_ERROR_AND_RESET_STATUS ]

	set	MEM_INIT, %g2
	
	st	%g2, [%g3 + MEC_MEMORY_CONFIGURATION]

	set	WST_INIT, %g2

	st	%g2, [%g3 + MEC_WAITSTATE_CONFIGURATION]

	set	CTR_INIT, %g2

	st	%g2, [%g3 + MEC_CONTROL_REGISTER]

	/*
	 ERC32 register initialisation
	 Because of a problem with the ERC32 IU chip, all processor
	 registers need to be written before they are read. The problem
	 is caused by internal parity checking of the registers, and
	 will result in a trap, causing an error reset or halt.
	*/

	mov	%g0, %y	/*	clears the Y register	*/

	mov	8, %g1	/* 8 loops to clear all the LOCAL an IN register windows	*/

	/*	Clear all the IN and LOCAL registers	*/
1:	mov	%g0, %i0
	mov	%g0, %i1
	mov	%g0, %i2
	mov	%g0, %i3
	mov	%g0, %i4
	mov	%g0, %i5
	mov	%g0, %i6
	mov	%g0, %i7
	mov	%g0, %l0
	mov	%g0, %l1
	mov	%g0, %l2
	mov	%g0, %l3
	mov	%g0, %l4
	mov	%g0, %l5
	mov	%g0, %l6
	mov	%g0, %l7
	
	save
	deccc	%g1
	bne	1b
	nop

/*	
 * If we configure the kernel to be executed from ROM memory we initialize the
 * RAM memory clear it and copying all the code and data from ROM to RAM
 * After these operations, the TBR may be at 0x02000000 address
 */
#ifdef CONFIG_ROMKERNEL

	/*	Initialize the memory to prevent EDAC errors	*/
	set	ERC32_RAM_START, %g1
	set	ERC32_RAM_SIZE, %g2

1:	subcc	%g2,8,%g2
	bne	1b
	std	%g0, [%g1 + %g2]	/*	Sets the memory to zero	*/
	
	/* Move code to RAM	*/

	set	ERC32_ROM_START,%g1
	set	CODE_START,%g2
	set	DATA_END,%o2
	sub	%o2,%g2,%o2		/*	In %o2 we have the code length	*/

1:	subcc	%o2,8,%o2
	ldd	[%g1+%o2],%o0
	std	%o0,[%g2+%o2]
	bne	1b
	nop

	set	1f,%g3		/* load the RAM address of the 1forward label	*/

	jmpl	%g3,%g0
	nop

#endif

1:	/* At this point the code is begin executed from RAM	*/

	/* Sets the new trap table, now in RAM	*/
	set	trap_table,%g2
	wr	%g2,%tbr		/* TBR at 0x02000000	*/

	nop
	nop
	nop

	/* Set PSR and enable traps	*/

	wr	%g0, PSR_INIT|PSR_ET, %psr

	nop
	nop
	nop

	/* Initialize FPU registers to zero	*/

	mov	1, %o0
	ld	[ %g0 ], %f0
	tst	%o0
	be	1f
	nop
	
	ld	[ %g0 ], %f1
	ld	[ %g0 ], %f2
	ld	[ %g0 ], %f3
	ld	[ %g0 ], %f4
	ld	[ %g0 ], %f5
	ld	[ %g0 ], %f6
	ld	[ %g0 ], %f7
	ld	[ %g0 ], %f8
	ld	[ %g0 ], %f9
	ld	[ %g0 ], %f10
	ld	[ %g0 ], %f11
	ld	[ %g0 ], %f12
	ld	[ %g0 ], %f13
	ld	[ %g0 ], %f14
	ld	[ %g0 ], %f15
	ld	[ %g0 ], %f16
	ld	[ %g0 ], %f17
	ld	[ %g0 ], %f18
	ld	[ %g0 ], %f19
	ld	[ %g0 ], %f20
	ld	[ %g0 ], %f21
	ld	[ %g0 ], %f22
	ld	[ %g0 ], %f23
	ld	[ %g0 ], %f24
	ld	[ %g0 ], %f25
	ld	[ %g0 ], %f26
	ld	[ %g0 ], %f27
	ld	[ %g0 ], %f28
	ld	[ %g0 ], %f29
	ld	[ %g0 ], %f30
	ld	[ %g0 ], %f31

	/*	Initialize the invalid window	*/
1:	wr	%g0, WIM_INIT, %wim
	nop;nop;nop

	/*	
	 * Initialize the stack pointer and the frame pointer at the end of the RAM	
	 */
//	set SPP_INIT - CPU_MINIMUM_STACK_FRAME_SIZE, %g2
	set SPP_INIT, %g2
	mov %g2, %sp
//	mov %g2, %fp
	
	/*
	 * At this point, the ASM initialization is finished so, we call the init
	 * method that initialize the uKernel
	 */
	 
	 call OS_Start
	 sub %sp, CPU_MINIMUM_STACK_FRAME_SIZE, %sp		/*	prepare the stack frame for the init method	*/
	 
fatal_return:
	mov	1, %g1
	ta      0                       ! Halt if _main returns ...
	nop

/*	FPU Interrupt Service Routine	*/
fpu_dis:
    rd %psr, %l0
    wr %l0, PSR_EF, %psr	! Enable the FP unit
    
    jmpl %l2 - 4, %g0       /*  return to the instruction that provoked the trap, prev instruction   */
    rett %l2
	nop
