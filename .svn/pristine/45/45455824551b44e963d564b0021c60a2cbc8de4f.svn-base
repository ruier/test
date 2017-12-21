/* sysSmEnd.c - BSP configuration module for shared memory END driver */

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
01a,09Sep07,y_w  initial creation, based on mpc8540
*/


/*
DESCRIPTION
This is the WRS-supplied configuration module for the VxWorks shared
memory END driver.  It performs the dynamic parameterization
of the smEnd driver.  This technique of 'just-in-time'
parameterization allows driver parameter values to be declared
as something other than static strings.
*/

#if (defined(INCLUDE_END) && defined (INCLUDE_SMEND)    \
     && defined (INCLUDE_SM_NET))

/* includes */

#include "vxWorks.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "end.h"
#include "bootLib.h"

#include <config.h>

#include "drv/end/smEnd.h"

/* define */

#ifdef SM_DEBUG
#   undef       LOCAL
#   define      LOCAL
#endif  /* SM_DEBUG */

#ifndef SM_MBLK_NUM
#define SM_MBLK_NUM     600
#define SM_CBLK_NUM     200
#endif

/* IMPORT */

extern int sysProcNumGet (void);
extern char * sysBootLine;

/* forward declarations */

/**********************************************************************
* sysSmEndLoad - load the shared memory END driver
*
* This function packs the loadstring, and calls the smEndLoad to load the
* the shared memory END driver
*
* RETURN: pointer to the END object or NULL
*
* SEE ALSO: smEndLoad()
*/

END_OBJ * sysSmEndLoad
    (
    char * pParamStr,       /* ptr to the initialization parameter string */
    void * unused           /* unused argument */
    )
    {
    /*
     * The smEnd loading string should be:
     * "<unit>:<pAnchor>:<smAddr>:<memSize>:<tasType>:
     *  <maxCpus>:<masterCpu>:<localCpu>:<maxPktBytes>:<maxInputPkts>:
     *  <intType>:<intArg1>:<intArg2>:<intArg3>:<mbNum>:<cbNum>:
     *  <configFlg>:<pBootParams>"
     */

    char * cp;
    char paramStr [END_INIT_STR_MAX];   /* from end.h */
    END_OBJ * pEnd;

    if (strlen (pParamStr) == 0)
        {
	  /*
           * If empty string, the driver is expected to return the
           * driver name in <pParamsStr>
           */

        pEnd = (END_OBJ *)smEndLoad (pParamStr);
        }
    else
	{
        /*
         * On the second pass though here, we actually create
         * the initialization parameter string on the fly.
         * Note that we will be handed our unit number on the
         * second pass through and we need to preserve that information.
         * So we use the unit number handed from the input string.
         */

        UINT32          configFlag = 0;
        char *          bootString = sysBootLine;   /* config.h */
        char *          pStr;
        BOOT_PARAMS	bootParams;

#ifdef INCLUDE_SM_SEQ_ADDR
        configFlag |= SMEND_SM_SEQ_ADDR;
#endif /* INCLUDE_SM_SEQ_ADDR */

#ifdef INCLUDE_PROXY_SERVER
        configFlag |= SMEND_PROXY_SERVER_FLAG;
#endif /* INCLUDE_PROXY_SERVER */

#ifdef INCLUDE_PROXY_CLIENT
        configFlag |= SMEND_PROXY_CLIENT_FLAG;
#endif /* INCLUDE_PROXY_CLIENT */

#ifdef INCLUDE_PROXY_DEFAULT_ADDR
        configFlag |= SMEND_PROXY_DEFAULT_ADDR_FLAG;
#endif /* INCLUDE_PROXY_DEFAULT_ADDR */

        /* get the boot parameters from the bootline */

    if (bootString == NULL)
	bootString = BOOT_LINE_ADRS;

    pStr = bootStringToStruct (bootString, &bootParams);

    if (*pStr != EOS)
	{
	bootParamsErrorPrint (bootString, pStr);
        printf ("sysSmEnd: Error to get the boot parameters \n");
	return (NULL);
	}

        /* compose the new string */

        cp = strcpy (paramStr, pParamStr); /* cp points to paramStr */

        /* Now, we advance cp, by finding the end the string */

        cp += strlen (paramStr);

        /* finish off the initialization parameter string */

        sprintf (cp, "0x%x:0x%x:0x%x:%d:%d:%d:%d:%d:%d:%d:0x%x:0x%x:0x%x:"
                     "0x%x:0x%x:0x%x:0x%x",
		 (UINT32)SM_ANCHOR_ADRS,      /* ANCHOR address */
  		 SM_MEM_ADRS,                 /* SM memory address */
                 SM_MEM_SIZE,                 /* SM memory size */
                 SM_TAS_HARD,                 /* SM_TAS_TYPE */
                 0,                           /* max CPU#, 0 default */
                 0,                           /* Master CPU#, 0 default */
                 (UINT32)(sysProcNumGet ()),  /* Local CPU# */
                 0,                           /* Max bytes per packet, 0default */
                 0,                           /* Max input packet, 0 default */
		 SM_INT_TYPE,                 /* SM interrupt type */
                 SM_INT_ARG1,                 /* SM interrupt argu 1 */
                 SM_INT_ARG2,                 /* SM interrupt argu 2 */
 		 SM_INT_ARG3,                 /* SM interrupt argu 3 */
                 SM_MBLK_NUM,                 /* Mblk num, 0 default */
                 SM_CBLK_NUM,                 /* cBlk num, 0 default */
                 configFlag,                  /* configuration flag */
                 (UINT32)&bootParams          /* pointer to the BOOT_PARAMS */
                 );

        if ((pEnd = smEndLoad (paramStr)) == (END_OBJ *)NULL)
	    {
            printf ("Error: device failed sysSmEndLoad routine.\n");
	    }
	}

    return (pEnd);
    }

#endif /* defined (INCLUDE_END) && defined (INCLUDE_SMEND) ... */

