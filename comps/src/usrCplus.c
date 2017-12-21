/* usrCplus.c - cplus configuration file */

/* Copyright 2001 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,16nov04,j_b  provide type for sysCplusEnable (SPR 103688)
01a,03oct01,fk   written
*/

/*
DESCRIPTION

Initialize global variable sysCplusEnable 
*/


IMPORT BOOL sysCplusEnable;

void usrCplusLibInit(void)
    {
    sysCplusEnable = TRUE;
    cplusLibInit();             /* Initialize cplus library */
    }
