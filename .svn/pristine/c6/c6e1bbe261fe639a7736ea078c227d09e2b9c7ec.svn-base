/* ebcDcr.s - IBM external bus controller DCR access assembly routines */

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
01e,08jan02,pch  changed EBC_CFGADDR and EBC_CFGDATA to EBC0_CFGADDR and
                 EBC0_CFGDATA resp. for agreement with ebcDcr.h vn 01c
01d,30nov01,kab  Update for T2.2
01c,19nov01,pch  cleanup
01b,08may01,pch  Add assembler abstractions (FUNC_EXPORT, FUNC_BEGIN, etc.)
01a,30may00,mcg  created
*/

/*
DESCRIPTION
This module contains chip-dependent routines written in assembly language.
*/

#define _ASMLANGUAGE

        /* globals */

        FUNC_EXPORT(sysDcrEbcGet)
        FUNC_EXPORT(sysDcrEbcSet)

	_WRS_TEXT_SEG_START

/*******************************************************************************
*
* sysDcrEbcGet - Get the contents of an external bus controller DCR register
*
* This routine reads the contents of the specified external bus controller
* register.
*
*
* RETURNS: N/A

* UINT32 sysDcrEbcGet(UINT32 EbcReg)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrEbcGet)

        mtdcr   EBC0_CFGADDR, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif

        mfdcr   r3, EBC0_CFGDATA
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrEbcGet)

/*******************************************************************************
*
* sysDcrEbcSet - Set the contents of an external bus controller DCR register
*
* This routine writes the contents of the specified external bus controller
* register.
*
*
* RETURNS: N/A

* void sysDcrEbcSet(UINT32 EbcReg, UINT32 value)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrEbcSet)

        mtdcr   EBC0_CFGADDR, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif

        mtdcr   EBC0_CFGDATA, r4
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrEbcSet)

