/* sysCpcr.h - IBM 440GX Clocking & Power and System Device access header */

/*
********************************************************************************
   This source and object code has been made available to you by IBM on an
   AS-IS basis.

   IT IS PROVIDED WITHOUT WARRANTY OF ANY KIND, INCLUDING THE WARRANTIES OF
   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE OR OF NONINFRINGEMENT
   OF THIRD PARTY RIGHTS.  IN NO EVENT SHALL IBM OR ITS LICENSORS BE LIABLE
   FOR INCIDENTAL, CONSEQUENTIAL OR PUNITIVE DAMAGES.  IBM'S OR ITS LICENSOR'S
   DAMAGES FOR ANY CAUSE OF ACTION, WHETHER IN CONTRACT OR IN TORT, AT LAW OR
   AT EQUITY, SHALL BE LIMITED TO A MAXIMUM OF $1,000 PER LICENSE.  No license
   under IBM patents or patent applications is to be implied by the copyright
   license.

   Any user of this software should understand that neither IBM nor its
   licensors will be responsible for any consequences resulting from the use
   of this software.

   Any person who transfers this source code or any derivative work must
   include the IBM copyright notice, this paragraph, and the preceding two
   paragraphs in the transferred software.

   Any person who transfers this object code or any derivative work must
   include the IBM copyright notice in the transferred software.

   COPYRIGHT   I B M   CORPORATION 2002
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M

********************************************************************************
\NOMANUAL
*/

/* Copyright 1984-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01c,29sep04,mdo  Documentation fixes for apigen
01b,13aug03,jtp  Formal code inspection changes: documentation
01a,15dec02,saw  created
*/

/*
This file contains the definitions of input and output functions for
accessing Chip Control Registers.  
*/

#ifndef INCsysCpcrh
#define INCsysCpcrh

#ifdef __cplusplus
    extern "C" {
#endif

#ifndef _ASMLANGUAGE

UINT32	sysCprInLong(UINT32);
void	sysCprOutLong(UINT32,UINT32);
UINT32	sysSdrInLong(UINT32);
void	sysSdrOutLong(UINT32,UINT32);

#endif /* _ASMLANGUAGE */

#ifdef __cplusplus
    }
#endif

#endif	/* INCsysCpcrh */
