/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/

#ifndef _PRIM_PP_H_
#define _PRIM_PP_H_

#include <stdio.h> // quitar este include en la version final.
#include <sys/types.h>

/*
extern "C" {
	void OS_TaskGetStackParameters(uint32 *_stackaddr, uint32 *_stacksize);
	uint32 OS_TimeGetTicks(void);
	int OS_RegisterIRQ (unsigned int _vec, void (*handler)(int level));
	int OS_RestoreIRQ(int vector);	
};*/


struct timeval {
    uint32 tv_sec;
    uint32 tv_usec;
};


#define CLICKS_PER_SEC  CLICKS_PER_SEC

#define USECS_PER_SEC 1000000

/**
 * \todo Documentar esta macro
 */
#define _timeval_to_ticks(_timeval, _ticks) \
    do { \
        _ticks = _timeval->tv_sec * CLICKS_PER_SEC + \
                 _timeval->tv_usec * CLICKS_PER_USEC; \
    }while(0) \

/**
 * \todo Documentar esta macro
 */
#define _ticks_to_timeval(_timeval, _ticks) \
    do { \
        _timeval->tv_sec = _ticks / 1000000; \
        _ticks -= _timeval->tv_sec * 1000000; \
        _timeval->tv_usec = _ticks; \
    }while(0) \


/* convenience functions */
#define _timeval_normalize(t) {\
    if ((t)->tv_usec >= USECS_PER_SEC) { \
     (t)->tv_usec -= USECS_PER_SEC; \
       (t)->tv_sec++; \
   } else if ((t)->tv_usec < 0) { \
       (t)->tv_usec += USECS_PER_SEC; \
       (t)->tv_sec--; \
   } \
}

#define _timeval_add(t1, t2) do { \
 (t1)->tv_usec += (t2)->tv_usec;  \
 (t1)->tv_sec += (t2)->tv_sec; \
    _timeval_normalize(t1);\
} while (0)

#define _timeval_sub(t1, t2) do { \
  (t1)->tv_usec -= (t2)->tv_usec;  \
 (t1)->tv_sec -= (t2)->tv_sec; \
    _timeval_normalize(t1);\
} while (0)

#define _timeval_add_us(t,n) do { \
  (t)->tv_usec += (n);  \
    _timeval_normalize(t); \
} while (0)
   
#define _timeval_nz(t) ((t)->tv_sec != 0 || (t)->tv_usec != 0)

#define _timeval_lt(t1, t2) ((t1)->tv_sec < (t2)->tv_sec || ((t1)->tv_sec == (t2)->tv_sec && (t1)->tv_usec < (t2)->tv_usec))

#define _timeval_gt(t1, t2) (_timeval_lt(t2, t1))

#define _timeval_ge(t1, t2) (!_timeval_lt(t1, t2))

#define _timeval_le(t1, t2) (!_timeval_gt(t1, t2))

#define _timeval_eq(t1, t2) ((t1)->tv_sec == (t2)->tv_sec && (t1)->tv_usec == (t2)->tv_usec)


/*  
 * Es necesario de momento para las funciones SYS_IRQ que no tienen equivalente
 * en POSIX (de momento)
 */
#include <syscall.h>

enum EDROOMPriority{
    EDROOMprioURGENT = 0,
    EDROOMprioVeryHigh = 1,
    EDROOMprioHigh = 2,
    EDROOMprioNormal = 3,
    EDROOMprioLow = 4,
    EDROOMprioVeryLow = 5,
    EDROOMprioMINIMUM = 6,
    EDROOMprioIDLE = 7,
}; 

#define ROOMPriority priority_t
#define _EDROOM_IS_DEBUG_

//********************************************************
//********************  Pr_Kernel  ***********************
//********************************************************

class Pr_Kernel
{
public:

    Pr_Kernel();
    ~Pr_Kernel(){};
    
    /** 
     * Kernel start method
     * 
     * This methos may be invoqued to start the kernel threads.
     */
    void Start();    
};

