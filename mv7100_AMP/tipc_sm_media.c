/* tipc_sm_media.c - TIPC Shared memory media routines */

/* Copyright (c) 2005-2007 Wind River Systems, Inc.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, 
 * this list of conditions and the following disclaimer. 
 * Redistributions in binary form must reproduce the above copyright notice, 
 * this list of conditions and the following disclaimer in the documentation 
 * and/or other materials provided with the distribution. 
 * Neither the names of the copyright holders nor the names of its contributors 
 * may be used to endorse or promote products derived from this software 
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */ 

/*
modification history
--------------------
01p,06jun07,bwa  added interrupt throttling when running alongside DSHM.
01o,03apr07,mze  post SMP code review changes
01n,14feb07,mze  post 1.7.2 code review changes
01m,26jan07,mze  fixes to get sm to work after port
01l,01dec06,mze  changes for TIPC 1.7.1 port
01k,12apr06,mze  check to see if pSmBearer is null before processing packets.SPR120140
01j,16mar06,bwa  improved logging when under stress.
01i,14mar06,ebh  remove tipc_msg_limit variables
01h,17feb06,ebh  removal of unused lines and added comments as per code review
01g,15feb06,ebh  bearer checks modified, adjust media descriptor as per code
                 review, change "started" to "sm_started"
01f,14feb06,bwa  updated to tipc-1.5.10.
01e,07feb06,bwa  smMediaStart() now returns a error/success status;
                 when enabling the bearer, there is now a check for 'sm0'.
01d,21dec05,ebh  updates for SF TIPC 1.5 code base
01c,14apr05,bwa  fit heartbeat setup with new SM layout; updated following
                 code review.
01b,08apr05,als  added licensing info and TIPC media descriptor structure
01a,03feb05,bwa  written, inspired by eth_media.c
*/

/*
DESCRIPTION
Shared memory media support provided by TIPC adaptation layer.

INTERNAL
This module binds to a low-overhead shared memory driver. It doesn't use any
network-specific libraries such as the MUX and isn't an END/NPT driver. Its
sole usage is for TIPC.

This bearer code sits directly on top of the shared memory smPktLib library.
Multiple concurrent instances of smPktLib are now possible in a system, each of
them isolated from the others. This sm_media driver uses instance #1, the smEnd driver using instance #0.
*/

/* includes */

#define USE_VXWORKS_LISTS               /* this must be defined before core.h */

#include "tipc_core.h"
#include <errnoLib.h>
#include <sysLib.h>
#include <wdLib.h>
#include <taskLib.h>
#include <tickLib.h>

#include <tipc_sm_media.h>
#include <smUtilLib.h>
#include <tipc.h>
#include <tipc_config.h>
#include <tipc_plugin_if.h>
#include <tipc_plugin_msg.h>
#include <string.h>
#include <drv/timer/timerDev.h>
#ifdef _WRS_CONFIG_SMP
#include <spinLockLib.h>
#endif

/* debug help */

#define SM_MEDIA_DEBUG  0


#if SM_MEDIA_DEBUG

  #define ALL_GLOBAL      1
  #ifdef  ALL_GLOBAL
    #undef  LOCAL
    #define LOCAL
  #endif  /* ALL_GLOBAL */

  #ifdef DBG_OUTPUT
    #undef DBG_OUTPUT
  #endif  /* DBG_OUTPUT */
  #define DBG_OUTPUT  0

  #define DPRINTF(fmt, args...) printf (fmt, ##args)
#else
  #define DPRINTF(fmt, args...)
#endif  /* SM_MEDIA_DEBUG */

/* defines */

#define SM_LINK_PRIORITY       15			/* default priority */
#define SM_LINK_TOLERANCE TIPC_DEF_LINK_TOL	/* default tolerance (1500 ms) */

