
#ifndef __EDROOMADDS_H__
#define __EDROOMADDS_H__

extern "C" {
#include <stdio.h>
#include <string.h>
#include <posix/trace.h>
}

#define UINT_MAX 4294967295UL

/*
 * EDROOMStream class
 */

class ios
{
	public:

	static int app;
};

class EDROOMstream
{
    private:
	bool traced;

	void trace(const int & integer);
	void trace(const unsigned int & uinteger);
	void trace(const float & floatnum);
		
	public:
	void open( const char *string, int dummy = 0);
	void close(void) {}
	int is_open() { return (1); }
	int good() { return (1); }
	
	EDROOMstream &operator<<( const char * string );
	EDROOMstream &operator<<( const int & integer);
	EDROOMstream &operator<<( const unsigned int & uinteger);
	EDROOMstream &operator<<( const float & floatnum );
	EDROOMstream() { traced = false; }
	EDROOMstream (const char *string) { open(string); }
};

extern EDROOMstream EDROOMFError;
extern EDROOMstream EDROOMFDeb;
extern EDROOMstream cout;

extern trace_event_id_t event;

int exit(int errcode);

extern "C" {
 	inline void __builtin_delete(void *p) {};
	inline void __pure_virtual() {};
	inline void __rtti_si(){};
	inline void __rtti_user(){};
}

#endif
