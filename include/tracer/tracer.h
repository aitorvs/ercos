/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/

#ifndef _TRACER_H_
#define _TRACER_H_

#include <sys/types.h>
#include <kernel/irq.h>

//------------------------------------------------------------------------------

/** This is the stream buffer where the events are going to be annotated  */
struct buffer {
    
    /** This is the stream buffer   */
    uint8 buf[KCONFIG_TRACER_BUF_SIZE];
    
    /** the buffer full policy  */
    uint8 full_policy;
    
    /** This flag enables/disables the buffer writing  */
    volatile uint8 disable_writing;
    
    /** This flag enables/disables the buffer reading   */
    volatile uint8 disable_reading;

};

/** This is the stream buffer where the events are going to be annotated  */
extern struct buffer tracer_buffer;

/** This variable stores the write pointer of the circular buffer    */
extern volatile unsigned int write_pos;
extern volatile unsigned int read_pos;

/** This MACRO checks if the stream buffer writing is enabled  */
#define IS_WRITE_ENABLED    (0 == tracer_buffer.disable_writing)
/** This MACRO checks if the stream buffer reading is enabled  */
#define IS_READ_ENABLED     (0 == tracer_buffer.disable_reading)

/** This MACRO disables they writing over the stream buffer  */
#define DISABLE_WRITING()    (tracer_buffer.disable_writing = 1)
/** This MACRO disables the reading over the stream buffer  */
#define DISABLE_READING()     (tracer_buffer.disable_reading = 1)

/** This MACRO enables the writing over the stream buffer  */
#define ENABLE_WRITING()     (tracer_buffer.disable_writing = 0)
/** This MACRO enables the reading over the stream buffer  */
#define ENABLE_READING()      (tracer_buffer.disable_reading = 0)

/**
 * Documetnar esta macro
 */
#define ctt(a, b, c) (((a <= b) + (b < c) + (c < a)) >= 2)

#define bsm KCONFIG_TRACER_BUF_MASK

/**
 * This routine saves an event into the stream buffer.
 * 
 * @param   _data:      This parameter is the event data
 * @param   _data_size: This parameter is the even data size
 * 
 * OS_TracerSaveEvent(char *_data, unsigned int _data_size);
 */
#define OS_TracerSaveEvent(_p, _bc)    \
{   \
    char *p = _p;   \
    unsigned int bc = _bc;   \
    \
    if(IS_WRITE_ENABLED) {    \
        \
        if(ctt((read_pos), (write_pos), ((write_pos + bc) & bsm))) {     \
            while (bc--) {    \
                *((char *)tracer_buffer.buf + write_pos) = *(p++);    \
                write_pos = (write_pos + 1) & bsm;    \
            }   \
            \
            ENABLE_READING();    \
        }    \
        else DISABLE_WRITING();    \
    }   \
    \
}

//------------------------------------------------------------------------------

/** Buffer full policies    */
enum buffer_policy
{
    /**
     * This label defines the buffer full pilicy. This policy stablish that the 
     * trace system will OVERWRITE the events in case of the buffer is full
     */
    _TRACE_OVERWRITING = 0,

    /**
     * This label defines the buffer full pilicy. This policy stablish that the 
     * trace system will automatically stop the event trace when the buffer is 
     * full
     */
    _TRACE_UNTIL_FULL,

};

/**
 * Structure to support the retrieve tracer routines
 */
typedef struct trace_retrieve
{
    /** This routine performs the event retrieve    */
    void (*retrieveFn)(uint8 _car);

}OS_Trace_Retrieve_t;

//------------------------------------------------------------------------------

/**
 * This routine extracs a marked event and sends out to the UARTB port
 */
void OS_TracerRetrieveEvent(void);

/**
 * This routine inits the tracer
 */
void OS_TracerInit(void);

/**
 * This routine enables the tracer
 */
void inline OS_TracerStart(void);

/**
 * This routine increments circular buffer write pointer
 * 
 * @return  The routine returns the write pointer position
 */
void inc_write_pos (void);

#endif


