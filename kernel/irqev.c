/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/

/*
 * File:    $Source$
 * 
 * $Id:irqev.c 240 2005-12-14 01:03:49 +0100 (mié, 14 dic 2005) aitor $
 *  
 * $Revision:240 $
 * $Date:2005-12-14 01:03:49 +0100 (mié, 14 dic 2005) $
 * Last CVS commit by $Author:aitor $
 * 
 * Registro de cambios
 * 
 * $Log$
 * Revision 1.7  2005/09/28 14:45:56  aitor
 * The OS_Interrupt_Control_t structure does not have any queue now. We use
 * the semaphore queue to manage the irq events.
 *
 * Revision 1.6  2005/09/28 09:25:32  aitor
 * Stable event implementation is done.
 *
 * Revision 1.5  2005/09/26 18:46:12  aitor
 * There is not necessary to use a semaphore to block the tasks waiting for an interrupt event
 *
 * Revision 1.4  2005/09/26 11:48:42  aitor
 * An error is been corrected in the OS_IRQWait() routine.
 *
 * Revision 1.3  2005/09/26 11:36:08  aitor
 * The routine OS_IRQMarkEvent() is been modified to treat all events. We also modified the
 * OS_IRQWait() routine to ensure the waiting only for interrupt events.
 *
 * Revision 1.2  2005/09/23 14:50:59  aitor
 * Added the OS_IRQWait() routine.
 * We also correct some errors in the event handler method.
 *
 * Revision 1.1  2005/09/19 11:32:26  aitor
 * Restoring the project.
 *
 * Revision 1.2  2005/09/01 11:31:15  aitor
 * The general purpose timer is now a periodic timer. The real-time clock is the one shot
 * system clock.
 *
 * Revision 1.1  2005/08/23 14:59:51  aitor
 * Added the IRQ event dealing.
 *
 */

#include <kernel/irqev.h>

#include <target.h>
#include <kernel/sem.h>
#include <kernel/list.h>
#include <kernel/sched.h>
#include <kernel/task.h>
#include <tracer/systracer.h>

//------------------------------------------------------------------------------

/** 
 * This flag is modified when an event marked. It is used in the IRQ epilogue 
 */
static uint32 event_pending = 0;

/** This list maintains the pending events  */
static OS_List_t event_list;

/**  This flag is necessary to notify a clock event  */
static uint8 clock_event = 0;

/** This macro increments the event_pending flag  */
#define INCREASE_PENDING_EVENTS() (event_pending++)

/** This macro decrease the event_pending flag  */
#define DECREASE_PENDING_EVENTS() (event_pending--)

/** This macro clears the event_pending flag  */
#define CLEAR_PENDING_EVENTS() (event_pending = 0)

/** This macro verifies that the event_pending flag is set */
#define IS_EVENT_PENDING() (0 < event_pending)

//------------------------------------------------------------------------------

extern OS_Interrupt_Control_t interrupt_handler[CPU_NUMBER_OF_INTERRUPTS];

extern OS_Sem_t semaphores[KCONFIG_MAX_BIN_SEMAPHORES];

#define SEM(_id_sem)    (&(semaphores[_id_sem]))

//------------------------------------------------------------------------------

/**
 * This routine carries out all the semaphore post operation over the IRQ 
 * semaphores. 
 * 
 * @param _sem: This parameter is the semaphore that must be signaled.
 * 
 * When a thread is blocked waitting for an IRQ it is blocked in
 * an event semaphore. The IRQ handler must signal the event semaphore but this
 * operation is not performed, only is marked. All of this operations are 
 * performed later when the OS_IRQHandlerEvent routine is executed, and the 
 * OS_IRQSemPost routine perform all the signal operations.
 */
static void OS_IRQSemPost(OS_Sem_t *_sem);

//------------------------------------------------------------------------------


void OS_IRQInitEvents(void)
{
    
    OS_ListReset(&event_list);
    
}

void OS_IRQHandleEvent(void)
{
    OS_Sem_t *sem;
    OS_Node_t *pos;
    
    if(IS_EVENT_PENDING())
    {
    
        SYSTRACE_TAM16(SYSTRACE_HANDLE_EVENT, 0);
        
        list_for_each(pos, &event_list)
        {
            sem = GET_ELEMENT(OS_Sem_t, event_list.first, node);
    
            DECREASE_PENDING_EVENTS();

            OS_EnableIRQ();
            OS_IRQSemPost(sem);
            OS_DisableIRQ();
    
        }
        /*  The event list must be reset    */
        OS_ListReset(&event_list);
        
        /*  
         * We clear the clock_event flag.
         * We use the while instead of the if sentence because it is possible
         * that another clock events was triggered when the OS_ClockBottomISR()
         * routine programs the next timer.
         */
        while(clock_event) {
            clock_event = 0;
    
            DECREASE_PENDING_EVENTS();

            OS_ClockBottomISR();

        }
    }
    
    CLEAR_PENDING_EVENTS();

}



void OS_IRQMarkClockEvent(void)
{
    /*
     * This routine must be executed in mutual exclusion with the INTERRUPTS 
     * DISABLED
     */

    /*  We notify the clock event   */
    clock_event = 1;

    /*  Signal the event to ensure the correct re-scheduling   */
    INCREASE_PENDING_EVENTS();
}

void OS_IRQMarkEvent(OS_SemID_t _sem)
{
    OS_SemID_t id_sem = _sem;
    
    OS_DisableIRQ();
    
    /*  Signal the event to ensure the correct re-scheduling   */
    INCREASE_PENDING_EVENTS();

    OS_ListAppend(&event_list, &(SEM(id_sem)->node));

    OS_EnableIRQ();
        
}

static void OS_IRQSemPost(OS_Sem_t *_sem)
{
    OS_task_t *task;

    if(OS_ListGetNum(&(_sem->list)) != 0)
    {

        /*  Outs the task from the semaphore queue  */
        task = GET_ELEMENT(OS_task_t, OS_ListGetNode(&(_sem->list)), resource_node);

        /*  Store what is the blocked thread in the semaphore structure */
        _sem->tid_in = task->tid;
        
        /*  Add the blocked task to the ready task queue    */
        OS_SchedTaskReady(task->tid);
    }
    else {
        /*  Increment the semaphore counter */
        _sem->sem_value = 1;
    }
}

