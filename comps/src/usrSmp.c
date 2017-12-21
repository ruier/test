/* usrSmp.c - Enabling SMP specific subsystems configlette */

/*
 * Copyright (c) 2007-2010, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01l,28feb12,kk  fixed comment in output (WIND00110930)
01k,07sep10,cww  Updated to use _Vx_ticks_t type
01j,04feb10,d_c  Defect WIND190082:  Fix prototype for usrSmpCpuEnableHook().
01k,11feb10,cww  Updated usrSmpCpuEnableHook param name
01j,03feb10,cww  Fixed usrSmpCpuEnableHook parameter (WIND00190082)
01i,03feb09,cww  Add support for dynamic CPU reservation
01h,14oct08,zl   added SMP ready queue head array
01g,25aug08,jpb  Renamed _WRS_VX_SMP to _WRS_CONFIG_SMP.
01f,16may08,zl   added usrSmpInit()
01e,10sep07,jmg  Call usrSmpCpuEnableHook() per enabled CPU
01d,23aug07,jmg  Added code to wait for all cpus to be ready before returning
                 in routine usrEnableCpu
01c,23jul07,h_k  moved PPC specific code to hpcNet8641 BSP. (CQ:99192)
01b,20jun07,kk   clean up to only be called for SMP
01a,14feb07,kk   written
*/

/*
DESCRIPTION
This configlette supports enabling of VxWorks SMP subsystems. It is included 
via the INCLUDE_SMP_INIT component, which is required for the SMP kernel.

When ENABLE_ALL_CPUS configuration parameter is set to TRUE, then all 
configured CPUs in a multiprocessor system will be automatically enabled 
for VxWorks SMP.

\NOMANUAL
*/

#ifndef  __INCusrSmp_c
#define  __INCusrSmp_c

#include <vsbConfig.h>
#include <kernelLib.h>
#include <vxCpuLib.h>

#ifdef _WRS_CONFIG_SMP
#include <private/vxCpuLibP.h>
#include <private/kernelLibP.h>
#include <private/tickLibP.h>
#include <vxIpiLib.h>
#include <vxBusLib.h>           /* vxbIntToCpuRoute */

/*
 * Per CPU ready queue head. These are in the configlette
 * because the array size is based on the number of CPUs configured in 
 * the project.
 */

Q_HEAD    _WRS_DATA_ALIGN_BYTES(16) readyQSmpHead [VX_SMP_NUM_CPUS];
BMAP_LIST _WRS_DATA_ALIGN_BYTES(16) readyQSmpBMap [VX_SMP_NUM_CPUS];

/* forward declarations */

LOCAL void usrEnableCpu (void);

/*******************************************************************************
*
* usrSmpInit - enable SMP configuration of the kernel
*
* This routine enables multiprocesor support of the kernel: sets up the
* scheduler IPI, enables CPUs depending on the configuration parameters
* 
* \NOMANUAL
*/

void usrSmpInit (void)
    {
    /* connect and enable scheduler IPI */

    if ((vxIpiConnect (IPI_INTR_ID_SCHED,
		       (IPI_HANDLER_FUNC) windRescheduleNotify, 
		       NULL) == ERROR) ||
	(vxIpiEnable (IPI_INTR_ID_SCHED) == ERROR))
	{
#ifdef	INCLUDE_LOGGING
	logMsg ("usrSmpInit: scheduler inter-processor interrupt "
		"initialization failed \n", 0, 0, 0, 0, 0, 0);
#endif
	return;
	}

    /* Initialize the CPU reservation pool */

    vxCpuReservedStringParse (VX_SMP_CPU_EXPLICIT_RESERVE);

    /* enable configured CPUs */

    usrEnableCpu ();
    }

/*******************************************************************************
*
* usrEnableCpu - enable all configured CPUs for an SMP system
*
* DESCRIPTION	
* This routine enables all configured CPUs in an SMP system. The set of
* configured CPUs is determined by taking the max of the VX_SMP_NUM_CPUS 
* define within the BSP and also the available CPUs discovered by the
* sysCpuAvailableGet() function. The max of these two values must be
* less than the VX_MAX_SMP_CPUS define.  
* 
* If this routine is unable to enable all CPUs specified, before the 
* timeout (VX_ENABLE_CPU_TIMEOUT) expires, the routine will return.  The
* enabled CPUs can be found by calling the routine vxCpuEnabledGet(). 
*
* \NOMANUAL
*/

