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
 * $Id: syscall.c 1269 2008-02-25 18:10:23Z cesar $
 *  
 * $Revision: 1269 $
 * $Date: 2008-02-25 19:10:23 +0100 (lun, 25 feb 2008) $
 * Last CVS commit by $Author: cesar $
 * 
 * Registro de cambios
 * 
 * $Log$
 * Revision 1.4  2005/09/28 09:22:48  aitor
 * Added all the system calls needed to manage irq event notification mechanism
 *
 * Revision 1.3  2005/09/27 14:14:03  aitor
 * The new OS_Sem_t semaphores implementation have a queue.
 *
 * Revision 1.2  2005/09/23 14:49:55  aitor
 * SYS_IRQWait() syscall added.
 *
 * Revision 1.1  2005/09/19 11:32:26  aitor
 * Restoring the project.
 *
 * Revision 1.17  2005/09/01 11:33:29  aitor
 * CPU_EnableIRQ and CPU_DisableIRQ are named now as _CPU_EnableIRQ and
 * _CPU_DisableIRQ
 *
 * Revision 1.16  2005/08/25 08:38:27  aitor
 * Error correction in the SYS_SemTimedWait() syscall.
 *
 * Revision 1.15  2005/08/23 17:38:18  aitor
 * SYS_SemTimedWait() syscall implemented.
 *
 * Revision 1.14  2005/08/23 15:01:53  aitor
 * Added the irq event dealing in the OS_SyscallExit routine.
 *
 * Revision 1.13  2005/08/22 15:53:44  aitor
 * The SYS_TaskCreate() syscall produce a scheduling.
 *
 * Revision 1.12  2005/08/19 13:39:32  aitor
 * SYS_TaskCreate() syscall now receives a function argument.
 *
 * Revision 1.11  2005/08/15 12:47:08  aitor
 * comments.
 *
 * Revision 1.10  2005/08/11 19:04:35  aitor
 * All Mutex system calls are implemented.
 * We are also implement priority system calls.
 *
 * Revision 1.9  2005/08/05 11:57:53  aitor
 * Modified the task create functions. Now this functions receives the stack
 * address. In case this address is not initialized, the O.S. gives an address
 * for thread stack.
 *
 * Revision 1.8  2005/08/03 10:37:44  aitor
 * Added the semaphore syscall interface.
 *
 * Revision 1.7  2005/07/21 10:56:22  aitor
 * Added the SYS_SetPriority() and SYS_GetPriority() routines.
 *
 * Revision 1.6  2005/07/29 11:27:30  aitor
 * Change the naming convention.
 *
 * Revision 1.5  2005/07/29 11:23:08  aitor
 * Change the naming convention.
 *
 * Revision 1.4  2005/07/29 10:28:33  aitor
 * Declare static some routines.
 *
 * Revision 1.3  2005/07/29 10:19:11  aitor
 * Change the syscall.h header file location.
 *
 * Revision 1.2  2005/07/29 08:45:40  aitor
 * Delete the cpu/syscall.h header file.
 *
 * Revision 1.1  2005/07/28 16:53:16  aitor
 * Added the syscall interface.
 *
 */

#include <syscall.h>
#include <kernel/task.h>
#include <kernel/sem.h>
#include <kernel/traps.h>
#include <kernel/debug.h>

#include <tracer/ktracer.h>
#include <tracer/systracer.h>

//------------------------------------------------------------------------------

volatile uint32 schedule_active = 0;

//------------------------------------------------------------------------------

/**
 * The execution of this routine is mandatory for all system calls. It sets
 * a flag in order to avoid the scheduling a allow the system call anidation
 */
static void OS_SyscallEntry(void);

/**
 * The execution of this routine is mandatory for all the systems calls. It is
 * executed at the end of the system call to perform the event handling and
 * the scheduling (if it is necessary)
 */
static void OS_SyscallExit(void);

//------------------------------------------------------------------------------

