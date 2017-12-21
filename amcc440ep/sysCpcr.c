/* sysCpcr.c - 440GX Clocking & Power and System Device access routines */

/*
**************************************************************************
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
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M"

****************************************************************************
\NOMANUAL
*/

/*
 * Copyright (c) 2002-2004, 2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

#include "copyright_wrs.h"

/*
modification history
--------------------
01f,02sep08,x_f  Remove sysDcr.h include, use dcr4xxALib.s in lib instead
01e,16nov04,dr   Including config.h for sharing file with bamboo.
01d,30aug04,mdo  Documentation fixes for apigen
01c,13aug03,jtp  Formal code inspection changes: documentation
01b,29may03,pch  partitioning for compressed bootrom
		 add #include files to enable standalone build
01a,10dec02,saw  Created
*/

/*
DESCRIPTION
This file provides access functions for the Chip Control Registers   

This is relocatable code.  The initial bootrom code is typically loaded
at a different address than it was linked.  The only memory facility
available to these routines is the stack, initialized to the PPC440's
onchip SRAM.  For these reasons, all code in this module must be
position-independent, make sparing use only of auto (stack) variables,
and must avoid use of data and BSS memory and static variables.
Finally, compiler output must be inspected to make sure that no jump
tables with absolute branches have been generated; switch statements and
if-else if-... statements are the leading culprits.

INCLUDE FILES:
*/

/* includes */

#include "vxWorks.h"
#include "intLib.h"
#include "cprDcr.h"
#include "sdrDcr.h"
#include "config.h"

/***************************************************************************
*
* sysCprOutLong - write value to CPR register
*
* This routine writes 32-bits of data to the specified CPR register.
*
* RETURNS: N/A
*
* ERRNO
*/

void sysCprOutLong
    (
    UINT32 cprReg, 
    UINT32 cprValue
    )
    {
    UINT32 level;
    UINT32 cprCfgaddrTemp;

    level = intLock();
    
    cprCfgaddrTemp = dcrInLong(CPR0_CFGADDR);
    
    dcrOutLong(CPR0_CFGADDR, cprReg);
    
    dcrOutLong(CPR0_CFGDATA, cprValue);
    
    dcrOutLong(CPR0_CFGADDR, cprCfgaddrTemp);
    
    intUnlock(level);

    return;
    }

/***************************************************************************
*
* sysCprInLong - read value from CPR register
*
* This routine reads 32-bits of data from the specified CPR register.
*
* RETURNS: reg value
*
* ERRNO
*/

UINT32 sysCprInLong
    (
    UINT32 cprReg
    )
    {
    UINT32 level;
    UINT32 cprCfgaddrTemp;
    UINT32 cprValue;

    level = intLock();
    
    cprCfgaddrTemp = dcrInLong(CPR0_CFGADDR);
    
    dcrOutLong(CPR0_CFGADDR, cprReg);
    
    cprValue = dcrInLong(CPR0_CFGDATA);
    
    dcrOutLong(CPR0_CFGADDR, cprCfgaddrTemp);
    
    intUnlock(level);

    return(cprValue);
    }

/***************************************************************************
*
* sysSdrOutLong - write value to SDR register
*
* This routine writes 32-bits of data to the specified SDR register.
*
* RETURNS: N/A
*
* ERRNO
*/

void sysSdrOutLong
    (
    UINT32 sdrReg, 
    UINT32 sdrValue
    )
    {
    UINT32 level;
    UINT32 sdrCfgaddrTemp;

    level = intLock();

    sdrCfgaddrTemp = dcrInLong(SDR0_CFGADDR);
    
    dcrOutLong(SDR0_CFGADDR, sdrReg);
    
    dcrOutLong(SDR0_CFGDATA, sdrValue);
    
    dcrOutLong(SDR0_CFGADDR, sdrCfgaddrTemp);
    
    intUnlock(level);

    return;
    }

/***************************************************************************
*
* sysSdrInLong - read value from SDR register
*
* This routine reads 32-bits of data from the specified SDR register.
*
* RETURNS: reg value
*
* ERRNO
*/

UINT32 sysSdrInLong
    (
    UINT32 sdrReg
    )
    {
    UINT32 level;
    UINT32 sdrCfgaddrTemp;
    UINT32 sdrValue;

    level = intLock();
    
    sdrCfgaddrTemp = dcrInLong(SDR0_CFGADDR);
    
    dcrOutLong(SDR0_CFGADDR, sdrReg);
    
    sdrValue = dcrInLong(SDR0_CFGDATA);
    
    dcrOutLong(SDR0_CFGADDR, sdrCfgaddrTemp);
    
    intUnlock(level);

    return(sdrValue);
    }

