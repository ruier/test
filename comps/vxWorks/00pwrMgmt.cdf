/* 00pwrMgmt.cdf - Power Management component description file */

/*
* Copyright 2011, 2012 Wind River Systems, Inc.
*
* The right to copy, distribute or otherwise make use of this software
* may be licensed only pursuant to the terms of an applicable Wind River
* license agreement. No license to Wind River intellectual property rights
* is granted herein. All rights not licensed by Wind River are reserved
* by Wind River.
*/

/*
modification history
--------------------
01f,12jan12,jlv  WIND00325771 - Add INCLUDE_PWR_POLICY_STATS component
01e,25oct11,rbc  WIND00312117 - Fix hang with PROFILE_DEVELOPMENT
01d,03oct11,rbc  WIND00307577 - Fix FOLDER_PWR_MGMT for tickless.
01c,29sep11,sem  WIND00308332 - Move in component definitions from arch and bsp
01b,16sep11,j_l  WIND00297582 - Move idle management entries from BSP to here.
01a,22aug11,rbc  Created

DESCRIPTION

This file contains the power management components.
*/

/* Power managment */

#ifdef _WRS_CONFIG_PWR_MGMT

Folder  FOLDER_PWR_MGMT {
    NAME        Power Management
    SYNOPSIS    Power Management Components
    CHILDREN    INCLUDE_CPU_PWR_ARCH \
                INCLUDE_CPU_PWR_MGMT \
                INCLUDE_PWR_CPU_MON \
                INCLUDE_PWR_POLICY_MGR \
                INCLUDE_DEV_PWR_MGMT \
                INCLUDE_TICKLESS_IDLE \
                SELECT_POWER_MGMT
}

/*******************************************************************************
*
* Power Management Library Functions
*
*/

Component INCLUDE_PWR_LIB_FUNCS {
    NAME            Power Management Library Functions
    SYNOPSIS        Power Management Library Functions
    _INIT_ORDER     usrInit
    CONFIGLETTES    $(TGT_DIR)/src/hwif/pwr/pwrLibFuncs.c
    INIT_RTN        pwrLibFuncInit();
    INIT_AFTER      INCLUDE_SYSHW_INIT
    INIT_BEFORE     INCLUDE_KERNEL
    PROTOTYPE       void pwrLibFuncInit (void);
    _CHILDREN       FOLDER_NOT_VISIBLE
    INCLUDE_WHEN    INCLUDE_CPU_PWR_ARCH \
                    INCLUDE_CPU_PWR_MGMT \
                    INCLUDE_DEV_PWR_MGMT \
                    INCLUDE_PWR_POLICY_MGR \
                    INCLUDE_TICKLESS_IDLE
}

/*******************************************************************************
*
* CPU Power Management
*
*/

