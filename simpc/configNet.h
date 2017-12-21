/* configNet.h - network configuration header */

/* Copyright 1984-1997 Wind River Systems, Inc. */

/*
modification history
--------------------
01j,19dec03,dbt  Added endDevTbl declaration.
01i,01sep03,elp  removed endDevTbl array.
01h,13aug03,elp  added simnet support.
01g,12dec02,jmp  added multiple network interface support (SPR #73834).
01f,19jan98,gnn  made addresses slave to new, patched BSP, addresses.
01e,20nov97,gnn  fixed spr#9555, configurable numRds and numTds
01d,03oct97,gnn  added function prototype for load function
01c,25sep97,gnn  SENS beta feedback fixes
01b,02jun97,map  updated ivec and ilevel.
01a,25apr97,map  written.

*/
 
#ifndef INCnetConfigh
#define INCnetConfigh

#include "vxWorks.h"
#include "end.h"

/*
 * endDevTbl array is no longer statically declared in this file. It is
 * automatically allocated from command line options given to vxsim. See
 * sysNet.c and vxsim '-ni' command line options for details.
 */

extern END_TBL_ENTRY endDevTbl [IP_MAX_UNITS + 1];

#endif /* INCnetConfigh */