#define WARN_TIME_THRESHOLD	20	/* in seconds */
#define WARN_MANAGED(nWarn, ts, opStr, typeStr, threshold) \
    do \
	{ \
	int tickDelta = tickGet() - ts; \
	++nWarn; \
	if (tickDelta > threshold) \
	    { \
	    if (threshold != -1) \
		{ \
		warn ("tipcSm ("opStr"): %d "typeStr" unavailable " \
		    	"in the last %d system ticks\n", \
		    	nWarn, tickDelta); \
		} \
	    else \
		{ \
	    	warn ("tipcSm ("opStr"): Out of "typeStr", " \
		    	"incoming SM message discarded (tick#: %d)\n", \
		    	tickDelta); \
	    	} \
	    threshold = WARN_TIME_THRESHOLD * sysClkRateGet(); \
	    ts += tickDelta; \
	    nWarn = 0; \
	    } \
	} while (0);
		    

/* prototypes */

LOCAL int    smTipcMediaStart (void);
LOCAL void   smTipcMediaStop  (void);
LOCAL void   beat             (int timeout);
LOCAL int    packetSend       (struct sk_buff         *pBuf,
                               struct tipc_bearer     *pBearer,
                               struct tipc_media_addr *pDest);
      STATUS tTipcSm          (void);
LOCAL int    smBearerEnable   (struct tipc_bearer * pBearer);
LOCAL void   smBearerDisable  (struct tipc_bearer * pBearer);
LOCAL void   isr              (void);
LOCAL STATUS intSetup         (const SM_TIPC_MEDIA_INT * pSmInt);

/* local variables */
LOCAL struct tipc_media    sm_media_info;
LOCAL WDOG_ID              heart     = NULL;   /* provides SM pkt heartbeat  */
LOCAL SM_PKT_DESC          pktDesc;            /* pkt descriptor (local mem) */
LOCAL SM_TIPC_MEDIA_INT    smInt;              /* SM interrupt type + args   */
LOCAL BOOL                 sm_started= FALSE;  /* SM Media started          */
LOCAL int                  smPktSize = 0;      /* smPkt packet size          */
LOCAL SM_HDR *             pSmHdr    = NULL;   /* SM header, off anchor      */
LOCAL SM_PKT_MEM_HDR *     pPktHdr   = NULL;   /* packet header, off anchor  */
LOCAL SM_PKT_CPU_DESC *    pCpuDesc  = NULL;   /* CPU desc for pkt support   */
LOCAL SEM_ID               pktQSem   = NULL;   /* signals pkts on recv queue */
LOCAL volatile int *       pPktCnt   = NULL;   /* ptr to num of pkts queued  */
LOCAL struct tipc_bearer * pSmBearer = NULL;   /* only instance of SM bearer */
LOCAL NET_POOL_ID          pool      = NULL;   /* pool of networking buffers */
LOCAL BOOL                 rxHandlerRunning = FALSE;/* int throttle */
#ifdef _WRS_CONFIG_SMP
LOCAL spinlockIsr_t tipcSmSpinlock;
#endif

/* global variables */

struct tipc_media_descr sm_media_desc = {
    smTipcMediaStart,                   /* initialization routine */
    smTipcMediaStop                     /* termination routine */
    };

/**
 * sm_media_addr_init - initialize Shared Memory media address structure
 * 
 * Structure's "value" field stores address info in the following format:
 * - unused [3 bytes of zeroes]
 * - shared memory media type identifier [1 byte]
 * - CPU number [4 bytes, in network byte order]
 * - unused [12 bytes of zeroes]
 * 
 * Note: This is the same format as the TIPC neighbour discovery message uses
 * which simplies the job of getting the media address into/out of the message header.
 */

LOCAL void sm_media_addr_init(struct tipc_media_addr *a, u32 cpu)
{
    memset(a->value, 0, sizeof(a->value));
    a->value[3] = TIPC_MEDIA_ID_SM;
    *(u32*)&(a->value[4]) = htonl(cpu);

    a->media_id = TIPC_MEDIA_ID_SM;
    a->broadcast =
        (*(u32*)&(a->value[4]) != *(u32*)&(sm_media_info.bcast_addr.value[4]));
}

