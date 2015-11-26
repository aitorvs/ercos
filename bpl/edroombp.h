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
 * $Id: edroombp.h 118 2005-11-16 16:21:10 +0100 (mié, 16 nov 2005) cesar $
 *  
 * $Revision: 118 $
 * $Date: 2005-11-16 16:21:10 +0100 (mié, 16 nov 2005) $
 * Last CVS commit by $Author: cesar $
 * 
 * Registro de cambios
 * 
 * $Log$
 * Revision 1.6  2005/10/12 16:35:08  aitor
 * Comments
 *
 * Revision 1.5  2005/09/30 08:52:02  aitor
 * The debug interface is now running.
 *
 * Revision 1.4  2005/09/28 16:37:56  aitor
 * Inlcuded the EDROOM debug mode in the tests. We also implemented the
 * Pr_IRQEvent class
 *
 * Revision 1.3  2005/09/26 11:43:14  aitor
 * Pr_IRQManager class added.
 *
 * Revision 1.2  2005/09/23 14:56:05  aitor
 * Modified the includes because the O.S code is now C++ compliant.
 *
 * Revision 1.1  2005/09/19 11:38:04  aitor
 * Restoring the project.
 *
 * Revision 1.11  2005/08/25 08:38:54  aitor
 * Pr_Task::GetTaskPrio() method implemented.
 *
 * Revision 1.10  2005/08/23 17:39:41  aitor
 * Pr_Semaphore::WaitTimed() method implemented.
 *
 * Revision 1.9  2005/08/22 15:52:01  aitor
 * Pr_Kernel::Start() method is implemented.
 *
 * Revision 1.8  2005/08/19 13:37:07  aitor
 * Thread specific storage implemented.
 *
 * Revision 1.7  2005/08/18 14:58:06  aitor
 * comments.
 *
 * Revision 1.6  2005/08/11 19:03:04  aitor
 * EDROOMPriority enumeration added.
 * We are also implement the Pr_SemaphoreRec class.
 *
 * Revision 1.5  2005/08/08 15:32:31  aitor
 * Include some new header files.
 *
 * Revision 1.4  2005/08/03 10:55:08  aitor
 * Implements the Pr_SemaphoreBin class.
 *
 * Revision 1.3  2005/08/03 10:39:46  aitor
 * Implement the Pr_Semaphore class.
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

#ifndef _EDROOM_BP_H_
#define _EDROOM_BP_H_

#include <stdio.h> // quitar este include en la version final.
#include <posix/pthread.h>
#include <posix/semaphore.h>
#include <posix/time.h>
#include <posix/intr.h>
#include <posix/trace.h>
#include <sys/resources.h>

#include "edroomdf.h"
#include "edroomtc.h"

enum EDROOMPriority{
    EDROOMprioURGENT = 0,
    EDROOMprioVeryHigh,
    EDROOMprioHigh,
    EDROOMprioNormal,
    EDROOMprioLow,
    EDROOMprioVeryLow,
    EDROOMprioMINIMUM,
    EDROOMprioIDLE,
}; 

#define EDROOM_PRIO_MINIMUM EDROOMprioIDLE

typedef unsigned EDROOMStackSizeType;

#define EDROOMClockTicksType unsigned int

#define _EDROOM_MainMustWait
#define _EDROOM_TASK_POSIX_
//#define _EDROOM_IS_DEBUG_

typedef unsigned char EDROOMByte;

//*****************************************************


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
     * The default semaphore value is 1.
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
    sem_t sem;

public:

