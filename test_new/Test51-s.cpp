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

#include "edroombp.h"

#include <stdio.h>

#include "global_test.h"

/* This test runs on Nanosat board with values for the DELAY macro ranging from
 * 17500 to 22000 (270ms - 338ms); 375ms (3s/8) is obtained with ~24300
 * iterations. */
//#define PRINTF(fmt, args...) printf (fmt, ##args)
#define PRINTF(fmt, args...)

DEFINE_TEST(51s);

/* Passing mark variable to check test correctness */

int passing_mark = 4;
int erroneus = 0;

void *test51s_thread0(void *ptr)
{
  	printf("Test51-s: Initializing test!\n");
	printf("Test51-s: Running...  ");
	
	SPIN();
	
	if (passing_mark == 0)
		passing_mark = 2;
	else
		erroneus = 1;
	
	PRINTF ("T0 D (errn=%d)\n", erroneus);
	DELAY();
    
	PRINTF ("T0 W0 (errn=%d)\n", erroneus);
	semrec[0]->Wait();
	
	PRINTF ("T0 D (errn=%d)\n", erroneus);
	DELAY();

	PRINTF ("T0 W1 (errn=%d)\n", erroneus);
	semrec[1]->Wait();
	
	if (passing_mark == 0)
		passing_mark = 3;
	else
		erroneus = 2;
		
	PRINTF ("T0 D (errn=%d)\n", erroneus);
	DELAY();
	
	if (passing_mark == 0)
		passing_mark = 4;
	else
		erroneus = 3;
		
	PRINTF ("T0 S1 (errn=%d)\n", erroneus);
	semrec[1]->Signal();
	
	PRINTF ("T0 D (errn=%d)\n", erroneus);
	DELAY();
	
	if (passing_mark == 0)
		passing_mark = 3;
	else
		erroneus = 4;
				
	PRINTF ("T0 S0 (errn=%d)\n", erroneus);
	semrec[0]->Signal();
	
	PRINTF ("T0 D (errn=%d)\n", erroneus);
	DELAY();
	
	if (passing_mark == 0)
		passing_mark = 0;
	else
		erroneus = 5;
	
	if (!erroneus)
		printf("\nTest51-s: Mission... ACCOMPLISHED!\n");
	else
		printf("\nTest51-s: Mission... FAILED (erroneous=%d)!\n", erroneus);
	Test51s.End();
		
    return NULL;
}

void *test51s_thread1(void *ptr)
{
	if (passing_mark == 1)
		passing_mark = 0;
	else
		erroneus = 6;
			
	PRINTF ("T1 Sl2.25 (errn=%d)\n", erroneus);
    Pr_DelayAt(Pr_Time(2.25f) += Test51s.start_time);
    
	PRINTF ("T1 D (errn=%d)\n", erroneus);
	DELAY();
    
   	if (passing_mark == 1)
		passing_mark = 0;   	
	else
		erroneus = 7;
    return NULL;
}

void *test51s_thread2(void *ptr)
{
	if (passing_mark == 2)
		passing_mark = 1;
	else
		erroneus = 8;

	PRINTF ("T2 Sl0.5 (errn=%d)\n", erroneus);
    Pr_DelayAt(Pr_Time(0.5f) += Test51s.start_time);
      
	PRINTF ("T2 D (errn=%d)\n", erroneus);
	DELAY();
    
   	if (passing_mark == 2)
		passing_mark = 0;
	else
		erroneus = 9;
			  
	PRINTF ("T2 W0 (errn=%d)\n", erroneus);
  	semrec[0]->Wait();
  	
	PRINTF ("T2 D (errn=%d)\n", erroneus);
	DELAY();
    
	PRINTF ("T2 S0 (errn=%d)\n", erroneus);
    semrec[0]->Signal();
    
	PRINTF ("T2 D (errn=%d)\n", erroneus);
	DELAY();
    
    if (passing_mark == 2)
		passing_mark = 1;
	else
		erroneus = 10;

    return NULL;
}

