/* usrSmAndDshm.c - Runtime check for concurrent SM stack and DSHM support */

/*
 * Copyright (c) 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,29aug08,ebh  Updated to only use the usrSmAndMipc.c check
01a,30apr07,bwa  written
*/

/*
NOTE
This is all handled in the usrSmAndMipc.c configlette.  This file left for
one release only and then will be removed.

DESCRIPTION
This file is used determine at runtime if both the SM stack and DSHM support
are present in the VxWorks system. If they are, both the SM stack
initialization and the DSHM drivers take special action to ensure that they can
coexist.

SM stack

The SM stack will leave the interrupt handling to the DSHM system. This
includes the bus interrupt acknowledgement if needed.

DSHM system

The DSHM system takes ownership of the interrupt handling. This involves
attaching the interrupt handler to the correct interrupt line and servicing it.
Since an SM stack running without DSHM takes care of this normally, and
attaches its own interrupt handler, this means that the DSHM interrupt handler
must now call the SM handler in addition to servicing DSHM messages. The SM
task-level interrupt work will still be running in the same context, namely
the tNetTask for incoming IP packets, the tTipcSm for TIPC traffic, and the
regular VxMP context for unpending tasks waiting on VxMP semaphores.

NOMANUAL
*/

#if 0
/* #if defined (INCLUDE_SM_COMMON) && defined (INCLUDE_DSHM_MUX) */
    extern void smUtilIntDshm (void);
    int smAndDshm = TRUE;
    FUNCPTR smIntHandler = (FUNCPTR)smUtilIntDshm;
#else
    int smAndDshm = FALSE;
    /* FUNCPTR smIntHandler = NULL; */
#endif

