
| C run time management stuff for ERCOS kernel on Motorola MC68332
|
| Copyright 2007 (c) by Cesar Rodriguez Ortega <cesar.rodriguez@srg.aut.uah.es>
| Copyright 2004-2005 (c) by Oscar Garcia Poblacion and Oscar Rodriguez Polo
| University of Alcala,
| Computer Engineering Department
| For further information, please visit http://atc1.aut.uah.es
| 
| This software is provided under the terms of the GNU General Public v2
| Licence. A full copy of the GNU GPL is provided in the file COPYING
| found in the development root of ERCOS-RT.

| NOTE -- ASM is defined in the command-line via -DASM (see Rules.make)
#include "crt0.h"

| Global symbols
.global m68k_reset_entry
.global startup
.global m68k_interrupt_entry
.global m68k_sup_disable_irqs
.global m68k_sup_enable_irqs
.global m68k_lpstop
.global m68k_trap0
.global m68k_trap1
.global m68k_trap2
.global m68k_trap3
.global m68k_atomic_inc
.global m68k_atomic_dec
.global m68k_get_sr | DEBUG

.section ".romvec"
.align 4
m69k_vector_table:
	.long end			| (0) Initial stack pointer
	.long m68k_reset_entry		| (1) Reset entry point
	.long m68k_interrupt_entry	| (2) Bus error
	.long m68k_interrupt_entry	| (3) Address error
	.long m68k_interrupt_entry	| (4) Illegal Instruction
	.long m68k_interrupt_entry	| (5) Zero division
	.long m68k_interrupt_entry	| (6) CHK, CHK2 instructions
	.long m68k_interrupt_entry	| (7) TRAPcc, TRAPV instructions
	.long m68k_interrupt_entry	| (8) Privilege violation
	.long m68k_interrupt_entry	| (9) Trace
	.long m68k_interrupt_entry	| (10) Line 1010 emulator
	.long m68k_interrupt_entry	| (11) Line 1111 emulator
	.long m68k_interrupt_entry	| (12) Hardware breakpoint
	.long m68k_interrupt_entry	| (13) (Reserved, coprocessor protocol violation)
	.long m68k_interrupt_entry	| (14) Format error and uninitialized interrupt
	.long m68k_interrupt_entry	| (15) Format error and uninitialized interrupt
	.long m68k_interrupt_entry	| (16) Reserved
	.long m68k_interrupt_entry	| (17) Reserved
	.long m68k_interrupt_entry	| (18) Reserved
	.long m68k_interrupt_entry	| (19) Reserved
	.long m68k_interrupt_entry	| (20) Reserved
	.long m68k_interrupt_entry	| (21) Reserved
	.long m68k_interrupt_entry	| (22) Reserved
	.long m68k_interrupt_entry	| (23) Reserved
	.long m68k_interrupt_entry	| (24) Spurious interrupt
	.long m68k_interrupt_entry	| (25) Level 1 interrupt autovector
	.long m68k_interrupt_entry	| (26) Level 2 interrupt autovector
	.long m68k_timer_entry		| (27) Level 3 intr. aut. (timer)
	.long m68k_interrupt_entry	| (28) Level 4 interrupt autovector
	.long m68k_interrupt_entry	| (29) Level 5 interrupt autovector
	.long m68k_interrupt_entry	| (30) Level 6 interrupt autovector
	.long m68k_interrupt_entry	| (31) Level 7 interrupt autovector
	.long m68k_interrupt_entry	| (32) Trap instruction vector 0
	.long m68k_interrupt_entry	| (33) Trap instruction vector 1
	.long m68k_interrupt_entry	| (34) Trap instruction vector 2
	.long m68k_interrupt_entry	| (35) Trap instruction vector 3
	.long m68k_interrupt_entry	| (36) Trap instruction vector 4
	.long m68k_interrupt_entry	| (37) Trap instruction vector 5
	.long m68k_interrupt_entry	| (38) Trap instruction vector 6
	.long m68k_interrupt_entry	| (39) Trap instruction vector 7
	.long m68k_interrupt_entry	| (40) Trap instruction vector 8
	.long m68k_interrupt_entry	| (41) Trap instruction vector 9
	.long m68k_interrupt_entry	| (42) Trap instruction vector 10
	.long m68k_interrupt_entry	| (43) Trap instruction vector 11
	.long m68k_interrupt_entry	| (44) Trap instruction vector 12
	.long m68k_interrupt_entry	| (45) Trap instruction vector 13
	.long m68k_interrupt_entry	| (46) Trap instruction vector 14
	.long m68k_interrupt_entry	| (47) Trap instruction vector 15

	.long m68k_interrupt_entry	| (48) (Reserved, coprocessor)
	.long m68k_interrupt_entry	| (49) (Reserved, coprocessor)
	.long m68k_interrupt_entry	| (50) (Reserved, coprocessor)
	.long m68k_interrupt_entry	| (51) (Reserved, coprocessor)
	.long m68k_interrupt_entry	| (52) (Reserved, coprocessor)
	.long m68k_interrupt_entry	| (53) (Reserved, coprocessor)
	.long m68k_interrupt_entry	| (54) (Reserved, coprocessor)
	.long m68k_interrupt_entry	| (55) (Reserved, coprocessor)
	.long m68k_interrupt_entry	| (56) (Reserved, coprocessor)
	.long m68k_interrupt_entry	| (57) (Reserved, coprocessor)
	.long m68k_interrupt_entry	| (58) (Reserved, coprocessor)
	.rept 5
	.long m68k_interrupt_entry	| (59-63) (Unassigned, reserved)
	.endr 
	.rept 192
	.long m68k_interrupt_entry	| (64-255) (Unassigned, reserved)
	.endr