/** This variable notifies the schedule necessity   */
static uint8 need_reesched = 0;

/** 
 * This MACRO sets the need_reesched flag in order to force a scheduling when 
 * the system call returns
 */
#define SET_NEEDREESCHED_FLAG()    \
do {    \
    need_reesched = 1;  \
}while(0);

#define SET_NEEDREESCHED()    \
do {    \
    need_reesched = 1;  \
    OS_SchedCurrentReady();  \
}while(0);

/** 
 * This MACRO clears the need_reesched flag in order to avoid the scheduling 
 * when the system call returns
 */
#define CLEAR_NEEDREESCHED_FLAG()  \
do {    \
    need_reesched = 0;  \
}while(0);

/** This MACRO verifies if a scheduling is needed   */
#define NEED_REESCHED           (need_reesched == 1)

#define SET_SCHEDULE_ACTIVE() \
do {    \
    OS_DisableIRQ();    \
    schedule_active = 1;  \
    OS_EnableIRQ();    \
}while(0);

#define CLEAR_SCHEDULE_ACTIVE() \
do {    \
    schedule_active = 0;  \
}while(0);

//------------------------------------------------------------------------------

static void OS_SyscallEntry(void)
{
    SET_SCHEDULE_ACTIVE();
}

static void OS_SyscallExit(void)
{

    OS_DisableIRQ();

    OS_IRQHandleEvent();
    
    if(NEED_REESCHED) {
        CLEAR_NEEDREESCHED_FLAG();
        
        /*  
         * Schedule. The Schedule must clear the schedule_active flag
         */
        OS_Schedule();
        SYS_Schedule();
    }
    else
        CLEAR_SCHEDULE_ACTIVE();    // Clear the schedule_active flag
        
    OS_EnableIRQ();
}

/******************************************************************************
 * TIME
 *****************************************************************************/

void SYS_Sleep(OS_Time_t _ticks)
{
    OS_Time_t the_ticks;
    int retval;

    OS_SyscallEntry();
    
    the_ticks = (OS_Time_t)OS_TimeGetTicks() + _ticks;
    
    SYSTRACE_TAM24(SYSTRACE_THREAD_SLEEP, (uint32)(the_ticks >> 32), (uint32)the_ticks);
    
    retval = OS_DoSleep(the_ticks);             /*  In time.c   */
    
    if(0 == retval) {
        SET_NEEDREESCHED();
    }else {
        SET_NEEDREESCHED_FLAG();
    }
    
    OS_SyscallExit();
}

void SYS_AbsSleep(OS_Time_t _tick)
{
    OS_SyscallEntry();
    int retval;

    SYSTRACE_TAM24(SYSTRACE_THREAD_SLEEP, (uint32)(_tick >> 32), (uint32)_tick);
    
    retval = OS_DoSleep(_tick);             /*  In time.c   */
    
    if(0 == retval) {
        SET_NEEDREESCHED();
    }else {
        SET_NEEDREESCHED_FLAG();
    }
    
    OS_SyscallExit();
}

void SYS_TimeGetTime(OS_Timeval_t *_system_time)
{
    OS_SyscallEntry();

    OS_TimeGetTime(_system_time);

    SET_NEEDREESCHED();
    
    OS_SyscallExit();
}

/******************************************************************************
 * TASK
 *****************************************************************************/

tid_t SYS_TaskCreate(uint32 *_stackaddr, uint32  _stacksize, void *_ip, void* _arg, uint8 _priority)
{
    sint32 tid;
    
    OS_SyscallEntry();

    SYSTRACE_TAM16(SYSTRACE_THREAD_CREATE, _priority);

    tid = OS_TaskCreate(_stackaddr, _stacksize, _ip, _arg, _priority);    /*  In sched.c  */

    /*  
     * We need to add the current thread to the sched queue in order to
     * reschedule correctly
     */
    SET_NEEDREESCHED();

    OS_SyscallExit();

    return tid;
}

/******************************************************************************
 * PRIORITY
 *****************************************************************************/

