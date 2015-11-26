/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT on ERC-32.
*/

/*!
 * \file include/sys/types.h
 * \brief Types definition of the kernel and POSIX layers.
 */

#ifndef _TYPES_H_
#define _TYPES_H_

#include <kernel/config.h>
#include <cpu.h>

//------------------------------------------------------------------------------

/**********************
 * ERCOS-RT data types
 *********************/

/** \brief Thread identifier type  */
typedef uint32 tid_t;

/** \brief Thread priority type    */
typedef uint8 priority_t;

/** \brief Used for sizes of objects. */
typedef uint32 size_t;

/** \brief Time data type  */
typedef uint64 OS_Time_t;

#ifndef bool
#define bool uint8
#endif

//------------------------------------------------------------------------------

/**********************
 * ERCOS-RT structures
 *********************/
struct mutex;

/** 
 * \brief List node structure. 
 * 
 * This is a generic structure that implements a node of a double linked list.
 */
typedef struct list_node {
   /** Next node of the list */
   struct list_node *next;
   /** Previous node of the list */
   struct list_node *prev;
   /** Flag that indicates that the node is already inserted in the list */
   volatile uint8 inside;  
}OS_Node_t;

/**
 * \brief OS Timeval structure.
 * 
 * Stores the system time in seconds and microseconds.
 */
typedef struct _timeval
{
    /** Number of seconds   */
    uint32 seconds;
    
    /** Number of microseconds  */
    uint32 useconds;
}OS_Timeval_t;

/** 
 * \brief OS list structure.
 * 
 * This structure implements the list header.
 */
typedef struct list {
	/** First node of the list */
    OS_Node_t *first;
    /** Last node of the list */
    OS_Node_t *last;
    /** Number of elements of the list */
    volatile uint32 num;
}OS_List_t;


/**
 * \brief Stack structure.
 * 
 * This structure is neccesary to implement the dynamic stack allocation when
 * in the creationthread creation the stack pointer is set to a NULL value.
 */
typedef struct stack {
	/** Initial address of the stack memory block */
    uint8 *addr;
    /** Size of the stack memory block */
    uint32 size;
    /** Node of the list of the stack memory blocks */
    OS_Node_t node;
}OS_Stack_t;


/** 
 * Semaphore structure
 */

/** \brief Mutex identifier type   */
typedef uint32  OS_SemID_t;

/** \brief Binary Semaphore Structure */
typedef struct semaphore
{
    /** Semaphore identifier  */
    OS_SemID_t sem_id;

    /** ID of the task blocked in the semaphore   */
    tid_t tid_in;
    
    /** Semaphore counter value */
    sint8 sem_value;
    
    /** Semaphore queue. In this queue the task will be blocked */
    OS_List_t list; 
    
    /** Neccesary to implement semaphore queues */
    OS_Node_t node;

}OS_Sem_t;

/** 
 * Thread Control Block structure
 */
 
/** 
 * \brief Task state. Indicates that the task is currently in control of the
 *        CPU.
 */
#define TS_RUNNING          0x00
/** 
 * \brief Task state. Indicates that the task is currently blocked in a
 *        semaphore queue.
 */
#define TS_SEM_BLOCKED      0x01
/** 
 * \brief Task state. Indicates that the task is currently blocked in the time
 *        queue.
 */
#define TS_TIME_BLOCKED     0x02
/** 
 * \brief Task state. Indicates that the task is currently ready to enter to
 *        the CPU and inserted in the ready queue.
 */
#define TS_READY            0x04

/** \brief Task structure */
typedef struct task
{
    /** Hardware context. Must be the first field   */
    CPU_context_t context;

    /** Task status */
    uint32 status;
    
    /** Thread priority   */
    priority_t priority;
    
    /** Thread base priority   */
    priority_t base_priority;
    
    /** Entry point to the task code */
    void (*entry_point)(void *);
    
    /** Entry point function argument   */
    void *arg;
    
    /** Task identifier */
    tid_t tid;
    
    /** Stack information structure  */
    OS_Stack_t stack;
    
    /** Absolute task timeout */
    OS_Time_t abs_timeout;
    
    /** Resource semaphore pointer */
    struct mutex *pmutex;
    
    /** Stores the semaphore identifier for which the thread is waitting. */
    OS_SemID_t wait_sem;
    
    /** Resource queue node */
    OS_Node_t resource_node;
    
    /** Sched queue node */
    OS_Node_t node;
    
    /** Time queue node */
    OS_Node_t time_node;
    
}OS_task_t;