/*******************************************************************************
*
* packetSend - send a TIPC packet over Shared Memory
*
* This routine attempts to send <pBuf> over shared memory bearer <pBearer> to
* the shared memory address specified by <pDest>.
*
* Note: This routine runs in the context of the sending task (which may be
* an application task or tTipcTask).
*
* RETURNS: 0 if successful, non-zero if unable to send
*
* NOMANUAL
*/
 
LOCAL int packetSend
    (
    struct sk_buff         * pBuf,      /* message to send */
    struct tipc_bearer     * pBearer,   /* generic bearer to use for send */
    struct tipc_media_addr * pDest      /* message destination */
    )
    {
    SM_PKT *pPkt;		   	/* recipient for pkt taken from free q*/
    int     len;		   	/* len of packet we're sending */
    static int nWarn = 0;	   	/* number of warnings issued */
    static int timestamp = 0;	   	/* timestamp of warning */
    static int warnTimeThreshold = -1;	/* initial warning threshold */
    struct tipc_msg * msg;              /* tipc msg structure */
    u32 cpu;                            /* cpu to send the message to */
    
    /* Grab SM packet from the empty list */
    msg = buf_msg (pBuf);
    msg_dbg (msg, "(SM packetSend)");
    if (smPktFreeGet(&pktDesc, &pPkt) != OK)
        {
        /* returning 0 will cause the upper layer to retransmit the packet
         * later.
         */
        DPRINTF ("packetSend(): send failed! (errno=0x%08x)\n", errnoGet ());
        return 0;
        }
        
    if (pPkt == NULL)
        {
	    WARN_MANAGED (nWarn, timestamp, "send", "SM pkt", warnTimeThreshold);
        return 0;
        }
    
    /*
     * Cache length of packet and copy data to SM - packet size MUST NOT be
     * converted to network byte-order here since smPktSend() looks at the
     * value to ensure packet is not too big and then converts it itself to
     * network byte-order. Then send packet.
     */
    
    len = pPkt->header.nBytes = msg_size(msg); 

    bcopy (pBuf->mBlkId->mBlkHdr.mData, pPkt->data, len);
    cpu = ntohl(*(u32*)&(pDest->value[4]));

    if (smPktSend (&pktDesc, pPkt, cpu) != OK)
        {
        DPRINTF ("smPktSend failed (errno=0x%08x) cpu = %d\n", errnoGet(), cpu);
        smPktFreePut (&pktDesc, pPkt);
        }
    
    return 0;
    }

/*******************************************************************************
*
* tTipcSm - process TIPC packets received over Shared Memory
*
* This routine receives a TIPC message from an shared memory interface and
* forwards it to the incoming message handler.
*
* Note: This routine runs in the context of the TIPC SM task (tTipcSm).
*
* RETURNS: ERROR if something went really wrong, doesn't return otherwise.
*
* NOMANUAL
*/
 
