/* 405gpDcr.s - IBM 405GP chip dependent DCR accesss assembly routines */

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
01i,03sep04,mdo  Documentation fixes for apigen
01h,30nov01,kab  Update for T2.2
01g,19nov01,pch  cleanup
01f,08may01,pch  Add assembler abstractions (FUNC_EXPORT, FUNC_BEGIN, etc.)
01e,30may00,mcg  added OCM, PLB, PLB arbiter, PLB-OPB register access functions
01d,15may00,mcg  register name updates to match 405GP User Manual
01c,19apr00,mcg  moved SDRAM function to its own file
01b,15jan00,mcg  moved MAL and UIC functions to their own files
01a,20sep99,mcg  created
*/

/*
DESCRIPTION
This module contains chip-dependent routines written in assembly language.
*/

#define _ASMLANGUAGE

        /* globals */

        FUNC_EXPORT(sysDcrCr0Get)
        FUNC_EXPORT(sysDcrCr0Set)
        FUNC_EXPORT(sysDcrCr1Get)
        FUNC_EXPORT(sysDcrCr1Set)
        FUNC_EXPORT(sysDcrPllmrGet)
        FUNC_EXPORT(sysDcrPsrGet)

        FUNC_EXPORT(sysDcrOcmisarcGet)
        FUNC_EXPORT(sysDcrOcmisarcSet)
        FUNC_EXPORT(sysDcrOcmiscntlGet)
        FUNC_EXPORT(sysDcrOcmiscntlSet)
        FUNC_EXPORT(sysDcrOcmdsarcGet)
        FUNC_EXPORT(sysDcrOcmdsarcSet)
        FUNC_EXPORT(sysDcrOcmdscntlGet)
        FUNC_EXPORT(sysDcrOcmdscntlSet)

        FUNC_EXPORT(sysDcrDcpcfgaddrSet)
        FUNC_EXPORT(sysDcrDcpcfgdataGet)
        FUNC_EXPORT(sysDcrDcpcfgdataSet)

        FUNC_EXPORT(sysDcrPlbbesrGet)
        FUNC_EXPORT(sysDcrPlbbesrClear)
        FUNC_EXPORT(sysDcrPlbbearGet)
        FUNC_EXPORT(sysDcrPlbacrGet)
        FUNC_EXPORT(sysDcrPlbacrSet)

        FUNC_EXPORT(sysDcrPobbesr0Get)
        FUNC_EXPORT(sysDcrPobbesr0Clear)
        FUNC_EXPORT(sysDcrPobbesr1Get)
        FUNC_EXPORT(sysDcrPobbesr1Clear)
        FUNC_EXPORT(sysDcrPobbearGet)

        _WRS_TEXT_SEG_START

/*******************************************************************************
*
* sysDcrCr0Get - get the contents of Chip Control Register 0
*
* This routine reads and returns the contents of Chip Control Register 0
*
* NOMANUAL

* UINT32 sysDcrCr0Get (void)

*/

FUNC_BEGIN(sysDcrCr0Get)

        mfdcr   r3, CPC0_CR0
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrCr0Get)

/*******************************************************************************
*
* sysDcrCr0Set - set the contents of Chip Control Register 0.
*
* This routine writes the contents of Chip Control Register 0.
*
* NOMANUAL

* void sysDcrCr0Set (UINT32)

*/

FUNC_BEGIN(sysDcrCr0Set)

        mtdcr   CPC0_CR0, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrCr0Set)

/*******************************************************************************
*
* sysDcrCr1Get - get the contents of Chip Control Register 0
*
* This routine reads and returns the contents of Chip Control Register 0
*
* NOMANUAL

* UINT32 sysDcrCr1Get (void)

*/

FUNC_BEGIN(sysDcrCr1Get)

        mfdcr   r3, CPC0_CR1
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrCr1Get)

/*******************************************************************************
*
* sysDcrCr1Set - set the contents of Chip Control Register 0.
*
* This routine writes the contents of Chip Control Register 0.
*
* NOMANUAL

* void sysDcrCr1Set (UINT32)

*/

FUNC_BEGIN(sysDcrCr1Set)

        mtdcr   CPC0_CR1, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrCr1Set)

/******************************************************************************* *
*
* sysDcrPllmrGet - get the contents of PLL Mode Register
*
* This routine reads and returns the contents of the PLL Mode Register.
*
* NOMANUAL

* UINT32 sysDcrPllmrGet (void)

*/

FUNC_BEGIN(sysDcrPllmrGet)

        mfdcr   r3, CPC0_PLLMR
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrPllmrGet)

/******************************************************************************* *
*
* sysDcrPsrGet - get the contents of Chip Strap register
*
* This routine reads and returns the contents of the Chip Strap register
*
* NOMANUAL

* UINT32 sysDcrPsrGet (void)

*/