void SYS_SetPriority(tid_t _tid, priority_t _priority)
{
    tid_t tid;
    
    OS_SyscallEntry();
    
    SYSTRACE_TAM24(SYSTRACE_SET_PRIORITY, _tid, _priority);

    tid = OS_GetCurrentTid();

    /*
     * The thread is only inserted in the task queue if it is not blocked in 
     * any other queue.
     */
    if(tid != _tid && !OS_TaskIsBlocked(_tid))
    {
        OS_SchedRemoveThread(_tid);
        OS_SetPriority(_tid, _priority);    /*  In sched.c  */
        OS_SchedTaskReady(_tid);
    }
    else
        OS_SetPriority(_tid, _priority);    /*  In sched.c  */

    /*  
     * We need to add the current thread to the sched queue in order to
     * reschedule correctly
     */
    SET_NEEDREESCHED();
    
    OS_SyscallExit();
}

void SYS_SetTmpPriority(tid_t _tid, priority_t _priority)
{
    tid_t tid;
    
    OS_SyscallEntry();
    
    SYSTRACE_TAM24(SYSTRACE_SET_TMP_PRIORITY, _tid, _priority);

    tid = OS_GetCurrentTid();

    /*  If the caller thread changes another thread priority, both the caller
     * (current) thread and the target thread must be inserted in the sched
     * queue.
     * The thread is only inserted in the task queue if it is not blocked in 
     * any other queue.
     */
    if(tid != _tid && !OS_TaskIsBlocked(tid)) {
        /*  Remove the thread from prev sched queue */
        OS_SchedRemoveThread(_tid);
    
        OS_SetTmpPriority(_tid, _priority);    /*  In sched.c  */

        OS_SchedTaskReady(_tid);
    }
    else
        OS_SetTmpPriority(_tid, _priority);    /*  In sched.c  */

    /*  
     * We need to add the current thread to the sched queue in order to
     * reschedule correctly
     */
    SET_NEEDREESCHED();
    
    OS_SyscallExit();
}

priority_t SYS_GetPriority(tid_t _tid)
{
    int retval;
    
    OS_SyscallEntry();
    retval = OS_GetPriority(_tid);    /*  In sched.c  */
    
    SET_NEEDREESCHED();
    
    OS_SyscallExit();
    
    return retval;
}

/******************************************************************************
 * SEMAPHORES
 *****************************************************************************/

int SYS_SemWait(OS_SemID_t _sem)
{
    int retval = 0;
    
    OS_SyscallEntry();

    SYSTRACE_TAM16(SYSTRACE_SEM_WAIT, (uint32)(_sem));
    
    /*  The event is traced in the sem.c source file  */

    retval = OS_SemWait(_sem);    /*  In sem.c  */
    
    if(0 == retval) {
        SET_NEEDREESCHED();
    }else {
        SET_NEEDREESCHED_FLAG();
    }
    
    OS_SyscallExit();
    
    return retval;
}

int SYS_SemTimedWait(OS_SemID_t _sem, OS_Time_t _ticks)
{
    int retval = 0;
    OS_Time_t the_ticks = OS_TimeGetTicks() + _ticks;
    
    OS_SyscallEntry();
    
    SYSTRACE_TAM24(SYSTRACE_SEM_TIMEDWAIT, (uint32)(_sem), the_ticks);
    
    if(0 == OS_DoSleep(the_ticks))
    {
        /*  The task is not blocked */
        SET_NEEDREESCHED();

        /*  The timeout expired */
        retval = -1;
    }else {
        SET_NEEDREESCHED_FLAG();

        if(0 == OS_SemWait(_sem)) {
            /*  The task is blocked in the time queue   */

            SET_NEEDREESCHED();
            OS_TimeRemoveFromQueue(current);
        }
        /*  The task is blocked in the two queues   */
        else {
            /*  The task is blocked in the semaphore and in the time queue  */

            OS_SyscallExit();   // performs a scheduling

            OS_SyscallEntry();
            if(!OS_SemIsCatched(_sem)) {
                retval = -1;
            }

            SET_NEEDREESCHED();
        }
    }
        
    OS_SyscallExit();

    /*  
     * If retval == 0 the semaphore has been catched, else 
     * the timeout has been expired
     */
    return retval;
}

