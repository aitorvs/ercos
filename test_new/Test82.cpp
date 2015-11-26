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

/* Interrupts list */

#define MASK_HW_ERR 	0x11 	/* Masked Hardware Errors */
#define INT0    		0x12	/* External INT 0 */	
#define INT1    		0x13	/* External INT 1 */
#define UARTA_DR		0x14	/* UART A Data Ready */
#define UARTB_DR		0x15	/* UART B Data Ready */
#define CORR_ERR_MEM 	0x16	/* Correctable Error in Memory */
#define UART_ERR		0x17	/* UART Error */
#define DMA_ACC_ERR		0x18	/* DMA Access Error */
#define DMA_TIMEOUT		0x19	/* DMA Timeout */
#define INT2			0x1A	/* External INT 2 */
#define INT3    		0x1B	/* External INT 3 */
#define GPTC			0x1C	/* General Purpose Timer Counter */
#define RTCT			0x1D	/* Real Time Timer Counter */
#define INT4   			0x1E    /* Ethernet */
#define WATCHDOG		0x1F	/* Watchdog Timeout */

static Pr_Task *task1, *task2, *task3, *task4, *task5, *task6, \
		*task7, *task8, *task9, *task10, *task11, *task12, \
		*task13, *task14, *task15, *init_task;

#define HANDLER_IRQ(num)						\
void handler_irq##num##(int level)					\
{															\
	int i = 0; \
    Pr_IRQEvent event("interrupt", level);					\
    														\
    printf("handler_irq%d: IRQ %d triggered. Signalling the event\n", num, num);	\
    event.Signal();											\
   															\
    printf("handler_irq%d: Forcing INT%d\n", num, num + 1);	\
    Pr_IRQManager::Force((MASK_HW_ERR + num - 0x10));	\
    														\
	printf("handler_irq%d: Finished!\n", num);	\
}												\

HANDLER_IRQ(1)
HANDLER_IRQ(2)/*
HANDLER_IRQ(3)
HANDLER_IRQ(4)*/
HANDLER_IRQ(5)
HANDLER_IRQ(6)
HANDLER_IRQ(7)
HANDLER_IRQ(8)
HANDLER_IRQ(9)
HANDLER_IRQ(10)
HANDLER_IRQ(11)
/*HANDLER_IRQ(12)
HANDLER_IRQ(13)
HANDLER_IRQ(14)*/

void handler_irq3(int level)					
{															
	int i = 0; 
    Pr_IRQEvent event("interrupt", level);					
    														
    printf("handler_irq3: IRQ 3 triggered. Signalling the event\n");	
    event.Signal();											
   															
    printf("handler_irq3: Forcing INT5\n");	
    Pr_IRQManager::Force((MASK_HW_ERR + 4 - 0x10));	
    														
	printf("handler_irq3: Finished!\n");
}

void handler_irq12(int level)
{
	int i = 0;

#ifdef _EDROOM_IS_DEBUG_
    Pr_IRQEvent event("interrupt", level);
#else
    Pr_IRQEvent event(level);
#endif
    
    printf("handler_irq12: IRQ 12 triggered. Signalling the event\n", level);
    event.Signal();
	printf("handler_irq12: Finished!\n");
}

void *init_thread(void* ptr)		
{									
	int i = 0;						
									
	printf("Task0: Hello! Registering ISRs\n");	
									
    Pr_IRQManager::Install(handler_irq1, MASK_HW_ERR);
    Pr_IRQManager::Install(handler_irq2, INT0);
    Pr_IRQManager::Install(handler_irq3, INT1);
//    Pr_IRQManager::Install(handler_irq4, UARTA_DR);
    Pr_IRQManager::Install(handler_irq5, UARTB_DR);
    Pr_IRQManager::Install(handler_irq6, CORR_ERR_MEM);
    Pr_IRQManager::Install(handler_irq7, UART_ERR);
    Pr_IRQManager::Install(handler_irq8, DMA_ACC_ERR);
    Pr_IRQManager::Install(handler_irq9, DMA_TIMEOUT);
    Pr_IRQManager::Install(handler_irq10, INT2);
    Pr_IRQManager::Install(handler_irq11, INT3);
    Pr_IRQManager::Install(handler_irq12, GPTC);
/*    Pr_IRQManager::Install(handler_irq13, RTCT);
    Pr_IRQManager::Install(handler_irq14, INT4);
    Pr_IRQManager::Install(handler_irq15, WATCHDOG);*/
    
    printf("Task0: ISRs Registered! Spinning for 3 secs\n");	
	for (i = 0; i < 8000000; i++);
	
	printf("Task0: Forcing MASK_HW_ERR -irq 0-\n"); 
    Pr_IRQManager::Force((MASK_HW_ERR - 0x10));
    
    printf("Task0: Spinning for 3 secs\n");
	for (i = 0; i < 8000000; i++);
	
	printf("Task0: Finished!\n");

}

