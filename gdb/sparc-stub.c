/****************************************************************************

                THIS SOFTWARE IS NOT COPYRIGHTED

   HP offers the following for use in the public domain.  HP makes no
   warranty with regard to the software or it's performance and the
   user accepts the software "AS IS" with all faults.

   HP DISCLAIMS ANY WARRANTIES, EXPRESS OR IMPLIED, WITH REGARD
   TO THIS SOFTWARE INCLUDING BUT NOT LIMITED TO THE WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

****************************************************************************/

/****************************************************************************
 *  Header: remcom.c,v 1.34 91/03/09 12:29:49 glenne Exp $
 *
 *  Module name: remcom.c $
 *  Revision: 1.34 $
 *  Date: 91/03/09 12:29:49 $
 *  Contributor:     Lake Stevens Instrument Division$
 *
 *  Description:     low level support for gdb debugger. $
 *
 *  Considerations:  only works on target hardware $
 *
 *  Written by:      Glenn Engel $
 *  ModuleState:     Experimental $
 *
 *  NOTES:           See Below $
 *
 *  Modified for SPARC by Stu Grossman, Cygnus Support.
 *
 *  This code has been extensively tested on the Fujitsu SPARClite demo board.
 *
 *  To enable debugger support, two things need to happen.  One, a
 *  call to set_debug_traps() is necessary in order to allow any breakpoints
 *  or error conditions to be properly intercepted and reported to gdb.
 *  Two, a breakpoint needs to be generated to begin communication.  This
 *  is most easily accomplished by a call to breakpoint().  Breakpoint()
 *  simulates a breakpoint by executing a trap #1.
 *
 *************
 *
 *    The following gdb commands are supported:
 *
 * command          function                               Return value
 *
 *    g             return the value of the CPU registers  hex data or ENN
 *    G             set the value of the CPU registers     OK or ENN
 *
 *    mAA..AA,LLLL  Read LLLL bytes at address AA..AA      hex data or ENN
 *    MAA..AA,LLLL: Write LLLL bytes at address AA.AA      OK or ENN
 *
 *    c             Resume at current address              SNN   ( signal NN)
 *    cAA..AA       Continue at address AA..AA             SNN
 *
 *    s             Step one instruction                   SNN
 *    sAA..AA       Step one instruction from AA..AA       SNN
 *
 *    k             kill
 *
 *    ?             What was the last sigval ?             SNN   (signal NN)
 *
 * All commands and responses are sent with a packet which includes a
 * checksum.  A packet consists of
 *
 * $<packet info>#<checksum>.
 *
 * where
 * <packet info> :: <characters representing the command or response>
 * <checksum>    :: < two hex digits computed as modulo 256 sum of <packetinfo>>
 *
 * When a packet is received, it is first acknowledged with either '+' or '-'.
 * '+' indicates a successful transfer.  '-' indicates a failed transfer.
 *
 * Example:
 *
 * Host:                  Reply:
 * $m0,10#2a               +$00010203040506070809101112131415#42
 *
 ****************************************************************************/
/*
    Copyright 2005-2006 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/


#include <signal.h>
#include <arch/erc32-stub.h>
#include <arch/mec.h>


/************************************************************************/
/* BUFMAX defines the maximum number of characters in inbound/outbound buffers*/
/* at least NUMREGBYTES*2 are needed for register packets */
#define BUFMAX 2048

static int initialized = 0;     /* !0 means we've been initialized */

static void set_mem_fault_trap(int enable);

static const char hexchars[]="0123456789abcdef";

const char version[] =
        "ERC32mon v0.1 - bug reports to Aitor Viana Sanchez (avs@aut.uah.es)";


