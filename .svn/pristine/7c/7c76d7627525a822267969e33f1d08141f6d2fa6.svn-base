/* configNet.h - network configuration header */

/*
 * Copyright (c) 2006-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01g,13jun08,wap  Switch to new VxBus TSEC driver
01f,27jul07,spw  Add WLAN support for Broadcom 43XX
01e,27nov06,tfo  Fix mottsec issue with WLAN defined.
01d,13sep06,tfo  WLAN support for Atheros AR521X
01c,10mar06,kds  modify for gei8254xEndLoad
01b,05aug05,j_b  remove FEI END
01a,04feb05,dtr  written from ads8560/configNet.h.
*/

#ifndef INCconfigNeth
#define INCconfigNeth

#include <vxWorks.h>
#include "config.h"
#include <end.h>
#include <miiLib.h>

/* defines */

#define SYS_END_BUFF_LOAN_PRI   1
#define SYS_END_BUFF_LOAN_SEC   2
#define SYS_END_BUFF_LOAN_TER   3
#define SYS_END_BUFF_LOAN_QUA   4
#define SYS_END_BUFF_LOAN_QUI   5
#define SYS_END_BUFF_LOAN_SEN   6


#define END_LOAD_STRING ""      /* created in sys<device>End.c */


/* max number of SENS ipAttachments we can have */

#ifndef IP_MAX_UNITS
#   define IP_MAX_UNITS (NELEMENTS (endDevTbl) - 1)
#endif

END_TBL_ENTRY endDevTbl [] =
{
/* Atheros AR521X WLAN Support */
#if defined INCLUDE_AR521X_END
    {-1, END_TBL_END, NULL, 0, NULL, FALSE}, /* up to 4 Atheros NICs */
    {-1, END_TBL_END, NULL, 0, NULL, FALSE},
    {-1, END_TBL_END, NULL, 0, NULL, FALSE},
    {-1, END_TBL_END, NULL, 0, NULL, FALSE},
#endif /* INCLUDE_AR521X_END */

/* Broadcom 4XXX WLAN Support */
    
#ifdef INCLUDE_BCM43XX_END
	{-1, END_TBL_END, NULL, 0, NULL, FALSE}, /* up to 4 Broadcom NICs */
	{-1, END_TBL_END, NULL, 0, NULL, FALSE},
	{-1, END_TBL_END, NULL, 0, NULL, FALSE},
	{-1, END_TBL_END, NULL, 0, NULL, FALSE},
#endif /* INCLUDE_BCM43XX_END */

    { 0, END_TBL_END, NULL, 0, NULL, FALSE},
};
#endif /* INCconfigNeth */