/** \brief Mutex structure */
typedef struct mutex
{
    /** Semaphore identifier  */
    uint8 mutex_id;

    /** Priority mutex FIFO queue    */
    OS_List_t list;
  
    /** Mutex counter value */
    sint8 counter;
    
    /** ID of the thread that has catched the mutex */
    tid_t in_thread;

    /** Priority policy of the mutex */
    uint8 type;
    
    /** Inherit maximum priority  */
    priority_t prio;
    
    /** Pointer to the next mutex in case a thread catch more than one mutex  */
    struct mutex *next;
    
}OS_Mutex_t;

/** \brief The mutex identifier type */
typedef uint32  OS_MutexID_t;

/**
 * \brief Data structure to manage the interrupt handler routines.
 * 
 * This structure provides the mechanism to carry out the event propagation to
 * the upper levels.
 */
typedef struct interrupt_control
{
    /** Current interrupt service routine   */
    void(*isr)(int _level);

    /** Old interrupt service routine   */
    void(*old_isr)(int _level);
    
}OS_Interrupt_Control_t;
 


/** This label identifies the inherit policy in the mutex   */
#define MUTEX_PRIO_NORMAL    0

/** This label identifies the inherit policy in the mutex   */
#define MUTEX_PRIO_INHERIT    1

/** This label identifies the priority ceiling policy in the mutex   */
#define MUTEX_PRIO_CEILING    2

/** Number of defined mutex priority policies */
#define MUTEX_PRIO_TYPES      MUTEX_PRIO_CEILING


//------------------------------------------------------------------------------

/**********************
 * ERCOS-RT POSIX
 *********************/

#ifdef CONFIG_ERCOS_POSIX

/** \brief Time type */
typedef uint32 time_t;

/** 
 * \brief Defines all the identifiers of the particular clocks on which to act.
 */
typedef enum {
    /** System-wide realtime clock. */
    CLOCK_REALTIME=0,
}clockid_t;

/** 
 * \brief Structure timespec specifies a single time value
 */
struct timespec {
    /** Seconds */
    time_t  tv_sec;
    /** Nanoseconds */
    unsigned long tv_nsec;
};

/** \brief Used to identify a thread attribute object. */
typedef struct attr
{
    /** Thread stack size */
    uint32 stacksize;
    
    /** Thread stack address  */
    uint32 *stackaddr;
    
    /** Thread priority   */
    uint32 thread_prio;
    
}pthread_attr_t;

/** \brief Mutex attributes  */
typedef struct mutex_attr
{
    /** Priority policy type  */
    uint8 prio_type;
    
    /** Mutex type    */
    uint8 mutex_type;
    
    /** Ceiling priority    */
    int prioceiling;
    
}pthread_mutexattr_t;

/** Sempahore structure definition  */
typedef OS_SemID_t sem_t;

/** MUtual EXclusion device type    */
typedef struct mutex_t
{
    /** Mutex identifier  */
    OS_MutexID_t OS_mutex;
    
    /** Priority policy type  */
    uint8 prio_type;
    
    /** Mutex type */
    uint8 mutex_type;
    
}pthread_mutex_t;

/** \brief Another thread scheduling policy. */
#define SCHED_OTHER 0

/** 
 * \brief Contains the scheduling parameters required for implementation of
 *        each supported scheduling policy.
 */
struct sched_param
{
    /** Process execution scheduling priority */
    int sched_priority;
};

/** \brief Type used to identify a thread. */
typedef tid_t pthread_t;

/** \brief Thread specific key type */
typedef uint32 pthread_key_t;

#endif //CONFIG_POSIX

#endif

