/* sdramDcr.s - IBM SDRAM controller DCR access assembly routines */

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

   COPYRIGHT   I B M   CORPORATION 2000
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M

********************************************************************************
\NOMANUAL
*/

/*
modification history
--------------------
01f,03sep04,mdo  Documentation fixes for apigen
01e,30nov01,kab  Update for T2.2
01d,19nov01,pch  cleanup
01c,08may01,pch  Add assembler abstractions (FUNC_EXPORT, FUNC_BEGIN, etc.)
01b,15may00,mcg  register name updates to match 405GP User Manual
01a,17jan00,mcg  created
*/

/*
DESCRIPTION
This module contains chip-dependent routines written in assembly language.
*/

#define _ASMLANGUAGE

        /* globals */

        FUNC_EXPORT(sysDcrSdramGet)

	_WRS_TEXT_SEG_START

/*******************************************************************************
*
* sysDcrSdramGet - Get the contents of an SDRAM controller DCR register
*
* This routine reads the contents of the specified SDRAM controller register.
*
* SYNOPSIS
* \ss
* UINT32 sysDcrSdramGet(UINT32 sdramReg)
* \se
*
* RETURNS: Contents of argument SDRAM register
*/

FUNC_BEGIN(sysDcrSdramGet)

        mtdcr   SDRAM_CFGADDR, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif

        mfdcr   r3, SDRAM_CFGDATA
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrSdramGet)

