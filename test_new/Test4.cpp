
/*
 * This test checks the implementation of the Pr_Time methods and operators.
 */
 
#include "edroombp.h"
#include <stdio.h>
#include "global_test.h"

DEFINE_TEST(4);

void *test4_thread0(void* ptr) {
    
    struct timespec time;
    
    printf("Test Pr_Time\n\n");   
    
    Pr_Time time8(90.0f);
    Pr_Time time9(360.0f);
    
    sembin[0]->Wait();
    
    printf("Wait until 1.23 in the program clock\n");
    Pr_DelayAt(Pr_Time(1.23f) += Test4.start_time); //Wait until 1.23 in the program clock

    Pr_Time time1;
    
    time1.GetTime();
    
    printf("1.23 clock reached\n");
    
    printf("Current time in seconds %d = 1.23\n", (uint32)time1.Secns());
    printf("Current time in millis %d = 1230\n", (uint32)time1.Millis());
    printf("Current time in micros %d = 1230000\n", (uint32)time1.Micros());
    printf("Current time in minutes %d = 1.23\n", (uint32)time1.Mints());
    
    printf("2 seconds delay\n");
    Pr_DelayIn(Pr_Time(2)); //Wait 2.0 seconds in the program clock

    Pr_Time time2(60);
    
    printf("60 seconds in minutes are %d\n", (uint32)time2.Mints());
    printf("60 seconds in millis are %d\n", (uint32)time2.Millis());
    printf("60 seconds in micros are %d\n", (uint32)time2.Micros());
    printf("60 seconds in ticks are %d\n", (uint32)time2.Ticks());
    
    Pr_DelayIn(Pr_Time(2)); //Wait 2.0 seconds in the program clock

    time.tv_sec = 0;
    time.tv_nsec = 1000000;
    Pr_Time time3(time);
    
    printf("\n");
    
    printf("100000 ticks in minutes are %d\n",(uint32)time3.Mints());
    printf("100000 ticks in seconds are %d\n", (uint32)time3.Secns());
    printf("100000 ticks in millis are %d\n", (uint32)time3.Millis());
    printf("100000 ticks in micros are %d\n", (uint32)time3.Micros());

    Pr_DelayIn(Pr_Time(2)); //Wait 2.0 seconds in the program clock

    time.tv_sec = 0;
    time.tv_nsec = 1000000000;
    Pr_Time time4(time);
    
    printf("\n");
    
    printf("1000000 micros in minutes are %d\n", (uint32)time4.Mints());
    printf("1000000 micros in seconds are %d\n", (uint32)time4.Secns());
    printf("1000000 micros in millis are %d\n",(uint32)time4.Millis());
    printf("1000000 micros in ticks are %d\n", (uint32)time4.Ticks());
    
    Pr_DelayIn(Pr_Time(2)); //Wait 2.0 seconds in the program clock

    time.tv_sec = 0;
    time.tv_nsec = 1000000000;
    Pr_Time time5(time);
    
    printf("\n");
    
    printf("1000 millis in minutes are %d\n", (uint32)time5.Mints());
    printf("1000 millis in seconds are %d\n", (uint32)time5.Secns());
    printf("1000 millis in micros are %d\n", (uint32)time5.Micros());
    printf("1000 millis in ticks are %d\n", (uint32)time5.Ticks());
    
    Pr_DelayIn(Pr_Time(2)); //Wait 2.0 seconds in the program clock

    time.tv_sec = 360;
    time.tv_nsec = 0;
    Pr_Time time6(time);
    
    printf("\n");
    
    printf("360 seconds in minutes are %d\n", (uint32)time6.Mints());
    printf("360 seconds in millis are %d\n", (uint32)time6.Millis());
    printf("360 seconds in micros are %d\n", (uint32)time6.Micros());
    printf("360 seconds in ticks are %d\n", (uint32)time6.Ticks());

    Pr_DelayIn(Pr_Time(2)); //Wait 2.0 seconds in the program clock

    time.tv_sec = 3 * 60;
    time.tv_nsec = 0;
    Pr_Time time7(time);
    
    printf("\n");
    
    printf("3 minutes in seconds are %d\n", (uint32)time7.Secns());
    printf("3 minutes in millis are %d\n", (uint32)time7.Millis());
    printf("3 minutes in micros are %d\n", (uint32)time7.Micros());
    printf("3 minutes in ticks are %d\n", (uint32)time7.Ticks());
    
    Pr_DelayIn(Pr_Time(2)); //Wait 2.0 seconds in the program clock

//    Pr_Time time;
    printf("\n");
    
//    printf("The clock interrupt period is at %d nanoseconds\n", 
//           time.period() * 1000000000);
//    printf("The max hours are %d\n", time.max_hours());
//    printf("The max days are %d\n", time.max_days());
//
//    Pr_DelayIn(Pr_Time(2)); //Wait 2.0 seconds in the program clock
//    printf("\n");
    

    time2 += (float)(5 * 60);
    printf("60 seconds + 5 minutes are %d minutes\n", (uint32)time2.Mints());
    
    time2 = 60.0f;
    time2 += 5.0f;
    printf("60 seconds + 5 seconds are %d seconds\n", (uint32)time2.Secns());
    
    time2 = 60.0f;
    time.tv_sec = 0;
    time.tv_nsec = 1000000000;
    time2 += Pr_Time(time);
    printf("60 seconds + 1000 millis (=1sec) are %d seconds\n", 
            (uint32)time2.Secns());
    
    
    Pr_DelayIn(Pr_Time(2)); //Wait 2.0 seconds in the program clock

    printf("\n");
    
    time2 = (float)(4 * 60);
    time2 -= 60;
    printf("4 minutes - 1 minute are %d minutes\n", (uint32)time2.Mints());
    
    time2 = 60.0f;
    time2 -= 20.0f;
    printf("60 seconds - 20 seconds are %d seconds\n", (uint32)time2.Secns());

    time2 = 60.0f;
    time.tv_sec = 0;
    time.tv_nsec = 1000000000;
    time2 -= Pr_Time(time);
    printf("60 seconds - 1000 millis(=1sec) are %d seconds\n", (uint32)time2.Secns());

            
    Pr_DelayIn(Pr_Time(2)); //Wait 2.0 seconds in the program clock

    printf("\n");
    printf("Operators\n");
    
    printf("time8 = %d seconds, time9 = %d seconds\n", (uint32)time8.Secns(), 
                                                       (uint32)time9.Secns());
    printf("time8 > time9 -> %s\n", (time8 > time9)? "OK" : "FALSE");
    printf("time8 < time9 -> %s\n", (time8 < time9)? "OK" : "FALSE");
    printf("time8 >= time9 -> %s\n", (time8 >= time9)? "OK" : "FALSE");
    printf("time8 <= time9 -> %s\n", (time8 <= time9)? "OK" : "FALSE");
    printf("time8 == time9 -> %s\n", (time8 == time9)? "OK" : "FALSE");
    printf("time8 != time9 -> %s\n", (time8 != time9)? "OK" : "FALSE");

    Pr_DelayIn(Pr_Time(2)); //Wait 2.0 seconds in the program clock

    printf("\n");

    time8 = time9;
    printf("Time9 takes the value of the time8(=%d secs) -> %d seconds\n", 
            (uint32)time8.Secns(),
            (uint32)time9.Secns());

    //Operator +=

    time8 = Pr_Time(10.0f);
    time9 = Pr_Time(1);
    
    time1 = time9  += time8;
    
    printf("Time9 = %d sec = time9(=1 sec) + time8(=%d)\n", 
            (uint32)time9.Secns(),
            (uint32)time8.Secns());
    printf("Time1 = %d sec = time9(=%d sec)\n", 
            (uint32)time1.Secns(),
            (uint32)time9.Secns());
    
    //Operator -=
    time9 = 1.0f;
    
    time1 = time8 -= time9;
    
    printf("Time8 = %d = time8(=10 sec) - time9(=%d)\n", (uint32)time8.Secns(),
                                                         (uint32)time9.Secns());
    printf("Time1 = %d = time8(=%d)\n", (uint32)time1.Secns(),
                                        (uint32)time8.Secns());
    Test4.End();
}

void *test4_thread1(void* ptr) {
    printf("Tarea Auxiliar\n");
    
    Pr_DelayAt(Pr_Time(1.0f) += Test4.start_time);
    sembin[0]->Signal();

}

void * test4_main (void *ptr) {
  
    Test4.Init();
    
#ifdef _EDROOM_IS_DEBUG_
    
    Pr_SemaphoreBin sem("sempahore", 0);
    
    Pr_Task ptask(test4_thread0, "tarea", EDROOMprioLow, 512);
    Pr_Task ptask2(test4_thread1, "tarea", EDROOMprioVeryLow, 512);
    
#else
    
    Pr_SemaphoreBin sem(0);
    
    Pr_Task ptask(test4_thread0, EDROOMprioLow, 512);
    Pr_Task ptask2(test4_thread1, EDROOMprioVeryLow, 512);
    
#endif
   
   	sembin[0] = &sem;
    task[0] = &ptask;
    task[1] = &ptask2;

    printf("\n\nStarting TEST4:\nTest for Pr_Time methods\n\n");
    
    Test4.Start();

    printf("\n\nTEST4: Finished!\n\n");
}

