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
 */

#include "edroombp.h"
#include <stdio.h>
#include "global_test.h"

DEFINE_TEST(5);

void *test5_thread0(void *ptr)
{
    printf("task0: delay 3.5\n");
    Pr_DelayAt(Pr_Time(3.5f) += Test5.start_time);
    
    printf("task0: Wait(S2)\n");
    semrec[1]->Wait();

    printf("task0: This must be the message 4\n");
    
    printf("task0: finish\n");
    
    return NULL;
}

void *test5_thread1(void *ptr)
{
    printf("task1: delay 2.0\n");
    Pr_DelayAt(Pr_Time(2.0f) += Test5.start_time);

    printf("task1: Wait(S1)\n");
    semrec[0]->Wait();
    
    printf("task1: This must be the message 6\n");

    return NULL;
}

void *test5_thread2(void *ptr)
{
    printf("task2: delay 2.5\n");
    Pr_DelayAt(Pr_Time(2.5f) += Test5.start_time);    

    printf("task2: This must be the message 2\n");
    
    printf("task2: delay 4.0\n");
    Pr_DelayAt(Pr_Time(4.0) += Test5.start_time);
    
    printf("task2: This must be the message 7\n");
    
    printf("task2: finish\n");
    return NULL;
}

void *test5_thread3(void *ptr)
{
    printf("task3: Wait(S1)\n");
    semrec[0]->Wait();

    printf("task3: delay at 2.5\n");
    Pr_DelayAt(Pr_Time(2.5) += Test5.start_time);
    
    printf("task3: This must be the message 1\n");
    
    printf("task3: Wait(S2)\n");
    semrec[1]->Wait();
    
    printf("task3: delay at 4.0\n");
    Pr_DelayAt(Pr_Time(4.0) += Test5.start_time);
    
    printf("task3: This must be the message 3\n");
    
    printf("task3: Signal(S2)\n");
    semrec[1]->Signal();
    
    printf("task3: message\n");

    printf("task3: Signal(S1)\n");
    semrec[0]->Signal();
    
    printf("task3: This must be the message last message\n");

    printf("task3: finish\n");
    
    Test5.End();

    return NULL;
}

void *test5_thread4(void *ptr)
{
    printf("task4: delay 4.0\n");
    Pr_DelayAt(Pr_Time(4.0f) += Test5.start_time);    
    
    printf("task4: This must be the message 5\n");

    printf("task4: finish\n");
    return NULL;
}



void * test5_main(void *ptr)
{

    Test5.Init();
       
#ifdef _EDROOM_IS_DEBUG_

    Pr_SemaphoreRec pS1("S1");
    Pr_SemaphoreRec pS2("S2");
    Pr_SemaphoreRec pS3("S3");

    Pr_Task ptask0(test5_thread0, "task", EDROOMprioVeryHigh, 512);
    Pr_Task ptask1(test5_thread1, "task", EDROOMprioHigh, 512);
    Pr_Task ptask2(test5_thread2, "task2", EDROOMprioNormal, 512);
    Pr_Task ptask3(test5_thread3, "task3", EDROOMprioLow, 512);
    Pr_Task ptask4(test5_thread4, "task", EDROOMprioHigh, 512);
    
#else

    Pr_SemaphoreRec pS1();
    Pr_SemaphoreRec pS2();
    Pr_SemaphoreRec pS3();

    Pr_Task ptask0(test5_thread0, EDROOMprioVeryHigh, 512);
    Pr_Task ptask1(test5_thread1, EDROOMprioHigh, 512);
    Pr_Task ptask2(test5_thread2, EDROOMprioNormal, 512);
    Pr_Task ptask3(test5_thread3, EDROOMprioLow, 512);
    Pr_Task ptask4(test5_thread4, EDROOMprioHigh, 512);
    
#endif
   
    semrec[0] = &pS1;
    semrec[1] = &pS2;
    semrec[2] = &pS3;
    
    task[0] = &ptask0;
    task[1] = &ptask1;
    task[2] = &ptask2;
    task[3] = &ptask3;
    task[4] = &ptask4;

    printf("\n\nStarting TEST5:\nResource Semaphore Test\n\n");
    
    Test5.Start();

    printf("\n\nTEST5: Finished!\n\n");
    return NULL;
}