//********************************************************
//********************  Pr_Semaphore  ********************
//********************************************************

class Pr_Time;
/**
 * \class Semaphore class declaration
 */
class Pr_Semaphore
{

public:
    /**
     * \todo Default constructor.
     * 
     * \param _value:   This parameter is the semaphore counter value.
     * 
     * The default semaphore value is 0.
     */
    Pr_Semaphore(unsigned int _value=0);

    /**
     * Semaphore Signal
     */
    void Signal();

    /**
     * Semaphore Wait
     */
    void Wait();

    /**
     * Semaphore conditional wait.
     * 
     * The method catch the semaphore if it is possible. In any other case the
     * thread is not blocked.
     * 
     * \return The method return 1 in case of the semaphore is catched. In any
     * other case the method returns 0.
     */
    int WaitCond();

    /**
     * \todo Documentar este método
     */
    bool WaitTimed(const Pr_Time &);

    /**
     * Destructor
     */
    ~Pr_Semaphore(){
    }

};

//********************************************************
//********************  Pr_SemaphoreBin  *****************
//********************************************************

/**
 * \class Binary semaphore class declaration
 */
class Pr_SemaphoreBin  :  public Pr_Semaphore
{
protected:
    OS_Sem_t sem;
    
public:

#ifdef _EDROOM_IS_DEBUG_

    char *name;

    Pr_SemaphoreBin(char *p_name="semaphore", uint32 _value = 0);

#else

    /**
     * Default Constructor.
     */
    Pr_SemaphoreBin(uint32 _value = 0);

#endif
    
    /**
     * Semaphore Signal
     */
    void Signal();

    /**
     * Semaphore Wait
     */
    void Wait();

    /**
     * Semaphore conditional wait.
     * 
     * The method catch the semaphore if it is possible. In any other case the
     * thread is not blocked.
     * 
     * \return The method return 1 in case of the semaphore is catched. In any
     * other case the method returns 0.
     */
    int WaitCond();

    /**
     * \todo Documentar este método
     */
    bool WaitTimed(const Pr_Time &);

    /**
     * Destructor
     */
    ~Pr_SemaphoreBin(){
        SYS_SemDestroy(&sem);
    }
};


//********************************************************
//********************  Pr_SemaphoreRec  *****************
//********************************************************

/**
 * \class Resource semaphore class declaration
 */
class Pr_SemaphoreRec : public Pr_Semaphore
{
private:
    /** This attribute is the class mutex   */
    OS_Mutex_t mutex;  

public:

#ifdef _EDROOM_IS_DEBUG_

    /** Default Debug constructor */
    Pr_SemaphoreRec(char *name);

    const char * name;

#else

    /** Default constructor */
    Pr_SemaphoreRec();
    
#endif

    /**
     * Semaphore signal.
     */
    void Signal(void);

    /**
     * Semaphore wait.
     */
    void Wait(void);

    /**
     * Semaphore conditional wait.
     * 
     * The method catch the semaphore if it is possible. In any other case the
     * thread is not blocked.
     * 
     * \return The method return 1 in case of the semaphore is catched. In any
     * other case the method returns 0.
     */
    int WaitCond(void);

    /**
     * \todo Documentar este m�todo
     * \todo Implementar este m�todo
     */
    int WaitTimed(Pr_Time &_time);
    
    /**
     * Default Destructor
     */
    ~Pr_SemaphoreRec(){
    	SYS_MutexDestroy(&mutex);}
};

//********************************************************
//********************  Pr_Task  *************************
//********************************************************

class Pr_Task
{
/**
 * This method sends information to any other task.
 * 
 * \param task:     This is the receiver task.
 * \param p_data:   This parameter is the data pointer to be sent.
 */
friend void Pr_Send(Pr_Task &task, void *p_data);

/**
 * This method receives a data from sent by other task.
 * 
 * \param ap_data:      This parameter is the pointer to the received data.
 * \param datalenght:   This parameter is the data length.
 */
friend void Pr_Receive(void * ap_data,unsigned datalength);
        
private:

