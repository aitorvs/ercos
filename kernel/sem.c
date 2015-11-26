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
 * $Id: sem.c 1263 2008-02-03 18:00:14Z aitorviana $
 *  
 * $Revision: 1263 $
 * $Date: 2008-02-03 19:00:14 +0100 (dom, 03 feb 2008) $
 * Last CVS commit by $Author: aitorviana $
 * 
 * Registro de cambios
 * 
 * $Log$
 * Revision 1.6  2005/09/28 09:23:45  aitor
 * Correct some erros and implement a real binary semaphore.
 *
 * Revision 1.5  2005/09/27 14:14:03  aitor
 * The new OS_Sem_t semaphores implementation have a queue.
 *
 * Revision 1.4  2005/09/26 18:46:12  aitor
 * There is not necessary to use a semaphore to block the tasks waiting for an interrupt event
 *
 * Revision 1.3  2005/09/26 11:54:34  aitor
 * Corrected the error in the OS_SemIntrPost() routine.
 *
 * Revision 1.2  2005/09/23 14:51:44  aitor
 * We modified some errors in the SemWait() method.
 * We also add the OS_SemIntrWait() and OS_SemIntrPost() routines.
 *
 * Revision 1.1  2005/09/19 11:32:26  aitor
 * Restoring the project.
 *
 * Revision 1.5  2005/08/19 13:39:02  aitor
 * Correct some errors.
 *
 * Revision 1.4  2005/08/15 12:44:46  aitor
 * Correct some errors.
 *
 * Revision 1.3  2005/08/08 15:30:23  aitor
 * Delete the function OS_InitSemStrcuts(). This function is not necessary.
 *
 * Revision 1.2  2005/08/03 10:40:21  aitor
 * Comments.
 *
 * Revision 1.1  2005/07/21 10:56:12  aitor
 * Added the semaphore interface.
 *
 */

#include <kernel/sem.h>

#include <syscall.h>
#include <kernel/list.h>
#include <kernel/task.h>
#include <kernel/debug.h>

//#ifdef CONFIG_ERCOS_TRACER
    #include <tracer/ktracer.h>
//#endif


//------------------------------------------------------------------------------

static uint8 sem_id = 1;

/** This is the maximum number of binary semaphores used in the system  */
OS_Sem_t semaphores[KCONFIG_MAX_BIN_SEMAPHORES];

#define GET_NEXT_SEM_ID(__sem_id)                    \
do {                                                 \
    __sem_id = sem_id;                                \
    sem_id++;                                         \
    ASSERT (sem_id < KCONFIG_MAX_BIN_SEMAPHORES);    \
} while(0);

#define SEM(_id_sem)    (&(semaphores[_id_sem]))

//------------------------------------------------------------------------------

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  OS_SemInit
 *  Description:  This function initializes a binary semaphore. The parameters
 *  taken from the function is the pointer to the binary semaphore structure and
 *  the initial value. The value only can be 0 (catched) or 1 (free).
 * =====================================================================================
 */
