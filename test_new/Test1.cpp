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

DEFINE_TEST(1);

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Resource Semaphore Test 
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/* All functions must start with "name_test_" prefix */


void *test1_thread0(void *ptr){

    printf("Task 1: Begin\n");
    
    semrec[0]->Wait();
    
    printf("Task 1: Delay at 2.5\n");
    
    Pr_DelayAt(Pr_Time(2.5) += Test1.start_time);
    
    printf("Task 1: This must be the Message 1\n");
    
    semrec[0]->Signal();
    
    printf("Task 1: This must be the Message 5\n");
    
    semrec[0]->Wait();
    
    task[1]->SetMaxPrioTmp(EDROOMprioURGENT);
    
    semrec[0]->Signal();
    
    printf("Task 1: This must be the Message 7\n");
    return NULL; 
}


void *test1_thread1(void *ptr){


    printf("Task 2: Begin\n");
    
    printf("Task 2: Delay at 2.5\n");
    
    Pr_DelayAt(Pr_Time(2.5) += Test1.start_time);
    
    printf("Task 2: This must be the message 3\n");
    
    semrec[0]->Wait();
    
    printf("Task 2: This must be the Message 4\n");
    
    task[1]->SetMaxPrioMsg(EDROOMprioHigh);
    
    task[0]->SetMaxPrioMsg(EDROOMprioVeryHigh);
    
    semrec[0]->Signal();
    
    printf("Task 2: This must be the Message 6\n");
    
    semrec[0]->Wait();
    
    task[1]->RestorePrioMsg();
    
    semrec[0]->Signal();
    
    printf("Task 2: This must be the Message 8\n");
    return NULL;   
}

void *test1_thread2(void *ptr){

    printf("Task 3: Begin\n");
    
    printf("Task 3: Delay at 2.0\n");
     
    Pr_DelayAt(Pr_Time(2.0) += Test1.start_time);
    
    printf("Task 3: Waits the semaphore\n");
    
    semrec[0]->Wait();
    
    printf("Task 3: This must be the message 2\n");
       
    printf("Task 3: Decrease my priority\n");
    
    task[2]->SetPriority(EDROOMprioVeryLow);
    
    printf("Task 3: Signals the semaphore\n");
    
    semrec[0]->Signal();
    
    printf("Task 3: This must be the last message\n");
    
    Test1.End();
    return NULL;
}


void * test1_main(void *ptr)
{
    Test1.Init();
       
#ifdef _EDROOM_IS_DEBUG_

    Pr_SemaphoreRec sem("Sem");
    
    Pr_Task ptask0(test1_thread0, "task", EDROOMprioLow, 512);
    Pr_Task ptask1(test1_thread1, "task2", EDROOMprioNormal, 512);
    Pr_Task ptask2(test1_thread2, "task3", EDROOMprioHigh, 512);

#else

    Pr_SemaphoreRec sem;
    
    Pr_Task ptask0(test1_thread0, EDROOMprioLow, 512);
    Pr_Task ptask1(test1_thread1, EDROOMprioNormal, 512);
    Pr_Task ptask2(test1_thread2, EDROOMprioHigh, 512);

#endif

    semrec[0] = &sem;
    
    task[0] = &ptask0;
    task[1] = &ptask1;
    task[2] = &ptask2;
    
    printf("\n\nStarting TEST1:\nResource Semaphore Test\n\n");
    
    Test1.Start();
    
    printf("\n\nTEST1: Finised!\n\n");
    return NULL;
}

/* vim:set ts=4 et: */
