/*
 * Buddy system memory allocator.
 *
 * The original buddy system memory allocation algorithm was taken from
 * "The Art of Computer Programming, Volume 1, 3rd Edition", by
 * Donald E. Knuth, pages 442-444.
 *
 * Written by Miguel Masmano Tello <mmasmano@disca.upv.es>
 * (C) 2002 Release under the terms of the GPL Version 2
 *
 */

#ifndef _MM_H_
#define _MM_H_

#include <string.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------

/**
 * MACROS to print debug messages
 */
#define PRINT_MSG(message) printf(message)
#define PRINT_MSG_1(message, arg) printf(message, arg)
#define PRINT_MSG_2(message, arg_1, arg_2) printf(message, arg_1, arg_2)
#define PRINT_MSG_6(message, arg_1, arg_2, arg_3, arg_4, arg_5, arg_6) printf(message, arg_1, arg_2, arg_3, arg_4, arg_5, arg_6)

#define PRINT_DBG_C(message) printf(message)
#define PRINT_DBG_D(message) printf("%i", message);
//#define PRINT_DBG_F(message) printf("%f", message);
#define PRINT_DBG_H(message) printf("%x", message);

//-----------------------------------------------------------------------------

/******************************************************************************
 * PUBLIC INTERFACE
 *****************************************************************************/

/** 
 * This function initialite all buffers needed  
 * by the buddy algorithm 
 * 
 * @param max_chunk_log2_size:  This parameter is the total amount of the pool
 * memory (log2 of the amount of memory)
 * 
 * @param min_chunk_log2_size:  This parameter is the minimum size that could
 * be allocated
 * 
 * @param initial_buffer:   This parameter is the pointer to the memory over
 * that the dynamic memory allocations will be performed
 * 
 * @return The routine returns 0 on success and -1 in any other case
 */
int OS_MMInitMemoryPool (int max_chunk_log2_size,
           int min_chunk_log2_size, char *initial_buffer);

/**
 * This routine destroys the memory pool reserved to be used in the dynamic
 * memory allocations
 */
void OS_MMDestroyMemoryPool(void);

/**
 * See man malloc
 * 
 * @param size:  This parameter is the amount of memory that must be allocated
 * 
 * @return The routine returns a pointer to the allocated memory or NULL in any
 * other case
 */
void *OS_Malloc (size_t size);

/*  
 * See man free 
 * 
 * @param ptr:  pointer to the memory that must be freed
 * 
 * OS_Fee() is only guaranteed to work if ptr is the address of a block
 * allocated by rt_malloc() (and not yet freed). 
 */
void OS_Free (void *ptr);

/******************************************************************************
 * DEBUGGING ROUTINES
 *****************************************************************************/

#ifdef __DEBUG__

/**
 *  memory_dump does a dumped of the memory context
 */
void memory_dump (void);

/**
 * \todo Documentar este método
 */
static void print_chunk(chunk_header_t *chunk);

/**
 * \todo Documentar este método
 */
int check_mem (void);

/**
 * \todo Documentar este método
 */
void global_state (int *free, int *used, int *overhead);

/**
 * \todo Documentar este método
 */
void free_blocks_context(void);

/**
 * \todo Documentar este método
 */
void statistics_data (void);

#endif // #ifdef __DEBUG__

#endif