LOCAL void usrEnableCpu (void)
    {
#if (ENABLE_ALL_CPUS == TRUE)
    UINT cpu;
    UINT numCpus = vxCpuConfiguredGet ();
    cpuset_t enabledCpus;
    _Vx_ticks64_t timeoutTicks, timeoutStart, currentTickCount;
    
    CPUSET_ZERO (enabledCpus);
    
    /* CPU zero is already up and running */
    
    CPUSET_SET (enabledCpus, 0);	
    
    vxCpuEnableTimeout = VX_ENABLE_CPU_TIMEOUT;
    timeoutTicks = vxCpuEnableTimeout * sysClkRateGet ();

    for (cpu = 1; cpu < numCpus; cpu++)
        {
        if (kernelCpuEnableInternal (cpu) == ERROR)
            {
#ifdef	INCLUDE_LOGGING
	    logMsg ("usrEnableCpu: error when enabling additional CPU: %x\n"
		    "userEnableCpu: kernelCpuEnableInternal() failed\n"
		     ,cpu, 0, 0, 0, 0, 0);

#endif	/* INCLUDE_LOGGING */
            }
        else
            CPUSET_SET (enabledCpus, cpu);
	}
	
    /* 
     * After kernelCpuEnable() is called for each individual CPU check
     * that all CPUs are enabled 
     */
    timeoutStart = vxAbsTicksGet ();
   
    do
	{
	currentTickCount = vxAbsTicksGet ();
	
	if ((currentTickCount - timeoutStart) > timeoutTicks)
	    {
#ifdef	INCLUDE_LOGGING
	    for (cpu = 1; cpu < numCpus; cpu ++)
		{
		if (!CPUSET_ISSET (_WRS_CPU_ENABLED (),cpu))
	    	    logMsg ("usrEnableCpu: Timeout (%d secs) waiting to enable "
	    		    "additional CPU: %x\n", vxCpuEnableTimeout, cpu, 
	    		    0, 0, 0, 0);
		}
#endif	/* INCLUDE_LOGGING */	    
	    
	    /* Break out of the do while loop with CPUs that are running */
	    
	    break; 
	    }
	
	/* 
	 * busy loop
	 * We don't know the order that the CPUs will be 
	 * enabled but this way as soon as they are all done we'll 
	 * get past the while loop.
	 */
	
	} while (_WRS_CPU_ENABLED () != enabledCpus);
    
    /* 
     * Call usrSmpCpuEnableHook() only for CPUs that have been fully enabled.
     */
    
    for (cpu = 1; cpu < numCpus; cpu++)
	{
   	if (CPUSET_ISSET (_WRS_CPU_ENABLED (), cpu))
   	    {
   	    usrSmpCpuEnableHook (cpu);
   	    }	
	}
    
#endif  /* ENABLE_ALL_CPUS */
    }

/*******************************************************************************
*
* usrSmpCpuEnableHook - routines that need to be called per CPU
*
* DESCRIPTION    
* This routine contains a set of functions that need to be call per
* CPU after the CPU ans been initialized.  Any subsystem that needs to have a
* specific routine caller per CPU after initialization should add their
* respective routine to the list below.  The ordering of the routines does
* mater and should be carefully considered when adding a routine to the list.
* In the future this function will be replace with a taskHook table
* for the routine kernelCpuEnableInternal().	
*
* \NOMANUAL
*/

STATUS usrSmpCpuEnableHook
    (
    UINT logicalCpuIndex     /* logical CPU index */
    )
    {
    /* route interrupts to the specified CPU */
    
    vxbIntToCpuRoute (logicalCpuIndex);
    
    return (OK);
    }

#endif  /* _WRS_CONFIG_SMP */

#endif /* __INCusrSmp_c */

