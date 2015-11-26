/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/

#include "edroombp_sys.h"

extern "C" {
#include <kernel/task.h>
#include <kernel/time.h>
#include <kernel/traps.h>
};

/* Not necessary, we'll go right through it! */

// static pthread_key_t key;

//******************************************************
//****************  Pr_Kernel **************************
//******************************************************

Pr_Kernel::Pr_Kernel()
{
}

void Pr_Kernel::Start()
{
    int current_tid = SYS_GetCurrentTid();
    
    SYS_SetPriority(current_tid, EDROOMprioIDLE);
}

//******************************************************
//****************  Pr_Task ****************************
//******************************************************


//****************  CONSTRUCTORS ***********************

#ifdef _EDROOM_IS_DEBUG_

Pr_Task::Pr_Task(void *(*_taskCode)(void*), /*  Task IP */
                char *_p_name,              /*  Task name   */
                ROOMPriority _priority,     /*  task priority   */
                unsigned _stackSize):name(_p_name)        /*  stack size  */
{

#else

Pr_Task::Pr_Task(void *(*_taskCode)(void*), /*  Task IP */
                ROOMPriority _priority,     /*  task priority   */
                unsigned _stackSize)        /*  stack size  */
{

#endif

    /*  Stablish the task instruction pointer   */
    this->TaskIP = _taskCode;
    
    /*  Init the task priorities    */
    priorityMsg = priorityTmp = _priority;
    
	taskID = SYS_TaskCreate(NULL,
                        _stackSize,    /*  The thread stack size   */
                        TaskFunction,      /*  The Instruction Pointer */
                        (void *)this,
                        _priority); /*  The thread priority     */

}

void *Pr_Task::TaskFunction(void *arg)
{
    Pr_Task *mythis = (Pr_Task*)arg;
    
//    pthread_setspecific(key, (const void *)mythis);

	uint32 stackaddr, stacksize;
    uint32 *stackend;
    
   
/*    if(((1 << key) & ts_bitmap) == 0)
        return -1;*/
        
    /*  Get the caller stack parameters */
    SYS_TaskGetStackParameters(&stackaddr, &stacksize);

    /*  End stack address   */
   stackend = ((uint32*)stackaddr - stacksize);
    
    /*  Init the thread specific storage area   */
     *(stackend) = (uint32)mythis;
//    *(stackend) = (uint32)mythis;
//    printf("Arranca tarea\n");
    mythis->TaskIP(NULL);
}

void Pr_Task::SetPriority(ROOMPriority _priority)
{
    priorityMsg = _priority;
    
    if (priorityTmp != priorityMsg) {
        priorityTmp = priorityMsg;
        ChangePriority(_priority);
    }
}

void Pr_Task::SetMaxPrioTmp(ROOMPriority _priority)
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

void Pr_Task::SetMaxPrioMsg(ROOMPriority _priority)
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

ROOMPriority Pr_Task::GetTaskPrio()
{
	return SYS_GetPriority(taskID);
}

void Pr_Task::ChangePriority(ROOMPriority _priority)
{
    SYS_SetPriority(taskID, _priority);
}

//******************************************************
//****************  Pr_Time ****************************
//******************************************************

//****************  CONSTRUCTORS ***********************

Pr_Time::Pr_Time()
{

    time.tv_sec = 0;
    time.tv_usec = 0;
}

Pr_Time::Pr_Time(Pr_Time &_time)
{
    time.tv_usec = _time.time.tv_usec;
    time.tv_sec = _time.time.tv_sec;
}

Pr_Time::Pr_Time(float _seconds)
{
    float secs = _seconds;
    
    time.tv_sec = time.tv_usec = 0;
    
    if(_seconds) {
        time.tv_sec = (uint32)secs;
        secs -= time.tv_sec;
        time.tv_usec = (unsigned long)(secs * 1000000);
        
    }
        
}

Pr_Time::Pr_Time(struct timeval _time)
{
    uint32 usec;
    uint32 sec;
    
    sec = (uint32)(_time.tv_usec / 1000000);
    
    time.tv_sec = _time.tv_sec + sec;
    usec = _time.tv_usec - (sec * 1000000);
    time.tv_usec = usec;
}

//**************** MODIFIYING METHODS **********************

void Pr_Time::GetTime(void)
{
    uint32 clicks;
    
    time.tv_sec = time.tv_usec = 0;
    
	clicks = OS_TimeGetTicks();
	
	_ticks_to_timeval((&time),clicks);

}

//**********   OPERATORS OVERLOAD  *********************

Pr_Time& Pr_Time::operator+=(const Pr_Time &_time)
{

    _timeval_add((&time), &(_time.time));    
    
    return *this;
}
   
Pr_Time& Pr_Time::operator-=(const Pr_Time &_time)
{

	uint32 clicks1, clicks2;
	_timeval_to_ticks((&time), clicks1);
	_timeval_to_ticks((&_time.time), clicks2);	
	
	clicks1 -= clicks2;
	
	_ticks_to_timeval((&time), clicks1);
	_timeval_normalize((&time));
   
    return *this;
}
  
Pr_Time& Pr_Time::operator=(const Pr_Time &_time)
{
    time.tv_usec = _time.time.tv_usec;
    time.tv_sec = _time.time.tv_sec;
    
    return *this;
}
   
int Pr_Time::operator==(const Pr_Time &_time)
{
    return (_timeval_eq(&time, &(_time.time)));

}
   
int Pr_Time::operator!=(const Pr_Time &_time)
{
    return (!_timeval_eq(&time, &(_time.time)));

}
   
int Pr_Time::operator>(const Pr_Time &_time)
{
    return (_timeval_gt(&time, &(_time.time)));
  
}

int Pr_Time::operator<(const Pr_Time &_time)
{
    return (_timeval_lt(&time, &(_time.time)));
}

int Pr_Time::operator>=(const Pr_Time &_time)
{
    return (_timeval_ge(&time, &(_time.time)));          
}

int Pr_Time::operator<=(const Pr_Time &_time)
{
    return (_timeval_le(&time, &(_time.time)));          
}

//*******   CONVERSION METHODS TO STANDARD UNITS   ******
  
float Pr_Time::Mints() const
{
    clock_t nticks;
    
    nticks = time.tv_sec * CLICKS_PER_SEC;
    
    if (time.tv_usec)
        nticks = nticks + time.tv_usec * CLICKS_PER_USEC;
    
    return (float)(uint32)nticks / (CLICKS_PER_SEC * 60);
}

float Pr_Time::Secns() const
{
    float aux;
//    struct timeval time1;
//    time1 = time;
    
//    _timeval_normalize((&time1));
    aux = time.tv_sec + (float)time.tv_usec / 1000000;
//	aux = time1.tv_sec + (float)time1.tv_usec / 1000000;
    
    
    return (float)(aux);
} 
 
float Pr_Time::Millis() const
{
    float aux;
    
    aux = time.tv_sec * 1000 + (float)time.tv_usec / 1000;
    
    return(aux);
    
}
  
float Pr_Time::Micros() const
{
    float aux;
    
    aux = time.tv_sec * 1000000 + time.tv_usec;
    
    
    return (float)(aux);
}
  
float Pr_Time::Ticks() const
{
    float aux;
    
    aux = time.tv_sec * CLICKS_PER_SEC + time.tv_usec * CLICKS_PER_USEC;
    
    return (float)(aux);
}

void Pr_DelayIn(const Pr_Time &_interval)
{
	uint32 clicks;
	
    _timeval_to_ticks((&_interval.time), clicks);
       
    SYS_Sleep(clicks);

}

void Pr_DelayAt( const Pr_Time &_time)
{
    uint32 current, future;

	_timeval_to_ticks((&_time.time), future);
	current = OS_TimeGetTicks();
	
	if (future > current)
		future -= current;
	else
		return;

	SYS_Sleep(future);
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


Pr_SemaphoreBin::Pr_SemaphoreBin(char *p_name, uint32 _value) : 
name(p_name), Pr_Semaphore(_value)
{
    SYS_SemInit(&sem, _value);
}

#else

Pr_SemaphoreBin::Pr_SemaphoreBin(uint32 _value) : Pr_Semaphore(_value)
{
    SYS_SemInit(&sem, _value);
}

#endif

void Pr_SemaphoreBin::Signal()
{
    SYS_SemPost(&sem);
}

void Pr_SemaphoreBin::Wait()
{
    SYS_SemWait(&sem);
}

int Pr_SemaphoreBin::WaitCond()
{
    int retval = 0;
    
    if (SYS_SemTryWait(&sem) == 0)
        retval = 1;
    else
        return retval;      /*  error, returna 0 */
        
    return retval;          /*  OK, return 1    */
}

bool Pr_SemaphoreBin::WaitTimed(const Pr_Time &_waittime)
{
    bool retval;
    uint32 clicks;

    _timeval_to_ticks((&_waittime.time), clicks);

    retval = SYS_SemTimedWait(&sem, clicks);
    
    return (retval == 0);
}

//********************************************************
//********************  Pr_SemaphoreRec  *****************
//********************************************************

#ifdef _EDROOM_IS_DEBUG_

Pr_SemaphoreRec::Pr_SemaphoreRec(char *p_name) : name(p_name)
{
	SYS_MutexInit(&mutex, MUTEX_PRIO_INHERIT);
}

#else

Pr_SemaphoreRec::Pr_SemaphoreRec()
{
    SYS_MutexInit(&mutex, MUTEX_PRIO_INHERIT);
}

#endif

void Pr_SemaphoreRec::Signal()
{
    SYS_MutexUnLock(&mutex);
}

void Pr_SemaphoreRec::Wait()
{
    SYS_MutexLock(&mutex);
}

int Pr_SemaphoreRec::WaitCond()
{
    if (SYS_MutexTryLock(&mutex) == 0)
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
    uint8 taskSlot;
    Pr_Task *receiver;
    
    uint32 stackaddr, stacksize;
    uint32 *stackend;
    
    /*  Get the caller stack parameters */
    SYS_TaskGetStackParameters(&stackaddr, &stacksize);
    
    stackend = ((uint32*)stackaddr - stacksize);
    
    receiver = (Pr_Task *)*(stackend);
    
    receiver->semReceive.Wait();
    
    uint8 *p_dataAux, *p_dataAuxReceiver;
    
    p_dataAux = (uint8 *)_p_data;
    p_dataAuxReceiver = (uint8 *)receiver->p_data;
    
    for (int i = 0; i < _datalength; ++i)
        *(p_dataAux + i) = *(p_dataAuxReceiver + i);
    
    
    receiver->semSend.Signal();
}

//********************************************************
//********************  Pr_IRQManager ********************
//********************************************************

void Pr_IRQManager::Enable(uint32 IRQnum)
{
	SYS_IntrUnlock(IRQnum);
}

void Pr_IRQManager::Disable(uint32 IRQnum)
{
    SYS_IntrLock(IRQnum);
}

void Pr_IRQManager::Install(Pr_IRQHandler handler,uint32 IRQnum)
{
	SYS_RegisterIRQ(IRQnum, handler);
}

void Pr_IRQManager::Restore(uint32 IRQnum)
{
	SYS_RestoreIRQ(IRQnum);
}

void Pr_IRQManager::Force(uint32 IRQnum)
{
    SYS_ForceIRQ(IRQnum);
}

//********************************************************
//********************  Pr_IRQManager ********************
//********************************************************

#ifdef _EDROOM_IS_DEBUG_
    
Pr_IRQEvent::Pr_IRQEvent(char * pname,unsigned char IRQ) : 
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


