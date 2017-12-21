/* 00cpuPwrMgmt.cdf - CPU Power management component description file */

/*
Copyright 2007,2008,2010,2011 Wind River Systems, Inc.

modification history
--------------------
01d,22aug11,rbc  WIND00294339 - Fix build error from moving cpuPwrLightLib.c
01c,12mar10,rec  CPU performance management update
01b,26aug08,jpb  Removed LAYER from components.
01a,21jun07,mmi  Created based on ../ppc/00cpuPwrMgmt.cdf 01a 

DESCRIPTION

This file contains the CPU power Management components.
*/

/* CPU Power managment  */

#ifdef _WRS_CONFIG_PWR_MGMT

Component INCLUDE_CPU_LIGHT_PWR_MGR {
    NAME        Light CPU Power Management 
    SYNOPSIS    Idle-halt CPU power management
    STATUS_DETAILS this component is disabled because VSB option \
                _WRS_CONFIG_PWR_MGMT is selected
    REQUIRES    COMPONENT_NOT_SUPPORTED
}

#else /* !_WRS_CONFIG_PWR_MGMT */

Folder  FOLDER_CPU_PWR_MGMT {
    NAME        Power Management
    SYNOPSIS    Power Management Component
    _CHILDREN   FOLDER_OS
    CHILDREN    INCLUDE_CPU_LIGHT_PWR_MGR
    DEFAULTS    INCLUDE_CPU_LIGHT_PWR_MGR
}

Component INCLUDE_CPU_LIGHT_PWR_MGR {
    NAME        Light CPU Power Management 
    SYNOPSIS    Idle-halt CPU power management
    SOURCE      $(TGT_DIR)/src/hwif/pwr
    MODULES     cpuPwrLightLib.o
    HDR_FILES   cpuPwrMgr.h
    INIT_RTN    cpuPwrLightMgrInit (); \
                cpuPwrMgrEnable (TRUE);
    EXCLUDES    INCLUDE_CPU_PERFORMANCE_MGMT
    HDR_FILES   cpuPwrMgr.h
}

#endif /* _WRS_CONFIG_PWR_MGMT */