FUNC_BEGIN(sysDcrPsrGet)

        mfdcr   r3, CPC0_PSR
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrPsrGet)

/*******************************************************************************
*
* sysDcrOcmisarcGet - get the contents of the on-chip memory instruction-side
*                     address range compare register
*
* This routine reads and returns the contents of the on-chip memory instruction-
* side address range compare register
*
* NOMANUAL

* UINT32 sysDcrOcmisarcGet (void)

*/

FUNC_BEGIN(sysDcrOcmisarcGet)

        mfdcr   r3, OCM_ISARC
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrOcmisarcGet)

/*******************************************************************************
*
* sysDcrOcmisarcSet - set the contents of the on-chip memory instruction-side
*                     address range compare register
*
* This routine writes the contents of the on-chip memory instruction-side
* address range compare register.
*
* NOMANUAL

* void sysDcrOcmisarcSet (UINT32)

*/

FUNC_BEGIN(sysDcrOcmisarcSet)

        mtdcr   OCM_ISARC, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrOcmisarcSet)

/*******************************************************************************
*
* sysDcrOcmiscntlGet - get the contents of the on-chip memory instruction-side
*                      control register
*
* This routine reads and returns the contents of the on-chip memory instruction-
* side control register
*
* NOMANUAL

* UINT32 sysDcrOcmiscntlGet (void)

*/

FUNC_BEGIN(sysDcrOcmiscntlGet)

        mfdcr   r3, OCM_ISCNTL
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrOcmiscntlGet)

/*******************************************************************************
*
* sysDcrOcmiscntlSet - set the contents of the on-chip memory instruction-side
*                      control register
*
* This routine writes the contents of the on-chip memory instruction-side
* control register.
*
* NOMANUAL

* void sysDcrOcmiscntlSet (UINT32)

*/

FUNC_BEGIN(sysDcrOcmiscntlSet)

        mtdcr   OCM_ISCNTL, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrOcmiscntlSet)

/*******************************************************************************
*
* sysDcrOcmdsarcGet - get the contents of the on-chip memory data-side
*                     address range compare register
*
* This routine reads and returns the contents of the on-chip memory data-
* side address range compare register
*
* NOMANUAL

* UINT32 sysDcrOcmdsarcGet (void)

*/

FUNC_BEGIN(sysDcrOcmdsarcGet)

        mfdcr   r3, OCM_DSARC
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrOcmdsarcGet)

/*******************************************************************************
*
* sysDcrOcmdsarcSet - set the contents of the on-chip memory data-side
*                     address range compare register
*
* This routine writes the contents of the on-chip memory data-side
* address range compare register.
*
* NOMANUAL

* void sysDcrOcmdsarcSet (UINT32)

*/

FUNC_BEGIN(sysDcrOcmdsarcSet)

        mtdcr   OCM_DSARC, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrOcmdsarcSet)

/*******************************************************************************
*
* sysDcrOcmdscntlGet - get the contents of the on-chip memory data-side
*                      control register
*
* This routine reads and returns the contents of the on-chip memory data-
* side control register
*
* NOMANUAL

* UINT32 sysDcrOcmdscntlGet (void)

*/

FUNC_BEGIN(sysDcrOcmdscntlGet)

        mfdcr   r3, OCM_DSCNTL
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrOcmdscntlGet)

/*******************************************************************************
*
* sysDcrOcmdscntlSet - set the contents of the on-chip memory data-side
*                      control register
*
* This routine writes the contents of the on-chip memory data-side
* control register.
*
* NOMANUAL

* void sysDcrOcmdscntlSet (UINT32)

*/

FUNC_BEGIN(sysDcrOcmdscntlSet)

        mtdcr   OCM_DSCNTL, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrOcmdscntlSet)

/*******************************************************************************
*
* sysDcrDcpcfgaddrSet - set the contents of the code decompression controller
*                       configuration address register
*
* This routine writes the contents of the code decompression controller
* configuration address register.
*
* NOMANUAL

* void sysDcrDcpcfgaddrSet (UINT32)

*/

FUNC_BEGIN(sysDcrDcpcfgaddrSet)

        mtdcr   DCP_CFGADDR, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDcpcfgaddrSet)

/*******************************************************************************
*
* sysDcrDcpcfgdataGet - get the contents of the code decompression controller
*                       configuration data register
*
* This routine reads and returns the contents of the code decompression
* controller configuration data register.
*
* NOMANUAL

* UINT32 sysDcrDcpcfgdataGet (void)

*/

FUNC_BEGIN(sysDcrDcpcfgdataGet)

        mfdcr   r3, DCP_CFGDATA
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDcpcfgdataGet)

