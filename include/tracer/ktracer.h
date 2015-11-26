/*
    Copyright 2005-2006 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/

#ifndef _KTRACER_H_
#define _KTRACER_H_

#include <tracer/tracer.h>
#include <kernel/time.h>

//------------------------------------------------------------------------------

typedef uint8   kevent_id_t;

/**
 * This structure allows to trace the kernel events
 */
struct ktracer_events {
    
    /** Trace event type idenfication   */    
    uint32 kevent_id;
     
    /** Event generation time   */
    OS_Time_t      timestamp;
    
    /** Id of the event generator thread    */
    tid_t       thread_id;

}__attribute__((packed));

typedef struct ktracer_events ktracer_events_t;


#ifdef CONFIG_ERCOS_KERNEL_TRACER

    #define KTRACE_EVENT(kevent_id)  OS_KTracerEvent(kevent_id);

    #define KTRACE_EVENT_TID(kevent_id, tid)  OS_KTracerEventTID(kevent_id,tid);

    #define KTRACE_EVENT_SEM(kevent_id, semid)  \
                        OS_KTracerEventSem(kevent_id,semid);

    #define KTRACE_EVENT_SEM_TID(kevent_id, semid, tid)  \
                        OS_KTracerEventSemTID(kevent_id,semid, tid);

    /** This array stores all the kernel events information */
    static ktracer_events_t kevents;

#else

    #define KTRACE_EVENT(kevent_id) 
    
    #define KTRACE_EVENT_TID(kevent_id, tid)

    #define KTRACE_EVENT_SEM(kevent_id, semid)

    #define KTRACE_EVENT_SEM_TID(kevent_id, semid, tid)

#endif  //#ifdef CONFIG_ERCOS_KERNEL_TRACER

#define KEVENT_THREAD_CREATE    0
#define KEVENT_LOCK_CATCHED     1
#define KEVENT_LOCK_BLOCKED     2
#define KEVENT_UNLOCK           3
#define KEVENT_THREAD_SLEEP     4
#define KEVENT_THREAD_EXIT      5
#define KEVENT_SCHEDULE_IN      6
#define KEVENT_SCHEDULE_OUT     7
#define KEVENT_MAX_EVENTS       8

//------------------------------------------------------------------------------



//------------------------------------------------------------------------------

#define OS_KTracerFillEventTime(_kevent_id) \
do {    \
    kevents.kevent_id = _kevent_id;    \
    kevents.timestamp = OS_TimeGetTicks();    \
}while(0);    \
    
/**
 * This macro trace an event for the current thread.
 * 
 * @param _kevent_id:   This is the event that must be traced
 */
#define OS_KTracerEvent(_kevent_id)    \
do {    \
    OS_KTracerFillEventTime(_kevent_id); \
    kevents.thread_id = OS_GetCurrentTid();    \
        \
    OS_TracerSaveEvent((char*)&kevents, sizeof(kevents));    \
}while(0);    \

/**
 * This macro trace an event for a certain thread identifier.
 * 
 * @param _kevent_id:   This is the event that must be traced
 * @param tid:          This is the thread identifier.
 */
#define OS_KTracerEventTID(_kevent_id, tid)   \
do {   \
    OS_KTracerFillEventTime(_kevent_id); \
    kevents.thread_id = tid;   \
       \
    OS_TracerSaveEvent((char*)&kevents, sizeof(kevents));   \
}while(0);   \

/**
 * This macro trace a semaphore event.
 * 
 * @param _kevent_id:   This is the event that must be traced
 * @param _sem_id:      This is the semaphore identifier over which the 
 * operation is performed.
 */
#define OS_KTracerEventSem(_kevent_id, _sem_id)   \
do {   \
    OS_KTracerFillEventTime(_kevent_id); \
    kevents.thread_id = OS_GetCurrentTid() | ((_sem_id << 16) & 0xFFFF0000);   \
       \
    OS_TracerSaveEvent((char*)&kevents, sizeof(kevents));   \
}while(0);   \

/**
 * This macro trace a semaphore event for a certain thread.
 * 
 * @param _kevent_id:   This is the event that must be traced
 * @param _sem_id:      This is the semaphore identifier over which the 
 * operation is performed.
 * @param _tid:         This is the thread identifier
 */
#define OS_KTracerEventSemTID(_kevent_id, _sem_id, _tid)   \
do {   \
    OS_KTracerFillEventTime(_kevent_id); \
    kevents.thread_id = _tid | ((_sem_id << 16) & 0xFFFF0000);;   \
       \
    OS_TracerSaveEvent((char*)&kevents, sizeof(kevents));   \
}while(0);   \

//------------------------------------------------------------------------------

#endif // #ifndef _KTRACER_H_

