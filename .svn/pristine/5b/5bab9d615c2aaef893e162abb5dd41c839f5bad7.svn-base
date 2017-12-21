/* 40vxbIaHpetTimer.cdf - Component Definition file for IA HPET timer */

/*
 * Copyright 2011-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,25apr13,rbc  WIND00414401 - Changes to support Unified BSP.
01b,20sep12,c_t  add parameter.
01a,06may11,sem  initial version

*/

Component   DRV_TIMER_IA_HPET {
    NAME        IA HPET timer Driver
    SYNOPSIS    IA HPET timer Driver
    CFG_PARAMS  IA_HPET_IOAPIC_UNIT \
                IA_HPET_PERIODIC_SYNC
    MODULES     vxbIaHpetTimer.o
    _CHILDREN   FOLDER_DRIVERS
    _INIT_ORDER hardWareInterFaceBusInit
    PROTOTYPE   void vxbIaHpetTimerDrvRegister (void);
    INIT_RTN    vxbIaHpetTimerDrvRegister();
    REQUIRES    INCLUDE_PLB_BUS \
                INCLUDE_TIMER_SYS
}

Parameter IA_HPET_IOAPIC_UNIT {
    NAME            I/O APIC unit
    SYNOPSIS        I/O APIC unit number which the HPET connected. '-1' means auto-detection.
    TYPE            int
    DEFAULT         (-1)
}

Parameter IA_HPET_PERIODIC_SYNC {
    NAME            Periodic Synchronize the HPET timer
    SYNOPSIS        Choose if periodic synchronize the HPET timer when it is configured as periodic timer.
    TYPE            BOOL
    DEFAULT         TRUE
}
