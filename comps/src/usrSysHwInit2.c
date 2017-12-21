/* usrSysHwInit2.c - second phase hardware initialization configuration file */

/*
 * Copyright (c) 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,03jul09,x_s  created.
*/

/*
DESCRIPTION

This is the second phase of bsp hardware initialization configuration file for 
use with project builds.
*/

/* includes */

#include <sysLib.h>

/*******************************************************************************
*
* usrSysHwInit2 - second Phase of BSP hardware initialization
*
* This routine calls sysHwInit2 function to connect system interrupts and do any 
* additional configuration necessary.
*
* RETURNS: N/A
*/

void usrSysHwInit2 (void)
   {
#ifndef INCLUDE_SYSCLK_INIT
   sysHwInit2();
#endif
   }
