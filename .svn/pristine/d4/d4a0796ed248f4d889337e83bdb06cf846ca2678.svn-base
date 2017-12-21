/* configNet.h - network configuration header */

/*
 * Copyright (c) 2006 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,19dec06,pmr  adapted from mds8323/configNet.h.
*/

#ifndef __INCconfigNeth
#define __INCconfigNeth

#include "vxWorks.h"
#include "config.h"
#include "end.h"
#include "miiLib.h"

/* defines */

#define END_LOAD_STRING ""      /* created in sys<device>End.c */

/* max number of SENS ipAttachments we can have */

#ifndef IP_MAX_UNITS
#   define IP_MAX_UNITS (NELEMENTS (endDevTbl) - 1)
#endif

END_TBL_ENTRY endDevTbl [] =
{
    { 0, END_TBL_END, NULL, 0, NULL, FALSE}
};
#endif /* INCconfigNeth */
