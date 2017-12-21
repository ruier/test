/* 405gpDcr.h - IBM 405GP DCR access functions */

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
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M"

********************************************************************************
\NOMANUAL
*/

/* Copyright 1984-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01g,03sep04,mdo  Documentation fixes for apigen
01f,12jun02,pch  SPR 74987: C++
01e,19nov01,pch  cleanup
01d,30may00,mcg  added OCM register access functions
01c,15may00,mcg  register name updates to match 405GP User Manual
01b,15jan00,mcg  moved MAL and UIC info to their own files
01a,21sep99,mcg  created
*/

/*
This file contains the definitions of the 405GP DCR register access functions.
*/

#ifndef INC405gpDcrh
#define INC405gpDcrh

#ifdef __cplusplus
    extern "C" {
#endif

#ifndef _ASMLANGUAGE

/*
 * Clocking, Power Management, and Chip Control DCR access functions
 */
UINT32  sysDcrCr0Get(void);
void    sysDcrCr0Set(UINT32);
UINT32  sysDcrCr1Get(void);
void    sysDcrCr1Set(UINT32);
UINT32  sysDcrPllmrGet(void);
UINT32  sysDcrPsrGet(void);

/*
 * On-chip Memory DCR access functions
 */

UINT32  sysDcrOcmisarcGet(void);
void    sysDcrOcmisarcSet(UINT32);
UINT32  sysDcrOcmiscntlGet(void);
void    sysDcrOcmiscntlSet(UINT32);
UINT32  sysDcrOcmdsarcGet(void);
void    sysDcrOcmdsarcSet(UINT32);
UINT32  sysDcrOcmdscntlGet(void);
void    sysDcrOcmdscntlSet(UINT32);

/*
 * CodePack code decompression controller DCR access functions
 */

void    sysDcrDcpcfgaddrSet(UINT32);
UINT32  sysDcrDcpcfgdataGet(void);
void    sysDcrDcpcfgdataSet(UINT32);


/*
 * 405GP PLB error, PLB arbiter configuration, and PLB-to-OPB bridge error
 * DCR access functions.
 */

UINT32  sysDcrPlbbesrGet(void);
void    sysDcrPlbbesrClear(UINT32);
UINT32  sysDcrPlbbearGet(void);

UINT32  sysDcrPlbacrGet(void);
void    sysDcrPlbacrSet(UINT32);

UINT32  sysDcrPobbesr0Get(void);
void    sysDcrPobbesr0Clear(UINT32);
UINT32  sysDcrPobbesr1Get(void);
void    sysDcrPobbesr1Clear(UINT32);
UINT32  sysDcrPobbearGet(void);

/*
 *  SDRAM Controller DCR access functions (for reading only)
 */

UINT32  sysDcrSdramGet(UINT32 sdramReg);

#endif /* _ASMLANGUAGE */

#ifdef __cplusplus
    }
#endif

#endif  /* INC405gpDcrh */
