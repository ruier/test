/* malDcr.s - IBM Memory Access Layer (MAL) DCR access assembly routines */

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

        FUNC_EXPORT(sysDcrMalcrGet)
        FUNC_EXPORT(sysDcrMalcrSet)
        FUNC_EXPORT(sysDcrMalesrGet)
        FUNC_EXPORT(sysDcrMalesrClear)
        FUNC_EXPORT(sysDcrMalierGet)
        FUNC_EXPORT(sysDcrMalierSet)
        FUNC_EXPORT(sysDcrMaltxcasrGet)
        FUNC_EXPORT(sysDcrMaltxcasrSet)
        FUNC_EXPORT(sysDcrMaltxcarrGet)
        FUNC_EXPORT(sysDcrMaltxcarrSet)
        FUNC_EXPORT(sysDcrMaltxeobisrGet)
        FUNC_EXPORT(sysDcrMaltxeobisrClear)
        FUNC_EXPORT(sysDcrMaltxdeirGet)
        FUNC_EXPORT(sysDcrMaltxdeirClear)
        FUNC_EXPORT(sysDcrMalrxcasrGet)
        FUNC_EXPORT(sysDcrMalrxcasrSet)
        FUNC_EXPORT(sysDcrMalrxcarrGet)
        FUNC_EXPORT(sysDcrMalrxcarrSet)
        FUNC_EXPORT(sysDcrMalrxeobisrGet)
        FUNC_EXPORT(sysDcrMalrxeobisrClear)
        FUNC_EXPORT(sysDcrMalrxdeirGet)
        FUNC_EXPORT(sysDcrMalrxdeirClear)
        FUNC_EXPORT(sysDcrMaltxctp0rGet)
        FUNC_EXPORT(sysDcrMaltxctp0rSet)
        FUNC_EXPORT(sysDcrMaltxctp1rGet)
        FUNC_EXPORT(sysDcrMaltxctp1rSet)
        FUNC_EXPORT(sysDcrMaltxctp2rGet)
        FUNC_EXPORT(sysDcrMaltxctp2rSet)
        FUNC_EXPORT(sysDcrMaltxctp3rGet)
        FUNC_EXPORT(sysDcrMaltxctp3rSet)
#ifdef MAL_CHANNELS_32
        FUNC_EXPORT(sysDcrMaltxctp4rGet)
        FUNC_EXPORT(sysDcrMaltxctp4rSet)
        FUNC_EXPORT(sysDcrMaltxctp5rGet)
        FUNC_EXPORT(sysDcrMaltxctp5rSet)
        FUNC_EXPORT(sysDcrMaltxctp6rGet)
        FUNC_EXPORT(sysDcrMaltxctp6rSet)
        FUNC_EXPORT(sysDcrMaltxctp7rGet)
        FUNC_EXPORT(sysDcrMaltxctp7rSet)
        FUNC_EXPORT(sysDcrMaltxctp8rGet)
        FUNC_EXPORT(sysDcrMaltxctp8rSet)
        FUNC_EXPORT(sysDcrMaltxctp9rGet)
        FUNC_EXPORT(sysDcrMaltxctp9rSet)
        FUNC_EXPORT(sysDcrMaltxctp10rGet)
        FUNC_EXPORT(sysDcrMaltxctp10rSet)
        FUNC_EXPORT(sysDcrMaltxctp11rGet)
        FUNC_EXPORT(sysDcrMaltxctp11rSet)
        FUNC_EXPORT(sysDcrMaltxctp12rGet)
        FUNC_EXPORT(sysDcrMaltxctp12rSet)
        FUNC_EXPORT(sysDcrMaltxctp13rGet)
        FUNC_EXPORT(sysDcrMaltxctp13rSet)
        FUNC_EXPORT(sysDcrMaltxctp14rGet)
        FUNC_EXPORT(sysDcrMaltxctp14rSet)
        FUNC_EXPORT(sysDcrMaltxctp15rGet)
        FUNC_EXPORT(sysDcrMaltxctp15rSet)
#endif /* MAL_CHANNELS_32 */
#ifdef MAL_CHANNELS_64
        FUNC_EXPORT(sysDcrMaltxctp16rGet)
        FUNC_EXPORT(sysDcrMaltxctp16rSet)
        FUNC_EXPORT(sysDcrMaltxctp17rGet)
        FUNC_EXPORT(sysDcrMaltxctp17rSet)
        FUNC_EXPORT(sysDcrMaltxctp18rGet)
        FUNC_EXPORT(sysDcrMaltxctp18rSet)
        FUNC_EXPORT(sysDcrMaltxctp19rGet)
        FUNC_EXPORT(sysDcrMaltxctp19rSet)
        FUNC_EXPORT(sysDcrMaltxctp20rGet)
        FUNC_EXPORT(sysDcrMaltxctp20rSet)
        FUNC_EXPORT(sysDcrMaltxctp21rGet)
        FUNC_EXPORT(sysDcrMaltxctp21rSet)
        FUNC_EXPORT(sysDcrMaltxctp22rGet)
        FUNC_EXPORT(sysDcrMaltxctp22rSet)
        FUNC_EXPORT(sysDcrMaltxctp23rGet)
        FUNC_EXPORT(sysDcrMaltxctp23rSet)
        FUNC_EXPORT(sysDcrMaltxctp24rGet)
        FUNC_EXPORT(sysDcrMaltxctp24rSet)
        FUNC_EXPORT(sysDcrMaltxctp25rGet)
        FUNC_EXPORT(sysDcrMaltxctp25rSet)
        FUNC_EXPORT(sysDcrMaltxctp26rGet)
        FUNC_EXPORT(sysDcrMaltxctp26rSet)
        FUNC_EXPORT(sysDcrMaltxctp27rGet)
        FUNC_EXPORT(sysDcrMaltxctp27rSet)
        FUNC_EXPORT(sysDcrMaltxctp28rGet)
        FUNC_EXPORT(sysDcrMaltxctp28rSet)
        FUNC_EXPORT(sysDcrMaltxctp29rGet)
        FUNC_EXPORT(sysDcrMaltxctp29rSet)
        FUNC_EXPORT(sysDcrMaltxctp30rGet)
        FUNC_EXPORT(sysDcrMaltxctp30rSet)
        FUNC_EXPORT(sysDcrMaltxctp31rGet)
        FUNC_EXPORT(sysDcrMaltxctp31rSet)
#endif /* MAL_CHANNELS_64 */

        FUNC_EXPORT(sysDcrMalrxctp0rGet)
        FUNC_EXPORT(sysDcrMalrxctp0rSet)
        FUNC_EXPORT(sysDcrMalrxctp1rGet)
        FUNC_EXPORT(sysDcrMalrxctp1rSet)
        FUNC_EXPORT(sysDcrMalrxctp2rGet)
        FUNC_EXPORT(sysDcrMalrxctp2rSet)
        FUNC_EXPORT(sysDcrMalrxctp3rGet)
        FUNC_EXPORT(sysDcrMalrxctp3rSet)
#ifdef MAL_CHANNELS_32
        FUNC_EXPORT(sysDcrMalrxctp4rGet)
        FUNC_EXPORT(sysDcrMalrxctp4rSet)
        FUNC_EXPORT(sysDcrMalrxctp5rGet)
        FUNC_EXPORT(sysDcrMalrxctp5rSet)
        FUNC_EXPORT(sysDcrMalrxctp6rGet)
        FUNC_EXPORT(sysDcrMalrxctp6rSet)
        FUNC_EXPORT(sysDcrMalrxctp7rGet)
        FUNC_EXPORT(sysDcrMalrxctp7rSet)
        FUNC_EXPORT(sysDcrMalrxctp8rGet)
        FUNC_EXPORT(sysDcrMalrxctp8rSet)
        FUNC_EXPORT(sysDcrMalrxctp9rGet)
        FUNC_EXPORT(sysDcrMalrxctp9rSet)
        FUNC_EXPORT(sysDcrMalrxctp10rGet)
        FUNC_EXPORT(sysDcrMalrxctp10rSet)
        FUNC_EXPORT(sysDcrMalrxctp11rGet)
        FUNC_EXPORT(sysDcrMalrxctp11rSet)
        FUNC_EXPORT(sysDcrMalrxctp12rGet)
        FUNC_EXPORT(sysDcrMalrxctp12rSet)
        FUNC_EXPORT(sysDcrMalrxctp13rGet)
        FUNC_EXPORT(sysDcrMalrxctp13rSet)
        FUNC_EXPORT(sysDcrMalrxctp14rGet)
        FUNC_EXPORT(sysDcrMalrxctp14rSet)
        FUNC_EXPORT(sysDcrMalrxctp15rGet)
        FUNC_EXPORT(sysDcrMalrxctp15rSet)
#endif /* MAL_CHANNELS_32 */
#ifdef MAL_CHANNELS_64
        FUNC_EXPORT(sysDcrMalrxctp16rGet)
        FUNC_EXPORT(sysDcrMalrxctp16rSet)
        FUNC_EXPORT(sysDcrMalrxctp17rGet)
        FUNC_EXPORT(sysDcrMalrxctp17rSet)
        FUNC_EXPORT(sysDcrMalrxctp18rGet)
        FUNC_EXPORT(sysDcrMalrxctp18rSet)
        FUNC_EXPORT(sysDcrMalrxctp19rGet)
        FUNC_EXPORT(sysDcrMalrxctp19rSet)
        FUNC_EXPORT(sysDcrMalrxctp20rGet)
        FUNC_EXPORT(sysDcrMalrxctp20rSet)
        FUNC_EXPORT(sysDcrMalrxctp21rGet)
        FUNC_EXPORT(sysDcrMalrxctp21rSet)
        FUNC_EXPORT(sysDcrMalrxctp22rGet)
        FUNC_EXPORT(sysDcrMalrxctp22rSet)
        FUNC_EXPORT(sysDcrMalrxctp23rGet)
        FUNC_EXPORT(sysDcrMalrxctp23rSet)
        FUNC_EXPORT(sysDcrMalrxctp24rGet)
        FUNC_EXPORT(sysDcrMalrxctp24rSet)
        FUNC_EXPORT(sysDcrMalrxctp25rGet)
        FUNC_EXPORT(sysDcrMalrxctp25rSet)
        FUNC_EXPORT(sysDcrMalrxctp26rGet)
        FUNC_EXPORT(sysDcrMalrxctp26rSet)
        FUNC_EXPORT(sysDcrMalrxctp27rGet)
        FUNC_EXPORT(sysDcrMalrxctp27rSet)
        FUNC_EXPORT(sysDcrMalrxctp28rGet)
        FUNC_EXPORT(sysDcrMalrxctp28rSet)
        FUNC_EXPORT(sysDcrMalrxctp29rGet)
        FUNC_EXPORT(sysDcrMalrxctp29rSet)
        FUNC_EXPORT(sysDcrMalrxctp30rGet)
        FUNC_EXPORT(sysDcrMalrxctp30rSet)
        FUNC_EXPORT(sysDcrMalrxctp31rGet)
        FUNC_EXPORT(sysDcrMalrxctp31rSet)
