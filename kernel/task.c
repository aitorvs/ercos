/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/


#include <kernel/task.h>

#include <kernel/config.h>  /*  Kernel configuration parameters */
#include <kernel/context.h>
#include <kernel/sched.h>
#include <kernel/mm.h>
#include <kernel/list.h>
#include <kernel/debug.h>
#include <kernel/sem.h>

#include <cpu.h>

//#ifdef CONFIG_ERCOS_TRACER
    #include <tracer/ktracer.h>
//#endif

//------------------------------------------------------------------------------
/*
 * Extern variables.
 */
extern uint32 TASK_STACKS;

/**
 * This is the array that stores all the Threads Control Blocks that the system
 * has
 */
OS_task_t task_table[KCONFIG_MAX_THREADS];

/**
 * This queue maintains the free TCBs
 */
static OS_List_t tcb_list;

/** This is the main thread identifier  */
#define MAIN_THREAD_TID 1

/** This is the kernel stack size   */
#define KERNEL_STACK_SIZE (CONFIG_KERNEL_STACK_SIZE * 1024)

/** This array stores the kernel stack which is used in the interrupts  */
static uint8 kernel_stack[KERNEL_STACK_SIZE];

/** This is the pointer to the kernel_stack which is used from the asm code */
uint8 *kstack = (kernel_stack + KERNEL_STACK_SIZE);
//------------------------------------------------------------------------------

/**
 * This is the IDLE task.
 */
static void OS_TaskMain(void);

/**
 * This funcion is the thread wrapper.
 * 
 * This function is the first "entry point" for all threads. The function 
 * invokes the correct thread entry point and then frees all the thread TCB
 * entries.
 */
static void OS_ThreadWrapper(void);


#ifdef CONFIG_ERCOS_TRACER

    /**
     * This task is the tracer analyser that retrieves all the information from
     * the stream buffer.
     */
    static void OS_TaskTracerAnalyser(void);
#endif

/* This is the main function header */
void main (void);

//------------------------------------------------------------------------------

#ifdef CONFIG_ERCOS_TRACER

static void OS_TaskTracerAnalyser(void)
{
    while(1) {
        OS_TracerRetrieveEvent();   /*  Security    */
    }
}

#endif  // #ifdef CONFIG_ERCOS_TRACER

/*
 * Function:    static void OS_IdleTask(void)
 * 
 * This is the IDLE task
 */
static void OS_TaskMain(void)
{
    /*  The main thread has the maximum system priority */
    SYS_SetPriority(MAIN_THREAD_TID, 0);

    main();

//    SYS_SetPriority(MAIN_THREAD_TID, KCONFIG_MIN_PRIORITY);

//    OS_SetPriority(MAIN_THREAD_TID, KCONFIG_MIN_PRIORITY);
//    OS_SchedCurrentReady();
//    SYS_Schedule();
    
    while(1) {   // Security

        #ifdef CONFIG_ERCOS_TRACER
            OS_SemID_t idleSem;
            OS_SemInit(&idleSem, 0);
            SYS_SemWait(idleSem);
        #else
            CPU_PowerDown();
        #endif  //#ifdef CONFIG_ERCOS_TRACER
        
    }
}


/*
 * Function: void OS_TCBInit(void)
 * 
 * This function initializes all the Thread Control Blocks to the init state
 */
void OS_TCBInit(void)
{
    int i;
    
    DEBUG("Task Control Blocks init...");

    OS_ListReset(&tcb_list);
    
    /* Assure that the priority of the task 0 is 0. This is important if ERCOS
     * is linked to C++ stuff; constructors (called from OS_CPPInit) may do
     * system calls and the task 0 will store the context of OS_Start function
     * before the first official dispatch (at the end of the function). Until
     * now, priority was 0 because task_table is allocated in the bss which is
     * zeroed at startup time (!!) */
    task_table[0].priority = 0;

    for (i = MAIN_THREAD_TID; i < KCONFIG_MAX_THREADS; ++i)
    {
        /*  Init the thread status  */
        task_table[i].status = 0;
        
        /*  Traps off, set S, PS and FP unit    */
        //task_table[i].context.psr = 0x000010c0;
        
        /*  Init the thread identifier  */
        task_table[i].tid = i;
        
        /*  Reset all the nodes associated with each thread */
        OS_ListNodeReset(&task_table[i].node);
        OS_ListNodeReset(&task_table[i].time_node);
        OS_ListNodeReset(&task_table[i].resource_node);
        
        
        /*  Add the TCB into the free TCBs queue   */
        OS_ListAppend(&tcb_list, &task_table[i].node);
    }
    
    /*  Init the IDLE task with the min priority  */
    OS_TaskCreate(NULL, KCONFIG_MAIN_STACK_SIZE, (void*)OS_TaskMain, NULL, 0);


    #ifdef CONFIG_ERCOS_TRACER
        /*  Create the tracer analyser  */
        OS_TaskCreate(NULL, KCONFIG_MAIN_STACK_SIZE, (void*)OS_TaskTracerAnalyser, NULL, KCONFIG_MIN_PRIORITY);
    #endif
}

