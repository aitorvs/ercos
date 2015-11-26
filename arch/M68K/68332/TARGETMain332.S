**********************************************************
* 	Oscar Rodriguez Polo	c) 2004

*	MAIN PROGRAM FOR THE PLATFORM
***********************************************************



	xref	_main
	xref	_exit
	xref	data
	xref 	rompOutSeg	
	xref 	_needed_destruction_head

	section init,,"bootcode"
	xdef	__main


	
__main
  
*	movea.l	#$00000000,a0	
	; reprogram the base address for the interrupt table to 0x100000
	movea.l	#$00100000,a0
	movec  	a0,VBR

	movea.l #$FFFA00,a4 	MCR register
	move    #$60CF,(a4)  

	movea.l #$FFFA04,a4 	Reloj interno a 16.77 Mhz , E Clock a 16.77/8
	move    #$7F08,(a4)  

	movea.l #$FFFA21,a4 	
*	move.b    #$00,(a4)    Clear SYPCR to disable the watchdog timer
*	move.b    #$C0,(a4)    Set SYPCR to enable the watchdog timer, T=8 seconds 
	move.b    #$D0,(a4)    Set SYPCR to enable the watchdog timer, T=32 seconds 

	movea.l #$FFFA44,a4 	CSPAR0 CS1..5
	move    #$3FFF,(a4)  

	movea.l #$FFFA46,a4 	CSPAR0 CS6..10 
	move    #$00FD,(a4)  	CS6 is A19

*	movea.l #$FFFA48,a4 	CSBART
*	move    #$0005,(a4)  

*	movea.l #$FFFA4A,a4 	CSORBT 
*	move    #$68F0,(a4)  	READ ONLY


	movea.l #$FFFA4C,a4 	CSBAR0 MSG RAM 
	move    #$2007,(a4)  

	movea.l #$FFFA4E,a4 	CSOR0 MSG RAM 
	move    #$7830,(a4)  	0 WAIT
*	move    #$78F0,(a4)  	3 WAIT



	movea.l #$FFFA50,a4 	CSBAR1 MSG RAM 
	move    #$3007,(a4)  

	movea.l #$FFFA52,a4 	CSOR1 MSG RAM 
	move    #$7830,(a4)  	0 WAIT
*	move    #$78F0,(a4)  	3 WAIT


	movea.l #$FFFA54,a4 	CSBAR2 MSG RAM 
	move    #$4007,(a4)  

	movea.l #$FFFA56,a4 	CSOR2 MSG RAM 
	move    #$7830,(a4)  	0 WAIT
*	move    #$78F0,(a4)  	3 WAIT

	movea.l #$FFFA58,a4 	CSBAR3 MSG RAM 
	move    #$5007,(a4)  

	movea.l #$FFFA5A,a4 	CSOR3 MSG RAM 
	move    #$7830,(a4)  	0 WAIT
*	move    #$78F0,(a4)  	3 WAIT


* EEPROM EXTERNA

	movea.l #$FFFA5C,a4 	CSBAR4 E2PROM EXTERNA
	move    #$0405,(a4)  

	movea.l #$FFFA5E,a4 	CSOR4 E2PROM 
	move    #$78F0,(a4)  

* FIN EEPROM EXTERNA

	movea.l #$FFFA68,a4 	CSBAR7 RAM EXTERNA
	move    #$1007,(a4)  

	movea.l #$FFFA6A,a4 	CSOR7 RAM EXTERNA
	move    #$3830,(a4)  	0 WAIT
*	move    #$3870,(a4)  	1 WAIT

	movea.l #$FFFA6C,a4 	CSBAR8 RAM EXTERNA
	move    #$1007,(a4)  

	movea.l #$FFFA6E,a4 	CSOR8 RAM EXTERNA
	move    #$5830,(a4)  	0 WAIT
*	move    #$5870,(a4)  	1 WAIT

	movea.l	#$01BFFFC,a7	Set SSP (A7) to absolute address 
*	movea.l	#$003FFFC,a7	Set SSP (A7) to absolute address 
	lea	data,a5		Load A5 with address of static data group.

	lea.l rompOutSeg,a0 	ORIGEN

rep0
	move.l (a0)+,d0	contador
	beq cont
	sub.l	#1,d0
	move.l (a0)+,a1
rep1
	move.b (a0)+,(a1)+
	dbf d0,rep1
	bra rep0

cont

	

	suba.l	a6,a6		A6 is the frame pointer; 0 marks top of stack.
*				Clear other registers for debugging purposes.
	suba.l	a4,a4
	suba.l	a3,a3
	suba.l	a2,a2
	suba.l	a1,a1
	suba.l	a0,a0
	clr.l	d7
	clr.l	d6
	clr.l	d5
	clr.l	d4
	clr.l	d3
	clr.l	d2
	clr.l	d1
	clr.l	d0
	move.l #$00000000,_needed_destruction_head
	clr.l	-(a7)		
	pea	(a7)		Pass pointer to 0. argv[argc] must be 0.
	clr.l	-(a7)		Pass a zero to main; might take it as "argc".
	bsr.l	_main		Jump to the main routine.
	clr.l	-(a7)		Pass status 0 to _exit
	bsr.l	_exit		Need to call exit for C++

* What happens if this call returns is somewhat arbitrary.
* There is nothing more to do, so the processor could halt.
* The following is a jump to self.
loop
	bra	loop

* The following absolute segment defines the reset vectors.
* Absolute location 0 contains the reset SSP and location 4 the reset PC.

	org	0
	dc.l	$01BFFFC
*	dc.l	$003FFFC
	dc.l	__main

* The label on the end directive defines __main as the ".start" address.
	end	__main