#endif /* MAL_CHANNELS_64 */

        FUNC_EXPORT(sysDcrMalrcbs0Get)
        FUNC_EXPORT(sysDcrMalrcbs0Set)
        FUNC_EXPORT(sysDcrMalrcbs1Get)
        FUNC_EXPORT(sysDcrMalrcbs1Set)
        FUNC_EXPORT(sysDcrMalrcbs2Get)
        FUNC_EXPORT(sysDcrMalrcbs2Set)
        FUNC_EXPORT(sysDcrMalrcbs3Get)
        FUNC_EXPORT(sysDcrMalrcbs3Set)
#ifdef MAL_CHANNELS_32
        FUNC_EXPORT(sysDcrMalrcbs4Get)
        FUNC_EXPORT(sysDcrMalrcbs4Set)
        FUNC_EXPORT(sysDcrMalrcbs5Get)
        FUNC_EXPORT(sysDcrMalrcbs5Set)
        FUNC_EXPORT(sysDcrMalrcbs6Get)
        FUNC_EXPORT(sysDcrMalrcbs6Set)
        FUNC_EXPORT(sysDcrMalrcbs7Get)
        FUNC_EXPORT(sysDcrMalrcbs7Set)
        FUNC_EXPORT(sysDcrMalrcbs8Get)
        FUNC_EXPORT(sysDcrMalrcbs8Set)
        FUNC_EXPORT(sysDcrMalrcbs9Get)
        FUNC_EXPORT(sysDcrMalrcbs9Set)
        FUNC_EXPORT(sysDcrMalrcbs10Get)
        FUNC_EXPORT(sysDcrMalrcbs10Set)
        FUNC_EXPORT(sysDcrMalrcbs11Get)
        FUNC_EXPORT(sysDcrMalrcbs11Set)
        FUNC_EXPORT(sysDcrMalrcbs12Get)
        FUNC_EXPORT(sysDcrMalrcbs12Set)
        FUNC_EXPORT(sysDcrMalrcbs13Get)
        FUNC_EXPORT(sysDcrMalrcbs13Set)
        FUNC_EXPORT(sysDcrMalrcbs14Get)
        FUNC_EXPORT(sysDcrMalrcbs14Set)
        FUNC_EXPORT(sysDcrMalrcbs15Get)
        FUNC_EXPORT(sysDcrMalrcbs15Set)
#endif /* MAL_CHANNELS_32 */
#ifdef MAL_CHANNELS_64
        FUNC_EXPORT(sysDcrMalrcbs16Get)
        FUNC_EXPORT(sysDcrMalrcbs16Set)
        FUNC_EXPORT(sysDcrMalrcbs17Get)
        FUNC_EXPORT(sysDcrMalrcbs17Set)
        FUNC_EXPORT(sysDcrMalrcbs18Get)
        FUNC_EXPORT(sysDcrMalrcbs18Set)
        FUNC_EXPORT(sysDcrMalrcbs19Get)
        FUNC_EXPORT(sysDcrMalrcbs19Set)
        FUNC_EXPORT(sysDcrMalrcbs20Get)
        FUNC_EXPORT(sysDcrMalrcbs20Set)
        FUNC_EXPORT(sysDcrMalrcbs21Get)
        FUNC_EXPORT(sysDcrMalrcbs21Set)
        FUNC_EXPORT(sysDcrMalrcbs22Get)
        FUNC_EXPORT(sysDcrMalrcbs22Set)
        FUNC_EXPORT(sysDcrMalrcbs23Get)
        FUNC_EXPORT(sysDcrMalrcbs23Set)
        FUNC_EXPORT(sysDcrMalrcbs24Get)
        FUNC_EXPORT(sysDcrMalrcbs24Set)
        FUNC_EXPORT(sysDcrMalrcbs25Get)
        FUNC_EXPORT(sysDcrMalrcbs25Set)
        FUNC_EXPORT(sysDcrMalrcbs26Get)
        FUNC_EXPORT(sysDcrMalrcbs26Set)
        FUNC_EXPORT(sysDcrMalrcbs27Get)
        FUNC_EXPORT(sysDcrMalrcbs27Set)
        FUNC_EXPORT(sysDcrMalrcbs28Get)
        FUNC_EXPORT(sysDcrMalrcbs28Set)
        FUNC_EXPORT(sysDcrMalrcbs29Get)
        FUNC_EXPORT(sysDcrMalrcbs29Set)
        FUNC_EXPORT(sysDcrMalrcbs30Get)
        FUNC_EXPORT(sysDcrMalrcbs30Set)
        FUNC_EXPORT(sysDcrMalrcbs31Get)
        FUNC_EXPORT(sysDcrMalrcbs31Set)
#endif /* MAL_CHANNELS_64 */

	_WRS_TEXT_SEG_START

/*******************************************************************************
*
* sysDcrMalcrGet - get the contents of MAL configuration register
*
* This routine reads and returns the contents of the MAL configuration register.
*
* NOMANUAL

* UINT32 sysDcrMalcrGet (void)

*/

FUNC_BEGIN(sysDcrMalcrGet)

        mfdcr   r3, MAL_CFG
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrMalcrGet)

/*******************************************************************************
*
* sysDcrMalcrSet - set the contents of MAL configuration register
*
* This routine writes the contents of the MAL configuration register
*
* NOMANUAL

* void sysDcrMalcrSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalcrSet)

        mtdcr   MAL_CFG, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrMalcrSet)

/*******************************************************************************
*
* sysDcrMalesrGet - get the contents of the MAL Error Status Register
*
* This routine reads and returns the contents of the MAL Error Status Register.
*
* NOMANUAL

* UINT32 sysDcrMalesrGet (void)

*/

FUNC_BEGIN(sysDcrMalesrGet)

        mfdcr   r3, MAL_ESR
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrMalesrGet)

/*******************************************************************************
*
* sysDcrMalesrClear - clears pending status in the MAL Error Status Register
*
* This routine writes to the MAL Error Status Register.  It will clear the
* status for each bit that is set to 1 in the input parameter.
*
* NOMANUAL

* void sysDcrMalesrClear (UINT32)

*/

FUNC_BEGIN(sysDcrMalesrClear)

        mtdcr   MAL_ESR, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrMalesrClear)

/*******************************************************************************
*
* sysDcrMalierGet - get the contents of MAL interrupt enable register
*
* This routine reads and returns the contents of the MAL interrupt enable
* register.
*
* NOMANUAL

* UINT32 sysDcrMalierGet (void)

*/

FUNC_BEGIN(sysDcrMalierGet)

        mfdcr   r3, MAL_IER
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrMalierGet)

/*******************************************************************************
*
* sysDcrMalierSet - set the contents of MAL interrupt enable register
*
* This routine writes the contents of the MAL interrupt enable register.
*
* NOMANUAL

* void sysDcrMalierSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalierSet)

        mtdcr   MAL_IER, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrMalierSet)

/*******************************************************************************
*
* sysDcrMaltxcasrGet - get the contents of the MAL Transmit Channel Active Set
*                      Register
*
* This routine reads and returns the contents of the MAL Transmit Channel Active
* Set register.
*
* NOMANUAL

* UINT32 sysDcrMaltxcasrGet (void)

*/

FUNC_BEGIN(sysDcrMaltxcasrGet)

        mfdcr   r3, MAL_TXCASR
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrMaltxcasrGet)

/*******************************************************************************
*
* sysDcrMaltxcasrSet - set the contents of the MAL Transmit Channel Active Set
*                      Register
*
* This function is used to enable MAL transmit channels. For each bit in the
* input parameter that is set to '1', its corresponding TX channel will be
* enabled.
*
* NOMANUAL

* void sysDcrMaltxcasrSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxcasrSet)

        mtdcr   MAL_TXCASR, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrMaltxcasrSet)

/*******************************************************************************
*
* sysDcrMaltxcarrGet - get the contents of the MAL Transmit Channel Active Reset
*                      Register
*
* This routine reads and returns the contents of the MAL Transmit Channel Active
* Reset register.
*
* NOMANUAL

* UINT32 sysDcrMaltxcarrGet (void)

*/

FUNC_BEGIN(sysDcrMaltxcarrGet)

        mfdcr   r3, MAL_TXCARR
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrMaltxcarrGet)

/*******************************************************************************
*
* sysDcrMaltxcarrSet - set the contents of the MAL Transmit Channel Active Set
*                      Register
*
* This function is used to disable MAL transmit channels. For each bit in the
* input parameter that is set to '1', its corresponding TX channel will be
* disabled.
*
* NOMANUAL

* void sysDcrMaltxcarrSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxcarrSet)

        mtdcr   MAL_TXCARR, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrMaltxcarrSet)

/*******************************************************************************
*
* sysDcrMaltxeobisrGet - get the contents of the MAL Transmit End-Of-Buffer
*                        Interrupt Status Register
*
* This routine reads and returns the contents of the MAL Transmit End-of-Buffer
* Interrupt Status Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxeobisrGet (void)

*/

FUNC_BEGIN(sysDcrMaltxeobisrGet)

        mfdcr   r3, MAL_TXEOBISR
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrMaltxeobisrGet)

/*******************************************************************************
*
* sysDcrMaltxeobisrClear - clears pending status in the MAL Transmit
*                          End-of-Buffer Interrupt Status Register
*
* This routine writes to the MAL Transmit End-of-Buffer Interrupt Status
* Register.  It will clear the status for each bit that is set to 1 in the
* input parameter.
*
* NOMANUAL

* void sysDcrMaltxeobisrClear (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxeobisrClear)

        mtdcr   MAL_TXEOBISR, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrMaltxeobisrClear)

/*******************************************************************************
*
* sysDcrMaltxdeirGet - get the contents of the MAL Transmit Descriptor Error
*                      Interrupt status Register
*
* This routine reads and returns the contents of the MAL Transmit Descriptor
* Error Interrupt Status Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxdeirGet (void)

*/