STATUS tTipcSm (void)
    {
    SM_PKT *  pPkt;		   /* packet obtained from smPktRecv() */
    M_BLK_ID  mblk;		   /* local storage of packet received */
    int       nWarnMblk = 0;	   /* # of warnings issued: out of SM mblks */
    int       nWarnTBuf = 0;	   /* # of warnings issued: out of TIPC Bufs */
    int       tsMblk = 0;	   /* timestamp of warning: SM mblks */
    int       tsTBuf = 0;	   /* timestamp of warning: TIPC buffers */
    int       warnTimeThMblk = -1; /* initial warning threshold for mblk */
    int       warnTimeThTBuf = -1; /* initial warning threshold for TIPC bufs */
   
    FOREVER
        {
#ifndef _WRS_CONFIG_SMP
        int key;
#endif
        /* XXX - when available, this will run as a jobLib job */
        if (semTake (pktQSem, WAIT_FOREVER) == ERROR)
            {
            DPRINTF ("SM tipc - problem with the semaphore\n");
            return ERROR;
            }

        /* drain queue until there are no packets left */
#ifdef _WRS_CONFIG_SMP
        spinLockIsrTake(&tipcSmSpinlock);
#else
        key = intLock ();
#endif
        while (ntohl (*pPktCnt) > 0)
            {
            struct sk_buff * pBuf;  /* message buffer        */
            int len;                /* cached packet length  */

#ifdef _WRS_CONFIG_SMP
            spinLockIsrGive(&tipcSmSpinlock);
#else
            intUnlock (key);
#endif
            if (smPktRecv (&pktDesc, &pPkt) != OK)
                {
                printf ("Unrecoverable error in TIPC SM support!\n"
                        "Shutting down the sub-system...\n");
                
                /* detach from SM, don't flush pkt Q and suspend self (debug) */
                smPktDetach (&pktDesc, TRUE);
                taskSuspend (taskIdSelf());
                return ERROR;
                }
                
            /* Queue drained? Go back to sleep. */
            if (pPkt == NULL)
                {
#ifdef _WRS_CONFIG_SMP
                spinLockIsrTake(&tipcSmSpinlock);
#else
                key = intLock ();
#endif
                break;
                }
                
            /*
             * Only process the packet if the media has been started. What can
             * happen is that a remote node has already started broadcasting
             * that it exists, and the local node is receiving those packets
             * on SM, since it is attached to SM and the interrupts are set up
             * as well. However, if TIPC hasn't announced that it wants to use
             * SM yet, it is just noise at this point and these packets must
             * be discarded.
             * 
	     * Check pSmBearer as the media may have been started but the
             * bearer may not have. 
             */
            if (pSmBearer == NULL)
                {
                /* discard noise as stated above. */
                smPktFreePut (&pktDesc, pPkt);
                DPRINTF ("tTipcSm: noise - discarded (%p)\n", pPkt);
#ifdef _WRS_CONFIG_SMP
                spinLockIsrTake(&tipcSmSpinlock);
#else
                key = intLock ();
#endif
                continue;
                }

            
            /*
             * cache length since in SM - it was passed over SM in network
             * byte-order by the packet library but was re-converted to
             * host byte-order by smPktRecv(), thus we MUST NOT do the
             * conversion here.
             */
            len = pPkt->header.nBytes;
                
            /* fetch a netbuf buffer to hand to TIPC */
            mblk = netMblkGet (pool, M_DONTWAIT, MT_DATA);
    
            if (mblk == NULL)
                {
                smPktFreePut (&pktDesc, pPkt);
				WARN_MANAGED (nWarnMblk, tsMblk, "recv", "SM TIPC mblks",
				warnTimeThMblk);
#ifdef _WRS_CONFIG_SMP
                spinLockIsrTake(&tipcSmSpinlock);
#else
                key = intLock ();
#endif
                continue;
                }
        
            /* setup netbuf packet for TIPC */
            bcopy (pPkt->data, mblk->mBlkHdr.mData, len);
            mblk->mBlkPktHdr.len  = len;
            mblk->mBlkHdr.mLen    = len;
            mblk->mBlkHdr.mFlags |= M_PKTHDR;
        
            smPktFreePut (&pktDesc, pPkt);

            /* Allocate message buffer to hold incoming message */
            pBuf = vxskb_attach (mblk);
            if (pBuf == NULL)
                {
                netMblkClFree (mblk);
		        WARN_MANAGED (nWarnTBuf, tsTBuf, "recv", "TIPC buffers",
				warnTimeThTBuf);
#ifdef _WRS_CONFIG_SMP
                spinLockIsrTake(&tipcSmSpinlock);
#else
                key = intLock ();
#endif
                continue;
                }

            /* Hand off message buffer for processing */
            msg_dbg (buf_msg (pBuf), "(SM recv)");
            tipc_recv_msg (pBuf, pSmBearer);
            
            /* next queued packet! */
#ifdef _WRS_CONFIG_SMP
            spinLockIsrTake(&tipcSmSpinlock);
#else
            key = intLock ();
#endif
            }
        
        /* no packets queued, go back to sleep */

        rxHandlerRunning = FALSE;
#ifdef _WRS_CONFIG_SMP
        spinLockIsrGive(&tipcSmSpinlock);
#else
        intUnlock (key);
#endif
        }
    
    return OK;
    }

