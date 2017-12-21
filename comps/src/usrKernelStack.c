/* usrKernelStack.c - interrupt stack protection support */

/*
 * Copyright (c) 2005-2008, 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01f,29jun10,jpb  Updated for LP64 adaptation.
01e,25aug08,jpb  Renamed _WRS_VX_SMP to _WRS_CONFIG_SMP.
01d,24mar08,jpb  Defect: 112923.  Changed the casting for 2nd parameter 
                 in vmStateSet.
01c,13sep07,pch  CQ102745: remove obsolete usrKernelIntStkSizeAdj()
01b,01dec06,dbt  Fixed for SMP.
01a,04feb05,pch  written, for SPR 102772.
*/

/*
DESCRIPTION
This file supports the INCLUDE_PROTECT_INTERRUPT_STACK component.

\NOMANUAL
*/

#ifndef  __INCusrKernelStack_c
#define  __INCusrKernelStack_c

#include <vsbConfig.h>
#include <private/kernelLibP.h>
#include <private/vxCpuLibP.h>

#ifdef  _ARCH_SUPPORTS_PROTECT_INTERRUPT_STACK
# ifdef  INCLUDE_PROTECT_INTERRUPT_STACK
#include <errno.h>

/* globals imported from kernelLib.c */

#ifndef	_WRS_CONFIG_SMP
IMPORT	char *    vxIntStackEnd;		/* end of interrupt stack */
IMPORT	char *    vxIntStackBase;		/* base of interrupt stack */
#endif	/* !_WRS_CONFIG_SMP */
IMPORT	size_t  vxIntStackOverflowSize;		/* size of overflow region */
IMPORT	size_t  vxIntStackUnderflowSize;	/* size of underflow region */

/* errnos from applying protection */
int	intStackOverflowProtectErrno = 0;
int	intStackUnderflowProtectErrno = 0;


/*******************************************************************************
*
* usrKernelIntStkProtect - apply protection to interrupt stack guard regions
*
* DESCRIPTION
* This function calls vmStateSet() to map the interrupt stack guard regions
* as VM_STATE_VALID_NOT.
*
* \NOMANUAL
*/

void usrKernelIntStkProtect(void)
    {
    STATUS	vmStat;

#  ifdef _WRS_CONFIG_SMP
    UINT32	cpuIx;	/* cpu index */

    for (cpuIx = 0; cpuIx < _WRS_CPU_CONFIGURED (); cpuIx++)
	{
	char * vxIntStackEnd =
			_WRS_KERNEL_CPU_GLOBAL_GET (cpuIx, vxIntStackEnd);
	char * vxIntStackBase =
			_WRS_KERNEL_CPU_GLOBAL_GET (cpuIx, vxIntStackBase);
#  endif /* _WRS_CONFIG_SMP */

#  if  (INTERRUPT_STACK_OVERFLOW_SIZE > 0)
	vmStat =
#   if  (_STACK_DIR == _STACK_GROWS_DOWN)
	    vmStateSet(NULL,
		   (VIRT_ADDR)(vxIntStackEnd - vxIntStackOverflowSize),
		   vxIntStackOverflowSize,
		   VM_STATE_MASK_VALID | MMU_ATTR_SPL_MSK,
		   VM_STATE_VALID_NOT | MMU_ATTR_NO_BLOCK);
#   else   /* _STACK_DIR */
	    vmStateSet(NULL,
		   (VIRT_ADDR)vxIntStackEnd,
		   vxIntStackOverflowSize,
		   VM_STATE_MASK_VALID | MMU_ATTR_SPL_MSK,
		   VM_STATE_VALID_NOT | MMU_ATTR_NO_BLOCK);
#   endif  /* _STACK_DIR */
	if (vmStat == ERROR)
	    intStackOverflowProtectErrno = errno;
#  endif  /* INTERRUPT_STACK_OVERFLOW_SIZE > 0 */
#  if  (INTERRUPT_STACK_UNDERFLOW_SIZE > 0)
	vmStat =
#   if  (_STACK_DIR == _STACK_GROWS_DOWN)
	    vmStateSet(NULL,
		   (VIRT_ADDR)vxIntStackBase,
		   vxIntStackUnderflowSize,
		   VM_STATE_MASK_VALID | MMU_ATTR_SPL_MSK,
		   VM_STATE_VALID_NOT | MMU_ATTR_NO_BLOCK);
#   else   /* _STACK_DIR */
	    vmStateSet(NULL,
		   (VIRT_ADDR)(vxIntStackBase - vxIntStackUnderflowSize),
		   vxIntStackUnderflowSize,
		   VM_STATE_MASK_VALID | MMU_ATTR_SPL_MSK,
		   VM_STATE_VALID_NOT | MMU_ATTR_NO_BLOCK);
#   endif  /* _STACK_DIR */
	if (vmStat == ERROR)
	    intStackUnderflowProtectErrno = errno;
#  endif  /* INTERRUPT_STACK_UNDERFLOW_SIZE > 0 */
#  ifdef _WRS_CONFIG_SMP
	}
#  endif /* _WRS_CONFIG_SMP */
    }
# endif  /* INCLUDE_PROTECT_INTERRUPT_STACK */
#endif  /* _ARCH_SUPPORTS_PROTECT_INTERRUPT_STACK */

#endif	/* __INCusrKernelStack_c */