FUNC_BEGIN(sysDcrMaltxdeirGet)

        mfdcr   r3, MAL_TXDEIR
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrMaltxdeirGet)

/*******************************************************************************
*
* sysDcrMaltxdeirClear - clears pending status in the MAL Transmit Descriptor
*                        Error Interrupt status Register
*
* This routine writes to the MAL Transmit Descriptor Error  Interrupt Status
* Register.  It will clear the status for each bit that is set to 1 in the
* input parameter.
*
* NOMANUAL

* void sysDcrMaltxdeirClear (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxdeirClear)

        mtdcr   MAL_TXDEIR, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrMaltxdeirClear)

/*******************************************************************************
*
* sysDcrMalrxcasrGet - get the contents of the MAL Receive Channel Active Set
*                      Register
*
* This routine reads and returns the contents of the MAL Receive Channel Active
* Set register.
*
* NOMANUAL

* UINT32 sysDcrMalrxcasrGet (void)

*/

FUNC_BEGIN(sysDcrMalrxcasrGet)

        mfdcr   r3, MAL_RXCASR
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrMalrxcasrGet)

/*******************************************************************************
*
* sysDcrMalrxcasrSet - set the contents of the MAL Receive Channel Active Set
*                      Register
*
* This function is used to enable MAL Receive channels. For each bit in the
* input parameter that is set to '1', its corresponding rx channel will be
* enabled.
*
* NOMANUAL

* void sysDcrMalrxcasrSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxcasrSet)

        mtdcr   MAL_RXCASR, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrMalrxcasrSet)

/*******************************************************************************
*
* sysDcrMalrxcarrGet - get the contents of the MAL Receive Channel Active Reset
*                      Register
*
* This routine reads and returns the contents of the MAL Receive Channel Active
* Reset register.
*
* NOMANUAL

* UINT32 sysDcrMalrxcarrGet (void)

*/

FUNC_BEGIN(sysDcrMalrxcarrGet)

        mfdcr   r3, MAL_RXCARR
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrMalrxcarrGet)

/*******************************************************************************
*
* sysDcrMalrxcarrSet - set the contents of the MAL Receive Channel Active Set
*                      Register
*
* This function is used to disable MAL Receive channels. For each bit in the
* input parameter that is set to '1', its corresponding rx channel will be
* disabled.
*
* NOMANUAL

* void sysDcrMalrxcarrSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxcarrSet)

        mtdcr   MAL_RXCARR, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrMalrxcarrSet)

/*******************************************************************************
*
* sysDcrMalrxeobisrGet - get the contents of the MAL Receive End-Of-Buffer
*                        Interrupt Status Register
*
* This routine reads and returns the contents of the MAL Receive End-of-Buffer
* Interrupt Status Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxeobisrGet (void)

*/

FUNC_BEGIN(sysDcrMalrxeobisrGet)

        mfdcr   r3, MAL_RXEOBISR
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrMalrxeobisrGet)

/*******************************************************************************
*
* sysDcrMalrxeobisrClear - clears pending status in the MAL Receive
*                          End-of-Buffer Interrupt Status Register
*
* This routine writes to the MAL Receive End-of-Buffer Interrupt Status
* Register.  It will clear the status for each bit that is set to 1 in the
* input parameter.
*
* NOMANUAL

* void sysDcrMalrxeobisrClear (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxeobisrClear)

        mtdcr   MAL_RXEOBISR, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrMalrxeobisrClear)

/*******************************************************************************
*
* sysDcrMalrxdeirGet - get the contents of the MAL Receive Descriptor Error
*                      Interrupt status Register
*
* This routine reads and returns the contents of the MAL Receive Descriptor
* Error Interrupt Status Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxdeirGet (void)

*/

FUNC_BEGIN(sysDcrMalrxdeirGet)

        mfdcr   r3, MAL_RXDEIR
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrMalrxdeirGet)

/*******************************************************************************
*
* sysDcrMalrxdeirClear - clears pending status in the MAL Receive Descriptor
*                        Error Interrupt status Register
*
* This routine writes to the MAL Receive Descriptor Error  Interrupt Status
* Register.  It will clear the status for each bit that is set to 1 in the
* input parameter.
*
* NOMANUAL

* void sysDcrMalrxdeirClear (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxdeirClear)

        mtdcr   MAL_RXDEIR, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrMalrxdeirClear)

/*******************************************************************************
*
* sysDcrMaltxctp0rGet - get the contents of MAL Transmit Channel 0 Table Pointer
*                       Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 0
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp0rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp0rGet)

        mfdcr   r3, MAL_TXCTP0R
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrMaltxctp0rGet)

/*******************************************************************************
*
* sysDcrMaltxctp0rSet - set the contents of MAL Transmit Channel 0 Table Pointer
*                      Register
*
* This routine writes the contents of the MAL Transmit Channel 0 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp0rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp0rSet)

        mtdcr   MAL_TXCTP0R, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrMaltxctp0rSet)

/*******************************************************************************
*
* sysDcrMaltxctp1rGet - get the contents of MAL Transmit Channel 1 Table Pointer
*                       Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 1
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp1rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp1rGet)

        mfdcr   r3, MAL_TXCTP1R
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrMaltxctp1rGet)

/*******************************************************************************
*
* sysDcrMaltxctp1rSet - set the contents of MAL Transmit Channel 1 Table Pointer
*                      Register
*
* This routine writes the contents of the MAL Transmit Channel 1 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp1rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp1rSet)

        mtdcr   MAL_TXCTP1R, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrMaltxctp1rSet)

/*******************************************************************************
*
* sysDcrMaltxctp2rGet - get the contents of MAL Transmit Channel 2 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 2
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp2rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp2rGet)

        mfdcr   r3, MAL_TXCTP2R
        blr
FUNC_END(sysDcrMaltxctp2rGet)

/*******************************************************************************
*
* sysDcrMaltxctp2rSet - set the contents of MAL Transmit Channel 2 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Transmit Channel 2 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp2rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp2rSet)

        mtdcr   MAL_TXCTP2R, r3
        blr
FUNC_END(sysDcrMaltxctp2rSet)

/*******************************************************************************
*
* sysDcrMaltxctp3rGet - get the contents of MAL Transmit Channel 3 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 3
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp3rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp3rGet)

        mfdcr   r3, MAL_TXCTP3R
        blr
FUNC_END(sysDcrMaltxctp3rGet)

/*******************************************************************************
*
* sysDcrMaltxctp3rSet - set the contents of MAL Transmit Channel 3 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Transmit Channel 3 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp3rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp3rSet)

        mtdcr   MAL_TXCTP3R, r3
        blr
FUNC_END(sysDcrMaltxctp3rSet)


#ifdef MAL_CHANNELS_32
/*******************************************************************************
*
* sysDcrMaltxctp4rGet - get the contents of MAL Transmit Channel 4 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 4
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp4rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp4rGet)

        mfdcr   r3, MAL_TXCTP4R
        blr
FUNC_END(sysDcrMaltxctp4rGet)

/*******************************************************************************
*
* sysDcrMaltxctp4rSet - set the contents of MAL Transmit Channel 4 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Transmit Channel 4 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp4rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp4rSet)

        mtdcr   MAL_TXCTP4R, r3
        blr
FUNC_END(sysDcrMaltxctp4rSet)

/*******************************************************************************
*
* sysDcrMaltxctp5rGet - get the contents of MAL Transmit Channel 5 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 5
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp5rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp5rGet)

        mfdcr   r3, MAL_TXCTP5R
        blr
FUNC_END(sysDcrMaltxctp5rGet)

/*******************************************************************************
*
* sysDcrMaltxctp5rSet - set the contents of MAL Transmit Channel 5 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Transmit Channel 5 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp5rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp5rSet)

        mtdcr   MAL_TXCTP5R, r3
        blr
FUNC_END(sysDcrMaltxctp5rSet)

/*******************************************************************************
*
* sysDcrMaltxctp6rGet - get the contents of MAL Transmit Channel 6 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 6
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp6rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp6rGet)

        mfdcr   r3, MAL_TXCTP6R
        blr
FUNC_END(sysDcrMaltxctp6rGet)

/*******************************************************************************
*
* sysDcrMaltxctp6rSet - set the contents of MAL Transmit Channel 6 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Transmit Channel 6 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp6rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp6rSet)

        mtdcr   MAL_TXCTP6R, r3
        blr
FUNC_END(sysDcrMaltxctp6rSet)

/*******************************************************************************
*
* sysDcrMaltxctp7rGet - get the contents of MAL Transmit Channel 7 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 7
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp7rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp7rGet)

        mfdcr   r3, MAL_TXCTP7R
        blr
FUNC_END(sysDcrMaltxctp7rGet)

/*******************************************************************************
*
* sysDcrMaltxctp7rSet - set the contents of MAL Transmit Channel 7 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Transmit Channel 7 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp7rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp7rSet)

        mtdcr   MAL_TXCTP7R, r3
        blr
FUNC_END(sysDcrMaltxctp7rSet)

/*******************************************************************************
*
* sysDcrMaltxctp8rGet - get the contents of MAL Transmit Channel 8 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 8
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp8rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp8rGet)

        mfdcr   r3, MAL_TXCTP8R
        blr
FUNC_END(sysDcrMaltxctp8rGet)

/*******************************************************************************
*
* sysDcrMaltxctp8rSet - set the contents of MAL Transmit Channel 8 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Transmit Channel 8 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp8rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp8rSet)

        mtdcr   MAL_TXCTP8R, r3
        blr
FUNC_END(sysDcrMaltxctp8rSet)

/*******************************************************************************
*
* sysDcrMaltxctp9rGet - get the contents of MAL Transmit Channel 9 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 9
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp9rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp9rGet)

        mfdcr   r3, MAL_TXCTP9R
        blr
FUNC_END(sysDcrMaltxctp9rGet)

/*******************************************************************************
*
* sysDcrMaltxctp9rSet - set the contents of MAL Transmit Channel 9 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Transmit Channel 9 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp9rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp9rSet)

        mtdcr   MAL_TXCTP9R, r3
        blr
FUNC_END(sysDcrMaltxctp9rSet)

/*******************************************************************************
*
* sysDcrMaltxctp10rGet - get the contents of MAL Transmit Channel 10 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 10
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp10rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp10rGet)

        mfdcr   r3, MAL_TXCTP10R
        blr
FUNC_END(sysDcrMaltxctp10rGet)

/*******************************************************************************
*
* sysDcrMaltxctp10rSet - set the contents of MAL Transmit Channel 10 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Transmit Channel 10 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp10rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp10rSet)

        mtdcr   MAL_TXCTP10R, r3
        blr
FUNC_END(sysDcrMaltxctp10rSet)

/*******************************************************************************
*
* sysDcrMaltxctp11rGet - get the contents of MAL Transmit Channel 11 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 11
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp11rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp11rGet)

        mfdcr   r3, MAL_TXCTP11R
        blr
FUNC_END(sysDcrMaltxctp11rGet)

/*******************************************************************************
*
* sysDcrMaltxctp11rSet - set the contents of MAL Transmit Channel 11 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Transmit Channel 11 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp11rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp11rSet)

        mtdcr   MAL_TXCTP11R, r3
        blr
FUNC_END(sysDcrMaltxctp11rSet)

/*******************************************************************************
*
* sysDcrMaltxctp12rGet - get the contents of MAL Transmit Channel 12 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 12
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp12rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp12rGet)

        mfdcr   r3, MAL_TXCTP12R
        blr
FUNC_END(sysDcrMaltxctp12rGet)

/*******************************************************************************
*
* sysDcrMaltxctp12rSet - set the contents of MAL Transmit Channel 12 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Transmit Channel 12 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp12rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp12rSet)

        mtdcr   MAL_TXCTP12R, r3
        blr
FUNC_END(sysDcrMaltxctp12rSet)

/*******************************************************************************
*
* sysDcrMaltxctp13rGet - get the contents of MAL Transmit Channel 13 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 13
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp13rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp13rGet)

        mfdcr   r3, MAL_TXCTP13R
        blr
FUNC_END(sysDcrMaltxctp13rGet)

/*******************************************************************************
*
* sysDcrMaltxctp13rSet - set the contents of MAL Transmit Channel 13 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Transmit Channel 13 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp13rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp13rSet)

        mtdcr   MAL_TXCTP13R, r3
        blr
FUNC_END(sysDcrMaltxctp13rSet)

/*******************************************************************************
*
* sysDcrMaltxctp14rGet - get the contents of MAL Transmit Channel 14 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 14
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp14rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp14rGet)

        mfdcr   r3, MAL_TXCTP14R
        blr
FUNC_END(sysDcrMaltxctp14rGet)

/*******************************************************************************
*
* sysDcrMaltxctp14rSet - set the contents of MAL Transmit Channel 14 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Transmit Channel 14 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp14rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp14rSet)

        mtdcr   MAL_TXCTP14R, r3
        blr
FUNC_END(sysDcrMaltxctp14rSet)

/*******************************************************************************
*
* sysDcrMaltxctp15rGet - get the contents of MAL Transmit Channel 15 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 15
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp15rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp15rGet)

        mfdcr   r3, MAL_TXCTP15R
        blr
FUNC_END(sysDcrMaltxctp15rGet)

/*******************************************************************************
*
* sysDcrMaltxctp15rSet - set the contents of MAL Transmit Channel 15 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Transmit Channel 15 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp15rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp15rSet)

        mtdcr   MAL_TXCTP15R, r3
        blr
FUNC_END(sysDcrMaltxctp15rSet)

#endif /* MAL_CHANNELS_32 */

