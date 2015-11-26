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
 * \file include/sys/resources.h
 * \brief POSIX priority management functions.
 */

#ifndef _RESOURCES_H_
#define _RESOURCES_H_

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * \brief ID label used when calling the getpriority() and setpriority()
 *        functions. */
#define PRIO_PROCESS    0

/**
 * \brief Obtains the priority of a given process.
 * 
 * \param which  Specific ID. Currently, it has only been defined the
 *               PRIO_PROCESS ID.
 * \param who    ID corresponding to the calling process. It has only been
 *               defined the ID of the current process, i. e. it must always
 *               take the value 0.
 * \return       Zero if the call refers to the current thread (\a who param is
 *               zero valued) with the PRIO_PROCESS ID (\a which is equal to
 *               PRIO_PROCESS), -1 otherwise.
 * 
 * Currently, this function only returns the priority of the current thread.
 */ 
int getpriority(int which, int who);

/**
 * \brief Sets the priority of a given process.
 * 
 * \param which  Specific ID. Currently, it has only been defined the
 *               PRIO_PROCESS ID.
 * \param who    ID corresponding to the calling process. It has only been
 *               defined the ID of the current process, i. e. it must always
 *               take the value 0.
 * \param prio   New priority of the task.
 * \return       Zero if the call refers to the current thread (\a who param is
 *               zero valued) with the PRIO_PROCESS ID (\a which is equal to
 *               PRIO_PROCESS), -1 otherwise.
 * 
 * Currently, this function can only be used to set the priority of the current
 * thread.
 */
int setpriority(int which, int who, int prio);

#ifdef __cplusplus
}
#endif

#endif

