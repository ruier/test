/* sysCpuPwr.c - wrSbcPowerQuiccII power management routines */

/* Copyright 2003 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/*
modification history
--------------------
01b,24sep04,rec  set power management default mode as configurtation param
01a,6oct03,rec  Created
*/

/*
DESCRIPTION
This file provides methods for managing the sleep mode and later wake-up.  
Functions for managing peripheral power management can be tied in here.

SEE ALSO: "Basic OS", windPwrLib

*/

#include "vxWorks.h"
#include "sysLib.h"
#include "windPwrLib.h"
#include "vxLib.h"
#include "configAll.h"
#warning "included configAll.h"


LOCAL void sysCpuPwrDown (WIND_PWR_MODE mode, ULONG nTicks);
LOCAL void sysCpuPwrUp (WIND_PWR_MODE mode, ULONG  * nTicks);

/****************************************************************************
*
* sysCpuPwrInit - install BSP CPU power management
*
* This routine installs the LOCAL routines sysCpuPwrDown(),
* sysCpuPwrUp(),  and windPwrDown() into Wind's Cpu power management facility.
* This routine is automatically invoked during system initialization in a
* project when the INCLUDE_POWER_MGMT_CPU_BSP_SUPPORT component is included.
*
* This file (and INCLUDE_POWER_MGMT_CPU_BSP_SUPPORT) is not necessary
* unless windPwrModeLong power down mode support is required.
*
* RETURNS: OK
*
* SEE ALSO: windPwrModeSet(), vxArchPowerDownSet(), sysCpuPwrEnable()
*/

STATUS sysCpuPwrInit (void)
    {
    windPwrDownRtnSet (sysCpuPwrDown);
    windPwrUpRtnSet (sysCpuPwrUp);
    
    /* off until board initialization completes */
    
    windPwrModeSet (windPwrModeOff); 
     
    return (OK);
    }

/****************************************************************************
*
* sysCpuPwrEnable - turns on the BSP CPU power management
*
* This routine enables power management on the target board.  It should
* be called after interrupts are enabled.
*
* SEE ALSO: windPwrModeSet(), sysCpuPwrInit()
*/

void sysCpuPwrEnable (void)
    {
    windPwrModeSet (POWER_MGMT_DEFAULT_MODE);
    }

/****************************************************************************
*
* sysCpuPwrDown - BSP CPU power down routine
*
* This routine is called when VxWorks is about to power down the CPU
* by calling VxArchPowerDown().  If the power mode is windPwrModeShort
* this routine doesn't have to do anything, but is invoked by Wind
* CPU power management in case BSP-supported chips should be turned off
* in sync with the CPU. If the power mode is windPwrModeLong, this
* routine turns off the system tick timer and arranges for a wakeup
* interrupt in nTicks.
*
* This routine may not make any kernel calls directly or indirectly and is
* invoked with interrupts locked.
*
* SEE ALSO: windPwrDownRtnSet(), windPwrUpRtnSet(),
*           vxArchPowerDownSet(), sysOneShotEnable(nTicks)
*/

LOCAL void sysCpuPwrDown /* called with interrupts unlocked */
    (
    WIND_PWR_MODE mode,
    ULONG nTicks
    )
    {
    if (mode == windPwrModeLong)
        {
        sysOneShotEnable (nTicks);
        }

    }

/****************************************************************************
*
* sysCpuPwrUp - BSP CPU power up routine
*
* This routine is called when VxWorks wakes up after the CPU was powered
* via vxArchPowerDown().  If the power mode is windPwrModeShort
* this routine doesn't have to do anything but is invoked by Wind CPU
* power management in case BSP-supported chips have to be turned back on
* in sync with the CPU. If the power mode is windPwrModeLong, this
* routine should determine how long the CPU was powered off in ticks,
* set nTicks accordingly and then enable the system tick timer.
*
* This routine may not make any kernel calls directly or indirectly and is
* invoked with interrupts locked.
*
* SEE ALSO: windPwrUpRtnSet(), windPwrDownRtnSet(),
*           vxArchPowerDownSet(), sysOneShotDisable()
*/

LOCAL void sysCpuPwrUp
    (
    WIND_PWR_MODE mode,
    ULONG * nTicks
    )
    {
    if (mode == windPwrModeLong)
        {
        *nTicks = sysOneShotDisable ();
        }
    else
        {
        *nTicks = 0;
        }
    }



