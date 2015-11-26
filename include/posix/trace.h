/*
    Copyright 2005-2006 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/

/*!
 * \file include/posix/trace.h
 * \brief POSIX functions related to tracing services
 */

#ifndef __POSIX_TRACE_H
#define __POSIX_TRACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <posix/trace/trace.h>

/* 
   -- PRIMITIVE SYSTEM DATA TYPES ----------------------------------
*/

/**
 * \brief Used to identify a trace event type.
 */
typedef  _trace_event_id_t   trace_event_id_t;

//#define PACKED_STRUCTURE __attribute__((packed))

/**  
 * \brief POSIX trace packet structure.
 */
struct posix_trace_event_info {
    
    /** Trace event type idenfication   */    
    trace_event_id_t    posix_event_id;
     
    /** This is the segment fragment identifier */
    uint8               posix_segment_id;
    
    /** Id of the event generator process   */
    uint8               posix_pid;
    
    /** This attribute is dummy in order to make a correct alignment */
    uint16              dummy;
    
    /** Event generation time */
    struct timespec     posix_timestamp;
    
    /** Id of the event generator thread */
    pthread_t           posix_thread_id;

    /** Address at which trace point was invoked */
    void *              posix_prog_address;
    
    /** Size of the extra dat   */
    size_t              posix_data_len;
    
    /** 
     * Any extra data that the system or the instrumented application wants
     * to associate with the event
     */
    void *              posix_extra_data;
    
}__attribute__((packed));

typedef struct posix_trace_event_info posix_trace_event_info_t;

/**
 * \brief Structure containing the configurable attributes of the tracing
 *        service.
 */
typedef struct trace_attr
{
    /** 
     * This attribute defines the policy to adopt in case of the buffer was full
     */
    uint8   fullpolicy;

}trace_attr_t;
//------------------------------------------------------------------------------

//int posix_trace_start(trace_id_t trid);

/**
 * \brief Creates a new event to be trace.
 * 
 * \param name   The name of the event.
 * \param event  The event identifier.
 * \return       There is no returned value defined.
 * 
 * The routine creates the new event and returns in the event parameter the new
 * event identifier. If the event was already created, its identifier is 
 * returned.
 */
int posix_trace_eventid_open(const char *name, trace_event_id_t *event);

/**
 * \brief Records the data in the trace system.
 * 
 * \param event_id  This parameter is the event identifier.
 * \param data_ptr  This parameter is the pointer to the data.
 * \param data_len  This parameters stores the data length.
 * \return          No return value is defined.
 * 
 * The \a posix_trace_event() function shall record the event_id and the user
 * data pointed to by \a data_ptr in the trace stream into which the calling
 * process is being traced and in which \a event_id is not filtered out. If the
 * total size of the user trace event data represented by \a data_len is not
 * greater than the declared maximum size for user trace event data, then the
 * truncation-status attribute of the trace event recorded is
 * \a POSIX_TRACE_NOT_TRUNCATED. 
 * Otherwise, the user trace event data is truncated to this declared maximum 
 * size and the truncation-status attribute of the trace event recorded is 
 * \a POSIX_TRACE_TRUNCATED_RECORD.
 */
int posix_trace_event(trace_event_id_t      event_id, 
                      const void            *data_ptr, 
                      size_t                data_len);

/**
 * \brief Creates a trace unit by creating a new stream buffer associated
 *        with.
 * 
 * \param pid       The process that belongs the stream buffer.
 * \param attr      This parameter is the attributes of the trace system.
 * \param trace_id  This parameters is the trace system identifier returned
 *                  by the routine.
 * \return          The routine returns 0 on success and other value in the
 *                  rest of cases.
 * 
 * The routine receives the identifier of the process that creates and belongs
 * the trace system. If this identifier is equal to 0, the caller process
 * identifier is taken. The routine also receives the trace system must be 
 * created with.
 */
int posix_trace_create(pid_t pid, trace_attr_t attr, trace_id_t *trace_id);

#ifdef __cplusplus
}
#endif


#endif

