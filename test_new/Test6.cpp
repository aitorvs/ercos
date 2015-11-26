/*
 * This test checks the implementation of the Pr_Time methods and operators.
 */
 
#include "edroombp.h"
#include <stdio.h>
#include "global_test.h"

DEFINE_TEST(6);

#define VERBOSE_AT(task,t)						\
	Pr_DelayAt(Pr_Time(t) += Test6.start_time);			\
	clock_gettime (CLOCK_REALTIME, &tp); 				\
	printf ("Task" #task ": Delay requested to " #t			\
		"; current time is %ds, %dus\n",			\
		tp.tv_sec, tp.tv_nsec / 1000);

#define SILENT_AT(task,t)						\
	Pr_DelayAt(Pr_Time(t) += Test6.start_time);

#define AT(task,t)							\
	VERBOSE_AT (task, t)
	

void *test6_thread0(void* ptr) {
    struct timespec tp;

    printf ("Task0: Test start time is %dus\n", (long) Test6.start_time.Micros ());

    AT (0, 1.230f);
    AT (0, 2.340f);
    AT (0, 3.000f);
    AT (0, 4.560f);
    AT (0, 5.120f);
    AT (0, 6.500f);
    AT (0, 7.100f);
    AT (0, 7.800f);
    AT (0, 8.900f);
    AT (0, 10.000f);
    AT (0, 11.500f);
    AT (0, 12.200f);
    AT (0, 14.400f);

    return NULL;
}

void *test6_thread1(void* ptr) {
    struct timespec tp;

    AT (1, 1.230f);
    AT (1, 2.340f);
    AT (1, 3.000f);
    AT (1, 4.560f);
    AT (1, 5.120f);
    AT (1, 6.500f);
    AT (1, 7.100f);
    AT (1, 7.800f);
    AT (1, 8.900f);
    AT (1, 10.000f);
    AT (1, 11.500f);
    AT (1, 12.200f);
    AT (1, 14.400f);

    Test6.End();
    return NULL;
}

void * test6_main (void *ptr) {
  
    Test6.Init();
    
    Pr_Task ptask(test6_thread0, "tarea", EDROOMprioNormal, 2048);
    Pr_Task ptask2(test6_thread0, "tarea", EDROOMprioNormal, 2048);
    Pr_Task ptask3(test6_thread0, "tarea", EDROOMprioNormal, 2048);
    Pr_Task ptask4(test6_thread1, "tarea", EDROOMprioLow, 2048);
    
    task[0] = &ptask;
    task[1] = &ptask2;
    task[2] = &ptask3;
    task[3] = &ptask4;

    printf("\n\nStarting TEST6:\nTest for implementation of Pr_Time methods\n\n");
    
    Test6.Start();

    printf("\n\nTEST6: Finished!\n\n");
    return NULL;
}

