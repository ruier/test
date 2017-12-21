/* usrTipcSm.c - TIPC Shared memory media configlette */

/* Copyright (c) 2005, 2012 Wind River Systems, Inc.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the names of the copyright holders nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */ 
 
/*
modification history
--------------------
01h,17oct12,whu  fixed defect WIND00381666. Replaced SM_MASTER with smMaster.
01g,04nov10,swg  add the explicit declaration for _procNumWasSet
                 (to fix WIND00237118).
01f,27sep05,bwa  added check for minimum packet size (SPR#112323).
01e,12may05,bwa  warnings cleanup
01d,26apr05,bwa  fixed race condition on anchor->smPktHeader for slave boards.
01c,02apr05,bwa  added debug aid, allocatable SM support, more probes, check
                 for BSP support.
01b,14apr05,als  added licensing info; incorporate code inspection fixes
01a,15fev05,bwa  written.
*/

/*
DESCRIPTION
Configuration file for Shared memory media support provided for TIPC
adaptation layer.

INTERNAL
This module binds to a low-overhead shared memory driver. It doesn't use any
network-specific libraries such as the MUX and isn't an END/NPT driver. Its
sole usage is for TIPC.

NOMANUAL
*/

#define SM_TIPC_INIT_DONE   0x5a5a5a5a

#include "wrn/coreip/netinet/in.h"

#ifndef SM_TIPC_ADRS_DEFAULT
#error SM TIPC not supported by this BSP!
#endif

#if(SM_TAS_TYPE != SM_TAS_HARD)
#error Error initializing TIPC SM support; HARDWARE test-and-set required.
#endif

#if((SM_TIPC_SM_PKT_SIZE != 0) && (SM_TIPC_SM_PKT_SIZE < 160))
#error 'SM_TIPC_SM_PKT_SIZE' is too small (160 bytes minimum)
#endif

LOCAL SM_PKT_MEM_HDR * usrTipcSmPktMemHdrCompute(const SM_ANCHOR * pAnchor);
extern BOOL _procNumWasSet;

/*******************************************************************************
*
* usrTipcSmInit - initialize TIPC shared memory media support
* 
* This routine initializes the shared memory anchor + data structures if needed
* and always sets up the TIPC SM driver.
*
* NOMANUAL
*
*/

