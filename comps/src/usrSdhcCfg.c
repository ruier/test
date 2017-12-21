/* usrSdhcCfg.c - SDHC hostcontroller task priority Configlette */

/*
 * Copyright (c) 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,08may13,e_d  written
*/

/*
DESCRIPTION

This file is the SDHC hostcontroller task priority configlet file.

*/

#ifndef __INCusrSdhcCfgc
#define __INCusrSdhcCfgc

/* defines */

#ifndef SD_HOSTMONITOR_TASK_PRIORITY
#   define SD_HOSTMONITOR_TASK_PRIORITY      100
#endif /* SD_HOSTMONITOR_TASK_PRIORITY */

#define SD_HOSTMONITOR_TASK_PRIORITY_DEFUALT 100
#define SD_HOSTMONITOR_TASK_PRIORITY_MIN     0
#define SD_HOSTMONITOR_TASK_PRIORITY_MAX     255


/* locals */

/*******************************************************************************
*
* usrSdhcMgrTaskPriorityGet - get SDHC monitor task priority
*
* This routine is to get SDHC monitor task priority.
*
* RETURNS: SDHC monitor task priority.
*
* ERRNO: N/A
*/

int usrSdhcMgrTaskPriorityGet (void)
    {

    /* The selection is a compile time cost */

    if ((SD_HOSTMONITOR_TASK_PRIORITY < SD_HOSTMONITOR_TASK_PRIORITY_MIN) ||
        (SD_HOSTMONITOR_TASK_PRIORITY > SD_HOSTMONITOR_TASK_PRIORITY_MAX))
        return SD_HOSTMONITOR_TASK_PRIORITY_DEFUALT;
    return SD_HOSTMONITOR_TASK_PRIORITY;
    }
#endif /* __INCusrSdhcCfgc */
