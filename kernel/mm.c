#include <kernel/mm.h>
#include <kernel/list.h>

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


#define INIT_THREAD_MUTEX()

#define THREAD_LOCK()
#define THREAD_UNLOCK()

//#define __DEBUG__


/* 
 * The maximal chunk size is 2**MAX_CHUNK_SIZE 
 * and the minimal 2**MIN_CHUNK_SIZE 
 */

static int MAX_CHUNK_LOG2_SIZE;
static int KCONFIG_MIN_CHUNK_LOG2_SIZE;

static int MAX_CHUNK_SIZE;
static int MIN_CHUNK_SIZE;

/* byte_t and half_t definitions do the work more easy */

typedef unsigned char byte_t;
typedef unsigned short int half_t;
typedef unsigned int word_t;
typedef unsigned long int dword_t;

#define BLOCK_FREE 0 // USED = !FREE

/*
 * All the chunks have this format:
 * 
 *   This part is the tag_overhead
 * ------------------------------------
 * ----------------------------------------------------------------------------
 * | magic num | state_block | log2size | div_code | prev | next | FREE SPACE |
 * ----------------------------------------------------------------------------
 *    2 bytes      2 bytes     2 bytes     2 byte    2 byte 2 bytes  ((2 ** 
 *                                                                   log2size 
 *                                                                   bytes)
 *                                                              - tag_overhead)
 *
 *                                                  --------------------------
 *                                                 This part will be allocated 
 *                                    
 *
 *
 *
 *
 *  The free list of chunks is a double linked list with the next format:
 *
 *     -------------          ----------------------------------------
 *     | MAX_C_L_S | -------> | NULL | x | y | NULL | | | FREE SPACE |   |->
 *     |-----------|          ----------------------- |---------------   |
 *                                                    --------------------
 *       .........
 *       
 *     |-----------|
 *     | MIN_C_L_S |  -----> NULL
 *     -------------
 *
 */


//-----------------------------------------------------------------------------

#define TAG_OVERHEAD (sizeof (half_t) + sizeof(half_t) + sizeof (half_t) + sizeof (half_t))

#define MAGIC_NUMBER 0xA55A

//-----------------------------------------------------------------------------

struct chunk_header {
  /* magic number is an ID */
  half_t magic_number;
  /* state block indicates if chunk is being used, when the chunk is
   used, state_block have the size which has been used */
  half_t state_block;
  /* Max size is the size of chunk in log2 (size) */
  half_t log2size;
  /* This field allow us to know if chunk is the odd or the even chunk */
  half_t div_code;
  /* These pointers are used to aim free chunks with the same size */
  struct chunk_header *prev;
  struct chunk_header *next;
};

typedef struct chunk_header chunk_header_t;

/* This is our first chunk */
static chunk_header_t *first_chunk;

/* This array contains all the free chunks */
static chunk_header_t **free_chunks_array;

static int all_ok = 0;

/******************************************************************************
 * STATIC ROUTINES DECLARATION
 *****************************************************************************/

/**
 *  These functions return log2 max and log2 min chunk size
 */
static int max_chunk_log2_size(void);

/**
 * \todo Documentar este método
 */
static int min_chunk_log2_size(void);

/**
 *  These functions return max and min chunk size
 */
static int max_chunk_size(void); 

/**
 * \todo Documentar este método
 */
static int min_chunk_size(void);

/**
 * \todo Documentar este método
 */
static int log2size(size_t len);

/**
 * \todo Documentar este método
 */
static int split (chunk_header_t *chunk);

/**
 * \todo Documentar este método
 */
static chunk_header_t *merge (chunk_header_t *chunk);

/**
 * \todo Documentar este método
 */
static void print_chunk(chunk_header_t *chunk);

//-----------------------------------------------------------------------------

/******************************************************************************
 * PUBLIC INTERFACE
 *****************************************************************************/

/* 
 * This function initialite all buffers needed  
 * by the buddy algorithm 
 */

