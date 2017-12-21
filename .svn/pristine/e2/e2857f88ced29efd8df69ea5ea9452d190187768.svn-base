/* usrMipcSm.c - Initialization of MIPC-SM */

/*
 * Copyright (c) 2008-2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
02r,06apr11,gjj  Ensure number of events is not more than 65535 (WIND00264637)
02q,03mar11,swg  change the maximum valid value for MIPC_SM_HEARTBEAT_PERIOD 
                 from 1000000000 to 65535, to fix WIND00254618.
02p,11nov10,rab  add support for arm guest os
02o,04nov10,swg  add the explicit declaration for _procNumWasSet
                 (to fix WIND00237118).
02n,23sep10,jlk  Cast error causes truncation on intConnect() calls for 64 bit.
02m,09sep10,jlk  Cleanup: Remove obsolete MIPC_SM_SEMTASK_PRIORITY parameter
02l,24aug10,jlk  Removed inclusion of funcBindP.h which is now deprecated
02k,20aug10,swg  Fix the defect WIND00217941
02j,05may10,als  Ensure error checking is done when simulator is used
02i,30mar10,als  Initial changes in support of 64-bit memory addressing
02h,29oct09,als  Handle missing SM region; complete VBI 2.0 conversion
02g,14sep09,bwa  remove default for IRQ
02f,21aug09,dcc  port to VBI 2.0
02e,25aug09,bwa  Add initialization of sysProcNum, used as node identifier.
02d,21aug09,bwa  Changed IPI default from 0 to 3, which is the MIPC reserved entry.
                 Removed demux of IRQ for VxMP/MIPC for hypervisor builds.
02c,13aug09,als  Improve robustness of packet pool
02b,06aug09,h_x  Fix WIND00170989
02a,04aug09,als  Convert shared memory offsets to use MIPC_OFFSET type
01z,04aug09,ebh  Fix compile issue for simulator projects
01y,27jul09,als  Event queue rework
01x,02jul09,dcc  added WRHV-Guest-OS/PPC specific code.
01w,02jul09,als  Changes to reflect updated file names
01v,25jun09,dcc  enabled to work with Hypervisor Guest OS
01v,24jun09,ebh  Updates for range-checking
01t,18jun09,ebh  Remove duplicate assignment
01s,15jun09,als  Add range checking for MIPC configuration settings
01r,08jun09,elp  added simulator support.
01q,02jun09,als  Rework system configuration/initialization as per HLD and CI
01p,28may09,als  Revise configuration based on MIPC 2.0 HLD feedback
01o,25may09,ebh  Code Inspection Modifications
01n,13may09,ebh  Add in missing include file atomic.h
01m,05may09,h_w  Replaced APIs vxAtomicXXX with macros MULTIOS_ATOMIC_XXX
01l,30apr09,ebh  Ensure that mipc.h is included first
01k,23apr09,ebh  Adjust the TX buffer per bus usage
01j,21apr09,ebh  Add capability to give all semaphores at task level
01i,18mar09,ebh  Initialization clean-up
01h,12mar09,ebh  Add some debugging code.
01g,24feb09,ebh  Modify QOS handling for multiple buses
01f,19feb09,ebh  Fix ISR mode freezing
01e,18feb09,ebh  Collapse 3 packet pools into a single pool per bus
01d,17feb09,ebh  Fix argument passed to processes
01c,17feb09,ebh  Turn off logging in checked in version
01b,13feb09,ebh  Processing mode changes
01a,25jan09,ebh  Updates for MIPC 2.0
01a,25nov08,apa  Initial version.
*/

/*
DESCRIPTION
This configlette passes the user's MIPC configuration settings to MIPC-SM.

It also contains code that handles any required sharing of resources between
MIPC and the VxWorks SM library, if both are present.
*/

#include <bootLib.h>
#include <sysLib.h>
#include <multios_ipc/mipc_sm_adapt.h>
#if defined (_WRS_CONFIG_WRHV_GUEST)
#include <wrhv/vbi.h>
#else
/* 
 * This header file isn't needed in the Hypervisor case as MIPC uses VBI APIs 
 * instead of the VxWorks IPI APIs
 */
#include <vxIpiLib.h>
#endif

/*
 * Undefine MIPC_DO_INIT to allow users to initialize MIPC manually from shell;
 * this allows MIPC output generated during initialization to be seen
 */

#define MIPC_DO_INIT

#if !defined(_WRS_CONFIG_WRHV_GUEST)
/*
 * Generate compile error if MIPC hardware settings needed for UAMP
 * are not defined by BSP or by project user
 */

#if !defined(_WRS_ARCH_IS_SIMULATOR)
#if (MIPC_SM_SYSTEM_POOL_BASE == -1)
	#error "MIPC_SM_SYSTEM_POOL_BASE is -1: it must be assigned a valid value by either the BSP or the configuration"
