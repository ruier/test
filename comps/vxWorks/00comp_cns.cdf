/* 00comp_cns.cdf - Component Notification System (CNS) configuration file */

/* Copyright 2006 Wind River Systems, Inc. */

/* 
modification history 
--------------------
01b,30jan06,xli  change the component name and added REQUIRES clause
01a,02dec06,tkf  written.
*/
 
 /*

DESCRIPTION
  This file contains descriptions for the CNS component.

*/

Component INCLUDE_CNS
	{
	NAME		Wind River Notification System
	SYNOPSIS	Wind River Component Notification System (CNS) Component
        CONFIGLETTES    ssi/usrCns.c
	INIT_RTN	usrCnsInit();
        REQUIRES        INCLUDE_SEM_BINARY_CREATE \
                        INCLUDE_SEM_COUNTING_CREATE \
                        INCLUDE_MSG_Q_CREATE_DELETE
}

