
#include <kernel/debug.h>

// helper type
typedef void (*func_ptr) (void);

// zero-terminated constructor tables for each group
extern func_ptr __ctors_GLOBAL__[];
extern void * _end_ctors;

/**
 * This routine is in charge of call all the C++ constructors in order to 
 * execute C++ code.
 */
void OS_CPPInit(void)
{
    unsigned int i = 0;
    
    while(__ctors_GLOBAL__[i] != 0)
    {
        /* DEBUG ("Calling constructor at %p", * (void **) (__ctors_GLOBAL__ + i)); */
        __ctors_GLOBAL__[i++] ();
    }  
}