#ifdef CONFIG_DEBUG
static const char *trap_type[] = {"Reset trap",
                                "Instruction access", "Ilegal instruction",
                                "Priviledge instruction","Floating point disable",
                                "Window overflow","Window underflow",
                                "Memory access not aligment","Flotating point exception",
                                "Data access exception","Tag overflow",
                                "Not handled exception","Not handled exception",
                                "Not handled exception","Not handled exception",
                                "Not handled exception","Not handled exception",
                                "Ext INT0","Ext INT1",
                                "UART A data ready or trasnmitter ready",
                                "UART B data ready or trasnmitter ready",
                                "Correctable error in memory","UART error",
                                "DMA access error","DMA time-out",
                                "EXT INT2","EXT INT3",
                                "General purpose timer","Real time clock",
                                "EXT INT4 (ethernet)","Watch dog time out",
};
#endif

#define NUMREGS 72

/* Number of bytes of registers.  */
#define NUMREGBYTES (NUMREGS * 4)
enum regnames { G0, G1, G2, G3, G4, G5, G6, G7,
                O0, O1, O2, O3, O4, O5, SP, O7,
                L0, L1, L2, L3, L4, L5, L6, L7,
                I0, I1, I2, I3, I4, I5, FP, I7,

                F0, F1, F2, F3, F4, F5, F6, F7,
                F8, F9, F10, F11, F12, F13, F14, F15,
                F16, F17, F18, F19, F20, F21, F22, F23,
                F24, F25, F26, F27, F28, F29, F30, F31,
                Y, PSR, WIM, TBR, PC, NPC, FPSR, CPSR };

/***************************  ASSEMBLY CODE MACROS *************************/
/*                                                                         */

extern void trap_low(void);
extern void fltr_set_mem_err(void);
extern void breakinst(void);

/* Convert ch from a hex digit to an int */

static int hex (unsigned char ch)
{
  if (ch >= 'a' && ch <= 'f')
    return ch-'a'+10;
  if (ch >= '0' && ch <= '9')
    return ch-'0';
  if (ch >= 'A' && ch <= 'F')
    return ch-'A'+10;
  return -1;
}

static char remcomInBuffer[BUFMAX];
static char remcomOutBuffer[BUFMAX];

/* This table contains the mapping between SPARC hardware trap types, and
   signals, which are primarily what GDB understands.  It also indicates
   which hardware traps we need to commandeer when initializing the stub. */

static struct hard_trap_info
{
  unsigned char tt;             /* Trap type code for SPARClite */
  unsigned char signo;          /* Signal that we map this trap into */
} hard_trap_info[] = {
  {1, SIGSEGV},                 /* instruction access error */
  {2, SIGILL},                  /* privileged instruction */
  {3, SIGILL},                  /* illegal instruction */
//  {4, SIGEMT},                  /* fp disabled */
  {36, SIGEMT},                 /* cp disabled */
  {7, SIGBUS},                  /* mem address not aligned */
  {9, SIGSEGV},                 /* data access exception */
  {10, SIGEMT},                 /* tag overflow */
  {128+1, SIGTRAP},             /* ta 1 - normal breakpoint instruction */
  {0, 0}                        /* Must be last */
};


//------------------------------------------------------------------------------

static void breakpoint (void);
static int computeSignal (int tt);
static unsigned char *getpacket (void);
static int hex (unsigned char ch);
static char * 
hex2mem (unsigned char *buf, unsigned char *mem, int count, int may_fault);
static int hexToInt(char **ptr, int *intValue);
static unsigned char *
mem2hex (unsigned char *mem, unsigned char *buf, int count, int may_fault);
static void putpacket (unsigned char *buffer);
static void set_mem_fault_trap (int enable);

//------------------------------------------------------------------------------


/******************************************************************************
 * PUBLIC INTERFACE
 *****************************************************************************/


/* Set up exception handlers for tracing and breakpoints */

void set_debug_traps (void)
{
    struct hard_trap_info *ht;

    for (ht = hard_trap_info; (ht->tt != 0) && (ht->signo != 0); ht++) {
        #ifdef CONFIG_DEBUG
        if (ht->tt < 32) {
            printf("Installing handler for %s\n", trap_type[ht->tt]);
        }
        #endif
        exceptionHandler(ht->tt, (unsigned int*)trap_low, rdtbr());
    }

    initialized = 1;
    
    ERC32_MEC.interrupt_mask |= (1 << 14); //mask the UARTA interrupt
}