int SYS_SemPost(OS_SemID_t _sem)
{
    int retval = 0;
    
    OS_SyscallEntry();
    
    SYSTRACE_TAM16(SYSTRACE_SEM_POST, (uint32)(_sem));
    
    retval = OS_SemPost(_sem);    /*  In sem.c  */
    
    SET_NEEDREESCHED();
    
    OS_SyscallExit();
    
    return retval;
}

int SYS_SemInit(OS_SemID_t *_sem, uint32 _value)
{
    int retval = 0;
    
    OS_SyscallEntry();

    SYSTRACE_TAM16(SYSTRACE_SEM_INIT, _value);

    retval = OS_SemInit(_sem, _value);    /*  In sem.c  */
    
    SET_NEEDREESCHED();
    
    OS_SyscallExit();
    
    return retval;
}

int SYS_SemTryWait(OS_SemID_t _sem)
{
    int retval;
    
    OS_SyscallEntry();
    
    SYSTRACE_TAM16(SYSTRACE_SEM_TRYWAIT, (uint32)(_sem));
    
    retval = OS_SemTryWait(_sem);       /*  in sem.c    */
    
    SET_NEEDREESCHED();
    
    OS_SyscallExit();
    
    return retval;
}

int SYS_SemDestroy(OS_SemID_t *_sem)
{
    int retval;
    
    OS_SyscallEntry();
    retval = OS_SemDestroy(_sem);       /*  in sem.c    */
    
    SET_NEEDREESCHED();
    
    OS_SyscallExit();
    
    return retval;
}

/******************************************************************************
 * MUTEX
 *****************************************************************************/

int SYS_MutexInit(OS_MutexID_t *_mutex, uint8 _type)
{
    int retval;
    
    OS_SyscallEntry();

    SYSTRACE_TAM16(SYSTRACE_MUTEX_INIT, _type);

    retval = OS_MutexInit(_mutex, _type);  /*  in mutex.c  */
    
    SET_NEEDREESCHED();
    
    OS_SyscallExit();
    
    return retval;
}

void SYS_MutexSetPrioCeiling(OS_MutexID_t *_mutex, int prioceiling)
{

    OS_SyscallEntry();
    
    OS_MutexSetPrioCeiling(*_mutex, prioceiling);
    
    SET_NEEDREESCHED();
    
    OS_SyscallExit();
}

int SYS_MutexDestroy(OS_MutexID_t *_mutex)
{
    int retval;
    
    OS_SyscallEntry();
    retval = OS_MutexDestroy(_mutex);
    
    SET_NEEDREESCHED();
    
    OS_SyscallExit();

    return retval;
}
int SYS_MutexLock(OS_MutexID_t _mutex)
{
    int retval;
    
    OS_SyscallEntry();
    
    SYSTRACE_TAM16(SYSTRACE_MUTEX_LOCK, (uint32)(_mutex));
    
    /*  The event is traced in the mutex.c source file  */
    
    retval = OS_MutexLock(_mutex);          /*  in mutex.c */
    
    if(0 == retval) {
        SET_NEEDREESCHED();
    }else {
        SET_NEEDREESCHED_FLAG();
    }
    
    OS_SyscallExit();
    
    return retval;
}

int SYS_MutexUnLock(OS_MutexID_t _mutex)
{
    int retval;
    
    OS_SyscallEntry();
    
    SYSTRACE_TAM16(SYSTRACE_MUTEX_UNLOCK, (uint32)(_mutex));
    
    retval = OS_MutexUnlock(_mutex);        /*  in mutex.c */
    
    SET_NEEDREESCHED();
    
    OS_SyscallExit();
    
    return retval;
}

