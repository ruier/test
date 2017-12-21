/* 02comp_sal_rtp.cdf - Component configuration file for RTP SNS */

/*
 * Copyright (c) 2003-2006 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01l,31aug06,mze  add requires INCLUDE_SAL_SERVER to INCLUDE_SNS_MP_RTP and 
                 INCLUDE_SNS_RTP to remove build issue  WIND00063884
01k,05mar06,ebh  add SYNOPSIS to all entries including directories SPR #117960
01j,10may05,ebh  allow user-defined distributed SNS server TIPC type +
                 instance
		 remove synopsis fields that duplicate name fields
01i,20apr05,ebh  added support for distributed SAL
01h,01mar05,kkz  Removing single quote
01g,01mar05,kkz  removed single quote from synopsis
01f,13jan05,bwa  added SNS_LISTEN_BACKLOG parameter (SPR#102955).
01e,13sep04,bwa  changed INCLUDE_SAL_RTP_HOOK to INCLUDE_SNS_RTP_HOOK.
01d,03sep04,ymz  Added dependency on INCLUDE_SNS_RTP_HOOK
01c,24apr04,p_r  fixed dsi pathnames
01b,08dec03,p_r  added COMP dependency and changed pathname.
01a,17nov03,p_r  written.
*/

/* 
DESCRIPTION

This file contains  description for a component used when SNS (Socket Name 
Service) is supposed to run in user space.
For more details on SNS and interaction with the Socket Application Library, 
please read the 00comp_net_sal.cdf file.
Only one SNS instance should run in the a node. Therefore, if no RTP is 
available the default would be the kernel. If RTP is available the SNS must 
run in a separate RTP.
This component is made of a simple configlette, which launches the sns daemon
as an RTP executable.
It assumes the executable is in a known location, specified by the SNS_PATHNAME
parameter. The SNS_PRIORITY and SNS_STACK_SIZE parameters should normally use
their default values.
The location is in a ROMFS file system.
*/

Component INCLUDE_SNS_RTP {
	NAME		Socket Name Service in RTP
	SYNOPSIS	The Socket Name Service server resides in RTP space, AF_LOCAL support only
	CONFIGLETTES    dsi/usrSalSnsRtp.c
	INIT_RTN	salSnsRtpInit ();
	CFG_PARAMS      SNS_PATHNAME   \
	                SNS_PRIORITY   \
	                SNS_STACK_SIZE \
                        SNS_LISTEN_BACKLOG
	EXCLUDES        INCLUDE_SNS \
			INCLUDE_SNS_MP_RTP \
			INCLUDE_SNS_MP
	REQUIRES	INCLUDE_SAL_SERVER \
			INCLUDE_RTP     \
			INCLUDE_SNS_RTP_HOOK \
			INCLUDE_UN_COMP \
			INCLUDE_ROMFS
}

Component INCLUDE_SNS_MP_RTP {
	NAME		Distributed Socket Name Service in RTP
	SYNOPSIS	The Distributed Socket Name Service server resides in RTP space, includes AF_LOCAL and AF_TIPC support
	CONFIGLETTES    dsi/usrSalSnsRtp.c
	INIT_RTN	salSnsRtpInit ();
	CFG_PARAMS      SNS_PATHNAME   \
	                SNS_PRIORITY   \
	                SNS_STACK_SIZE \
                        SNS_LISTEN_BACKLOG \
			SNS_DISTRIBUTED_SERVER_TYPE \
			SNS_DISTRIBUTED_SERVER_INSTANCE
	EXCLUDES        INCLUDE_SNS \
			INCLUDE_SNS_RTP \
			INCLUDE_SNS_MP
	REQUIRES	INCLUDE_SAL_SERVER \
			INCLUDE_RTP     \
			INCLUDE_SNS_RTP_HOOK \
			INCLUDE_UN_COMP \
			INCLUDE_TIPC \
			INCLUDE_ROMFS
}

Parameter SNS_PATHNAME
        {
        NAME            Socket Name Service pathname
	SYNOPSIS	Default path: /romfs/snsServer.vxe
	TYPE            string
        DEFAULT         "/romfs/snsServer.vxe"
        }

Parameter SNS_PRIORITY
        {
        NAME            Socket Name Service execution priority 
	SYNOPSIS	Recommended priority value: 50
	TYPE            int
        DEFAULT         50
        }

Parameter SNS_STACK_SIZE
        {
        NAME            Socket Name Service task stack size
	SYNOPSIS	Recommended stack size: 20000
	TYPE            int
        DEFAULT         20000
        }

Parameter SNS_LISTEN_BACKLOG
        {
        NAME            Socket Name Service listen sockets backlog size
        SYNOPSIS        Increase if connections get refused.  Default:  5
        TYPE            int
        DEFAULT         5
        }

Parameter SNS_DISTRIBUTED_SERVER_TYPE
        {
        NAME            Distributed Socket Name Service TIPC Type
        SYNOPSIS        Forms TIPC type,instance of SNS server.  Default Type:  70
        TYPE            int
        DEFAULT         70
        }

Parameter SNS_DISTRIBUTED_SERVER_INSTANCE
        {
        NAME            Distributed Socket Name Service TIPC Instance
        SYNOPSIS        Forms TIPC type,instance of SNS server.  Default Instance:  1
        TYPE            int
        DEFAULT         1
        }

