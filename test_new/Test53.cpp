
/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez, Cesar Rodriguez Ortega
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

DEFINE_TEST(9);

#define PRINT(task,fmt,args...) \
	clock_gettime (CLOCK_REALTIME, &tp); \
	printf ("%03d,%03ds: T%d: " fmt "\n", tp.tv_sec, tp.tv_nsec / 1000000, \
			task, ##args);

void *test9_thread0(void *ptr) {
	struct timespec tp;

	PRINT (0, "Alive, starting!");

	PRINT (0, "Delaying until 3.5s");
	Pr_DelayAt(Pr_Time(3.5f) += Test9.start_time);

	PRINT (0, "Running at 3.5s; waiting for S1");
	semrec[1]->Wait();

	PRINT (0, "Running after wait for S1; This must be the message 4");

	PRINT (0, "Bye!");
	return NULL;
}

void *test9_thread1(void *ptr) {
	struct timespec tp;

	PRINT (1, "Alive, starting!");

	PRINT (1, "Delaying until 2.0s");
	Pr_DelayAt(Pr_Time(2.0f) += Test9.start_time);

	PRINT (1, "Running at 2.0s; waiting for S0");
	semrec[0]->Wait();

	PRINT (1, "Running after wait for S0; This must be the message 6");

	PRINT (1, "Bye!");
	return NULL;
}

void *test9_thread2(void *ptr) {
	struct timespec tp;

	PRINT (2, "Alive, starting!");

	PRINT (2, "Delaying until 2.5s");
	Pr_DelayAt(Pr_Time(2.5f) += Test9.start_time);    

	PRINT (2, "Running at 2.5s; This must be the message 2");

	PRINT (2, "Delaying until 4.0s");
	Pr_DelayAt(Pr_Time(4.0) += Test9.start_time);

	PRINT (2, "Running at 4.0s; This must be the message 7");

	PRINT (2, "Bye!");
	return NULL;
}

void *test9_thread3(void *ptr) {
	struct timespec tp;

	PRINT (3, "Alive, starting!");

	PRINT (3, "Waiting for S0");
	semrec[0]->Wait();

	PRINT (3, "After wait for S0; delaying until 2.5s");
	Pr_DelayAt(Pr_Time(2.5) += Test9.start_time);

	PRINT (3, "Running at 2.5s; This must be the message 1");
	PRINT (3, "Waiting for S1");
	semrec[1]->Wait();

	PRINT (3, "Running after wait for S1; delaying 4.0s");
	Pr_DelayAt(Pr_Time(4.0) += Test9.start_time);

	PRINT (3, "Running at 4.0s; This must be the message 3");
	PRINT (3, "Signaling S1");
	semrec[1]->Signal();

	PRINT (3, "Signaling S0");
	semrec[0]->Signal();

	PRINT (3, "This must be the message last message");

	PRINT (3, "Bye!");
	Test9.End();

	return NULL;
}

void *test9_thread4(void *ptr) {
	struct timespec tp;

	PRINT (4, "Alive, starting!");

	PRINT (4, "Delaying until 4.0s");
	Pr_DelayAt(Pr_Time(4.0f) += Test9.start_time);    

	PRINT (4, "Running at 4.0s; This must be the message 5");

	PRINT (4, "Bye!");
	return NULL;
}

void * test9_main(void *ptr)
{

    Test9.Init();
       
#ifdef _EDROOM_IS_DEBUG_

    Pr_SemaphoreRec pS1("S1");
    Pr_SemaphoreRec pS2("S2");
    Pr_SemaphoreRec pS3("S3");

    Pr_Task ptask0(test9_thread0, "task", EDROOMprioVeryHigh, 512);
    Pr_Task ptask1(test9_thread1, "task", EDROOMprioHigh, 512);
    Pr_Task ptask2(test9_thread2, "task2", EDROOMprioNormal, 512);
    Pr_Task ptask3(test9_thread3, "task3", EDROOMprioLow, 512);
    Pr_Task ptask4(test9_thread4, "task", EDROOMprioHigh, 512);
    
#else

    Pr_SemaphoreRec pS1();
    Pr_SemaphoreRec pS2();
    Pr_SemaphoreRec pS3();

    Pr_Task ptask0(test9_thread0, EDROOMprioVeryHigh, 512);
    Pr_Task ptask1(test9_thread1, EDROOMprioHigh, 512);
    Pr_Task ptask2(test9_thread2, EDROOMprioNormal, 512);
    Pr_Task ptask3(test9_thread3, EDROOMprioLow, 512);
    Pr_Task ptask4(test9_thread4, EDROOMprioHigh, 512);
    
#endif
   
    semrec[0] = &pS1;
    semrec[1] = &pS2;
    semrec[2] = &pS3;
    
    task[0] = &ptask0;
    task[1] = &ptask1;
    task[2] = &ptask2;
    task[3] = &ptask3;
    task[4] = &ptask4;

    printf("\n\nStarting TEST9:\nResource Semaphore Test\n\n");
    
    Test9.Start();

    printf("\n\nTEST9: Finished!\n\n");
    return NULL;
}

