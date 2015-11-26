/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/
 
/*
 * File:    $Source$
 * 
 * $Id: asm.h 1025 2006-06-19 14:19:24 +0000 (lun, 19 jun 2006) aitor $
 *  
 * $Revision: 1025 $
 * $Date: 2006-06-19 14:19:24 +0000 (lun, 19 jun 2006) $
 * Last CVS commit by $Author: aitor $
 * 
 * Registro de cambios
 * 
 * $Log$
 * Revision 1.2  2005/09/22 11:30:22  aitor
 * CVS Problems
 *
 * Revision 1.3  2005/07/29 10:20:28  aitor
 * Added all the deleted head.h header file.
 *
 * Revision 1.2  2005/07/12 11:00:19  aitor
 * Correcci�n de errores  de la macro PUBLIC.
 *
 * Revision 1.1  2005/06/23 12:08:37  aitor
 * Nueva estructura de directorios.
 *
 * Revision 1.1  2005/06/23 10:56:18  aitor
 * Archivo con macros de ensamblador.
 *
 */

#ifndef _CPU_ASM_H_
#define _CPU_ASM_H_

#include <autoconf.h>

//------------------------------------------------------------------------------

#define CPU_STACK_ALIGNMENT        16

/*******************************************************************************
 * WINDOW REGISTERS
 ******************************************************************************/

/*
 *  Offsets of fields with Context_Control for assembly routines.
 */

/* Global registers */
#define G0_OFFSET    0x00
#define G1_OFFSET    0x04
#define G2_OFFSET    0x08
#define G3_OFFSET    0x0C
#define G4_OFFSET    0x10
#define G5_OFFSET    0x14
#define G6_OFFSET    0x18
#define G7_OFFSET    0x1C

/*  Local registers */
#define L0_OFFSET    0x20
#define L1_OFFSET    0x24
#define L2_OFFSET    0x28
#define L3_OFFSET    0x2C
#define L4_OFFSET    0x30
#define L5_OFFSET    0x34
#define L6_OFFSET    0x38
#define L7_OFFSET    0x3C

/*  Input registers */
#define I0_OFFSET    0x40
#define I1_OFFSET    0x44
#define I2_OFFSET    0x48
#define I3_OFFSET    0x4C
#define I4_OFFSET    0x50
#define I5_OFFSET    0x54
#define I6_FP_OFFSET 0x58
#define I7_OFFSET    0x5C

/*  Output registers    */
#define O0_OFFSET    0x60
#define O1_OFFSET    0x64
#define O2_OFFSET    0x68
#define O3_OFFSET    0x6C
#define O4_OFFSET    0x70
#define O5_OFFSET    0x74
#define O6_SP_OFFSET 0x78
#define O7_OFFSET    0x7C

/** PSR register    */
#define PSR_OFFSET   0x80

#define Y_OFFSET   0x84

/**  Context control size    */
#define CONTEXT_CONTROL_SIZE 0x88

#define CPU_STACK_FRAME_L0_OFFSET             0x00
#define CPU_STACK_FRAME_L1_OFFSET             0x04
#define CPU_STACK_FRAME_L2_OFFSET             0x08
#define CPU_STACK_FRAME_L3_OFFSET             0x0c
#define CPU_STACK_FRAME_L4_OFFSET             0x10
#define CPU_STACK_FRAME_L5_OFFSET             0x14
#define CPU_STACK_FRAME_L6_OFFSET             0x18
#define CPU_STACK_FRAME_L7_OFFSET             0x1c
#define CPU_STACK_FRAME_I0_OFFSET             0x20
#define CPU_STACK_FRAME_I1_OFFSET             0x24
#define CPU_STACK_FRAME_I2_OFFSET             0x28
#define CPU_STACK_FRAME_I3_OFFSET             0x2c
#define CPU_STACK_FRAME_I4_OFFSET             0x30
#define CPU_STACK_FRAME_I5_OFFSET             0x34
#define CPU_STACK_FRAME_I6_FP_OFFSET          0x38
#define CPU_STACK_FRAME_I7_OFFSET             0x3c

/*  Minimum Stack frame size    */
#define CPU_MINIMUM_STACK_FRAME_SIZE          0x60


#define CONTEXT_CONTROL_INTERRUPT_FRAME_SIZE CPU_MINIMUM_STACK_FRAME_SIZE + 0x50 


#define SPARC_NUMBER_OF_REGISTER_WINDOWS      8

