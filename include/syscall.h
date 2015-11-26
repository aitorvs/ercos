/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/

/*!
 * \file include/syscall.h
 * \brief Functions of the syscall layer of ERCOS-RT.
 */

#ifndef _SYSCALL_H_
#define _SYSCALL_H_

#ifdef __cplusplus
extern "C" {
#endif


/*  C Code  */
#ifndef ASM

    #include <sys/types.h>
    #include <kernel/sched.h>
    #include <kernel/time.h>
    #include <kernel/mutex.h>
    #include <kernel/irqev.h>

#ifdef CONFIG_ERCOS_TRACER

    #include <tracer/tracer.h>

#endif  //#ifdef CONFIG_ERCOS_TRACER

#endif  //#ifndef ASM

/** \brief Schedule syscall number. */
#define _NR_schedule        0
/** \brief Disable IRQ syscall number. */
#define _NR_disable_irq     3
/** \brief Enable IRQ syscall number. */
#define _NR_enable_irq      4


//------------------------------------------------------------------------------

/*  C Code  */
#ifndef ASM

extern void _SYS_Schedule(void);

/** brief Forced re-schedule syscall. */
#define SYS_Schedule() \
    do { \
        _SYS_Schedule(); \
    }while(0);


//------------------------------------------------------------------------------
    
/**
 * \brief Relative sleep syscall.
 * 
 * \param _ticks  The number of ticks to sleep.
 * 
 * The thread blocks itself for a number of clock ticks.
 */
void SYS_Sleep(OS_Time_t _ticks);

/**
 * \brief Absolute sleep syscall.
 * 
 * \param _ticks  The tick until which the thread requests to sleep.
 * 
 * The thread gets blocked until the time reach the requested tick. If the tick
 * has already passed, the function will return immediately.
 */
void SYS_AbsSleep(OS_Time_t _tick);

/**
 * \todo Documentar esta funci�n
 */
void SYS_TimeGetTime(OS_Timeval_t *_system_time);

/******************************************************************************
 * THREADS
 *****************************************************************************/

/**
 * \brief Creation of a new thread.
 * 
 * \param _stackaddr  This parameter is the thread stack address.
 * \param _stacksize  This parameter is the thread stack size.
 * \param _ip         This parameter is the instruction pointer.
 * \param _arg        This parameter is the thread argument to be passed to
 *                    routine pointed by _ip
 * \param _priority   This parameter is the thread priority.
 * \return            The system call returns the thread identifier on success
 *                    and -1 on error.
 */
tid_t SYS_TaskCreate(uint32 *_stackaddr, uint32  _stacksize, void *_ip, void *_arg, uint8 _priority);

/**
 * \brief Sets the priority of a given thread.
 * 
 * \param _tid       The thread identifier.
 * \param _priority  The new thread priority.
 */
void SYS_SetPriority(tid_t _tid, priority_t _priority);

/**
 * \brief Sets the temporal priority of a given thread.
 * 
 * \param _tid       This parameter is the thread identifier.
 * \param _priority  This paramter is the new temporal priority.
 */
void SYS_SetTmpPriority(tid_t _tid, priority_t _priority);

/**
 * \brief Returns the priority of a given thread.
 * 
 * \param _tid   The thread identifier from which the priority must be
 *               returned.
 * \return       The thread priority.
 */
priority_t SYS_GetPriority(tid_t _tid);

/**
 * \brief Exits a thread.
 * 
 * The thread that makes this call terminates its execution.
 */
void SYS_ThreadExit(void);

/**
 * \brief Returns the thread identifier of the current thread
 * 
 * \return  The thread identifier of the current thread.
 */
int SYS_GetCurrentTid(void);

/**
 * \brief Returns the stack parameters of the caller thread.
 * 
 * \param _stackaddr  In this parameter the stack address of the current
 *                    thread is stored.
 * \param _stacksize  In this parameter the stacksize of the current thread
 *                    is stored.
 */
void SYS_TaskGetStackParameters(uint32 *_stackaddr, uint32 *_stacksize);

/******************************************************************************
 * SEMAPHORES
 *****************************************************************************/
 
/**
 * \brief Performs a catch (lock) of a given a semaphore.
 * 
 * \param sem  The sempahore that must be catched.
 * \return     Zero if the semaphore was free, -1 if the thread was blocked
 *             previous to the catch.
 */
int SYS_SemWait(OS_SemID_t sem);

/**
 * \brief Frees a semaphore.
 * 
 * \param sem  The sempahore that must be freed.
 * \return     Zero in any case.
 */
int SYS_SemPost(OS_SemID_t sem);

/**
 * \brief Initializes a semaphore.
 * 
 * \param _sem    Reference to the sempahore that must be initialized.
 * \param _value  The initial value of the semaphore.
 * \return        Zero in any case.
 */
int SYS_SemInit(OS_SemID_t *_sem, uint32 _value);

/**
 * \brief Non-blocking version of \a SYS_SemWait().
 * 
 * \param _sem  This parameter is the semaphore that must be catched.
 * \return      The system call returns 0 on success and -1 if the semaphore
 *              was not catched.
 */
int SYS_SemTryWait(OS_SemID_t _sem);

/**
 * \brief Performs a catch operation over a semaphore for a given timeout.
 * 
 * \param _sem    The semaphore that must be catched.
 * \param _ticks  The number of ticks the thread waits to catch the semaphore.
 * \return        The system call returns 0 on success and -1 if the timeout
 *                expired.
 */
int SYS_SemTimedWait(OS_SemID_t _sem, OS_Time_t _ticks);

/**
 * \brief Destroys the structures associated with a given semaphore.
 * 
 * \param _sem  This is the semaphore that must be freed.
 * \return      There is no return value defined.
 */
int SYS_SemDestroy(OS_SemID_t *_sem);

/******************************************************************************
 * MUTEX
 *****************************************************************************/

/**
 * \brief Performs a lock of a given mutex semaphore.
 * 
 * \param _mutex  The sempahore that must be catched. 
 * \return        Zero if the semaphore was free, -1 if the thread was blocked
 *                previous to the catch.
 */
int SYS_MutexLock(OS_MutexID_t _mutex);

/**
 * \brief Performs an unlock operation of a given mutex semaphore.
 * 
 * \param _mutex  This parameter is the sempahore that must be freed.
 * \return        Zero in any case.
 */
int SYS_MutexUnLock(OS_MutexID_t _mutex);

/**
 * \brief Initializes a mutex semaphore.
 * 
 * \param _mutex  Reference to the sempahore that must be initialized.
 * \param _type   The mutex semaphore type.
 * \return        Always returns zero.
 */
int SYS_MutexInit(OS_MutexID_t *_mutex, uint8 _type);

/**
 * \brief Sets the priority ceil of a given mutex semaphore.
 * 
 * \param _mutex       Reference to the mutex semaphore.
 * \param prioceiling  New value of the priority ceil.
 */
void SYS_MutexSetPrioCeiling(OS_MutexID_t *_mutex, int prioceiling);

/**
 * \brief Destroys the structures associated with a given mutex semaphore.
 * 
 * \param _mutex  The semaphore that must be freed.
 * \return        The returned value is undefined.
 */
int SYS_MutexDestroy(OS_MutexID_t *_mutex);

/**
 * \brief Non-blocking version of \a SYS_MutexLock().
 * 
 * \param _mutex  The semaphore that must be catched.
 * \return        Returns 0 on success and -1 if the semaphore was not catched.
 */
int SYS_MutexTryLock(OS_MutexID_t _mutex);

/**
 * \brief Saves an event into the stream buffer.
 * 
 * \param _data       Pointer to the data of the event.
 * \param _data_size  Size of the event data.
 * \return            Returns 0 on sucess and -1 if the event was not be saved
 *                    correctly
 */
void SYS_TracerSaveEvent(char *_data, unsigned int _data_size);

/******************************************************************************
 * EVENTS
 *****************************************************************************/

/**
 * \brief Marks the occurrence of an IRQ.
 * 
 * \param _sem  This parameter is the semaphore corresponding to the event
 *              that has occurred.
 */

void SYS_IRQMarkEvent(OS_SemID_t _sem);

/******************************************************************************
 * IRQ MANAGEMENT
 *****************************************************************************/

/**
 * \brief Disables an interrupt.
 * 
 * \param _vector  The vector number of the interrupt that must be disabled.
 */
void SYS_IntrLock(uint32 _vector);

/**
 * \brief Enables an interrupt.
 * 
 * \param _vector  The vector number of the interrupt that must be enabled.
 */
void SYS_IntrUnlock(uint32 _vector);

/**
 * \brief Installs an interrupt handler for a certain interrupt.
 * 
 * \param _vector  The vector number of the interrupt.
 * \param handler  The pointer to the interrupt service routine that mut be
 *                 installed.
 * \return         Returns 0 on success and -1 in any other case.
 */
int SYS_RegisterIRQ(uint32 _vector, void (*handler)(int level));

/**
 * \brief Restores the previous interrupt service routine of a certain
 *        interrupt.
 * 
 * \param _vector  The vector number of the interrupt.
 * \return         Returns 0 on success.
 */
int SYS_RestoreIRQ(uint32 _vector);

/**
 * \brief Forces an interrupt specified as parameter.
 * 
 * \param _vector  The vector number of the interrupt.
 */
void SYS_ForceIRQ(uint32 _vector);

#endif  //ASM

#ifdef __cplusplus
}
#endif


#endif

