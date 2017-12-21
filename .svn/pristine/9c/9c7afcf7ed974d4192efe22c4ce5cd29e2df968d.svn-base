/* usrCoprocs.c - coprocessor library initialization configlette */

/* 
Copyright (c) 1984-2005 Wind River Systems, Inc. 

The right to copy, distribute, modify or otherwise make use 
of this software may be licensed only pursuant to the terms 
of an applicable Wind River license agreement. 
*/

/*
modification history
--------------------
01e,31may05,rec  add Coldfire support
01d,21apr04,h_k  fixed the name of the SH user coprocessor file.
01c,01mar04,pcs  Remove build errors. Change usrCoproc<ARCH>.h to
                 usrCoproc<ARCH>.c
01b,13feb04,yvp  Include arch-specific configlettes. Moved PPC code there.
01a,21mar03,yvp  written.
*/

/*
DESCRIPTION

This module calls architecture-specific code to implement the discovery phase
for coprocessors. 
*/

/* includes */

#if     (CPU_FAMILY==PPC)
#include "usrCoprocPpc.c"
#elif   (CPU_FAMILY==ARM)
#include "usrCoprocArm.c"
#elif   (CPU_FAMILY==COLDFIRE)
#include "usrCoprocColdfire.c"
#elif   (CPU_FAMILY==MIPS)
#include "usrCoprocMips.c"
#elif   (CPU_FAMILY==SH)
#include "usrCoprocSh.c"
#elif   (CPU_FAMILY==SIMNT)
#include "usrCoprocSimnt.c"
#elif   (CPU_FAMILY==SIMLINUX)
#include "usrCoprocSimlinux.c"
#elif   (CPU_FAMILY==I80X86)
#include "usrCoprocI86.c"
#elif   (CPU_FAMILY==SIMSPARCSOLARIS)
#include "usrCoprocSimsolaris.c"
#else
/* #else */
/* #error "THIS ARCHITECTURE DOES NOT SUPPORT COPROCESSORS!!" */
#endif