/*******************************************************************************
*
* smBearerEnable - enable a shared memory bearer
*
* This routine enables TIPC processing over the specified shared memory
* interface.
*
* RETURNS: OK if bearer is enabled, -EINVAL otherwise
*
* NOMANUAL
*/
 
LOCAL int smBearerEnable 
    (
    struct tipc_bearer * pBearer        /* bearer to enable */
    )
    {
    if (pSmBearer != NULL)
        {
        warn ("Shared memory bearer already enabled\n");
        return -EINVAL;
        }
    if (0 != strcmp (pBearer->name, "sm:sm0"))
        {
        warn ("Shared memory bearer '%s' not a valid name"
              " (must be 'sm:sm0')\n", pBearer->name);
        return -EINVAL;
        }
    
    pSmBearer = pBearer;
    
    pBearer->usr_handle = (void *)pSmBearer;
    pBearer->mtu = smPktSize;
    pBearer->blocked = FALSE;  
    sm_media_addr_init(&pBearer->addr, sysProcNumGet());
    
    DPRINTF ("bearer enabled\n");
    return OK;
    }

/*******************************************************************************
*
* smBearerDisable - disable a shared memory bearer
*
* This routine disables TIPC processing over the specified shared memory
* interface.
*
* NOMANUAL
*/
 
LOCAL void smBearerDisable
    (
    struct tipc_bearer * pBearer        /* bearer to disable */
    )
    {
    pSmBearer = NULL;
    return;
    }

/*******************************************************************************
 *
 * sm_addr2str - convert SM address from host form to string form
 *
 * This routine populates an ASCII string buffer using the info
 * contained in a SM media address structure.
 *
 * RETURNS: 0 if successful, 1 unable to do conversion
 *
 * NOMANUAL
 */

LOCAL int sm_addr2str(struct tipc_media_addr *a, char *str_buf, int str_size)
{
    u32 cpu;

    if ((a->media_id != TIPC_MEDIA_ID_SM) || (str_size < 11))
	    return 1;
    
    cpu = ntohl(*(u32 *)&(a->value[4]));
    sprintf(str_buf, "%u", cpu);  
    return 0;
}

/*******************************************************************************
 *
 * sm_str2addr - convert SM address from string form to host form
 *
 * This routine populates a SM media address structure using the info
 * contained in an ASCII string buffer.
 *
 * RETURNS: 0 if successful, 1 unable to do conversion
 *
 */

LOCAL int sm_str2addr(struct tipc_media_addr *a, char *str_buf)
{                     
    char dummy; /* a dummy character to read in, if present invalidates cpu */
    u32 cpu;

    if (1 != sscanf(str_buf, "%u%c", &cpu, &dummy))
	    return 1;

    sm_media_addr_init(a, cpu);
    return 0;

}

/*******************************************************************************
 *
 * sm_msg2addr - convert SM address from message form to host form
 *
 * This routine populates a SM media address structure using the info
 * contained in the media address area of a LINK_CONFIG message.
 *
 * RETURNS: 0 if successful, 1 unable to do conversion
 *
 * NOMANUAL
 */

LOCAL int sm_msg2addr(struct tipc_media_addr *a, u32 *msg_area)
{
    int cpu;
    if (msg_area[0] != htonl(TIPC_MEDIA_ID_SM))
	return 1;
    cpu = ntohl(msg_area[1]);
    sm_media_addr_init(a, cpu);
    return 0;
}

/*******************************************************************************
 *
 * sm_addr2msg - convert SM address from host form to message form 
 *
 * This routine populates the media address area of a LINK_CONFIG message
 * using the info contained in a SM media address structure.
 *
 * RETURNS: 0 if successful, 1 unable to do conversion
 *
 * NOMANUAL
 */

