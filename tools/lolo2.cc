#include <iostream>
#include <fstream>
#include <vector>

using namespace std;


typedef unsigned int uint32;
typedef unsigned int tid_t;

enum kevent_id_t {
    KEVENT_THREAD_CREATE=0,
    KEVENT_LOCK_CATCHED,
    KEVENT_LOCK_BLOCKED,
    KEVENT_UNLOCK,
    KEVENT_TRYLOCK,
    KEVENT_THREAD_SLEEP,
    KEVENT_THREAD_EXIT,
    KEVENT_SCHEDULE_IN,
    KEVENT_SCHEDULE_OUT,
    KEVENT_MAX_EVENTS,
};

struct ktracer_events_t {
	    
	/** Event generation time   */
	unsigned long long timestamp;
		    
	/** Id of the event generator thread    */
	tid_t       thread_id;

	unsigned int semaphore_id;
	
	/** Trace event type idenfication   */    
	string kevent_id;

};

class VSemaphores : private vector<int>
{
    public :
	int &operator[](int position)
	{
	    if (position >= size())
		resize(position + 1);
	    return vector<int>::operator[](position);
	}
};

class VTasks : private vector<int>
{
    public :
	int &operator[](int position)
	{
	    if (position >= size())
		resize(position + 1);
	    return vector<int>::operator[](position);
	}
};

int main(int argc, char * argv[])
{
    unsigned int initial_time = 0;
    double time_seconds = 0;
    unsigned int max_semaphores = 0;
    ktracer_events_t event;
    ifstream original;
    ofstream destination;

    VSemaphores semaphores;
    VTasks tasks;

    if (argc < 3)
    {
	cout << "Way of use: lolo2 ORIGINAL_FILE DESTINATION_FILE\n";
	exit(1);
    }

    original.open(argv[1]);
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
    else
    {
	destination << "DECIMAL_DIGITS 6\n";
	destination << "ZOOM_X 1\n";
    destination << "LINE_NAME 0 Sched\n";
    destination << "LINE_NAME 1 Idle task\n";
//	destination << "ZOOM_Y 1\n";
	destination << "PALETTE Rainbow\n\n";

	destination << fixed;
	destination.precision(6);

	destination << "0.000000\tREADY-B 0\n";
	destination << "0.000000\tSTART 0\n";
	destination << "0.000000\tEXEC-B 0\n";
    }

    if (original.good())
    {
	original >> hex >> event.timestamp >> dec >> event.thread_id >> event.kevent_id;
	initial_time = event.timestamp;
    }
    else
    {
	cout << "ERROR: reading from the original log file\n";
	exit(1);
    }

    while (original.good())
    {
	event.timestamp -= initial_time;
	time_seconds = (double)(event.timestamp / (double)1000000);
	
	if (event.kevent_id == "THREAD_CREATE")
	{
	    destination << time_seconds << "\tSTART " << event.thread_id << "\n";
	    destination << time_seconds << "\tREADY-B " << event.thread_id << "\n\n";
	    tasks[event.thread_id] = true;
	    
	}
	else if (event.kevent_id == "LOCK_CATCHED")
	{
	    event.semaphore_id = ((event.thread_id & ~0xFFFFUL) >> 16);
	    event.thread_id = event.thread_id & 0xFFFFUL;
	    if (tasks[event.thread_id])
	    {
		destination << time_seconds << "\tEXEC-E " << event.thread_id;
		for (int i = 0; i <= max_semaphores; i++)
		{
		    if (semaphores[i] == event.thread_id) 
		    {
			destination << " gray70";
			break;
		    }
		}
		destination << "\n" << time_seconds << "\tEXEC-B " << event.thread_id << "\n";
		destination << time_seconds << "\tREADY-B " << event.thread_id << "\n\n";
	    }
	    else
		tasks[event.thread_id] = true;	    

	    semaphores[event.semaphore_id] = event.thread_id;
	    max_semaphores = (max_semaphores > event.semaphore_id) ? max_semaphores : event.semaphore_id;
	    destination << time_seconds << "\tLOCK " << event.thread_id << " s" << event.semaphore_id << "\n";
	}
	else if (event.kevent_id == "LOCK_BLOCKED")
	{
	    event.semaphore_id = ((event.thread_id & ~0xFFFFUL) >> 16);
	    event.thread_id = event.thread_id & 0xFFFFUL;
	    destination << time_seconds << "\tBLOCK " << event.thread_id << " s" << event.semaphore_id << "\n";
	    destination << time_seconds << "\tREADY-E " << event.thread_id << "\n\n";
	    tasks[event.thread_id] = false;
	} 
	else if (event.kevent_id == "UNLOCK")
	{
	    event.semaphore_id = ((event.thread_id & ~0xFFFFUL) >> 16);
	    event.thread_id = event.thread_id & 0xFFFFUL;
	    destination << time_seconds << "\tUNLOCK " << event.thread_id << " s" << event.semaphore_id << "\n";
	    if ( semaphores[event.semaphore_id] == event.thread_id )
	    {
	    	destination << time_seconds << "\tEXEC-E " << event.thread_id << " gray70\n";
	    	destination << time_seconds << "\tEXEC-B " << event.thread_id << "\n\n";
		semaphores[event.semaphore_id] = -1; 
	    }
	}
	else if (event.kevent_id == "THREAD_SLEEP")
	{
	    destination << time_seconds << "\tREADY-E " << event.thread_id << "\n";
	    destination << time_seconds << "\tEXEC-E " << event.thread_id;
	    for (int i = 0; i <= max_semaphores; i++)
	    {
		if (semaphores[i] == event.thread_id) 
		{
		    destination << " gray70";
		    break;
		}
	    }
	    destination << "\n\n";
	}
	else if (event.kevent_id == "SCHED_OUT")
	{
	    destination << time_seconds << "\tREADY-E " << 0 << "\n";
	    destination << time_seconds << "\tEXEC-E " << 0 << "\n";
	    destination << time_seconds << "\tREADY-B " << event.thread_id << "\n";
	    destination << time_seconds << "\tEXEC-B " << event.thread_id << "\n\n";
	} 
	else if (event.kevent_id == "SCHED_IN")
	{
	    if (event.thread_id != 0) 
	    {
		destination << time_seconds << "\tREADY-E " << event.thread_id << "\n";
		destination << time_seconds << "\tEXEC-E " << event.thread_id;
		for (int i = 0; i <= max_semaphores; i++)
		{
		    if (semaphores[i] == event.thread_id) 
		    {
		    	destination << " gray70";
			break;
		    }
		}
	    	destination << "\n" << time_seconds << "\tREADY-B " << 0 << "\n";
	    	destination << time_seconds << "\tEXEC-B " << 0 << "\n\n"; 
	    }
	} 
	else if (event.kevent_id == "THREAD_EXIT")
	{
	    destination << time_seconds << "\tREADY-E " << event.thread_id << "\n";
	    destination << time_seconds << "\tEXEC-E " << event.thread_id;
	    for (int i = 0; i <= max_semaphores; i++)
	    {
		if (semaphores[i] == event.thread_id) 
		{
		    destination << " gray70";
		    break;
		}
	    }
	    destination << "\n" << time_seconds << "\tSTOP " << event.thread_id << "\n\n";
	}

	original >> hex >> event.timestamp >> dec >> event.thread_id >> event.kevent_id;
    }

    original.close();
    destination.close();

    return 0;
}