void gdb_monitor(void)
{
    printf ("%s\n", version);

    /* Set up exception handlers for tracing and breakpoints */
    set_debug_traps();

    printf("Waiting for GDB connection...\n");

    /*  Breakpoint, control to GDB  */
    breakpoint();
}

void handle_exception (unsigned long *registers)
{
  int tt;                       /* Trap type */
  int sigval;
  int addr;
  int length;
  char *ptr;
  unsigned long *sp;
 
  

/* First, we must force all of the windows to be spilled out
 * NOTE: Only meaningful in "all windows clean" mode */

#ifdef CONFIG_FAST_MODE
  printf("Fast mode handleing\n");
  
  asm(" 
        save %sp, -64, %sp
        save %sp, -64, %sp
        save %sp, -64, %sp
        save %sp, -64, %sp
        save %sp, -64, %sp
        save %sp, -64, %sp
        save %sp, -64, %sp
        save %sp, -64, %sp
        restore
        restore
        restore
        restore
        restore
        restore
        restore
        restore
");

#endif

  if (registers[PC] == (unsigned long)breakinst)
    {
      registers[PC] = registers[NPC];
      registers[NPC] += 4;
    }

  sp = (unsigned long *)registers[SP];

  tt = (registers[TBR] >> 4) & 0xff;

  /* reply to host that an exception has occurred */
  sigval = computeSignal(tt);
  ptr = remcomOutBuffer;

  *ptr++ = 'T';
  *ptr++ = hexchars[sigval >> 4];
  *ptr++ = hexchars[sigval & 0xf];

  *ptr++ = hexchars[PC >> 4];
  *ptr++ = hexchars[PC & 0xf];
  *ptr++ = ':';
  ptr = mem2hex((unsigned char *)&registers[PC], ptr, 4, 0);
  *ptr++ = ';';

  *ptr++ = hexchars[FP >> 4];
  *ptr++ = hexchars[FP & 0xf];
  *ptr++ = ':';
  ptr = mem2hex((unsigned char *)(sp + 8 + 6), ptr, 4, 0); /* FP */
  *ptr++ = ';';

  *ptr++ = hexchars[SP >> 4];
  *ptr++ = hexchars[SP & 0xf];
  *ptr++ = ':';
  ptr = mem2hex((unsigned char *)&sp, ptr, 4, 0);
  *ptr++ = ';';

  *ptr++ = hexchars[NPC >> 4];
  *ptr++ = hexchars[NPC & 0xf];
  *ptr++ = ':';
  ptr = mem2hex((unsigned char *)&registers[NPC], ptr, 4, 0);
  *ptr++ = ';';

  *ptr++ = hexchars[O7 >> 4];
  *ptr++ = hexchars[O7 & 0xf];
  *ptr++ = ':';
  ptr = mem2hex((unsigned char *)&registers[O7], ptr, 4, 0);
  *ptr++ = ';';

  *ptr++ = 0;

  putpacket(remcomOutBuffer);

  while (1)
    {
        remcomOutBuffer[0] = 0;

        ptr = getpacket();
        switch (*ptr++)
        {
        case '?':
            remcomOutBuffer[0] = 'S';
            remcomOutBuffer[1] = hexchars[sigval >> 4];
            remcomOutBuffer[2] = hexchars[sigval & 0xf];
            remcomOutBuffer[3] = 0;
            break;

        case 'd':                /* toggle debug flag */
            break;

        case 'g':                /* return the value of the CPU registers */
        {
            ptr = remcomOutBuffer;
            ptr = mem2hex((unsigned char *)registers, ptr, 16 * 4, 0); /* G & O regs */
            ptr = mem2hex((unsigned char *)(sp + 0), ptr, 16 * 4, 0); /* L & I regs */
            memset(ptr, '0', 32 * 8); /* Floating point */
            mem2hex((char *)&registers[Y],
                    ptr + 32 * 4 * 2,
                    8 * 4,
                    0);                /* Y, PSR, WIM, TBR, PC, NPC, FPSR, CPSR */
        }
        break;

        case 'G':           /* set the value of the CPU registers - return OK */
        {
            unsigned long *newsp, psr;

            psr = registers[PSR];

            hex2mem(ptr, (char *)registers, 16 * 4, 0); /* G & O regs */
            hex2mem(ptr + 16 * 4 * 2, (unsigned char *)(sp + 0), 16 * 4, 0); /* L & I regs */
            hex2mem(ptr + 64 * 4 * 2, (unsigned char *)&registers[Y],
                    8 * 4, 0);        /* Y, PSR, WIM, TBR, PC, NPC, FPSR, CPSR */

            /* See if the stack pointer has moved.  If so, then copy the saved
               locals and ins to the new location.  This keeps the window
               overflow and underflow routines happy.  */

            newsp = (unsigned long *)registers[SP];
            if (sp != newsp)
                sp = (unsigned long*)memcpy(newsp, sp, 16 * 4);

            /* Don't allow CWP to be modified. */

            if (psr != registers[PSR])
                registers[PSR] = (psr & 0x1f) | (registers[PSR] & ~0x1f);

            strcpy(remcomOutBuffer,"OK");
        }
        break;

        case 'm':          /* mAA..AA,LLLL  Read LLLL bytes at address AA..AA */
          /* Try to read %x,%x.  */

            if (hexToInt(&ptr, &addr)
                && *ptr++ == ','
                && hexToInt(&ptr, &length))
            {
                if (mem2hex((char *)addr, remcomOutBuffer, length, 1))
                    break;

                strcpy (remcomOutBuffer, "E03");
            }
            else
                strcpy(remcomOutBuffer,"E01");
            break;

        case 'M': /* MAA..AA,LLLL: Write LLLL bytes at address AA.AA return OK */
          /* Try to read '%x,%x:'.  */

            if (hexToInt(&ptr, &addr)
                && *ptr++ == ','
                && hexToInt(&ptr, &length)
                && *ptr++ == ':')
            {
                if (hex2mem(ptr, (char *)addr, length, 1))
                    strcpy(remcomOutBuffer, "OK");
                else
                    strcpy(remcomOutBuffer, "E03");
            }
            else
                strcpy(remcomOutBuffer, "E02");
            break;

        case 'c':    /* cAA..AA    Continue at address AA..AA(optional) */
          /* try to read optional parameter, pc unchanged if no parm */

            if (hexToInt(&ptr, &addr))
            {
                registers[PC] = addr;
                registers[NPC] = addr + 4;
            }
//      printf("Le has dado a continuar\n");

/* Need to flush the instruction cache here, as we may have deposited a
   breakpoint, and the icache probably has no way of knowing that a data ref to
   some location may have changed something that is in the instruction cache.
 */

          flush_i_cache();
          return;

          /* kill the program */
        case 'k' :
            break; /* do nothing */
#if 0
        case 't':                /* Test feature */
            asm (" std %f30,[%sp]");
            break;
#endif
        case 'r':                /* Reset */
            asm ("call 0
                  nop ");
            break;
        } /* switch */

      /* reply to the request */
      putpacket(remcomOutBuffer);
      
    }   /* while(1) */
}

/******************************************************************************
 * PRIVATE INTERFACE
 *****************************************************************************/

/* scan for the sequence $<data>#<checksum>     */

static unsigned char *getpacket (void)
{
  unsigned char *buffer = &remcomInBuffer[0];
  unsigned char checksum;
  unsigned char xmitcsum;
  int count;
  char ch;

    while (1)
    {
      /* wait around for the start character, ignore all other characters */
        while ((ch = getDebugChar ()) != '$');

retry:
      checksum = 0;
      xmitcsum = -1;
      count = 0;

      /* now, read until a # or end of buffer is found */
      while (count < BUFMAX)
        {
          ch = getDebugChar ();
          if (ch == '$')
            goto retry;
          if (ch == '#')
            break;
          checksum = checksum + ch;
          buffer[count] = ch;
          count = count + 1;
        }
      buffer[count] = 0;

      if (ch == '#')
        {
          ch = getDebugChar ();
          xmitcsum = hex (ch) << 4;
          ch = getDebugChar ();
          xmitcsum += hex (ch);

          if (checksum != xmitcsum)
            {
              putDebugChar ('-');        /* failed checksum */
            }
          else
            {
              putDebugChar ('+');        /* successful transfer */

              /* if a sequence char is present, reply the sequence ID */
              if (buffer[2] == ':')
                {
                  putDebugChar (buffer[0]);
                  putDebugChar (buffer[1]);

                  return &buffer[3];
                }

              return &buffer[0];
            }
        }
    }
}

/* send the packet in buffer.  */

static void putpacket (unsigned char *buffer)
{
  unsigned char checksum;
  int count;
  unsigned char ch;

  /*  $<packet info>#<checksum>. */
  do
    {
      putDebugChar('$');
      checksum = 0;
      count = 0;

      while (ch = buffer[count])
        {
          putDebugChar(ch);
          checksum += ch;
          count += 1;
        }

      putDebugChar('#');
      putDebugChar(hexchars[checksum >> 4]);
      putDebugChar(hexchars[checksum & 0xf]);

    }
  while (getDebugChar() != '+');
}

/* Indicate to caller of mem2hex or hex2mem that there has been an
   error.  */
volatile int mem_err = 0;

/* Convert the memory pointed to by mem into hex, placing result in buf.
 * Return a pointer to the last char put in buf (null), in case of mem fault,
 * return 0.
 * If MAY_FAULT is non-zero, then we will handle memory faults by returning
 * a 0, else treat a fault like any other fault in the stub.
 */

static unsigned char *
mem2hex (unsigned char *mem, unsigned char *buf, int count, int may_fault)
{
  unsigned char ch;

  set_mem_fault_trap(may_fault);

  while (count-- > 0)
    {
      ch = *mem++;
      if (mem_err)
        return 0;
      *buf++ = hexchars[ch >> 4];
      *buf++ = hexchars[ch & 0xf];
    }

  *buf = 0;

  set_mem_fault_trap(0);

  return buf;
}

/* convert the hex array pointed to by buf into binary to be placed in mem
 * return a pointer to the character AFTER the last byte written */

static char * 
hex2mem (unsigned char *buf, unsigned char *mem, int count, int may_fault)
{
  int i;
  unsigned char ch;

  set_mem_fault_trap(may_fault);

  for (i=0; i<count; i++)
    {
      ch = hex(*buf++) << 4;
      ch |= hex(*buf++);
      *mem++ = ch;
      if (mem_err)
        return 0;
    }

  set_mem_fault_trap(0);

  return mem;
}

static void set_mem_fault_trap (int enable)
{
//  extern void fltr_set_mem_err();
  mem_err = 0;

  if (enable)
    exceptionHandler(9, (unsigned int*)fltr_set_mem_err, rdtbr());
  else
    exceptionHandler(9, (unsigned int*)trap_low, rdtbr());
}

/* Convert the SPARC hardware trap type code to a unix signal number. */

static int computeSignal (int tt)
{
  struct hard_trap_info *ht;

  for (ht = hard_trap_info; ht->tt && ht->signo; ht++)
    if (ht->tt == tt)
      return ht->signo;

  return SIGHUP;                /* default for things we don't know about */
}

/*
 * While we find nice hex chars, build an int.
 * Return number of chars processed.
 */

static int hexToInt(char **ptr, int *intValue)
{
  int numChars = 0;
  int hexValue;

  *intValue = 0;

  while (**ptr)
    {
      hexValue = hex(**ptr);
      if (hexValue < 0)
        break;

      *intValue = (*intValue << 4) | hexValue;
      numChars ++;

      (*ptr)++;
    }

  return (numChars);
}

/* This function will generate a breakpoint exception.  It is used at the
   beginning of a program to sync up with a debugger and can be used
   otherwise as a quick means to stop program execution and "break" into
   the debugger. */

static void breakpoint (void)
{
  if (!initialized)
    return;

  asm(" .globl breakinst

        breakinst: ta 1
      ");
}