#endif
#else
/*
 * Simulator's CDF defines MIPC_SM_SYSTEM_POOL_BASE to be an alias for a
 * global variable which contains the address of MIPC's shared memory region
 * (which is dynamically allocated at run-time)
 */
extern void *sysMipcSmAddr;
#endif /* !(_WRS_ARCH_IS_SIMULATOR) */

#if (MIPC_SM_SYSTEM_POOL_SIZE == -1)
	#error "MIPC_SM_SYSTEM_POOL_SIZE is -1: it must be assigned a valid value by either the BSP or the configuration"
#endif

#if (MIPC_SM_NODE_IRQ == -1)
	#error "MIPC_SM_NODE_IRQ is -1: it must be assigned a valid value by either the BSP or the configuration"
#endif

#endif /* !(_WRS_CONFIG_WRHV_GUEST) */


/*
 * Generate compile error if MIPC CDF settings are invalid
 */

#if (MIPC_SM_BUFFERS < 1) || (MIPC_SM_BUFFERS > 65535)
	#error "MIPC_SM_BUFFERS must be between 1 and 65535"
#endif

#if (MIPC_SM_MTU < 1)
	#error "MIPC_SM_MTU must be at least 1"
#endif

#if (MIPC_SM_NODES < 1) || (MIPC_SM_NODES > 1024)
	#error "MIPC_SM_NODES must be between 1 and 1024"
#endif

#if (MIPC_SM_PORTS < 2) || (MIPC_SM_PORTS > 1024)
	#error "MIPC_SM_PORTS must be between 2 and 1024"
#endif

#if (MIPC_SM_EVENTS < 1) || (MIPC_SM_EVENTS > 65535)
	#error "MIPC_SM_EVENTS must be between 1 and 65535"
#endif

#if (MIPC_SM_QOS < -1) || (MIPC_SM_QOS > 255)
	#error "MIPC_SM_QOS must be between -1 and 255"
#endif

#if (MIPC_SM_SOCKETS < -1) || (MIPC_SM_SOCKETS > 1048576)
	#error "MIPC_SM_SOCKETS must be between -1 and 1048576"
#endif

#if (MIPC_SM_INIT_MODE < 0) || (MIPC_SM_INIT_MODE > 2)
	#error "MIPC_SM_INIT_MODE must be between 0 and 2"
#endif

#if (MIPC_SM_HEARTBEAT_PERIOD < 10) || (MIPC_SM_HEARTBEAT_PERIOD > 65535)
	#error "MIPC_SM_HEARTBEAT_PERIOD must be between 10 and 65535"
#endif

#if (MIPC_SM_RXTASK_STACK < 2000)
	#error "MIPC_SM_RXTASK_STACK must be at least 2000"
#endif

/* Determine if MIPC is sharing interrupts with the SM library. */
#ifndef _WRS_CONFIG_WRHV_GUEST
    #ifdef INCLUDE_SM_COMMON
	#define MIPC_INTERRUPT_SHARING
    #endif
#endif

/* Globals used by rest of MIPC-SM adaptation layer */

int mipc_sm_rxtask_stack = MIPC_SM_RXTASK_STACK;

#if defined _WRS_CONFIG_WRHV_GUEST
#if (CPU_FAMILY == PPC)

extern int vb2HwConvert(int vbVector); 

#endif /* CPU_FAMILY == PPC */
#endif /* _WRS_CONFIG_WRHV_GUEST */

extern BOOL _procNumWasSet;

/*****************************************************************************
 * 
 * usrMipcSmInitReal - initialize MIPC-SM
 *
 * This routine passes the configuration settings for MIPC-SM (specified via
 * CDF) to MIPC's main initialization routine.
 *
 * Returns: 0 if successful; otherwise -errno
 *
 * NOMANUAL
 */

int usrMipcSmInitReal(void)
{
	struct mipc_sm_config cfg;	/* MIPC configuration settings */

#if defined _WRS_CONFIG_WRHV_GUEST
    	uint32_t   vbSmAttr;
    	uint32_t   systemPoolSize;
	void *systemPoolBaseAddr;

	if (vbiShmemRegionFind ((int8_t *)"mipc_sm",
				&systemPoolBaseAddr,  
				(uint32_t *)&systemPoolSize,
				(uint32_t *)&vbSmAttr)) {
	    MIPC_PRINT(MIPC_ALERT "shared memory not found\n");
	    return -MIPC_ENOMEM;
	}

	cfg.systemPoolBaseAddr = systemPoolBaseAddr;
	cfg.systemPoolSize = (MIPC_MEMSIZE)systemPoolSize;
	/* 
	 * cfg.nodeIntId is not set since the interrupt defined in XML is
	 * obtained via VBI.
	 */
#else
	cfg.systemPoolBaseAddr =	(void *)(ULONG)MIPC_SM_SYSTEM_POOL_BASE;
	cfg.systemPoolSize = 		(MIPC_MEMSIZE)MIPC_SM_SYSTEM_POOL_SIZE;
	cfg.nodeIntId =			MIPC_SM_NODE_IRQ;
#endif 
	cfg.busList =			MIPC_SM_BUS_LIST;
	cfg.nodeBuffers =		MIPC_SM_BUFFERS;
	cfg.nodeMtu =			MIPC_SM_MTU;
	cfg.nodeMaxNodes =		MIPC_SM_NODES;
	cfg.nodeMaxPorts =		MIPC_SM_PORTS;
	cfg.nodeEvents =		MIPC_SM_EVENTS;
	cfg.nodeQos =			MIPC_SM_QOS;
	cfg.nodeSockets =		MIPC_SM_SOCKETS;
	cfg.nodeInitMode =		MIPC_SM_INIT_MODE;
	cfg.nodeHeartbeatPeriod =	MIPC_SM_HEARTBEAT_PERIOD;
	
	return mipc_sm_init(&cfg);
}

