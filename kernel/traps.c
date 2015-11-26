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
 * File:    $Source$
 * 
 * $Id: traps.c 1261 2008-02-01 16:04:36Z cesar $
 *  
 * $Revision: 1261 $
 * $Date: 2008-02-01 17:04:36 +0100 (vie, 01 feb 2008) $
 * Last CVS commit by $Author: cesar $
 * 
 * Registro de cambios
 * 
 * $Log$
 * Revision 1.5  2005/09/28 14:45:56  aitor
 * The OS_Interrupt_Control_t structure does not have any queue now. We use
 * the semaphore queue to manage the irq events.
 *
 * Revision 1.4  2005/09/28 09:25:07  aitor
 * The OS_TrapEntry routine do not signal any event.
 *
 * Revision 1.3  2005/09/26 11:37:20  aitor
 * The register and restore interrupt operations must be done with the interrupts disabled.
 *
 * Revision 1.2  2005/09/23 14:55:24  aitor
 * Modified all the trap model. Now the traps has an structure to allows the event propagation.
 *
 * Revision 1.1  2005/09/19 11:32:26  aitor
 * Restoring the project.
 *
 * Revision 1.5  2005/07/21 11:04:58  aitor
 * Change the types.h file location.
 *
 * Revision 1.4  2005/07/29 11:27:30  aitor
 * Change the naming convention.
 *
 * Revision 1.3  2005/07/29 11:23:08  aitor
 * Change the naming convention.
 *
 * Revision 1.2  2005/07/28 16:53:53  aitor
 * Error correction in the OS_trap_entry routine.
 *
 * Revision 1.1  2005/07/28 14:26:41  aitor
 * traps.c and traps.h files are now in the independent code part.
 *
 * Revision 1.7  2005/07/27 08:50:12  aitor
 * Delete some unnecesaty header files.
 *
 * Revision 1.6  2005/07/26 08:38:08  aitor
 * Named change.
 *
 * Revision 1.5  2005/07/21 14:03:02  aitor
 * Delete unnecesary variables.
 *
 * Revision 1.4  2005/07/19 07:55:08  aitor
 * Include the <types.h> instead of <types.h>
 *
 * Revision 1.3  2005/07/14 14:45:14  aitor
 * The trap_list array that contains the interrupt vector table now is named
 * trap_table.
 *
 * Revision 1.2  2005/06/23 12:08:37  aitor
 * Nueva estructura de directorios.
 *
 * Revision 1.1  2005/06/23 10:52:35  aitor
 * Nueva estructura de directorios para separar entre CPU y diferentes
 * tarjetas de una misma CPU
 *
 * Revision 1.1  2005/06/21 10:28:15  aitor
 * Alta del mini sistema ERCOS-RT
 *
 */

#include <kernel/traps.h>

#include <kernel/debug.h>
#include <kernel/sched.h>
#include <kernel/time.h>
#include <sys/types.h>
#include <kernel/panic.h>
#include <target.h>

#ifdef CONFIG_ERCOS_TRACER
#include <tracer/tracer.h>
#endif

//----------------------------------------------------------------------------


//----------------------------------------------------------------------------

/**
 * The trap list handler structure
 */
OS_Interrupt_Control_t interrupt_handler[CPU_NUMBER_OF_INTERRUPTS];

//----------------------------------------------------------------------------

/*
 * Function: int OS_RegisterIRQ(unsigned int _vec, void (*handler)(int level))
 * 
 * Register a IRQ interrupt
 */
int OS_RegisterIRQ(unsigned int _vec, void (*handler)(int level))
{
    /*  Never register interrupt handler for the time management    */
    if(IS_TIMER_INTERRUPT(_vec) && interrupt_handler[_vec].isr != NULL) {
        return -1;
    }

    /*  Verifies if the IRQ vector is no longer than 32    */
    if (_vec > CPU_TRAP_MIN_NUMBER && _vec < CPU_TRAP_MAX_NUMBER) {
        interrupt_handler[_vec].old_isr = interrupt_handler[_vec].isr;
        interrupt_handler[_vec].isr = handler;

        /*  It is possible to enable/dissable only the interrupts  */
        if (OS_IsInterrupt(_vec)) {
            OS_EnableInterrupt(_vec);
        }

        return 0;   /*  OK  */
    }

    return -1;  /*  UPS   */
}

