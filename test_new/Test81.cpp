/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT project.
*/

/*
 * This test checks the implementation of the Pr_IRQManager methods and operators.
 */
 
#include "edroombp.h"
#include <stdio.h>
#include "global_test.h"

/* Interrupts list */

#define INT0    0x12
#define INT1    0x13
#define INT2    0x1A
#define INT3    0x1B
#define INT4    0x1E    /* Ethernet */
#define RTC		0x1D

DEFINE_TEST(81);

void test81_handler_irq0(int level)
{
	int i = 0;

#ifdef _EDROOM_IS_DEBUG_
    Pr_IRQEvent event("interrupt", level);
#else
    Pr_IRQEvent event(level);
#endif
    
    printf("handler_irq0: IRQ 0 triggered. Signalling the event\n", level);
    event.Signal();   
    printf("handler_irq0: Spinning for 3 secs\n");	
		for (i = 0; i < 8000000; i++);
	printf("handler_irq0: Finished!\n");
}

void test81_handler_rtc(int level)
{
	int i = 0;

#ifdef _EDROOM_IS_DEBUG_
    Pr_IRQEvent event("interrupt", level);
#else
    Pr_IRQEvent event(level);
#endif
    
    printf("handler_rtc: RTC triggered. Signalling the event\n", level);
    event.Signal();   
    printf("handler_rtc: Spinning for 3 secs\n");	
		for (i = 0; i < 8000000; i++);
	printf("handler_rtc: Finished!\n");
}

void *test81_thread0(void* ptr)
{
	int i = 0;
	
	printf("Task0: Hello! Registering ISR for INT0 and RTC\n");
    Pr_IRQManager::Install(test81_handler_irq0, INT0);
    Pr_IRQManager::Install(test81_handler_rtc, RTC);
    
    printf("Task0: ISRs Registered! Spinning for 3 secs\n");	
	for (i = 0; i < 8000000; i++);
	
	printf("Task0: Forcing INT0\n"); 
    Pr_IRQManager::Force((INT0 - 0x10));
    
    printf("Task0: Spinning for 3 secs\n");
	for (i = 0; i < 8000000; i++);
	
	printf("Task0: Finished!\n");
	
	Pr_IRQManager::Restore(INT0);
//	Pr_IRQManager::Restore(RTC);
	
	Test81.End();

}

void *test81_thread1(void* ptr) 
{
	int i = 0;
#ifdef _EDROOM_IS_DEBUG_
    Pr_IRQEvent eventINT0("intr0", INT0);
#else
    Pr_IRQEvent eventINT0(INT0);
#endif

    printf("Task 1: I am going to wait untile INT0\n");
    eventINT0.Wait();
    
    printf("Task1: IRQ0 Woken up! Now spinning for 3 secs\n");
    for (i = 0; i < 8000000; i++);
     
    printf("Task1: Finished!\n");  
}

void *test81_thread2(void* ptr) 
{
	int i = 0;
	
	printf("Task2: Hello! I'll wake up at instant 4 secs\n");
	Pr_DelayAt(Pr_Time(4.0f) += Test81.start_time);
	
	printf("Task2: Woken up! Now spinning for 3 secs\n");
    for (i = 0; i < 8000000; i++);
    
    printf("Task2: Finished!\n");
}

void * test81_main (void *ptr) {
  
    Test81.Init();

#ifdef _EDROOM_IS_DEBUG_
    
    Pr_Task ptask0(test81_thread0, "tarea", EDROOMprioNormal, (4*1024));
    Pr_Task ptask1(test81_thread1, "tarea", EDROOMprioHigh, (4*1024));
    Pr_Task ptask2(test81_thread2, "tarea", EDROOMprioVeryHigh, (4*1024));

#else

    Pr_Task ptask0(test81_thread0, EDROOMprioNormal, (4*1024));
    Pr_Task ptask1(test81_thread1, EDROOMprioHigh, (4*1024));
    Pr_Task ptask2(test81_thread2, EDROOMprioVeryHigh, (4*1024));

#endif

    task[0] = &ptask0;    
    task[1] = &ptask1;
    task[2] = &ptask2;

    printf("\n\nStarting TEST81:\nYet another test for implementation of Pr_IRQManager methods\n\n");
    
    Test81.Start();

    printf("\n\nTEST8: Finished!\n\n");   
}