    /** This member is the task identifier  */
    tid_t   taskID;

#ifdef _EDROOM_IS_DEBUG_
    
    /** This is the task name   */
    char *name;

#endif
    
    /** This is the thread attributes defined in <sys/types.h<   */
//    pthread_attr_t attr;
              
    /** Pointer to the data sent through Pr_Send method  */
    void  *p_data;
    
    //*************   SEMAPHORES *****************
    
    /** 
     * This semaphore is used in the Pr_Send/Pr_Receive methods to synchronize
     * the sender and receiver threads
     */
    Pr_SemaphoreBin semSend;

    /** 
     * This semaphore is used in the Pr_Send/Pr_Receive methods to synchronize
     * the sender and receiver threads
     */
    Pr_SemaphoreBin semReceive;

    /**
     * Change the task priority
     * 
     * \param _priority:    This parameter is the task priority
     */
    void ChangePriority(ROOMPriority _priority);
    
    /**
     * This member is the IP task pointer.
     */
    void *(*TaskIP)(void*);
    
    static void *TaskFunction(void *arg);
    
protected:
    
    /**
     * Message priority.
     * 
     * This is the message treatment priority
     */
    ROOMPriority priorityMsg;
    
    /**
     * Temporal priority.
     */
    ROOMPriority priorityTmp;
  
public:

    /**
     * Default Constructor
     */
    Pr_Task(){}

#ifdef _EDROOM_IS_DEBUG_

    /**
     * Debug Constructor
     * 
     * \param _taskCode:    This parameter is the pointer to the task code
     * \param _p_name:      This parameter is the task name
     * \param _priority:    This parameter is the task priority
     * \param _stackSize:   This parameter is the task stack size
     * 
     * The constructor creates a task. Assigns the needed stack size and the
     * IP code pointer to the task structure.
     */
    Pr_Task(void *(*_taskCode)(void *), /*  Task IP */
            char *_p_name,              /*  Task name   */
            ROOMPriority _priority,     /*  task priority   */
            unsigned _stackSize);       /*  stack size  */

#else

    /**
     * Debug Constructor
     * 
     * \param _taskCode:    This parameter is the pointer to the task code
     * \param _priority:    This parameter is the task priority
     * \param _stackSize:   This parameter is the task stack size
     * 
     * The constructor creates a task. Assigns the needed stack size and the
     * IP code pointer to the task structure.
     */
    Pr_Task(void *(*_taskCode)(void *), /*  Task IP */
            ROOMPriority _priority,     /*  task priority   */
            unsigned _stackSize);       /*  stack size  */

#endif

    /**
     * This methos stablis the task priority
     * 
     * \param _priority:    This parameter is the task priority
     */
    void SetPriority(ROOMPriority _priority);

    /**
     * This method modifies temporally the task priority.
     * 
     * \param _priority:    the task priority
     * 
     * The task priority is only modify when the priority pass argument is 
     * greater than the current task priority. The attribute priorityTmp takes
     * the mayor value between the priority passed an its value.
     * The attribute priorityMsg is not modified.
     */
    void SetMaxPrioTmp(ROOMPriority _priority);

    /**
     * This method stablish the message treatment priority.
     * 
     * \param _priority:    the task priority
     * 
     * The priorityMsg attribute is modified only if the priority passed 
     * argument is greater than priorityMsg.
     * The attributes priorityMsg and priorityTmp takes the greater value
     * between these and the priority passed argument.
     */
    void SetMaxPrioMsg(ROOMPriority _priority);

    /**
     * This method restores the task priority to the message priority.
     * 
     * The method is invoked after been modified the temporal priority task
     * (priorityTmp). The priorityTmp takes the priorityMsg value to maintain
     * the coherence between the real priority task and the priorityTmp value.
     */
    void RestorePrioMsg(void);
    
