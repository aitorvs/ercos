/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/

#include <sys/resources.h>
#include <syscall.h>

int getpriority(int which, int who)
{
    int error = -1;
    
    switch(which)
    {
        case PRIO_PROCESS:
            if(who == 0)
                return (SYS_GetCurrentTid());
        break;
    }
    
    return error;
}


int setpriority(int which, int who, int prio)
{
    int error = -1;
    
    switch(which)
    {
        case PRIO_PROCESS:
            if(who == 0) {
                int current_tid = SYS_GetCurrentTid();
                
                SYS_SetPriority(current_tid, prio);
            }
        break;
    }
    
    return error;
}