/*****************************************************************************
 * 
 * usrMipcSmInit - initialization routine called during VxWorks startup
 *
 * NOMANUAL
 */

int usrMipcSmInit(void)
{
	/* Set OS-wide sysProcNum, which is really the node number (different
	 * from the CPU number; yes, it is really badly named...). Other sub-
	 * systems might try to set it, namely the network stack, but they
	 * come waaay too late in the game and MIPC initializes *very* early
	 * on. */
	if(!_procNumWasSet) {
		BOOT_PARAMS p;
		char *pEndStr;
		pEndStr = bootStringToStruct(sysBootLine, &p);
    		if (*pEndStr != EOS) {
			errnoSet(EINVAL);
        		bootParamsErrorPrint(sysBootLine, pEndStr);
        		return ERROR;
        	}
		sysProcNumSet(p.procNum);
		_procNumWasSet = TRUE;
	}

#ifdef MIPC_DO_INIT
	return usrMipcSmInitReal();
#else
	return 0;
#endif
}

/*****************************************************************************
	      MIPC-SM and SM library interrupt sharing code
 *****************************************************************************/

#ifdef MIPC_INTERRUPT_SHARING

MIPC_ISR_FUNC mipc_isr_func;
extern FUNCPTR smIntHandler;
/*****************************************************************************
 * 
 * mipcIsrShareFunc - ISR used when MIPC-SM and SM are sharing an interrupt
 *
 * This routine invokes the ISR MIPC-SM has requested, then invokes the
 * interrupt handler for the SM library.
 *
 * NOMANUAL
 */

void mipcIsrShareFunc(void *arg)
{
    (*mipc_isr_func)(arg);
    smIntHandler();
}

#endif /* MIPC_INTERRUPT_SHARING */

/*****************************************************************************
* mipc_sm_isr_set - register an ISR for use by MIPC-SM
*
* This routine causes the specified routine to be invoked (with its argument)
* when the designated interrupt occurs.  In the event that the SM library is
* also using that interrupt, MIPC registers a demuxing routine that allows
* both MIPC and SM to respond to the interrupt.
*/
int mipc_sm_isr_set(uint16_t intId, MIPC_ISR_FUNC func, void *arg)
{
#ifdef MIPC_INTERRUPT_SHARING
    /* Register interrupt sharing ISR, but remember which MIPC ISR is wanted */

    mipc_isr_func = func;
    func = mipcIsrShareFunc;
#endif /* MIPC_INTERRUPT_SHARING */

#if defined _WRS_CONFIG_WRHV_GUEST
#if (CPU_FAMILY == I80X86)
    if (intConnect ((VOIDFUNCPTR *) (INUM_TO_IVEC(intId + INT_NUM_IRQ0)), 
                    (VOIDFUNCPTR) func, (_Vx_usr_arg_t) arg) != OK)
#elif (CPU_FAMILY == PPC) || (CPU_FAMILY == ARM)

    logMsg ("intId = 0x%x, conv = 0x%x \n", intId, vb2HwConvert(intId),0,0,0,0);

    if (intConnect ((VOIDFUNCPTR *) vb2HwConvert(intId), 
                    (VOIDFUNCPTR) func, (_Vx_usr_arg_t) arg) != OK)
#else
	#error "arch not supported for MIPC on WRHV Guest OS"
#endif
	{
	return -MIPC_EINVAL;
	}

    vbiVioapicIntUnmask ((vbiIrq_t) intId);

#else
    if (vxIpiConnect(intId, func, arg) != OK)
	{
	return -MIPC_EINVAL;
	}

    if (vxIpiEnable(intId) != OK)
	{
	vxIpiDisconnect(intId, func, arg);
	return -MIPC_EINVAL;
	}
#endif /*_WRS_CONFIG_WRHV_GUEST*/

    return MIPC_OK;
}

