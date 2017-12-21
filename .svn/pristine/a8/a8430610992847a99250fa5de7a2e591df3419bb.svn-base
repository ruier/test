/* dmaDcr.s - IBM DMA controller DCR access assembly routines */

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
01e,03sep04,mdo  Documentation fixes for apigen
01d,30nov01,kab  Update for T2.2
01c,19nov01,pch  cleanup
01b,08may01,pch  Add assembler abstractions (FUNC_EXPORT, FUNC_BEGIN, etc.)
01a,26may00,mcg  created
*/

/*
DESCRIPTION
This module contains chip-dependent routines written in assembly language.
*/

#define _ASMLANGUAGE

        /* globals */

        FUNC_EXPORT(sysDcrDmacr0Get)
        FUNC_EXPORT(sysDcrDmacr0Set)
        FUNC_EXPORT(sysDcrDmacr1Get)
        FUNC_EXPORT(sysDcrDmacr1Set)
        FUNC_EXPORT(sysDcrDmacr2Get)
        FUNC_EXPORT(sysDcrDmacr2Set)
        FUNC_EXPORT(sysDcrDmacr3Get)
        FUNC_EXPORT(sysDcrDmacr3Set)

        FUNC_EXPORT(sysDcrDmact0Get)
        FUNC_EXPORT(sysDcrDmact0Set)
        FUNC_EXPORT(sysDcrDmact1Get)
        FUNC_EXPORT(sysDcrDmact1Set)
        FUNC_EXPORT(sysDcrDmact2Get)
        FUNC_EXPORT(sysDcrDmact2Set)
        FUNC_EXPORT(sysDcrDmact3Get)
        FUNC_EXPORT(sysDcrDmact3Set)

        FUNC_EXPORT(sysDcrDmada0Get)
        FUNC_EXPORT(sysDcrDmada0Set)
        FUNC_EXPORT(sysDcrDmada1Get)
        FUNC_EXPORT(sysDcrDmada1Set)
        FUNC_EXPORT(sysDcrDmada2Get)
        FUNC_EXPORT(sysDcrDmada2Set)
        FUNC_EXPORT(sysDcrDmada3Get)
        FUNC_EXPORT(sysDcrDmada3Set)

        FUNC_EXPORT(sysDcrDmasa0Get)
        FUNC_EXPORT(sysDcrDmasa0Set)
        FUNC_EXPORT(sysDcrDmasa1Get)
        FUNC_EXPORT(sysDcrDmasa1Set)
        FUNC_EXPORT(sysDcrDmasa2Get)
        FUNC_EXPORT(sysDcrDmasa2Set)
        FUNC_EXPORT(sysDcrDmasa3Get)
        FUNC_EXPORT(sysDcrDmasa3Set)

        FUNC_EXPORT(sysDcrDmasg0Get)
        FUNC_EXPORT(sysDcrDmasg0Set)
        FUNC_EXPORT(sysDcrDmasg1Get)
        FUNC_EXPORT(sysDcrDmasg1Set)
        FUNC_EXPORT(sysDcrDmasg2Get)
        FUNC_EXPORT(sysDcrDmasg2Set)
        FUNC_EXPORT(sysDcrDmasg3Get)
        FUNC_EXPORT(sysDcrDmasg3Set)

        FUNC_EXPORT(sysDcrDmasrGet)
        FUNC_EXPORT(sysDcrDmasrSet)

        FUNC_EXPORT(sysDcrDmasgcGet)
        FUNC_EXPORT(sysDcrDmasgcSet)

        FUNC_EXPORT(sysDcrDmaslpGet)
        FUNC_EXPORT(sysDcrDmaslpSet)

        FUNC_EXPORT(sysDcrDmapolGet)
        FUNC_EXPORT(sysDcrDmapolSet)

	_WRS_TEXT_SEG_START

