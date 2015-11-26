#include <cpu/cputypes.h>
#include <cpu/arch.h>
#include <cpu/syscall.h>
#include <syscall.h>

extern OS_task_t *current;
extern OS_task_t *old_current;

void syscall(struct exceptionStackFrame_t* trapFrame) {

	word16 syscallNR;
	//uint32* contextDataRegistersIndex;
	uint32* contextStatusRegisterIndex;
	uint32* contextAdressRegistersIndex;
	
	syscallNR= *(word16*)(trapFrame+1);
	
	printf ("llamada al sistema con ID=%d \n",syscallNR);
	
	switch (syscallNR) {
		case _NR_schedule:
			/** Just return, since all TRAPS calls scheduler before
			 * trying to return to user mode
			 */
			force_dispatch=1;
			break;
	}


}


