/* ppcProc.h - PowerPC Processor Version and Revision Definitions */

/*
 * Copyright (c) 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2003-2005 Motorola Inc. All Rights Reserved */

/*
modification history
--------------------
01e,10sep09,l_z  Fix typo error. (WIND00162041).
01d,31jan05,cak  Ported from 01c, mv6100/ppcProc.h.
*/

/*
This file contains the version and revision definitions for the 
PowerPC family of processors, as found in the SVR (System Version
Register).  Also contained here are the macros used to extract the
version and revision values from the SVR.
*/

#ifndef __INCppcProch
#define INCppcProch

#ifdef __cplusplus
   extern "C" {
#endif

/* CPU Version Macros */

#define CPU_TYPE	((CCSR_READ32(CCSBAR, CCSR_SVR) >> 16) & 0xffff)

/* CPU Versions */

#define CPU_TYPE_MPC8540	0x8030	/* MPC8540 PowerQUICC III */

#ifdef __cplusplus
    }
#endif

#endif /* __INCppcProch */