int OS_MMInitMemoryPool(int max_chunk_log2_size,
              int min_chunk_log2_size,
           char *buffer){

  chunk_header_t *chunk;
  void *addr;
  int i;
 
  MAX_CHUNK_LOG2_SIZE = max_chunk_log2_size;
  KCONFIG_MIN_CHUNK_LOG2_SIZE = min_chunk_log2_size;
  MAX_CHUNK_SIZE = (1 << MAX_CHUNK_LOG2_SIZE);
  MIN_CHUNK_SIZE = (1 << KCONFIG_MIN_CHUNK_LOG2_SIZE);
 
  if (min_chunk_log2_size < 4 || max_chunk_log2_size < min_chunk_log2_size){
    PRINT_MSG ("ERROR: Memory pool can't be initialited\n");
    return -1;
  }

  INIT_THREAD_MUTEX();
  
  free_chunks_array = (chunk_header_t **) buffer;
  
  addr = (void *)((unsigned long) buffer + 
       (unsigned long)(MAX_CHUNK_LOG2_SIZE + 1) * 
        (unsigned long) sizeof (chunk_header_t *));
  
  first_chunk = (chunk_header_t *) addr;
  chunk = (chunk_header_t *) addr;
  
  chunk -> magic_number = MAGIC_NUMBER;
  chunk -> state_block = BLOCK_FREE;
  chunk -> log2size = log2size(MAX_CHUNK_SIZE -((MAX_CHUNK_LOG2_SIZE + 1) * 
                      sizeof(chunk_header_t *)));
  chunk -> div_code = 0;
  chunk -> next = NULL;
  chunk -> prev = NULL;
  
  for (i = 0; i < MAX_CHUNK_LOG2_SIZE; i++)
    free_chunks_array [i] = NULL;

  free_chunks_array [chunk -> log2size] = chunk;

  all_ok = 1;
  return 0;
}


/* is necessary an explication of this function ??? */
void OS_MMDestroyMemoryPool(void){
}


/* see man malloc */
void *OS_Malloc (size_t size){
  int i_log2size, i;
  chunk_header_t *chunk;  
  byte_t *aux;

  if (size == 0 || !all_ok)
    return NULL;
  
  i_log2size = log2size (size + TAG_OVERHEAD);
  /* 8 bytes are the overhead for buddy structures */
  
  if (i_log2size == -1) return NULL;

  THREAD_LOCK();

  /* find the smallest i_log2size that has a free chunk */
  for (i = i_log2size; i <= MAX_CHUNK_LOG2_SIZE; i++)
    if ((chunk = free_chunks_array [i]) != NULL) break;
  
  if (i > MAX_CHUNK_LOG2_SIZE) {
    PRINT_MSG_1 ("ERROR: There isn't any chunk of %d bytes\n", size);
    THREAD_UNLOCK();
    return NULL;
  }
  
  /* split larger i_log2size until we reach the desired size */
  while (i > i_log2size) {
    /* a free i_log2size indicates that the previous split failed */
    if ((chunk = free_chunks_array [i]) == NULL) {
      PRINT_MSG ("ERROR: Memory pool exhausted\n");
      THREAD_UNLOCK();
      return NULL;
    }
    split(chunk);
    i--;
  }
  
  if ((chunk = free_chunks_array[i_log2size]) == NULL){
    THREAD_UNLOCK();
    return NULL;
  }


  if (chunk -> magic_number != MAGIC_NUMBER 
      || chunk -> state_block != BLOCK_FREE){
    PRINT_MSG ("ERROR: rt_malloc function has an error !!!\n");
    THREAD_UNLOCK();
    return NULL;
  }
  
  free_chunks_array[i_log2size] = chunk -> next;
  
  if (chunk -> next != NULL) 
    chunk -> next -> prev = NULL;
  
  /* 
   * we store in state_block the difference between 
   * size given and size demanded
   */
  
  chunk -> state_block = (1 << chunk -> log2size) - (size + TAG_OVERHEAD); 

  if (chunk -> state_block == 0) chunk -> state_block = 0xFFFF;
  
  aux = (byte_t *) chunk;
  THREAD_UNLOCK();
  
  #ifdef __DEBUG__
  PRINT_MSG_1("MM_DEBUG: Malloc returns 0x%x\n",aux + TAG_OVERHEAD);
  #endif

  return (void *)(aux + TAG_OVERHEAD);
}

/* 
 *  see man free
 *
 *  rt_free() is only guaranteed to work if ptr is the address of a block
 *  allocated by rt_malloc() (and not yet freed).
 */

