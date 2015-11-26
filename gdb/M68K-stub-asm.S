#ifndef __REGISTER_PREFIX__
#define __REGISTER_PREFIX__
#endif

#ifndef __IMMEDIATE_PREFIX__
#define __IMMEDIATE_PREFIX__ #
#endif

/* ANSI concatenation macros.  */

#define CONCAT1(a, b) CONCAT2(a, b)
#define CONCAT2(a, b) a ## b

/* Use the right prefix for global labels.  */

#define SYM(x) CONCAT1 (__USER_LABEL_PREFIX__, x)

/* Use the right prefix for registers.  */

#define REG(x) CONCAT1 (__REGISTER_PREFIX__, x)

/* Use the right prefix for immediate values.  */

#define IMM(x) CONCAT1 (__IMMEDIATE_PREFIX__, x)

#define d0 REG (d0)
#define d1 REG (d1)
#define d2 REG (d2)
#define d3 REG (d3)
#define d4 REG (d4)
#define d5 REG (d5)
#define d6 REG (d6)
#define d7 REG (d7)
#define a0 REG (a0)
#define a1 REG (a1)
#define a2 REG (a2)
#define a3 REG (a3)
#define a4 REG (a4)
#define a5 REG (a5)
#define a6 REG (a6)
#define a7 REG (a7)
#define fp REG (fp)
#define sp REG (sp)

#define sr REG (sr)

#define DISABLE_INTERRUPTS()	oriw   #0x0700,sr
#define BREAKPOINT() 			trap #1


#ifdef __HAVE_68881_
/* do an fsave, then remember the address to begin a restore from */
#define SAVE_FP_REGS()    fsave   a0@-		\
			  fmovemx fp0-fp7,_registers+72 \
			  fmoveml fpcr/fpsr/fpi,_registers+168
			   
#define RESTORE_FP_REGS() fmoveml  _registers+168,fpcr/fpsr/fpi            \
    fmovemx  _registers+72,fp0-fp7                   \
    cmpl     #-1,a0@    /*  skip frestore flag set ?*/ \
    beq      skip_frestore                           \
    frestore a0@+                                    \
skip_frestore:                                       

#else
#define SAVE_FP_REGS()
#define RESTORE_FP_REGS()
#endif /* __HAVE_68881__ */

	.text 
	.global return_to_super 
return_to_super: 
        movel   _registers+60,sp /* get new stack pointer */        
        movel   _lastFrame,a0   /* get last frame info  */			
        bra     return_to_any

	.global return_to_user 												
return_to_user:													
        movel   _registers+60,a0 /* get usp */                      
        movel   a0,usp           /* set usp */						
        movel   _superStack,sp  /* get original stack pointer */    
																	
return_to_any:														
        movel   _lastFrame,a0   /* get last frame info  */          
        movel   a0@+,_lastFrame /* link in previous frame     */    
        addql   #8,a0           /* skip over pc, vector#*/          
        movew   a0@+,d0       /* get # of words in cpu frame */   
        addw    d0,a0           /* point to end of data        */   
        addw    d0,a0           /* point to end of data        */   
        movel   a0,a1                                               
#                                                                   
# copy the stack frame                                              
        subql   #1,d0                                               
copyUserLoop:                                                       
        movew   a1@-,sp@-                                           
        dbf     d0,copyUserLoop

        RESTORE_FP_REGS()                                              
        
		moveml  _registers,d0-d7/a0-a6			        
		rte  /* pop and go! */                                    




/* this function is called immediately when a level 7 interrupt occurs */
/* if the previous interrupt level was 7 then we're already servicing  */
/* this interrupt and an rte is in order to return to the debugger.    */
/* For the 68000, the offset for sr is 6 due to the jsr return address */

	.text
	.globl __debug_level7
	
__debug_level7:
	movew   d0,sp@-
	
#if defined (mc68020) || defined (mc68332)
	movew   sp@(2),d0
#else
	movew   sp@(6),d0
#endif

	andiw   #0x700,d0
	cmpiw   #0x700,d0
	beq     _already7
	movew   sp@+,d0
	bra     __catchException
_already7:
	movew   sp@+,d0
	
#if !defined (mc68020) && !defined (mc68332)
	lea     sp@(4),sp     /* pull off 68000 return address */
#endif
	rte
	
	
#if defined (mc68020) || defined (mc68332)

/* This function is called when a 68020 exception occurs.  It saves
 * all the cpu and fpcp regs in the _registers array, creates a frame on a
 * linked list of frames which has the cpu and fpcp stack frames needed
 * to properly restore the context of these processors, and invokes
 * an exception handler (remcom_handler).
 *
 * stack on entry:                       stack on exit:
 *   N bytes of junk                     exception # MSWord
 *   Exception Format Word               exception # MSWord
 *   Program counter LSWord              
 *   Program counter MSWord             
 *   Status Register                    
 *                                       
 *                                       
 */

	.text
	.globl _catchException
_catchException:

DISABLE_INTERRUPTS();

	moveml  d0-d7/a0-a6,_registers 	/* save registers        */
	movel	_lastFrame,a0			/* last frame pointer */

SAVE_FP_REGS();        

	lea     _registers,a5   /* get address of registers     */
	movew   sp@,d1          /* get status register          */
	movew   d1,a5@(66)      /* save sr		 	*/				
	movel   sp@(2),a4       /* save pc in a4 for later use  */
    movel   a4,a5@(68)      /* save pc in _regisers[]      	*/
																
