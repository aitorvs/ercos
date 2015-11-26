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
 * File:    $Source$
 * 
 * $Id: prim_pp.cpp 197 2005-11-23 18:22:02 +0100 (mié, 23 nov 2005) aitor $
 *  
 * $Revision: 197 $
 * $Date: 2005-11-23 18:22:02 +0100 (mié, 23 nov 2005) $
 * Last CVS commit by $Author: aitor $
 * 
 * Registro de cambios
 * 
 * $Log$
 * Revision 1.4  2005/09/30 08:52:02  aitor
 * The debug interface is now running.
 *
 * Revision 1.3  2005/09/28 16:37:48  aitor
 * Inlcuded the EDROOM debug mode in the tests. We also implemented the
 * Pr_IRQEvent class
 *
 * Revision 1.2  2005/09/26 11:43:30  aitor
 * Pr_IRQManager class added.
 *
 * Revision 1.1  2005/09/19 11:38:05  aitor
 * Restoring the project.
 *
 * Revision 1.14  2005/08/25 14:49:17  aitor
 * Modify the Pr_DelayAt() method in order to increase the performance.
 *
 * Revision 1.13  2005/08/25 10:11:25  aitor
 * Delete some commented code.
 *
 * Revision 1.12  2005/08/25 08:38:54  aitor
 * Pr_Task::GetTaskPrio() method implemented.
 *
 * Revision 1.11  2005/08/23 17:39:41  aitor
 * Pr_Semaphore::WaitTimed() method implemented.
 *
 * Revision 1.10  2005/08/22 15:52:01  aitor
 * Pr_Kernel::Start() method is implemented.
 *
 * Revision 1.9  2005/08/19 13:38:04  aitor
 * Pr_Send() and Pr_Receive() methods are know implemented.
 * We also modified the Pr_Task constructor to addapt it to the new
 * thread function structures.
 *
 * Revision 1.8  2005/08/15 12:48:03  aitor
 * Initialises the priorityMsg and priorityTmp in the Pr_Task constructor.
 *
 * Revision 1.7  2005/08/11 19:03:15  aitor
 * EDROOMPriority enumeration added.
 * We are also implement the Pr_SemaphoreRec class.
 *
 * Revision 1.6  2005/08/08 15:32:19  aitor
 * Delete some header files.
 *
 * Revision 1.5  2005/08/05 11:58:31  aitor
 * *** empty log message ***
 *
 * Revision 1.4  2005/08/03 10:55:08  aitor
 * Pr_SemaphoreBin class implemented.
 *
 * Revision 1.3  2005/08/03 10:39:46  aitor
 * Pr_Semaphore class implemented.
 *
 * Revision 1.2  2005/07/21 10:58:38  aitor
 * Added the Pr_Task Class.
 *
 * Revision 1.1  2005/07/05 16:08:37  aitor
 * Added the prim_pp. In this first version is added the Pr_Time and Pr_Kernel
 * classes.
 * It is also added the Pr_Time test.
 *
 */

#include "edroombp.h"

static pthread_key_t key;

//******************************************************
//****************  Pr_Kernel **************************
//******************************************************

Pr_Kernel::Pr_Kernel()
{
    pthread_key_create(&key, (void(*)(void*))NULL);
}

void Pr_Kernel::Start()
{

    setpriority(PRIO_PROCESS, 0, EDROOMprioIDLE);
    
    #ifdef CONFIG_ERCOS_TRACER
    {
        #ifdef _EDROOM_IS_DEBUG_
        Pr_SemaphoreBin StartSem("StartSem", 0);
        #else
        Pr_SemaphoreBin StartSem(0);
        #endif
        
        StartSem.Wait();
    }
    #else
        while(1);
    #endif
}

//******************************************************
//****************  Pr_Task ****************************
//******************************************************


//****************  CONSTRUCTORS ***********************

#ifdef _EDROOM_IS_DEBUG_

