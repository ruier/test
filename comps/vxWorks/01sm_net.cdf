/* 01sm_net.cdf - shared memory network components and parameters */

/* 
 * Copyright (c) 2006 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,05nov09,zhw  move FOLDER_SM_NET to FOLDER_OS (WIND00188643)
01b,06nov06,mze  modified to work with ipnet release
01a,19sep06,mze  written to add INCLUDE_SM_NET support WIND00058841
*/

/*
DESCRIPTION
  This file contains the description for the VxWorks shared memory network component.
*/

/*
 *      Even if "sm" is not used as the boot device, a backplane
 *      device is still created using a variety of scenarios, depending on 
 *      whether proxy arp and/or sequential addressing are selected.
 *
 *      If "sm" is used as the boot device and DHCP is included with
 *      that flag set, the network initialization must return ERROR unless
 *      the proxy flag is also set.
 *
 *      Whether or not the device was booted over the backplane, the network
 *      initialization must create a proxy server when that component is
 *      included and INCLUDE_SM_NET is defined. It must also initialize
 *      the shared memory network show routines if included.
 *
 */
 
Folder FOLDER_SM_NET
	{
	NAME		Shared memory network components
	SYNOPSIS	Shared memory network components and facilities
        DEFAULT         INCLUDE_SM_NET
        _CHILDREN	FOLDER_OS
	CHILDREN	INCLUDE_SM_NET \
                        INCLUDE_SM_NET_SHOW
	}
        
Component INCLUDE_SM_NET
	{
	NAME 		Shared memory network initialization
	SYNOPSIS 	Creates a device accessing backplane like a network 
        _CHILDREN	FOLDER_SM_NET
	REQUIRES        INCLUDE_SM_COMMON \
                        INCLUDE_SMEND \
                        INCLUDE_IPATTACH    
       	CFG_PARAMS	SM_PKTS_SIZE \
			SM_MAX_PKTS
	HDR_FILES       drv/end/smEnd.h
        CONFIGLETTES 	usrSmNet.c net/coreip/usrNetBoot.c net/coreip/usrNetBootConfig.c \
                        net/coreip/usrNetIpAttachCommon.c
        INIT_RTN	usrSmNetInit ();
	} 
        
Component INCLUDE_SMEND
        {
        NAME            SMEND 
        SYNOPSIS        Required for INCLUDE_SM_NET
        _CHILDREN	FOLDER_NOT_VISIBLE
        } 
                
Parameter SM_MAX_PKTS {
	NAME		Max # of shared memory packets 
        SYNOPSIS	Max # of shared memory packets to support (default 200)
	TYPE		uint
	DEFAULT		200
}

Parameter SM_PKTS_SIZE {
	NAME            Shared memory packet size
        SYNOPSIS	Shared memory packet size, 0 = DEFAULT_PKTS_SIZE
	TYPE		uint
	DEFAULT		0
}

Component INCLUDE_SM_NET_SHOW
	{
	NAME		Shared memory network driver show routines
	SYNOPSIS	Initializes the shared memory network driver show facility
        _CHILDREN	FOLDER_SM_NET
        REQUIRES	INCLUDE_SM_NET
        }


        