/*******************************************************************************
*
* sysDcrDcpcfgdataSet - set the contents of the code decompression controller
*                       configuration data register
*
* This routine writes the contents of the code decompression controller
* configuration data register.
*
* NOMANUAL

* void sysDcrDcpcfgdataSet (UINT32)

*/

FUNC_BEGIN(sysDcrDcpcfgdataSet)

        mtdcr   DCP_CFGDATA, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDcpcfgdataSet)

/*******************************************************************************
*
* sysDcrPlbbesrGet - get the contents of the PLB error status register
*
* This routine reads and returns the contents of the PLB error status register.
*
* NOMANUAL

* UINT32 sysDcrPlbbesrGet (void)

*/

FUNC_BEGIN(sysDcrPlbbesrGet)

        mfdcr   r3, PLB_BESR
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrPlbbesrGet)

/*******************************************************************************
*
* sysDcrPlbbesrClear - clears pending status in the PLB error register
*
* This routine writes to the PLB error status register.  It will clear the
* status for each bit that is set to 1 in the input parameter.
*
* NOMANUAL

* void sysDcrPlbbesrClear (UINT32)

*/

FUNC_BEGIN(sysDcrPlbbesrClear)

        mtdcr   PLB_BESR, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrPlbbesrClear)

/*******************************************************************************
*
* sysDcrPlbbearGet - get the contents of the PLB error address register
*
* This routine reads and returns the contents of the PLB error address register.
*
* NOMANUAL

* UINT32 sysDcrPlbbearGet (void)

*/

FUNC_BEGIN(sysDcrPlbbearGet)

        mfdcr   r3, PLB_BEAR
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrPlbbearGet)

/*******************************************************************************
*
* sysDcrPlbacrGet - get the contents of the PLB arbiter configuration register
*
* This routine reads and returns the contents of the PLB arbiter configuration
* register.
*
* NOMANUAL

* UINT32 sysDcrPlbacrGet (void)

*/

FUNC_BEGIN(sysDcrPlbacrGet)

        mfdcr   r3, PLB_ACR
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrPlbacrGet)
	
/*******************************************************************************
*
* sysDcrPlbacrSet - set the contents of the PLB arbiter configuration register
*
* This routine writes the contents of the PLB arbiter configuration register.
*
* NOMANUAL

* void sysDcrPlbacrSet (UINT32)

*/

FUNC_BEGIN(sysDcrPlbacrSet)

        mtdcr   PLB_ACR, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrPlbacrSet)
	
/*******************************************************************************
*
* sysDcrPobbesr0Get - get the contents of the PLB to OPB bridge error status
*                     register 0
*
* This routine reads and returns the contents of the PLB to OPB bridge error
* status register 0
*
* NOMANUAL

* UINT32 sysDcrPobbesr0Get (void)

*/

FUNC_BEGIN(sysDcrPobbesr0Get)

        mfdcr   r3, POB_BESR0
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrPobbesr0Get)	

/*******************************************************************************
*
* sysDcrPobbesr0Clear - clears pending status in the PLB to OPB bridge error
*                       status register 0
*
* This routine writes to the PLB to OPB bridge error status register 0.  It will
* clear the status for each bit that is set to 1 in the input parameter.
*
* NOMANUAL

* void sysDcrPobbesr0Clear (UINT32)

*/

FUNC_BEGIN(sysDcrPobbesr0Clear)

        mtdcr   POB_BESR0, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrPobbesr0Clear)
	
/*******************************************************************************
*
* sysDcrPobbesr1Get - get the contents of the PLB to OPB bridge error status
*                     register 1
*
* This routine reads and returns the contents of the PLB to OPB bridge error
* status register 1
*
* NOMANUAL

* UINT32 sysDcrPobbesr1Get (void)

*/

FUNC_BEGIN(sysDcrPobbesr1Get)

        mfdcr   r3, POB_BESR1
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrPobbesr1Get)
	
/*******************************************************************************
*
* sysDcrPobbesr1Clear - clears pending status in the PLB to OPB bridge error
*                       status register 1
*
* This routine writes to the PLB to OPB bridge error status register 1.  It will
* clear the status for each bit that is set to 1 in the input parameter.
*
* NOMANUAL

* void sysDcrPobbesr1Clear (UINT32)

*/

FUNC_BEGIN(sysDcrPobbesr1Clear)

        mtdcr   POB_BESR1, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrPobbesr1Clear)
	
/*******************************************************************************
*
* sysDcrPobbearGet - get the contents of the PLB to OPB bridge error address
*                    register
*
* This routine reads and returns the contents of the PLB to OPB bridge error
* address register.
*
* NOMANUAL

* UINT32 sysDcrPobbearGet (void)

*/

FUNC_BEGIN(sysDcrPobbearGet)

        mfdcr   r3, POB_BEAR
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrPobbearGet)