#ifdef MAL_CHANNELS_64
/*******************************************************************************
*
* sysDcrMaltxctp16rGet - get the contents of MAL Transmit Channel 16 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 16
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp16rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp16rGet)

        mfdcr   r3, MAL_TXCTP16R
        blr
FUNC_END(sysDcrMaltxctp16rGet)

/*******************************************************************************
*
* sysDcrMaltxctp16rSet - set the contents of MAL Transmit Channel 16 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Transmit Channel 16 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp16rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp16rSet)

        mtdcr   MAL_TXCTP16R, r3
        blr
FUNC_END(sysDcrMaltxctp16rSet)

/*******************************************************************************
*
* sysDcrMaltxctp17rGet - get the contents of MAL Transmit Channel 17 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 17
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp17rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp17rGet)

        mfdcr   r3, MAL_TXCTP17R
        blr
FUNC_END(sysDcrMaltxctp17rGet)

/*******************************************************************************
*
* sysDcrMaltxctp17rSet - set the contents of MAL Transmit Channel 17 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Transmit Channel 17 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp17rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp17rSet)

        mtdcr   MAL_TXCTP17R, r3
        blr
FUNC_END(sysDcrMaltxctp17rSet)

/*******************************************************************************
*
* sysDcrMaltxctp18rGet - get the contents of MAL Transmit Channel 18 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 18
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp18rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp18rGet)

        mfdcr   r3, MAL_TXCTP18R
        blr
FUNC_END(sysDcrMaltxctp18rGet)

/*******************************************************************************
*
* sysDcrMaltxctp18rSet - set the contents of MAL Transmit Channel 18 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Transmit Channel 18 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp18rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp18rSet)

        mtdcr   MAL_TXCTP18R, r3
        blr
FUNC_END(sysDcrMaltxctp18rSet)

/*******************************************************************************
*
* sysDcrMaltxctp19rGet - get the contents of MAL Transmit Channel 19 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 19
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp19rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp19rGet)

        mfdcr   r3, MAL_TXCTP19R
        blr
FUNC_END(sysDcrMaltxctp19rGet)

/*******************************************************************************
*
* sysDcrMaltxctp19rSet - set the contents of MAL Transmit Channel 19 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Transmit Channel 19 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp19rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp19rSet)

        mtdcr   MAL_TXCTP19R, r3
        blr
FUNC_END(sysDcrMaltxctp19rSet)

/*******************************************************************************
*
* sysDcrMaltxctp20rGet - get the contents of MAL Transmit Channel 20 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 20
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp20rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp20rGet)

        mfdcr   r3, MAL_TXCTP20R
        blr
FUNC_END(sysDcrMaltxctp20rGet)

/*******************************************************************************
*
* sysDcrMaltxctp20rSet - set the contents of MAL Transmit Channel 20 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Transmit Channel 20 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp20rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp20rSet)

        mtdcr   MAL_TXCTP20R, r3
        blr
FUNC_END(sysDcrMaltxctp20rSet)

/*******************************************************************************
*
* sysDcrMaltxctp21rGet - get the contents of MAL Transmit Channel 21 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 21
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp21rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp21rGet)

        mfdcr   r3, MAL_TXCTP21R
        blr
FUNC_END(sysDcrMaltxctp21rGet)

/*******************************************************************************
*
* sysDcrMaltxctp21rSet - set the contents of MAL Transmit Channel 21 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Transmit Channel 21 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp21rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp21rSet)

        mtdcr   MAL_TXCTP21R, r3
        blr
FUNC_END(sysDcrMaltxctp21rSet)

/*******************************************************************************
*
* sysDcrMaltxctp22rGet - get the contents of MAL Transmit Channel 22 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 22
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp22rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp22rGet)

        mfdcr   r3, MAL_TXCTP22R
        blr
FUNC_END(sysDcrMaltxctp22rGet)


/*******************************************************************************
*
* sysDcrMaltxctp22rSet - set the contents of MAL Transmit Channel 22 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Transmit Channel 22 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp22rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp22rSet)

        mtdcr   MAL_TXCTP22R, r3
        blr
FUNC_END(sysDcrMaltxctp22rSet)

/*******************************************************************************
*
* sysDcrMaltxctp23rGet - get the contents of MAL Transmit Channel 23 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 23
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp23rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp23rGet)

        mfdcr   r3, MAL_TXCTP23R
        blr
FUNC_END(sysDcrMaltxctp23rGet)

/*******************************************************************************
*
* sysDcrMaltxctp23rSet - set the contents of MAL Transmit Channel 23 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Transmit Channel 23 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp23rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp23rSet)

        mtdcr   MAL_TXCTP23R, r3
        blr
FUNC_END(sysDcrMaltxctp23rSet)

/*******************************************************************************
*
* sysDcrMaltxctp24rGet - get the contents of MAL Transmit Channel 24 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 24
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp24rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp24rGet)

        mfdcr   r3, MAL_TXCTP24R
        blr
FUNC_END(sysDcrMaltxctp24rGet)

/*******************************************************************************
*
* sysDcrMaltxctp24rSet - set the contents of MAL Transmit Channel 24 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Transmit Channel 24 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp24rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp24rSet)

        mtdcr   MAL_TXCTP24R, r3
        blr
FUNC_END(sysDcrMaltxctp24rSet)

/*******************************************************************************
*
* sysDcrMaltxctp25rGet - get the contents of MAL Transmit Channel 25 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 25
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp25rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp25rGet)

        mfdcr   r3, MAL_TXCTP25R
        blr
FUNC_END(sysDcrMaltxctp25rGet)

/*******************************************************************************
*
* sysDcrMaltxctp25rSet - set the contents of MAL Transmit Channel 25 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Transmit Channel 25 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp25rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp25rSet)

        mtdcr   MAL_TXCTP25R, r3
        blr
FUNC_END(sysDcrMaltxctp25rSet)

/*******************************************************************************
*
* sysDcrMaltxctp26rGet - get the contents of MAL Transmit Channel 26 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 26
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp26rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp26rGet)

        mfdcr   r3, MAL_TXCTP26R
        blr
FUNC_END(sysDcrMaltxctp26rGet)

/*******************************************************************************
*
* sysDcrMaltxctp26rSet - set the contents of MAL Transmit Channel 26 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Transmit Channel 26 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp26rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp26rSet)

        mtdcr   MAL_TXCTP26R, r3
        blr
FUNC_END(sysDcrMaltxctp26rSet)

/*******************************************************************************
*
* sysDcrMaltxctp27rGet - get the contents of MAL Transmit Channel 27 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 27
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp27rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp27rGet)

        mfdcr   r3, MAL_TXCTP27R
        blr
FUNC_END(sysDcrMaltxctp27rGet)

/*******************************************************************************
*
* sysDcrMaltxctp27rSet - set the contents of MAL Transmit Channel 27 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Transmit Channel 27 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp27rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp27rSet)

        mtdcr   MAL_TXCTP27R, r3
        blr
FUNC_END(sysDcrMaltxctp27rSet)

/*******************************************************************************
*
* sysDcrMaltxctp28rGet - get the contents of MAL Transmit Channel 28 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 28
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp28rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp28rGet)

        mfdcr   r3, MAL_TXCTP28R
        blr
FUNC_END(sysDcrMaltxctp28rGet)

/*******************************************************************************
*
* sysDcrMaltxctp28rSet - set the contents of MAL Transmit Channel 28 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Transmit Channel 28 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp28rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp28rSet)

        mtdcr   MAL_TXCTP28R, r3
        blr
FUNC_END(sysDcrMaltxctp28rSet)

/*******************************************************************************
*
* sysDcrMaltxctp29rGet - get the contents of MAL Transmit Channel 29 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 29
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp29rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp29rGet)

        mfdcr   r3, MAL_TXCTP29R
        blr
FUNC_END(sysDcrMaltxctp29rGet)

/*******************************************************************************
*
* sysDcrMaltxctp29rSet - set the contents of MAL Transmit Channel 29 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Transmit Channel 29 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp29rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp29rSet)

        mtdcr   MAL_TXCTP29R, r3
        blr
FUNC_END(sysDcrMaltxctp29rSet)

/*******************************************************************************
*
* sysDcrMaltxctp30rGet - get the contents of MAL Transmit Channel 30 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 30
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp30rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp30rGet)

        mfdcr   r3, MAL_TXCTP30R
        blr
FUNC_END(sysDcrMaltxctp30rGet)

/*******************************************************************************
*
* sysDcrMaltxctp30rSet - set the contents of MAL Transmit Channel 30 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Transmit Channel 30 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp30rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp30rSet)

        mtdcr   MAL_TXCTP30R, r3
        blr
FUNC_END(sysDcrMaltxctp30rSet)

/*******************************************************************************
*
* sysDcrMaltxctp31rGet - get the contents of MAL Transmit Channel 31 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Transmit Channel 31
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMaltxctp31rGet (void)

*/

