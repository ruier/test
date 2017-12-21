/* 00comp_ssi.cdf - Component configuration file */

/* Copyright 2006 Wind River Systems, Inc. */

/* 
modification history 
--------------------
01b,30jan06,xli  changed the component name and added REQUIRES clause
01a,02dec06,tkf  written.
*/

/*

DESCRIPTION
  This file contains descriptions for the SSI component.

*/

Component INCLUDE_SSI
	{
	NAME		Wind River Dynamic Initialization
	SYNOPSIS	Wind River Startup Sequence and Initialization (SSI) Component
        CONFIGLETTES    ssi/usrSsi.c
	INIT_RTN	usrSsiInit();
        REQUIRES        INCLUDE_CNS \
                        INCLUDE_SEM_BINARY_CREATE \
                        INCLUDE_SEM_COUNTING_CREATE \
                        INCLUDE_MSG_Q_CREATE_DELETE

	}