STATUS usrTipcSmInit (char * pBootLine)
    {
    static SM_ANCHOR * pAnchor  = NULL;     /* anchor */
    STATUS             result   = ERROR;    /* return status */
    SM_TIPC_MEDIA_INT  smInt;               /* int type + args */
    BOOT_PARAMS        params;              /* boot parameters */
    int                cpu      = -1;       /* this cpu's number */
    char               bb;                  /* bit bucket for probes */
    SM_PKT_MEM_HDR *   pPktHdr;

    /* already initialized ? bail out. */
    if (pAnchor != NULL)
        {
        printf ("usrTipcSm - Support already enabled.\n");
        return ERROR;
        } 
        
    /* interpret boot command */
    if (usrBootLineCrack (pBootLine, &params) != OK)
        {
        printf ("usrTipcSm - error parsing bootline\n");
        return (ERROR);
        }

    /* set processor number: may establish vme bus access, etc. */
    if (_procNumWasSet != TRUE)
	{
    	sysProcNumSet (params.procNum);
	_procNumWasSet = TRUE;
	}

    /* grab anchor value from bootline if set, BSP/Project parameters if not */
    if (strncmp (params.bootDev, "sm=", 3) == 0)
        {
        if (bootBpAnchorExtract (params.bootDev, (char**)&pAnchor) < 0)
            {
	    printf ("\nError initializing shared memory objects, invalid ");
            printf ("anchor address specified: \"%s\"\n", params.bootDev);
            return (ERROR);
            }
        }
    else
        {
        pAnchor = (SM_ANCHOR *)SM_ANCHOR_ADRS;
        }
    
    cpu = sysProcNumGet();
    if (cpu == smMaster)
        {
	/* volatile is needed to prevent warnings when comparing to NONE */
        volatile char * pSmTipcAdrs = (char *)SM_TIPC_ADRS;
        int             smTipcSize  = SM_TIPC_SIZE;

        /* check cache configuration - must be read and write coherent */
	if (!CACHE_DMA_IS_WRITE_COHERENT() || !CACHE_DMA_IS_READ_COHERENT())
	    {
	    printf ("usrTipcSmInit - cache coherent buffer not available. "
                    "Giving up.\n");
	    return (ERROR);
	    }

        if (pSmTipcAdrs == (char *)NONE)
            {
            /* check cache configuration - must be read and write coherent */

            pSmTipcAdrs = (char *) cacheDmaMalloc (smTipcSize);

            if (pSmTipcAdrs == NULL)
		{
		printf ("usrTipcSmInit - cannot allocate shared memory pool."
                        "Giving up.\n");
                return (ERROR);
		}
            }
        else
            {
            /* if fixed locations and they overlap, push TIPC region forward
             * (basically trying to fix a user error)
             */
            if ((pAnchor + 1) > (SM_ANCHOR *)pSmTipcAdrs)
                {
                pSmTipcAdrs = ((char *)pSmTipcAdrs) + sizeof (SM_ANCHOR);
                smTipcSize -= sizeof (SM_ANCHOR);
                }
            }

        /* probe SM, it could be offboard */

	/* probe anchor address */

	if (vxMemProbe ((char *)pAnchor, VX_READ, sizeof (char), &bb) != OK)
	    {
	    printf ("usrTipcSmInit - anchor address %p unreachable. "
                    "Giving up.\n", pAnchor);
	    return (ERROR);
	    }

	/* probe beginning of shared memory */

	if (vxMemProbe ((char*)pSmTipcAdrs, VX_WRITE, sizeof (char), &bb) != OK)
	    {
	    printf ("usrTipcSmInit - shared memory address %p unreachable. "
                    "Giving up.\n", (unsigned int) pSmTipcAdrs);
	    return (ERROR);
	    }

        result = smPktSetupInst (
                                  pAnchor
                                 ,(char *)pSmTipcAdrs
                                 ,smTipcSize
                                 ,SM_TAS_HARD
                                 ,SM_CPUS_MAX
                                 ,SM_TIPC_SM_PKT_SIZE
                                 ,1 
                            );
        
        if (result != OK)
            {
            printf ("usrTipcSm - smPktSetup failed!\n");
            return ERROR;    
            }

        pPktHdr = usrTipcSmPktMemHdrCompute (pAnchor);
        pPktHdr->reserved2 = htonl (SM_TIPC_INIT_DONE);
        }
    else
        {
        int temp;           /* will contain anchor address */
        int tics = cpu;     /* ticks to wait; wait time depends on CPU */

        /*
         * probe before doing anything on slaves since SM might be on the master
         * and it might not have booted yet.
         */

        /* probe, wait for anchor to be accessible */
        for (tics <<= 1; tics < SM_MAX_WAIT; tics <<= 1)
            {
            smUtilDelay (NULL, tics);
            if ((smUtilMemProbe (
                                  (char *)pAnchor
                                 ,VX_READ
                                 ,sizeof (UINT)
                                 ,(char *)&temp
                                ) == OK)
                &&
                (ntohl (temp) == SM_READY))
                {
                break;
                }
            }

        /* even when anchor is accessible, the smPktHeader entry might now have
         * been initialized yet in the anchor, probe for it and ensure it is
         * non-zero */
        for (; tics < SM_MAX_WAIT; tics <<= 1)
            {
            if ((smUtilMemProbe (
                                  (char *)&(pAnchor->smPktHeader)
                                 ,VX_READ
                                 ,sizeof (UINT)
                                 ,(char *)&temp
                                ) == OK)
                &&
                (ntohl (temp) != 0))
                {
                break;
                }
            smUtilDelay (NULL, tics);
            }

        if (tics >= SM_MAX_WAIT)
            {
            printf ("usrTipcSm - Anchor @ %p unreachable. Giving up.\n",
                    pAnchor);
            return (ERROR);
            }
        pPktHdr = usrTipcSmPktMemHdrCompute (pAnchor);
        
        
        /* probe until the reserved2 field in the pkt hdr is set to
         * SM_TIPC_INIT_DONE: this is the signal from the master board that
         * the slave setup can start. */
        for (; tics < SM_MAX_WAIT; tics <<= 1)
            {
            if ((smUtilMemProbe (
                                  (char *)&(pPktHdr->reserved2)
                                 ,VX_READ
                                 ,sizeof (UINT)
                                 ,(char *)&temp
                                ) == OK)
                &&
                (ntohl (temp) == SM_TIPC_INIT_DONE))
                {
                break;
                }
            smUtilDelay (NULL, tics);
            }
        if (tics >= SM_MAX_WAIT)
            {
            printf ("usrTipcSm - Can't find initialized pkt lib. Giving up.\n",
                    pAnchor);
            return (ERROR);
            }
        }
    
    /* setup packet header */
    smTipcMediaPktHdrSetup (pAnchor);
            
    /* master board provides heartbeat */
    if (cpu == smMaster)
        {
        if (smTipcMediaHeartbeatSetup (0) == ERROR)
            {
            printf ("usrTipcSm - Heartbeat setup failed!\n");
            return ERROR;
            }
        }
    
    /* store for external use (sm_media.c) */
    smInt.type = SM_INT_TYPE;
    smInt.arg1 = SM_INT_ARG1;
    smInt.arg2 = SM_INT_ARG2;
    smInt.arg3 = SM_INT_ARG3;
    
    /* setup driver */
    if (smTipcMediaDriverSetup (
                                 pAnchor
                                ,&smInt
                                ,cpu
                                ,SM_TIPC_PKT_Q_LEN
                                ,SM_TIPC_NUM_BUF
                                ,1
                               ) != OK)
        {
        printf ("usrTipcSm - Driver setup failed!\n");
        return ERROR;
        }
        
    printf ("TIPC Shared memory support is ENABLED.\n");
    
    return OK;
    }
    
/*******************************************************************************
*
* usrTipcSmPktMemHdrCompute - compute the location of instance 1 of smPktHdr
* 
* NOMANUAL
*
*/

LOCAL SM_PKT_MEM_HDR * usrTipcSmPktMemHdrCompute (const SM_ANCHOR * pAnchor)
    {
    SM_HDR *           pSmHdr;
    int                maxCpus;
    SM_PKT_MEM_HDR *   pPktHdr;

    pPktHdr = SM_OFFSET_TO_LOCAL(
                                  ntohl(pAnchor->smPktHeader)
                                 ,(int)pAnchor
                                 ,SM_PKT_MEM_HDR *
                                );
    pSmHdr =  SM_OFFSET_TO_LOCAL ( ntohl (pAnchor->smHeader)
                                  ,(int)pAnchor
                                  ,SM_HDR *);
    maxCpus = ntohl(pSmHdr->maxCpus);
    pPktHdr = (SM_PKT_MEM_HDR *) ((char *) pPktHdr +
                                  sizeof (SM_PKT_MEM_HDR) +
                                  sizeof (SM_PKT_CPU_DESC) * maxCpus);
    return pPktHdr;
    }