Component INCLUDE_CPU_PWR_MGMT {
    NAME            CPU Power Management
    SYNOPSIS        CPU Power Management
    PROTOTYPE       STATUS pwrCpuLibInit (UINT32, UINT32, UINT32, UINT32, \
                                          UINT32, UINT32, UINT32, UINT32, \
                                          UINT32, UINT32, UINT32);
    INIT_RTN        pwrCpuLibInit(CPU_PWR_POLICY_SAMPLE_INTERVAL, \
                                  CPU_PWR_POLICY_LOW_FILTER_INTERVAL, \
                                  CPU_PWR_POLICY_HIGH_FILTER_INTERVAL, \
                                  CPU_PWR_POLICY_SLEEP_FILTER_INTERVAL, \
                                  CPU_PWR_POLICY_LOW_HEADROOM_THRESH, \
                                  CPU_PWR_POLICY_HIGH_HEADROOM_THRESH, \
                                  CPU_PWR_POLICY_LOW_HEADROOM_ALARM_THRESH, \
                                  CPU_PWR_POLICY_IDLE_MAX_INTERRUPTS, \
                                  CPU_PWR_POLICY_SLEEP_HEADROOM_THRESH, \
                                  CPU_PWR_POLICY_PSTATE_CAPABILITY, \
                                  CPU_PWR_POLICY_CSTATE_CAPABILITY);
    CFG_PARAMS      CPU_PWR_POLICY_SAMPLE_INTERVAL \
                    CPU_PWR_POLICY_LOW_FILTER_INTERVAL \
                    CPU_PWR_POLICY_HIGH_FILTER_INTERVAL \
                    CPU_PWR_POLICY_SLEEP_FILTER_INTERVAL \
                    CPU_PWR_POLICY_LOW_HEADROOM_THRESH \
                    CPU_PWR_POLICY_HIGH_HEADROOM_THRESH \
                    CPU_PWR_POLICY_LOW_HEADROOM_ALARM_THRESH \
                    CPU_PWR_POLICY_IDLE_MAX_INTERRUPTS \
                    CPU_PWR_POLICY_SLEEP_HEADROOM_THRESH \
                    CPU_PWR_POLICY_PSTATE_CAPABILITY \
                    CPU_PWR_POLICY_CSTATE_CAPABILITY
    _CHILDREN       FOLDER_PWR_MGMT
    _INIT_ORDER     usrRoot
    INIT_BEFORE     INCLUDE_PWR_POLICY_MGR
    INIT_AFTER      INCLUDE_PWR_CPU_MON
    REQUIRES        INCLUDE_CPU_PWR_ARCH \
                    INCLUDE_PWR_CPU_MON \
                    INCLUDE_PWR_CPU_MON_ARCH \
                    INCLUDE_PWR_LIB_FUNCS
}

/*******************************************************************************
*
* Power Policy Manager
*
*/

Component INCLUDE_PWR_POLICY_STATS {
    NAME            Power Policy Manager Stats
    SYNOPSIS        Power Policy Manager Power State Statistics
    INIT_RTN        pwrPolicyMgrStatsInit();
    PROTOTYPE       STATUS pwrPolicyMgrStatsInit(void);
    REQUIRES        INCLUDE_PWR_POLICY_MGR
    _CHILDREN       FOLDER_PWR_MGMT
    _INIT_ORDER     usrRoot
    INIT_BEFORE     INCLUDE_PWR_POLICY_MGR
}

Component INCLUDE_PWR_POLICY_MGR {
    NAME            Power Policy Manager
    SYNOPSIS        Power Policy Manager
    INIT_RTN        pwrPolicyMgrInit(PWR_POLICY_MGR_TASK_PRIORITY);
    PROTOTYPE       STATUS pwrPolicyMgrInit(UINT32);
    REQUIRES        INCLUDE_ERF \
                    INCLUDE_PWR_LIB_FUNCS \
                    INCLUDE_MSG_Q_CREATE_DELETE
    CFG_PARAMS      PWR_POLICY_MGR_TASK_PRIORITY
    _CHILDREN       FOLDER_PWR_MGMT
}

Parameter PWR_POLICY_MGR_TASK_PRIORITY {
    NAME        Power Policy Manager Task Priority
    SYNOPSIS    This is the default task priority for the PPM maintenance task.
    TYPE        UINT32
    DEFAULT     200
}

Parameter CPU_PWR_POLICY_SAMPLE_INTERVAL  {
    NAME        CPU utilization sample interval
    SYNOPSIS    This is the interval used by the CPU utilization \
        computation function.  CPU utilization is computed at least once each \
        interval unless the processor is idle for an extended period of time.\
        (ticks, range 1..n)
    TYPE        UINT32
    DEFAULT     300
}

Parameter CPU_PWR_POLICY_LOW_FILTER_INTERVAL  {
    NAME        Low headroom filter interval
    SYNOPSIS    This is number of CPU_PWR_POLICY_SAMPLE_INTERVAL periods that a low \
        headroom event must be maintained before an event is signalled from the \
        utilization function to the CPU Power Manager.\
        (CPU_PWR_POLICY_SAMPLE_INTERVAL periods, range 1..n)
    TYPE        UINT32
    DEFAULT     2
}