#ifdef _EDROOM_IS_DEBUG_

    const char *name;

    Pr_SemaphoreBin(const char *p_name="semaphore", uint32 _value = 0);

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
     * Default Destructor.
     * 
     * The destructor destroys the semaphore.
     */
    ~Pr_SemaphoreBin()
    {
        sem_destroy(&sem);
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
    pthread_mutex_t mutex;
    
    /** This attribute is the mutex attr    */
    pthread_mutexattr_t attr;
    

public:

#ifdef _EDROOM_IS_DEBUG_

    const char * name;

    /** Default Debug constructor */
    Pr_SemaphoreRec(const char *name);

    /** Constructor for priority ceiling resource sempahore */
    Pr_SemaphoreRec(const char *name, int prioceiling);

#else

    /** Default constructor */
    Pr_SemaphoreRec();
    
    /** Constructor for priority ceiling resource sempahore */
    Pr_SemaphoreRec(int prioceiling);

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
    ~Pr_SemaphoreRec(){pthread_mutex_destroy(&mutex);}
};

/* Pr_Receive function header */
void Pr_Receive(void * ap_data,unsigned datalength);

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
    pthread_t   taskID;

#ifdef _EDROOM_IS_DEBUG_
    
    /** This is the task name   */
    const char *name;

#endif
    
    /** This is the thread attributes defined in <sys/types.h<   */
    pthread_attr_t attr;
              
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
    void ChangePriority(EDROOMPriority _priority);
    
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
    EDROOMPriority priorityMsg;
    
    /**
     * Temporal priority.
     */
    EDROOMPriority priorityTmp;
  
public:

    /**
     * Default Constructor
     */
    Pr_Task(){

#ifdef _EDROOM_IS_DEBUG_
	    name = "";
#endif

    }

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
            const char *_p_name,              /*  Task name   */
            EDROOMPriority _priority,     /*  task priority   */
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
            EDROOMPriority _priority,     /*  task priority   */
            unsigned _stackSize);       /*  stack size  */

#endif

    /**
     * This methos stablis the task priority
     * 
     * \param _priority:    This parameter is the task priority
     */
    void SetPriority(EDROOMPriority _priority);

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
    void SetMaxPrioTmp(EDROOMPriority _priority);

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
    void SetMaxPrioMsg(EDROOMPriority _priority);

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
    EDROOMPriority GetTaskPrio();
    
    /**
     * Defaul Destructor
     */
    ~Pr_Task(){}
};

/* Pr_Send function header */
void Pr_Send(Pr_Task &task, void *p_data);

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
        secns, 
        /** Minutes */
        mints  
    };

private:

    /** This parameter stores the system time    */
    struct timespec time;
    
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
    Pr_Time(struct timespec _time);

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
    Pr_Time(const Pr_Time &_time);
    
    /**
     * \todo Comentar este constructor
     */
    Pr_Time(Pr_Time::unit _units, uint32 _measure);
    
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
    
    /**
     * \todo Implementar este método si hace falta.
     */
    EDROOMClockTicksType GetTicks();
    
    
             
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
    static void Enable(intr_t IRQnum);
 
    /**
     * This method disables the interrupt.
     * 
     * The method disables the interrupt indentifing by the IRQ number.
     * 
     * \param IRQ:  This parameter is the IRQ number.
     */
    static void Disable(intr_t IRQnum);
  
    /**
     * This method installs the user interrupt handler.
     * 
     * The method installs an interrupt handler.
     * 
     * \param handler:  This parameter is the interrupt handler routine.
     * \param IRQnum:   This parameter is the irq vector number.
     */
    static void Install(Pr_IRQHandler handler,intr_t IRQnum);
    
    /**
     * This method restores the old interrupt handler routine in case of
     * the RTOS has one.
     * 
     * \param IRQnum:   This parameter is the interrupt vector number.
     */
    static void Restore(intr_t IRQnum);
    
    /**
     * Auxiliar debug method.
     * 
     * This method forces an interrupt. This is specific for the ERC32
     * architecture and it is allow us to check the rest of methods in this
     * class
     * 
     * \param IRQnum:   This parameter is the interrupt vector number.
     */
    static void Force(intr_t IRQnum);

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
    
    const char *name;
    
    /**
     * Debug Constructor.
     * 
     * \param name: This parameter is the irq event name.
     * \param IRQ:  This parameter is the IRQ vector number.
     */
    Pr_IRQEvent(const char * name,unsigned char IRQ);

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



