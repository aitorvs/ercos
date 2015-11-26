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

#define INT0    0x12
#define INT1    0x13
#define INT2    0x1A
#define INT3    0x1B
#define INT4    0x1E    /* Ethernet */

DEFINE_TEST(8);

/** Define all the IRQ events   */
#ifdef _EDROOM_IS_DEBUG_
    Pr_IRQEvent eventINT0("intr0", INT0);
    Pr_IRQEvent eventINT2("intr2", INT2);
    Pr_IRQEvent eventINT1("intr1", INT1);
    Pr_IRQEvent eventINT3("intr3", INT3);
#else
    Pr_IRQEvent eventINT0(INT0);
    Pr_IRQEvent eventINT2(INT2);
    Pr_IRQEvent eventINT1(INT1);
    Pr_IRQEvent eventINT3(INT3);
#endif

void test8_handler(int level)
{

    
    printf("handler: IRQ 0x%x triggered, anoting the event\n", level);
    
    switch(level)
    {
        case INT0:
            eventINT0.Signal();
            break;
        case INT1:
            eventINT1.Signal();
            break;
        case INT2:
            eventINT2.Signal();
            break;
        case INT3:
            eventINT3.Signal();
            break;
    }
}

void *test8_thread1(void* ptr) {

    printf("Task 1: I am going to wait until INT0\n");
    eventINT0.Wait();
    printf("Task1: INT0 received\n");
    
    printf("Task 1: I am going to wait until INT2\n");
    eventINT2.Wait();
    printf("Task1: INT2 received\n");
    
    printf("Task 1: End\n");
    
    return NULL;
}

void *test8_thread2(void* ptr) {

    printf("Task 2: I am going to wait until INT1\n");
    eventINT1.Wait();
    printf("Task2: INT1 received\n");

    printf("Task 2: I am going to wait until INT3\n");
    eventINT3.Wait();
    printf("Task2: INT3 received\n");

    printf("Task 2: End\n");
    
    Test8.End();
    return NULL;
}

void *test8_thread4(void* ptr) {
    printf("Task 4: I am going to force some interrupts\n");
    
    printf("Task4: Registering ISR's for INT0, INT1, INT2 and INT3\n");
    Pr_IRQManager::Install(test8_handler, INT0);
    Pr_IRQManager::Install(test8_handler, INT1);
    Pr_IRQManager::Install(test8_handler, INT2);
    Pr_IRQManager::Install(test8_handler, INT3);
    
    
    printf("Task 4: forcing the INT0 at 2.0 time\n");
    Pr_DelayAt(Pr_Time(2.0f) += Test8.start_time);
    Pr_IRQManager::Force((INT0 - 0x10));

    printf("Task 4: forcing the INT1 at 4.0 time\n");
    Pr_DelayAt(Pr_Time(4.0f) += Test8.start_time);
    Pr_IRQManager::Force((INT1 - 0x10));

    printf("Task 4: forcing the INT2 at 6.5 time\n");
    Pr_DelayAt(Pr_Time(6.5f) += Test8.start_time);
    Pr_IRQManager::Force((INT2 - 0x10));

    printf("Task 4: forcing the INT3 at 7.0 time\n");
    Pr_DelayAt(Pr_Time(7.0f) += Test8.start_time);
    Pr_IRQManager::Force((INT3 - 0x10));

    printf("Task 4: End\n");
    
    Pr_IRQManager::Restore(INT0);
    Pr_IRQManager::Restore(INT1);
    Pr_IRQManager::Restore(INT2);
    Pr_IRQManager::Restore(INT3);  

    return NULL;
}

void * test8_main (void *ptr) {
  
    Test8.Init();

#ifdef _EDROOM_IS_DEBUG_
    
    Pr_Task ptask(test8_thread1, "tarea", EDROOMprioHigh, (4*1024));
    Pr_Task ptask2(test8_thread2, "tarea", EDROOMprioHigh, (4*1024));
    Pr_Task ptask4(test8_thread4, "tarea", EDROOMprioNormal, (4*1024));

#else

    Pr_Task ptask(test8_thread1, EDROOMprioHigh, (4*1024));
    Pr_Task ptask2(test8_thread2, EDROOMprioHigh, (4*1024));
    Pr_Task ptask4(test8_thread4, EDROOMprioNormal, (4*1024));

#endif
    
    task[0] = &ptask;
    task[2] = &ptask2;
    task[4] = &ptask4;

    printf("\n\nStarting TEST8:\nTest for implementation of Pr_IRQManager methods\n\n");
    
	Test8.Start();
	
    printf("\n\nTEST8: Finished!\n\n");
    return NULL;
}

