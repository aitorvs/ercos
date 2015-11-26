/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez, Pablo Parra Espada
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/

/*
 * Context Switch Benchmark based on latency context benchmark implemented
 * by HBench-OS.
 * 
 * Since ERC32 processor does not have any sort of cache system, we don't need
 * to simulate any working set between the token passing loop. This fact makes
 * the test much more accurate, eliminating almost all sources of undetermination.
 */

#include "edroombp.h"
#include "timing.h"
#include "utils.h"

#include <stdio.h>

#include "global_test.h"

DEFINE_TEST(lat_ctx2);

/* Number of tasks joining the benchmark */

#define NUM_TASKS 10

/* Number of iterations done to obtain the measure */

#define NUM_ITERATIONS 1024

/* Array of Binary Semaphores used for token passing */

Pr_SemaphoreBin sem_array[NUM_TASKS];

/** This function simulates the token passing but within the scope 
 * of a single process.
 * 
 * \param _num_iterations
 * \param _time_spent
 * 
 */

int do_overhead1(int _num_iterations, Pr_Time *_time_spent)
{
	int i = 0, j = 0;

	/* Binary semaphores are initially closed, so it firstly needs to
	 * make a Signal to pass through it */

	sem_array[0].Signal();
	
	/* This is point. Firstly, it takes the general purpose counter value,
	 * then it does a complete token passing through all the semaphores
	 * _num_iterations times. When it finishes, takes another time measure
	 * and stores the difference in _time_spent. */
	
	start();
	for (i = 0; i < _num_iterations; i++)
	{
		do
		{
			sem_array[j].Wait();			
			if (++j == NUM_TASKS)
				j = 0;
			sem_array[j].Signal();

		} while (j);
	}
	*_time_spent = stop();
	
	return (0);
}

/** This function executes do_overhead1() OVERHEADAVG_LOOPS times and
 * stores the average of the time spent in doing it in _time_spent.
 * 
 * \param _num_iterations
 * \param _time_spent
 * 
 */

int do_overhead2(int _num_iterations, Pr_Time *_time_spent)
{
	int 	i;
	Pr_Time	time_measure, tmp;

	centeravg_reset(OVERHEADAVG_LOOPS, OVERHEADAVG_TAILS);

	for (i = OVERHEADAVG_LOOPS; i > 0; i--) {
		do_overhead1(_num_iterations, &time_measure);
		tmp = time_measure.Secns()/(float)(NUM_TASKS*_num_iterations);
		centeravg_add(tmp); /* get 1-sw overhead */

	}

	centeravg_done(_time_spent);

	return (0);
}

/** This function does the REAL stuff. It does the token pass between all the
 * processes which live in the system _num_iterations times measuring the time
 * spent on it.
 * 
 * \param _num_iterations
 * \param _time_spent
 * 
 */
 
int do_context_switch(int _num_iterations, Pr_Time *_time_spent)
{
	int i;

	/*
	 * Go once around the loop to make sure that everyone is ready and
	 * to get the token in the pipeline.
	 */
	sem_array[0].Signal();
	sem_array[NUM_TASKS-1].Wait();
	sem_array[0].Signal();

	/*
	 * Main process - all others should be ready to roll, time the
	 * loop.
	 */
	 
	printf("Time to loop!\n");
	start();
	for (i = 0; i < _num_iterations; i++)
	{
		sem_array[NUM_TASKS-1].Wait();
	    sem_array[0].Signal();
	}
	*_time_spent = stop();
	Pr_Time tmp = *_time_spent;
	
	printf("Microsegundos: %d\n", (uint32)tmp.Micros());

	return (0);
}

/* Helper defines for Pr_Task management */

#if __GNUC__ < 3

#define TASK_FUNCTION(num)				\
void *lat_ctx2_thread##num##(void *ptr)			\
{							\
	while (1)					\
	{						\
		sem_array[num-1].Wait();		\
		sem_array[num].Signal();		\
	}						\
}