    /**
     * Returns the task priority
     * 
     * \return This method returns the current task priority.
     */
    ROOMPriority GetTaskPrio();
    
    /**
     * Defaul Destructor
     */
    ~Pr_Task(){}
};


//********************************************************
//********************  Pr_Time  *************************
//********************************************************

/**
 * \class Pr_Time
 * 
 * \brief   This class implements the time EDROOM time management.
 * 
 * The class allows to modify the time, set absolute and relative delays and
 * work with the time in many units.
 */
class Pr_Time
{
friend class Pr_Kernel;
friend class Pr_SemaphoreBin;
friend class Pr_IRQEvent;

public:
    /** This enumeration declares all the time units */
    enum unit {/** Time ticks  */
        ticks,   
        /** Microseconds    */
        micros,  
        /** Milliseconds    */
        millis,  
        /** Seconds */
        seconds, 
        /** Minutes */
        minutes  
    };

private:

    /** This parameter stores the system time    */
     struct timeval time;
    
public:

    /**
     * Default Constructor
     */
    Pr_Time();

    /**
     * Constructor.
     * 
     * \param   _unit:  This parameter is the time unit.
     * \param   _value: This parameter is the time value.
     * 
     * The constructor inits the object with the time value and units passed
     * as parameters.
     */
    Pr_Time::Pr_Time(struct timeval _time);

    /**
     * Constructor.
     * 
     * \param   _seconds:   This parameter is the time value in seconds.
     * 
     * This constructor inits the object with the time value expressed in 
     * seconds.
     */
    Pr_Time(float _seconds);

    /**
     * Constructor.
     * 
     * \param   _time:  This parameter is a Pr_Time object used to init the 
     * current object.
     * 
     * This contructor inits the current object with the same value of the object
     * passed as parameter.
     */
    Pr_Time(Pr_Time &_time);
    
    /**
     * Relative Delay method.
     * 
     * \param   _delay:  This parameter is the time delay
     * 
     * The method sleeps the caller task during the delay passed as parameter.
     */
    friend void Pr_DelayIn( const Pr_Time &_delay);

    /**
     * Absolute Delay method.
     * 
     * \param   _time:  This parameter is the time to be awaked
     * 
     * The method sleeps the caller. The system awake the task at the specified 
     * time  (_time)
     */
    friend void Pr_DelayAt( const Pr_Time &_time);
    
    /**
     * Get the current system time.
     * 
     * This method stores in the _time object the current system time.
     */
    void GetTime(void);    
    
    /**
     * + Operator overload
     * 
     * \param   _time:  This parameter is the time structure.
     */
    Pr_Time& operator+=(const Pr_Time& _time);

    /**
     * - Operator overload
     * 
     * \param   _time:  This parameter is the time structure.
     */
    Pr_Time& operator-=(const Pr_Time& _time);

    /**
     * = operator overload
     * 
     * \param   _time:  This parameter is the time structure.
     */
    Pr_Time& operator=(const Pr_Time& _time);

    /**
     * == operator overload
     * 
     * \param   _time:  This parameter is the time structure.
     */
    int operator==(const Pr_Time& _time);

    /**
     * != operator overload
     * 
     * \param   _time:  This parameter is the time structure.
     */
    int operator!=(const Pr_Time & _time);

    /**
     * > operator overload
     * 
     * \param   _time:  This parameter is the time structure.
     */
    int operator>(const Pr_Time& _time);

    /**
     * < operator overload
     * 
     * \param   _time:  This parameter is the time structure.
     */
    int operator<(const Pr_Time& _time);

    /**
     * >= operator overload
     * 
     * \param   _time:  This parameter is the time structure.
     */
    int operator>=(const Pr_Time& _time);

    /**
     * <= operator overload
     * 
     * \param   _time:  This parameter is the time structure.
     */
    int operator<=(const Pr_Time& _time);
    