Parameter CPU_PWR_POLICY_HIGH_FILTER_INTERVAL  {
    NAME        High headroom filter interval
    SYNOPSIS    This is number of CPU_PWR_POLICY_SAMPLE_INTERVAL periods that a high \
        headroom event must be maintained before an event is signalled from the \
        utilization function to the CPU Power Manager. \
        (CPU_PWR_POLICY_SAMPLE_INTERVAL periods, range 1..n)
    TYPE        UINT32
    DEFAULT     2
}

Parameter CPU_PWR_POLICY_SLEEP_FILTER_INTERVAL  {
    NAME        Sleep filter interval
    SYNOPSIS    This is number of sampleInterval periods that a low \
        CPU utilization must be maintained before the CPU Power Manager shifts \
        into sleep mode.\
        (CPU_PWR_POLICY_SAMPLE_INTERVAL periods, range 1..n)
    TYPE        UINT32
    DEFAULT     10
}

Parameter  CPU_PWR_POLICY_LOW_HEADROOM_THRESH {
    NAME        Low headroom threshold
    SYNOPSIS    This is the low headroom threshold. (percentage, range 1..99)
    TYPE        UINT32
    DEFAULT     60
}

Parameter  CPU_PWR_POLICY_HIGH_HEADROOM_THRESH {
    NAME        High headroom threshold
    SYNOPSIS    This is the high headroom threshold. (percentage, range 1..99)
    TYPE        UINT32
    DEFAULT     40
}

Parameter  CPU_PWR_POLICY_LOW_HEADROOM_ALARM_THRESH {
    NAME        Low headroom alarm threshold
    SYNOPSIS    This is the low headroom alarm trip-point.  This \
        event is not filtered and is computed for only one \
        CPU_PWR_POLICY_SAMPLE_INTERVAL. \
        It is normally set to a high number such as 99%. \
        (percentage, range 1..99)
    TYPE        UINT32
    DEFAULT     95
}

Parameter  CPU_PWR_POLICY_IDLE_MAX_INTERRUPTS {
    NAME    Maximum sleep interrupts
    SYNOPSIS    If the CPU is servicing few interrupts per sample and \
        it is running with a utilization rate below the \
        CPU_PWR_POLICY_HIGH_HEADROOM_THRESH, it may place into a low power \
        mode (C3).  There may be significant amount of overhead involved with \
        the transition so it may not be done if the CPU is servicing interrupts. \
        This control parameter defines the maximum number of interrupts that \
        may be serviced within a sample period before it is prevented from \
        putting the system into C3.
    TYPE        UINT32
    DEFAULT     4
}

Parameter  CPU_PWR_POLICY_SLEEP_HEADROOM_THRESH {
    NAME   Sleep headroom threshold
    SYNOPSIS    This is the maximum CPU utilization permitted before \
        the processor can set the C3 processor power state. (percentage, range 1..99)
    TYPE        UINT32
    DEFAULT     2
}

Parameter  CPU_PWR_POLICY_PSTATE_CAPABILITY {
    NAME   P-State capability bitmask
    SYNOPSIS    This is the P-State capabilities supported by the policy manager \
        generally P0 through P12. (1111111111111)
    TYPE        UINT32
    DEFAULT     0x1FFF
}

Parameter  CPU_PWR_POLICY_CSTATE_CAPABILITY {
    NAME   C-State capability bitmask
    SYNOPSIS     This is the C-State capabilities supported by the policy manager \
        normally C0, C1, C3. (1011)
    TYPE        UINT32
    DEFAULT     0xB
}

/*******************************************************************************
*
* Cpu Load Monitor and Event Generator
*
*/

