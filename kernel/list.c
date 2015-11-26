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
 * \file kernel/list.c
 * \brief List management functions definition.
 */

#include <kernel/list.h>
#include <kernel/irq.h>

//------------------------------------------------------------------------------

/*
 * \brief Inserts a new entry between two known consecutive entries.
 * 
 * \param node  Node to insert.
 * \param prev  First of the two consecutive entries.
 * \param next  Second of the two consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_add(OS_Node_t *node,OS_Node_t *prev,OS_Node_t *next);

//------------------------------------------------------------------------------

OS_Node_t *OS_ListGetNode(OS_List_t *ls)
{
    OS_Node_t *n;
    
    n =ls->first;
    
    if(n != NULL) {
        if (n->next)
            n->next->prev = n->prev;
        else
            ls->last = n->prev;
        
        if (n->prev)
            n->prev->next = n->next;
        else
            ls->first = n->next;
        
        n->next = n->prev = NULL;
        
        ls->num --;
        n->inside = 0;
    }

    return n;
}


OS_Node_t * OS_ListGetLast (OS_List_t * ls)
{
  return ls->last;
}

OS_Node_t * OS_ListGetNext (OS_Node_t * n)
{
  return n->next;
}

OS_Node_t * OS_ListGetPrev (OS_Node_t * n)
{
  return n->prev;
}

void OS_ListReset (OS_List_t * ls)
{

    ls->first = NULL;
    ls->last = NULL;
    ls->num = 0;

}

void OS_ListNodeReset (OS_Node_t * n)
{

    n->next = n->prev = NULL;
    n->inside = 0;

}

void OS_ListInsert (OS_List_t * ls, OS_Node_t * n)
{

    if(n->inside == 0) {
        n->prev = NULL;
    
        if ((n->next = ls->first))
            n->next->prev = n;
        else
            ls->last = n;
    
        ls->first = n;
        ls->num ++;
        n->inside = 1;
    }

}

void OS_ListAppend (OS_List_t * ls, OS_Node_t * n)
{

    if(n->inside == 0) {
        n->next = NULL;
    
        if ((n->prev = ls->last))
            n->prev->next = n;
        else
            ls->first = n;
    
        ls->last = n;
        ls->num ++;
        n->inside = 1;
    }
        
}

void OS_ListExtract (OS_List_t * ls, OS_Node_t * n)
{

    if(n->inside) {

        if (n->next)
             n->next->prev = n->prev;
        else
            ls->last = n->prev;
        
        if (n->prev)
            n->prev->next = n->next;
        else
            ls->first = n->next;
        
        n->next = n->prev = NULL;
        
        n->inside = 0;
            
        ls->num--;
    }

}

void OS_ListAdd(OS_List_t * ls, OS_Node_t *node, OS_Node_t *head)
{

    if(node->inside == 0) {
        __list_add(node, head, head->next);
        node->inside = 1;
        ls->num++;
    }

}

static inline void __list_add(OS_Node_t *node,OS_Node_t *prev,OS_Node_t *next)
{
    next->prev = node;
    node->next = next;
    node->prev = prev;
    prev->next = node;
}