Pr_Task::Pr_Task(void *(*_taskCode)(void*), /*  Task IP */
                const char *_p_name,              /*  Task name   */
                EDROOMPriority _priority,     /*  task priority   */
                unsigned _stackSize):name(_p_name),        /*  stack size  */
                                semSend("TaskSem Send",0), semReceive("TaskSem recv",0)
{

#else

Pr_Task::Pr_Task(void *(*_taskCode)(void*), /*  Task IP */
                EDROOMPriority _priority,     /*  task priority   */
                unsigned _stackSize):        /*  stack size  */
                                semSend(0), semReceive(0)
{

#endif

    /*  Stablish the task instruction pointer   */
    this->TaskIP = _taskCode;
    
    struct sched_param param;
    
    param.sched_priority = _priority;
    
    /*  Init the task priorities    */
    priorityMsg = priorityTmp = _priority;
    
    /*  Init the default attr thread values */
    pthread_attr_init(&attr);

    /*  Stablish the thread stack size  */
    pthread_attr_setstacksize(&attr, _stackSize);

    /*  Init the attr priority with the param.sched_priority value  */
    pthread_attr_setschedparam(&attr, &param);

    /*  Create the thread   */
//    pthread_create(&taskID, &attr, TaskIP, NULL);
    pthread_create(&taskID, &attr, TaskFunction, (void *)this);
    
}

void *Pr_Task::TaskFunction(void *arg)
{
    Pr_Task *mythis = (Pr_Task*)arg;
    
    pthread_setspecific(key, (const void *)mythis);
    
    mythis->TaskIP(NULL);
    return NULL;
}

void Pr_Task::SetPriority(EDROOMPriority _priority)
{
    priorityMsg = _priority;
    
    if (priorityTmp != priorityMsg) {
        priorityTmp = priorityMsg;
        ChangePriority(_priority);
    }
}

void Pr_Task::SetMaxPrioTmp(EDROOMPriority _priority)
{
    /*  
     * We only change the priority if the new priority is greater than 
     * (less value) the priorityTmp value
     */
    if (_priority < priorityTmp) {
        priorityTmp = _priority;
        ChangePriority(_priority);
    }
}

void Pr_Task::SetMaxPrioMsg(EDROOMPriority _priority)
{
    if (_priority < priorityMsg) {
        priorityMsg = _priority;
        if (priorityMsg < priorityTmp) {
            priorityTmp = priorityMsg;
            ChangePriority(_priority);
        }
    }
}

void Pr_Task::RestorePrioMsg(void)
{
    
    if (priorityTmp != priorityMsg) {
        priorityTmp = priorityMsg;
        
        ChangePriority(priorityMsg);
    }
}

EDROOMPriority Pr_Task::GetTaskPrio()
{
    struct sched_param param;
    int policy;
    
    pthread_getschedparam(taskID, &policy, &param);
    
    return (EDROOMPriority)param.sched_priority;
}

void Pr_Task::ChangePriority(EDROOMPriority _priority)
{
    struct sched_param param;
    int policy;
    
    param.sched_priority = _priority;
    
    /*  We do not care about the task sched parameters */
    pthread_getschedparam(taskID, &policy, (struct sched_param*)NULL);
    /*  Sets the task priority  */
    pthread_setschedparam(taskID, policy, &param);
}

//******************************************************
//****************  Pr_Time ****************************
//******************************************************

//****************  CONSTRUCTORS ***********************

Pr_Time::Pr_Time()
{

    time.tv_sec = 0;
    time.tv_nsec = 0;
}

Pr_Time::Pr_Time(const Pr_Time &_time)
{
    time.tv_nsec = _time.time.tv_nsec;
    time.tv_sec = _time.time.tv_sec;
}

Pr_Time::Pr_Time(float _seconds)
{
    float secs = _seconds;
    
    time.tv_sec = time.tv_nsec = 0;
    
    if(_seconds) {
        time.tv_sec = (time_t)secs;
        secs -= time.tv_sec;
        time.tv_nsec = (unsigned long)(secs * 1000000000);
        
    }
        
}

Pr_Time::Pr_Time(struct timespec _time)
{
    unsigned long nsec;
    time_t sec;
    
    sec = (time_t)(_time.tv_nsec / 1000000000);
    
    time.tv_sec = _time.tv_sec + sec;
    nsec = _time.tv_nsec - (sec * 1000000000);
    time.tv_nsec = nsec;
}


Pr_Time::Pr_Time(Pr_Time::unit _units, uint32 _measure)
{
    time.tv_sec = time.tv_nsec = 0;
    
    if (_measure) {
                switch (_units) {
                                case ticks:
                                _POSIX_ticks_to_timespec((&time), _measure);
                                break;
                                case micros:
                                time.tv_nsec = _measure * 1000;
                                _POSIX_timespec_normalize((&time));
                                break;
                                case millis:
                                time.tv_nsec = _measure * 1000000;
                                _POSIX_timespec_normalize((&time));
                                break;
                                case secns:
                                time.tv_sec = _measure;
                                break;
                                case mints:
                                time.tv_sec = _measure * 60;
                                break;
                }
        }
}

//**************** MODIFIYING METHODS **********************

void Pr_Time::GetTime(void)
{
    struct timespec timeval;
    
    if (clock_gettime(CLOCK_REALTIME, &timeval) == 0)
    {
        time.tv_nsec = timeval.tv_nsec;
        time.tv_sec = timeval.tv_sec;
    }

}

EDROOMClockTicksType Pr_Time::GetTicks()
{
    struct timespec current_time;
    EDROOMClockTicksType ticks = 0;
    
    if (clock_gettime(CLOCK_REALTIME, &current_time) == 0)
    {
        struct timespec *p = &current_time;
        _POSIX_timespec_to_ticks(p, ticks);
    }
    
    return ticks;
}

//**********   OPERATORS OVERLOAD  *********************

Pr_Time& Pr_Time::operator+=(const Pr_Time &_time)
{

    _POSIX_timespec_add(&time, &(_time.time));    
    
    return *this;
}

Pr_Time& Pr_Time::operator-=(const Pr_Time &_time)
{
    if (_time.time.tv_sec <= time.tv_sec)
        {
                time.tv_sec -= _time.time.tv_sec;
                if (_time.time.tv_nsec > time.tv_nsec)
                {
                        if (time.tv_sec)
                        {
                                time.tv_sec--;
                                time.tv_nsec += 1000000000 - _time.time.tv_nsec;
                        }
                        else
                                time.tv_sec = time.tv_nsec = 0;
                }
                else
                        time.tv_nsec -= _time.time.tv_nsec;
        }
        else
                time.tv_sec = time.tv_nsec = 0;

//  _POSIX_timespec_sub(&time, &(_time.time));    
   
    return *this;
}
  
Pr_Time& Pr_Time::operator=(const Pr_Time &_time)
{
    time.tv_nsec = _time.time.tv_nsec;
    time.tv_sec = _time.time.tv_sec;
    
    return *this;
}
   
int Pr_Time::operator==(const Pr_Time &_time)
{
    return (_POSIX_timespec_eq(&time, &(_time.time)));

}
   
int Pr_Time::operator!=(const Pr_Time &_time)
{
    return (!_POSIX_timespec_eq(&time, &(_time.time)));

}
   
int Pr_Time::operator>(const Pr_Time &_time)
{
    return (_POSIX_timespec_gt(&time, &(_time.time)));
  
}

int Pr_Time::operator<(const Pr_Time &_time)
{
    return (_POSIX_timespec_lt(&time, &(_time.time)));
}

int Pr_Time::operator>=(const Pr_Time &_time)
{
    return (_POSIX_timespec_ge(&time, &(_time.time)));          
}

int Pr_Time::operator<=(const Pr_Time &_time)
{
    return (_POSIX_timespec_le(&time, &(_time.time)));          
}

//*******   CONVERSION METHODS TO STANDARD UNITS   ******
  
float Pr_Time::Mints() const
{
    clock_t nticks;
    
    nticks = time.tv_sec * CLICKS_PER_SEC;
    
    if (time.tv_nsec > 1000)
        nticks = nticks + (time.tv_nsec / 1000) * CLICKS_PER_USEC;
    
    return (float)(uint32)nticks / (CLICKS_PER_SEC * 60);
}

float Pr_Time::Secns() const
{
    float aux;
    
    aux = time.tv_sec + (float)time.tv_nsec / 1000000000;
    
    
    return (float)(aux);
} 
 
float Pr_Time::Millis() const
{
    float aux;
    
    aux = time.tv_sec + (float)time.tv_nsec / 1000000000.0;
    
    return(aux * 1000);
    
}
  
float Pr_Time::Micros() const
{
    float aux;
    
    aux = time.tv_sec + (float)time.tv_nsec / 1000000000;
    
    
    return (float)(aux * 1000000);
}
  
float Pr_Time::Ticks() const
{
    float aux;
    
    aux = time.tv_sec + (float)time.tv_nsec / 1000000000;
    
    
    return (float)(aux * CLICKS_PER_SEC);
}

void Pr_DelayIn(const Pr_Time &_interval)
{
    struct timespec time = _interval.time;

    nanosleep(&time, (struct timespec*)NULL);

}

#include <kernel/debug.h> /* DEBUG */
void Pr_DelayAt( const Pr_Time &_time)
{
    int ret;
    ret = clock_nanosleep (CLOCK_REALTIME, TIMER_ABSTIME, &_time.time, (struct timespec *) 0);
    if (ret < 0) DEBUG ("clock_nanosleep returned %d!!!", ret);
}

//********************************************************
//********************  Pr_Semaphore  ********************
//********************************************************

Pr_Semaphore::Pr_Semaphore(unsigned int _value)
{
}


//********************************************************
//********************  Pr_SemaphoreBin  *****************
//********************************************************


#ifdef _EDROOM_IS_DEBUG_


Pr_SemaphoreBin::Pr_SemaphoreBin(const char *p_name, uint32 _value) : 
 Pr_Semaphore(_value), name(p_name)
{
    sem_init(&sem, 0, _value);
}

#else

Pr_SemaphoreBin::Pr_SemaphoreBin(uint32 _value) : Pr_Semaphore(_value)
{
    sem_init(&sem, 0, _value);
}

#endif

void Pr_SemaphoreBin::Signal()
{
    sem_post(&sem);
}

void Pr_SemaphoreBin::Wait()
{
    sem_wait(&sem);
}

int Pr_SemaphoreBin::WaitCond()
{
    int retval = 0;
    
    if (sem_trywait(&sem) == 0)
        retval = 1;
        
    return retval;          /*  OK, return 1    */
}

bool Pr_SemaphoreBin::WaitTimed(const Pr_Time &_waittime)
{
    bool retval;
    struct timespec time;
    
    time.tv_sec = _waittime.time.tv_sec;
    time.tv_nsec = _waittime.time.tv_nsec;
    
    retval = sem_timedwait (&sem, &time);
    
    return (retval == 0);
}

//********************************************************
//********************  Pr_SemaphoreRec  *****************
//********************************************************

#ifdef _EDROOM_IS_DEBUG_
Pr_SemaphoreRec::Pr_SemaphoreRec(const char *p_name) : name(p_name)
#else
Pr_SemaphoreRec::Pr_SemaphoreRec()
#endif
{
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL);
    pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT);
    pthread_mutex_init(&mutex, &attr);
}

