/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/

#include "edroombp.h"
#include <stdio.h>
#include "global_test.h"


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Test for:
//						Pr_Time friend functions Pr_DelayAt Pr_DelayIn
//						Pr_Time method GetTime
//						Pr_SemaphoreBin method WaitTimed
//
//  Test behaviour:
//      - The task2 tries to catch the Sem2 during a second without OK result
//      - The task1 goes to sleep during 5
//      - The task2 tries to catch the Sem2 during 8 seconds
//      - When the task1 wakes up, signals the Sem2
//      - In order to the Sem2 signal of the task1, the task2 can catch the Sem2
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


DEFINE_TEST(3);

/* All functions must have thr "test_name_" prefix */

void *test3_thread0 (void * ptr) {

    printf("Start: Waiting 2 seconds\n");
    Pr_DelayIn(Pr_Time(2.0f));
    printf("Test Start\n");
    sembin[2]->Signal();
    return NULL;
}

void *test3_thread1 (void* ptr) {

    sembin[0]->Wait();
    
    printf("I am task1:\n");
    printf("I am task1:  I am going to sleep 5 seconds\n");
    
    Pr_Time time0, time1;

    time0.GetTime();
    Pr_DelayIn(Pr_Time(5.0f));
    time1.GetTime();
    
    float aux, aux1;
    
    aux = time0.Secns();
    aux1 = time1.Secns();
    
    sembin[1]->Signal();
    
    printf("I am task1: Interval between %d and %d must be 5 seconds\n",
            (uint32)aux, (uint32)aux1 );

    printf("I am task1: delayed time is %dus; requested 5000ms",
            (uint32) (aux1 * 1000000.0f - aux * 1000000.0f));


    printf("\ntask1 End\n");
    return NULL;
}

void *test3_thread2 (void* ptr) {

    sembin[2]->Wait();
    sembin[0]->Signal();  
    
    printf("\nI am the task2\n");
    printf("I am task2:, try to catch Sem2 during 1 second\n");
    
    if (sembin[1]->WaitTimed(Pr_Time(1)))
        printf("I am task2: OK I lock the Sem2\n");
    else
        printf("I am task2: ERROR I do not have enough time to catch Sem2\n");
        
    printf("I am task2, wait 8 seconds to catch Sem2\n");  

    if (sembin[1]->WaitTimed(Pr_Time(8)))
        printf("I am task2: OK I lock the Sem2\n");
    else
        printf("I am task2: ERROR I do not have enough time to catch Sem2\n");
              
    printf("\ntask2 End\n");
    
    Test3.End();
    return NULL;
}

void* test3_main (void *ptr) 
{
    Test3.Init();
    
#ifdef _EDROOM_IS_DEBUG_

    Pr_SemaphoreBin pSem0("semaphore2", 0);
    Pr_SemaphoreBin pSem1("semaphore2", 0);
    Pr_SemaphoreBin pSem2("semaphore2", 0);
    
    Pr_Task ptask0(test3_thread0,"taskStart",EDROOMprioVeryHigh,512);   
    Pr_Task ptask1(test3_thread1,"task1",EDROOMprioNormal,512);
    Pr_Task ptask2(test3_thread2,"task2",EDROOMprioNormal,512);
    
#else
    
    Pr_SemaphoreBin pSem0(0);
    Pr_SemaphoreBin pSem1(0);
    Pr_SemaphoreBin pSem2(0);
    
    Pr_Task ptask0(test3_thread0, EDROOMprioVeryHigh,512);
    Pr_Task ptask1(test3_thread1, EDROOMprioNormal,512);
    Pr_Task ptask2(test3_thread2, EDROOMprioNormal,512);
    
#endif

    sembin[0] = &pSem0;
    sembin[1] = &pSem1;
    sembin[2] = &pSem2;
   
    task[1] = &ptask1;
    task[2] = &ptask2;
    task[0] = &ptask0;
    
    printf("\n\nStarting TEST3:\nTest for Pr_Time and GetTime\n\n");
      
    Test3.Start();  

    printf("\n\nTEST3: Finished!\n\n");
    return NULL;
}
