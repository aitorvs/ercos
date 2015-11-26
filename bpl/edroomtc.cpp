

#include "edroomtc.h"

trace_event_id_t event;

EDROOMstream cout;

void EDROOMstream::trace(const int & integer)
{
    char buffer[1024];
    unsigned int numchar = 0;
    numchar = sprintf(buffer, "%d", integer);

    posix_trace_event(event, (void *)buffer, numchar);
}

void EDROOMstream::trace(const unsigned int & uinteger)
{
    char buffer[1024];
    unsigned int numchar = 0;
    numchar = sprintf(buffer, "%d", uinteger);

    posix_trace_event(event, (void *)buffer, numchar);
}

void EDROOMstream::trace(const float & floatnum)
{
    char buffer[1024];
    unsigned int numchar = 0;
    numchar = sprintf(buffer, "%d", (uint32)floatnum);

    posix_trace_event(event, (void *)buffer, numchar);
}

void EDROOMstream::open( const char *string, int dummy)
{
    traced = true;
    /* We will suppose that there will be only one kind
     * of stream for the EDROOM level. We will assign this
     * level the value 1. Since the information is passed
     * through the data strings, there is no need for more
     * than one event */

    trace_attr_t attr;
    trace_id_t trace_id;

    posix_trace_create(0, attr, &trace_id);

    /* Now we must stablish the one and only event */

    event = 1; /* Level 1 */

    posix_trace_eventid_open("EDROOM event", &event);
}

EDROOMstream & EDROOMstream::operator<<( const char *string )
{
    if (!traced)
	printf("%s", string);
    else
    	posix_trace_event(event, (void *)string, strlen(string));
    /* No problem with the string. Just throw it all through the posix trace */

    return *this;
}

EDROOMstream & EDROOMstream::operator<<( const int & integer )
{

    
    if (!traced)
	printf("%d", integer);
    else
	trace(integer);
    
    return *this;
}

EDROOMstream & EDROOMstream::operator<<( const unsigned int & uinteger )
{

    if (!traced)
	printf("%d", uinteger);
    else
	trace(uinteger);
    
    return *this;
}

EDROOMstream & EDROOMstream::operator<<( const float & floatnum )
{
    if (!traced)
	printf("%d", (uint32)floatnum);
    else
	trace(floatnum);
    
    return *this;
}

int ios::app = 0;

int exit(int errcode)
{
	*((volatile unsigned int *) 0x01f80008) = 0;

	/* We're not supposed to reach HERE */
	return (0);
}
