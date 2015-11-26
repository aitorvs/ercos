/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez, Pablo Parra Espada
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/


#ifndef _TIMING_H_
#define _TIMING_H_

#include "edroombp.h"
#include <stdio.h> // quitar este include en la version final.

/**
 * Takes the start time measure
 * 
 * Takes a measure of current time and loads it on start_time
 * global PR_Time object.
 */
 
void start();

/**
 * Adds a task into the sched queue from the tail.
 * 
 * \param _task:    This is the tid of the task to be inserted.
 * 
 * The routine inserts the specified task into the tail sched queue.
 */
 
/*
 * Function stop()
 * Takes a measure of current time and loads it on stop_time
 * global PR_Time object. Returns the difference between start_time
 * and stop_time minus timing_overhead (initially 0, updated after
 * all the init_timing calculus)
 */
 
Pr_Time stop();

/*
 * Function init_timing()
 * One of the greatest. It calculates timing overhead by making
 * series of time measures and after obtaining the average value of them  */
 
void init_timing();

uint32 generate_iterations (int (*function)(int, Pr_Time *), float clock_multiplier);

#endif