void *test51s_thread3(void *ptr)
{
	if (passing_mark == 3)
		passing_mark = 2;
	else
		erroneus = 11;
			
	PRINTF ("T3 Sl1.25 (errn=%d)\n", erroneus);
    Pr_DelayAt(Pr_Time(1.25f) += Test51s.start_time);
    
	PRINTF ("T3 D (errn=%d)\n", erroneus);
	DELAY();
    
    if (passing_mark == 3)
		passing_mark = 4;
	else
		erroneus = 12;
    
	PRINTF ("T3 W2 (errn=%d)\n", erroneus);
    semrec[2]->Wait();
        
	PRINTF ("T3 D (errn=%d)\n", erroneus);
	DELAY();
    
    if (passing_mark == 3)
		passing_mark = 0;
	else
		erroneus = 13;
    
	PRINTF ("T3 W0 (errn=%d)\n", erroneus);
    semrec[0]->Wait();
    
	PRINTF ("T3 D (errn=%d)\n", erroneus);
	DELAY();
    
	PRINTF ("T3 S0 (errn=%d)\n", erroneus);
    semrec[0]->Signal();
    
	PRINTF ("T3 D (errn=%d)\n", erroneus);
	DELAY();
    
    if (passing_mark == 3)
		passing_mark = 4;
	else
		erroneus = 14;
           
	PRINTF ("T3 S2 (errn=%d)\n", erroneus);
    semrec[2]->Signal();
    
	PRINTF ("T3 D (errn=%d)\n", erroneus);
	DELAY();
    
    if (passing_mark == 3)
		passing_mark = 2;
	else
		erroneus = 15;
             
    return NULL;
}

void *test51s_thread4(void *ptr)
{
	if (passing_mark == 4)
		passing_mark = 3;
	else
		erroneus = 16;
	
	PRINTF ("T4 Sl1.75 (errn=%d)\n", erroneus);
    Pr_DelayAt(Pr_Time(1.75f) += Test51s.start_time);

	PRINTF ("T4 D (errn=%d)\n", erroneus);
	DELAY();

    if (passing_mark == 4)
		passing_mark = 0;
	else
		erroneus = 17;
	    
	PRINTF ("T4 W1 (errn=%d)\n", erroneus);
    semrec[1]->Wait();
    
	PRINTF ("T4 D (errn=%d)\n", erroneus);
	DELAY();
    
    if (passing_mark == 4)
		passing_mark = 3;
	else
		erroneus = 18;
		
	PRINTF ("T4 W2 (errn=%d)\n", erroneus);
    semrec[2]->Wait();
    
	PRINTF ("T4 D (errn=%d)\n", erroneus);
	DELAY();
    
	PRINTF ("T4 S2 (errn=%d)\n", erroneus);
    semrec[2]->Signal();
    
	PRINTF ("T4 D (errn=%d)\n", erroneus);
	DELAY();
  
	PRINTF ("T4 S1 (errn=%d)\n", erroneus);
    semrec[1]->Signal();
    
	PRINTF ("T4 D (errn=%d)\n", erroneus);
	DELAY();
    
    if (passing_mark == 4)
		passing_mark = 3;
	else
		erroneus = 19;
    
    return NULL;
}

void *test51s_main(void *ptr)
{
	Test51s.Init();
        
#ifdef _EDROOM_IS_DEBUG_

    Pr_SemaphoreRec s1("s1");
    Pr_SemaphoreRec s2("s2");
    Pr_SemaphoreRec s3("s3");

    Pr_Task ptask0(test51s_thread0, "Very Low Priority Task", EDROOMprioVeryLow, 512);
    Pr_Task ptask1(test51s_thread1, "Low Priority Task", EDROOMprioLow, 512);
    Pr_Task ptask2(test51s_thread2, "Normal Priority Task", EDROOMprioNormal, 512);
	Pr_Task ptask3(test51s_thread3, "High Priority Task", EDROOMprioHigh, 512);
    Pr_Task ptask4(test51s_thread4, "Very High Priority Task", EDROOMprioVeryHigh, 512);

#else

    Pr_SemaphoreRec s1;
    Pr_SemaphoreRec s2;
    Pr_SemaphoreRec s3;

    Pr_Task ptask0(test51s_thread0, EDROOMprioVeryLow, 512);
    Pr_Task ptask1(test51s_thread1, EDROOMprioLow, 512);
    Pr_Task ptask2(test51s_thread2, EDROOMprioNormal, 512);
	Pr_Task ptask3(test51s_thread3, EDROOMprioHigh, 512);
    Pr_Task ptask4(test51s_thread4, EDROOMprioVeryHigh, 512);

#endif
   
    semrec[0] = &s1;
    semrec[1] = &s2;
    semrec[2] = &s3;
    
    task[0] = &ptask0;
    task[1] = &ptask1;
    task[2] = &ptask2;
    task[3] = &ptask3;
    task[4] = &ptask4;  
    
    printf("\n\nStarting TEST51s:\nSilent Resource Semaphore Test\n\n");
    
  	Test51s.Start();
     
    printf("\n\nTEST51s: Finished!\n\n");
    return NULL;
}

