/* 01sysctl.cdf - sysctl Component configuration file */

/*
 * Copyright (c) 2004, 2006, 2009-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement.
 */

/*
modification history
--------------------
01j,18aug10,pee  update init order for INCLUDE_SYSCTL
01i,17aug09,cww  Use usrRtpInit in place of INCLUDE_RTP for orderings
01h,01dec06,tlu  Added forward prototype definition for sysctlScLibInit()
01g,11nov04,job  Added INCLUDE_SC_SYSCTL to usrScInit init group
01f,29jun04,job  INCLUDE_SYSCTL_HW now has requirement on INCLUDE_SYSCTL
01e,28jun04,job  renamed INCLUDE_SYSCTL_API to INCLUDE_SYSCTL. Added new
                 component INCLUDE_SYSCTL_HW
01d,14jun04,job  renamed usrSysctl2Init to usrSysctlInit
01c,11jun04,rec  fix merge conflicts
01b,23may04,dbt  Added use of usrSysctl2.c
01a,20may03,job  created from 00comp_net_sysctl.cdf
*/

Folder FOLDER_SYSCTL {
        NAME            SYSCTL Component
        _CHILDREN       FOLDER_OS
        CHILDREN        INCLUDE_SYSCTL \
                        INCLUDE_SYSCTL_CLI
        DEFAULTS        INCLUDE_SYSCTL
}

Component INCLUDE_SC_SYSCTL {
        NAME            sysctl System Call
        SYNOPSIS        System call support for sysctl()
        INIT_RTN        sysctlScLibInit ();
	PROTOTYPE	STATUS sysctlScLibInit (void);
        MODULES         sysctlScLib.o
        INCLUDE_WHEN    INCLUDE_SYSCTL INCLUDE_RTP
        REQUIRES        INCLUDE_SYSCTL INCLUDE_RTP
	_CHILDREN	FOLDER_NOT_VISIBLE
        _INIT_ORDER     usrScInit
}

Component INCLUDE_SYSCTL_CLI {
	NAME		SYSCTL CLI
	SYNOPSIS	Command Line SYSCTL 
	INIT_RTN	sysctlLibInit();
	MODULES		sysctl.o
        REQUIRES        INCLUDE_SYSCTL
        _INIT_ORDER     usrRoot
        INIT_BEFORE     INCLUDE_USER_APPL
}

Component INCLUDE_SYSCTL {
	NAME		SYSCTL
	SYNOPSIS	System control function
	HDR_FILES       sysctlLib.h
	CONFIGLETTES    usrSysctl.c
	INIT_RTN	usrSysctlInit();
	MODULES         kern_mib.o \
	                kern_sysctl.o
}

Component INCLUDE_SYSCTL_HW {
	NAME		SYSCTL H/W
	SYNOPSIS	System control hardware info registration
	HDR_FILES       sysctlLib.h
	CONFIGLETTES    usrHwSysctl.c
        _INIT_ORDER     usrRoot
        INIT_BEFORE     usrRtpInit
	INIT_AFTER      INCLUDE_SYSCTL
        INCLUDE_WHEN    INCLUDE_SYSCTL INCLUDE_RTP
	REQUIRES        INCLUDE_SYSCTL
	INIT_RTN	usrHwSysctlInit();
}
