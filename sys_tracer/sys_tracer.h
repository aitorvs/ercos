#include <stdio.h>
#include <kernel/list.h>
#include <kernel/config.h>
#include <sys/types.h>
#include <syscall.h>

#if 0

/** unsigned byte   */
typedef unsigned char	uint8;
/** unsigned double byte    */
typedef unsigned short	uint16;
/** unsigned 32 bit word    */
typedef unsigned int	uint32;
/** unsidned 64 bit word    */
typedef unsigned long long uint64;

/** Signed byte */
typedef signed char	sint8;
/** Signed 16 bit word  */
typedef signed short	sint16;
/** Signed 32 bit word  */
typedef signed int	sint32;
/** Signed 64 bit word  */
typedef signed long long sint64;

typedef uint32 tid_t;

#endif

extern void OS_Start(void);
extern void OS_ClockISR(int _level);
extern OS_List_t sched_list[KCONFIG_NUM_PRIORITIES];
extern OS_Sem_t semaphores[KCONFIG_MAX_BIN_SEMAPHORES];
extern OS_Mutex_t mutexes[KCONFIG_MAX_RESOURCE_SEMAPHORES];
extern OS_task_t *current;
extern OS_List_t time_list;
extern OS_Time_t global_time;
