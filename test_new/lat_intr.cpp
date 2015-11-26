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
 * This test calculates the interrupt latency.
 */
 
#include "edroombp.h"
#include "timing.h"
#include "utils.h"
#include "global_test.h"

#include <stdio.h>

#define INT0    0x12    /*  External interrupt  */

DEFINE_TEST(lat_intr);

void lat_intr_handler(int level)
{
    Pr_Time time_spent;


    /*  Stop the test timing    */
    time_spent = stop();
    
    /*  Store the measurement to calculate the average after    */
    centeravg_add(time_spent); /* get 1-sw overhead */
    
}

void *lat_intr_thread0(void* ptr) {
    int i;
    Pr_Time time_spent;
    
    printf("Main Thread: Interrupt latency\n");
    
    /*  Install the IRQ handler */
    printf("Main Thread: Registering ISR's for INT0\n");
    Pr_IRQManager::Install(lat_intr_handler, INT0);
    
    /*  Init the timin to calculate the overhead    */
    printf("Main Thread: Init the timing\n");
    init_timing();
    
    printf("Task 4: forcing the INT0 at 2.0 time\n");
    Pr_DelayAt(Pr_Time(3.0f) += Testlat_intr.start_time);
    
    /*  Reset the average measurements  */
    centeravg_reset(OVERHEADAVG_LOOPS, OVERHEADAVG_TAILS);
    
    /*  This loop takes all the test values */
    for (i = 0; i < OVERHEADAVG_LOOPS; ++i)
    {
        start();
        Pr_IRQManager::Force((INT0 - 0x10));
    }
    
    /*  Center the measurements and estimates the final value   */
    centeravg_done(&time_spent);

    printf("Interrupt latency: %d microseconds\n", (uint32)time_spent.Micros());


    printf("Task 4: End\n");
    
    Pr_IRQManager::Restore(INT0);
    
    Testlat_intr.End();
    return NULL;
}

void * testlat_intr_main (void *ptr) {
  
    Testlat_intr.Init();

#ifdef _EDROOM_IS_DEBUG_
    
    Pr_Task ptask(lat_intr_thread0, "tarea", EDROOMprioNormal, (1024));

#else

    Pr_Task ptask(lat_intr_thread0, EDROOMprioNormal, (1024));

#endif
    
    task[0] = &ptask;

    printf("\n\nStarting Interrupt Handling Latency Benchmark\n\n");
    
    Testlat_intr.Start();
    
    printf("\n\nInterrupt Handling Latency Benchmark: Finished!\n\n");
    return NULL;
}

