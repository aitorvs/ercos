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

DEFINE_TEST(2);

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//	Test for the Pr_Task friend functions Pr_Send and Pr_Receive
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


bool SynchrERROR;

float dataF;


void *test2_thread1 (void *ptr) {

	unsigned int dataI;



	printf("I am task1: Wait to catch Sem\n");
   

	sembin[0]->Wait();


	dataI = 55;

	printf("I am task1: I Send dataI = %u\n", dataI);

	Pr_Send(*task[2], &dataI);
 
    printf("I am task1: integer data sent OK\n");
 

	dataF = 0.567f;
   
    printf("I am task1: I Send dataF = %d\n", (uint32)(dataF * 1000));

	Pr_Send(*task[2], &dataF);
    
    printf("I am task1: float data sent OK\n");


	if (SynchrERROR)
        printf("I am task1: Synchr ERROR. Sender must Wait\n");

   
    printf("task1 End\n");
    return NULL;
}


void *test2_thread2 (void *ptr) {
	
    unsigned int data;

 
    
    printf("I am task2: Wait Sem2 \n");

	sembin[1]->Wait();

	printf("I am task2: Signal Sem \n");

    sembin[0]->Signal();   


	printf("I am task2: Ready to receive integer data \n");

    Pr_Receive(&data,sizeof(data));
    
    printf("I am task2: Integer data received\n");


	if (data != 55) 
        printf("I am task2: ERROR in reception Int %d\n", data)  ;

	else 
        printf("I am task2: reception Int %d OK\n", data) ;


	SynchrERROR = 0;

	printf("I am task2: Ready to receive float data \n");
    
    float data2;
    Pr_Receive(&data2, sizeof(data2));
   
    printf("I am task2: Float data received\n");
    
	if (data2 == 0.567f) {
        printf("I am task2: reception Float %d/1000 OK\n", (uint32)(data2 * 1000));
	}
	else {
			printf("I am task2: ERROR in reception Float %d/1000\n", (uint32)(data2 * 1000));
	}

   
    printf("task2 End\n");

	Test2.End();

    return NULL;
}


void *test2_thread0 (void *ptr)

{
    
    printf("Starting the test\n");
    sembin[1]->Signal();
    return NULL;
}


void * test2_main(void *ptr)
{

    SynchrERROR = 1;
    
    Test2.Init();
    
#ifdef _EDROOM_IS_DEBUG_

    Pr_SemaphoreBin sem("Sem", 0);
    Pr_SemaphoreBin sem2("Sem2", 0);
    
    Pr_Task ptask1(test2_thread1, "task1", EDROOMprioHigh,512);
    Pr_Task ptask2(test2_thread2, "task2", EDROOMprioNormal,512);
    Pr_Task ptask3(test2_thread0, "taskStart", EDROOMprioVeryHigh,512);
    
#else

    Pr_SemaphoreBin sem(0);
    Pr_SemaphoreBin sem2(0);
    
    Pr_Task ptask1(test2_thread1, EDROOMprioHigh,512);
    Pr_Task ptask2(test2_thread2, EDROOMprioNormal,512);
    Pr_Task ptask3(test2_thread0, EDROOMprioVeryHigh,512);
    
#endif    

    sembin[0] = &sem;
    sembin[1] = &sem2;
    
    task[1] = &ptask1;
    task[2] = &ptask2;
    task[0] = &ptask3;

    printf("\n\nStarting TEST2:\nTest for Pr_Send and Pr_Receive\n\n");
    
    Test2.Start();
   
    printf("\n\nTEST2: Finished!\n\n"); 
    return NULL;
}


