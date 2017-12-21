/* 10vbi.cdf - virtual board interface components */

/*
 * Copyright (c) 2009-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement.
 */

/*
modification history
--------------------
01f,02sep12,pfl  migrated MILS VDK to VBI
01e,30aug12,jpb  Renamed _WRS_CONFIG_MILS_VDK_1_1 to _WRS_CONFIG_MILS_VBI.
01d,30nov11,kab  MILS VDK support
01e,21sep11,c_t  add frame scheduler interrupt support for cert Hypervisor.
01d,01mar11,mze  fix for WIND00255132
01c,19mar10,v_r  No more configlette now that wrload access routines are
                 registered within wrhvWrload.c
01b,25feb10,rgo  Add a new hidden component for guestOS wrload access-routine
                 init.
01a,21jan09,to   written
*/

/*
DESCRIPTION
This file contains descriptions for the virtual board interface (VBI)
components. All components in this file are only visible for projects created
with guest OS support.
*/

#ifdef _WRS_CONFIG_WRHV_GUEST
#ifdef _WRS_CONFIG_MILS_VBI

Component INCLUDE_VB_SHOW {
	NAME		VB show routine
	MODULES		vbShow.o
	INIT_RTN	vbShowInit ();
        HDR_FILES       mils/vbi/vbi.h
	INCLUDE_WHEN	INCLUDE_SHOW_ROUTINES
	_CHILDREN	FOLDER_SHOW_ROUTINES
}

#else /* !MILS */
#ifndef _WRS_CONFIG_WRHV_GUEST_CERT
Component INCLUDE_VBI_SHOW {
	NAME		VBI show routines
	SYNOPSIS	virtual board interface (VBI) show routines
	MODULES		vbiShow.o
	LINK_SYMS	vbiConfigShow
	INCLUDE_WHEN	INCLUDE_SHOW_ROUTINES
	_CHILDREN	FOLDER_SHOW_ROUTINES
}

Component INCLUDE_WRHV_WRLOAD_INIT {
    NAME                WRLoad GuestOS Init for access-routines
    SYNOPSIS            WRLoad GuestOS init for access-routines
    MODULES             wrhvWrload.o
    _INIT_ORDER         usrToolsInit
    PROTOTYPE           void wrhvWrloadInit(void);
    INIT_RTN            wrhvWrloadInit();
    _CHILDREN           FOLDER_NOT_VISIBLE
    _REQUIRES           INCLUDE_WRLOAD
    REQUIRES            INCLUDE_WRLOAD
}
#endif /* _WRS_CONFIG_WRHV_GUEST_CERT */

#if defined(_WRS_CONFIG_WRHV_GUEST_CERT) ||\
    defined(_WRS_CONFIG_WRHV_GUEST_CERT_DEBUG)
Component INCLUDE_SCHED_INT_CALLBACK {
     NAME        Frame schedule interrupt callback
     SYNOPSIS    Guest OS frame interrupt support
    _CHILDREN    FOLDER_OS
}
#endif /* defined(_WRS_CONFIG_WRHV_GUEST_CERT)... */
#endif /* _WRS_CONFIG_MILS_VBI */
#endif /* _WRS_CONFIG_WRHV_GUEST */
