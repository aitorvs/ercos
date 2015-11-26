/*
    Copyright 2005-2006 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/

#include <posix/trace.h>
#include <posix/time.h>
#include <syscall.h>

//------------------------------------------------------------------------------

#define GET_SIZE(TYPE, FIELD) \
        (int)&(((TYPE*)0)->FIELD)
        
#define LEVEL_ID_MASK   0xFFFF0000
#define EVENT_ID_MASK   ~LEVEL_ID_MASK
#define LEVEL_ID_SHIFT  16
#define MAX_EVENTS      256

static uint8 event_ids[MAX_EVENTS];

//------------------------------------------------------------------------------

/******************************************************************************
 * STATIC ROUTINES DECLARATION
 *****************************************************************************/

/**
 * This routine fills some information in the event_info structure.
 * 
 * @param event_id:     This is the event identifier
 * @param event_info:   This parameter points to the event_info structure
 */
static void posix_trace_eventinfo_fill(posix_trace_event_info_t  *event_info);
/**
 * This routine saves an event into the kernel buffer stream.
 * 
 * @param event_info:   This parameters stores the structure that must be saved.
 */
static void posix_trace_save_event(posix_trace_event_info_t  event_info);
                                     
//------------------------------------------------------------------------------


/******************************************************************************
 * PUBLIC INTERFACE
 *****************************************************************************/

int posix_trace_create(pid_t pid, trace_attr_t attr, trace_id_t *trace_id)
{
    /*  We only can trace the current process -> minimal kernel */
    if(pid != 0)
        return -1;
        
    /*  Utilizar el attr.fullpolicy para establecer la politica del buffer  */
    //SYS_TraceStart(attr.fullpolicy, ...);
    
    /*  We only have one buffer stream so...    */
    *trace_id = 1;
    
    return 0;
}

int posix_trace_eventid_open(const char *name, trace_event_id_t *event)
{
    uint16 level_id;
    
    /*  We do nothing with the name field   */
    
    /*  The user pass the level identifier in the event field   */
    level_id = *event;
    
    /*  Reset the event value   */
    *event = 0;
    
    /*  
     * The high 16 bits has the level identifier.
     * The lower 16 bits has the event identifier
     */
    *event = ((level_id << LEVEL_ID_SHIFT) & LEVEL_ID_MASK) | 
            (event_ids[level_id++] & EVENT_ID_MASK);
    
    return 0;
}

#ifndef CONFIG_ERCOS_KERNEL_DISABLE_HIGHTRACE

int posix_trace_event(trace_event_id_t      event_id, 
                      const void            *data_ptr, 
                      size_t                data_len)

{
    posix_trace_event_info_t event_info;
    
    
    /*  
     * Fills all the header information less than the event_id, posix_data_len
     * and posix_extra_data fields
     */
    posix_trace_eventinfo_fill(&event_info);
    
    event_info.posix_event_id = event_id;

    /*  There is no fragments   */
    if(data_len <= TRACE_PAYLOAD_MAX_SEGMENT_SIZE)
    {
        /*  Fills posix_data_len and posix_extra_data fields  */  
        event_info.posix_extra_data = (void *)data_ptr;
        event_info.posix_data_len = data_len;
        
        posix_trace_save_event(event_info);
        
    }
    /*  Ohhh, some fragments must be sent   */
    else
    {
        int remind_size = data_len;

        event_info.posix_data_len = TRACE_PAYLOAD_MAX_SEGMENT_SIZE;
        
        do {
            /*  Places the pointer to the extra data    */
            event_info.posix_extra_data = (void *)((char *)data_ptr + 
                                          (data_len - remind_size));
            
            if(remind_size > TRACE_PAYLOAD_MAX_SEGMENT_SIZE)
            {
                /*  Increment the segment identifier    */
                event_info.posix_segment_id++;
            }
            else {
                /*  Segment identifier must be zero to mark the final segment */
                event_info.posix_segment_id = 0;
                
                /*  The extra data length is the remind_size  */
                event_info.posix_data_len = remind_size;
            }
            
            /*  Stores the header and the data into the stream buffer    */
            posix_trace_save_event(event_info);
            
            remind_size -= TRACE_PAYLOAD_MAX_SEGMENT_SIZE;            
            
        }while(remind_size > 0);
    }
    
    return 0;
}
#else

int posix_trace_event(trace_event_id_t      event_id, 
                      const void            *data_ptr, 
                      size_t                data_len)

{
    /*  dummy routine   */
    return 0;
}

#endif
/******************************************************************************
 * PRIVATE INTERFACE
 *****************************************************************************/
 
static void posix_trace_save_event(posix_trace_event_info_t  event_info)
{
    int header_size = GET_SIZE(posix_trace_event_info_t, posix_extra_data);
    
    /*  Store the header information in the kernel tracer stream buffer   */
    SYS_TracerSaveEvent((char *)&event_info, header_size);

    /*  Now store the extra data field into the kernel tracer stream buffer */
    SYS_TracerSaveEvent((char *)event_info.posix_extra_data, 
                                event_info.posix_data_len);
}

static void posix_trace_eventinfo_fill(posix_trace_event_info_t  *event_info)
{
    struct timespec timestamp;
    
    /*  Takes the timestamp */
    clock_gettime(CLOCK_REALTIME, &timestamp);
    
    /*  This code fills the event_info structure    */
    // The event id must placed after
    event_info->posix_segment_id = 0;
    event_info->posix_pid = 0;
    event_info->posix_timestamp = timestamp;
    event_info->posix_thread_id = SYS_GetCurrentTid();
    event_info->posix_prog_address = 0x0;
    
}