#define PRTASK_TASK(num, priority)			\
Pr_Task ptask##num##(lat_ctx2_thread##num##, priority, 8*512); \
task[##num##] = &ptask##num##;

#define PRTASK_TASK_DEBUG(num, priority)		\
Pr_Task ptask##num##(lat_ctx2_thread##num##, "Task #num", priority, 8*512); 	\
task[##num##] = &ptask##num##;

#else

#define TASK_FUNCTION(num)				\
void *lat_ctx2_thread##num(void *ptr)			\
{							\
	while (1)					\
	{						\
		sem_array[num-1].Wait();		\
		sem_array[num].Signal();		\
	}						\
}

#define PRTASK_TASK(num, priority)			\
Pr_Task ptask##num(lat_ctx2_thread##num, priority, 8*512); \
task[num] = &ptask##num;

#define PRTASK_TASK_DEBUG(num, priority)		\
Pr_Task ptask##num(lat_ctx2_thread##num, "Task #num", priority, 8*512); 	\
task[num] = &ptask##num;

#endif


/* Now we define all the Pr_Task pointers with their respective codes */
		
TASK_FUNCTION(1)
TASK_FUNCTION(2)
TASK_FUNCTION(3)
TASK_FUNCTION(4)
TASK_FUNCTION(5)
TASK_FUNCTION(6)
TASK_FUNCTION(7)
TASK_FUNCTION(8)
TASK_FUNCTION(9)

/* Since task0 has a different piece of code, it has to be declared and defined
 * separately */

/** Task0's code. It does all the initial stuff to calculate all the overheads
 * and launches the token pass */

void *lat_ctx2_thread0(void *ptr)
{
	int tmp_iterations;
	Pr_Time overhead;
	Pr_Time time_spent, tmp, total_time;
	
	/* Obtain the timing overhead */
	printf("Task0: Hello, let's init_timing()\n");
	init_timing();

	/* Calculate the number of executions of do_overhead1() that can
	 * be done in 1 second */
	 
	tmp_iterations = generate_iterations (&do_overhead1, 1);
	printf("%d complete iterations in 1 second\n", tmp_iterations);
	
	/* Now obtains the token passing overhead */
	tmp_iterations >>= 1;
	overhead = 0.0;
	do_overhead2(tmp_iterations, &overhead);
	
	/* After obtaining all the overheads, it launches the context swith latency
	 * measuring process */
	do_context_switch(NUM_ITERATIONS, &time_spent);
	
	/* Now, it calculates the context swith latency time */
	printf("Time spent: %d\n", (uint32)time_spent.Micros());
	tmp = (float)(overhead.Secns() * (float)(NUM_ITERATIONS * NUM_TASKS));
	printf("Total overhead: %d\n", (uint32)tmp.Micros());
	time_spent -= tmp;

	total_time = (float)(time_spent.Secns() / (NUM_ITERATIONS * NUM_TASKS));
	
	printf("Latency of context switch: %d usecs\n", (uint32)total_time.Micros());
	
	Testlat_ctx2.End();
	
    return NULL;
}

void *testlat_ctx2_main(void *ptr)
{
	Testlat_ctx2.Init();  
       
#ifdef _EDROOM_IS_DEBUG_
  
    Pr_Task ptask0(lat_ctx2_thread0, "Initial Task", EDROOMprioVeryHigh, 8*512);
	task[0] = &ptask0;
    
    PRTASK_TASK_DEBUG(1, EDROOMprioVeryHigh)
    PRTASK_TASK_DEBUG(2, EDROOMprioVeryLow)
    PRTASK_TASK_DEBUG(3, EDROOMprioVeryHigh)
    PRTASK_TASK_DEBUG(4, EDROOMprioVeryLow)
    PRTASK_TASK_DEBUG(5, EDROOMprioVeryHigh)
    PRTASK_TASK_DEBUG(6, EDROOMprioVeryLow)
    PRTASK_TASK_DEBUG(7, EDROOMprioVeryHigh)
    PRTASK_TASK_DEBUG(8, EDROOMprioVeryLow)
    PRTASK_TASK_DEBUG(9, EDROOMprioVeryHigh)

#else

	Pr_Task ptask0(lat_ctx2_thread0, EDROOMprioVeryHigh, 8*512);
	task[0] = &ptask0;

    PRTASK_TASK(1, EDROOMprioVeryHigh)
    PRTASK_TASK(2, EDROOMprioVeryLow)
    PRTASK_TASK(3, EDROOMprioVeryHigh)
    PRTASK_TASK(4, EDROOMprioVeryLow)
    PRTASK_TASK(5, EDROOMprioVeryHigh)
    PRTASK_TASK(6, EDROOMprioVeryLow)
    PRTASK_TASK(7, EDROOMprioVeryHigh)
    PRTASK_TASK(8, EDROOMprioVeryLow)
    PRTASK_TASK(9, EDROOMprioVeryHigh)

#endif

    printf("\n\nStarting Context Switch Latency Benchmark\n\n");
        
    Testlat_ctx2.Start();

    printf("\n\nContext Switch Latency Benchmark: Finished!\n\n");  
    return NULL;
}
 