void OS_ThreadExit(void)
{
    int tid = current->tid;
    
    /*  Init the thread status  */
    task_table[tid].status = 0;
    /*  Traps off, set S, PS and FP unit    */
    //task_table[tid].context.psr = 0x000010c0;
    
    /*  Free the thread stack   */
    OS_Free(task_table[tid].stack.addr);
    
    /*  Add the TCB into the free TCB queue   */
    OS_ListAppend(&tcb_list, &task_table[tid].node);
}

int OS_TaskGetStatus(tid_t _tid)
{
    return task_table[_tid].status;
}

static void OS_ThreadWrapper(void)
{
    /*  Execute the thread entry point  */
    current->entry_point(current->arg);
    
    /*  The thread is finished so we free all the TCB entries   */
    SYS_ThreadExit();
}

/*
 * Function: tid_t OS_TaskCreate(uint32  _stack_size, void *_ip, uint8 _priority)
 * 
 * This routine creates a new thread and returns the TCB created
 */
tid_t OS_TaskCreate(uint32 *_stackaddr, uint32  _stacksize, void *_ip, void *_arg, uint8 _priority)
{
    OS_task_t *task;
    OS_Node_t *n;

    /*  Outs a free thread control block    */
    n = (OS_Node_t *)OS_ListGetNode(&tcb_list);
    
    if ((task = GET_ELEMENT(OS_task_t, n, node)) == NULL)
    {
	DEBUG("No more threads can be created");
        return -1;
    }

    /* DEBUG2 ("(_stackaddr=%p, _stacksize=%p, _ip=%p, _arg=%p, priority=%p); task->tid=%p", _stackaddr, _stacksize, _ip, _arg, _priority, task->tid); */

    /*  Stablish the thread stack */
    if (_stackaddr == NULL)
    {
        if (_stacksize < KCONFIG_MINSTACK) _stacksize = KCONFIG_MINSTACK;
        _stackaddr = OS_Malloc (_stacksize);
    } else {
        if (_stacksize < KCONFIG_MINSTACK)
        {
            DEBUG ("Provided stack is smaller than the minimum stack!");
        }
    }
    task->stack.size = _stacksize;
    task->stack.addr = (uint8 *)_stackaddr;

    /*  Init the new thread context */
    CPU_ContextInit(&(task->context), task->stack.addr, task->stack.size, 1, OS_ThreadWrapper);

    /*  Stablish the thread status  */
    task->status = TS_READY;

    /*  Stablish the thread entry point  */
    task->entry_point = _ip;
    task->arg = _arg;
    
    /*  Init the pointer to the catched mutex   */
    task->pmutex =(OS_Mutex_t*) NULL;
    
    /*  Check the priority range    */
    if (_priority <= 0) 
        _priority = 0;              /*  Maximum priority    */
    else if (_priority > KCONFIG_MIN_PRIORITY)
        _priority = KCONFIG_MIN_PRIORITY;   /*  Minimum priority    */

    /*  Set the thread base and temporal priorities   */
    task->priority = _priority;
    task->base_priority = _priority;
    
    /*  Add the task to the correspond sched queue  */
    OS_SchedTaskReady(task->tid);

    KTRACE_EVENT_TID(KEVENT_THREAD_CREATE, task->tid);
    
    /*  Returns the pointer to the Thread Control Block */
    return task->tid;
}

void OS_TaskGetStackParameters(uint32 *_stackaddr, uint32 *_stacksize)
{
    *_stackaddr = (uint32)current->stack.addr;
    *_stacksize = current->stack.size;
    
}

int OS_TaskIsBlocked(tid_t _tid)
{
    return (task_table[_tid].status & (TS_SEM_BLOCKED | TS_TIME_BLOCKED));
}


