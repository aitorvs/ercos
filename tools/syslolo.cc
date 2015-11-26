#include <iostream>
#include <fstream>

using namespace std;

typedef unsigned long long  uint64;
typedef unsigned int uint32;
typedef char byte;

typedef union uint32_switch
{
    byte MsgBigEnd[sizeof (uint32)];
    uint32 Val;
} uint32_switch_t;

typedef union uint64_switch
{
    byte MsgBigEnd[sizeof (uint64)];
    uint64 Val;
} uint64_switch_t;

struct systracer_events16 
{
    /** Traced system call identifier   */
    uint32      sys_id;

    /** System call timestamp   */
    uint64   timestamp;

    /** System call argument    */
    uint32      arg;

}__attribute__((packed));

typedef struct systracer_events16 systracer_events16_t;

struct systracer_events24
{
    /** Traced system call identifier   */
    uint32      sys_id;

    /** System call timestamp   */
    uint64   timestamp;

    /** Argument one    */
    uint32      arg1;

    /** Argument two    */
    uint32      arg2;

}__attribute__((packed));

typedef struct systracer_events24 systracer_events24_t;

/**
 * All of this definitions are the traced system calls
 */

#define SYSTRACE_THREAD_SLEEP       0
#define SYSTRACE_THREAD_CREATE      1
#define SYSTRACE_SEM_WAIT           2
#define SYSTRACE_SEM_POST           3
#define SYSTRACE_SEM_TRYWAIT        4
#define SYSTRACE_MUTEX_LOCK         5
#define SYSTRACE_MUTEX_UNLOCK       6
#define SYSTRACE_MUTEX_TRYLOCK      7
#define SYSTRACE_THREAD_EXIT        8
#define SYSTRACE_IRQ_EVENT          9
#define SYSTRACE_CLOCK_EVENT        10
#define SYSTRACE_HANDLE_EVENT       11
#define SYSTRACE_SEM_INIT           12
#define SYSTRACE_MUTEX_INIT         13

/** This system calls needs 24 bytes to be traced   */
#define SYSTRACE_SET_PRIORITY       101
#define SYSTRACE_SET_TMP_PRIORITY   102
#define SYSTRACE_SEM_TIMEDWAIT      103

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
    uint64_switch_t timestamp;
    uint32_switch_t sys_id, argument;
    uint32 originaldata32bits;
    uint64 originaldata64bits;
    
    ifstream original;
    ofstream destination;

    if (argc < 3)
    {
	cout << "Way of use: syslolo ORIGINAL_FILE DESTINATION_FILE\n";
	exit(1);
    }

    original.open(argv[1], ios::in | ios::binary);
    
    if (!original.is_open())
    {
	cout << "ERROR: opening the original log file\n";
	exit(1);
    }

    destination.open(argv[2]);
    if (!destination.is_open())
    {
	cout << "ERROR: opening the destination log file\n";
	exit(1);
    }

    if (!original.good())
    {
	cout << "ERROR: reading from the original log file\n";
	exit(1);
    }

    do
    {
	original.read((char *)(&(originaldata32bits)), sizeof(uint32));
	do_switch_uint32(&sys_id, (char *)&originaldata32bits);

	destination << sys_id.Val << " ";

	original.read((char *)(&(originaldata64bits)), sizeof(uint64));
	do_switch_uint64(&timestamp, (char *)&originaldata64bits);

	destination << timestamp.Val << " ";

	original.read((char *)(&(originaldata32bits)), sizeof(uint32));
	do_switch_uint32(&argument, (char *)&originaldata32bits);

	if (sys_id.Val == SYSTRACE_THREAD_SLEEP)
	{
	    uint64 dummy = ((uint64)argument.Val << 32);
	    
	    original.read((char *)(&(originaldata32bits)), sizeof(uint32));
	    do_switch_uint32(&argument, (char *)&originaldata32bits);

	    dummy |= argument.Val;
	    destination << dummy << " 0";
	}
	else
	{
	    destination << argument.Val;

	    if (sys_id.Val > 100)
	    {
		original.read((char *)(&(originaldata32bits)), sizeof(uint32));
		do_switch_uint32(&argument, (char *)&originaldata32bits);

		destination << " " << argument.Val;
	    }
	    else
		destination << " 0";
	}

	destination << endl;

    } while (original.good());

    original.close();
    destination.close();

    return 0;
}