/*
 * Function:  int OS_FreeIRQ(int vector,void(*handler)(int level)))
 * 
 * Frees the irq handler interrupt
 */
int OS_FreeIRQ(int vector,void(*handler)(int level))
{
    if (vector > CPU_TRAP_MIN_NUMBER && vector < CPU_TRAP_MAX_NUMBER)
    {
        if (interrupt_handler[vector].isr == handler)
        {
            /*  No interrupt handler    */
            interrupt_handler[vector].isr = NULL;
    
            /*  No previvous interrupt handler  */
            interrupt_handler[vector].old_isr = NULL;        
    
            /*  Is an IRQ - Mask it   */
            if (OS_IsInterrupt(vector))
                OS_MaskInterrupt(vector);
          
            return 0;   /*  OK  */
        }
    }
  
    return -1;  /*  ERROR   */
}

int OS_RestoreIRQ(int vector)
{
    /*  Disable the interrupts while registering the handler */
    OS_DisableIRQ();
    
    if (vector > CPU_TRAP_MIN_NUMBER && vector < CPU_TRAP_MAX_NUMBER)
    {
        if (interrupt_handler[vector].isr != NULL)
        {
            interrupt_handler[vector].isr = interrupt_handler[vector].old_isr;

            interrupt_handler[vector].old_isr = NULL;
                        
            /*  Is an IRQ - Mask it   */
            if (OS_IsInterrupt(vector)) {
                OS_MaskInterrupt(vector);
            }
          
            OS_EnableIRQ();

            return 0;   /*  OK  */
        }
    }
  
    OS_EnableIRQ();

    return -1;  /*  ERROR   */
    
}

/*
 * Function:  void OS_TrapInit(void)
 * 
 * This function initializes the interrupts and traps.
 */
void OS_TrapInit(void)
{
    unsigned int i;
    
    DEBUG ("Initializing the Trap Table...");
    
    /*  Initialize all the interrupt handlers to NULL   */
    for (i = CPU_TRAP_MIN_NUMBER; i < CPU_TRAP_MAX_NUMBER ; ++i) {
        /*  No interrupt handler    */
        interrupt_handler[i].isr = NULL;

        /*  No previvous interrupt handler  */
        interrupt_handler[i].old_isr = NULL;        

    }
    
    /*  Mask all interrupts */
    OS_MaskAllInterrupts();
}

/*
 * Function:  void OS_TrapEntry(int vector, int addr1, int addr2)
 * 
 * Trap entry function.
 */
void OS_TrapEntry(int vector, int addr1, int addr2)
{
    
    if (vector > CPU_TRAP_MIN_NUMBER && vector < CPU_TRAP_MAX_NUMBER) {
        /*  If there is handler interrupt, we execute it    */
        if (interrupt_handler[vector].isr) {

            /*  Execute the interrupt handler   */
            interrupt_handler[vector].isr(vector);
        }

        /*  No handler, kernel panic    */
        else {

            #if CONFIG_DEBUG
            CPU_TraceTraps(vector, addr1, addr2);
            #endif
            
            /*  AIEEE !!! kernel panic  */
            OS_KernelPanic("halt");

        }
        
        /*  
         * If the trap is an IRQ, clear it
         * The watchdog and the irq number 16 is not cleared
         * 
         * By write a "1" in the interrupt clear register, we clear the refenced
         * interrupt
         * We cannot read the interrupt clear register. Only can write it.
         */
        if (OS_IsInterrupt(vector)) {
            OS_ClearInterrupt(vector);
        }
    } 
    /*  if the vector number is ilegal...AIEEE  */
    else {
        OS_KernelPanic("** ILEGAL TRAP **\n");
    }

}