int SYS_MutexTryLock(OS_MutexID_t _mutex)
{
    int retval;
    
    OS_SyscallEntry();
    
    SYSTRACE_TAM16(SYSTRACE_MUTEX_TRYLOCK, (uint32)(_mutex));
    
    retval = OS_MutexTryLock(_mutex);
    
    SET_NEEDREESCHED();
    
    OS_SyscallExit();
    
    return retval;
}

/******************************************************************************
 * THREADS
 *****************************************************************************/

int SYS_GetCurrentTid(void)
{
    int retval;

    OS_SyscallEntry();

    retval = OS_GetCurrentTid();    /*  in kernel/sched.c  */

    SET_NEEDREESCHED();

    OS_SyscallExit();
    
    return retval;
}


void SYS_TaskGetStackParameters(uint32 *_stackaddr, uint32 *_stacksize)
{

    OS_SyscallEntry();

    OS_TaskGetStackParameters(_stackaddr, _stacksize); /*  in kernel/task.c  */

    SET_NEEDREESCHED();

    OS_SyscallExit();
    
}

void SYS_ThreadExit(void)
{
    OS_SyscallEntry();
    
    KTRACE_EVENT(KEVENT_THREAD_EXIT);
    
    SYSTRACE_TAM16(SYSTRACE_THREAD_EXIT, 0);
    
    OS_ThreadExit();                    /*  in task.c   */
    
    SET_NEEDREESCHED_FLAG()
    
    OS_SyscallExit();
}

/******************************************************************************
 * IRQ MANAGEMENT
 *****************************************************************************/

void SYS_IntrLock(uint32 _vector)
{

    OS_SyscallEntry();

    OS_MaskInterrupt(_vector);

    SET_NEEDREESCHED();

    OS_SyscallExit();
    
}

void SYS_IntrUnlock(uint32 _vector)
{

    OS_SyscallEntry();

    CPU_EnableInterrupt(_vector);

    SET_NEEDREESCHED();

    OS_SyscallExit();
    
}

int SYS_RegisterIRQ(uint32 _vector, void (*handler)(int level))
{
    int retval;
    
    OS_SyscallEntry();

    retval = OS_RegisterIRQ(_vector, handler);

    CLEAR_NEEDREESCHED_FLAG();

    OS_SyscallExit();
    
    return retval;
}

int SYS_RestoreIRQ(uint32 _vector)
{
    int retval;
    
    OS_SyscallEntry();

    retval = OS_RestoreIRQ(_vector);

    SET_NEEDREESCHED();

    OS_SyscallExit();
    
    return retval;
}

void SYS_ForceIRQ(uint32 _vector)
{

    OS_SyscallEntry();

    OS_ForceInterrupt(_vector);

    CLEAR_NEEDREESCHED_FLAG();

    OS_SyscallExit();
    
}

/******************************************************************************
 * EVENTS
 *****************************************************************************/

void SYS_IRQMarkEvent(OS_SemID_t _sem)
{

    OS_SyscallEntry();
    
    SYSTRACE_TAM16(SYSTRACE_IRQ_EVENT, (uint32)(_sem));

    OS_IRQMarkEvent(_sem);

    CLEAR_NEEDREESCHED_FLAG();

    OS_SyscallExit();

}

/******************************************************************************
 * TRACER
 *****************************************************************************/

#ifdef CONFIG_ERCOS_TRACER

void SYS_TracerSaveEvent(char *_data, unsigned int _data_size)
{
    
    OS_SyscallEntry();

    OS_TracerSaveEvent(_data, _data_size);    /*  in kernel/tracer.c  */
    
    SET_NEEDREESCHED();
    
    OS_SyscallExit();
    
}


int SYS_TracerStart(void)
{
    OS_SyscallEntry();
    OS_TracerStart();    /*  in kernel/tracer.c  */
    OS_SyscallExit();
    
    return 0;
}


#endif