#ifdef _EDROOM_IS_DEBUG_
Pr_SemaphoreRec::Pr_SemaphoreRec(const char *p_name, int prioceiling) : name(p_name)
#else
Pr_SemaphoreRec::Pr_SemaphoreRec(int prioceiling)
#endif
{
    int priority_ceiling;
    
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL);
    pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_PROTECT);
    
    /*
     * The priority in pthread_mutex with priority ceiling protocol
     * varies from 0 to 127 means 127 the higher priority. In EDROOM
     * the 0 priority value is the higher priority so we need to make a
     * change
     */
    priority_ceiling = EDROOM_PRIO_MINIMUM - prioceiling;
    priority_ceiling = (priority_ceiling < 0) ? 0 : priority_ceiling;
    
    pthread_mutexattr_setprioceiling (&attr, priority_ceiling);
    pthread_mutex_init(&mutex, &attr);
}

void Pr_SemaphoreRec::Signal()
{
    pthread_mutex_unlock(&mutex);
}

void Pr_SemaphoreRec::Wait()
{
    pthread_mutex_lock(&mutex);
}

int Pr_SemaphoreRec::WaitCond()
{
    if (pthread_mutex_trylock(&mutex) == 0)
        return 1;
    else
        return 0;
}


void Pr_Send(Pr_Task &_task, void *_p_data)
{
    _task.p_data = _p_data;
    _task.semReceive.Signal();
    _task.semSend.Wait();
}

