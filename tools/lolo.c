#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


typedef unsigned long long  uint64;
typedef unsigned int uint32;
typedef int int32;
typedef short int16;
typedef unsigned short uint16;
typedef unsigned int tid_t;
typedef char byte;


typedef union uint16_switch{
      byte MsgBigEnd[sizeof (uint16)];
          uint16 Val;
}uint16_switch_t;

typedef union uint32_switch{
      byte MsgBigEnd[sizeof (uint32)];
          uint32 Val;
}uint32_switch_t;

typedef union uint64_switch{
      byte MsgBigEnd[sizeof (uint64)];
          uint64 Val;
}uint64_switch_t;

typedef union int16_switch{
      byte MsgBigEnd[sizeof (int16)];
          int16 Val;
}int16_switch_t;

typedef union int32_switch{
      byte MsgBigEnd[sizeof (int32)];
          int32 Val;
}int32_switch_t;

typedef union float_switch{
      byte MsgBigEnd[sizeof (float)];
          float Val;
}float_switch_t;

typedef union double_switch{
      byte MsgBigEnd[sizeof (double)];
          double Val;
}double_switch_t;


typedef enum {
    KEVENT_THREAD_CREATE=0,
    KEVENT_LOCK_CATCHED,
    KEVENT_LOCK_BLOCKED,
    KEVENT_UNLOCK,
    KEVENT_THREAD_SLEEP,
    KEVENT_THREAD_EXIT,
    KEVENT_SCHEDULE_IN,
    KEVENT_SCHEDULE_OUT,
    KEVENT_MAX_EVENTS,
}kevent_id_t;

#define EVENT_VALUE(event)  (kevent_id.Val & 0x0000FFFF)

#define SEM_ID(event)   ((kevent_id.Val & 0xFFFF0000) >> 16)

typedef struct ktracer_events {

    /** Trace event type idenfication   */    
    kevent_id_t kevent_id;

    /** Event generation time   */
    uint64      timestamp;

    /** Id of the event generator thread    */
    tid_t       thread_id;

} ktracer_events_t;

static char *kevents[] = {"THREAD_CREATE", "LOCK_CATCHED", "LOCK_BLOCKED", "UNLOCK", "THREAD_SLEEP","THREAD_EXIT", "SCHED_IN", "SCHED_OUT"};

void do_switch_double(double_switch_t * aux, byte stream[sizeof (double)]){
    int i;
    for (i=0; i< sizeof(double); i--)
        (*aux).MsgBigEnd[i]=stream[sizeof(double)-1-i];
}

void do_switch_uint32(uint32_switch_t * aux, byte stream[sizeof (uint32)]){
    int i;
    char *pbyte = stream;
    for (i=0; i< sizeof(uint32); i++) {
        (*aux).MsgBigEnd[i]=stream[sizeof(uint32)-1-i];
    }
}

void do_switch_uint64(uint64_switch_t * aux, byte stream[sizeof (uint64)]){
    int i;
    char *pbyte = stream;
    for (i=0; i< sizeof(uint64); i++) {
        (*aux).MsgBigEnd[i]=stream[sizeof(uint64)-1-i];
    }
}

int main(int argc, char * argv[])
{
    int log_fd;
    ktracer_events_t *events;
    struct stat s;
    int n_events, i;

    if (argc < 2)
    {
	printf("Using: lolo Log_File\n");
	exit(1);
    }

    if((log_fd = open(argv[1], O_RDWR)) == -1)
    {
	printf("ERROR: opening the log file %s\n", argv[1]);
	exit(1);
    }

    fstat(log_fd, &s);

    n_events = s.st_size / sizeof(ktracer_events_t);

    events = (ktracer_events_t *)mmap( NULL, s.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, log_fd, 0);

    for(i = 0; i < n_events; ++i)
    {
	uint32_switch_t thread_id, kevent_id;
	uint64_switch_t timestamp;

	do_switch_uint64(&timestamp, (char*)&events[i].timestamp);
	do_switch_uint32(&thread_id, (char*)&events[i].thread_id);
	do_switch_uint32(&kevent_id, (char*)&events[i].kevent_id);

	if ((kevent_id.Val >= 0) && (kevent_id.Val <= 8))
	{
	    printf("0x%llx %d %s\n", timestamp.Val, thread_id.Val, kevents[EVENT_VALUE(kevent_id.Val)]);
	}
	else
	{
	    printf("Reading error: wrong data\n");
	    close(log_fd);
	    exit(1);
	}
    }

    return 0;
}