/*******************************************************************************
*
* sysDcrDmacr0Get - get the contents of DMA channel 0 config register
*
* This routine reads and returns the contents of the DMA channel 0 configuration
* register.
*
* RETURNS: N/A

* UINT32 sysDcrDmacr0Get (void)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmacr0Get)

        mfdcr   r3, DMA_CR0
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmacr0Get)
	
/*******************************************************************************
*
* sysDcrDmacr0Set - set the contents of DMA channel 0 configuration register
*
* This routine writes the contents of the DMA channel 0 configuration register
*
* RETURNS: N/A

* void sysDcrDmacr0Set (UINT32)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmacr0Set)

        mtdcr   DMA_CR0, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmacr0Set)
	
/*******************************************************************************
*
* sysDcrDmacr1Get - get the contents of DMA channel 1 config register
*
* This routine reads and returns the contents of the DMA channel 1 configuration
* register.
*
* RETURNS: N/A

* UINT32 sysDcrDmacr1Get (void)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmacr1Get)

        mfdcr   r3, DMA_CR1
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmacr1Get)
	
/*******************************************************************************
*
* sysDcrDmacr1Set - set the contents of DMA channel 1 configuration register
*
* This routine writes the contents of the DMA channel 1 configuration register
*
* RETURNS: N/A

* void sysDcrDmacr1Set (UINT32)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmacr1Set)

        mtdcr   DMA_CR1, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmacr1Set)
	
/*******************************************************************************
*
* sysDcrDmacr2Get - get the contents of DMA channel 2 config register
*
* This routine reads and returns the contents of the DMA channel 2 configuration
* register.
*
* RETURNS: N/A

* UINT32 sysDcrDmacr2Get (void)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmacr2Get)

        mfdcr   r3, DMA_CR2
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmacr2Get)
	
/*******************************************************************************
*
* sysDcrDmacr2Set - set the contents of DMA channel 2 configuration register
*
* This routine writes the contents of the DMA channel 2 configuration register
*
* RETURNS: N/A

* void sysDcrDmacr2Set (UINT32)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmacr2Set)

        mtdcr   DMA_CR2, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmacr2Set)
	
/*******************************************************************************
*
* sysDcrDmacr3Get - get the contents of DMA channel 3 config register
*
* This routine reads and returns the contents of the DMA channel 3 configuration
* register.
*
* RETURNS: N/A

* UINT32 sysDcrDmacr3Get (void)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmacr3Get)

        mfdcr   r3, DMA_CR3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmacr3Get)
	
/*******************************************************************************
*
* sysDcrDmacr3Set - set the contents of DMA channel 3 configuration register
*
* This routine writes the contents of the DMA channel 3 configuration register
*
* RETURNS: N/A

* void sysDcrDmacr3Set (UINT32)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmacr3Set)

        mtdcr   DMA_CR3, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmacr3Set)
	
/*******************************************************************************
*
* sysDcrDmact0Get - get the contents of DMA channel 0 count register
*
* This routine reads and returns the contents of the DMA channel 0 count
* register.
*
* RETURNS: N/A

* UINT32 sysDcrDmact0Get (void)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmact0Get)

        mfdcr   r3, DMA_CT0
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmact0Get)
	
/*******************************************************************************
*
* sysDcrDmact0Set - set the contents of DMA channel 0 count register
*
* This routine writes the contents of the DMA channel 0 count register
*
* RETURNS: N/A

* void sysDcrDmact0Set (UINT32)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmact0Set)

        mtdcr   DMA_CT0, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmact0Set)
	
/*******************************************************************************
*
* sysDcrDmact1Get - get the contents of DMA channel 1 count register
*
* This routine reads and returns the contents of the DMA channel 1 count
* register.
*
* RETURNS: N/A

* UINT32 sysDcrDmact1Get (void)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmact1Get)

        mfdcr   r3, DMA_CT1
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmact1Get)
	
/*******************************************************************************
*
* sysDcrDmact1Set - set the contents of DMA channel 1 count register
*
* This routine writes the contents of the DMA channel 1 count register
*
* RETURNS: N/A

* void sysDcrDmact1Set (UINT32)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmact1Set)

        mtdcr   DMA_CT1, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmact1Set)
	
/*******************************************************************************
*
* sysDcrDmact2Get - get the contents of DMA channel 2 count register
*
* This routine reads and returns the contents of the DMA channel 2 count
* register.
*
* RETURNS: N/A

* UINT32 sysDcrDmact2Get (void)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmact2Get)

        mfdcr   r3, DMA_CT2
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmact2Get)
	
/*******************************************************************************
*
* sysDcrDmact2Set - set the contents of DMA channel 2 count register
*
* This routine writes the contents of the DMA channel 2 count register
*
* RETURNS: N/A

* void sysDcrDmact2Set (UINT32)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmact2Set)

        mtdcr   DMA_CT2, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmact2Set)
	
/*******************************************************************************
*
* sysDcrDmact3Get - get the contents of DMA channel 3 count register
*
* This routine reads and returns the contents of the DMA channel 3 count
* register.
*
* RETURNS: N/A

* UINT32 sysDcrDmact3Get (void)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmact3Get)

        mfdcr   r3, DMA_CT3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmact3Get)
	
/*******************************************************************************
*
* sysDcrDmact3Set - set the contents of DMA channel 3 count register
*
* This routine writes the contents of the DMA channel 3 count register
*
* RETURNS: N/A

* void sysDcrDmact3Set (UINT32)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmact3Set)

        mtdcr   DMA_CT3, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmact3Set)
	
/*******************************************************************************
*
* sysDcrDmada0Get - get the contents of DMA channel 0 destination address
*                   register
*
* This routine reads and returns the contents of the DMA channel 0 destination
* address register.
*
* RETURNS: N/A

* UINT32 sysDcrDmada0Get (void)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmada0Get)

        mfdcr   r3, DMA_DA0
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmada0Get)
	
/*******************************************************************************
*
* sysDcrDmada0Set - set the contents of DMA channel 0 destination address
*                   register
*
* This routine writes the contents of the DMA channel 0 destination address
* register.
*
* RETURNS: N/A

* void sysDcrDmada0Set (UINT32)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmada0Set)

        mtdcr   DMA_DA0, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmada0Set)
	
/*******************************************************************************
*
* sysDcrDmada1Get - get the contents of DMA channel 1 destination address
*                   register
*
* This routine reads and returns the contents of the DMA channel 1 destination
* address register.
*
* RETURNS: N/A

* UINT32 sysDcrDmada1Get (void)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmada1Get)

        mfdcr   r3, DMA_DA1
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmada1Get)
	
/*******************************************************************************
*
* sysDcrDmada1Set - set the contents of DMA channel 1 destination address
*                   register
*
* This routine writes the contents of the DMA channel 1 destination address
* register.
*
* RETURNS: N/A

* void sysDcrDmada1Set (UINT32)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmada1Set)

        mtdcr   DMA_DA1, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmada1Set)
	
/*******************************************************************************
*
* sysDcrDmada2Get - get the contents of DMA channel 2 destination address
*                   register
*
* This routine reads and returns the contents of the DMA channel 2 destination
* address register.
*
* RETURNS: N/A

* UINT32 sysDcrDmada2Get (void)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmada2Get)

        mfdcr   r3, DMA_DA2
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmada2Get)
	
/*******************************************************************************
*
* sysDcrDmada2Set - set the contents of DMA channel 2 destination address
*                   register
*
* This routine writes the contents of the DMA channel 2 destination address
* register.
*
* RETURNS: N/A

* void sysDcrDmada2Set (UINT32)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmada2Set)

        mtdcr   DMA_DA2, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmada2Set)
	
/*******************************************************************************
*
* sysDcrDmada3Get - get the contents of DMA channel 3 destination address
*                   register
*
* This routine reads and returns the contents of the DMA channel 3 destination
* address register.
*
* RETURNS: N/A

* UINT32 sysDcrDmada3Get (void)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmada3Get)

        mfdcr   r3, DMA_DA3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmada3Get)
	
/*******************************************************************************
*
* sysDcrDmada3Set - set the contents of DMA channel 3 destination address
*                   register
*
* This routine writes the contents of the DMA channel 3 destination address
* register.
*
* RETURNS: N/A

* void sysDcrDmada3Set (UINT32)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmada3Set)

        mtdcr   DMA_DA3, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmada3Set)
	
/*******************************************************************************
*
* sysDcrDmasa0Get - get the contents of DMA channel 0 source address
*                   register
*
* This routine reads and returns the contents of the DMA channel 0 source
* address register.
*
* RETURNS: N/A

* UINT32 sysDcrDmasa0Get (void)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmasa0Get)

        mfdcr   r3, DMA_SA0
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmasa0Get)
	
/*******************************************************************************
*
* sysDcrDmasa0Set - set the contents of DMA channel 0 source address
*                   register
*
* This routine writes the contents of the DMA channel 0 source address
* register.
*
* RETURNS: N/A

* void sysDcrDmasa0Set (UINT32)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmasa0Set)

        mtdcr   DMA_SA0, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmasa0Set)
	
/*******************************************************************************
*
* sysDcrDmasa1Get - get the contents of DMA channel 1 source address
*                   register
*
* This routine reads and returns the contents of the DMA channel 1 source
* address register.
*
* RETURNS: N/A

* UINT32 sysDcrDmasa1Get (void)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmasa1Get)

        mfdcr   r3, DMA_SA1
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmasa1Get)
	
/*******************************************************************************
*
* sysDcrDmasa1Set - set the contents of DMA channel 1 source address
*                   register
*
* This routine writes the contents of the DMA channel 1 source address
* register.
*
* RETURNS: N/A

* void sysDcrDmasa1Set (UINT32)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmasa1Set)

        mtdcr   DMA_SA1, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmasa1Set)
	
/*******************************************************************************
*
* sysDcrDmasa2Get - get the contents of DMA channel 2 source address
*                   register
*
* This routine reads and returns the contents of the DMA channel 2 source
* address register.
*
* RETURNS: N/A

* UINT32 sysDcrDmasa2Get (void)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmasa2Get)

        mfdcr   r3, DMA_SA2
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmasa2Get)
	
/*******************************************************************************
*
* sysDcrDmasa2Set - set the contents of DMA channel 2 source address
*                   register
*
* This routine writes the contents of the DMA channel 2 source address
* register.
*
* RETURNS: N/A

* void sysDcrDmasa2Set (UINT32)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmasa2Set)

        mtdcr   DMA_SA2, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmasa2Set)
	
/*******************************************************************************
*
* sysDcrDmasa3Get - get the contents of DMA channel 3 source address
*                   register
*
* This routine reads and returns the contents of the DMA channel 3 source
* address register.
*
* RETURNS: N/A

* UINT32 sysDcrDmasa3Get (void)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmasa3Get)

        mfdcr   r3, DMA_SA3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmasa3Get)
	
/*******************************************************************************
*
* sysDcrDmasa3Set - set the contents of DMA channel 3 source address
*                   register
*
* This routine writes the contents of the DMA channel 3 source address
* register.
*
* RETURNS: N/A

* void sysDcrDmasa3Set (UINT32)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmasa3Set)

        mtdcr   DMA_SA3, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmasa3Set)
	
/*******************************************************************************
*
* sysDcrDmasg0Get - get the contents of DMA channel 0 scatter/gather descriptor
*                   address register
*
* This routine reads and returns the contents of the DMA channel 0
* scatter/gather descriptor address register.
*
* RETURNS: N/A

* UINT32 sysDcrDmasg0Get (void)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmasg0Get)

        mfdcr   r3, DMA_SG0
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmasg0Get)
	
/*******************************************************************************
*
* sysDcrDmasg0Set - set the contents of DMA channel 0 scatter/gather descriptor
*                   address register
*
* This routine writes the contents of the DMA channel 0 scatter/gather
* descriptor address register.
*
* RETURNS: N/A

* void sysDcrDmasg0Set (UINT32)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmasg0Set)

        mtdcr   DMA_SG0, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmasg0Set)
	
/*******************************************************************************
*
* sysDcrDmasg1Get - get the contents of DMA channel 1 scatter/gather descriptor
*                   address register
*
* This routine reads and returns the contents of the DMA channel 1
* scatter/gather descriptor address register.
*
* RETURNS: N/A

* UINT32 sysDcrDmasg1Get (void)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmasg1Get)

        mfdcr   r3, DMA_SG1
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmasg1Get)
	
/*******************************************************************************
*
* sysDcrDmasg1Set - set the contents of DMA channel 1 scatter/gather descriptor
*                   address register
*
* This routine writes the contents of the DMA channel 1 scatter/gather
* descriptor address register.
*
* RETURNS: N/A

* void sysDcrDmasg1Set (UINT32)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmasg1Set)

        mtdcr   DMA_SG1, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmasg1Set)
	
/*******************************************************************************
*
* sysDcrDmasg2Get - get the contents of DMA channel 2 scatter/gather descriptor
*                   address register
*
* This routine reads and returns the contents of the DMA channel 2
* scatter/gather descriptor address register.
*
* RETURNS: N/A

* UINT32 sysDcrDmasg2Get (void)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmasg2Get)

        mfdcr   r3, DMA_SG2
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmasg2Get)
	
/*******************************************************************************
*
* sysDcrDmasg2Set - set the contents of DMA channel 2 scatter/gather descriptor
*                   address register
*
* This routine writes the contents of the DMA channel 2 scatter/gather
* descriptor address register.
*
* RETURNS: N/A

* void sysDcrDmasg2Set (UINT32)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmasg2Set)

        mtdcr   DMA_SG2, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmasg2Set)
	
/*******************************************************************************
*
* sysDcrDmasg3Get - get the contents of DMA channel 3 scatter/gather descriptor
*                   address register
*
* This routine reads and returns the contents of the DMA channel 3
* scatter/gather descriptor address register.
*
* RETURNS: N/A

* UINT32 sysDcrDmasg3Get (void)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmasg3Get)

        mfdcr   r3, DMA_SG3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmasg3Get)
	
/*******************************************************************************
*
* sysDcrDmasg3Set - set the contents of DMA channel 3 scatter/gather descriptor
*                   address register
*
* This routine writes the contents of the DMA channel 3 scatter/gather
* descriptor address register.
*
* RETURNS: N/A

* void sysDcrDmasg3Set (UINT32)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmasg3Set)

        mtdcr   DMA_SG3, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmasg3Set)
	
/*******************************************************************************
*
* sysDcrDmasrGet - get the contents of DMA status register
*
* This routine reads and returns the contents of the DMA status register
*
* RETURNS: N/A

* UINT32 sysDcrDmasrGet (void)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmasrGet)

        mfdcr   r3, DMA_SR
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmasrGet)
	
/*******************************************************************************
*
* sysDcrDmasrSet - set the contents of DMA status register
*
* This routine writes the contents of the DMA status register
*
* RETURNS: N/A

* void sysDcrDmasrSet (UINT32)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmasrSet)

        mtdcr   DMA_SR, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmasrSet)
	
/*******************************************************************************
*
* sysDcrDmasgcGet - get the contents of DMA scatter/gather command register
*
* This routine reads and returns the contents of the DMA scatter/gather command
* register
*
* RETURNS: N/A

* UINT32 sysDcrDmasgcGet (void)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmasgcGet)

        mfdcr   r3, DMA_SGC
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmasgcGet)
	
/*******************************************************************************
*
* sysDcrDmasgcSet - set the contents of DMA scatter/gather status register
*
* This routine writes the contents of the DMA scatter/gather status register
*
* RETURNS: N/A

* void sysDcrDmasgcSet (UINT32)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmasgcSet)

        mtdcr   DMA_SGC, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmasgcSet)

/*******************************************************************************
*
* sysDcrDmaslpGet - get the contents of DMA sleep mode register
*
* This routine reads and returns the contents of the DMA sleep mode register
*
* RETURNS: N/A

* UINT32 sysDcrDmaslpGet (void)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmaslpGet)

        mfdcr   r3, DMA_SLP
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmaslpGet)

/*******************************************************************************
*
* sysDcrDmaslpSet - set the contents of DMA sleep mode register
*
* This routine writes the contents of the DMA sleep mode register
*
* RETURNS: N/A

* void sysDcrDmaslpSet (UINT32)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmaslpSet)

        mtdcr   DMA_SLP, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmaslpSet)

/*******************************************************************************
*
* sysDcrDmapolGet - get the contents of DMA polarity register
*
* This routine reads and returns the contents of the DMA polarity register
*
* RETURNS: N/A

* UINT32 sysDcrDmapolGet (void)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmapolGet)

        mfdcr   r3, DMA_POL
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmapolGet)

/*******************************************************************************
*
* sysDcrDmapolSet - set the contents of DMA polarity register
*
* This routine writes the contents of the DMA polarity register
*
* RETURNS: N/A

* void sysDcrDmapolSet (UINT32)

* NOMANUAL
*/

FUNC_BEGIN(sysDcrDmapolSet)

        mtdcr   DMA_POL, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrDmapolSet)