void OS_Free (void *ptr){
  byte_t *aux;
  chunk_header_t *chunk, *new_chunk;  
  
  if (!all_ok || ptr == NULL)
    return;

  THREAD_LOCK();

  aux = (byte_t *) ptr;
  aux = aux - TAG_OVERHEAD;

  chunk = (chunk_header_t *) aux;
  
  if(chunk -> magic_number != MAGIC_NUMBER) {
    PRINT_MSG ("CRITICAL ERROR: chunk corrupted\n");
    THREAD_UNLOCK();
    return;
  }

  chunk -> state_block = BLOCK_FREE;
 
  while ((new_chunk = merge(chunk)) != NULL) 
    if (new_chunk != NULL) chunk = new_chunk;
     
  /* new chunk is inserted in chunk */
  if (free_chunks_array [chunk -> log2size] != NULL)
    free_chunks_array [chunk -> log2size] -> prev = chunk;
    
  chunk -> next = free_chunks_array [chunk -> log2size];         
  free_chunks_array [chunk -> log2size] = chunk;

  THREAD_UNLOCK();

  #ifdef __DEBUG__
  PRINT_MSG_1("MM_DEBUG: FREE - 0x%x has been freed\n",(uint32)ptr);
  #endif
}

/******************************************************************************
 * STATIC ROUTINES
 *****************************************************************************/
static int log2size(size_t len)
{
        int i;
 
   if (len <= 0) {
      PRINT_MSG ("ERROR: Chunk length must be > 0\n");
   return -1;
   }
        
        if (len <= MIN_CHUNK_SIZE)
    return KCONFIG_MIN_CHUNK_LOG2_SIZE;

        for (i = KCONFIG_MIN_CHUNK_LOG2_SIZE; 
       (i <= MAX_CHUNK_LOG2_SIZE) && (len > (1 << i)) ; i++)
   ;
    
   if (i > MAX_CHUNK_LOG2_SIZE) {
   PRINT_MSG ("ERROR: Maximum chunk size exceeded\n");
    return -1;
   }
  
        return i;
}

/*
 * Split a free memory chunk into two halves
 */

static int split (chunk_header_t *chunk){
  chunk_header_t *half;
  byte_t *aux;
  
  if( chunk -> magic_number != MAGIC_NUMBER) {
    PRINT_MSG ("CRITICAL ERROR: chunk corrupted\n");
    return -1;
  }

  if (chunk -> state_block != BLOCK_FREE) {
    PRINT_MSG ("ERROR: Used chunk can't be splitted\n");
    return -1;
  }

  if (chunk -> log2size == KCONFIG_MIN_CHUNK_LOG2_SIZE){
    PRINT_MSG ("ERROR: This chunk has minim size and it can't be splitted\n");
    return -1;
  }

  if (chunk -> prev != NULL)
    chunk -> prev -> next = chunk -> next;

  if (chunk -> next != NULL)
    chunk -> next -> prev = chunk -> prev;
  
  if (free_chunks_array [chunk -> log2size] == chunk)
    free_chunks_array [chunk -> log2size] = chunk -> next;
          
  
  chunk -> log2size --;
  chunk -> div_code ++;
  
  aux = (byte_t *) chunk;
  aux += (1 << chunk -> log2size);

  half = (chunk_header_t *) aux;   

  half -> magic_number = MAGIC_NUMBER;
  half -> state_block = BLOCK_FREE;
  half -> log2size = chunk -> log2size;
  half -> div_code = 0;
  
  chunk -> next = half;
  half -> prev = chunk;
  
  chunk -> prev = NULL; 
  half -> next =  free_chunks_array [chunk -> log2size]; 
 
  free_chunks_array [chunk -> log2size] = chunk;

  return 0;
}


/*
 * Merge a free memory chunk with its buddy
 */