Component   INCLUDE_PWR_CPU_MON {
    NAME        CPU Load Monitor
    SYNOPSIS    CPU Load Monitor
    MODULES     pwrCpuMonLib.o
    _CHILDREN   FOLDER_PWR_MGMT
    _INIT_ORDER usrRoot
    INIT_AFTER  INCLUDE_PWR_CPU_MON_ARCH
    INIT_BEFORE INCLUDE_CPU_PWR_MGMT
    PROTOTYPE   STATUS pwrCpuMonLibInit (void);
    INIT_RTN    pwrCpuMonLibInit();
}

/*******************************************************************************
*
* Idle Power Management (Off, Short, Long/Tickless Idle)
*
*/

Selection SELECT_POWER_MGMT {
    NAME            Idle Power Management Operating Mode
    SYNOPSIS        Default behavior when image boots (select one)
    COUNT           1-1
    CHILDREN        INCLUDE_CPU_SLEEP_MODE_OFF \
                    INCLUDE_CPU_SLEEP_MODE_SHORT \
                    INCLUDE_CPU_SLEEP_MODE_LONG
    DEFAULTS        INCLUDE_CPU_SLEEP_MODE_LONG
    REQUIRES        INCLUDE_CPU_SLEEP_MODE_LONG_PARAMS \
                    INCLUDE_IDLE_PWR_MGMT_INIT \
                    INCLUDE_PWR_LIB_FUNCS \
                    INCLUDE_TICKLESS_IDLE
}

/* The following INCLUDE_TICKLESS_IDLE component overrides the one in 00bsp.cdf */

Component INCLUDE_TICKLESS_IDLE {
    NAME            Tickless-idle component 
    SYNOPSIS        Reduce timer interrupts when in idle 
    INIT_RTN        pwrTicklessOneShotInit (); pwrTicklessEnable ();
    PROTOTYPE       void pwrTicklessOneShotInit (void); extern void pwrTicklessEnable (void);
    REQUIRES        SELECT_POWER_MGMT
}

Component INCLUDE_IDLE_PWR_MGMT_INIT {
    NAME            Idle Power Management Initialization
    SYNOPSIS        Idle Power Management Initialization
    _INIT_ORDER     usrRoot
    CONFIGLETTES    $(TGT_DIR)/src/hwif/pwr/pwrTickless.c
    INIT_RTN        pwrTicklessInit();
    INIT_AFTER      INCLUDE_DEV_PWR_MGMT
    INIT_BEFORE     INCLUDE_CPU_ODG_POLICY_MGMT
    HDR_FILES       ../h/hwif/pwr/pwrTickless.h
    _CHILDREN       FOLDER_NOT_VISIBLE
    REQUIRES        SELECT_POWER_MGMT
}

Component INCLUDE_CPU_SLEEP_MODE_LONG_PARAMS {
    NAME            Idle Power Management Tickless Mode Configuration
    SYNOPSIS        Tickless operating mode parameters
    _CHILDREN       FOLDER_PWR_MGMT
    CFG_PARAMS      CPU_SLEEP_MODE_LONG_THRESHOLD \
                    CPU_SLEEP_MODE_LONG_MAXIMUM \
                    CPU_SLEEP_MODE_LONG_OFFSET \
                    CPU_SLEEP_MODE_LONG_LATENCY_C3
}

Parameter CPU_SLEEP_MODE_LONG_THRESHOLD {
    NAME            Idle Power Management Tickless - Threshold
    SYNOPSIS        Reduces clock timer interrupts and idle-off \
                    when the next forecast OS event occurs beyond \
                    this threshold number of system ticks. \
                    (ticks, range 1..0xFFFFFFFF)
    TYPE            uint
    DEFAULT         1
}

Parameter CPU_SLEEP_MODE_LONG_MAXIMUM {
    NAME            Idle Power Management Tickless - Maximum
    SYNOPSIS        Maximum tickless idle period in ticks. \
                    (ticks, range 1..0xFFFFFFFF)
    TYPE            uint
    DEFAULT         0xffffffff
}

