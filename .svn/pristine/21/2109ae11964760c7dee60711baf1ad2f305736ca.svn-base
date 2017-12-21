/* vxTestV2Layered.h - vxTestV2Layered module header */

/* Copyright 1998-2005 Wind River Systems, Inc. */

/*
modification history
--------------------
01d,20jul06,mfj  adding declaration
01c,22sep05,mfj  updating for MKL
01b,16aug05,uo	including necessary libraries required in BSP builds
01a,17jun05,uo	written.
*/

#ifndef __INCvxTestV2Layeredh
#define __INCvxTestV2Layeredh

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * vxTestV2Layered - Header file for vxTestV2Layered module, external functions' prototypes.
 * see source file of this module for a better understanding.
 */ 


/* includes */
#include "vxWorks.h"
#include "scMemVal.h"
#include "shellInterpCmdLib.h"
#include "moduleLib.h"
#include "wdb/wdbLib.h"
#include "ctype.h"
#include "envLib.h"
#include "errno.h"
#include "intLib.h"
#include "ioLib.h"
#include "loadLib.h"
#include "logLib.h" 
#include "math.h"
#include "memLib.h"
#include "msgQLib.h"
#include "private/scMemValP.h"
#include "private/symLibP.h"
#include "rtpLib.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "symLib.h"
#include "sysLib.h"
#include "syscallLib.h"
#include "taskLib.h"
#include "tickLib.h"
#include "time.h"
#include "unistd.h"
#include "unldLib.h"
#include "vxTest.h"
#include "vxTestV2Int.h"
#include "vxTestV2Rtp.h"
#include "wdLib.h"

#include <errno.h>
#include <fioLib.h>
#include <memLib.h>
#include <memPartLib.h>
#include <msgQLib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysLib.h>
#include <syscall.h>

/* defines */
/*
 * stderr is an exceptional case, it is not a function symbol 
 * but replaced by a function symbol, so we will replace stderr
 * with _pex_stderr using preprocessor likewise
 */

#define _pex_stderr	NULL

/* typedefs */

/* function declarations */

STATUS vxTestV2LayeredInit (void);

char * vxTestCpuVariantGet (void);

char * vxTestToolGet (void);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCvxTestV2Layeredh */