FUNC_BEGIN(sysDcrMaltxctp31rGet)

        mfdcr   r3, MAL_TXCTP31R
        blr
FUNC_END(sysDcrMaltxctp31rGet)

/*******************************************************************************
*
* sysDcrMaltxctp31rSet - set the contents of MAL Transmit Channel 31 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Transmit Channel 31 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMaltxctp31rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMaltxctp31rSet)

        mtdcr   MAL_TXCTP31R, r3
        blr
FUNC_END(sysDcrMaltxctp31rSet)

#endif /* MAL_CHANNELS_64 */

/*******************************************************************************
*
* sysDcrMalrxctp0rGet - get the contents of MAL Receive Channel 0 Table Pointer
*                       Register
*
* This routine reads and returns the contents of the MAL Receive Channel 0
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp0rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp0rGet)

        mfdcr   r3, MAL_RXCTP0R
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrMalrxctp0rGet)

/*******************************************************************************
*
* sysDcrMalrxctp0rSet - set the contents of MAL Receive Channel 0 Table Pointer
*                      Register
*
* This routine writes the contents of the MAL Receive Channel 0 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp0rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp0rSet)

        mtdcr   MAL_RXCTP0R, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrMalrxctp0rSet)

/*******************************************************************************
*
* sysDcrMalrxctp1rGet - get the contents of MAL Receive Channel 1 Table Pointer
*                       Register
*
* This routine reads and returns the contents of the MAL Receive Channel 1
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp1rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp1rGet)

        mfdcr   r3, MAL_RXCTP1R
        blr
FUNC_END(sysDcrMalrxctp1rGet)

/*******************************************************************************
*
* sysDcrMalrxctp1rSet - set the contents of MAL Receive Channel 1 Table Pointer
*                      Register
*
* This routine writes the contents of the MAL Receive Channel 1 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp1rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp1rSet)

        mtdcr   MAL_RXCTP1R, r3
        blr
FUNC_END(sysDcrMalrxctp1rSet)

/*******************************************************************************
*
* sysDcrMalrxctp2rGet - get the contents of MAL Receive Channel 2 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Receive Channel 2
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp2rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp2rGet)

        mfdcr   r3, MAL_RXCTP2R
        blr
FUNC_END(sysDcrMalrxctp2rGet)

/*******************************************************************************
*
* sysDcrMalrxctp2rSet - set the contents of MAL Receive Channel 2 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Receive Channel 2 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp2rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp2rSet)

        mtdcr   MAL_RXCTP2R, r3
        blr
FUNC_END(sysDcrMalrxctp2rSet)

/*******************************************************************************
*
* sysDcrMalrxctp3rGet - get the contents of MAL Receive Channel 3 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Receive Channel 3
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp3rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp3rGet)

        mfdcr   r3, MAL_RXCTP3R
        blr
FUNC_END(sysDcrMalrxctp3rGet)

/*******************************************************************************
*
* sysDcrMalrxctp3rSet - set the contents of MAL Receive Channel 3 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Receive Channel 3 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp3rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp3rSet)

        mtdcr   MAL_RXCTP3R, r3
        blr
FUNC_END(sysDcrMalrxctp3rSet)

#ifdef MAL_CHANNELS_32
/*******************************************************************************
*
* sysDcrMalrxctp4rGet - get the contents of MAL Receive Channel 4 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Receive Channel 4
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp4rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp4rGet)

        mfdcr   r3, MAL_RXCTP4R
        blr
FUNC_END(sysDcrMalrxctp4rGet)

/*******************************************************************************
*
* sysDcrMalrxctp4rSet - set the contents of MAL Receive Channel 4 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Receive Channel 4 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp4rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp4rSet)

        mtdcr   MAL_RXCTP4R, r3
        blr
FUNC_END(sysDcrMalrxctp4rSet)

/*******************************************************************************
*
* sysDcrMalrxctp5rGet - get the contents of MAL Receive Channel 5 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Receive Channel 5
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp5rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp5rGet)

        mfdcr   r3, MAL_RXCTP5R
        blr
FUNC_END(sysDcrMalrxctp5rGet)

/*******************************************************************************
*
* sysDcrMalrxctp5rSet - set the contents of MAL Receive Channel 5 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Receive Channel 5 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp5rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp5rSet)

        mtdcr   MAL_RXCTP5R, r3
        blr
FUNC_END(sysDcrMalrxctp5rSet)

/*******************************************************************************
*
* sysDcrMalrxctp6rGet - get the contents of MAL Receive Channel 6 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Receive Channel 6
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp6rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp6rGet)

        mfdcr   r3, MAL_RXCTP6R
        blr
FUNC_END(sysDcrMalrxctp6rGet)

/*******************************************************************************
*
* sysDcrMalrxctp6rSet - set the contents of MAL Receive Channel 6 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Receive Channel 6 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp6rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp6rSet)

        mtdcr   MAL_RXCTP6R, r3
        blr
FUNC_END(sysDcrMalrxctp6rSet)

/*******************************************************************************
*
* sysDcrMalrxctp7rGet - get the contents of MAL Receive Channel 7 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Receive Channel 7
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp7rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp7rGet)

        mfdcr   r3, MAL_RXCTP7R
        blr
FUNC_END(sysDcrMalrxctp7rGet)

/*******************************************************************************
*
* sysDcrMalrxctp7rSet - set the contents of MAL Receive Channel 7 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Receive Channel 7 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp7rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp7rSet)

        mtdcr   MAL_RXCTP7R, r3
        blr
FUNC_END(sysDcrMalrxctp7rSet)

/*******************************************************************************
*
* sysDcrMalrxctp8rGet - get the contents of MAL Receive Channel 8 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Receive Channel 8
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp8rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp8rGet)

        mfdcr   r3, MAL_RXCTP8R
        blr
FUNC_END(sysDcrMalrxctp8rGet)

/*******************************************************************************
*
* sysDcrMalrxctp8rSet - set the contents of MAL Receive Channel 8 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Receive Channel 8 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp8rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp8rSet)

        mtdcr   MAL_RXCTP8R, r3
        blr
FUNC_END(sysDcrMalrxctp8rSet)

/*******************************************************************************
*
* sysDcrMalrxctp9rGet - get the contents of MAL Receive Channel 9 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Receive Channel 9
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp9rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp9rGet)

        mfdcr   r3, MAL_RXCTP9R
        blr
FUNC_END(sysDcrMalrxctp9rGet)

/*******************************************************************************
*
* sysDcrMalrxctp9rSet - set the contents of MAL Receive Channel 9 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Receive Channel 9 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp9rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp9rSet)

        mtdcr   MAL_RXCTP9R, r3
        blr
FUNC_END(sysDcrMalrxctp9rSet)

/*******************************************************************************
*
* sysDcrMalrxctp10rGet - get the contents of MAL Receive Channel 10 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Receive Channel 10
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp10rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp10rGet)

        mfdcr   r3, MAL_RXCTP10R
        blr
FUNC_END(sysDcrMalrxctp10rGet)

/*******************************************************************************
*
* sysDcrMalrxctp10rSet - set the contents of MAL Receive Channel 10 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Receive Channel 10 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp10rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp10rSet)

        mtdcr   MAL_RXCTP10R, r3
        blr
FUNC_END(sysDcrMalrxctp10rSet)

/*******************************************************************************
*
* sysDcrMalrxctp11rGet - get the contents of MAL Receive Channel 11 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Receive Channel 11
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp11rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp11rGet)

        mfdcr   r3, MAL_RXCTP11R
        blr
FUNC_END(sysDcrMalrxctp11rGet)

/*******************************************************************************
*
* sysDcrMalrxctp11rSet - set the contents of MAL Receive Channel 11 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Receive Channel 11 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp11rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp11rSet)

        mtdcr   MAL_RXCTP11R, r3
        blr
FUNC_END(sysDcrMalrxctp11rSet)

/*******************************************************************************
*
* sysDcrMalrxctp12rGet - get the contents of MAL Receive Channel 12 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Receive Channel 12
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp12rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp12rGet)

        mfdcr   r3, MAL_RXCTP12R
        blr
FUNC_END(sysDcrMalrxctp12rGet)

/*******************************************************************************
*
* sysDcrMalrxctp12rSet - set the contents of MAL Receive Channel 12 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Receive Channel 12 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp12rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp12rSet)

        mtdcr   MAL_RXCTP12R, r3
        blr
FUNC_END(sysDcrMalrxctp12rSet)

/*******************************************************************************
*
* sysDcrMalrxctp13rGet - get the contents of MAL Receive Channel 13 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Receive Channel 13
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp13rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp13rGet)

        mfdcr   r3, MAL_RXCTP13R
        blr
FUNC_END(sysDcrMalrxctp13rGet)

/*******************************************************************************
*
* sysDcrMalrxctp13rSet - set the contents of MAL Receive Channel 13 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Receive Channel 13 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp13rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp13rSet)

        mtdcr   MAL_RXCTP13R, r3
        blr
FUNC_END(sysDcrMalrxctp13rSet)

/*******************************************************************************
*
* sysDcrMalrxctp14rGet - get the contents of MAL Receive Channel 14 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Receive Channel 14
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp14rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp14rGet)

        mfdcr   r3, MAL_RXCTP14R
        blr
FUNC_END(sysDcrMalrxctp14rGet)

/*******************************************************************************
*
* sysDcrMalrxctp14rSet - set the contents of MAL Receive Channel 14 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Receive Channel 14 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp14rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp14rSet)

        mtdcr   MAL_RXCTP14R, r3
        blr
FUNC_END(sysDcrMalrxctp14rSet)

/*******************************************************************************
*
* sysDcrMalrxctp15rGet - get the contents of MAL Receive Channel 15 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Receive Channel 15
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp15rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp15rGet)

        mfdcr   r3, MAL_RXCTP15R
        blr
FUNC_END(sysDcrMalrxctp15rGet)

/*******************************************************************************
*
* sysDcrMalrxctp15rSet - set the contents of MAL Receive Channel 15 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Receive Channel 15 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp15rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp15rSet)

        mtdcr   MAL_RXCTP15R, r3
        blr
FUNC_END(sysDcrMalrxctp15rSet)

#endif /* MAL_CHANNELS_32 */