#define IRQ_WAIT_THREAD(num)		\
void *thread##num##(void* ptr) 		\
{									\
	int i = 0;						\
    Pr_IRQEvent eventIRQ("interrupt", MASK_HW_ERR + num);	\
    								\
    printf("Task%d: I am going to wait until IRQ %X\n", num, MASK_HW_ERR + num);	\
    eventIRQ.Wait();				\
    								\
    printf("Task%d: IRQ %X Woken up! Now spinning for 3 secs\n", num, MASK_HW_ERR + num);	\
    for (i = 0; i < 8000000; i++);	\
     								\
    printf("Task%d: Finished!\n", num);  	\
}									\

IRQ_WAIT_THREAD(1)
IRQ_WAIT_THREAD(2)
IRQ_WAIT_THREAD(3)
// IRQ_WAIT_THREAD(4)
IRQ_WAIT_THREAD(5)
IRQ_WAIT_THREAD(6)
IRQ_WAIT_THREAD(7)
IRQ_WAIT_THREAD(8)
IRQ_WAIT_THREAD(9)
IRQ_WAIT_THREAD(10)
IRQ_WAIT_THREAD(11)
IRQ_WAIT_THREAD(12)
/*IRQ_WAIT_THREAD(13)
IRQ_WAIT_THREAD(14)
IRQ_WAIT_THREAD(15)*/

void *main_test82(void *ptr);
Test Test82(main_test82);


void *main_test82(void *ptr) 
{
  
    Test82.Init();

#ifdef _EDROOM_IS_DEBUG_

	Pr_Task pinit_task(init_thread, "hilo inicial", EDROOMprioLevel16, (4*1024));    
    Pr_Task ptask1(thread1, "tarea", EDROOMprioLevel10, (4*1024));
    Pr_Task ptask2(thread2, "tarea", EDROOMprioLevel7, (4*1024));
    Pr_Task ptask3(thread3, "tarea", EDROOMprioLevel15, (4*1024));
//    Pr_Task ptask4(thread4, "tarea", EDROOMprioLevel6, (4*1024));
    Pr_Task ptask5(thread5, "tarea", EDROOMprioLevel1, (4*1024));
    Pr_Task ptask6(thread6, "tarea", EDROOMprioLevel9, (4*1024));
    Pr_Task ptask7(thread7, "tarea", EDROOMprioLevel12, (4*1024));
    Pr_Task ptask8(thread8, "tarea", EDROOMprioLevel2, (4*1024));
    Pr_Task ptask9(thread9, "tarea", EDROOMprioLevel14, (4*1024));
    Pr_Task ptask10(thread10, "tarea", EDROOMprioLevel4, (4*1024));
    Pr_Task ptask11(thread11, "tarea", EDROOMprioLevel3, (4*1024));
    Pr_Task ptask12(thread12, "tarea", EDROOMprioLevel13, (4*1024));
/*    Pr_Task ptask13(thread13, "tarea", EDROOMprioLevel11, (4*1024));
    Pr_Task ptask14(thread14, "tarea", EDROOMprioLevel5, (4*1024));
    Pr_Task ptask15(thread15, "tarea", EDROOMprioLevel8, (4*1024));*/

#else

	Pr_Task pinit_task(init_thread, EDROOMprioLevel16, (4*1024));    
    Pr_Task ptask1(thread1, EDROOMprioLevel10, (4*1024));
    Pr_Task ptask2(thread2, EDROOMprioLevel7, (4*1024));
    Pr_Task ptask3(thread3, EDROOMprioLevel15, (4*1024));
//    Pr_Task ptask4(thread4, EDROOMprioLevel6, (4*1024));
    Pr_Task ptask5(thread5, EDROOMprioLevel1, (4*1024));
    Pr_Task ptask6(thread6, EDROOMprioLevel9, (4*1024));
    Pr_Task ptask7(thread7, EDROOMprioLevel12, (4*1024));
    Pr_Task ptask8(thread8, EDROOMprioLevel2, (4*1024));
    Pr_Task ptask9(thread9, EDROOMprioLevel14, (4*1024));
    Pr_Task ptask10(thread10, EDROOMprioLevel4, (4*1024));
    Pr_Task ptask11(thread11, EDROOMprioLevel3, (4*1024));
    Pr_Task ptask12(thread12, EDROOMprioLevel13, (4*1024));
/*    Pr_Task ptask13(thread13, EDROOMprioLevel11, (4*1024));
    Pr_Task ptask14(thread14, EDROOMprioLevel5, (4*1024));
    Pr_Task ptask15(thread15, EDROOMprioLevel8, (4*1024));*/

#endif

	init_task = &pinit_task;
    task1 = &ptask1;
    task2 = &ptask2;
    task3 = &ptask3;
//    task4 = &ptask4;
    task5 = &ptask5;
    task6 = &ptask6;
    task7 = &ptask7;
    task8 = &ptask8;
    task9 = &ptask9;
    task10 = &ptask10;
    task11 = &ptask11;
    task12 = &ptask12;
/*    task13 = &ptask13;
    task14 = &ptask14;
    task15 = &ptask15; */
    
    Test82.Start()

}