    /**
     * Value of the system clock in minutes
     * 
     * \return This method returns the value of the system clock in minutes.
     */
    float Mints() const;

    /**
     * Value of the system clock in seconds
     * 
     * \return This method returns the value of the system clock in seconds.
     */
    float Secns() const;

    /**
     * Value of the system clock in milliseconds
     * 
     * \return This method returns the value of the system clock in milliseconds.
     */
    float Millis() const;
    
    /**
     * Value of the system clock in ticks
     * 
     * \return This method returns the value of the system clock in ticks.
     */
    float Ticks() const;

    /**
     * Value of the system clock in microseconds
     * 
     * \return This method returns the value of the system clock in microseconds.
     */
    float Micros() const;

    /**
     * \todo Implementar este método si hace falta.
     */
    static float period();
             
};

//********************************************************
//********************  Pr_IRQManager ********************
//********************************************************

/** Interrupt handler routine prototype */
typedef void (* Pr_IRQHandler) (int);

class Pr_IRQManager{ 
 
 
public: 
 
    /**
     * This method enables the interrupt.
     * 
     * The method enables the interrupt indentifing by the IRQ number.
     * 
     * \param IRQ:  This parameter is the IRQ number.
     */
    static void Enable(uint32 IRQnum);
 
    /**
     * This method disables the interrupt.
     * 
     * The method disables the interrupt indentifing by the IRQ number.
     * 
     * \param IRQ:  This parameter is the IRQ number.
     */
    static void Disable(uint32 IRQnum);
  
    /**
     * This method installs the user interrupt handler.
     * 
     * The method installs an interrupt handler.
     * 
     * \param handler:  This parameter is the interrupt handler routine.
     * \param IRQnum:   This parameter is the irq vector number.
     */
    static void Install(Pr_IRQHandler handler,uint32 IRQnum);
    
    /**
     * This method restores the old interrupt handler routine in case of
     * the RTOS has one.
     * 
     * \param IRQnum:   This parameter is the interrupt vector number.
     */
    static void Restore(uint32 IRQnum);
    
    /**
     * Auxiliar debug method.
     * 
     * This method forces an interrupt. This is specific for the ERC32
     * architecture and it is allow us to check the rest of methods in this
     * class
     * 
     * \param IRQnum:   This parameter is the interrupt vector number.
     */
    static void Force(uint32 IRQnum);

}; 

//********************************************************
//********************  Pr_IRQEvent **********************
//********************************************************

/**
 * \class Pr_IRQEvent. This class implements the event handling.
 * 
 * The class allows the threads waitting for an interrupt event with the 
 * Wait methods.
 * The irq handlers will also signal the event to propagate it to the waiting
 * threads.
 */
 
class Pr_IRQEvent{

private:
    Pr_SemaphoreBin eventSem;
    
public:
    
#ifdef _EDROOM_IS_DEBUG_
    
    char *name;
    
    /**
     * Debug Constructor.
     * 
     * \param name: This parameter is the irq event name.
     * \param IRQ:  This parameter is the IRQ vector number.
     */
    Pr_IRQEvent(char * name,unsigned char IRQ);

#else

    /**
     * Constructor.
     * 
     * \param IRQ:  This parameter is the IRQ vector number.
     */
    Pr_IRQEvent(unsigned char IRQ);

#endif 

    /**
     * Signal event.
     * 
     * This method signals an interrupt. The signaling provokes the event 
     * propagation to the waiting threads.
     */
    void Signal();
    
    /**
     * Wait for an event.
     * 
     * This method allows threads to wait for an interrupt event.
     */
    void Wait();

    /**
     * Conditional Wait for an event.
     * 
     * This method allows threads to wait for an interrupt event.
     */
    bool WaitCond();
    
    /**
     * Timed Waitting for an event.
     * 
     * This method allows threads to wait for an interrupt event for a time.
     */
    bool WaitTimed(Pr_Time time);

};

#endif