#ifdef MAL_CHANNELS_64
/*******************************************************************************
*
* sysDcrMalrxctp16rGet - get the contents of MAL Receive Channel 16 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Receive Channel 16
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp16rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp16rGet)

        mfdcr   r3, MAL_RXCTP16R
        blr
FUNC_END(sysDcrMalrxctp16rGet)

/*******************************************************************************
*
* sysDcrMalrxctp16rSet - set the contents of MAL Receive Channel 16 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Receive Channel 16 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp16rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp16rSet)

        mtdcr   MAL_RXCTP16R, r3
        blr
FUNC_END(sysDcrMalrxctp16rSet)
	
/*******************************************************************************
*
* sysDcrMalrxctp17rGet - get the contents of MAL Receive Channel 17 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Receive Channel 17
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp17rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp17rGet)

        mfdcr   r3, MAL_RXCTP17R
        blr
FUNC_END(sysDcrMalrxctp17rGet)

/*******************************************************************************
*
* sysDcrMalrxctp17rSet - set the contents of MAL Receive Channel 17 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Receive Channel 17 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp17rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp17rSet)

        mtdcr   MAL_RXCTP17R, r3
        blr
FUNC_END(sysDcrMalrxctp17rSet)

/*******************************************************************************
*
* sysDcrMalrxctp18rGet - get the contents of MAL Receive Channel 18 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Receive Channel 18
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp18rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp18rGet)

        mfdcr   r3, MAL_RXCTP18R
        blr
FUNC_END(sysDcrMalrxctp18rGet)

/*******************************************************************************
*
* sysDcrMalrxctp18rSet - set the contents of MAL Receive Channel 18 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Receive Channel 18 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp18rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp18rSet)

        mtdcr   MAL_RXCTP18R, r3
        blr
FUNC_END(sysDcrMalrxctp18rSet)

/*******************************************************************************
*
* sysDcrMalrxctp19rGet - get the contents of MAL Receive Channel 19 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Receive Channel 19
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp19rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp19rGet)

        mfdcr   r3, MAL_RXCTP19R
        blr
FUNC_END(sysDcrMalrxctp19rGet)

/*******************************************************************************
*
* sysDcrMalrxctp19rSet - set the contents of MAL Receive Channel 19 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Receive Channel 19 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp19rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp19rSet)

        mtdcr   MAL_RXCTP19R, r3
        blr
FUNC_END(sysDcrMalrxctp19rSet)

/*******************************************************************************
*
* sysDcrMalrxctp20rGet - get the contents of MAL Receive Channel 20 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Receive Channel 20
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp20rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp20rGet)

        mfdcr   r3, MAL_RXCTP20R
        blr
FUNC_END(sysDcrMalrxctp20rGet)

/*******************************************************************************
*
* sysDcrMalrxctp20rSet - set the contents of MAL Receive Channel 20 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Receive Channel 20 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp20rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp20rSet)

        mtdcr   MAL_RXCTP20R, r3
        blr
FUNC_END(sysDcrMalrxctp20rSet)

/*******************************************************************************
*
* sysDcrMalrxctp21rGet - get the contents of MAL Receive Channel 21 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Receive Channel 21
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp21rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp21rGet)

        mfdcr   r3, MAL_RXCTP21R
        blr
FUNC_END(sysDcrMalrxctp21rGet)

/*******************************************************************************
*
* sysDcrMalrxctp21rSet - set the contents of MAL Receive Channel 21 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Receive Channel 21 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp21rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp21rSet)

        mtdcr   MAL_RXCTP21R, r3
        blr
FUNC_END(sysDcrMalrxctp21rSet)

/*******************************************************************************
*
* sysDcrMalrxctp22rGet - get the contents of MAL Receive Channel 22 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Receive Channel 22
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp22rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp22rGet)

        mfdcr   r3, MAL_RXCTP22R
        blr
FUNC_END(sysDcrMalrxctp22rGet)

/*******************************************************************************
*
* sysDcrMalrxctp22rSet - set the contents of MAL Receive Channel 22 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Receive Channel 22 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp22rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp22rSet)

        mtdcr   MAL_RXCTP22R, r3
        blr
FUNC_END(sysDcrMalrxctp22rSet)

/*******************************************************************************
*
* sysDcrMalrxctp23rGet - get the contents of MAL Receive Channel 23 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Receive Channel 23
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp23rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp23rGet)

        mfdcr   r3, MAL_RXCTP23R
        blr
FUNC_END(sysDcrMalrxctp23rGet)

/*******************************************************************************
*
* sysDcrMalrxctp23rSet - set the contents of MAL Receive Channel 23 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Receive Channel 23 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp23rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp23rSet)

        mtdcr   MAL_RXCTP23R, r3
        blr
FUNC_END(sysDcrMalrxctp23rSet)

/*******************************************************************************
*
* sysDcrMalrxctp24rGet - get the contents of MAL Receive Channel 24 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Receive Channel 24
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp24rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp24rGet)

        mfdcr   r3, MAL_RXCTP24R
        blr
FUNC_END(sysDcrMalrxctp24rGet)

/*******************************************************************************
*
* sysDcrMalrxctp24rSet - set the contents of MAL Receive Channel 24 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Receive Channel 24 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp24rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp24rSet)

        mtdcr   MAL_RXCTP24R, r3
        blr
FUNC_END(sysDcrMalrxctp24rSet)

/*******************************************************************************
*
* sysDcrMalrxctp25rGet - get the contents of MAL Receive Channel 25 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Receive Channel 25
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp25rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp25rGet)

        mfdcr   r3, MAL_RXCTP25R
        blr
FUNC_END(sysDcrMalrxctp25rGet)

/*******************************************************************************
*
* sysDcrMalrxctp25rSet - set the contents of MAL Receive Channel 25 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Receive Channel 25 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp25rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp25rSet)

        mtdcr   MAL_RXCTP25R, r3
        blr
FUNC_END(sysDcrMalrxctp25rSet)

/*******************************************************************************
*
* sysDcrMalrxctp26rGet - get the contents of MAL Receive Channel 26 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Receive Channel 26
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp26rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp26rGet)

        mfdcr   r3, MAL_RXCTP26R
        blr
FUNC_END(sysDcrMalrxctp26rGet)

/*******************************************************************************
*
* sysDcrMalrxctp26rSet - set the contents of MAL Receive Channel 26 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Receive Channel 26 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp26rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp26rSet)

        mtdcr   MAL_RXCTP26R, r3
        blr
FUNC_END(sysDcrMalrxctp26rSet)

/*******************************************************************************
*
* sysDcrMalrxctp27rGet - get the contents of MAL Receive Channel 27 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Receive Channel 27
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp27rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp27rGet)

        mfdcr   r3, MAL_RXCTP27R
        blr
FUNC_END(sysDcrMalrxctp27rGet)

/*******************************************************************************
*
* sysDcrMalrxctp27rSet - set the contents of MAL Receive Channel 27 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Receive Channel 27 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp27rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp27rSet)

        mtdcr   MAL_RXCTP27R, r3
        blr
FUNC_END(sysDcrMalrxctp27rSet)

/*******************************************************************************
*
* sysDcrMalrxctp28rGet - get the contents of MAL Receive Channel 28 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Receive Channel 28
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp28rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp28rGet)

        mfdcr   r3, MAL_RXCTP28R
        blr
FUNC_END(sysDcrMalrxctp28rGet)

/*******************************************************************************
*
* sysDcrMalrxctp28rSet - set the contents of MAL Receive Channel 28 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Receive Channel 28 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp28rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp28rSet)

        mtdcr   MAL_RXCTP28R, r3
        blr
FUNC_END(sysDcrMalrxctp28rSet)

/*******************************************************************************
*
* sysDcrMalrxctp29rGet - get the contents of MAL Receive Channel 29 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Receive Channel 29
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp29rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp29rGet)

        mfdcr   r3, MAL_RXCTP29R
        blr
FUNC_END(sysDcrMalrxctp29rGet)

/*******************************************************************************
*
* sysDcrMalrxctp29rSet - set the contents of MAL Receive Channel 29 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Receive Channel 29 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp29rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp29rSet)

        mtdcr   MAL_RXCTP29R, r3
        blr
FUNC_END(sysDcrMalrxctp29rSet)

/*******************************************************************************
*
* sysDcrMalrxctp30rGet - get the contents of MAL Receive Channel 30 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Receive Channel 30
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp30rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp30rGet)

        mfdcr   r3, MAL_RXCTP30R
        blr
FUNC_END(sysDcrMalrxctp30rGet)

/*******************************************************************************
*
* sysDcrMalrxctp30rSet - set the contents of MAL Receive Channel 30 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Receive Channel 30 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp30rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp30rSet)

        mtdcr   MAL_RXCTP30R, r3
        blr
FUNC_END(sysDcrMalrxctp30rSet)

/*******************************************************************************
*
* sysDcrMalrxctp31rGet - get the contents of MAL Receive Channel 31 Table
*                        Pointer Register
*
* This routine reads and returns the contents of the MAL Receive Channel 31
* Table Pointer Register.
*
* NOMANUAL

* UINT32 sysDcrMalrxctp31rGet (void)

*/