//------------------------------------------------------------------------------

/*******************************************************************************
 * TRAPS
 ******************************************************************************/

/**
 * Software trap
 * This macro provokes a software trap
 */
#define SOFT_TRAP    ta 0;nop;nop;nop;

/**
 * This macro is for traps we should NEVER get
 * The PSR is stored in the %lo register
 * The TRAP TYPE is stored in the %l7 register
 * The WIM is stored in the %l3 register
 */
#define BAD_TRAP(num) \
        rd %psr, %l0; mov num, %l7; b bad_trap_handler; rd %wim, %l3;
        
/**
 * The TRAP_ENTRY macro:
 *  - moves the status register (PSR) into the L0 register
 *   - moves the window invalid register (WIM) into the L4 register
 *   - moves the trap type into the L3 register
 *   - branch to the low_trap entry that manages the trap interrupt
 */ 
#define TRAP_ENTRY(type) mov %psr, %l0; mov %wim, %l4;\
                         b irq_entry;mov type, %l3
                         
/**
 *  Entry for traps which jump to a programmer-specified trap handler.
 */
 
#define TRAP(_vector, _handler)  \
  mov   %psr, %l0 ; \
  sethi %hi(_handler), %l4 ; \
  jmp   %l4+%lo(_handler); \
  mov   _vector, %l3


#define PUBLIC(sym) .globl sym

/**
 * This macro decrements the value of a variable passed as an argument.
 * The value of the variable is stored in the reg parameter.
 * 
 * @param variable: this parameter is the variable to be decremented.
 * @param reg:      this parameter is the register that stores the final value
 * of the variable.
 * 
 * The macro use the %l4 local register.
 */
#define DEC_VARIABLE(variable, reg) \
    sethi   %hi(variable), %l4;   \
    ld      [%l4 + %lo(variable)], %reg;   \
    dec     %reg; \
    st      %reg, [%l4 + %lo(variable)];
    
/**
 * This macro increments the value of a variable passed as an argument.
 * The value of the variable is stored in the reg parameter.
 * 
 * @param variable: this parameter is the variable to be incremented.
 * @param reg:      this parameter is the register that stores the final value
 * of the variable.
 * 
 * The macro use the %l4 local register.
 */
#define INC_VARIABLE(variable, reg) \
    sethi   %hi(variable), %l4;   \
    ld      [%l4 + %lo(variable)], %reg;   \
    inc     %reg;   \
    st      %reg, [%l4 + %lo(variable)];

/**
 * This macro verifies the value of the variable passed as argument and branch
 * to the specified label in case of the variable is set.
 * 
 * @param variable: This parameter is the variable that must be tested.
 * @param label:    This parameter is the branch label.
 * 
 * The macro use the %l4 and %l5 local registers.
 */
#define BRANCH_IF_VARIABLE_SET(variable, label)    \
    sethi   %hi(variable), %l4;   \
    ld      [%l4 + %lo(variable)], %l5;   \
    cmp     %l5, 0;   \
    bnz      label;   \
    nop;

/**
 * This macro sets the variable passed as paramenter.
 * 
 * @param variable: This parameter is the parameter to be set.
 * 
 * The macro use the %l5 and %l4 registers.
 */
#define SET_VARIABLE(variable)  \
    mov     1, %l5;     \
    sethi   %hi(variable), %l4;   \
    st      %l5, [%l4 + %lo(variable)];     \

/**
 * This macro saves the PIL stored in the reg register.
 * 
 * @param reg:  This parameter is the registers that will store the PIL
 */
#define SAVE_PIL(reg)   \
    rd %psr, %reg;   \
    and %reg, PSR_PIL, %reg;

/**
 * This macro restores the PIL stored in the reg register.
 * 
 * @param reg:  This parameter is the registers that stores the PIL
 * @param raux:   This parameter is an auxiliar register
 */
#define RESTORE_PIL(reg,raux)   \
    rd %psr, %raux; \
    wr %raux, %reg, %psr; \
    WRITE_PAUSE

/**
 * This macro performs a pause
 */
#define WRITE_PAUSE nop;nop;nop

/**
 * This macro installs a new PIL.
 * 
 * @level:  This parameter is the interrupt level
 * @oreg:   This parameter stores the new PIL value
 */
#define CALCULATE_PIL(level,oreg)   \
    and %level, 0xF, %oreg;   \
    sll %oreg, 8, %oreg;   \


#endif