static chunk_header_t *merge (chunk_header_t *chunk){
  chunk_header_t *half, *new_chunk;
  byte_t *aux;

  if( chunk -> magic_number != MAGIC_NUMBER) {
    PRINT_MSG ("CRITICAL ERROR: chunk corrupted\n");
    return NULL;
  }

  /* merge an used chunk ?????, this has to be an error */
  if (chunk -> state_block != BLOCK_FREE) {
    PRINT_MSG ("ERROR: Used chunk can't be merged\n");
    return NULL;
  }

  /* it can be only if we have reserved several chunks of the same size */
  if (chunk -> log2size == MAX_CHUNK_LOG2_SIZE)
    return NULL;

  chunk -> prev = NULL;
  chunk -> next = NULL;

  aux = (byte_t *) chunk;

  /* we will search the buddy of the chunk */
  if (chunk -> div_code == 0) { // even
    aux -= (1 << chunk -> log2size);
    half = (chunk_header_t *) aux;
    new_chunk = half;
  } else { // odd
    aux += (1 << chunk -> log2size);
    half = (chunk_header_t *) aux;
    new_chunk = chunk;
  }

  if( half -> magic_number != MAGIC_NUMBER) {
    PRINT_MSG ("CRITICAL ERROR: chunk corrupted\n");
    return NULL;
  }

  /* can we merge the chunks?, it could be */
  if (half -> state_block == BLOCK_FREE && 
      half -> log2size == chunk -> log2size) {
    // The chunks will be merged
    
    if (free_chunks_array [half -> log2size] == half) 
      free_chunks_array [half -> log2size] = half -> next;
   
    if (half -> prev != NULL)
      half -> prev -> next = half -> next;
     
    if (half -> next != NULL)
      half -> next -> prev = half -> prev;
    
    new_chunk -> state_block = BLOCK_FREE;
    new_chunk -> log2size ++;
    
    new_chunk -> div_code --;
    new_chunk -> prev = NULL;
    new_chunk -> next = NULL;
    
    return new_chunk;
  }  
  /* Merge operation hasn't been able done */
  return NULL;
}
 
static int max_chunk_log2_size(void){
  return MAX_CHUNK_LOG2_SIZE;
}

static int min_chunk_log2_size(void){
  return KCONFIG_MIN_CHUNK_LOG2_SIZE;
}

static int max_chunk_size(void){
  return MAX_CHUNK_SIZE;
} 

static int min_chunk_size(void){
  return MIN_CHUNK_SIZE;
}


/******************************************************************************
 * DEBUGGING ROUTINES
 *****************************************************************************/
 
#ifdef __DEBUG__

/* 
 * This function gives information about 
 * algorithm structures
 */

void statistics_data (void){
  int n = 0;
  byte_t *aux;
  chunk_header_t *chunk;

  if (!all_ok) return;

  aux = (byte_t *) first_chunk;
 
  while (n < MAX_CHUNK_SIZE){
    
    chunk =  (chunk_header_t *) aux;
    
    n += (1 << (chunk -> log2size));
    print_chunk(chunk);

    aux += (1 << chunk -> log2size);
  }
}

/* memory_dump does a dumped of the memory context */

void memory_dump (void){
  int i;
  byte_t *tmp;

  if (!all_ok) return;
  
  tmp = (byte_t *) first_chunk;

  PRINT_DBG_C ("\nMemory region dumped: 0x");
  PRINT_DBG_H (first_chunk);
  PRINT_DBG_C (" - 0x");
  PRINT_DBG_H (tmp + MAX_CHUNK_SIZE);
  PRINT_DBG_C ("\n\n");

  for (i = 0; i < MAX_CHUNK_SIZE; i = i + sizeof(byte_t)){
    PRINT_DBG_H (tmp[i]);
    PRINT_DBG_C (" ");
  }  
  PRINT_DBG_C ("\n");
}

/* 
 * free_blocks_context does a dump
 * of free_chunks_array and its context 
 */

void free_blocks_context(void){
  int i;
  chunk_header_t *chunk;

  if (!all_ok) return;

  for (i = MAX_CHUNK_LOG2_SIZE; i >= KCONFIG_MIN_CHUNK_LOG2_SIZE; i--){
    PRINT_DBG_C ("[");
    PRINT_DBG_D(i);
    PRINT_DBG_C ("] -> ");
 
    if (free_chunks_array[i]!=NULL){
      chunk = free_chunks_array [i];
      while (chunk != NULL) {
  PRINT_DBG_C ("MNum: 0x");
  PRINT_DBG_H (chunk -> magic_number);
   PRINT_DBG_C (", Address: 0x");
 PRINT_DBG_H (chunk);
   PRINT_DBG_C (", State: ");
 PRINT_DBG_D (chunk -> state_block);
    PRINT_DBG_C (", Log2size: ");
  PRINT_DBG_D (chunk -> log2size);
   PRINT_DBG_C (", Div_code: ");
  PRINT_DBG_D (chunk -> div_code);
   PRINT_DBG_C ("\n\tPrev: 0x");
  PRINT_DBG_H (chunk -> prev);
   PRINT_DBG_C (", Next: 0x");
    PRINT_DBG_H (chunk -> next);
   PRINT_DBG_C ("\n");
    chunk = chunk -> next;
      }
    } else PRINT_MSG ("NULL\n");
  }
}

