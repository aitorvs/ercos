/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez, Pablo Parra Espada
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/

#include "edroombp.h"
#include "timing.h"
#include "utils.h"

#include <stdio.h>

#include "global_test.h"

DEFINE_TEST(lat_syscall);

#define NUM_ITERATIONS 1024

Pr_SemaphoreBin semaphore;

void *lat_syscall_thread0(void *ptr)
{
	int i, j, sum;
	int tmp_iterations;
	Pr_Time overhead;
	Pr_Time time_spent, tmp, time_measure;
	
	/* Obtain the timing overhead */
	printf("Task0: Hello, let's init_timing()\n");
	init_timing();
	
	centeravg_reset(OVERHEADAVG_LOOPS, OVERHEADAVG_TAILS);

	for (int i = OVERHEADAVG_LOOPS; i > 0; i--)
	{
		start();
		for (int j = 0; j < NUM_ITERATIONS; j++)
		{
			semaphore.Signal();
		}
		time_measure = stop();
		tmp = time_measure.Secns()/(float)(NUM_ITERATIONS);
		centeravg_add(tmp); /* get 1-sw overhead */
	}

	centeravg_done(&time_spent);

	printf("Time spent: %d usecs\n", (uint32)time_spent.Micros());
	
	Testlat_syscall.End();
	
}

void *testlat_syscall_main(void *ptr)
{
	Testlat_syscall.Init();  
       
#ifdef _EDROOM_IS_DEBUG_
  
    Pr_Task ptask0(lat_syscall_thread0, "Initial Task", EDROOMprioVeryHigh, 8*512);
	task[0] = &ptask0;

#else

	Pr_Task ptask0(lat_syscall_thread0, EDROOMprioVeryHigh, 8*512);
	task[0] = &ptask0;

#endif

    printf("\n\nStarting Tracer Latency Benchmark\n\n");
        
    Testlat_syscall.Start();

    printf("\n\nTracer Latency Benchmark: Finished!\n\n");  
}
