/* uicDcr.s - IBM Universal Int Controller (UIC) DCR access assembly routines */

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
01f,03sep04,mdo  Documentation fixes for apgen
01e,30nov01,kab  Update for T2.2: asm and doc macros
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

        FUNC_EXPORT(sysDcrUiccrGet)
        FUNC_EXPORT(sysDcrUiccrSet)
        FUNC_EXPORT(sysDcrUicerGet)
        FUNC_EXPORT(sysDcrUicerSet)
        FUNC_EXPORT(sysDcrUicmsrGet)
        FUNC_EXPORT(sysDcrUicprGet)
        FUNC_EXPORT(sysDcrUicprSet)
        FUNC_EXPORT(sysDcrUicsrGet)
        FUNC_EXPORT(sysDcrUicsrClear)
        FUNC_EXPORT(sysDcrUictrGet)
        FUNC_EXPORT(sysDcrUictrSet)
        FUNC_EXPORT(sysDcrUicvrGet)
        FUNC_EXPORT(sysDcrUicvcrSet)

	_WRS_TEXT_SEG_START

/*******************************************************************************
*
* sysDcrUiccrGet - get the contents of the UIC Critical Register
*
* This routine reads and returns the contents of the UIC Critical Register.
*
* NOMANUAL
* \ss
* UINT32 sysDcrUiccrGet (void)
* \se
*
* RETURNS: value of UICCR
*/

FUNC_BEGIN(sysDcrUiccrGet)

        mfdcr   r3, UIC_CR
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrUiccrGet)

/*******************************************************************************
*
* sysDcrUiccrSet - set the contents of the UIC Critical Register
*
* This routine writes the contents of the UIC Critical Register.
*
* NOMANUAL
* \ss
* VOID sysDcrUiccrSet (UINT32 uiccr)
* \se
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysDcrUiccrSet)

        mtdcr   UIC_CR, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrUiccrSet)

/*******************************************************************************
*
* sysDcrUicerGet - get the contents of the UIC Interrupt Enable Register
*
* This routine reads and returns the contents of the UIC Interrupt Enable
* Register.
*
* NOMANUAL
* \ss
* UINT32 sysDcrUicerGet (void)
* \se
*
* RETURNS: 32-bit value UIC Interrupt Enable Register
*/

FUNC_BEGIN(sysDcrUicerGet)

        mfdcr   r3, UIC_ER
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrUicerGet)

/*******************************************************************************
*
* sysDcrUicerSet - set the contents of the UIC Interrupt Enable Register
*
* This routine writes the contents of the UIC Interrupt Enable register.
*
* NOMANUAL
* \ss
* VOID sysDcrUicerSet (UINT32 uicer)
* \se
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysDcrUicerSet)

        mtdcr   UIC_ER, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrUicerSet)

/*******************************************************************************
*
* sysDcrUicmsrGet - get the contents of the UIC Masked Status Register
*
* This routine reads and returns the contents of the UIC Masked Status 
* register.
*
* NOMANUAL
* \ss
* UINT32 sysDcrUicmsrGet (void)
* \se
*
* RETURNS: Value of UIC Masked Status Register
*/

FUNC_BEGIN(sysDcrUicmsrGet)

        mfdcr   r3, UIC_MSR
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrUicmsrGet)

/*******************************************************************************
*
* sysDcrUicprGet - get the contents of the UIC Polarity Register
*
* This routine reads and returns the contents of the UIC Polarity Register.
*
* NOMANUAL
* \ss
* UINT32 sysDcrUicprGet (void)
* \se
*
* RETURNS: Contents of UIC Polarity Register
*/

FUNC_BEGIN(sysDcrUicprGet)

        mfdcr   r3, UIC_PR
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrUicprGet)

/*******************************************************************************
*
* sysDcrUicprSet - set the contents of the UIC Polarity Register
*
* This routine writes the contents of the UIC Polarity Register.
*
* NOMANUAL
* \ss
* VOID sysDcrUicprSet (UINT32)
* \se
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysDcrUicprSet)

        mtdcr   UIC_PR, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrUicprSet)

/*******************************************************************************
*
* sysDcrUicsrGet - get the contents of the UIC Status Register
*
* This routine reads and returns the contents of the UIC Status Register.
*
* NOMANUAL
* \ss
* UINT32 sysDcrUicsrGet (void)
* \se
*
* RETURNS: 32-bit contents of the UIC Status Register
*/

FUNC_BEGIN(sysDcrUicsrGet)

        mfdcr   r3, UIC_SR
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrUicsrGet)

/*******************************************************************************
*
* sysDcrUicsrClear - clears pending interrupts in the UIC Status Register
*
* This routine writes to the UIC Status Register.  It will clear the interrupt
* status for each bit that is set to 1 in the input parameter.
*
* NOMANUAL
* \ss
* VOID sysDcrUicsrClear (UINT32 bitset)
* \se
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysDcrUicsrClear)

        mtdcr   UIC_SR, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrUicsrClear)

/*******************************************************************************
*
* sysDcrUictrGet - get the contents of the UIC Triggering Register
*
* This routine reads and returns the contents of the UIC Triggering Register.
*
* NOMANUAL
* \ss
* UINT32 sysDcrUictrGet (void)
* \se
*
* RETURNS: Contents of the UIC Triggering Register.
*/

FUNC_BEGIN(sysDcrUictrGet)

        mfdcr   r3, UIC_TR
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrUictrGet)

/*******************************************************************************
*
* sysDcrUictrSet - set the contents of the UIC Triggering Register
*
* This routine writes the contents of the UIC Triggering Register.
*
* NOMANUAL
* \ss
* VOID sysDcrUictrSet (UINT32)
* \se
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysDcrUictrSet)

        mtdcr   UIC_TR, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrUictrSet)

/*******************************************************************************
*
* sysDcrUicvrGet - get the contents of the UIC Vector Register
*
* This routine reads and returns the contents of the UIC Vector Register.
*
* NOMANUAL
* \ss
* UINT32 sysDcrUicvrGet (void)
* \se
*
* RETURNS: Contents of the UIC Vector Register.
*/

FUNC_BEGIN(sysDcrUicvrGet)

        mfdcr   r3, UIC_VR
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrUicvrGet)

/*******************************************************************************
*
* sysDcrUicvcrSet - set the contents of the UIC Vector Configuration Register
*
* This routine writes the contents of the UIC Vector Configuration Register.
*
* NOMANUAL
* \ss
* VOID sysDcrUicvcrSet (UINT32)
* \se
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysDcrUicvcrSet)

        mtdcr   UIC_VCR, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrUicvcrSet)