void global_state (int *free, int *used, int *overhead){
  chunk_header_t *b;
  byte_t *aux;
  int n = 0;
  *free = 0;
  *used = 0;
  *overhead = 0;
  if (!all_ok) return;
  aux = (byte_t *)first_chunk;
  while (n < MAX_CHUNK_SIZE){
    b = (chunk_header_t *) aux;
    n += (1<< b -> log2size); 
    if (b -> state_block != 0) {
      *used += (1 << b -> log2size);
    } else {
      *free += (1 << b -> log2size);
    }
    aux += (1 << b -> log2size);
    *overhead += TAG_OVERHEAD;
  }

}

/* 
 * This function checks memory searching 
 * errors and incoherences 
 * return :
 * 0 if there aren't errors 
 * or 
 * -1 in other case
 */

int check_mem (void){
  int n = 0, frag;
  byte_t *aux;
  chunk_header_t *chunk, *chunk_r;
  
  if (!all_ok) return -1;

  aux = (byte_t *) first_chunk;

  if(first_chunk -> magic_number != MAGIC_NUMBER) {
    return -1;
  }

  while (n < MAX_CHUNK_SIZE){
    
    chunk =  (chunk_header_t *) aux;

    n += (1<<(chunk -> log2size)); 
    
    if (chunk < first_chunk ||
 chunk > first_chunk + MAX_CHUNK_SIZE ||
    chunk -> magic_number != MAGIC_NUMBER || 
  chunk -> log2size > MAX_CHUNK_LOG2_SIZE || 
    chunk -> log2size < KCONFIG_MIN_CHUNK_LOG2_SIZE || 
    chunk -> div_code > MAX_CHUNK_LOG2_SIZE) return -1;
    
     if (chunk -> state_block == BLOCK_FREE){

      chunk_r =  free_chunks_array [chunk -> log2size];
      while (chunk_r != NULL && chunk_r != chunk) chunk_r = chunk_r -> next;   
      /* ERROR, the chunk_r 
       * must be the same than chunk
       */
      if (chunk_r == NULL) return -1;
  
      if (chunk_r -> next != NULL)
        if (chunk_r -> next < first_chunk ||
       chunk_r -> next > first_chunk + MAX_CHUNK_SIZE ||
      chunk_r -> next -> magic_number != MAGIC_NUMBER ||
     chunk_r -> next -> state_block != BLOCK_FREE)
    return -1;
   
      if (chunk_r -> prev != NULL)
    if (chunk_r -> prev < first_chunk ||
       chunk_r -> prev > first_chunk + MAX_CHUNK_SIZE ||
      chunk_r -> prev -> magic_number != MAGIC_NUMBER ||
     chunk_r -> prev -> state_block != BLOCK_FREE)
    return -1;
     } else {
       frag = (chunk -> state_block == 0xFFFF)? 0 : chunk -> state_block;
       if (frag > (1 << chunk -> log2size))
    return -1;
     }
     aux += (1 << chunk -> log2size);
  } 
  return 0;
}

static void print_chunk(chunk_header_t *chunk){

  PRINT_DBG_C ("MAGIC NUMBER 0x");
  PRINT_DBG_H (chunk -> magic_number);
  PRINT_DBG_C ("\nBLOCK 0x");
  PRINT_DBG_H (chunk);
  if (chunk -> state_block == BLOCK_FREE) {
    PRINT_DBG_C ("\nFREE");
  } else {
    PRINT_DBG_C ("\nUSED\nFRAGMENTATION ");
    if (chunk -> state_block == 0xFFFF){
      PRINT_DBG_D (chunk -> state_block);
    } else {
      PRINT_DBG_D (chunk -> state_block);
    }
  }
  PRINT_DBG_C ("\nLOG2 SIZE ");
  PRINT_DBG_D (chunk -> log2size);
  PRINT_DBG_C ("\nSIZE ");
  PRINT_DBG_D ((1 << chunk -> log2size));
  PRINT_DBG_C ("\nDIV CODE ");
  PRINT_DBG_D (chunk -> div_code);
  PRINT_DBG_C ("\n");
  if (chunk -> state_block == BLOCK_FREE) {
    PRINT_DBG_C ("PREV BLOCK 0x");
    PRINT_DBG_H (chunk -> prev);
    PRINT_DBG_C ("\nNEXT BLOCK 0x");
    PRINT_DBG_H (chunk -> next);
    PRINT_DBG_C ("\n");
  }
}

#endif // #ifdef __DEBUG__