void Pr_Receive(void * _p_data,unsigned _datalength)
{
    // uint8 taskSlot;
    Pr_Task *receiver;
    
    receiver = (Pr_Task*)pthread_getspecific(key);
    
    receiver->semReceive.Wait();
    
    uint8 *p_dataAux, *p_dataAuxReceiver;
    
    p_dataAux = (uint8 *)_p_data;
    p_dataAuxReceiver = (uint8 *)receiver->p_data;
    
    for (unsigned i = 0; i < _datalength; ++i)
        *(p_dataAux + i) = *(p_dataAuxReceiver + i);
    
    
    receiver->semSend.Signal();
}

//********************************************************
//********************  Pr_IRQManager ********************
//********************************************************

void Pr_IRQManager::Enable(intr_t IRQnum)
{
    intr_unlock(IRQnum);
}

void Pr_IRQManager::Disable(intr_t IRQnum)
{
    intr_lock(IRQnum);
}

void Pr_IRQManager::Install(Pr_IRQHandler handler,intr_t IRQnum)
{
    intr_capture(handler, IRQnum);
}

void Pr_IRQManager::Restore(intr_t IRQnum)
{
    intr_restore(IRQnum);
}

void Pr_IRQManager::Force(intr_t IRQnum)
{
    intr_force(IRQnum);
}

//********************************************************
//********************  Pr_IRQEvent ********************
//********************************************************

#ifdef _EDROOM_IS_DEBUG_
    
Pr_IRQEvent::Pr_IRQEvent(const char * pname,unsigned char IRQ) : 
                        eventSem("Event Semaphore", 0), // init the semaphore
                        name(pname)                     // the Event name
{
}

#else

Pr_IRQEvent::Pr_IRQEvent(unsigned char IRQ) : 
                        eventSem(0)    // init the semaphore
{}

#endif

void Pr_IRQEvent::Signal()
{
    eventSem.Signal();
}

void Pr_IRQEvent::Wait()
{
    eventSem.Wait();
}

bool Pr_IRQEvent::WaitTimed(Pr_Time _time)
{

    return eventSem.WaitTimed(_time);
}

bool Pr_IRQEvent::WaitCond()
{

    return eventSem.WaitCond();
}

