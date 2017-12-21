/* configNet.h - Network configuration header file */

/*
 * Copyright (c) 2011 Wind River Systems, Inc. 
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,23aug11,rlg  created.  (copied from ammc_taihu which is obsolete )
*/

#ifndef __INCconfigNeth
#define __INCconfigNeth

#ifdef __cplusplus
    extern "C" {
#endif

#ifdef INCLUDE_END /* ignore everything if END not included */

#include <vxWorks.h>
#include <end.h>

END_TBL_ENTRY endDevTbl [] = {
    {0, END_TBL_END, NULL, 0, NULL, FALSE},
};

#ifdef __cplusplus
    }
#endif

#endif /* INCLUDE_END */
#endif /* __INCconfigNeth */