int OS_SemInit(OS_SemID_t *_sem, uint32 _value)
{
    OS_SemID_t id_sem;
    
    GET_NEXT_SEM_ID(id_sem);

    SEM(id_sem)->sem_id = id_sem;
    
    if(_value <= 0)
        SEM(id_sem)->sem_value = 0;    /*  Binary semaphore    */
    else 
        SEM(id_sem)->sem_value = 1;    /*  Counter semaphore = 1 */
        
    SEM(id_sem)->tid_in = 0;
    
    OS_ListReset(&(SEM(id_sem)->list));
    
    *_sem = id_sem;
    
    return 0;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  OS_SemDestroy
 *  Description:  Destroys a binary semaphore and resets the associated list.
 * =====================================================================================
 */
int OS_SemDestroy(OS_SemID_t *_sem)
{
    OS_SemID_t id_sem = *_sem;
    
    _sem = NULL;
    OS_ListReset(&(SEM(id_sem)->list));
    
    return 0;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  OS_SemPost
 *  Description:  This functions release the semaphore and wakes up the task
 *  being blocked on it. A task can be also be waiting for a time event, so this
 *  function vefiries whether the relased task is also waiting for a time event
 *  or not. In case it is, the task is also removed from the time queue.
 * =====================================================================================
 */
int OS_SemPost(OS_SemID_t _sem)
{
    OS_task_t *task;
    OS_Node_t *n;
    OS_SemID_t id_sem = _sem;

    KTRACE_EVENT_SEM(KEVENT_UNLOCK, SEM(id_sem)->sem_id);
    
    if(OS_ListGetNum(&(SEM(id_sem)->list)) != 0)
    {

        /*  Outs the task from the semaphore queue  */
        n = OS_ListGetNode(&(SEM(id_sem)->list));
        task = GET_ELEMENT(OS_task_t, n, resource_node);

        /*  Store what is the blocked thread in the semaphore structure */
        SEM(id_sem)->tid_in = task->tid;
        
        KTRACE_EVENT_SEM_TID(KEVENT_LOCK_CATCHED, SEM(id_sem)->sem_id, task->tid);
        
        /*  
         * Verifies if the thread is blocked in the time queue
         * due to a TimedWait system call
         */
        if(task->status | TS_TIME_BLOCKED) {
            OS_TimeRemoveFromQueue(task);
        }

        /*  Reset the wait_sem value    */    
        task->wait_sem = 0;

        /*  Add the blocked task to the ready task queue    */
        OS_SchedTaskReady(task->tid);
    }
    else {
        /*  Increment the semaphore counter */
        SEM(id_sem)->sem_value = 1;
        OS_ListReset(&(SEM(id_sem)->list));
    }
    
    return 0;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  OS_SemWait
 *  Description:  This function tries to catch a binary semaphore. The calling
 *  task may get blocked in the semaphore queue whether the semaphore is already
 *  taken from another task. If the semaphore was not taken, it is signaled as
 *  taken and the calling task continues the execution.
 * =====================================================================================
 */
int OS_SemWait(OS_SemID_t _sem)
{
    OS_SemID_t id_sem = _sem;
    
    if(SEM(id_sem)->sem_value == 0) {

        current->status |= TS_SEM_BLOCKED;   /*  Block the caller thread */
        
        /** Trace the event in case of need */
        KTRACE_EVENT_SEM(KEVENT_LOCK_BLOCKED, SEM(id_sem)->sem_id);
        
        /*  
         * The sem_wait field stores the semaphore identifier for which the
         * thread is waitting.
         */
        current->wait_sem = _sem;
        
        OS_ListAppend(&(SEM(id_sem)->list), &(current->resource_node));
        
        return -1;
    }
    else {
        
        /** Trace the event in case of need */
        KTRACE_EVENT_SEM(KEVENT_LOCK_CATCHED, SEM(id_sem)->sem_id);

        /*  Lock the semaphore  */
        SEM(id_sem)->sem_value = 0;
        
        /*  Store what is the blocked thread in the semaphore structure */
        SEM(id_sem)->tid_in = current->tid;
        
        /*  The thread must be added to the ready queue */
        return 0;
    }

}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  OS_SemTryWait
 *  Description:  This function tries to catch a binary semaphore. If the
 *  semaphore was not taken before, the calling task catchs it and continue the
 *  execution. If the semaphore is already catched, the function returns an
 *  error but the calling task is not blocked at all.
 * =====================================================================================
 */
int OS_SemTryWait(OS_SemID_t _sem)
{
    int retval = 0;
    OS_SemID_t id_sem = _sem;
        
    /*  Semaphore catched  */
    if (SEM(id_sem)->sem_value == 1) {

        KTRACE_EVENT_SEM(KEVENT_LOCK_CATCHED, SEM(id_sem)->sem_id);

        SEM(id_sem)->sem_value = 0;
        SEM(id_sem)->tid_in = current->tid;
    }
    /*  Semaphore not catched   */
    else
        retval = -1;

    return retval;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  OS_SemIsCatched
 *  Description:  This function returns whether the semaphore is catched or not.
 * =====================================================================================
 */
int OS_SemIsCatched(OS_SemID_t _sem)
{
    OS_SemID_t id_sem = _sem;

    return (SEM(id_sem)->tid_in == current->tid);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  OS_SemExtractThreadFromList
 *  Description:  This function extracts the given task from the given semaphore
 *  list. This is done because a task may be waiting for both a time event and a
 *  semaphore to be released. If the time event is raised, the task must be
 *  taken out from the semaphore list.
 * =====================================================================================
 */
void OS_SemExtracThreadFromList(OS_SemID_t _sem, OS_task_t *task)
{
    OS_SemID_t id_sem = _sem;

    /*  Delete the semaphore blocked status */
    task->status &= ~(TS_SEM_BLOCKED);
    
    /*  Extract the thread from the semaphore queue */
    OS_ListExtract(&(SEM(id_sem)->list), &(task->resource_node));
    
    /*  Reset the wait_sem value    */    
    task->wait_sem = 0;
}

