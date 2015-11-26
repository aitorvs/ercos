
#ifndef _M68K_MESS_H_
#define _M68K_MESS_H_

#include <kernel/config.h>

#ifdef CONFIG_DEBUG
#define _M68K_DEBUG
#endif

#define __FUNCTION__ (__func__)

#ifdef _M68K_DEBUG
#include <kernel/debug.h>
#define M68K_DEBUG(fmt,args...) \
	DEBUG (fmt, ##args)
	/* printf ("%s: " fmt "\n", __FUNCTION__, ##args) */
#define M68K_TRACE(expr,type)	M68K_DEBUG (#expr " = '%" type "'", expr)
#else
#define M68K_DEBUG(fmt,args...)
#define M68K_TRACE(expr,type)	This will produce an error (as it should be)!
#endif

/* OS_Start symbol publication */
void OS_Start (void);

/* System Integration Module register management macros */
#define M68K_SIM_BASE			0x00fff000

#define M68K_SIM_MCR			0x00000a00
#define M68K_SIM_SYNCR			0x00000a04
#define M68K_SIM_SYPCR			0x00000a21
#define M68K_SIM_PICR			0x00000a22
#define M68K_SIM_PITR			0x00000a24
#define M68K_SIM_CSPAR0			0x00000a44
#define M68K_SIM_CSPAR1			0x00000a46
#define M68K_SIM_CSBARBT		0x00000a48
#define M68K_SIM_CSORBT			0x00000a4a
#define M68K_SIM_CSBAR0			0x00000a4c
#define M68K_SIM_CSOR0			0x00000a4e
#define M68K_SIM_CSBAR1			0x00000a50
#define M68K_SIM_CSOR1			0x00000a52
#define M68K_SIM_CSBAR2			0x00000a54
#define M68K_SIM_CSOR2			0x00000a56
#define M68K_SIM_CSBAR3			0x00000a58
#define M68K_SIM_CSOR3			0x00000a5a
#define M68K_SIM_CSBAR4			0x00000a5c
#define M68K_SIM_CSOR4			0x00000a5e
#define M68K_SIM_CSBAR7			0x00000a68
#define M68K_SIM_CSOR7			0x00000a6a
#define M68K_SIM_CSBAR8			0x00000a6c
#define M68K_SIM_CSOR8			0x00000a6e

/**
 * La situación de los registros de estado no depende de la configuración
 * de los chipselect???
 * FIXME: Comprobar eso
 */

/* Queued Serial Module management macros */
#define M68K_QSM_BASE			0x00fffc00

/* QSM global registers */
#define M68K_QSM_QSMCR			0x00000000
#define M68K_QSM_QILR			0x00000005
#define M68K_QSM_QIVR			0x00000004

/* Serial Communication Interface registers */
#define M68K_QSM_SCCR0			0x00000008
#define M68K_QSM_SCCR1			0x0000000a
#define	M68K_QSM_SCSR			0x0000000c
#define	M68K_QSM_SCDR			0x0000000e

/* Generic write to a register file (8 & 16 bit) */
#define M68K_WRITE8(base,reg,val) 	\
			*((uint8 *) ((base) + (reg))) = (uint8) (val)
#define M68K_WRITE16(base,reg,val) 	\
			*((uint16 *) ((base) + (reg))) = (uint16) (val)

/* Generic read to a register file (8 & 16 bit) */
#define M68K_READ8(base,reg) 	\
			(*((uint8 *) ((base) + (reg))))
#define M68K_READ16(base,reg) 	\
			(*((uint16 *) ((base) + (reg))))

#define M68K_SIM_WRITE8(reg,val)	M68K_WRITE8 (M68K_SIM_BASE, reg, val)
#define M68K_SIM_WRITE16(reg,val)	M68K_WRITE16 (M68K_SIM_BASE, reg, val)
#define M68K_SIM_READ8(reg)		M68K_READ8 (M68K_SIM_BASE, reg)
#define M68K_SIM_READ16(reg)		M68K_READ16 (M68K_SIM_BASE, reg)

#define M68K_QSM_WRITE8(reg,val)	M68K_WRITE8 (M68K_QSM_BASE, reg, val)
#define M68K_QSM_WRITE16(reg,val)	M68K_WRITE16 (M68K_QSM_BASE, reg, val)
#define M68K_QSM_READ8(reg)		M68K_READ8 (M68K_QSM_BASE, reg)
#define M68K_QSM_READ16(reg)		M68K_READ16 (M68K_QSM_BASE, reg)

#endif
