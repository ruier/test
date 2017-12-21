/* usrIsrDefer.c - ISR deferral configuration file */

/*
 * Copyright (c) 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement.
 */

/*
modification history
--------------------
01c,07may07,h_k  made the global ISR deferral mode configurable.
01b,02mar07,h_k  removed obsoleted code.
01a,07feb07,h_k  written.
*/

/*
DESCRIPTION

User configurable ISR deferral library intialization.
*/

#ifndef	ISR_DEFER_MODE
#define ISR_DEFER_MODE		ISR_DEFER_MODE_PER_CPU
#endif	/* ISR_DEFER_MODE */

void usrIsrDeferInit (void)
    {
    isrDeferLibInit (ISR_DEFER_MODE);
    }