FUNC_BEGIN(sysDcrMalrxctp31rGet)

        mfdcr   r3, MAL_RXCTP31R
        blr
FUNC_END(sysDcrMalrxctp31rGet)

/*******************************************************************************
*
* sysDcrMalrxctp31rSet - set the contents of MAL Receive Channel 31 Table
*                        Pointer Register
*
* This routine writes the contents of the MAL Receive Channel 31 Table Pointer
* Register.
*
* NOMANUAL

* void sysDcrMalrxctp31rSet (UINT32)

*/

FUNC_BEGIN(sysDcrMalrxctp31rSet)

        mtdcr   MAL_RXCTP31R, r3
        blr
FUNC_END(sysDcrMalrxctp31rSet)

#endif /* MAL_CHANNELS_64 */


/*******************************************************************************
*
* sysDcrMalrcbs0Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 0
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs0Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs0Get)

        mfdcr   r3, MAL_RCBS0
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrMalrcbs0Get)

/*******************************************************************************
*
* sysDcrMalrcbs0Set - set the contents of MAL Receive Channel 0 Buffer Size
*                     Register for RX channel 0
*
* This routine writes the contents of the MAL Receive Channel 0 Buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs0Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs0Set)

        mtdcr   MAL_RCBS0, r3
#if defined (PPC405GP_REVB) || defined (PPC405GP_REVC)   /* revB/C errata # 8 */
        nop
        nop
        nop
#endif
        blr
FUNC_END(sysDcrMalrcbs0Set)

/*******************************************************************************
*
* sysDcrMalrcbs1Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 1
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs1Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs1Get)

        mfdcr   r3, MAL_RCBS1
        blr
FUNC_END(sysDcrMalrcbs1Get)

/*******************************************************************************
*
* sysDcrMalrcbs1Set - set the contents of MAL Receive Channel 1 Buffer Size
*                     Register
*
* This routine writes the contents of the MAL Receive Channel 1 Buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs1Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs1Set)

        mtdcr   MAL_RCBS1, r3
        blr
FUNC_END(sysDcrMalrcbs1Set)

/*******************************************************************************
*
* sysDcrMalrcbs2Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 2
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs2Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs2Get)

        mfdcr   r3, MAL_RCBS2
        blr
FUNC_END(sysDcrMalrcbs2Get)

/*******************************************************************************
*
* sysDcrMalrcbs2Set - set the contents of MAL Receive Channel 2 Buffer Size
*                     Register
*
* This routine writes the contents of the MAL Receive Channel 2 Buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs2Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs2Set)

        mtdcr   MAL_RCBS2, r3
        blr
FUNC_END(sysDcrMalrcbs2Set)

/*******************************************************************************
*
* sysDcrMalrcbs3Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 3
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs3Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs3Get)

        mfdcr   r3, MAL_RCBS3
        blr
FUNC_END(sysDcrMalrcbs3Get)

/*******************************************************************************
*
* sysDcrMalrcbs3Set - set the contents of MAL Receive Channel 3 Buffer Size
*                     Register
*
* This routine writes the contents of the MAL Receive Channel 3 Buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs3Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs3Set)

        mtdcr   MAL_RCBS3, r3
        blr
FUNC_END(sysDcrMalrcbs3Set)


#ifdef MAL_CHANNELS_32
/*******************************************************************************
*
* sysDcrMalrcbs4Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 4
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs4Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs4Get)

        mfdcr   r3, MAL_RCBS4
        blr
FUNC_END(sysDcrMalrcbs4Get)

/*******************************************************************************
*
* sysDcrMalrcbs4Set - set the contents of MAL Receive Channel 4 Buffer Size
*                     Register
*
* This routine writes the contents of the MAL Receive Channel 4 Buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs4Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs4Set)

        mtdcr   MAL_RCBS4, r3
        blr
FUNC_END(sysDcrMalrcbs4Set)

/*******************************************************************************
*
* sysDcrMalrcbs5Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 5
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs5Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs5Get)

        mfdcr   r3, MAL_RCBS5
        blr
FUNC_END(sysDcrMalrcbs5Get)

/*******************************************************************************
*
* sysDcrMalrcbs5Set - set the contents of MAL Receive Channel 5 Buffer Size
*                     Register
*
* This routine writes the contents of the MAL Receive Channel 5 Buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs5Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs5Set)

        mtdcr   MAL_RCBS5, r3
        blr
FUNC_END(sysDcrMalrcbs5Set)

/*******************************************************************************
*
* sysDcrMalrcbs6Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 6
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs6Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs6Get)

        mfdcr   r3, MAL_RCBS6
        blr
FUNC_END(sysDcrMalrcbs6Get)

/*******************************************************************************
*
* sysDcrMalrcbs6Set - set the contents of MAL Receive Channel 6 Buffer Size
*                     Register
*
* This routine writes the contents of the MAL Receive Channel 6 Buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs6Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs6Set)

        mtdcr   MAL_RCBS6, r3
        blr
FUNC_END(sysDcrMalrcbs6Set)

/*******************************************************************************
*
* sysDcrMalrcbs7Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 7
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs7Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs7Get)

        mfdcr   r3, MAL_RCBS7
        blr
FUNC_END(sysDcrMalrcbs7Get)

/*******************************************************************************
*
* sysDcrMalrcbs7Set - set the contents of MAL Receive Channel 7 Buffer Size
*                     Register
*
* This routine writes the contents of the MAL Receive Channel 7 Buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs7Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs7Set)

        mtdcr   MAL_RCBS7, r3
        blr
FUNC_END(sysDcrMalrcbs7Set)

/*******************************************************************************
*
* sysDcrMalrcbs8Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 8
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs8Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs8Get)

        mfdcr   r3, MAL_RCBS8
        blr
FUNC_END(sysDcrMalrcbs8Get)

/*******************************************************************************
*
* sysDcrMalrcbs8Set - set the contents of MAL Receive Channel 8 Buffer Size
*                     Register
*
* This routine writes the contents of the MAL Receive Channel 8 Buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs8Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs8Set)

        mtdcr   MAL_RCBS8, r3
        blr
FUNC_END(sysDcrMalrcbs8Set)

/*******************************************************************************
*
* sysDcrMalrcbs9Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 9
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs9Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs9Get)

        mfdcr   r3, MAL_RCBS9
        blr
FUNC_END(sysDcrMalrcbs9Get)

/*******************************************************************************
*
* sysDcrMalrcbs9Set - set the contents of MAL Receive Channel 9 Buffer Size
*                     Register
*
* This routine writes the contents of the MAL Receive Channel 0 Buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs9Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs9Set)

        mtdcr   MAL_RCBS9, r3
        blr
FUNC_END(sysDcrMalrcbs9Set)

/*******************************************************************************
*
* sysDcrMalrcbs10Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 10
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs10Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs10Get)

        mfdcr   r3, MAL_RCBS10
        blr
FUNC_END(sysDcrMalrcbs10Get)

/*******************************************************************************
*
* sysDcrMalrcbs10Set - set the contents of MAL Receive Channel 10 Buffer Size
*                     Register
*
* This routine writes the contents of the MAL Receive Channel 10 Buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs10Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs10Set)

        mtdcr   MAL_RCBS10, r3
        blr
FUNC_END(sysDcrMalrcbs10Set)

/*******************************************************************************
*
* sysDcrMalrcbs11Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 11
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs11Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs11Get)

        mfdcr   r3, MAL_RCBS11
        blr
FUNC_END(sysDcrMalrcbs11Get)

/*******************************************************************************
*
* sysDcrMalrcbs11Set - set the contents of MAL Receive Channel 11 Buffer Size
*                     Register
*
* This routine writes the contents of the MAL Receive Channel 11 Buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs11Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs11Set)

        mtdcr   MAL_RCBS11, r3
        blr
FUNC_END(sysDcrMalrcbs11Set)

/*******************************************************************************
*
* sysDcrMalrcbs12Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 12
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs12Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs12Get)

        mfdcr   r3, MAL_RCBS12
        blr
FUNC_END(sysDcrMalrcbs12Get)

/*******************************************************************************
*
* sysDcrMalrcbs12Set - set the contents of MAL Receive Channel 12 Buffer Size
*                     Register
*
* This routine writes the contents of the MAL Receive Channel 0 Buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs12Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs12Set)

        mtdcr   MAL_RCBS12, r3
        blr
FUNC_END(sysDcrMalrcbs12Set)

/*******************************************************************************
*
* sysDcrMalrcbs13Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 13
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs13Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs13Get)

        mfdcr   r3, MAL_RCBS13
        blr
FUNC_END(sysDcrMalrcbs13Get)

/*******************************************************************************
*
* sysDcrMalrcbs13Set - set the contents of MAL Receive Channel 13 Buffer Size
*                     Register
*
* This routine writes the contents of the MAL Receive Channel 0 Buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs13Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs13Set)

        mtdcr   MAL_RCBS13, r3
        blr
FUNC_END(sysDcrMalrcbs13Set)

/*******************************************************************************
*
* sysDcrMalrcbs14Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 14
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs14Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs14Get)

        mfdcr   r3, MAL_RCBS14
        blr
FUNC_END(sysDcrMalrcbs14Get)

/*******************************************************************************
*
* sysDcrMalrcbs14Set - set the contents of MAL Receive Channel 14 Buffer Size
*                     Register
*
* This routine writes the contents of the MAL Receive Channel 0 Buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs14Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs14Set)

        mtdcr   MAL_RCBS14, r3
        blr
FUNC_END(sysDcrMalrcbs14Set)

/*******************************************************************************
*
* sysDcrMalrcbs15Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 15
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs15Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs15Get)

        mfdcr   r3, MAL_RCBS15
        blr
FUNC_END(sysDcrMalrcbs15Get)

/*******************************************************************************
*
* sysDcrMalrcbs15Set - set the contents of MAL Receive Channel 15 Buffer Size
*                     Register
*
* This routine writes the contents of the MAL Receive Channel 15 buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs15Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs15Set)

        mtdcr   MAL_RCBS15, r3
        blr
FUNC_END(sysDcrMalrcbs15Set)

#endif /* MAL_CHANNELS_32 */

