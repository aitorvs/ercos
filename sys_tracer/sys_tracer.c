#include "sys_tracer.h"

static int sched_syscall = 1;

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

/* This system calls needs 24 bytes to be traced */

#define SYSTRACE_SET_PRIORITY       101
#define SYSTRACE_SET_TMP_PRIORITY   102
#define SYSTRACE_SEM_TIMEDWAIT      103

#define SET_SCHED_SYSCALL() (sched_syscall = 1);
#define CLEAR_SCHED_SYSCALL() (sched_syscall = 0);
#define IS_SCHED_SYSCALL_SETTING    (sched_syscall == 1)

typedef struct systracer_events
{
    /** Traced system call identifier   */
    uint32      sys_id;
    /** System call timestamp   */
    uint64   timestamp;
    
    /** Argument one    */
    /* IMPORTANT NOTE: This argument is 64 bits sized only HERE. The reason
     * is to be fully compatible with all the syscalls (since the SYS_Sleep
     * call receives an argument of 8 bytes)
     */
    uint64      arg1;
    /** Argument two    */
    uint32      arg2;

} systracer_events_t;

int main(int argc, char * argv[])
{
    int i = 0;
    OS_Node_t *pos;
    OS_task_t *task;
    OS_SemID_t semaphoreId;
    OS_MutexID_t mutexId;
    
    systracer_events_t event;

    FILE *fp;

    if (argc < 2)
    {
	printf("Oops! Way of use: sys_tracer log file\n");
	return(1);
    }

    fp = fopen(argv[1], "r");
    
    if (!fp)
    {
	printf("ERROR: opening the original log file\n");
	return(1);
    }
    printf("Initializing the system...\n");
    
    OS_Start();
    
    printf("\n\n-------------------------------------------\n\n");
    printf(" Hi! I am the syscall tracer.\n");
    printf(" Press any key to start decoding the events!\n\n");
    printf("--------------------------------------------\n\n");

    getchar();

    while (fscanf(fp, "%d %lld %lld %d", &event.sys_id, &event.timestamp, &event.arg1, &event.arg2))
    {
    global_time = event.timestamp;
	printf("Time stamp: %lld\n", event.timestamp);
    printf("\nOLD TASK => %d\n", current->tid);
	switch(event.sys_id)
	{
	    case(SYSTRACE_THREAD_SLEEP):
		printf("System call: SYS_Sleep()\n");
		printf("Argument: OS_Time_t _ticks = %lld\n\n", event.arg1);
		SYS_Sleep(event.arg1 - event.timestamp);
        SET_SCHED_SYSCALL();
		break;
	    case(SYSTRACE_THREAD_CREATE):
		printf("System call: SYS_TaskCreate()\n");
		printf("Argument: uint8 _priority = %d\n\n", event.arg1);
		SYS_TaskCreate(NULL, 0, NULL, NULL, (uint8)event.arg1);
        SET_SCHED_SYSCALL();
		break;
	    case(SYSTRACE_SEM_WAIT):
		printf("System call: SYS_SemWait()\n");
		printf("Argument: OS_SemID_t _sem = %d\n\n", event.arg1);
		semaphoreId = (OS_SemID_t)event.arg1;
		SYS_SemWait(semaphoreId);
        SET_SCHED_SYSCALL();
		break;
	    case(SYSTRACE_SEM_POST):
		printf("System call: SYS_SemPost()\n");
		printf("Argument: OS_SemID_t _sem = %d\n\n", event.arg1);
		semaphoreId = (OS_SemID_t)event.arg1;
		SYS_SemPost(semaphoreId);
        SET_SCHED_SYSCALL();
		break;
	    case(SYSTRACE_SEM_TRYWAIT):
		printf("System call: SYS_SemTryWait()\n");
		printf("Argument: OS_SemID_t _sem = %d\n\n", event.arg1);
		semaphoreId = (OS_SemID_t)event.arg1;
		SYS_SemTryWait(semaphoreId);
        SET_SCHED_SYSCALL();
		break;
	    case(SYSTRACE_MUTEX_LOCK):
		printf("System call: SYS_MutexLock()\n");
		printf("Argument: OS_MutexID_t _mutex = %d\n\n", event.arg1);
		mutexId = (OS_MutexID_t)event.arg1;
		SYS_MutexLock(mutexId);
        SET_SCHED_SYSCALL();
		break;
	    case(SYSTRACE_MUTEX_UNLOCK):
		printf("System call: SYS_MutexUnLock()\n");
		printf("Argument: OS_MutexID_t _mutex = %d\n\n", event.arg1);
		mutexId = (OS_MutexID_t)event.arg1;
		SYS_MutexUnLock(mutexId);
        SET_SCHED_SYSCALL();
		break;
	    case(SYSTRACE_MUTEX_TRYLOCK):
		printf("System call: SYS_MutexTryLock()\n");
		printf("Argument: OS_MutexID_t _mutex = %d\n\n", event.arg1);
		mutexId = (OS_MutexID_t)event.arg1;
		SYS_MutexTryLock(mutexId);
        SET_SCHED_SYSCALL();
		break;
	    case(SYSTRACE_THREAD_EXIT):
		printf("System call: SYS_ThreadExit()\n");
		printf("No arguments this time\n\n");
		SYS_ThreadExit();
        SET_SCHED_SYSCALL();
		break;
	    case(SYSTRACE_IRQ_EVENT):
		printf("System call: SYS_IRQMarkEvent()\n");
		printf("Argument: OS_SemID_t _sem = %d\n\n", event.arg1);
		semaphoreId = (OS_SemID_t)event.arg1;
		SYS_IRQMarkEvent(semaphoreId);
        SET_SCHED_SYSCALL();
		break;
	    case(SYSTRACE_CLOCK_EVENT):
		printf("System call: OS_IRQMarkClockEvent()\n");
		printf("No arguments this time\n\n");
		OS_IRQMarkClockEvent();
        CLEAR_SCHED_SYSCALL();
		break;
	    case(SYSTRACE_HANDLE_EVENT):
		printf("System call: OS_IRQHandleEvent()\n");
		printf("No arguments this time\n\n");
		OS_IRQHandleEvent();
        CLEAR_SCHED_SYSCALL();
		break;
	    case(SYSTRACE_SEM_INIT):
		printf("System call: SYS_SemInit()\n");
		printf("Argument: uint32 _value = %d\n\n", event.arg1);
		SYS_SemInit(&semaphoreId, (uint32)event.arg1);
        SET_SCHED_SYSCALL();
		break;
	    case(SYSTRACE_MUTEX_INIT):
		printf("System call: SYS_MutexInit()\n");
		printf("Argument: uint32 _sem = %d\n\n", event.arg1);
		SYS_MutexInit(&mutexId, (uint32)event.arg1);
        SET_SCHED_SYSCALL();
		break;
	    case(SYSTRACE_SCHEDULE):
        if(!IS_SCHED_SYSCALL_SETTING) {
    		printf("System call: SYS_Schedule()\n");
    		printf("No arguments this time\n\n");
            OS_SchedCurrentReady();
    		SYS_Schedule();
        }
        CLEAR_SCHED_SYSCALL();
		break;
	    case(SYSTRACE_SET_PRIORITY):
		printf("System call: SYS_SetPriority()\n");
		printf("Argument: tid_t _tid = %d\n", event.arg1);
		printf("Second argument: priority_t _priority = %d\n\n", event.arg2);
		SYS_SetPriority((tid_t)event.arg1, (priority_t)event.arg2);
        SET_SCHED_SYSCALL();
		break;
	    case(SYSTRACE_SET_TMP_PRIORITY):
		printf("System call: SYS_SetTmpPriority()\n");
		printf("Argument: tid_t _tid = %d\n", event.arg1);
		printf("Second argument: priority_t _priority = %d\n\n", event.arg2);
		SYS_SetTmpPriority((tid_t)event.arg1, (priority_t)event.arg2);
        SET_SCHED_SYSCALL();
		break;
	    case(SYSTRACE_SEM_TIMEDWAIT):
		printf("System call: SYS_TimedWait()\n");
		printf("Argument: OS_SemID_t _sem = %d\n", event.arg1);
		printf("Second argument: uint32 _ticks = %d\n\n", event.arg2);
		semaphoreId = (OS_SemID_t)event.arg1;
		SYS_SemTimedWait(semaphoreId, event.arg2);
        SET_SCHED_SYSCALL();
		break;
	}

//    if (IS_SCHED_SYSCALL_SETTING)
//    {
//        CLEAR_SCHED_SYSCALL();
//        continue;
//    }
	printf("The queues are:\n");
	
	printf("PRIORITIES QUEUES =>\n\n");

	for (i = 0; i < KCONFIG_NUM_PRIORITIES; i++)
	{
	    if (sched_list[i].first != NULL)
	    {
		printf("Priority %d:", i);
		list_for_each(pos, (sched_list + i))
		{
		    task = GET_ELEMENT(OS_task_t, pos, node);
		    printf(" %d", task->tid);
		}
		printf("\n");
	    }
	}

	printf("\nBINARY SEMAPHORES QUEUES =>\n\n");

	for (i = 0; i < KCONFIG_MAX_BIN_SEMAPHORES; i++)
	{
	    if (semaphores[i].list.first != NULL)
	    {
		printf("Semaphore Id %d: ", i);
		list_for_each(pos, &semaphores[i].list)
		{
		    task = GET_ELEMENT(OS_task_t, pos, resource_node);
		    printf(" %d", task->tid);
		}
		printf("\n");
	    }
	}
	
	printf("\nRESOURCES SEMAPHORES QUEUES =>\n\n");

	for (i = 0; i < KCONFIG_MAX_RESOURCE_SEMAPHORES; i++)
	{
	    if (mutexes[i].list.first != NULL)
	    {
		printf("Mutex Id %d: ", i);
		list_for_each(pos, &mutexes[i].list)
		{
		    task = GET_ELEMENT(OS_task_t, pos, resource_node);
		    printf(" %d", task->tid);
		}
		printf("\n");
	    }
	}
 
    printf("\nTIME QUEUE => ");

    list_for_each(pos, &time_list)
    {
        task = GET_ELEMENT(OS_task_t, pos, time_node);
        printf(" %d", task->tid);
    }
    printf("\n");

 
    printf("\nNEW TASK => %d\n", current->tid);


//	printf("\n-------------------------------------------\n\n");
//	printf("Press any key to continue\n\n");
//    printf("-------------------------------------------\n\n");
//	getchar();

    }

    return 0;
		
}

