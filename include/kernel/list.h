/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT on ERC-32.
*/

/*!
 * \file include/kernel/list.h
 * \brief Functions and MACROS to manage lists
 */

#ifndef __LIST_H
#define __LIST_H

//------------------------------------------------------------------------------

#include <sys/types.h>

//------------------------------------------------------------------------------

/**
 * \brief Obtains a precise element from a node of the list.
 * 
 * \param TYPE   Type of the element.
 * \param NODE   Node of the list. It is a \a OS_Node_t structure.
 * \param FIELD  Name of the element.
 */
#define GET_ELEMENT(TYPE,NODE,FIELD)                    \
                 ((TYPE*)((int)(NODE) -              \
                          (int)&((TYPE*)0)->FIELD))


#ifdef INLINE
#undef INLINE
#endif
#define INLINE

/**
 * \brief Iterates over a list.
 * 
 * \pos  The struct \a list_head to use as a loop counter.
 * \ls   The list.
 */
#define list_for_each(pos, ls) \
 for (pos = (ls)->first; pos != NULL; \
          pos = pos->next)

/**
 * \brief This macro returns the number of the list elements.
 * 
 * \param ls  The list.
 */
#define OS_ListGetNum(ls)   ((ls)->num)
//------------------------------------------------------------------------------

/**
 * \brief Obtains the last element from a given list.
 * 
 * \param ls  The list.
 * \return    The \a OS_Node_t structure corresponding to the last element of
 *            the list.
 */
OS_Node_t * OS_ListGetLast (OS_List_t * ls);

/**
 * \brief Obtains the next element of a list from a given node.
 * 
 * \param n  The node of the list previous to the one we wish to obtain.
 * \return   The next node of the list, following to the one passed as
 *           parameter.
 */
OS_Node_t * OS_ListGetNext (OS_Node_t * n);

/**
 * \brief Obtains the previous element of a list from a given node.
 * 
 * \param n  The node of the list following the one we wish to obtain.
 * \return   The previous node of the list, preceding to the one passed as
 *           parameter.
 */
OS_Node_t * OS_ListGetPrev (OS_Node_t * n);

/**
 * \brief Resets a given list.
 * 
 * \param ls  The list.
 * 
 * Makes zero all its parameters.
 */
void OS_ListReset (OS_List_t * ls);

void OS_ListNodeReset (OS_Node_t * n);

/**
 * \brief Insters a new element on the given list at the beginning.
 * 
 * \param ls  The list.
 * \param n   The \a OS_Node_t structure corresponding to the node to insert.
 */
void OS_ListInsert (OS_List_t * ls, OS_Node_t * n);

/**
 * \brief Inserts a new element on the given list at the end.
 * 
 * \param ls  The list.
 * \param n   The \a OS_Node_t structure corresponding to the node to insert.
 */
void OS_ListAppend (OS_List_t * ls, OS_Node_t * n);

/**
 * \brief Extracts the first element of the given list.
 * 
 * \param ls  The list.
 * \return    The reference to the \a OS_Node_t structure corresponding to the
 *            extracted node.
 */
OS_Node_t *OS_ListGetNode(OS_List_t *ls);

/**
 * \brief Extracts a specific node from the given list.
 * 
 * \param ls  The list.
 * \param n   The \a OS_Node_t structure corresponding to the node to extract.
 */
void OS_ListExtract (OS_List_t * ls, OS_Node_t * n);

/**
 * \brief Adds a new entry to a given list.
 * 
 * \param node  New entry to be added
 * \param head  List head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
void OS_ListAdd(OS_List_t * ls, OS_Node_t *node, OS_Node_t *head);

#endif