LOCAL int sm_addr2msg(struct tipc_media_addr *a, u32 *msg_area)
{
	if (a->media_id != TIPC_MEDIA_ID_SM)
		return 1;

	memcpy(msg_area, a->value, sizeof(a->value));
	return 0;
}

/*
 * Shared memory media registration info required by TIPC
 */

LOCAL struct tipc_media sm_media_info = {
	TIPC_MEDIA_ID_SM,
	"sm",
	SM_LINK_PRIORITY,
	SM_LINK_TOLERANCE,
	TIPC_DEF_LINK_WIN,
	packetSend,
	smBearerEnable,
	smBearerDisable,
	sm_addr2str,
	sm_str2addr,
	sm_msg2addr,
	sm_addr2msg,
	{{0, 0, 0, TIPC_MEDIA_ID_SM, 0xbb, 0xbb, 0xbb, 0xbb, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  /* SM_BROADCAST = 0xbbbbbbbb */
	 TIPC_MEDIA_ID_SM, 1}
};


/*******************************************************************************
*
* smTipcMediaStart - start up shared memory media
* 
* This routine allows TIPC to use shared memory interfaces. It registers the
* media with TIPC and allows the tTipcSm task to actually process the packets.
*
* RETURNS: OK or -EINVAL
*
* NOMANUAL
*/

LOCAL int smTipcMediaStart (void)
    {			
    int register_status;
    
    if (sm_started)
        {
        warn ("Attempt to re-initialize Shared Memory media ignored\n");
        return -EINVAL;
        }
            
    DPRINTF ("starting SM media\n");
    register_status = tipc_register_media(&sm_media_info);

    if (0 == register_status)
        {
        sm_started = TRUE;
    	}
    else
	{
	warn ("Cannot start shared memory media\n");
	}

    return register_status;
    }

/*******************************************************************************
*
* smTipcMediaStop - shut down shared memory bearers
* 
* This routine stops TIPC from using any shared memory interfaces.
*
* NOMANUAL
*/

LOCAL void smTipcMediaStop (void)
    {
    if (!sm_started)
        return;
    
    DPRINTF ("Stopping SM media\n");
    sm_started = FALSE;
    DPRINTF ("SM media stopped\n");
    }

/*******************************************************************************
*
* isr - ISR that services TIPC SM interrupts.
* 
* This routine basically signals the 'tTipcSm' task that packets are available
* to be processed. It does interrupt throttling in case the task-level handler
* is already running.
*
* NOMANUAL
*/

LOCAL void isr (void)
    {
#ifndef _WRS_CONFIG_SMP
    int key;
#endif

#ifdef _WRS_CONFIG_SMP
    spinLockIsrTake(&tipcSmSpinlock);
#else
    key = intLock ();
#endif
    if (!rxHandlerRunning)
        {
        rxHandlerRunning = TRUE;
#ifdef _WRS_CONFIG_SMP
        spinLockIsrGive(&tipcSmSpinlock);
#else
        intUnlock (key);
#endif
        semGive (pktQSem);
        return;
        }
#ifdef _WRS_CONFIG_SMP
    spinLockIsrGive(&tipcSmSpinlock);
#else
    intUnlock (key);
#endif
    }

/*******************************************************************************
*
* smTipcMediaPktHdrSetup - setup packet header, from anchor
* 
* Initializes the local reference to the SM packet header, obtaining its value
* from the table stored in the SM anchor. TIPC uses instance #1 of smPktLib.
* 
* Invoked by the configlette, usrTipcSmInit().
*
* NOMANUAL
*
*/

void smTipcMediaPktHdrSetup (const SM_ANCHOR * pAnchor)
    {
    int maxCpus;
    pPktHdr = SM_OFFSET_TO_LOCAL(
                                  ntohl(pAnchor->smPktHeader)
                                 ,(int)pAnchor
                                 ,SM_PKT_MEM_HDR *
                                );
    pSmHdr  = SM_OFFSET_TO_LOCAL (ntohl (pAnchor->smHeader), (int)pAnchor,
                                  SM_HDR *);
    maxCpus = ntohl(pSmHdr->maxCpus);

    DPRINTF ("smTipcMediaPktHdrSetup - table: %p\n", pPktHdr);
    pPktHdr = (SM_PKT_MEM_HDR *) ((char *) pPktHdr +
                                  sizeof (SM_PKT_MEM_HDR) +
                                  sizeof (SM_PKT_CPU_DESC) * maxCpus);
    DPRINTF ("smTipcMediaPktHdrSetup - instance: %p\n", pPktHdr);
    }
         
/*******************************************************************************
*
* beat - generate one TIPC SM heartbeat
* 
* Used as an 'alive' signal to smPktLib. It also schedules the next beat with
* a delay of <timeout> ticks. The first call to beat() should always be called
* from smTipcMediaHeartbeatSetup(), which passes in a valid timeout value.
* 
* NOMANUAL
*
*/

LOCAL void beat (int timeout)
    {
    smPktBeat (pPktHdr);
    wdStart ( heart ,timeout ,(FUNCPTR)beat ,timeout );
    }

/*******************************************************************************
*
* smTipcMediaHeartbeatSetup - setup TIPC SM heartbeat system
* 
* Create the heartbeat watchdog, starts heartbeats. <timeout> value can be any
* positive integer (in ticks) or 0; if the latter, the timeout will be set to
* the system clock rate, resulting in a delay of 1 second between beats.
* However, since the timeout value is cached and never updated after the first
* beat, if the system clock rate is changed at runtime thereafter, the delay
* won't adjust and thus beat with a period different than one second.
* 
* Invoked by the configlette, usrTipcSmInit().
*
* RETURNS: ERROR if watchdog can't be created, OK otherwise.
* 
* NOMANUAL
*
*/

STATUS smTipcMediaHeartbeatSetup (const int timeout)
    {
    if (timeout < 0)
        {
        DPRINTF ("smTipcMediaHeartbeatSetup: error - negative timeout\n");
        return ERROR;
        }
    
    heart = wdCreate ();
    
    if (heart == NULL)
        {
        DPRINTF ("smTipcMediaHeartbeatSetup - can't create 'heart' watchdog\n");
        return ERROR;
        }
        
    beat (timeout == 0 ? sysClkRateGet() : timeout);

    return OK;
    }

/*******************************************************************************
*
* intSetup - setup interrupts servicing sub-system
* 
* Creates the receive packets processing task and semaphore, connects to SM
* interrupts specified by the configuration, and enables interrupts.
* 
* RETURNS: OK if everything went fine, ERROR otherwise.
* 
* NOMANUAL
*
*/

LOCAL STATUS intSetup
    (
    const SM_TIPC_MEDIA_INT * pSmInt    /* interrupt type + arguments */
    )
    {
    int tid;    /* tTipcSm task ID */
    
    /* store interrupt information locally */
    bcopy ((char *)pSmInt, (char *)&smInt, sizeof (SM_TIPC_MEDIA_INT));
    
    /* create processing task semaphore */
    pktQSem = semBCreate (SEM_EMPTY, SEM_Q_FIFO);
    if (pktQSem == NULL)
        return ERROR;
    
    /* spawn the task-level ISR handler */
    tid = taskSpawn (
                      "tTipcSm"
                     ,45
                     ,0x0
                     ,0x20000
                     ,(FUNCPTR)tTipcSm
                     ,0,1,2,3,4,5,6,7,8,9
                    );
                     
    if (tid == ERROR)
        {
        semDelete (pktQSem);
        DPRINTF ("smTipcMediaIntSetup - Spawning tTipcSm failed!\n");
        return ERROR;
        }
    
    /* connect ISR to interrupt sub-system */
    if (smUtilIntConnect (
                           TIPC_PRIORITY
                          ,(FUNCPTR)isr
                          ,(int)NULL
                          ,pSmInt->type
                          ,pSmInt->arg1
                          ,pSmInt->arg2
                          ,pSmInt->arg3
                         ) != OK)
        {
        semDelete (pktQSem);    /* this will clean up the task as well. */
        DPRINTF ("smTipcMediaIntSetup - "
             "Connecting to interrupt subsystem failed!\n");
        return ERROR;
        }
        
    /* enable the reception of interrupts at specified level */
    sysIntEnable(pSmInt->arg1);
    
    return OK;
    }

/*******************************************************************************
*
* smTipcMediaDriverSetup - setup interface to SM hardware
* 
* Invoked by the configlette, usrTipcSmInit().
*
* NOMANUAL
*
*/

STATUS smTipcMediaDriverSetup
    (
    SM_ANCHOR *                pAnchor,  /* anchor local address */
    const SM_TIPC_MEDIA_INT *  pSmInt,   /* interrupt type and arguments */
    const int                  cpu,      /* this CPU's number */
    const unsigned int         qlen,     /* max # packets in SM queue */
    const int                  numbuf,   /* number of buffers in the pool */
    const int                  instance  /* instance number of smPktLib */
    )
    {
    NETBUF_CL_DESC netcldesc[1];
    NETBUF_CFG     bufcfg = {
                             "SM_TIPC"    /* pool name */
                            ,ATTR_AI_ISR  /* int aligned, private, isr safe */
                            ,NULL         /* kernel domain */
                            ,0	          /* SET BELOW: number of mblks */
                            ,NULL	  /* mblk partition: kernel */
                            ,0	          /* number of extra clusters */
                            ,NULL	  /* partition for extra clusters */
                            ,NULL	  /* SET BELOW: &netcldesc[0] */
                            ,1	          /* number of entries in netcldesc */
                            };
    
    /* cache SM header, SM size and maximum packet size locally */
    pSmHdr = SM_OFFSET_TO_LOCAL (ntohl (pAnchor->smHeader),
                                 (int)pAnchor, SM_HDR *);
    smPktSize = ntohl (pPktHdr->maxPktBytes);
        
#ifdef _WRS_CONFIG_SMP
    spinLockIsrInit(&tipcSmSpinlock, 0);
#endif

    /* setup local packet descriptor, bzero() is done on pktDesc in there */
    smPktInit (
               &pktDesc
              ,pAnchor
              ,qlen
              ,0            /* ticks per beat, pick default */
              ,pSmInt->type
              ,pSmInt->arg1
              ,pSmInt->arg2
              ,pSmInt->arg3
              );
    
    /* Setup driver's own pool - local copy of incoming packets */
    bufcfg.ctrlNumber = numbuf;
    bufcfg.pClDescTbl = &netcldesc[0];
    netcldesc[0].clNum = numbuf;
    netcldesc[0].clSize = smPktSize;
    if ((pool = netPoolCreate (&bufcfg, _pLinkPoolFuncTbl)) == NULL)
        {
        printf ("TIPC bearer - can't create buffer pool\n");
        return (ERROR);
        }
    
    /* attach descriptor (and local CPU) to shared memory packet sub-system */
    if (smPktAttachTo (&pktDesc, instance) == ERROR)
        {
        /* netPoolDestroy (pool); */
        printf ("TIPC bearer - smPktAttachTo instance failed!\n");    
        return ERROR;
        }
        
    /* first gets the table, second the local CPU's entry */
    pCpuDesc = pktDesc.cpuLocalAdrs;
    pCpuDesc = &(pCpuDesc[cpu]);
    
    /* hook into list count to get number of packets on the incoming queue */
    pPktCnt = (volatile int *)&(pCpuDesc->inputList.count);
    
    /* setup and start interrupt sub-system */
    if (intSetup (pSmInt) != OK)
        {
        /* netPoolDestroy (pool); */
        printf ("TIPC bearer - Interrupt setup failed!\n");
        return ERROR;
        }

    /* There may have been some packets queued before interrupts were enabled,
     * but smPktLib code won't generate interrupts if said packets are queued 
     * when sending a new one; thus, we must kickstart the receiving task.
     */
    semGive (pktQSem);

    return OK;
    }

