/* configNet.h - network configuration header */

/*
 * Copyright (c) 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,26jun10,syt  initial creation.
*/

#ifndef __INCconfigNeth
#define __INCconfigNeth

#include <vxWorks.h>
#include "config.h"
#include <end.h>
#include <miiLib.h>

/* max number of SENS ipAttachments we can have */

#ifndef IP_MAX_UNITS
#   define IP_MAX_UNITS (NELEMENTS (endDevTbl) - 1)
#endif  /* IP_MAX_UNITS */

END_TBL_ENTRY endDevTbl [] =
    {
    { 0, END_TBL_END, NULL, 0, NULL, FALSE}
    };
#endif  /* __INCconfigNeth */