Parameter CPU_SLEEP_MODE_LONG_OFFSET {
    NAME            Idle Power Management Tickless - Offset
    SYNOPSIS        Subtract this number of ticks from calculated idle number of \
                    ticks.  This can be used to account for things that might \
                    take some time to come out of tickless idle mode. \
                    (ticks, range 0..0xFFFFFFFF)
    TYPE            uint
    DEFAULT         0
}

/*
 * Latency values that can be added as needed to CDF file.
 * Valid parameters are CPU_SLEEP_MODE_LONG_LATENCY_C2,
 * CPU_SLEEP_MODE_LONG_LATENCY_C3, etc. up to
 * CPU_SLEEP_MODE_LONG_LATENCY_C31.
 * To create a parameter for another C state entry, copy the following entry
 * and change C3's to another C state number.  Then modify the default value.
 * Finally, add the newly created paramter to the CFG_PARAMS field of
 * INCLUDE_CPU_SLEEP_MODE_LONG_PARAMS component above.
 * C state latency defaults to zero if the latency parameter is not
 * specified for the C state in a CDF file.
 */

Parameter CPU_SLEEP_MODE_LONG_LATENCY_C3 {
    NAME            Idle Power Management Tickless - C3 State Latency in Ticks
    SYNOPSIS        Maximum latency to enter and exit C3 state. \
                    (ticks, range 0..0xFFFFFFFF)
    TYPE            uint
    DEFAULT         0
}

#else  /* _WRS_CONFIG_PWR_MGMT */

Component INCLUDE_PWR_LIB_FUNCS {
    NAME            Power Management Library Functions
    SYNOPSIS        Power Management Library Functions
    STATUS_DETAILS this component is disabled because VSB option \
                _WRS_CONFIG_PWR_MGMT is not selected
    REQUIRES    COMPONENT_NOT_SUPPORTED
}

Component INCLUDE_CPU_PWR_MGMT {
    NAME            CPU Power Management
    SYNOPSIS        CPU Power Management
    STATUS_DETAILS this component is disabled because VSB option \
                _WRS_CONFIG_PWR_MGMT is not selected
    REQUIRES    COMPONENT_NOT_SUPPORTED
}

Component INCLUDE_PWR_POLICY_MGR {
    NAME            Power Policy Manager
    SYNOPSIS        Power Policy Manager
    STATUS_DETAILS this component is disabled because VSB option \
                _WRS_CONFIG_PWR_MGMT is not selected
    REQUIRES    COMPONENT_NOT_SUPPORTED
}

Component INCLUDE_PWR_CPU_MON {
    NAME        CPU Load Monitor
    SYNOPSIS    CPU Load Monitor
    STATUS_DETAILS this component is disabled because VSB option \
                _WRS_CONFIG_PWR_MGMT is not selected
    REQUIRES    COMPONENT_NOT_SUPPORTED
}

Component INCLUDE_TICKLESS_IDLE {
    NAME            Tickless-idle component 
    SYNOPSIS        Reduce timer interrupts when in idle 
    STATUS_DETAILS this component is disabled because VSB option \
                _WRS_CONFIG_PWR_MGMT is not selected
    REQUIRES    COMPONENT_NOT_SUPPORTED
}

Component INCLUDE_IDLE_PWR_MGMT_INIT {
    NAME            Idle Power Management Initialization
    SYNOPSIS        Idle Power Management Initialization
    STATUS_DETAILS this component is disabled because VSB option \
                _WRS_CONFIG_PWR_MGMT is not selected
    REQUIRES    COMPONENT_NOT_SUPPORTED
}

Component INCLUDE_CPU_SLEEP_MODE_LONG_PARAMS {
    NAME            Idle Power Management Tickless Mode Configuration
    SYNOPSIS        Tickless operating mode parameters
    STATUS_DETAILS this component is disabled because VSB option \
                _WRS_CONFIG_PWR_MGMT is not selected
    REQUIRES    COMPONENT_NOT_SUPPORTED
}

#endif /* _WRS_CONFIG_PWR_MGMT */
