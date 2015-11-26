/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/

/*!
 * \file include/kernel/context.h
 * \brief Context switch inter-layer adapting functions
 */

#ifndef _CONTEXT_H_
#define _CONTEXT_H_

/**
 *  \brief Performs a Context Switch between two threads
 *
 *  \param _current  Context record in which to save the current context.
 *  \param _heir     Context record of the thread that is to be restored
 */
#define _OS_Context_Switch( _current, _heir ) \
    do { \
        _CPU_Context_Switch( _current, _heir ); \
    }while(0)

/**
 * \brief Calls the Context Restore routine.
 * 
 * \param _heir  The thread context to be restored.
 */
#define _OS_Context_Restore( _heir ) \
    do { \
        _CPU_Context_Restore( _heir ); \
    }while(0)

#endif


