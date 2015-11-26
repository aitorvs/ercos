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
 * \file include/posix/trace/trace.h
 * \brief POSIX trace service definitions.
 */

#ifndef __TRACE_POSIX_TRACE_H
#define __TRACE_POSIX_TRACE_H

#include <sys/types.h>
#include <tracer/tracer.h>


//------------------------------------------------------------------------------


/** \brief It defines the maximum payload size */
#define TRACE_PAYLOAD_MAX_SEGMENT_SIZE    16
/** \brief Trace buffer behaviour policy. When the buffer is full, no more
 *         messages are included in the buffer
 */
#define POSIX_TRACE_UNTIL_FULL  _TRACE_UNTIL_FULL
/** \brief Trace buffer behaviour policy. When the buffer is full, a new
 *         message overwrites a previous one.
 */
#define POSIX_TRACE_OVERWRITING _TRACE_OVERWRITING

//------------------------------------------------------------------------------
 
/**
 * \brief The event_id type.
 * 
 * The event_id is structure as shows below.
 * 
 * The level identifier identifies the level in which the event has been defined
 * 
 * The event identifier identifies the different levels that can to be traced.
 * 
 * The user pass the level identifier in the creation of the event. The system
 * fills the event identifier a returns it to the user
 * 
 * ----------------------------------------------------------------------------
 * |   Level identifier (16 bits)     |       Event identifier (16 bits)      |
 * ----------------------------------------------------------------------------
 */
typedef  uint32         _trace_event_id_t;

/**
 * \brief The process identifier. Because of it is not used it only has a byte
 */
typedef uint8           pid_t;

/**
 * \brief The trace identifier. We only have one stream buffer, so we only have
 *        one trace system -> one byte
 */
typedef uint8           trace_id_t;



#endif

