/* configNet.h - Network configuration header file */

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

   COPYRIGHT   I B M   CORPORATION 2004
   LICENSED MATERIAL  -  PROGRAM PROPERTY OF  I B M"

********************************************************************************
\NOMANUAL
*/

/* Copyright 2006 Wind River Systems, Inc. */

/*
modification history
--------------------
01c,21mar08,wap  Add VxBus support, switch to VxBus EMAC driver
01b,09mar06,mmi removed the fei82557End (1,2,3) devices from endDevTbl
01a,09mar06,mmi created based on ../bamboo/sysNet.h 01r

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
#define FEI_LOAD_FUNC sysFei82557EndLoad     /* driver external interface */
#define FEI_BUFF_LOAN 1

/* The END_LOAD_STRING is defined empty and created dynamically */
#define FEI_LOAD_STRING "" /* created in sysFei82557EndLoad */

IMPORT END_OBJ * FEI_LOAD_FUNC (char*, void*);
#endif /* INCLUDE_FEI82557END */

END_TBL_ENTRY endDevTbl [] =
{

#ifdef INCLUDE_FEI82557END
    { 0, FEI_LOAD_FUNC, FEI_LOAD_STRING, FEI_BUFF_LOAN, NULL, FALSE},
#endif /* INCLUDE_FEI82557END */

#ifdef INCLUDE_LN97XEND
    {0, LN_97X_LOAD_FUNC, LN_97X_LOAD_STR_0, TRUE, NULL, FALSE},
#endif  /* INCLUDE_LN97XEND */

    {0, END_TBL_END,        NULL,                0,    NULL, FALSE},
};

#ifdef __cplusplus
    }
#endif

#endif /* INCconfigNeth */