.section ".text"
.align 4 

startup:
m68k_reset_entry:

	| for a start, initialize the stack pointer. kstack points to the stack
	| top (see annotations at kernel/task.c)
	lea		kstack, %a6
	movea.l		(%a6), %ssp

	| initialize Status Register: no tracing, supervisor state, interrupt
	| priority mask = 7, no extend flag, no negative, no zero, no overflow,
	| no carry
	move.w		#0x2700, %sr

	| initialize SYPCR
	lea		0x00fffa21, %a0
	move.b		#M68K_SYPCR, (%a0)

	| clear bss area
	lea		bss_start, %a0
	lea		end, %a1
1:
	clr.l		(%a0)+
	cmp.l		%a0, %a1
	bgt		1b

	| call C code (it should never return!)
	jsr		m68k_cstart

	| freeze
1:	bra		1b

m68k_timer_entry :
	| disable interrupts
	ori.w		#0x0700, %sr
	| decrement timer countdown
	subq.l		#1, (m68k_timer_countdown)
	| enter in the regular handler if 0
	beq		1f
	| check if another interrupt was triggered in the first instruction of
	| this handler
	tst.l		(m68k_timer_interrupted)
	bne		1f
	rte

1:
	| enable interrupts prior to entering in the regular interrupt handler
	andi.w		#0xfbff, %sr

m68k_interrupt_entry :
	| save data and address registers
	movem.l		%d0-%d7, -(%sp)
	movem.l		%a0-%a6, -(%sp)

	| save usp
	move.l		%usp, %a0
	move.l		%a0, -(%sp)

	| argument 2: context pointer
	move.l		%sp, -(%sp)

	| argument 1: copy of the vector offset word, pushed by hardware
	move.w		(%sp,74), %a1
	move.l		%a1, -(%sp)

	| call the high-level interrupt handler
	jsr		m68k_interrupt_handler2

	| discard the arguments
	adda.l		#0x8, %sp

	| restore usp
	move.l		(%sp)+, %a0
	move.l		%a0, %usp

	| restore address and data registers
	movem.l		(%sp)+, %a6-%a0
	movem.l		(%sp)+, %d7-%d0

	| the non-return point!
	rte

| unsigned m68k_get_sr (void);
m68k_get_sr :
	move		%sr, %d0
	rts

| void m68k_sup_disable_irqs (void);
m68k_sup_disable_irqs:
	move.w		%sr, %d0
	ori.w		#0xf8ff, %d0
	move.w		%d0, (m68k_sr_cache)
	ori		#0x0700, %sr
	rts

| void m68k_sup_enable_irqs (void);
m68k_sup_enable_irqs:
	move.w		%sr, %d0
	and.w		(m68k_sr_cache), %d0
	move.w		%d0, %sr
	rts

| void m68k_lpstop (void);
m68k_lpstop:
	| NOTE -- that the next instruction assumes we are executing at IRL 0
	lpstop		#0x2000
	rts

| void m68k_trap0 (void);
m68k_trap0:
	trap		#0
	rts

| void m68k_trap1 (void);
m68k_trap1:
	trap		#1
	rts

| void m68k_trap2 (void);
m68k_trap2:
	trap		#2
	rts

| void m68k_trap3 (void);
m68k_trap3:
	trap		#3
	rts

| void m68k_atomic_inc (volatile long *i);
m68k_atomic_inc:
	move.l		(%sp,4), %a0
	addq.l		#1, (%a0)
	rts

| void m68k_atomic_dec (volatile long *i);
m68k_atomic_dec:
	move.l		(%sp,4), %a0
	subq.l		#1, (%a0)
	rts

.section ".bss"
m68k_sr_cache:		.space 4

| vim:ts=8:syn=asm68k:

