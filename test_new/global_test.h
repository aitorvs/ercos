/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/

/*
 *  Resurce semaphore test.
 *  In this test we try to prove the inherit policy when a same task lock a
 * resource semaphore.
 * This is the "silent" version of Test51.cpp
 */
 
#ifndef _GLOBAL_TEST_H_
#define _GLOBAL_TEST_H_

#include "edroombp.h"

#include <stdio.h>


/* char foo is used by SPIN() MACRO. It's declared extern
 * so the compiler will create just a single copy of it. 
 * Inconvenience = it has to be defined in global_test.cpp */
 
static char foo;

/* The spinning procedure
 * It prints the secuency: '/' '-' '\' '|'  */

#define SPIN()								\
do {									\
	printf("\b%c", "/-\\|"[++foo&3]);				\
} while (0)

/* DELAY() MACRO. Used to implement a delay in the
 * thread execution while still being in use of the CPU */

#define DELAY_ITERS		239423		/* ERC32 (on tsim-erc32, 14Mhz) */
//#define DELAY_ITERS		2884558		/* LEON3 (on tsim-leon3) */
//#define DELAY_ITERS		24300		/* m68k (on test board) */

/* 375 ms delay */
#define DELAY()								\
do {									\
	for (volatile int i = 0; i < DELAY_ITERS; i++);			\
	SPIN();								\
} while (0)


/* MAX_TASKS is the maximum number of tasks launched during
 * the execution of one single test */

#define MAX_TASKS 20

/* Array of Pr_Task pointers used by the different tests to
 * reference all the created tasks. They are to be reused
 * by all the tests */

extern Pr_Task *task[MAX_TASKS];

/* MAX_SEMAPHOREREC defines the maximum number of
 * Pr_SemaphoreRec objects used during the execution of
 * one single test */

#define MAX_SEMAPHOREREC 20

/* Array of Pr_SemaphoreRec pointers used by the different
 * tests. They are to be reused by all the tests */

extern Pr_SemaphoreRec *semrec[MAX_SEMAPHOREREC];

/* MAX_SEMAPHOREBIN defines the maximum number of
 * Pr_SemaphoreBin objects used during the execution of
 * one single test */

#define MAX_SEMAPHOREBIN 20

/* Array of Pr_SemaphoreBin pointers used by the different
 * tests. They are to be reused by all the tests */

extern Pr_SemaphoreBin *sembin[MAX_SEMAPHOREBIN];



class KernelInitializer
{
	public:
	static Pr_Kernel * kernel;
	KernelInitializer();
};

typedef void * (*ptrFunctionTest)(void *);

class Test
{
	KernelInitializer KernelInit;
	Pr_Task pmain_test;
	volatile bool ended;
	
	public:
	Pr_Time start_time;

	Test (ptrFunctionTest fn)
#ifdef _EDROOM_IS_DEBUG_
	: pmain_test(fn, "Test", EDROOMprioMINIMUM, 512) {}
#else
	: pmain_test(fn, EDROOMprioMINIMUM, 512) {}
#endif
	void Init()
	{
		pmain_test.SetPriority(EDROOMprioURGENT);
		ended = false;
	}
	void Start()
	{
		start_time.GetTime();
		pmain_test.SetPriority(EDROOMprioMINIMUM);
		while (!ended);
	}
	void End()
	{
		ended = true;
	}
};

#if __GNUC__ < 3 

#define DEFINE_TEST(name)				\
void *test##name##_main(void *ptr);		\
Test Test##name##(test##name##_main)

#else

#define DEFINE_TEST(name)				\
void *test##name##_main(void *ptr);		\
Test Test##name(test##name##_main)

#endif

#endif
