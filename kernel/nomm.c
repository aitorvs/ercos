/*
    Copyright 2005-2006 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/

#include <kernel/nomm.h>
#include <kernel/debug.h>

//------------------------------------------------------------------------------

static void *stack_address = 0x0;

//------------------------------------------------------------------------------

void OS_Free (void *ptr) {
}

void *OS_Malloc (size_t size) {
    stack_address -= size;
    
    DEBUG ("(size=%p); returning %p", size, stack_address);
    return (stack_address);
}

int OS_MMInitMemoryPool(int max_chunk_log2_size, int min_chunk_log2_size,
                        void *buffer)
{
    /*  Init the begin of all stack addresses */
    stack_address = buffer;
    return 0;
}

