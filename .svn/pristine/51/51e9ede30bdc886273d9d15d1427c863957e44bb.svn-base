/* usrCns.c - CNS initialization */

/* Copyright 2006 Wind River Systems, Inc. */

/*
modification history
--------------------
01c,09feb06,xli  replaced the printf with SSI_PRINTF
01b,19jan06,xli  Added comp media init
01a,02aug06,tkf  Created
*/

/*
DESCRIPTION

CNS initialization code
*/

/* includes */

#include <vxWorks.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lstLib.h>
#include "ssi/ssm.h"
#include "ssi/csm.h"
#include "ssi/cnsLib.h"
#include <ssi/cnsCompLib.h>
/* defines */
#ifndef CNS_MAX_MEDIA_COUNT
#define CNS_MAX_MEDIA_COUNT 0   /* cnsLib will use default count */
#endif

/* forward declarations */

/*****************************************************************************
* usrCnsInit - initializes CNS
*
* This function initializes CNS
*
* RETURNS: ERROR if initialization problems occur
* ERRNO: N/A
*/

void usrCnsInit(void) 
{
    /* Initialize the CNS data base */
    SSI_PRINTF ("usrCnsInit: Initializing CNS ... ");
    cnsLibInit(CNS_MAX_MEDIA_COUNT);
    SSI_PRINTF ("\nusrCnsInit: Initializing COMP media ... ");
    cnsCompLibInit();
    SSI_PRINTF ("\nDone.\n");
}
