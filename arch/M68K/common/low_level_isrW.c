#include <target.h>
#include <cpu/cputypes.h>
#include <cpu/arch.h>
#include <cpu/cpu.h>
#include <cpu/syscall.h>
#include <sys/types.h>
#include <kernel/irq.h>

extern OS_task_t *current;
extern OS_task_t *old_current;

extern uint8 force_dispatch;
extern volatile uint32 schedule_active;

int irq_counter;

/**
 * TODO: La siguiente macro no sirve para nada. Habría que quitarla porque
 * además no es nada fácil implementarla. La pongo para que no pete la 
 * compilación
 */
int CPU_IS_IN_TRAP_HANDLER() { return   (0 != irq_counter); }


#define SAVE_ALL_REGISTERS __asm__ __volatile__ \
  ("movem.l %%d0-%%d7/%%a0-%%a6, -(%%sp)":::"%sp")
#define RESTORE_ALL_REGISTERS __asm__ __volatile__ \
  ("movem.l (%a7)+,%d0-%d7/%a0-%a6")

#define SAVE_STACK_TO_CONTEXT(x)      __asm__ __volatile__ \
  ("move.l %%sp,%0" :: "m" (x) )
#define RESTORE_STACK_FROM_CONTEXT(x) __asm__ __volatile__ \
  ("move.l %0,%%sp" :  "=m" (x) )
#define RETURN_FROM_EXCEPTION		  __asm__ __volatile__ \
  ("rte")

volatile void dispatch_event(struct exceptionStackFrame_t* stackFrame);

/*
 * volatile void low_level_isrW (uint32 D0, uint32 D1, uint32 D2, uint32 D3,
							  uint32 D4, uint32 D5, uint32 D6, uint32 D7,
							  uint32 A0, uint32 A1, uint32 A2, uint32 A3,
							  uint32 A4, uint32 A5, uint32 A6,
							  struct exceptionStackFrame_t stackFrame) {
							  */
volatile void low_level_isrW (struct exceptionStackFrame_t* stackFrame) {	
	
	dispatch_event(stackFrame);
	printf("x");
	
	OS_DisableIRQ();
	
	if (stackFrame->statusRegister & 
      ACTIVE_SUPERVISOR_BIT_MASK == ACTIVE_SUPERVISOR_BIT_MASK) {
		/**
		 * Then we have at least another exception nested. We don't process
		 * the isr handlers nor call schedule nor call dispatch until we had
		 * called OS_TrapEntry for all of them
		 * */
			
		 
	} else {
		/** 
		 * This means that when we return from this exception we will be in
		 * user mode again. It's time to process all the exceptions, call the
		 * scheduler and possibly make a context switch
		 */
		 
		if (schedule_active && !force_dispatch) {
		 	/** This means that the interrupts we were processing happended
		 	 * while in a system call (Not a trap; In ERCOS "system call" 
		 	 * is really a "call to a system function"). We are not going
		 	 * to call scheduler here. We are going to yield control to the
		 	 * interrupted system call and it will call the scheduler when it
		 	 * finishs
		 	 * 
		 	 * TODO: Ahora que lo pienso, esto funcionará bien siempre que 
		 	 * todas las llamadas al sistema requieran planificación
		 	 * (NEED_REESCHED), porque podría darse el caso de que se 
		 	 * produzcan interrupciones en una llamada al sistema que no 
		 	 * requiera replanificación y como las ints delegan la llamada
		 	 * al planificador, esta no se produciría nunca :(
		 	 */
		 	 printf(".");
		 	 
		} else {
			 	
			printf("#\n"); 
			//OS_SchedCurrentReady();
			OS_IRQHandleEvent();
			printf("low_level_isrW: replanificando la vida\n");	 
			OS_Schedule();
				 
			if (current != old_current || force_dispatch ) {
				/**
			 	* Then things have changed. Now there is another thread with
			 	* higher priority so we make a context switch as instructed by
			 	* the scheduler
			 	*/
			 	printf("low_level_isrW: ¡Cambio de contexto va!\n");
			 	printf("low_level_isrW: current=%x y old_current=%x\n", 
          current, old_current);
			 	force_dispatch = 0;
			 	SAVE_STACK_TO_CONTEXT(old_current->context.A7);
			 	RESTORE_STACK_FROM_CONTEXT(current->context.A7);
			}
		}
	}
	__asm__ __volatile__ ("adda #8,%sp");
	RESTORE_ALL_REGISTERS;
	RETURN_FROM_EXCEPTION;
}

volatile void dispatch_event(struct exceptionStackFrame_t* stackFrame) {
	word16 nVector;
	word16 offsetVector;
	
	offsetVector=stackFrame->vectorOffsetAndFormat & 0x0FFF;
	nVector = offsetVector / BYTES_PER_VECTOR;
	
	if (nVector==VECTOR_FOR_TRAP_0) {
		/** Then is a syscall */
		syscall(stackFrame);
	} else {
		/** Then is another kind of exception */
		OS_TrapEntry(nVector, 0, 0);
	}
}



