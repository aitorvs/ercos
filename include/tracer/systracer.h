/*
    Copyright 2005-2006 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/

#ifndef _SYSTRACER_H_
#define _SYSTRACER_H_

#include <tracer/tracer.h>
#include <kernel/time.h>

//------------------------------------------------------------------------------

/**
 * This structure allows to trace the kernel events
 */
struct systracer_events16 {
    
    /** Traced system call identifier   */
    size_t      sys_id;
     
    /** System call timestamp   */
    OS_Time_t   timestamp;
    
    /** System call argument    */
    size_t      arg;

}__attribute__((packed));

typedef struct systracer_events16 systracer_events16_t;

struct systracer_events24 {
    
    /** Traced system call identifier   */
    size_t      sys_id;
     
    /** System call timestamp   */
    OS_Time_t   timestamp;
    
    /** Argument one    */
    size_t      arg1;

    /** Argument two    */
    size_t      arg2;

}__attribute__((packed));

typedef struct systracer_events24 systracer_events24_t;

#ifdef CONFIG_ERCOS_SYSCALL_TRACER

    #define SYSTRACE_TAM16(sysid, arg)  OS_SYSTracerEvent16(sysid, arg);

    #define SYSTRACE_TAM24(sysid, arg1, arg2)  OS_SYSTracerEvent24(sysid, arg1, arg2);

    /** This array stores all the kernel events information */
    static systracer_events16_t sys_events16;
    static systracer_events24_t sys_events24;

#else

    #define SYSTRACE_TAM16(sysid, arg)
    #define SYSTRACE_TAM24(sysid, arg1, arg2)
    
#endif   // #ifdef CONFIG_ERCOS_SYSCALL_TRACER

//------------------------------------------------------------------------------

/**
 * All of this definitions are the traced system calls
 */
#define SYSTRACE_THREAD_SLEEP       0
#define SYSTRACE_THREAD_CREATE      1
#define SYSTRACE_SEM_WAIT           2
#define SYSTRACE_SEM_POST           3
#define SYSTRACE_SEM_TRYWAIT        4
#define SYSTRACE_MUTEX_LOCK         5
#define SYSTRACE_MUTEX_UNLOCK       6
#define SYSTRACE_MUTEX_TRYLOCK      7
#define SYSTRACE_THREAD_EXIT        8
#define SYSTRACE_IRQ_EVENT          9
#define SYSTRACE_CLOCK_EVENT        10
#define SYSTRACE_HANDLE_EVENT       11
#define SYSTRACE_SEM_INIT           12
#define SYSTRACE_MUTEX_INIT         13
#define SYSTRACE_SCHEDULE           14

/** This system calls needs 24 bytes to be traced   */
#define SYSTRACE_SET_PRIORITY       101
#define SYSTRACE_SET_TMP_PRIORITY   102
#define SYSTRACE_SEM_TIMEDWAIT      103

//------------------------------------------------------------------------------

/**
 * This macro fills the sys event structure with the syscal identifier and the
 * current time
 * 
 * @param _sys_events:  This attribute is the sys_events associated structure
 * @param _sys_id:      This attribute is the syscall ID
 */
#define OS_SYSTracerFillEventTime(_sys_events, _sys_id) \
do {    \
    _sys_events.sys_id = _sys_id;    \
    _sys_events.timestamp = OS_TimeGetTicks();    \
}while(0);    \
    
/**
 * This macro trace a system call with 16 bytes length
 * 
 * @param _sys_id:  This parameter is the system call identifier
 * @param _arg:     This parameter is the system call argument
 */
#define OS_SYSTracerEvent16(_sys_id, _arg)    \
do {    \
    OS_SYSTracerFillEventTime(sys_events16, _sys_id); \
    sys_events16.arg = _arg;    \
        \
    OS_TracerSaveEvent((char*)&sys_events16, sizeof(sys_events16));    \
}while(0);    \

/**
 * This macro trace a system call with 24 bytes length
 * 
 * @param _sys_id:  This parameter is the system call identifier
 * @param _arg1:    This parameter is the system call argument one
 * @param _arg1:    This parameter is the system call argument two
 */
#define OS_SYSTracerEvent24(_sys_id, _arg1, _arg2)    \
do {    \
    OS_SYSTracerFillEventTime(sys_events24, _sys_id); \
    sys_events24.arg1 = _arg1;    \
    sys_events24.arg2 = _arg2;    \
        \
    OS_TracerSaveEvent((char*)&sys_events24, sizeof(sys_events24));    \
}while(0);    \


//------------------------------------------------------------------------------

#endif // #ifndef _SYSTRACER_H_

