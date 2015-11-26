/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/

/* Main function for the all-in-one ERCOS-RT tests
 */

#include "edroombp.h"

#include <stdio.h>

#include "global_test.h"

extern "C" int atexit(void (*)()) {return 0;}

/* Definition of task, semrec and sembin, declared extern
 * in global_test.h */

Pr_Task *task[MAX_TASKS];
Pr_SemaphoreRec *semrec[MAX_SEMAPHOREREC];
Pr_SemaphoreBin *sembin[MAX_SEMAPHOREBIN];

Pr_Kernel * KernelInitializer::kernel;
static Pr_Kernel gkernel;

KernelInitializer::KernelInitializer()
{
	KernelInitializer::kernel = &gkernel;
}

void begin_tests(void)
{
    KernelInitializer::kernel->Start();
}

int main(void)
{
   
    printf("Initializing all tests. Let's go!\n");
   
	begin_tests();
    
    printf("All tests are over!\n");
    
//    while(1);   /*  idle task   */
}