#ifdef MAL_CHANNELS_64
/*******************************************************************************
*
* sysDcrMalrcbs16Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 16
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs16Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs16Get)

        mfdcr   r3, MAL_RCBS16
        blr
FUNC_END(sysDcrMalrcbs16Get)

/*******************************************************************************
*
* sysDcrMalrcbs16Set - set the contents of MAL Receive Channel 16 Buffer Size
*                     Register
*
* This routine writes the contents of the MAL Receive Channel 16 Buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs16Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs16Set)

        mtdcr   MAL_RCBS16, r3
        blr
FUNC_END(sysDcrMalrcbs16Set)

/*******************************************************************************
*
* sysDcrMalrcbs17Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 17
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs17Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs17Get)

        mfdcr   r3, MAL_RCBS17
        blr
FUNC_END(sysDcrMalrcbs17Get)

/*******************************************************************************
*
* sysDcrMalrcbs17Set - set the contents of MAL Receive Channel 17 Buffer Size
*                     Register
*
* This routine writes the contents of the MAL Receive Channel 17 Buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs17Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs17Set)

        mtdcr   MAL_RCBS17, r3
        blr
FUNC_END(sysDcrMalrcbs17Set)

/*******************************************************************************
*
* sysDcrMalrcbs18Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 18
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs18Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs18Get)

        mfdcr   r3, MAL_RCBS18
        blr
FUNC_END(sysDcrMalrcbs18Get)

/*******************************************************************************
*
* sysDcrMalrcbs18Set - set the contents of MAL Receive Channel 18 Buffer Size
*                     Register
*
* This routine writes the contents of the MAL Receive Channel 18 Buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs18Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs18Set)

        mtdcr   MAL_RCBS18, r3
        blr
FUNC_END(sysDcrMalrcbs18Set)

/*******************************************************************************
*
* sysDcrMalrcbs19Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 19
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs19Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs19Get)

        mfdcr   r3, MAL_RCBS19
        blr
FUNC_END(sysDcrMalrcbs19Get)

/*******************************************************************************
*
* sysDcrMalrcbs19Set - set the contents of MAL Receive Channel 19 Buffer Size
*                     Register
*
* This routine writes the contents of the MAL Receive Channel 19 Buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs19Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs19Set)

        mtdcr   MAL_RCBS19, r3
        blr
FUNC_END(sysDcrMalrcbs19Set)

/*******************************************************************************
*
* sysDcrMalrcbs20Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 20
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs20Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs20Get)

        mfdcr   r3, MAL_RCBS20
        blr
FUNC_END(sysDcrMalrcbs20Get)

/*******************************************************************************
*
* sysDcrMalrcbs20Set - set the contents of MAL Receive Channel 20 Buffer Size
*                     Register
*
* This routine writes the contents of the MAL Receive Channel 20 Buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs20Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs20Set)

        mtdcr   MAL_RCBS20, r3
        blr
FUNC_END(sysDcrMalrcbs20Set)

/*******************************************************************************
*
* sysDcrMalrcbs21Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 21
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs21Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs21Get)

        mfdcr   r3, MAL_RCBS21
        blr
FUNC_END(sysDcrMalrcbs21Get)

/*******************************************************************************
*
* sysDcrMalrcbs21Set - set the contents of MAL Receive Channel 21 Buffer Size
*                     Register
*
* This routine writes the contents of the MAL Receive Channel 21 Buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs21Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs21Set)

        mtdcr   MAL_RCBS21, r3
        blr
FUNC_END(sysDcrMalrcbs21Set)

/*******************************************************************************
*
* sysDcrMalrcbs22Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 22
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs22Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs22Get)

        mfdcr   r3, MAL_RCBS22
        blr
FUNC_END(sysDcrMalrcbs22Get)

/*******************************************************************************
*
* sysDcrMalrcbs22Set - set the contents of MAL Receive Channel 22 Buffer Size
*                     Register
*
* This routine writes the contents of the MAL Receive Channel 22 Buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs22Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs22Set)

        mtdcr   MAL_RCBS22, r3
        blr
FUNC_END(sysDcrMalrcbs22Set)

/*******************************************************************************
*
* sysDcrMalrcbs23Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 23
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs23Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs23Get)

        mfdcr   r3, MAL_RCBS23
        blr
FUNC_END(sysDcrMalrcbs23Get)

/*******************************************************************************
*
* sysDcrMalrcbs23Set - set the contents of MAL Receive Channel 23 Buffer Size
*                     Register
*
* This routine writes the contents of the MAL Receive Channel 23 Buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs23Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs23Set)

        mtdcr   MAL_RCBS23, r3
        blr
FUNC_END(sysDcrMalrcbs23Set)

/*******************************************************************************
*
* sysDcrMalrcbs24Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 24
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs24Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs24Get)

        mfdcr   r3, MAL_RCBS24
        blr
FUNC_END(sysDcrMalrcbs24Get)

/*******************************************************************************
*
* sysDcrMalrcbs24Set - set the contents of MAL Receive Channel 24 Buffer Size
*                     Register
*
* This routine writes the contents of the MAL Receive Channel 24 Buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs24Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs24Set)

        mtdcr   MAL_RCBS24, r3
        blr
FUNC_END(sysDcrMalrcbs24Set)

/*******************************************************************************
*
* sysDcrMalrcbs25Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 25
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs25Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs25Get)

        mfdcr   r3, MAL_RCBS25
        blr
FUNC_END(sysDcrMalrcbs25Get)

/*******************************************************************************
*
* sysDcrMalrcbs25Set - set the contents of MAL Receive Channel 25 Buffer Size
*                     Register
*
* This routine writes the contents of the MAL Receive Channel 25 Buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs25Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs25Set)

        mtdcr   MAL_RCBS25, r3
        blr
FUNC_END(sysDcrMalrcbs25Set)

/*******************************************************************************
*
* sysDcrMalrcbs26Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 26
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs26Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs26Get)

        mfdcr   r3, MAL_RCBS26
        blr
FUNC_END(sysDcrMalrcbs26Get)

/*******************************************************************************
*
* sysDcrMalrcbs26Set - set the contents of MAL Receive Channel 26 Buffer Size
*                     Register
*
* This routine writes the contents of the MAL Receive Channel 26 Buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs26Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs26Set)

        mtdcr   MAL_RCBS26, r3
        blr
FUNC_END(sysDcrMalrcbs26Set)

/*******************************************************************************
*
* sysDcrMalrcbs27Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 27
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs27Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs27Get)

        mfdcr   r3, MAL_RCBS27
        blr
FUNC_END(sysDcrMalrcbs27Get)

/*******************************************************************************
*
* sysDcrMalrcbs27Set - set the contents of MAL Receive Channel 27 Buffer Size
*                     Register
*
* This routine writes the contents of the MAL Receive Channel 27 Buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs27Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs27Set)

        mtdcr   MAL_RCBS27, r3
        blr
FUNC_END(sysDcrMalrcbs27Set)

/*******************************************************************************
*
* sysDcrMalrcbs28Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 28
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs28Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs28Get)

        mfdcr   r3, MAL_RCBS28
        blr
FUNC_END(sysDcrMalrcbs28Get)

/*******************************************************************************
*
* sysDcrMalrcbs28Set - set the contents of MAL Receive Channel 28 Buffer Size
*                     Register
*
* This routine writes the contents of the MAL Receive Channel 28 Buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs28Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs28Set)

        mtdcr   MAL_RCBS28, r3
        blr
FUNC_END(sysDcrMalrcbs28Set)

/*******************************************************************************
*
* sysDcrMalrcbs29Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 29
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs29Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs29Get)

        mfdcr   r3, MAL_RCBS29
        blr
FUNC_END(sysDcrMalrcbs29Get)

/*******************************************************************************
*
* sysDcrMalrcbs29Set - set the contents of MAL Receive Channel 29 Buffer Size
*                     Register
*
* This routine writes the contents of the MAL Receive Channel 29 Buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs29Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs29Set)

        mtdcr   MAL_RCBS29, r3
        blr
FUNC_END(sysDcrMalrcbs29Set)

/*******************************************************************************
*
* sysDcrMalrcbs30Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 30
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs30Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs30Get)

        mfdcr   r3, MAL_RCBS30
        blr
FUNC_END(sysDcrMalrcbs30Get)

/*******************************************************************************
*
* sysDcrMalrcbs30Set - set the contents of MAL Receive Channel 30 Buffer Size
*                     Register
*
* This routine writes the contents of the MAL Receive Channel 30 Buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs30Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs30Set)

        mtdcr   MAL_RCBS30, r3
        blr
FUNC_END(sysDcrMalrcbs30Set)

/*******************************************************************************
*
* sysDcrMalrcbs31Get - get the contents of MAL Receive Channel Buffer Size
*                     Register for RX channel 31
*
* This routine reads and returns the contents of the MAL Receive Channel Buffer
* Size Register.
*
* NOMANUAL
*

* UINT32 sysDcrMalrcbs31Get(void)

*/

FUNC_BEGIN(sysDcrMalrcbs31Get)

        mfdcr   r3, MAL_RCBS31
        blr
FUNC_END(sysDcrMalrcbs31Get)

/*******************************************************************************
*
* sysDcrMalrcbs31Set - set the contents of MAL Receive Channel 31 Buffer Size
*                     Register
*
* This routine writes the contents of the MAL Receive Channel 31 Buffer Size
* Register.
*
* NOMANUAL

* UINT32 sysDcrMalrcbs31Set(void)

*/

FUNC_BEGIN(sysDcrMalrcbs31Set)

        mtdcr   MAL_RCBS31, r3
        blr
FUNC_END(sysDcrMalrcbs31Set)

#endif /* MAL_CHANNELS_64 */