#																
# figure out how many bytes in the stack frame					
	movew   sp@(6),d0		/* get '020 exception format	*/
    movew   d0,d2           /* make a copy of format word   */
    andiw   #0xf000,d0      /* mask off format type         */
    rolw    #5,d0           /* rotate into the low byte *2  */
    lea     _exceptionSize,a1 
    addw    d0,a1           /* index into the table         */
	movew   a1@,d0          /* get number of words in frame */
    movew   d0,d3           /* save it                      */
    subw    d0,a0			/* adjust save pointer          */
    subw    d0,a0			/* adjust save pointer(bytes)   */
	movel   a0,a1           /* copy save pointer            */
	subql   #1,d0           /* predecrement loop counter    */
#
# copy the frame
saveFrameLoop:
	movew  	sp@+,a1@+
	dbf     d0,saveFrameLoop
#
# now that the stack has been clenaed,
# save the a7 in use at time of exception
    movel   sp,_superStack  /* save supervisor sp           */
    andiw   #0x2000,d1      /* were we in supervisor mode ? */
    beq     userMode
	movel   a7,a5@(60)      /* save a7                  */
    bra     a7saveDone
userMode:
	movel   usp,a1
	movel   a1,a5@(60)      /* save user stack pointer	*/
a7saveDone:

#
# save size of frame
	movew   d3,a0@-

#
# compute exception number
	andl    #0xfff,d2   	/* mask off vector offset	*/
	lsrw    #2,d2   	/* divide by 4 to get vect num	*/
    movel   d2,a0@-         /* save it                      */
#																
# save pc causing exception										
    movel   a4,a0@-													
#
# save old frame link and set the new value
	movel	_lastFrame,a1	/* last frame pointer */
	movel   a1,a0@-		/* save pointer to prev frame	*/
    movel   a0,_lastFrame

    movel   d2,sp@-		/* push exception num           */
	movel   _exceptionHook,a0  /* get address of handler */
	jbsr    a0@             /* and call it */
    clrl    sp@             /* replace exception num parm with frame ptr */
    jbsr     __returnFromException   /* jbsr, but never returns */


#else /* mc68000 */
/* This function is called when an exception occurs.  It translates the
 * return address found on the stack into an exception vector # which
 * is then handled by either handle_exception or a system handler.
 * _catchException provides a front end for both.  
 *
 * stack on entry:                       stack on exit:
 *   Program counter MSWord              exception # MSWord 
 *   Program counter LSWord              exception # MSWord
 *   Status Register                     
 *   Return Address  MSWord              
 *   Return Address  LSWord             
 */
asm("																	\n\
.text																	\n\
.globl __catchException													\n\
__catchException:");

DISABLE_INTERRUPTS();
asm("																	\n\
	moveml d0-d7/a0-a6,_registers  /* save registers               */	\n\
	movel	_lastFrame,a0	/* last frame pointer */					\n\
");

SAVE_FP_REGS();        

asm("																	\n\
    lea     _registers,a5   /* get address of registers     */			\n\
    movel   sp@+,d2         /* pop return address           */			\n\
	addl 	#1530,d2        /* convert return addr to 	*/				\n\
	divs 	#6,d2   	/*  exception number		*/					\n\
	extl    d2   														\n\
																		\n\
        moveql  #3,d3           /* assume a three word frame     */		\n\
																		\n\
        cmpiw   #3,d2           /* bus error or address error ? */		\n\
        bgt     normal          /* if >3 then normal error      */		\n\
        movel   sp@+,a0@-       /* copy error info to frame buff*/		\n\
        movel   sp@+,a0@-       /* these are never used         */		\n\
        moveql  #7,d3           /* this is a 7 word frame       */		\n\
     																	\n\
normal:   																\n\
	movew   sp@+,d1         /* pop status register          */			\n\
        movel   sp@+,a4         /* pop program counter          */		\n\
        movew   d1,a5@(66)      /* save sr		 	*/					\n\
        movel   a4,a5@(68)      /* save pc in _regisers[]      	*/		\n\
        movel   a4,a0@-         /* copy pc to frame buffer      */		\n\
	movew   d1,a0@-         /* copy sr to frame buffer      */			\n\
																		\n\
        movel   sp,_superStack  /* save supervisor sp          */		\n\
																		\n\
        andiw   #0x2000,d1      /* were we in supervisor mode ? */		\n\
        beq     userMode       											\n\
        movel   a7,a5@(60)      /* save a7                  */			\n\
        bra     saveDone             									\n\
userMode:																\n\
        movel   usp,a1    	/* save user stack pointer 	*/				\n\
        movel   a1,a5@(60)      /* save user stack pointer	*/			\n\
saveDone:																\n\
																		\n\
        movew   d3,a0@-         /* push frame size in words     */		\n\
        movel   d2,a0@-         /* push vector number           */		\n\
        movel   a4,a0@-         /* push exception pc            */		\n\
																		\n\
#																		\n\
# save old frame link and set the new value								\n\
	movel	_lastFrame,a1	/* last frame pointer */					\n\
	movel   a1,a0@-		/* save pointer to prev frame	*/				\n\
        movel   a0,_lastFrame											\n\
																		\n\
        movel   d2,sp@-		/* push exception num           */			\n\
	movel   _exceptionHook,a0  /* get address of handler */				\n\
        jbsr    a0@             /* and call it */						\n\
        clrl    sp@             /* replace exception num parm with frame ptr */ \n\
        jbsr     __returnFromException   /* jbsr, but never returns */	\n\
");
#endif
	
/*
 * remcomHandler is a front end for handle_exception.  It moves the
 * stack pointer into an area reserved for debugger use in case the
 * breakpoint happened in supervisor mode.
 */
 	.globl remcomHandler
remcomHandler:
	addl    #4,sp				/* pop off return address     */
	movel   sp@+,d0				/* get the exception number   */
	movel   _stackPtr,sp		/* move to remcom stack area  */
	movel   d0,sp@-				/* push exception onto stack  */
	jbsr    _handle_exception	/* this never returns */
	rts							/* return */






        
        
