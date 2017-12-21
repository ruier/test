/* configNet.h - Network configuration header file */

/*
*******************************************************************************
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

****************************************************************************
\NOMANUAL
*/

/* Copyright 1984-2009 Wind River Systems, Inc. */

/*
modification history
--------------------
01j,20apr09,wap  Switch to VxBus EMAC driver
01i,31aug04,mdo  Documentation fixes for apigen
01h,21jul03,jln  rename ibmEmacEndLoad to emacEndLoad
01g,12jun02,pch  SPR 74987: C++
01f,09jan02,pch  prevent setting USE_BOTH_EMACS if there is only one
01e,29nov01,mcg  added support for two EMACs
01d,25jul01,pmr  re-organized to allow for multiple device configuration
01c,01may01,pmr  Changed INCLUDE_PCI_NETWORK to INCLUDE_LN97XEND.
01b,21mar00,mcg  changes for ibmEmacEndBldLoadStr function
01a,20sep99,mcg  created
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


END_TBL_ENTRY endDevTbl [] =
{

#ifdef INCLUDE_LN97XEND
    {0, LN_97X_LOAD_FUNC, LN_97X_LOAD_STR_0, TRUE, NULL, FALSE},
#endif	/* INCLUDE_LN97XEND */

    {0, END_TBL_END,        NULL,                0,    NULL, FALSE},
};

#ifdef __cplusplus
    }
#endif

#endif /* INCconfigNeth */









