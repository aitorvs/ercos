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
 *  Resurce semaphore test with priority ceiling
 */

#include "edroombp.h"

#include <stdio.h>

#include "global_test.h"

DEFINE_TEST(52);

/* Passing mark variable to check test correctness */

int passing_mark = 4;
int erroneus = 0;

void *test52_thread0(void *ptr)
{

    if (passing_mark != 0)
        erroneus = 1;
    
	DELAY();
    
	semrec[0]->Wait();
	
	DELAY();

	semrec[1]->Wait();
	
	DELAY();
	
	if (passing_mark == 0)
		passing_mark = 4;
	else
		erroneus = 2;
		
	semrec[1]->Signal();
	
    if (passing_mark != 0)
        erroneus = 3;

	DELAY();
	
    if (passing_mark == 0)
        passing_mark = 2;
    else
        erroneus = 4;

	semrec[0]->Signal();
	
    if (passing_mark != 0)
        erroneus = 5;

	DELAY();

	semrec[1]->Wait();

	DELAY();

    if (passing_mark == 0)
        passing_mark = 4;
    else
        erroneus = 6;

	semrec[1]->Signal();
	
    if (passing_mark != 0)
        erroneus = 7;

    DELAY();

    if (!erroneus)
        printf("\nTest52: Mission... ACCOMPLISHED!\n");
    else
        printf("\nTest52: Mission... FAILED (erroneus=%d)!\n", erroneus);

	Test52.End();
		
    return NULL;
}

void *test52_thread1(void *ptr)
{
	if (passing_mark == 1)
		passing_mark = 0;
	else
		erroneus = 8;
			
    Pr_DelayAt(Pr_Time(3.25f) += Test52.start_time);
    
    if (passing_mark != 1)
        erroneus = 9;

	DELAY();
    
    if (passing_mark == 1)
        passing_mark = 0;
    else
        erroneus = 10;

    return NULL;
}

void *test52_thread2(void *ptr)
{
	if (passing_mark == 2)
		passing_mark = 1;
	else
		erroneus = 11;

    Pr_DelayAt(Pr_Time(2.0f) += Test52.start_time);

    if (passing_mark != 2)
        erroneus = 12;

	DELAY();
    
  	semrec[0]->Wait();
  	
	DELAY();
    
	semrec[0]->Signal();
    
	DELAY();

    if (passing_mark == 2)
        passing_mark = 1;
    else
        erroneus = 13;

	Pr_DelayAt(Pr_Time(5.125f) += Test52.start_time);

    if (passing_mark != 2)
        erroneus = 14;

	DELAY();

    if (passing_mark == 2)
        passing_mark = 0;
    else
        erroneus = 16;

    return NULL;
}

void *test52_thread3(void *ptr)
{
	if (passing_mark == 3)
		passing_mark = 2;
	else
		erroneus = 17;
			
    Pr_DelayAt(Pr_Time(1.75f) += Test52.start_time);
    
	DELAY();

    if (passing_mark == 3)
        passing_mark = 0;
    else
        erroneus = 18;
    
    return NULL;
}

void *test52_thread4(void *ptr)
{
    printf("Running the Test... ");
    
	if (passing_mark == 4)
		passing_mark = 3;
	else
		erroneus = 19;

    Pr_DelayAt(Pr_Time(0.875f) += Test52.start_time);

    if (passing_mark != 4)
        erroneus = 20;

	DELAY();

	semrec[1]->Wait();
    
	DELAY();
    
    semrec[1]->Signal();
    
	DELAY();

    if (passing_mark == 4)
        passing_mark = 3;
    else
        erroneus = 21;

	Pr_DelayAt(Pr_Time(5.0f) += Test52.start_time);
     
    if (passing_mark != 4)
        erroneus = 22;

	DELAY();

    if (passing_mark == 4)
        passing_mark = 2;
    else
        erroneus = 23;
    
    return NULL;
}

void *test52_main(void *ptr)
{
	Test52.Init();
        
#ifdef _EDROOM_IS_DEBUG_

    Pr_SemaphoreRec s1("s1", EDROOMprioNormal);
    Pr_SemaphoreRec s2("s2", EDROOMprioVeryHigh);

    Pr_Task ptask4(test52_thread4, "Very High Priority Task", EDROOMprioVeryHigh, 512);
    Pr_Task ptask3(test52_thread3, "High Priority Task", EDROOMprioHigh, 512);
    Pr_Task ptask2(test52_thread2, "Normal Priority Task", EDROOMprioNormal, 512);
    Pr_Task ptask1(test52_thread1, "Low Priority Task", EDROOMprioLow, 512);
    Pr_Task ptask0(test52_thread0, "Very Low Priority Task", EDROOMprioVeryLow, 512);

#else

    Pr_SemaphoreRec s1(EDROOMprioNormal);
    Pr_SemaphoreRec s2(EDROOMprioVeryHigh);

    Pr_Task ptask0(test52_thread0, EDROOMprioVeryLow, 512);
    Pr_Task ptask1(test52_thread1, EDROOMprioLow, 512);
    Pr_Task ptask2(test52_thread2, EDROOMprioNormal, 512);
	Pr_Task ptask3(test52_thread3, EDROOMprioHigh, 512);
    Pr_Task ptask4(test52_thread4, EDROOMprioVeryHigh, 512);

#endif
   
    semrec[0] = &s1;
    semrec[1] = &s2;
    
    task[0] = &ptask0;
    task[1] = &ptask1;
    task[2] = &ptask2;
    task[3] = &ptask3;
    task[4] = &ptask4;  
    
    printf("\n\nStarting TEST52:\nPriority Ceiling Resource Semaphore Test\n\n");
    
  	Test52.Start();
     
    printf("\n\nTEST52: Finished!\n\n");
    return NULL;
}




