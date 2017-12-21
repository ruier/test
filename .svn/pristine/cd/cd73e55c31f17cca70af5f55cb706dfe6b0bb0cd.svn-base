/* usrIdleStacks.c - interrupt stack protection support */

/*
 * Copyright (c) 2007-2008, 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,20jul10,jpb  Updated vxIdleExcepStackOverflowSize type.
01b,25aug08,jpb  Renamed _WRS_VX_SMP to _WRS_CONFIG_SMP.
01a,13sep07,pch  written, from usrKernelStack.c vn of 13sep07
*/

/*
DESCRIPTION
This file supports the INCLUDE_PROTECT_IDLE_TASK_STACK component.

\NOMANUAL
*/

#ifndef	__INCusrIdleStacks_c
#define	__INCusrIdleStacks_c

#include <vsbConfig.h>
#include <private/kernelLibP.h>
#include <private/vxCpuLibP.h>

#ifdef	_WRS_CONFIG_SMP
# ifdef  _ARCH_SUPPORTS_PROTECT_IDLE_TASK_STACK
#  ifdef  INCLUDE_PROTECT_IDLE_TASK_STACK
#include <errno.h>

/* globals imported from kernelLib.c */

IMPORT	size_t vxIdleExcepStackOverflowSize;  /* size of overflow region */
IMPORT	size_t vxIdleExcepStackUnderflowSize; /* size of underflow region */

/* errnos from applying protection */

int	idleExcepStackOverflowProtectErrno = 0;
int	idleExcepStackUnderflowProtectErrno = 0;


/*******************************************************************************
*
* usrIdleTaskStkProtect - protect idle-task exception stack guard regions
*
* DESCRIPTION
* This function calls vmStateSet() to map the idle-task exception stack
* guard regions as VM_STATE_VALID_NOT.
*
* \NOMANUAL
*/

void usrIdleTaskStkProtect(void)
    {
    STATUS	vmStat;
    UINT32	cpuIx;	/* cpu index */

    for (cpuIx = 0; cpuIx < _WRS_CPU_CONFIGURED (); cpuIx++)
	{
	WIND_TCB * idleTaskId = _WRS_KERNEL_CPU_GLOBAL_GET (cpuIx, idleTaskId);
	char * stackBase = idleTaskId->pExcStackBase;
	char * stackEnd = idleTaskId->pExcStackEnd;

#  if  (IDLE_TASK_EXC_STACK_OVERFLOW_SIZE > 0)
	vmStat =
#   if  (_STACK_DIR == _STACK_GROWS_DOWN)
	    vmStateSet(NULL,
		   (VIRT_ADDR)(stackEnd - vxIdleExcepStackOverflowSize),
		   vxIdleExcepStackOverflowSize,
		   VM_STATE_MASK_VALID | MMU_ATTR_SPL_MSK,
		   VM_STATE_VALID_NOT | MMU_ATTR_NO_BLOCK);
#   else   /* _STACK_DIR */
	    vmStateSet(NULL,
		   (VIRT_ADDR)stackEnd,
		   vxIdleExcepStackOverflowSize,
		   VM_STATE_MASK_VALID | MMU_ATTR_SPL_MSK,
		   VM_STATE_VALID_NOT | MMU_ATTR_NO_BLOCK);
#   endif  /* _STACK_DIR */
	if (vmStat == ERROR)
	    idleExcepStackOverflowProtectErrno = errno;
#  endif  /* IDLE_TASK_EXC_STACK_OVERFLOW_SIZE > 0 */
#  if  (IDLE_TASK_EXC_STACK_UNDERFLOW_SIZE > 0)
	vmStat =
#   if  (_STACK_DIR == _STACK_GROWS_DOWN)
	    vmStateSet(NULL,
		   (VIRT_ADDR)stackBase,
		   vxIdleExcepStackUnderflowSize,
		   VM_STATE_MASK_VALID | MMU_ATTR_SPL_MSK,
		   VM_STATE_VALID_NOT | MMU_ATTR_NO_BLOCK);
#   else   /* _STACK_DIR */
	    vmStateSet(NULL,
		   (VIRT_ADDR)(stackBase - vxIdleExcepStackUnderflowSize),
		   vxIdleExcepStackUnderflowSize,
		   VM_STATE_MASK_VALID | MMU_ATTR_SPL_MSK,
		   VM_STATE_VALID_NOT | MMU_ATTR_NO_BLOCK);
#   endif  /* _STACK_DIR */
	if (vmStat == ERROR)
	    idleExcepStackUnderflowProtectErrno = errno;
#  endif  /* IDLE_TASK_EXC_STACK_UNDERFLOW_SIZE > 0 */
	}
    }
#  endif  /* INCLUDE_PROTECT_IDLE_TASK_STACK */
# endif  /* _ARCH_SUPPORTS_PROTECT_IDLE_TASK_STACK */
#endif	/* _WRS_CONFIG_SMP */

#endif	/* __INCusrIdleStacks_c */
