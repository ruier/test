/* usrSmAndMipc.c - Runtime check for concurrent SM stack and MIPC support */

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
01e,24sep09,h_x  Remove MIPC_INTERRUPTS_INCLUDED which isn't needed any more
01d,23sep09,h_x  Fix for WIND00182749
01c,21oct08,ebh  Remove MOB_WDB from code
01b,02sep08,ebh  Renamed MOB_PLB_2 to MOB_PLB_1
01a,29aug08,ebh  written
*/

/*
DESCRIPTION
This file is used determine at runtime if both the SM stack and MIPC support
are present in the VxWorks system. If they are, both the SM stack
initialization and the MIPC drivers take special action to ensure that they can
coexist.

SM stack

The SM stack will leave the interrupt handling to the MIPC system. This
includes the bus interrupt acknowledgement if needed.

MIPC system

The MIPC system takes ownership of the interrupt handling. This involves
attaching the interrupt handler to the correct interrupt line and servicing it.
Since an SM stack running without MIPC takes care of this normally, and
attaches its own interrupt handler, this means that the MIPC interrupt handler
must now call the SM handler in addition to servicing MIPC messages. The SM
task-level interrupt work will still be running in the same context, namely
the tNetTask for incoming IP packets, the tTipcSm for TIPC traffic, and the
regular VxMP context for unpending tasks waiting on VxMP semaphores.

NOMANUAL
*/

#if defined (INCLUDE_SM_COMMON) && (defined (INCLUDE_DSHM_MUX) || defined(INCLUDE_MIPC_SM))
    extern void smUtilIntMipc (void);
    int smAndMipc = TRUE;
    FUNCPTR smIntHandler = (FUNCPTR)smUtilIntMipc;
#else
    int smAndMipc = FALSE;
    FUNCPTR smIntHandler = NULL;
#endif

