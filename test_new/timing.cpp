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
 *  Context Switch Benchmark based on latency context benchmark by HBench-OS
 *  This file contains all the time measuring related functions of the benchmark
 */

#include "timing.h"
#include "utils.h"

/*
 * First we start defining all the timing functions and resources
 * Some of the names used in HBench-OS benchmark have been changed in
 * order to simplify syntax and understanding.
 */
 
/*
 * Number of outer and inner loops made to calculate the overhead
 * of time measures.
 */
 
#define OVERHEAD_OUTER_LOOPS 100
#define OVERHEAD_INNER_LOOPS 10

/*
 * OVERHEAD_TAILS expresses the number of measures discarded 
 * by excess and defect previous to obtain the average value.
 */

#define OVERHEAD_TAILS 2

/*
 * PR_Time object containing the average timing overhead
 * Initialized to 0.
 */
 
Pr_Time timing_overhead(0.0f);

/*
 * PR_Time objects containing the measured start time and stop time 
 * respectively.
 */

Pr_Time start_time, stop_time;

/* time_values array. The function will store in it all the
 * time measures in order to obtain the timing overhead */
  
Pr_Time time_values[OVERHEAD_OUTER_LOOPS];
 
/** It takes the initial time value of the general purpose counter */

void start()
{
	start_time.GetTime();
}

/** It takes the final time value of the general purpose counter and
 * returns the difference between that value and the one from the initial
 * measure */
 
Pr_Time stop()
{
	stop_time.GetTime();
	Pr_Time tmp = stop_time;
	tmp -= start_time;
	tmp -= timing_overhead;
	return (tmp);
}

void init_timing()
{
	int i, j;
	
	/* Firts round of measures and assingments to ensure the
	 * validness of the first measure: ensure no cache misses
	 * during the first data obtaining loop */
	 
	timing_overhead = 0.0f;
	start();
	stop();
	
	/* THE time measures obtaining loop
	 * Every position of time_values array will contain an
	 * average of OVERHEAD_INNER_LOOPS measures */
	
	for (i = 0; i < OVERHEAD_OUTER_LOOPS; i++)
	{
		time_values[i] = 0.0f;
		for (j = 0; j < OVERHEAD_INNER_LOOPS; j++)
		{
			start();
			time_values[i] += stop();
		}
		time_values[i] = time_values[i].Secns() / (float) OVERHEAD_INNER_LOOPS;
	}
	
	/* Now it will sort all the measures in increasing order */
	bubble_sort(time_values, OVERHEAD_OUTER_LOOPS);
	
	/* A new loop will remove the 2 greatest and the 2 lowest
	 * values from time_values array and obtain the sum of all
	 * the rest measures in order to calculate the average value */
	 
	for (i = OVERHEAD_TAILS; i < OVERHEAD_OUTER_LOOPS - OVERHEAD_TAILS; i++)
		timing_overhead += time_values[i];
	
	timing_overhead = timing_overhead.Secns() / (float)(OVERHEAD_OUTER_LOOPS - 2* OVERHEAD_TAILS);
	
	printf("Average timing overhead in usecs: %d\n", (uint32) timing_overhead.Micros());
}

uint32 generate_iterations (int (*_function)(int, Pr_Time *), float _clock_multiplier)
{
	uint32 num = 1;
	Pr_Time return_time;
	float time = 0.0;
	
	if (!_function)
		return 1;
	
	if ((*_function)(1, &return_time) != 0)
		return (1);
		
	while ((time = (return_time.Micros())*_clock_multiplier) < 1000000.)
	{
		/* while less than one second */
		num <<= 1;
		if ((*_function)(num, &return_time) != 0)
		{
			num >>= 1;
			break;
		}
	}
	return (num);
}
