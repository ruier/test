/* 00comp_net_sc_socklib.cdf - Component configuration file */

/* Copyright 2003-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01c,01dev06,tlu  Added forward prototyping for sockScLibInit()
01b,11nov04,job  Added INCLUDE_SC_SOCKLIB to usrScInit init group
01a,02dec03,vvv  written
*/

Component INCLUDE_SC_SOCKLIB {
	NAME		Socket API System Call support
	SYNOPSIS	System call support for socket API
	INIT_RTN	sockScLibInit ();
	PROTOTYPE	STATUS sockScLibInit (void);
	MODULES		sockScLib.o
	INCLUDE_WHEN    INCLUDE_SOCKLIB INCLUDE_RTP
	REQUIRES        INCLUDE_SOCKLIB INCLUDE_RTP
	_INIT_ORDER     usrScInit
}
