/* configNet.h - amcc_taishan network configuration header file */

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

/*
 * Copyright (c) 2003-2004, 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01f,17apr07,x_s  Correct the copyright format.
01e,29sep04,mdo  Documentation fixes for apigen
01d,06aug03,jtp  Formal code inspection changes. Added EMAC0_ENABLE macro.
01c,16jul03,jln  change ibmEmacEndLoad to emacEndLoad  
01b,09jul03,jtp  Support FEI
01a,06jan03,saw  created based on 440GP v01g
*/

#ifndef INCconfigNeth
#define INCconfigNeth

#ifdef __cplusplus
    extern "C" {
#endif

#include "vxWorks.h"
#include "end.h"

/*
 * END structure for AMD PCI Ethernet card
 */

#ifdef INCLUDE_LN97XEND

IMPORT  char sys97xLoadString[];

#define LN_97X_LOAD_FUNC        ln97xEndLoad
#define LN_97X_LOAD_STR_0       sys97xLoadString

IMPORT END_OBJ * LN_97X_LOAD_FUNC (char *, void *);

#endif /* INCLUDE_LN97XEND */

/*
 * END structure for Intel 8255[7-9] Fast Ethernet Interface (FEI)
 */

#ifdef INCLUDE_FEI82557END
#define FEI_LOAD_FUNC	sysFei82557EndLoad     /* driver external interface */
#define FEI_BUFF_LOAN	1

/* The END_LOAD_STRING is defined empty and created dynamically */
#define FEI_LOAD_STRING "" /* created in sysFei82557EndLoad */

IMPORT END_OBJ * FEI_LOAD_FUNC (char*, void*);
#endif /* INCLUDE_FEI82557END */


/*
 * END structure for integrated EMAC Ethernet
 */

#ifdef INCLUDE_EMAC_NETWORK

#define IBM_EMAC_LOAD_FUNC        emacEndLoad

#ifdef EMAC0_ENABLE 
IMPORT  char sysEmacLoadString0[];
#define IBM_EMAC_LOAD_STR_0       sysEmacLoadString0
#endif /* EMAC0_ENABLE */
#ifdef EMAC1_ENABLE 
IMPORT  char sysEmacLoadString1[];
#define IBM_EMAC_LOAD_STR_1       sysEmacLoadString1
#endif /* EMAC1_ENABLE */
#ifdef EMAC2_ENABLE 
IMPORT  char sysEmacLoadString2[];
#define IBM_EMAC_LOAD_STR_2       sysEmacLoadString2
#endif /* EMAC2_ENABLE */
#ifdef EMAC3_ENABLE 
IMPORT  char sysEmacLoadString3[];
#define IBM_EMAC_LOAD_STR_3       sysEmacLoadString3
#endif /* EMAC3_ENABLE */

IMPORT END_OBJ * IBM_EMAC_LOAD_FUNC (char *, void *);

#endif /* INCLUDE_EMAC_NETWORK */

/* Set IP_MAX_UNITS based on number of entries in the endDevTbl array */
#undef  IP_MAX_UNITS
#define IP_MAX_UNITS  (NELEMENTS(endDevTbl) - 1)

END_TBL_ENTRY endDevTbl [] =
{

#ifdef INCLUDE_EMAC_NETWORK

#ifdef EMAC0_ENABLE 
    {0, IBM_EMAC_LOAD_FUNC, IBM_EMAC_LOAD_STR_0, TRUE, NULL, FALSE},
#endif /* EMAC0_ENABLE */
#ifdef EMAC1_ENABLE 
    {1, IBM_EMAC_LOAD_FUNC, IBM_EMAC_LOAD_STR_1, TRUE, NULL, FALSE},
#endif  /* EMAC1_ENABLE */
#ifdef EMAC2_ENABLE 
    {2, IBM_EMAC_LOAD_FUNC, IBM_EMAC_LOAD_STR_2, TRUE, NULL, FALSE},
#endif  /* EMAC2_ENABLE */
#ifdef EMAC3_ENABLE 
    {3, IBM_EMAC_LOAD_FUNC, IBM_EMAC_LOAD_STR_3, TRUE, NULL, FALSE},
#endif  /* EMAC3_ENABLE */

#endif /* INCLUDE_EMAC_NETWORK */

#ifdef INCLUDE_FEI82557END
    { 0, FEI_LOAD_FUNC, FEI_LOAD_STRING, FEI_BUFF_LOAN, NULL, FALSE},
    { 1, FEI_LOAD_FUNC, FEI_LOAD_STRING, FEI_BUFF_LOAN, NULL, FALSE},
    { 2, FEI_LOAD_FUNC, FEI_LOAD_STRING, FEI_BUFF_LOAN, NULL, FALSE},
    { 3, FEI_LOAD_FUNC, FEI_LOAD_STRING, FEI_BUFF_LOAN, NULL, FALSE},
#endif /* INCLUDE_FEI82557END */

#ifdef INCLUDE_LN97XEND
    {0, LN_97X_LOAD_FUNC, LN_97X_LOAD_STR_0, TRUE, NULL, FALSE},
#endif	/* INCLUDE_LN97XEND */

    {0, END_TBL_END,        NULL,                0,    NULL, FALSE},
};

#ifdef __cplusplus
    }
#endif

#endif /* INCconfigNeth */
