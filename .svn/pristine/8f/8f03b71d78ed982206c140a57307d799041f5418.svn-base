/* tsi108End.c - Tundra Tsi108 GigE END driver */

/* Copyright (c) 2006, 2008 Wind River Systems, Inc */
/* Copyright (C) 2005 Tundra Semiconductor */

/*
modification history
--------------------
01f,09sep08,wap  Create the phyLock and miscLock semaphores before trying to
                 use them (WIND00130667)
01e,31mar06,dtr  Use the last descriptor for the tx mBlk chain instead of the
                 first.
01d,09feb06,avb   fixed bug in tsi108EndSend();
                  changed format of some debug messages;
		  replaced taskDelay() with sysUsDelay() in initialization.
01c,01feb06,dtr   tidyup
01b,27jan06,avb   added support for non-coherent mode if snooping is disabled;
                  modified tsi108EndStop() to stop all DMA transactions, needed
		  for reboot() to prevent bootloader failure.
01a,05Oct05,klai  created based on templateEnd.c
*/

/*
DESCRIPTION
This module implements the TSI108 GigE (10/100/1000BaseT) driver.

EXTERNAL INTERFACE
The driver provides the standard external interface, tsi108EndLoad(), which
takes a string of colon separated parameters. The parameter string is parsed 
using strtok_r() and each parameter in converted from a string representation
to a binary.
*/


/* includes */

#include <vxWorks.h>
#include <stdlib.h>
#include <cacheLib.h>
#include <intLib.h>
#include <end.h>			/* Common END structures. */
#include <endLib.h>
#include <lstLib.h>			/* Needed to maintain protocol list. */
#include <iv.h>
#include <semLib.h>
#include <logLib.h>
#include <netLib.h>
#include <stdio.h>
#include <sysLib.h>
#include <errno.h>
#include <errnoLib.h>
#include <memLib.h>
#include <iosLib.h>
#undef	ETHER_MAP_IP_MULTICAST
#include <etherMultiLib.h>		/* multicast stuff. */

#include <net/mbuf.h>
#include <netinet/if_ether.h>

#include <sys/ioctl.h>
#include <sys/times.h>
#include <assert.h>

#include "tsi108End.h"
#include "sysTsi108Mpic.h"

/* imports */

IMPORT void   sysUsDelay (UINT32);

/* defines */

#define NUM_RAR                         16
#define NUM_MTA                         128
#define NUM_VLAN                        128
#define ETHER_POLY (0x04C11DB7)

#define END_BUFSIZ      (ETHERMTU + SIZEOF_ETHERHEADER + 6)
#define EH_SIZE		(14)
#define END_SPEED_10M	10000000	/* 10Mbs */
#define END_SPEED_100M	100000000	/* 100Mbs */
#define END_SPEED_1000M	1000000000	/* 1G */
#define END_SPEED       END_SPEED_1000M
#define TSI108_MIN_FBUF	(1536)		/* min first buffer size */

#define TSI108_GIGE_REG_WR(pDrvCtrl, offset, value) \
	do { \
	*(volatile UINT32*)(pDrvCtrl->regs + (offset)) = (UINT32)value; \
	_WRS_ASM (" eieio"); \
	} while((0))

#define TSI108_GIGE_REG_RD(pDrvCtrl, offset) \
	    *(volatile UINT32*)(pDrvCtrl->regs + (offset))

#define TSI108_GIGE_PHYREG_WR(pDrvCtrl, offset, value) \
	do { \
	*(volatile UINT32*)(pDrvCtrl->phyRegs + (offset)) = value; \
	_WRS_ASM (" eieio"); \
	} while((0))

#define TSI108_GIGE_PHYREG_RD(pDrvCtrl, offset) \
	*(volatile UINT32*)(pDrvCtrl->phyRegs + (offset))

/* A shortcut for getting the hardware address from the MIB II stuff. */

#define END_HADDR(pEnd)	\
		((pEnd)->mib2Tbl.ifPhysAddress.phyAddress)

#define END_HADDR_LEN(pEnd) \
		((pEnd)->mib2Tbl.ifPhysAddress.addrLength)


#define END_FLAGS_ISSET(setBits) \
		((&pDrvCtrl->end)->flags & (setBits))


/* bus/CPU address translation macros */

#define TSI108_VIRT_TO_PHYS(virtAddr) \
	END_CACHE_VIRT_TO_PHYS ((char *)(virtAddr))

/* cache macros */

#define TSI108_USER_CACHE_FLUSH(address, len) \
	CACHE_USER_FLUSH(address, len)

#define TSI108_USER_CACHE_INVALIDATE(address, len) \
	CACHE_USER_INVALIDATE(address, len)

#define TSI108_CACHE_FLUSH(address, len) \
	CACHE_DRV_FLUSH (&cacheDmaFuncs, (address), (len))

#define TSI108_CACHE_INVALIDATE(address, len) \
	CACHE_DRV_INVALIDATE (&cacheDmaFuncs, (address), (len))

#define END_CACHE_PHYS_TO_VIRT(address) \
	CACHE_DRV_PHYS_TO_VIRT (&cacheDmaFuncs, (address))

#define END_CACHE_VIRT_TO_PHYS(address) \
	CACHE_DRV_VIRT_TO_PHYS (&cacheDmaFuncs, (address))

/* misc. */

#define MAX_MII_DELAY 5000 /* max time to poll PHY in mSec */

#define TSI108_RXRING_LEN     64

/* NOTE: The driver currently does not support receiving packets
 * larger than the buffer size, so don't decrease this (unless you
 * want to add such support).
 */

#define TSI108_RXBUF_SIZE     2048/* 1536 */

#define TSI108_TXBUF_SIZE     2048

#define TSI108_TXRING_LEN	64

#define TSI108_TX_INT_FREQ	(TSI108_TXRING_LEN/2)

#define TSI108_MAX_NUM_MULTI	512


/* Definitions for the local flags field */

#define TSI108_PROMISCUOUS	0x1
#define TSI108_POLLING		0x2

#define	TSI108END_DBG_SHOW  /* Enables/disables internal show routine */

#define	TSI108_END_DEBUG

#ifdef  TSI108_END_DEBUG
#define TSI108_DEBUG_OFF	0x0000
#define TSI108_DEBUG_RX		0x0001
#define TSI108_DEBUG_TX		0x0002
#define TSI108_DEBUG_INT	0x0004
#define TSI108_DEBUG_POLL_RX	0x0008
#define TSI108_DEBUG_POLL_TX	0x0010
#define TSI108_DEBUG_LOAD	0x0020
#define TSI108_DEBUG_IOCTL	0x0040
#define TSI108_DEBUG_ERROR	0x0080

#define TSI108_DEBUG_POLL   (TSI108_DEBUG_POLL_RX | TSI108_DEBUG_POLL_TX)

int	tsi108DebugFlags =  TSI108_DEBUG_ERROR;

#define TSI108_LOG(FLG, X0, X1, X2, X3, X4, X5, X6) \
    do  {					    \
	if (tsi108DebugFlags & FLG)		    \
	    logMsg (X0, X1, X2, X3, X4, X5, X6);    \
	} while ((0))

#define TSI108_PRINT(FLG,X)	    \
    do  {			    \
	if (tsi108DebugFlags & FLG) \
	    printf X;		    \
	} while ((0))

#else /* TSI108_END_DEBUG */
#define TSI108_LOG(FLG, X0, X1, X2, X3, X4, X5, X6)
#define TSI108_PRINT(FLG,X)
#endif /* TSI108_DRV_DEBUG */

/* The Tsi108End driver control structure */

typedef struct tsi108_drv_ctrl
    {
    END_OBJ	end;		/* the class we inherit from. */
    UINT32	flags;
    CL_POOL_ID	pClPoolId;	/* cluster pool ID */
    UINT32	clSz;		/* cluster size for RX buffer */
    END_ERR	pError;
    char*	pMclkArea;	/* address of Mclk */
    char*	pRxBufBase;
    int		unit;
    int 	portNum;	/* GIGE Port Number */
    UINT32      regs;		/* Base address of normal regs */
    UINT32	phyRegs;	/* Base addr used for PHY access */
    UINT32	intMask;	/* Saved interrupt mask */
    int		phy;		/* Index of PHY for this interface */
    int		irqNum;
    /* Rx Queue */
    int		rxTail;		/* Next entry in rxring to read */
    int		rxHead;		/* Next entry in rxring to give a new buffer */
    int		rxFree;		/* Number of free, allocated RX buffers */
    volatile BOOL rxPending;	/* Rx Task Pending flag */
    /* Tx Queue */
    int		txTail;		/* Next TX descriptor to check status on */
    int		txHead;		/* Next TX descriptor to use */
    int		txFree;		/* Number of free TX descriptors. */
    volatile BOOL txPending;	/* Tx Task Pending flag */
    volatile BOOL txStall;      /* flag for transmit to stall */

    UCHAR	enetAddr[6];	/* Ether address for this adaptor */
    int		phy_ok;		/* The PHY is currently powered on. */

    /* PHY status (duplex is 1 for half, 2 for full,
     * so that the default 0 indicates that neither has
     * yet been configured).
     */

    int		link_up;
    int		speed;
    int		duplex;

    volatile TX_DESC*	txRing;
    volatile RX_DESC*	rxRing;

    M_BLK_ID	rxMblks[TSI108_RXRING_LEN];
    M_BLK_ID	txMblks[TSI108_TXRING_LEN];
    char*	txBufs[TSI108_TXRING_LEN];
    char*	rxBufs[TSI108_RXRING_LEN];

    UINT32	txPhys;
    UINT32	rxPhys;
    SEM_ID	phyLock;
    SEM_ID	miscLock;

    unsigned long  mcHash[16];
    END_IFDRVCONF  endStatsConf;
    END_IFCOUNTERS endStatsCounters;

    /* Statistics carry interrupt counters */
    UINT32	carryRxBytes;
    UINT32	carryRxPkt;
    UINT32	carryRxBcastPkt;
    UINT32	carryRxMcastPkt;
    UINT32	carryTxBytes;
    UINT32	carryTxPkt;
    UINT32	carryTxBcastPkt;
    UINT32	carryTxMcastPkt;

#ifdef TSI108END_DBG_SHOW
    UINT32	rxCount;
    UINT32	txCount;
    UINT32	rxRefillCount;
    M_BLK_ID	pMblk;
#endif /* TSI108END_DBG_SHOW */
    } TSI108_DRV_CTRL;

UCHAR enetAddrTbl[2][6] = {{0x00, 0x06, 0xd2, 0x00, 0x01, 0x08},
			   {0x00, 0x06, 0xd2, 0x00, 0x02, 0x09}};

M_CL_CONFIG tsi108MClBlkConfig = /* Mblk */
    {
    /*
    no. mBlks   no. clBlks   memArea   memSize
    ---------   ----------   -------   -------
    */
    0,            0,           NULL,     0
    };

CL_DESC tsi108ClDesc[] = /* Cluster descriptor */
    {
    /*
    clusterSize   num   memArea   memSize
    -----------   ---   -------   -------
    */
    { 0,            0,    NULL,     0}
    };

/* This is the only externally visible interface. */

END_OBJ* 	tsi108EndLoad (char* initString);

LOCAL int	tsi108EndIoctl(TSI108_DRV_CTRL* pDrvCtrl,
						    int cmd, caddr_t data);
LOCAL STATUS	tsi108EndMCastAdd(TSI108_DRV_CTRL* pDrvCtrl, char* pAddress);
LOCAL STATUS	tsi108EndMCastDel(TSI108_DRV_CTRL* pDrvCtrl, char* pAddress);
LOCAL STATUS	tsi108EndMCastGet(TSI108_DRV_CTRL* pDrvCtrl,
				   MULTI_TABLE* pTable);
LOCAL STATUS	tsi108EndMemInit(TSI108_DRV_CTRL * pDrvCtrl);
LOCAL STATUS	tsi108EndParse(TSI108_DRV_CTRL* pDrvCtrl, char * initString);
LOCAL STATUS	tsi108EndPollRcv(TSI108_DRV_CTRL* pDrvCtrl, M_BLK_ID pMblk);
LOCAL STATUS	tsi108EndPollSend(TSI108_DRV_CTRL* pDrvCtrl, M_BLK_ID pMblk);
LOCAL STATUS	tsi108EndEncap(TSI108_DRV_CTRL* pDrvCtrl, M_BLK_ID pBuf);
LOCAL STATUS	tsi108EndSend(TSI108_DRV_CTRL* pDrvCtrl, M_BLK_ID pBuf);
LOCAL STATUS	tsi108EndStart(TSI108_DRV_CTRL* pDrvCtrl);
LOCAL STATUS    tsi108EndStatsDump (TSI108_DRV_CTRL* pDrvCtrl);
LOCAL STATUS	tsi108EndStop(TSI108_DRV_CTRL* pDrvCtrl);
LOCAL STATUS	tsi108EndUnload(TSI108_DRV_CTRL* pDrvCtrl);

LOCAL int	tsi108CleanCompletedTbd(TSI108_DRV_CTRL * pDrvCtrl);
LOCAL int	tsi108CheckForCompletedTx(TSI108_DRV_CTRL * pDrvCtrl);
LOCAL STATUS	tsi108GetMacAddress(TSI108_DRV_CTRL * pDrvCtrl);
LOCAL STATUS    tsi108HashTblPopulate(TSI108_DRV_CTRL* pDrvCtrl);
LOCAL void	tsi108InitMac(TSI108_DRV_CTRL * pDrvCtrl);
LOCAL void	tsi108KillPhy(TSI108_DRV_CTRL * pDrvCtrl);
LOCAL STATUS	tsi108PollStart(TSI108_DRV_CTRL * pDrvCtrl);
LOCAL STATUS	tsi108PollStop(TSI108_DRV_CTRL * pDrvCtrl);
LOCAL void	tsi108ResetPort(TSI108_DRV_CTRL * pDrvCtrl);
LOCAL void	tsi108RestartTx(TSI108_DRV_CTRL * pDrvCtrl);
LOCAL void	tsi108RestartRx(TSI108_DRV_CTRL * pDrvCtrl);
LOCAL STATUS	tsi108SetMacAddress(TSI108_DRV_CTRL * pDrvCtrl, void *addr);

IMPORT void sysMsDelay(UINT32 delay);
/*
 * Driver Functions Table.
 */
NET_FUNCS tsi108EndFuncTable =
    {
    (FUNCPTR) tsi108EndStart,     /* Function to start the device. */
    (FUNCPTR) tsi108EndStop,      /* Function to stop the device. */
    (FUNCPTR) tsi108EndUnload,    /* Unloading function for the driver. */
    (FUNCPTR) tsi108EndIoctl,     /* Ioctl function for the driver. */
    (FUNCPTR) tsi108EndSend,      /* Send function for the driver. */
    (FUNCPTR) tsi108EndMCastAdd,  /* Multicast add func for the driver. */
    (FUNCPTR) tsi108EndMCastDel,  /* Multicast delete func for the driver. */
    (FUNCPTR) tsi108EndMCastGet,  /* Multicast retrieve func for the driver */
    (FUNCPTR) tsi108EndPollSend,  /* Polling send function */
    (FUNCPTR) tsi108EndPollRcv,   /* Polling receive function */
    endEtherAddressForm,          /* put address info into a NET_BUFFER */
    endEtherPacketDataGet,        /* get pointer to data in NET_BUFFER */
    endEtherPacketAddrGet         /* Get packet addresses. */
    };

TSI108_DRV_CTRL *tsi108Ends[TSI108_ETH_PORT_NUM] = {NULL, NULL};

/*******************************************************************************
*
* tsi108EndPollSend - routine to send a packet in polled mode.
*
* Polled mode operation takes place without any kernel or other OS
* services available.  Use extreme care to insure that this code does not
* call any kernel services.  Polled mode is only for WDB system mode use.
* Kernel services, semaphores, tasks, etc, are not available during WDB
* system mode.
*
* A typical implementation is to set aside a fixed buffer for polled send
* operation.  Copy the mblk data to the buffer and pass the fixed buffer
* to the device.  Performance is not a consideration for polled operations.
*
* An alternate implementation is a synchronous one.  The routine accepts
* user data but does not return until the data has actually been sent.  If an
* error occurs, the routine returns EAGAIN and the user will retry the request.
*
* If the device returns OK, then the data has been sent and the user may free
* the associated mblk.  The driver never frees the mblk in polled mode.
* The calling routine will free the mblk upon success.
*
* RETURNS: OK upon success.  EAGAIN if device is busy or no resources.
* A return of ERROR indicates a hardware fault or no support for polled mode
* at all.
*/

LOCAL STATUS tsi108EndPollSend (TSI108_DRV_CTRL* pDrvCtrl, M_BLK_ID pMblk)
    {
    int	i = 0; 
    int tx;

    if (!(pDrvCtrl->flags & TSI108_POLLING))
	return (ERROR);

    if (!pMblk )
        return (EAGAIN);

    TSI108_LOG(TSI108_DEBUG_POLL_TX, "tsi108EndPollSend : IN\n",1,2,3,4,5,6);

    if (pDrvCtrl->txFree)
	{
	tx = pDrvCtrl->txHead;

	TSI108_USER_CACHE_FLUSH(pMblk->mBlkHdr.mData, pMblk->mBlkHdr.mLen);

	pDrvCtrl->txRing[tx].buf0 =
			(UINT32)TSI108_VIRT_TO_PHYS(pMblk->mBlkHdr.mData);
	pDrvCtrl->txRing[tx].len = pMblk->mBlkHdr.mLen;

	EIEIO;
	pDrvCtrl->txRing[tx].misc =
			    (UINT32)(TSI108_TX_SOF | TSI108_TX_EOF | TSI108_TX_OWN);
	EIEIO;

	TSI108_CACHE_FLUSH(&pDrvCtrl->txRing[tx], sizeof(TX_DESC));

	pDrvCtrl->txMblks[tx] = NULL; /* no Mblk to keep for polling mode */

        /* We need to initiate transmission here. */

        tsi108RestartTx(pDrvCtrl);

	/* Wait until packet is transmitted */
        do
	    {
            if (i++ > 10000)
                break;

	    TSI108_CACHE_INVALIDATE(&pDrvCtrl->txRing[tx], sizeof(TX_DESC));

	    } while (pDrvCtrl->txRing[tx].misc & TSI108_TX_OWN);
            
	    
	pDrvCtrl->txHead = (pDrvCtrl->txHead + 1) % TSI108_TXRING_LEN;
	pDrvCtrl->txFree--;
	}

    tsi108CleanCompletedTbd(pDrvCtrl);

    TSI108_LOG(TSI108_DEBUG_POLL_TX, "tsi108EndPollSend : OUT\n",1,2,3,4,5,6);

    return (OK);
    }


/*******************************************************************************
*
* tsi108CleanCompletedTbd - check completed frame transmission
*
* This routine checks for completed frame transmissions, recover
* processed tx descriptors and associated packet memory.
*
* RETURNS: number of freed TBDs
*/

LOCAL int tsi108CleanCompletedTbd
    (
    TSI108_DRV_CTRL * pDrvCtrl
    )
    {
    int count = 0;
    int tx;

    while (!pDrvCtrl->txFree || (pDrvCtrl->txHead != pDrvCtrl->txTail))
	{
	tx = pDrvCtrl->txTail;

	TSI108_CACHE_INVALIDATE(&pDrvCtrl->txRing[tx], sizeof(TX_DESC));

	/* Check if descriptor is owned by GIGE port */
	if (pDrvCtrl->txRing[tx].misc & TSI108_TX_OWN)
	    break;
	
	/* FRAME_TRANSMIT_OK is only set if the descriptor's EOF is set,
	 * status bits are zeroed if descriptor does not have EOF bit set.
	 * This is because transmission status is not known until all
	 * descriptors of the frame have been processed.
	 */

	if (!(pDrvCtrl->txRing[tx].misc & TSI108_TX_OK) &&
	    (pDrvCtrl->txRing[tx].misc & TSI108_TX_EOF))
	    {
	    TSI108_LOG(TSI108_DEBUG_TX,("tsiG port%d: bad tx packet, misc 0x%x\n"),
			(int)pDrvCtrl->portNum,
			(int)pDrvCtrl->txRing[tx].misc, 0, 0, 0, 0);
	    }

	pDrvCtrl->txTail = (pDrvCtrl->txTail + 1) % TSI108_TXRING_LEN;
	pDrvCtrl->txFree++;
	count++;
	}

    return (count);
    }


/*******************************************************************************
*
* tsi108EndPollRcv - routine to receive a packet in polled mode.
*
* Polled mode operation takes place without any kernel or other OS
* services available.  Use extreme care to insure that this code does not
* call any kernel services.  Polled mode is only for WDB system mode use.
* Kernel services, semaphores, tasks, etc, are not available during WDB
* system mode.
*
* The WDB agent polls the device constantly looking for new data.  Typically
* the device has a ring of RFDs to receive incoming packets.  This routine
* examines the ring for any new data and copies it to the provided mblk.
* The concern here is to keep the device supplied with empty buffers at all
* time.
*
* RETURNS: OK upon success.  EAGAIN is returned when no packet is available.
* A return of ERROR indicates a hardware fault or no support for polled mode
* at all.
*/

LOCAL STATUS tsi108EndPollRcv(TSI108_DRV_CTRL* pDrvCtrl, M_BLK_ID pMblk)
    {
    int rx;
    UINT16    len;        /* number of bytes received */
    char*     pData;      /* receve data pointer */
    int       retVal;

    if (!(pDrvCtrl->flags & TSI108_POLLING))
	return (ERROR);

    if (!(pMblk->mBlkHdr.mFlags & M_EXT))
        {
	TSI108_LOG(TSI108_DEBUG_POLL_RX, "tsi108EndPollRcv : bad Mblk\n",
			1,2,3,4,5,6);
        return (EAGAIN);
        }

    TSI108_LOG(TSI108_DEBUG_POLL_RX, "tsi108EndPollRcv : IN\n",1,2,3,4,5,6);

    rx = pDrvCtrl->rxTail;

    /* Check RX descriptor status if it's not ready, come back later */

    TSI108_CACHE_INVALIDATE (&pDrvCtrl->rxRing[rx], sizeof(RX_DESC));

    if (pDrvCtrl->rxRing[rx].misc & TSI108_RX_OWN)
        return (EAGAIN);

    len = pDrvCtrl->rxRing[rx].len;

    if ((pDrvCtrl->rxRing[rx].misc & TSI108_RX_BAD) || 
	(pMblk->mBlkHdr.mLen < len))
        {
        retVal = EAGAIN;
        }
    else
	{
	/* set up Mblk */
	pMblk->mBlkHdr.mFlags |= M_PKTHDR;
	pMblk->mBlkHdr.mLen = len;
	pMblk->mBlkPktHdr.len = len;

	pData = pDrvCtrl->rxMblks[rx]->m_data;

	/* make the packet data coherent */
	TSI108_CACHE_INVALIDATE (pData, len);

        bcopy ((char *) pData, (char *) pMblk->mBlkHdr.mData, len);

        retVal = OK;
	}

    pDrvCtrl->rxRing[rx].misc = TSI108_RX_OWN | TSI108_RX_INT;
    TSI108_CACHE_FLUSH(&pDrvCtrl->rxRing[rx], sizeof(RX_DESC));
    pDrvCtrl->rxTail = (pDrvCtrl->rxTail + 1) % TSI108_RXRING_LEN;
    pDrvCtrl->rxHead = (pDrvCtrl->rxHead + 1) % TSI108_RXRING_LEN;

    if (!TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_EC_RXSTAT) &
        TSI108_EC_RXSTAT_QUEUE0)
        tsi108RestartRx(pDrvCtrl);

    TSI108_LOG(TSI108_DEBUG_POLL_RX, "tsi108EndPollRcv : OUT\n",1,2,3,4,5,6);

    return (retVal);
    }


/*******************************************************************************
*
* tsi108EndMCastAdd - add a multicast address for the device
*
* This routine adds a multicast address to whatever the driver
* is already listening for.  It then resets the address filter.
*
* RETURNS: OK or ERROR.
*/

LOCAL STATUS tsi108EndMCastAdd
    (
    TSI108_DRV_CTRL*	pDrvCtrl,   /* device pointer */
    char*		pAddress    /* new address to add */
    )
    {
    int error;
    UINT32 rxcfg;

    if ((error = etherMultiAdd (&pDrvCtrl->end.multiList, pAddress)) == 
	ENETRESET)
	{
	pDrvCtrl->end.nMulti++;
	/* Enable multicast packets if we add first element */
	if (pDrvCtrl->end.nMulti == 1)
	    {
	    rxcfg = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_EC_RXCFG);
	    rxcfg |= (TSI108_EC_RXCFG_MFE | TSI108_EC_RXCFG_MC_HASH);
	    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_RXCFG, rxcfg);
	    }

	if (pDrvCtrl->end.nMulti  > TSI108_MAX_NUM_MULTI)
	    {
	    pDrvCtrl->end.nMulti--;

	    etherMultiDel (&pDrvCtrl->end.multiList,pAddress);

	    return ERROR;
	    }
	else
	    {
	    return (tsi108HashTblPopulate(pDrvCtrl));
	    }
	
	}

    return (OK);
    }


/*******************************************************************************
*
* tsi108EndMCastDel - delete a multicast address for the device
*
* This routine removes a multicast address from whatever the driver
* is listening for. It then reloads controller's hash table.
*
* RETURNS: OK or ERROR.
*/

LOCAL STATUS tsi108EndMCastDel
    (
    TSI108_DRV_CTRL*	pDrvCtrl,   /* device pointer */
    char*   pAddress		    /* address to be deleted */
    )
    {
    int error;
    UINT32 rxcfg;

    if ((error = etherMultiDel (&pDrvCtrl->end.multiList,(char *)pAddress)) 
        == ENETRESET)
        {
	tsi108HashTblPopulate(pDrvCtrl);
        pDrvCtrl->end.nMulti--;

	/* Disable multicast packets if we have no addresses in the list */
	if (pDrvCtrl->end.nMulti == 0)
	    {
	    rxcfg = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_EC_RXCFG);
	    rxcfg &= ~(TSI108_EC_RXCFG_MFE | TSI108_EC_RXCFG_MC_HASH);
	    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_RXCFG, rxcfg);
	    }
        }

    return (OK);
    }


/*******************************************************************************
*
* tsi108EndMCastGet - get the multicast address list for the device
*
* This routine gets the multicast list of whatever the driver
* is already listening for.
*
* RETURNS: OK or ERROR.
*/

LOCAL STATUS tsi108EndMCastGet
    (
    TSI108_DRV_CTRL*    pDrvCtrl,   /* device pointer */
    MULTI_TABLE*	pTable      /* address table to be filled in */
    )
    {
    return (etherMultiGet (&pDrvCtrl->end.multiList, pTable));
    }


/*******************************************************************************
* tsi108HashTblPopulate - populate the hash table
*
* This routine populates the hash table with the entries found in the
* multicast table. Every time entire 512-bit hash table is regenerated
* and written into controller. 
*
* RETURNS: OK, or ERROR.
*/
LOCAL STATUS tsi108HashTblPopulate
    (
    TSI108_DRV_CTRL*  pDrvCtrl	/* pointer to the driver control structure */
    )
    {
    ETHER_MULTI * mCastNode = NULL;
    UINT8         byte;
    UINT8         msb;
    UINT32        crc;
    int           count;
    UINT32        hash[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int           i;

    /* scan the multicast address list to re-populate */

    for (
	mCastNode = (ETHER_MULTI*)lstFirst(&pDrvCtrl->end.multiList);
        mCastNode != NULL;
        mCastNode = (ETHER_MULTI*)lstNext(&mCastNode->node)
	)
	{
        /* add this entry */
        crc = 0xffffffff;

        for (i = 0; i < 6; i++) /* for every byte in address */
            {
            byte = mCastNode->addr[i];

            for (count = 0; count < 8; count++)
                {
                if (crc&0x80000000)
                    msb = 1;
                else
                    msb = 0;
                crc <<= 1;
                if (msb ^ (byte&1) )
                    crc ^= ETHER_POLY;
                byte>>=1;
                }
            }

        /* The 9 most significant bits determine the hash code. */
        
        crc = ((~crc) >> 23) & 0x000001ff;

        hash[(crc >> 5)] |= (1 << (crc&0x1f));
        }

    /* Update controller hash table */

    /* Set auto-icrement mode and start index 0 for multi-cast table */
    TSI108_GIGE_REG_WR(pDrvCtrl,
		       TSI108_EC_HASHADDR,
		       TSI108_EC_HASHADDR_AUTOINC | TSI108_EC_HASHADDR_MCAST);

    for (i = 0; i < 16; i++)
	{
	TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_HASHDATA, hash[i]);

	/* Do dummy read to avoid back-to-back writes to the data register.*/
	crc = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_EC_HASHADDR);
	}

    return(OK);
    }


/*******************************************************************************
*
* tsi108ReadMii - inline function to read PHY registers through MII interface
*
* Note that the PHY is accessed through the same registers for both
* interfaces, so this can't be made interface-specific.
*
* RETURNS: register content if status is OK
*          0xffff if status if ERROR
*/

LOCAL __inline__ UINT16 tsi108ReadMii
    (
    TSI108_DRV_CTRL * pDrvCtrl,
    int reg,
    int *status
    )
    {
    int i;
    UINT16 ret;

    TSI108_GIGE_PHYREG_WR(pDrvCtrl,
			  TSI108_MAC_MII_ADDR,
			  ((pDrvCtrl->phy <<
			    TSI108_MAC_MII_ADDR_PHY) |
			   (reg <<
			    TSI108_MAC_MII_ADDR_REG)));
    
    TSI108_GIGE_PHYREG_WR(pDrvCtrl,
			  TSI108_MAC_MII_CMD,
			  0);
    
    TSI108_GIGE_PHYREG_WR(pDrvCtrl,
			  TSI108_MAC_MII_CMD,
			  TSI108_MAC_MII_CMD_READ);

    for (i = 0; i < 100; i++)
	{
	if (!(TSI108_GIGE_PHYREG_RD(pDrvCtrl, TSI108_MAC_MII_IND) & 
	      (TSI108_MAC_MII_IND_NOTVALID | TSI108_MAC_MII_IND_BUSY)))
	    break;

	sysUsDelay(10);
	}
	
    if (i == 100)
	{
	if (status)
	    *status = ERROR;
		
	ret = 0xffff;
	}
    else
	{
	if (status)
	    *status = OK;
	 	
	ret = TSI108_GIGE_PHYREG_RD(pDrvCtrl, TSI108_MAC_MII_DATAIN);
	}
	
    return ret;
    }

/*******************************************************************************
*
* tsi108WriteMii - inline function to write PHY registers through MII
*                  interface
*
* RETURNS: none
*/

LOCAL __inline__ void tsi108WriteMii
    (
    TSI108_DRV_CTRL * pDrvCtrl,
    int reg,
    UINT16 val
    )
    {
    TSI108_GIGE_PHYREG_WR(pDrvCtrl,
			  TSI108_MAC_MII_ADDR,
			  ((pDrvCtrl->phy <<
			    TSI108_MAC_MII_ADDR_PHY) |
			   (reg <<
			    TSI108_MAC_MII_ADDR_REG)));
    
    TSI108_GIGE_PHYREG_WR(pDrvCtrl,
			  TSI108_MAC_MII_DATAOUT,
			  val);
    while (TSI108_GIGE_PHYREG_RD(pDrvCtrl, TSI108_MAC_MII_IND) & 
	    TSI108_MAC_MII_IND_BUSY);
    }

/*******************************************************************************
*
* tsi108WriteTbi - inline function to write to TBI registers
*
* RETURNS: none
*/

LOCAL __inline__ void tsi108WriteTbi
    (
    TSI108_DRV_CTRL * pDrvCtrl,
    int reg,
    UINT16 val
    )
    {
    
    TSI108_GIGE_REG_WR(pDrvCtrl,
		       TSI108_MAC_MII_ADDR,
		       ((0x1e << TSI108_MAC_MII_ADDR_PHY) |
			(reg << TSI108_MAC_MII_ADDR_REG)));
    
    TSI108_GIGE_REG_WR(pDrvCtrl,
		       TSI108_MAC_MII_DATAOUT,
		       val);

    while (TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_MAC_MII_IND)
	   & TSI108_MAC_MII_IND_BUSY);
    }

/*******************************************************************************
*
* tsi108CheckPhy - check PHY link and control status
*
* This routine checks the PHY link and control status, use this information
* to configure the MAC configuration and port control registers, and
* initialize members of the driver control structure.
*
* RETURNS: none
*/

LOCAL void tsi108CheckPhy
    (
    TSI108_DRV_CTRL * pDrvCtrl
    )
    {
    UINT16 sumstat;
    UINT32 tmp = 0;

    /* Do a dummy read, as for some reason the first read
     * after a link becomes up returns link down, even if
     * it's been a while since the link came up.
     */

    semTake(pDrvCtrl->phyLock, WAIT_FOREVER);
	
    if (!pDrvCtrl->phy_ok)
	goto out;
	
    tsi108ReadMii(pDrvCtrl, PHY_STAT, NULL);
	
    if (!(tsi108ReadMii(pDrvCtrl, PHY_STAT, NULL) & PHY_STAT_LINKUP))
	{
	if (pDrvCtrl->link_up == 1)
	    {
	    pDrvCtrl->link_up = 0; 
	    }
		
	goto out;
	}

#ifdef TSI108_PHY_MV88E10XX
    sumstat = tsi108ReadMii(pDrvCtrl, PHY_SUM_STAT, NULL);
    /*	printf("PHY sumstat = 0x%08x\n",sumstat); */

    tmp = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_MAC_CFG2);

    switch (sumstat & PHY_SUM_STAT_SPEED_MASK)
	{
	case PHY_SUM_STAT_SPEED_10:
	    if (pDrvCtrl->speed != 10)
		{
		tmp &= ~TSI108_MAC_CFG2_IFACE_MASK;

		TSI108_GIGE_REG_WR(pDrvCtrl,
				   TSI108_MAC_CFG2,
				   tmp);
		
		tmp = (TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_MAC_CFG2)
		       | TSI108_MAC_CFG2_NOGIG);
		TSI108_GIGE_REG_WR(pDrvCtrl,
				   TSI108_MAC_CFG2,
				   tmp);
					 
		tmp = (TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_EC_PORTCTRL)
		       | TSI108_EC_PORTCTRL_NOGIG);
		TSI108_GIGE_REG_WR(pDrvCtrl,
				   TSI108_EC_PORTCTRL,
				   tmp);
		
		pDrvCtrl->speed = 10;
		}
	    break;
		
	case PHY_SUM_STAT_SPEED_100:
	    if (pDrvCtrl->speed != 100)
		{
		tmp &= ~TSI108_MAC_CFG2_IFACE_MASK;

		TSI108_GIGE_REG_WR(pDrvCtrl,
				   TSI108_MAC_CFG2,
				   tmp);
		
		tmp = (TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_MAC_CFG2)
		       | TSI108_MAC_CFG2_NOGIG);
		TSI108_GIGE_REG_WR(pDrvCtrl,
				   TSI108_MAC_CFG2,
				   tmp);
		
		tmp = (TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_EC_PORTCTRL)
		       | TSI108_EC_PORTCTRL_NOGIG);
		TSI108_GIGE_REG_WR(pDrvCtrl,
				   TSI108_EC_PORTCTRL,
				   tmp);
		
		pDrvCtrl->speed = 100;
		}
	    break;
		
	case PHY_SUM_STAT_SPEED_1000:
	    if (pDrvCtrl->speed != 1000)
		{
		tmp &= ~TSI108_MAC_CFG2_IFACE_MASK;

		TSI108_GIGE_REG_WR(pDrvCtrl,
				   TSI108_MAC_CFG2,
				   tmp);
		
		tmp = (TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_MAC_CFG2)
		       | TSI108_MAC_CFG2_GIG);
		TSI108_GIGE_REG_WR(pDrvCtrl,
				   TSI108_MAC_CFG2,
				   tmp);
		
		tmp = (TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_EC_PORTCTRL)
		       & ~TSI108_EC_PORTCTRL_NOGIG);
		TSI108_GIGE_REG_WR(pDrvCtrl,
				   TSI108_EC_PORTCTRL,
				   tmp);
		
		pDrvCtrl->speed = 1000;
		}
	    break;

	default:

	    printf( "PHY reported invalid speed, summary status %x\n",
		    sumstat);

	    goto out;
	}

    if (sumstat & PHY_SUM_STAT_FULLDUPLEX)
	{
	if (pDrvCtrl->duplex != 2)
	    {
	    tmp = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_MAC_CFG2);
	    TSI108_GIGE_REG_WR(pDrvCtrl,
			TSI108_MAC_CFG2,
			    tmp | TSI108_MAC_CFG2_FULLDUPLEX);
	    
	    tmp = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_EC_PORTCTRL);

	    TSI108_GIGE_REG_WR(pDrvCtrl,
			    TSI108_EC_PORTCTRL,
				tmp & ~TSI108_EC_PORTCTRL_HALFDUPLEX);

	    pDrvCtrl->duplex = 2;
	    }
	}
    else
	{
	if (pDrvCtrl->duplex != 1)
	    {
	    tmp = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_MAC_CFG2);
	    TSI108_GIGE_REG_WR(pDrvCtrl,
				TSI108_MAC_CFG2,
				tmp & ~TSI108_MAC_CFG2_FULLDUPLEX);

	    tmp = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_EC_PORTCTRL);

	    TSI108_GIGE_REG_WR(pDrvCtrl,
			       TSI108_EC_PORTCTRL,
			       tmp | TSI108_EC_PORTCTRL_HALFDUPLEX);

	    pDrvCtrl->duplex = 1;
	    }
	}
#else /* TSI108_PHY_BCM54XX */
    {
    UINT32 mac_cfg2_reg;
    UINT32 portctrl_reg;
    UINT32 fdx_flag = 0;
    UINT32 reg_update = 0;

    mac_cfg2_reg = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_MAC_CFG2);
    portctrl_reg = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_EC_PORTCTRL);

    sumstat = tsi108ReadMii(pDrvCtrl, PHY_SUM_STAT, NULL);

    switch (sumstat & PHY_SUM_STAT_SPEED_MASK) {
    case PHY_SUM_STAT_1000T_FD:
	fdx_flag++;
    case PHY_SUM_STAT_1000T_HD:
	if (pDrvCtrl->speed != 1000)
	    {
	    mac_cfg2_reg &= ~TSI108_MAC_CFG2_IFACE_MASK;
	    mac_cfg2_reg |= TSI108_MAC_CFG2_GIG;
	    portctrl_reg &= ~TSI108_EC_PORTCTRL_NOGIG;
	    pDrvCtrl->speed = 1000;
	    reg_update++;
	    }
	break;


    case PHY_SUM_STAT_100TX_FD:
	fdx_flag++;
    case PHY_SUM_STAT_100TX_HD:
	if (pDrvCtrl->speed != 100)
	    {
	    mac_cfg2_reg &= ~TSI108_MAC_CFG2_IFACE_MASK;
	    mac_cfg2_reg |= TSI108_MAC_CFG2_NOGIG;
	    portctrl_reg |= TSI108_EC_PORTCTRL_NOGIG;
	    pDrvCtrl->speed = 100;
	    reg_update++;
	    }
	break;
		
    case PHY_SUM_STAT_10T_FD:
	fdx_flag++;
    case PHY_SUM_STAT_10T_HD:
	if (pDrvCtrl->speed != 10)
	    {
	    mac_cfg2_reg &= ~TSI108_MAC_CFG2_IFACE_MASK;
	    mac_cfg2_reg |= TSI108_MAC_CFG2_NOGIG;
	    portctrl_reg |= TSI108_EC_PORTCTRL_NOGIG;
	    pDrvCtrl->speed = 10;
	    reg_update++;
	    }
	break;

    default:
	printf( "PHY reported invalid speed, summary status %x\n",
		sumstat);

	goto out;
    }

    if (fdx_flag)
	{
	if (pDrvCtrl->duplex != 2)
	    {
	    mac_cfg2_reg |= TSI108_MAC_CFG2_FULLDUPLEX;
	    portctrl_reg &= ~TSI108_EC_PORTCTRL_HALFDUPLEX;
	    pDrvCtrl->duplex = 2;
	    reg_update++;
	    }
	}
    else
	{
	if (pDrvCtrl->duplex != 1) {
	mac_cfg2_reg &= ~TSI108_MAC_CFG2_FULLDUPLEX;
	portctrl_reg |= TSI108_EC_PORTCTRL_HALFDUPLEX;
	pDrvCtrl->duplex = 1;
	reg_update++;
	}
	}

    if (reg_update)
	{
	TSI108_GIGE_REG_WR(pDrvCtrl,
			   TSI108_MAC_CFG2,
			   mac_cfg2_reg);
		
	TSI108_GIGE_REG_WR(pDrvCtrl,
			   TSI108_EC_PORTCTRL,
			   portctrl_reg);
	}
    }
#endif
    
    if (pDrvCtrl->link_up == 0)
	{
	/* The manual says it can take 3-4 usecs for the speed change
	 * to take effect.
	 */
	sysUsDelay(5);

	pDrvCtrl->link_up = 1;
	}

    out:
    semGive(pDrvCtrl->phyLock);
    }


/*******************************************************************************
*
* tsi108RestartRx - restart GigE rx path
*
* This routine sets the VALID, GO and QUEUE_ENABLES bits, essentially
* making the rx descriptor valid, enabling frame reception and rx queue0.
*
* RETURNS: OK or ERROR
*/
LOCAL void tsi108RestartRx
    (
    TSI108_DRV_CTRL * pDrvCtrl
    )
    {
    TSI108_GIGE_REG_WR(pDrvCtrl,
	TSI108_EC_RXQ_PTRHIGH, TSI108_EC_RXQ_PTRHIGH_VALID);
    
    TSI108_LOG(TSI108_DEBUG_RX,	("*** tsi108RestartRx ***\n"),1,2,3,4,5,6);

    TSI108_GIGE_REG_WR(pDrvCtrl,
	TSI108_EC_RXCTRL, (TSI108_EC_RXCTRL_GO | TSI108_EC_RXCTRL_QUEUE0));
    }

/*******************************************************************************
*
* tsi108RestartTx - restart GigE tx path
*
* This routine sets the VALID, GO and QUEUE_ENABLES bits, essentially
* making the tx descriptor valid, enabling frame transmission and tx queue0.
*
* RETURNS: OK or ERROR
*/
LOCAL void tsi108RestartTx
    (
    TSI108_DRV_CTRL * pDrvCtrl
    )
    {
    TSI108_GIGE_REG_WR(pDrvCtrl,
	TSI108_EC_TXQ_PTRHIGH, TSI108_EC_TXQ_PTRHIGH_VALID);

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_TXCTRL,
	(TSI108_EC_TXCTRL_IDLEINT |
	    TSI108_EC_TXCTRL_GO | TSI108_EC_TXCTRL_QUEUE0));
    }


/*******************************************************************************
*
* tsi108CheckForCompletedTx - check completed frame transmission
*
* This routine checks for completed frame transmissions, recover
* processed tx descriptors and associated packet memory.
*
* RETURNS: OK or ERROR
*/

LOCAL int tsi108CheckForCompletedTx
    (
    TSI108_DRV_CTRL * pDrvCtrl
    )
    {
    M_BLK_ID pMblk;
    int count = 0;
    int tx;

    while (!pDrvCtrl->txFree || pDrvCtrl->txHead != pDrvCtrl->txTail)
	{
	tx = pDrvCtrl->txTail;

	TSI108_CACHE_INVALIDATE(&pDrvCtrl->txRing[tx], sizeof(TX_DESC));

	/* Check if the descriptor is owned by GIGE port */
	if (pDrvCtrl->txRing[tx].misc & TSI108_TX_OWN)
	    break;
	
	/* FRAME_TRANSMIT_OK is only set if the descriptor's EOF is set,
	 * status bits are zeroed if descriptor does not have EOF bit set.
	 * This is because transmission status is not known until all
	 * descriptors of the frame have been processed.
	 */
	if (!(pDrvCtrl->txRing[tx].misc & TSI108_TX_OK) &&
	    (pDrvCtrl->txRing[tx].misc & TSI108_TX_EOF))
	    {
	    TSI108_LOG(TSI108_DEBUG_TX,
		("tsi108CheckForCompletedTx:%d: bad tx packet, misc %x\n"),
		(int)pDrvCtrl->portNum,	(int)pDrvCtrl->txRing[tx].misc,
		0, 0, 0, 0);
	    }

	pDrvCtrl->txTail = (pDrvCtrl->txTail + 1) % TSI108_TXRING_LEN;
	pDrvCtrl->txFree++;
	count++;

	pMblk = pDrvCtrl->txMblks[tx];

	if (pMblk != NULL)
            {
	    netMblkClChainFree(pMblk);
	    pDrvCtrl->txMblks[tx] = NULL;
            }

	}

    if (pDrvCtrl->txFree < TSI108_TXRING_LEN &&
        !(TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_EC_TXSTAT) &
	    TSI108_EC_TXSTAT_QUEUE0))
        tsi108RestartTx(pDrvCtrl);

    if (count != 0)
	if (pDrvCtrl->txStall)
	    {
	    netJobAdd((FUNCPTR)muxTxRestart, (int)&pDrvCtrl->end, 0, 0, 0, 0);
	    pDrvCtrl->txStall = FALSE;             
	    }

    return (count);
    }

/*******************************************************************************
*
* tsi108EndEncap - encapsulate an mBlk chain within the TX DMA ring
*
* This routine sets up a scatter/gather DMA for an mBlk chain. If
* the chain is too long, or there aren't enough descriptors available,
* the function aborts any changes it may have made to the DMA ring and
* returns an error, otherwise it initiates the transmission.
*
* RETURNS: OK or ERROR
*/

#define TSI108_MAX_TX_FRAGS 16

LOCAL STATUS tsi108EndEncap
    (
    TSI108_DRV_CTRL * pDrvCtrl,
    M_BLK    * pMblk
    )
    {
    volatile TX_DESC	*pTbd = NULL;
    M_BLK_ID    pCurr;
    UINT32      used;
    int         tx, ix, first;

    /* save the first TBD pointer */

    tx = first = pDrvCtrl->txHead;

    for (pCurr = pMblk, used = 0; pCurr != NULL; pCurr = pCurr->m_next)
        {
        if (pCurr->m_len != 0)
            {
            if (used == TSI108_MAX_TX_FRAGS || pDrvCtrl->txFree == 0)
                break;

            /* get the current pTbd */

            pTbd = &pDrvCtrl->txRing[tx];

            /*
             * Sanity check: if the descriptor is still in use,
             * we're out of descriptors.
             */

            TSI108_CACHE_INVALIDATE (pTbd, sizeof(TX_DESC));
            if (pTbd->misc & TSI108_TX_OWN)
                break;

            /* Set up len, buffer addr, descriptor flags. */

            pTbd->len = pCurr->m_len;
            pTbd->buf0 = (UINT32)TSI108_VIRT_TO_PHYS(pCurr->m_data);
            pTbd->misc = 0;

            if ((tx % TSI108_TX_INT_FREQ == 0) &&
                ((TSI108_TXRING_LEN - pDrvCtrl->txFree) >= TSI108_TX_INT_FREQ))
                pTbd->misc |= TSI108_TX_INT;

            TSI108_USER_CACHE_FLUSH(pCurr->m_data, pCurr->m_len);

            /*
             * Set the ownership bit everywhere except the first
             * descriptor.
             */

            if (used)
                pTbd->misc |= TSI108_TX_OWN;
            else
                pTbd->misc |= TSI108_TX_SOF;

            EIEIO;

            TSI108_CACHE_FLUSH(pTbd, sizeof(TX_DESC));

            tx = (tx + 1) % TSI108_TXRING_LEN;

            used++;
            pDrvCtrl->txFree--;
            }
        }

    if (pCurr != NULL)
        goto noDescs;

    /* Save the mBlk chain. */
    pDrvCtrl->txMblks[(first + used - 1) % TSI108_TXRING_LEN ] = pMblk;
    /* Save the new index location. */
    pDrvCtrl->txHead = tx;

    /* Set the end of packet bit in the last descriptor. */
    pTbd->misc |= TSI108_TX_EOF;
    EIEIO;
    TSI108_CACHE_FLUSH(pTbd, sizeof(TX_DESC));

    /* Set the own bit in the first descriptor. */
    pTbd = &pDrvCtrl->txRing[first];
    pTbd->misc |= TSI108_TX_OWN;
    EIEIO;
    TSI108_CACHE_FLUSH(pTbd, sizeof(TX_DESC));

    /* Flush any completed transmissions. */
    tsi108CheckForCompletedTx(pDrvCtrl);

    /* Restart the DMA channel, if needed. */

    if (!(TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_EC_TXSTAT) &
	    TSI108_EC_TXSTAT_QUEUE0))
        tsi108RestartTx(pDrvCtrl);

    return (OK);

noDescs:

    /*
     * Ran out of descriptors: undo all relevant changes
     * and fall back to copying.
     */

    tx = first;
    for (ix = 0; ix < used; ix++)
        {
        pTbd = &pDrvCtrl->txRing[tx];
        pTbd->misc = 0;
        pTbd->buf0 = 0;
        TSI108_CACHE_FLUSH(pTbd, sizeof(TX_DESC));
        pDrvCtrl->txFree++;
        tx = (tx + 1) % TSI108_TXRING_LEN;
        }

    return (ERROR);
    }

/*******************************************************************************
*
* tsi108EndSend - the tsi108 GigE driver send routine
*
* This routine takes a M_BLK_ID sends off the data in the M_BLK_ID.
* The buffer must already have the addressing information properly installed
* in it.  This is done by a higher layer.  
*
* RETURNS: OK, ERROR, or END_ERR_BLOCK.
*/

LOCAL STATUS tsi108EndSend
    (
    TSI108_DRV_CTRL * pDrvCtrl,
    M_BLK    * pMblk
    )
    {
    int rval;
    int len;
    M_BLK_ID pTmp;

    END_TX_SEM_TAKE(&pDrvCtrl->end, WAIT_FOREVER);

    if (!pDrvCtrl->phy_ok)
	TSI108_LOG(TSI108_DEBUG_TX, ("tsi108EndSend:%d: Transmit while PHY is down!\n"),
		   pDrvCtrl->unit,2,3,4,5,6);

    if (!pDrvCtrl->link_up)
	{
	TSI108_LOG(TSI108_DEBUG_TX, ("tsi108EndSend:%d: link down!\n"),
		    pDrvCtrl->unit, 2,3,4,5,6);

        if (pDrvCtrl->txStall != TRUE)
            {
            pDrvCtrl->txStall = TRUE;
	    }

        goto send_block;
	}

    if (pDrvCtrl->txStall == TRUE || pDrvCtrl->txFree == 0)
        goto send_block;

    /*
     * First, try to do an in-place transmission, using
     * gather-write DMA.
     */

    rval = tsi108EndEncap (pDrvCtrl, pMblk);

    /*
     * In-place DMA didn't work, coalesce everything
     * into a single buffer and try again.
     */

    if (rval == ERROR)
        {
        if ((pTmp = netTupleGet (pDrvCtrl->end.pNetPool,
                                  TSI108_RXBUF_SIZE - sizeof(CL_POOL_ID),
                                  M_DONTWAIT, MT_DATA, 0)) == NULL)
            goto send_block;

        len = netMblkToBufCopy (pMblk, mtod(pTmp, char *), NULL);
        pTmp->m_len = pTmp->m_pkthdr.len = len;
        pTmp->m_flags = pMblk->m_flags;
        netMblkClChainFree (pMblk);
        pMblk = pTmp;
        /* Try transmission again, must succeed this time. */
        rval = tsi108EndEncap (pDrvCtrl, pMblk);
        }

    if (rval == ERROR)
        goto send_block;

    END_TX_SEM_GIVE (&pDrvCtrl->end);

    return (OK);

send_block:

    pDrvCtrl->txStall = TRUE;
    END_TX_SEM_GIVE (&pDrvCtrl->end);

    return (END_ERR_BLOCK);
    }

/*******************************************************************************
*
* tsi108HandleRx - the tsi108 GigE driver receive routine
*
* This routine is the tsi108 GigE driver receive routine that runs at
* task level. When a rx interrupt occurs, this routine is scheduled to
* run.
*
* RETURNS: none
*/

#define TSI108_MAX_RX_BUFS 16

LOCAL int tsi108HandleRx
    (
    TSI108_DRV_CTRL * pDrvCtrl
    )
    {
    M_BLK_ID pMblk;
    M_BLK_ID pNewMblk;
    volatile RX_DESC * pRbd;
    int rx, loopCounter;
    UINT32 tmp;

    loopCounter = TSI108_MAX_RX_BUFS;

    rx = pDrvCtrl->rxTail;
    pRbd = &pDrvCtrl->rxRing[rx];
    TSI108_CACHE_INVALIDATE (pRbd, sizeof(RX_DESC));

    while (loopCounter && !(pRbd->misc & TSI108_RX_OWN)) 
        {

	pDrvCtrl->rxTail = (pDrvCtrl->rxTail + 1) % TSI108_RXRING_LEN;
	pDrvCtrl->rxFree--;

        /* Check for RX errors. */

	if (pRbd->misc & TSI108_RX_BAD)
	    {
	    TSI108_LOG(TSI108_DEBUG_ERROR,
			("*** TSI108_RX_BAD ***\n"),0,0,0,0,0,0);

	    if (pRbd->misc & TSI108_RX_CRC)
		{
		TSI108_LOG(TSI108_DEBUG_ERROR,
			    ("*** TSI108_RX_CRC ***\n"),0,0,0,0,0,0);
		}
	    if (pRbd->misc & TSI108_RX_OVER)
		{
		TSI108_LOG(TSI108_DEBUG_ERROR,
			    ("*** TSI108_RX_OVER ***\n"),0,0,0,0,0,0);
		}

            pRbd->misc = TSI108_RX_OWN|TSI108_RX_INT;
            TSI108_CACHE_FLUSH (pRbd, sizeof(RX_DESC));
            rx = pDrvCtrl->rxTail;
            pRbd = &pDrvCtrl->rxRing[rx];
            TSI108_CACHE_INVALIDATE (pRbd, sizeof(RX_DESC));
	    pDrvCtrl->rxFree++;
	    continue;
	    }

        /*
         * Try to conjure up a new mBlk tuple to replace the one
         * we're about to loan out. If that fails, leave the
         * old one in place and reset the descriptor to allow
         * a new packet to be received.
         */

        pNewMblk = netTupleGet (pDrvCtrl->end.pNetPool,
                                TSI108_RXBUF_SIZE - sizeof(CL_POOL_ID),
                                M_DONTWAIT, MT_DATA, 0);

        if (pNewMblk == NULL)
            {
            pRbd->misc = TSI108_RX_OWN|TSI108_RX_INT;
            TSI108_CACHE_FLUSH (pRbd, sizeof(RX_DESC));
            rx = pDrvCtrl->rxTail;
            pRbd = &pDrvCtrl->rxRing[rx];
            TSI108_CACHE_INVALIDATE (pRbd, sizeof(RX_DESC));
	    pDrvCtrl->rxFree++;
	    continue;
            }

        /* Set up the new packet. */

        pMblk = pDrvCtrl->rxMblks[rx];
        pMblk->m_len        = (int)pDrvCtrl->rxRing[rx].len - ETHER_CRC_LEN;
        pMblk->m_pkthdr.len = pMblk->m_len;
        pMblk->m_flags      = M_PKTHDR|M_EXT;

        /* Invalidate the buffer */
        TSI108_CACHE_INVALIDATE (pMblk->m_data, pMblk->m_len);

        /*
         * Swap out the old mBlk with the new one and
         * reset the descriptor.
         */

        pDrvCtrl->rxMblks[rx] = pNewMblk;
        pRbd->buf0 = (UINT32)TSI108_VIRT_TO_PHYS(pNewMblk->m_data);
	pRbd->blen = TSI108_RXBUF_SIZE;
        EIEIO;
        pRbd->misc = TSI108_RX_OWN|TSI108_RX_INT;
        TSI108_CACHE_FLUSH (pRbd, sizeof(RX_DESC));
        pDrvCtrl->rxFree++;

#ifdef TSI108END_DBG_SHOW
	pDrvCtrl->pMblk = pMblk;
#endif

	/* call the upper layer's receive routine. */
	END_RCV_RTN_CALL(&pDrvCtrl->end, pMblk);

#ifdef TSI108END_DBG_SHOW
	pDrvCtrl->rxCount++;
#endif

        /* Advance to the next descriptor */

        rx = pDrvCtrl->rxTail;
        pRbd = &pDrvCtrl->rxRing[rx];
        TSI108_CACHE_INVALIDATE (pRbd, sizeof(RX_DESC));
        }

    pDrvCtrl->rxHead = pDrvCtrl->rxTail;

    /*
     * If we exceeded our work quota, queue ourselves up for
     * another round of processing. Otherwise, re-enable interrupts
     * and just bail.
     */

    if (loopCounter == 0)
        {
        netJobAdd ((FUNCPTR)tsi108HandleRx, (int)pDrvCtrl, 0,0,0,0);
        return(0);
        }

    /*
     * Now we do the following:
     * - clear the rxPending flag
     * - re-enable RX interrupts
     * - restart the RX DMA channel, if necessary
     * We must do things in this order to avoid any race
     * conditions.
     */
 
    pDrvCtrl->rxPending = FALSE;

    tmp = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_EC_INTMASK);

    tmp &= ~(TSI108_INT_RXQUEUE0 |
	    TSI108_INT_RXTHRESH  |
	    TSI108_INT_RXOVERRUN |
	    TSI108_INT_RXERROR |
	    TSI108_INT_RXWAIT);


    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_INTMASK, tmp);

    if (!(TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_EC_RXSTAT) &
          TSI108_EC_RXSTAT_QUEUE0))
        tsi108RestartRx(pDrvCtrl);

    return (0);
    }

/*******************************************************************************
*
* tsi108RxInt - the tsi108 GigE driver receive interrupt handler
*
* This routine scheduled 'tsi108HandleRx' to run at task level when rx interrupt
* occurs.
*
* RETURNS: none
*/

LOCAL void tsi108RxInt
    (
    TSI108_DRV_CTRL * pDrvCtrl
    )
    {
    UINT32 tmp = 0;
    UINT32 estat;
    UINT32 err;
    UINT32 intstat;

    /* A race could cause dev to already be scheduled, so it's not an
     * error if that happens (and interrupts shouldn't be re-masked,
     * because that can cause harmful races, if poll has already
     * unmasked them but not cleared LINK_STATE_SCHED).  
     *
     * This can happen if this code races with tsi108HandleRx(), which masks
     * the interrupts after tsi108_irq_one() read the mask, but before
     * netif_rx_schedule is called.  It could also happen due to calls
     * from tsi108_check_rxring(). */

    /* disable receive interrupt */
    tmp = (TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_EC_INTMASK) |
				(TSI108_INT_RXQUEUE0 | TSI108_INT_RXTHRESH |
				TSI108_INT_RXOVERRUN | TSI108_INT_RXERROR |
				TSI108_INT_RXWAIT));

    intstat = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_EC_INTSTAT);
    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_INTSTAT, tmp);

    /* Acknowledge interrupts. */
    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_INTMASK, tmp);
    estat = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_EC_RXESTAT);
    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_RXESTAT, estat);

    if (tmp & TSI108_INT_RXERROR)
	{
	err = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_EC_RXERR);
	TSI108_GIGE_REG_WR(pDrvCtrl,
			   TSI108_EC_RXERR,
			   err);
        }

    if (!(pDrvCtrl->rxPending))
	{
	pDrvCtrl->rxPending = TRUE;
	netJobAdd((FUNCPTR)tsi108HandleRx, (int)pDrvCtrl, 0, 0, 0, 0);
	}
    }


/*******************************************************************************
*
* tsi108ClearTx - the Tx BD ring cleaning routine scheduled by tx
*                 interrupt handler
*
* This routine clears tx interrupt conditions, recovers tx descriptors
* for transmitted frames and frees packet memory.
*
* RETURNS: none
*/

LOCAL void tsi108ClearTx(TSI108_DRV_CTRL * pDrvCtrl)
    {
    END_TX_SEM_TAKE(&pDrvCtrl->end, WAIT_FOREVER);
    tsi108CheckForCompletedTx(pDrvCtrl);
    END_TX_SEM_GIVE (&pDrvCtrl->end);
    pDrvCtrl->txPending = FALSE;
    }

/*******************************************************************************
*
* tsi108TxInt - the tsi108 GigE driver tx interrupt handler
*
* This routine clears tx interrupt conditions and if require schedules
* task that recovers tx descriptors for transmitted frames and frees
* packet memory.
*
* RETURNS: none
*/

LOCAL void tsi108TxInt
    (
    TSI108_DRV_CTRL * pDrvCtrl
    )
    {
    UINT32 estat = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_EC_TXESTAT);
    UINT32 err;

    /* clearing extended stat register */
    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_TXESTAT, estat);

    TSI108_GIGE_REG_WR(pDrvCtrl,
		       TSI108_EC_INTSTAT,
		       (TSI108_INT_TXQUEUE0 |
			TSI108_INT_TXIDLE |
			TSI108_INT_TXERROR));
    
    if (estat & TSI108_EC_TXESTAT_Q0_ERR)
	{
	err = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_EC_TXERR);
	TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_TXERR, err);
		
	if (err)
	    {
	    TSI108_LOG(TSI108_DEBUG_ERROR, ("tsi108TxInt:%d: TX error 0x%x\n"),
					    pDrvCtrl->unit, err, 0,0,0,0);
	    }
	}
	
    if (estat & (TSI108_EC_TXESTAT_Q0_DESCINT | TSI108_EC_TXESTAT_Q0_EOQ))
	{
	if (pDrvCtrl->txPending == FALSE)
	    {
	    pDrvCtrl->txPending = TRUE;
	    netJobAdd((FUNCPTR)tsi108ClearTx, (int)pDrvCtrl, 0, 0, 0, 0);
	    }
	}
    }


/*******************************************************************************
*
* tsi108StatCarryInt - the tsi108 GigE driver Statistics Counters Carry
*			interrupt handler
*
* This routine keeps track of signalled counter overflows
* for MIB2 statistics data
*
* RETURNS: none
*/

LOCAL void tsi108StatCarryInt
    (
    TSI108_DRV_CTRL * pDrvCtrl
    )
    {
    UINT32 statCarry;

    statCarry = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_STAT_CARRY1);

    /* Clear signalled carry flags */
    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_STAT_CARRY1, statCarry);

    /* Process first statistics carry register */
    if (statCarry & TSI108_STAT_CARRY1_RXBYTES)
	pDrvCtrl->carryRxBytes++;
    if (statCarry & TSI108_STAT_CARRY1_RXPKTS)
	pDrvCtrl->carryRxPkt++;
    if (statCarry & TSI108_STAT_CARRY1_RXMCAST)
	pDrvCtrl->carryRxMcastPkt++;
    if (statCarry & TSI108_STAT_CARRY1_RXBCAST)
	pDrvCtrl->carryRxBcastPkt++;

    statCarry = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_STAT_CARRY2);

    /* Clear signalled carry flags */
    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_STAT_CARRY2, statCarry);

    if (statCarry & TSI108_STAT_CARRY2_TXBYTES)
	pDrvCtrl->carryTxBytes++;
    if (statCarry & TSI108_STAT_CARRY2_TXPKTS)
	pDrvCtrl->carryTxPkt++;
    if (statCarry & TSI108_STAT_CARRY2_TXBCAST)
	pDrvCtrl->carryTxBcastPkt++;
    if (statCarry & TSI108_STAT_CARRY2_TXMCAST)
	pDrvCtrl->carryTxMcastPkt++;
    }


/*******************************************************************************
*
* tsi108Interrupt - the tsi108 GigE driver interrupt handler
*
* This routine is the main interrupt handling routine for the tsi108 GigE
* driver, it checks the interrupt source and call the relevant handling
* routine.
*
* RETURNS: none
*/

void tsi108Interrupt
    (
    TSI108_DRV_CTRL * pDrvCtrl
    )
    {
    UINT32 stat = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_EC_INTSTAT);

    if (!(stat & TSI108_INT_ANY))
	return;  /* Not our interrupt */

    stat &= ~TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_EC_INTMASK);

    if (stat & (TSI108_INT_TXQUEUE0 | TSI108_INT_TXIDLE | TSI108_INT_TXERROR))
	tsi108TxInt(pDrvCtrl);
    
    if (stat & (TSI108_INT_RXQUEUE0 | TSI108_INT_RXTHRESH | TSI108_INT_RXWAIT |
		TSI108_INT_RXOVERRUN | TSI108_INT_RXERROR))
	tsi108RxInt(pDrvCtrl);

    if (stat & TSI108_INT_SFN)
	{
	TSI108_LOG(TSI108_DEBUG_ERROR, ("tsi108Interrupt:%d: SFN error\n"),
					pDrvCtrl->unit, 0, 0, 0, 0, 0);

	TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_INTSTAT, TSI108_INT_SFN);
	}

    if (stat & TSI108_INT_STATCARRY)
	{
	tsi108StatCarryInt(pDrvCtrl);

	TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_INTSTAT, TSI108_INT_STATCARRY);
	}
    }

/*******************************************************************************
*
* tsi108StopEthernet - stops tsi108 GigE port
*
* This routine disable both the rx and tx path of the tsi108 GigE port.
*
* RETURNS: none
*/
LOCAL void tsi108StopEthernet
    (
    TSI108_DRV_CTRL * pDrvCtrl
    )
    {
    /* Disable all TX and RX queues... */

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_TXCTRL, 0);

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_RXCTRL, 0);

    /* ...and wait for them to become idle. */

    while (TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_EC_TXSTAT) &
						TSI108_EC_TXSTAT_ACTIVE);
    while (TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_EC_RXSTAT) &
						TSI108_EC_RXSTAT_ACTIVE);
    }

/*******************************************************************************
*
* tsi108ResetEthernet - resets GigE port
*
* This routine resets the MAC, stats, tx/rx logic and MII MGMT.
*
* RETURNS: none
*/

LOCAL void tsi108ResetPort
    (
    TSI108_DRV_CTRL * pDrvCtrl
    )
    {
    UINT32 tmp = 0;
    
    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_MAC_CFG1, TSI108_MAC_CFG1_SOFTRST);

    sysUsDelay(100);

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_MAC_CFG1, 0);

    TSI108_GIGE_REG_WR(pDrvCtrl,
		TSI108_EC_PORTCTRL, TSI108_EC_PORTCTRL_STATRST);

    sysUsDelay(100);

    tmp = (TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_EC_PORTCTRL) &
		~TSI108_EC_PORTCTRL_STATRST);

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_PORTCTRL, tmp);

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_TXCFG, TSI108_EC_TXCFG_RST);

    sysUsDelay(100);

    tmp = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_EC_TXCFG) & ~TSI108_EC_TXCFG_RST;

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_TXCFG, tmp);

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_RXCFG, TSI108_EC_RXCFG_RST);

    sysUsDelay(100);

    tmp = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_EC_RXCFG) & ~TSI108_EC_RXCFG_RST;

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_RXCFG, tmp);

    tmp = (TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_MAC_MII_MGMT_CFG) |
			TSI108_MAC_MII_MGMT_RST);

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_MAC_MII_MGMT_CFG, tmp);

    sysUsDelay(100);

    tmp = (TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_MAC_MII_MGMT_CFG) &
		~(TSI108_MAC_MII_MGMT_RST |TSI108_MAC_MII_MGMT_CLK));

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_MAC_MII_MGMT_CFG, tmp);

    tmp = (TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_MAC_MII_MGMT_CFG) |
		TSI108_MAC_MII_MGMT_CLK);

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_MAC_MII_MGMT_CFG, tmp);
    }


/*******************************************************************************
*
* tsi108GetMacAddress - gets MAC address from HW
*
* This routine gets the MAC address for the GigE port station address
* registers and udates corresponding field in the driver control structure
* if the address is valid.
*
* RETURNS: OK if MAC address is valid and updated,
*          ERROR - if MAC address reported by HW is not valid.
*/

LOCAL STATUS tsi108GetMacAddress
    (
    TSI108_DRV_CTRL * pDrvCtrl
    )
    {
    UINT32 word1, word2;
    UCHAR enetAddr[6];


    word1 = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_MAC_ADDR1);
    word2 = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_MAC_ADDR2);

    if (word1 != 0 && word2 != 0)
	{
	enetAddr[0] = (word2 >> 16) & 0xFF;
	enetAddr[1] = (word2 >> 24) & 0xFF;

	enetAddr[2] = word1 & 0xFF;
	enetAddr[3] = (word1 >> 8) & 0xFF;
	enetAddr[4] = (word1 >> 16) & 0xFF;
	enetAddr[5] = (word1 >> 24) & 0xFF;

	memcpy(pDrvCtrl->enetAddr, enetAddr, 6);

	return OK;
	}
    else
	return ERROR;
    }


/*******************************************************************************
*
* tsi108SetMacAddress - sets the MAC address
*
* This routine sets the MAC address of the GigE port.
*
* RETURNS: none
*/

LOCAL STATUS tsi108SetMacAddress
    (
    TSI108_DRV_CTRL * pDrvCtrl,
    void *addr
    )
    {
    UINT32 word1, word2;

    word2 = (*(char *)addr << 16) |
	(*((char *)addr + 1) << 24);

    word1 = (*((char *)addr + 2)) |
	(*((char *)addr + 3) << 8) |
	(*((char *)addr + 4) << 16) |
	(*((char *)addr + 5) << 24);

    semTake(pDrvCtrl->miscLock, WAIT_FOREVER);

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_MAC_ADDR1, word1);

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_MAC_ADDR2, word2);

    memcpy(pDrvCtrl->enetAddr, addr, 6);

    semGive(pDrvCtrl->miscLock);

    TSI108_LOG(TSI108_DEBUG_LOAD, ("tsi108SetMacAddress:%d: 0x%08x 0x%08x\n"),
				   pDrvCtrl->unit,word1,word2,4,5,6);
	
    return OK;
    }


/*******************************************************************************
*
* tsi108InitPhy - initialize the PHY interface
*
* This routine initialize the PHY and configure it into proper mode.
*
* RETURNS: none
*/

LOCAL void tsi108InitPhy
    (
    TSI108_DRV_CTRL * pDrvCtrl
    )
    {
    UINT32 i = 0;
    UINT16 phyVal = 0;
    UINT32 tmp = 0;

    semTake(pDrvCtrl->phyLock, WAIT_FOREVER);

    tmp = (TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_MAC_IFCTRL) |
	   TSI108_MAC_IFCTRL_PHYMODE);

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_MAC_IFCTRL, tmp);
    
    tsi108WriteMii(pDrvCtrl, PHY_CTRL, PHY_CTRL_RESET);

    while (tsi108ReadMii(pDrvCtrl, PHY_CTRL, NULL) & PHY_CTRL_RESET);

#ifdef TSI108_PHY_BCM54XX
    tsi108WriteMii(pDrvCtrl, 0x09, 0x0300);
    tsi108WriteMii(pDrvCtrl, 0x10, 0x1020);
    tsi108WriteMii(pDrvCtrl, 0x1c, 0x8c00);
#endif

    tsi108WriteMii(pDrvCtrl,
		    PHY_CTRL, PHY_CTRL_AUTONEG_EN | PHY_CTRL_AUTONEG_START);

    while (tsi108ReadMii(pDrvCtrl, PHY_CTRL, NULL) & PHY_CTRL_AUTONEG_START);

    /* Set G/MII mode and receive clock select in TBI control #2.  The
     * second port won't work if this isn't done, even though we don't
     * use TBI mode. */

    tsi108WriteTbi(pDrvCtrl, 0x11, 0x30);

    while (!((phyVal = tsi108ReadMii(pDrvCtrl, PHY_STAT, NULL)) & PHY_STAT_LINKUP))
	{
	/* Poll every 10 mSec until specified delay time expired */
	sysMsDelay(10);
	if (i++ > (MAX_MII_DELAY/10))
	    break;
	}

    TSI108_LOG(TSI108_DEBUG_LOAD,
		("tsi108InitPhy:%d:PHY_STAT reg contains 0x%08x\n"),
		pDrvCtrl->unit,phyVal,3,4,5,6);

    pDrvCtrl->phy_ok = 1; 
    pDrvCtrl->link_up = 0;

    semGive(pDrvCtrl->phyLock);
    }

/*******************************************************************************
*
* tsi108KillPhy - put the PHY interface down
*
* This routine powerdowns the PHY interface.
*
* RETURNS: none
*/

LOCAL void tsi108KillPhy
    (
    TSI108_DRV_CTRL * pDrvCtrl
    )
    {
    semTake(pDrvCtrl->phyLock, WAIT_FOREVER);

    tsi108WriteMii(pDrvCtrl, PHY_CTRL, PHY_CTRL_POWERDOWN);
    pDrvCtrl->phy_ok = 0;

    semGive(pDrvCtrl->phyLock);
    }

/*******************************************************************************
*
* tsi108EndMemInit - allocate and initialize memory for driver
*
* This routine allocates and initializes memory for descriptors and 
* corresponding buffers, and sets up the receive data pool for receiving 
* packets. 
*
* RETURNS: OK or ERROR.
*/

LOCAL STATUS tsi108EndMemInit
    (
    TSI108_DRV_CTRL * pDrvCtrl
    )
    {
    int i;
    M_BLK_ID pTmpMblk;
  
    if ((pDrvCtrl->end.pNetPool = malloc (sizeof(NET_POOL))) == NULL)
	return ERROR;
  
    bzero ((char *)&tsi108MClBlkConfig, sizeof(tsi108MClBlkConfig));
    bzero ((char *)&tsi108ClDesc, sizeof(tsi108ClDesc));

    /*
     * Initialize Cluster Pool Configuration Table
     */
    tsi108ClDesc[0].clSize = TSI108_RXBUF_SIZE - sizeof(CL_POOL_ID);
    tsi108ClDesc[0].clNum = TSI108_RXRING_LEN * DEFAULT_LOAN_RXBUF_FACTOR;

    /*
     * Calculate memory size required for all clusters:
     *
     * MemSize = [# of clusters] * ( [Cluster size] + [PoolID overhead] )
     *
     * NOTE: Alignment reserve (64 bytes) is added during allocation to ensure
     * that we have enough space to realign clusters to meet HW requirement
     * of 8-byte aligned data buffers. 
     */

    tsi108ClDesc[0].memSize = tsi108ClDesc[0].clNum * (tsi108ClDesc[0].clSize +
				sizeof(CL_POOL_ID));

    /* Allocate the memory for the clusters from cache safe memory. */
    /* cacheDmaMalloc memory is assumed to be cache line aligned ! */

    tsi108ClDesc[0].memArea =(char *)cacheDmaMalloc(tsi108ClDesc[0].memSize +
						    (_CACHE_ALIGN_SIZE*2));
    if (tsi108ClDesc[0].memArea == NULL)
	{
	TSI108_LOG(TSI108_DEBUG_ERROR,
	    ("tsi108EndInitMem: buffer memory unavailable\n"),
		1,2,3,4,5,6);

	return ERROR;
	}

    /* Save pointer to the allocated space */
    pDrvCtrl->pRxBufBase = tsi108ClDesc[0].memArea;

    TSI108_CACHE_INVALIDATE(pDrvCtrl->pRxBufBase,
			    (tsi108ClDesc[0].memSize + (_CACHE_ALIGN_SIZE*2)));

    /*
     * Adjust location of the cluster pool to ensure that beginning of
     * data buffers is 8-byte aligned (after 4-byte PoolID)
     */
    tsi108ClDesc[0].memArea += (32 - 4);

    /*
     * Initialize mbuf Configuration Table
     */

    tsi108MClBlkConfig.mBlkNum = tsi108ClDesc[0].clNum;
    tsi108MClBlkConfig.clBlkNum = tsi108ClDesc[0].clNum;

    /* calculate the total memory for all the M-Blks and CL-Blks. */

    tsi108MClBlkConfig.memSize =
	((tsi108MClBlkConfig.mBlkNum * (M_BLK_SZ + sizeof(long))) +
		(tsi108MClBlkConfig.clBlkNum * (CL_BLK_SZ + sizeof(long))));

    if ((tsi108MClBlkConfig.memArea = (char *) memalign (sizeof(long), 
						tsi108MClBlkConfig.memSize))
	== NULL)
	{
	TSI108_LOG(TSI108_DEBUG_ERROR,
	    ("tsi108EndInitMem: MclBlkConfig memory unavailable\n"),
		1,2,3,4,5,6);

	return ERROR;
	}

    pDrvCtrl->clSz = tsi108ClDesc[0].clSize;
    pDrvCtrl->pMclkArea = tsi108MClBlkConfig.memArea;

    memset((void *)pDrvCtrl->pRxBufBase, 0, tsi108ClDesc[0].memSize);


    /* initialize the memory pool. */

    if (netPoolInit(pDrvCtrl->end.pNetPool, &tsi108MClBlkConfig,
		    &tsi108ClDesc[0], 1, NULL) == ERROR)
	{
	TSI108_LOG(TSI108_DEBUG_ERROR, ("netPoolInit failed!\n"),1,2,3,4,5,6);
	return ERROR;
	}

    if ((pDrvCtrl->pClPoolId = netClPoolIdGet (pDrvCtrl->end.pNetPool,
				    tsi108ClDesc[0].clSize, FALSE)) == NULL)
	{
	TSI108_LOG(TSI108_DEBUG_ERROR,
		("Unable get ID for specified pool\n"),1,2,3,4,5,6);
	return ERROR;
	}

    pDrvCtrl->rxRing = cacheDmaMalloc(TSI108_RXRING_LEN * sizeof(RX_DESC));

    if (pDrvCtrl->rxRing == NULL)
	{
	TSI108_LOG(TSI108_DEBUG_ERROR,
		("tsi108EndMemInit:%d: Unable allocate memory\n"),
		    pDrvCtrl->portNum,2,3,4,5,6);
	return ERROR;
	}

    memset((void *)pDrvCtrl->rxRing, 0, (TSI108_RXRING_LEN * sizeof(RX_DESC)));

    pDrvCtrl->txRing = cacheDmaMalloc(TSI108_TXRING_LEN * sizeof(TX_DESC));

    if (!pDrvCtrl->txRing)
	{
	TSI108_LOG(TSI108_DEBUG_ERROR,
		    ("tsi108EndMemInit:%d: Unable allocate memory \n"),
		    pDrvCtrl->portNum,2,3,4,5,6);

	return ERROR;
    }

    memset ((void *)pDrvCtrl->txRing, 0, (TSI108_TXRING_LEN * sizeof(TX_DESC)));

    TSI108_LOG(TSI108_DEBUG_LOAD, ("rxRing: 0x%08x  txRing: 0x%08x\n"),
	    (UINT32)(pDrvCtrl->rxRing),(UINT32)(pDrvCtrl->txRing),3,4,5,6);

    pDrvCtrl->rxPhys = (UINT32)TSI108_VIRT_TO_PHYS((char *)pDrvCtrl->rxRing);

    pDrvCtrl->txPhys = (UINT32)TSI108_VIRT_TO_PHYS((char *)pDrvCtrl->txRing);

    TSI108_LOG(TSI108_DEBUG_LOAD, ("rxPhys: 0x%08x  txPhys: 0x%08x\n"),
	    pDrvCtrl->rxPhys,pDrvCtrl->txPhys,3,4,5,6);

    /*-----------------------------------------------------------------------
     * Initialize Rx Buffer Descriptors Ring
     */
    for (i = 0; i < TSI108_RXRING_LEN; i++)
	{
	pDrvCtrl->rxRing[i].next0 =
				pDrvCtrl->rxPhys + (i + 1) * sizeof(RX_DESC);
	pDrvCtrl->rxRing[i].blen = TSI108_RXBUF_SIZE;
	pDrvCtrl->rxRing[i].vlan = 0;
	}

    /* Wrap Rx BD Ring */	
    pDrvCtrl->rxRing[TSI108_RXRING_LEN - 1].next0 = pDrvCtrl->rxPhys;

    pDrvCtrl->rxTail = 0;
    pDrvCtrl->rxHead = 0;


    /*
     * Get clusters to store received packets into ahead of time.
     * Here we take clusters out of the pool and attach them to the device.
     * The device will fill them with incoming packets and trigger an
     * interrupt. This will schedule the receive handling routine which will
     * unlink the cluster and send it to the stack. When the stack is done
     * with the data, the cluster is freed and returned to the cluster pool
     * to be used by the device again.
     */

    for (i = 0; i < TSI108_RXRING_LEN; i++)
	{
        pTmpMblk = netTupleGet (pDrvCtrl->end.pNetPool,
                                TSI108_RXBUF_SIZE - sizeof(CL_POOL_ID),
                                M_DONTWAIT, MT_DATA, 0);

	if (pTmpMblk == NULL)
	    {
	    /* Bah.  No memory for now, but maybe we'll get some more later.
	     * For now, we'll live with the smaller ring. */
	    TSI108_LOG(TSI108_DEBUG_LOAD,
		("tsi108EndMemInit:%d: allocate only %d receive buffer(s)\n"),
		    pDrvCtrl->portNum,i,3,4,5,6);

	    pDrvCtrl->rxHead = i;
	    break;
	    }

	pDrvCtrl->rxMblks[i] = pTmpMblk;
	pDrvCtrl->rxRing[i].buf0 =
            (UINT32)TSI108_VIRT_TO_PHYS(pTmpMblk->m_data);
	pDrvCtrl->rxRing[i].misc = TSI108_RX_OWN | TSI108_RX_INT;
	}

    TSI108_CACHE_FLUSH(pDrvCtrl->rxRing, TSI108_RXRING_LEN * sizeof(RX_DESC));

    pDrvCtrl->rxFree = i;

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_RXQ_PTRLOW, pDrvCtrl->rxPhys);

    /*-----------------------------------------------------------------------
     * Initialize Tx Buffer Descriptors Ring
     */

    for (i = 0; i < TSI108_TXRING_LEN; i++)
	{
	pDrvCtrl->txRing[i].next0 =
				pDrvCtrl->txPhys + (i + 1) * sizeof(TX_DESC);
	pDrvCtrl->txRing[i].misc = 0;
	}
	
    /* Wrap Tx BD Ring*/	
    pDrvCtrl->txRing[TSI108_TXRING_LEN - 1].next0 = pDrvCtrl->txPhys;

    TSI108_CACHE_FLUSH(pDrvCtrl->txRing, TSI108_TXRING_LEN * sizeof(TX_DESC));

    pDrvCtrl->txTail = 0;
    pDrvCtrl->txHead = 0;
    pDrvCtrl->txFree = TSI108_TXRING_LEN;

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_TXQ_PTRLOW, pDrvCtrl->txPhys);

    return OK;
    }

/*******************************************************************************
*
* tsi108EndFreeAllMem - free memory for driver
*
* This routine frees memory allocated for descriptors and 
* corresponding buffers, and the receive data pool for receiving 
* packets. 
*
* RETURNS: OK or ERROR.
*/

VOID tsi108EndFreeAllMem
    (
    TSI108_DRV_CTRL * pDrvCtrl
    )
    {
    if (pDrvCtrl == NULL)
	return;

    if (pDrvCtrl->rxRing != NULL)
	{
        cacheDmaFree ((void *)pDrvCtrl->rxRing);
        pDrvCtrl->rxRing = NULL;
	}

    if (pDrvCtrl->txRing != NULL)
	{
        cacheDmaFree ((void *)pDrvCtrl->txRing);
        pDrvCtrl->txRing = NULL;
	}

    if (pDrvCtrl->pRxBufBase != NULL)
	{
        cacheDmaFree (pDrvCtrl->pRxBufBase);
        pDrvCtrl->pRxBufBase = NULL;
	}

    if (pDrvCtrl->pMclkArea != NULL)
	{
	free((char*)pDrvCtrl->pMclkArea);
	pDrvCtrl->pMclkArea = NULL;
	}

    if (pDrvCtrl->end.pNetPool != NULL)
	{
	if (netPoolDelete(pDrvCtrl->end.pNetPool) != OK)
	    {
	    printf("netPoolDelete fails\n");
	    }

	free ((char *)pDrvCtrl->end.pNetPool);
	pDrvCtrl->end.pNetPool = NULL;
	}

    /* Free MIB-II entries */
    endM2Free(&pDrvCtrl->end);

    }


/*******************************************************************************
*
* tsi108EndStart - start the tsi108 END device
*
* This function calls BSP functions to connect interrupts and start the
* device running in interrupt mode.
*
* RETURNS: OK or ERROR
*
*/

LOCAL STATUS tsi108EndStart
    (
    TSI108_DRV_CTRL * pDrvCtrl
    )
    {
    int retVal;

    TSI108_LOG(TSI108_DEBUG_LOAD,
		("Enter tsi108EndStart()\n"),1,2,3,4,5,6);

    retVal=intConnect(INUM_TO_IVEC(pDrvCtrl->irqNum),
				tsi108Interrupt, (int)pDrvCtrl);
	
    if (retVal != OK)
	{
	TSI108_LOG(TSI108_DEBUG_LOAD,
	    ("tsi108_eth%d: Could not allocate IRQ%d.\n"),
	    pDrvCtrl->portNum,(INT_VEC_GIGE0 + pDrvCtrl->portNum),3,4,5,6);

	return retVal;
	}

    TSI108_LOG(TSI108_DEBUG_LOAD,
	("tsi108EndStart:%d: Assigned IRQ %d\n"),
	    pDrvCtrl->portNum,pDrvCtrl->irqNum,3,4,5,6);

    /* Enable GigE interrupt here for now */
    sysIntEnable(pDrvCtrl->irqNum);

    tsi108InitPhy(pDrvCtrl);
    tsi108CheckPhy(pDrvCtrl);

    END_FLAGS_SET (&pDrvCtrl->end, IFF_UP | IFF_RUNNING);

    /* clear all interrupts */
    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_INTSTAT, ~0);
    
    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_INTMASK,
			~(TSI108_INT_TXQUEUE0 | TSI108_INT_RXERROR |
			  TSI108_INT_RXTHRESH | TSI108_INT_RXQUEUE0 |
			  TSI108_INT_RXOVERRUN | TSI108_INT_RXWAIT |
			  TSI108_INT_SFN | TSI108_INT_STATCARRY));
    
    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_MAC_CFG1,
		    TSI108_MAC_CFG1_RXEN | TSI108_MAC_CFG1_TXEN);

    tsi108RestartRx(pDrvCtrl);

    return 0;
    }

/*******************************************************************************
*
* tsi108EndStop - stop the tsi108 END device
*
* This function calls BSP functions to disconnect interrupts and stop
* the device from operating in interrupt mode.
*
* RETURNS: OK or ERROR.
*/

LOCAL STATUS tsi108EndStop
    (
    TSI108_DRV_CTRL * pDrvCtrl
    )
    {
    STATUS result = OK;

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_MAC_CFG1, TSI108_MAC_CFG1_SOFTRST);

    SYNC;

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_MAC_CFG1, 0);

    SYNC;

    /* mark interface down */
    END_FLAGS_CLR (&pDrvCtrl->end, (IFF_UP | IFF_RUNNING));    

    return result;
    }

/*******************************************************************************
*
* tsi108EndUnload - unload the tsi108 END driver from the system
*
* This function first brings down the device, and then frees any
* stuff that was allocated by the driver in the load function.
*
* RETURNS: OK or ERROR.
*/

LOCAL STATUS tsi108EndUnload
    (
    TSI108_DRV_CTRL * pDrvCtrl
    )
    {
    /* int ret = 0; */

    tsi108EndStop(pDrvCtrl);
    tsi108StopEthernet(pDrvCtrl);
    tsi108KillPhy(pDrvCtrl);

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_INTMASK, ~0);

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_MAC_CFG1, 0);

    /* Discard the RX ring. */

    /* Freeing all allocated memory has to be done last. */
    tsi108EndFreeAllMem(pDrvCtrl);

    return 0;
    }


/*******************************************************************************
*
* tsi108InitMac - initialize MAC
*
* This routine initialize/enable the MAC, stats, and configure the rx and
* tx paths.
*
* RETURNS: OK or ERROR.
*/

LOCAL void tsi108InitMac
    (
    TSI108_DRV_CTRL * pDrvCtrl
    )
    {
    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_MAC_CFG2,
		(TSI108_MAC_CFG2_DFLT_PREAMBLE | TSI108_MAC_CFG2_PADCRC));

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_TXTHRESH,
		((192 << TSI108_EC_TXTHRESH_STARTFILL) |
		(192 << TSI108_EC_TXTHRESH_STOPFILL)));

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_STAT_CARRYMASK1,
		~(TSI108_STAT_CARRY1_RXBYTES | TSI108_STAT_CARRY1_RXPKTS |
		TSI108_STAT_CARRY1_RXMCAST | TSI108_STAT_CARRY1_RXBCAST));

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_STAT_CARRYMASK2,
		~(TSI108_STAT_CARRY2_TXBYTES | TSI108_STAT_CARRY2_TXPKTS |
		TSI108_STAT_CARRY2_TXBCAST | TSI108_STAT_CARRY2_TXMCAST));

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_PORTCTRL,
		TSI108_EC_PORTCTRL_STATZOR | TSI108_EC_PORTCTRL_STATEN);

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_MAC_CFG1, 0);

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_RXCFG,
		TSI108_EC_RXCFG_SE | TSI108_EC_RXCFG_BFE);

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_TXQ_CFG,
		(TSI108_EC_TXQ_CFG_DESC_INT | TSI108_EC_TXQ_CFG_EOQ_OWN_INT |
		TSI108_GE_TX_BD_SWAP |
		(TSI108_GE_DEST_PORT << TSI108_EC_TXQ_CFG_SFNPORT)));

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_RXQ_CFG,
		(TSI108_EC_RXQ_CFG_DESC_INT | TSI108_EC_RXQ_CFG_EOQ_OWN_INT |
		TSI108_GE_RX_BD_SWAP |
		(TSI108_GE_DEST_PORT << TSI108_EC_RXQ_CFG_SFNPORT)));

    /*
     * The original TX buffer burst size specified by Tundra
     * was 256 bytes, however it was observed that this sometimes
     * caused transmissions in 1000Mbps mode to fail silently
     * in some scenarios, possibly due to data corruption. The burst
     * size was changed to 8 to mitigate this.
     */

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_TXQ_BUFCFG,
		(TSI108_EC_TXQ_BUFCFG_BURST256 | TSI108_GE_TX_DATA_SWAP |
		(TSI108_GE_DEST_PORT << TSI108_EC_TXQ_BUFCFG_SFNPORT)));

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_RXQ_BUFCFG,
		(TSI108_EC_RXQ_BUFCFG_BURST256 | TSI108_GE_RX_DATA_SWAP |
		(TSI108_GE_DEST_PORT << TSI108_EC_RXQ_BUFCFG_SFNPORT)));

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_INTMASK, ~0);
    }


/*******************************************************************************
*
* tsi108PollStart - start polled mode operations
*
* RETURNS: OK or ERROR.
*/

LOCAL STATUS tsi108PollStart
    (
    TSI108_DRV_CTRL * pDrvCtrl	/* device to be polled */
    )
    {
    int	    oldLevel;

    if (pDrvCtrl->flags & TSI108_POLLING)
	return (ERROR);

    END_TX_SEM_TAKE(&pDrvCtrl->end, WAIT_FOREVER);

    oldLevel = intLock ();

    /* Mask all interrupts from GIGE port */

    pDrvCtrl->intMask = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_EC_INTMASK);

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_INTMASK, ~0);

    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_INTSTAT, ~0);

    pDrvCtrl->flags |= TSI108_POLLING;

    intUnlock (oldLevel);

    END_TX_SEM_GIVE (&pDrvCtrl->end);

    TSI108_LOG(TSI108_DEBUG_POLL,("Polled mode started\n"),1,2,3,4,5,6);

    return (OK);
    }


/*******************************************************************************
*
* tsi108PollStop - stop polled mode operations
*
* This function terminates polled mode operation.  The device returns to
* interrupt mode.
*
* The device interrupts are enabled, the current mode flag is switched
* to indicate interrupt mode and the device is then reconfigured for
* interrupt operation.
*
* RETURNS: OK or ERROR.
*/

LOCAL STATUS tsi108PollStop
    (
    TSI108_DRV_CTRL * pDrvCtrl	/* device to be polled */
    )
    {
    int         oldLevel;

    if (!(pDrvCtrl->flags & TSI108_POLLING))
	return (ERROR);

    oldLevel = intLock ();

    /* Restore interrupt mask */
    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_INTMASK, pDrvCtrl->intMask);

    /* Clear all pending interrupt requests */
    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_INTSTAT, ~0);

    pDrvCtrl->flags &= ~TSI108_POLLING;

    intUnlock (oldLevel);

    TSI108_LOG(TSI108_DEBUG_POLL,("Polled mode stopped\n"),1,2,3,4,5,6);

    return (OK);
    }


/*******************************************************************************
*
* tsi108EndIoctl - the driver I/O control routine
*
* Process an ioctl request.
*
* RETURNS: A command specific response, usually OK or ERROR.
*/
LOCAL int tsi108EndIoctl
    (
    TSI108_DRV_CTRL * pDrvCtrl,	/* device receiving command */
    int cmd,			/* ioctl command code */
    caddr_t data		/* command argument */
    )
    {
    int error = 0;
    long value;
    UINT32 rxcfg;

    switch ((unsigned)cmd)
	{
	case EIOCSADDR:	    /* set MAC address */
	    if (data == NULL)
		error = EINVAL;
	    else
		{
		bcopy((char *)data, (char *)END_HADDR(&pDrvCtrl->end),
			    END_HADDR_LEN(&pDrvCtrl->end));

		tsi108SetMacAddress(pDrvCtrl, (char *)data);

		TSI108_LOG(TSI108_DEBUG_IOCTL,
			"Ioctl EIOCSADDR %02x%02x%02x%02x%02x%02x\n",
			    data[0],data[1],data[2],data[3],data[4],data[5]);
		}

	    break;

	case EIOCGADDR:	    /* get MAC address */
	    if (data == NULL)
		error = EINVAL;
	    else
		{
		bcopy((char *)END_HADDR(&pDrvCtrl->end), (char *)data,
				END_HADDR_LEN(&pDrvCtrl->end));

		TSI108_LOG(TSI108_DEBUG_IOCTL,
			"Ioctl EIOCGADDR %02x%02x%02x%02x%02x%02x\n",
			    data[0],data[1],data[2],data[3],data[4],data[5]);
		}

	    break;

	case EIOCSFLAGS:	/* set (or clear) flags */
	    TSI108_LOG(TSI108_DEBUG_IOCTL,("Ioctl EIOCSFLAGS\n"),1,2,3,4,5,6);
	    value = (long)data;
	    if (value < 0)
		{
		value = -value;
		value--;
		END_FLAGS_CLR (&pDrvCtrl->end, value);
		}
	    else
		{
		END_FLAGS_SET (&pDrvCtrl->end, value);
		}

	    /* Set or clear promisc. mode (IFF_PROMISC flag) */

	    if (END_FLAGS_ISSET (IFF_PROMISC))
		{
		if (!(pDrvCtrl->flags & TSI108_PROMISCUOUS))
		    {
		    rxcfg = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_EC_RXCFG);
		    rxcfg |= TSI108_EC_RXCFG_UFE;
		    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_RXCFG, rxcfg);
		    }
		}
	    else
		{
		if(pDrvCtrl->flags & TSI108_PROMISCUOUS)
		    {
		    rxcfg = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_EC_RXCFG);
		    rxcfg &= ~TSI108_EC_RXCFG_UFE;
		    TSI108_GIGE_REG_WR(pDrvCtrl, TSI108_EC_RXCFG, rxcfg);
		    }
		}

	    break;

	case EIOCGFLAGS:	/* get flags */
	    TSI108_LOG(TSI108_DEBUG_IOCTL,
			    ("Ioctl EIOCGFLAGS\n"),1,2,3,4,5,6);
	    if (data == NULL)
		error = EINVAL;
	    else
		*(int*)data = END_FLAGS_GET(&pDrvCtrl->end);
	    break;

	case EIOCPOLLSTART:	/* Begin polled operation */
	    tsi108PollStart(pDrvCtrl);
	    break;

	case EIOCPOLLSTOP:	/* End polled operation */
	    tsi108PollStop (pDrvCtrl);
	    break;

	case EIOCGMIB2233:
        case EIOCGMIB2:
	    error = endM2Ioctl (&pDrvCtrl->end, cmd, data);
	    break;

	case EIOCGPOLLCONF:
	    if ((data == NULL))
		error = EINVAL;
	    else
		*((END_IFDRVCONF **)data) = &pDrvCtrl->endStatsConf;

	    break;

        case EIOCGPOLLSTATS:
	    if ((data == NULL))
		error = EINVAL;
	    else
		{
		error = tsi108EndStatsDump(pDrvCtrl);
		if (error == OK)
		*((END_IFCOUNTERS **)data) = &pDrvCtrl->endStatsCounters;
		}

	    break;

        case EIOCGFBUF:	    /* return minimum First Buffer for chaining */
	    TSI108_LOG(TSI108_DEBUG_IOCTL,("Ioctl EIOCGFBUF\n"),1,2,3,4,5,6);
	    if (data == NULL)
		return (EINVAL);
	    *(int *)data = TSI108_MIN_FBUF;
	    break;

	case EIOCGHDRLEN:
	    TSI108_LOG(TSI108_DEBUG_IOCTL,("Ioctl EIOCGHDRLEN\n"),1,2,3,4,5,6);
	    if (data == NULL)
		return (EINVAL);
	    *(int *)data = EH_SIZE;
	    break;

	case EIOCMULTIADD:
	    TSI108_LOG(TSI108_DEBUG_IOCTL,
			    ("Ioctl EIOCMULTIADD\n"),1,2,3,4,5,6);
	    if (data == NULL)
		error = EINVAL;
	    else
		error = tsi108EndMCastAdd(pDrvCtrl, (char *) data);
	    break;

	case EIOCMULTIGET:
	    TSI108_LOG(TSI108_DEBUG_IOCTL,
			    ("Ioctl EIOCMULTIGET\n"),1,2,3,4,5,6);
	    if (data == NULL)
		error = EINVAL;
	    else
		error = tsi108EndMCastGet(pDrvCtrl, (MULTI_TABLE *) data);
	    break;

	case EIOCMULTIDEL:
	    TSI108_LOG(TSI108_DEBUG_IOCTL,
			    ("Ioctl EIOCMULTIDEL\n"),1,2,3,4,5,6);
	    if (data == NULL)
		error = EINVAL;
	    else
		error = tsi108EndMCastDel(pDrvCtrl, (char *) data);
	    break;

	default:    /* unknown request */
	    TSI108_LOG(TSI108_DEBUG_IOCTL,
			    ("Ioctl unknown 0x%x\n"),cmd,2,3,4,5,6);
	    error = EINVAL;
	}

    return (error);
    }


/*******************************************************************************
*
* tsi108EndStatsDump - Dump statistic registers for MIB2 (RFC2233)
*
* This routine dumps statistic registers for MIB2 update
*
* RETURNS: OK
*/

LOCAL STATUS tsi108EndStatsDump
    (
    TSI108_DRV_CTRL*  pDrvCtrl    /* device driver control structure */
    )
    {
    END_IFCOUNTERS *    pEndStatsCounters;
    UINT32 val;

    pEndStatsCounters = &pDrvCtrl->endStatsCounters;

    /* 
     * All HW counters are zeroed on read. For each counter we also have 
     * to reset corresponding overflow counters.
     */

    /* Get number of RX'ed octets */
    val = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_STAT_RXBYTES);
    pEndStatsCounters->ifInOctets =
	(pDrvCtrl->carryRxBytes * TSI108_STAT_RXBYTES_CARRY) + val;
    pDrvCtrl->carryRxBytes = 0;

    /* Get number of TX'ed octets */
    val = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_STAT_TXBYTES);
    pEndStatsCounters->ifOutOctets =
	(pDrvCtrl->carryTxBytes * TSI108_STAT_TXBYTES_CARRY) + val; 
    pDrvCtrl->carryTxBytes = 0;

    /*
     * Get RX'ed unicasts, broadcasts, multicasts
     */
    val = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_STAT_RXPKTS);
    pEndStatsCounters->ifInUcastPkts =
	(pDrvCtrl->carryRxPkt * TSI108_STAT_RXPKTS_CARRY) + val;
    pDrvCtrl->carryRxPkt = 0;

    val = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_STAT_RXBCAST);
    pEndStatsCounters->ifInBroadcastPkts =
	(pDrvCtrl->carryRxBcastPkt * TSI108_STAT_RXBCAST_CARRY) + val;
    pDrvCtrl->carryRxBcastPkt = 0;

    val = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_STAT_RXMCAST);
    pEndStatsCounters->ifInMulticastPkts =
	(pDrvCtrl->carryRxMcastPkt * TSI108_STAT_RXMCAST_CARRY) + val;
    pDrvCtrl->carryRxMcastPkt = 0;

    pEndStatsCounters->ifInUcastPkts -=
		(pEndStatsCounters->ifInMulticastPkts +
			pEndStatsCounters->ifInBroadcastPkts);

    /*
     * Get TX'ed unicasts, broadcasts, multicasts
     */
    val = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_STAT_TXPKTS);
    pEndStatsCounters->ifOutUcastPkts =
	(pDrvCtrl->carryTxPkt * TSI108_STAT_TXPKTS_CARRY) + val;
    pDrvCtrl->carryTxPkt = 0;

    val = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_STAT_TXBCAST);
    pEndStatsCounters->ifOutBroadcastPkts =
	(pDrvCtrl->carryTxBcastPkt * TSI108_STAT_TXBCAST_CARRY) + val;
    pDrvCtrl->carryTxBcastPkt = 0;

    val = TSI108_GIGE_REG_RD(pDrvCtrl, TSI108_STAT_TXMCAST);
    pEndStatsCounters->ifOutMulticastPkts =
	(pDrvCtrl->carryTxMcastPkt * TSI108_STAT_TXMCAST_CARRY) + val;
    pDrvCtrl->carryTxMcastPkt = 0;

    pEndStatsCounters->ifOutUcastPkts -=
		(pEndStatsCounters->ifOutMulticastPkts +
			pEndStatsCounters->ifOutBroadcastPkts);

    return (OK);
    }


/*******************************************************************************
*
* tsi108EndParse - parse the init string
*
* Parse the input string.  Fill in values in the driver control structure.
*
* The muxLib.o module automatically prepends the unit number to the user's
* initialization string from the BSP (configNet.h).
*
* The initialization string format is:
* "<portNum>:<regs>:<phyRegs>:<phy>:<irqNum>"
*
* .IP <unit>
* Device unit number, a small integer.
* .IP <vecNum>
* Interrupt vector number
* .IP <intLvl>
* Interrupt level
* .LP
*
* RETURNS: OK or ERROR for invalid arguments.
*/

LOCAL STATUS tsi108EndParse
    (
    TSI108_DRV_CTRL * pDrvCtrl,	/* device pointer */
    char * initString		/* information string */
    )
    {
    char*	tok;
    char*	pHolder = NULL;

    /* Parse the initString */

    /* Unit number. (from muxLib.o) */

    tok = strtok_r (initString, ":", &pHolder);
    if (tok == NULL)
	return ERROR;
    pDrvCtrl->unit = atoi(tok);

    /* Port number. */
    tok = strtok_r (NULL, ":", &pHolder);
    if (tok == NULL)
	return ERROR;
    pDrvCtrl->portNum = atoi(tok);

    /* Register base address. */
    tok=strtok_r(NULL, ":", &pHolder);
    if (tok == NULL)
	return ERROR;
    pDrvCtrl->regs = (UINT32)strtoul(tok, NULL, 16);

    /* Phy register base address. */
    tok=strtok_r(NULL, ":", &pHolder);
    if (tok == NULL)
	return ERROR;
    pDrvCtrl->phyRegs = (UINT32)strtoul(tok, NULL, 16);

    /* Phy address. */
    tok=strtok_r(NULL, ":", &pHolder);
    if (tok == NULL)
	return ERROR;
    pDrvCtrl->phy = strtoul(tok, NULL, 16);

    /* Interrupt vector. Make sure irqNum is in decimal in initString. */
    tok = strtok_r (NULL, ":", &pHolder);
    if (tok == NULL)
	return ERROR;
    pDrvCtrl->irqNum = atoi (tok);

    TSI108_LOG(TSI108_DEBUG_LOAD,
	    ("tsi108End: Processed all arguments\n"),1,2,3,4,5,6);
    TSI108_LOG(TSI108_DEBUG_LOAD,
	    ("unit: %d, portNum: %d, regs: %08x, phyReg: %08x, phy: %d," \
	    " irqNum: %d\n"),
	    pDrvCtrl->unit, pDrvCtrl->portNum, (UINT32)(pDrvCtrl->regs),
	    (UINT32)(pDrvCtrl->phyRegs), pDrvCtrl->phy, pDrvCtrl->irqNum);

    return OK;
    }


/*******************************************************************************
* tsi108EndLoad - initialize the driver and device
*
* This routine initializes the driver and the device to the operational state.
* All of the device specific parameters are passed in the initString.
*
* This routine can be called in two modes. If it is called with an empty, but
* allocated string then it places the name of this device (i.e. dc) into the
* initString and returns 0.
*
* If the string is allocated then the routine attempts to perform its load
* functionality.
*
* RETURNS: An END object pointer or NULL on error or 0 and the name of the
* device if the initString was NULL.
*/

END_OBJ* tsi108EndLoad
    (
    char * initString
    )
    {
    TSI108_DRV_CTRL * pDrvCtrl;

    TSI108_LOG(TSI108_DEBUG_LOAD,
		("Loading tsi108End Driver...\n"),1,2,3,4,5,6);

    if (initString == NULL)
	{
	TSI108_LOG(TSI108_DEBUG_LOAD,
		("tsi108End: NULL initSting\n"),1,2,3,4,5,6);
	return NULL;
	}

    if (initString[0] == 0)
	{
	bcopy ((char *) DEVICE_NAME, (void *)initString, DEVICE_NAME_LENGTH);
	return NULL;
	}

    pDrvCtrl = (TSI108_DRV_CTRL *)calloc(sizeof(TSI108_DRV_CTRL), 1);

    if (pDrvCtrl == NULL)
	{
	TSI108_LOG(TSI108_DEBUG_ERROR,
	    ("tsi108End: Failed to allocate control structure for port\n"),
	       1,2,3,4,5,6);
	goto errorExit;
	}

    if (tsi108EndParse(pDrvCtrl, initString) == ERROR)
	{
	TSI108_LOG(TSI108_DEBUG_ERROR,
	    ("tsi108End: initString parse error\n"),1,2,3,4,5,6);
	goto errorExit;
	}

    if (pDrvCtrl->portNum >= TSI108_ETH_PORT_NUM)
	{
	TSI108_LOG(TSI108_DEBUG_ERROR,
	    ("tsi108End: Unsupported port number\n"),1,2,3,4,5,6);
	goto errorExit;
	}

    pDrvCtrl->phyLock = semMCreate(SEM_Q_PRIORITY|
        SEM_DELETE_SAFE|SEM_INVERSION_SAFE);
    pDrvCtrl->miscLock = semMCreate(SEM_Q_PRIORITY|
        SEM_DELETE_SAFE|SEM_INVERSION_SAFE);

    /*
     * IMPORTANT: The following initialization has to be done before
     *            tsi108EndMemInit() is called.
     */

    tsi108ResetPort(pDrvCtrl);
    tsi108KillPhy(pDrvCtrl);

    /* Memory allocation and setting up descriptors. */
	
    if (tsi108EndMemInit(pDrvCtrl) == ERROR)
	goto errorExit;

    tsi108Ends[pDrvCtrl->portNum] = pDrvCtrl;

    if (tsi108GetMacAddress(pDrvCtrl) == ERROR)
	{
	TSI108_LOG(TSI108_DEBUG_LOAD,
	    ("tsi108End: Using default MAC address\n"),1,2,3,4,5,6);
	tsi108SetMacAddress(pDrvCtrl, &(enetAddrTbl[pDrvCtrl->portNum][0]));
	}

    if (END_OBJ_INIT(&pDrvCtrl->end, (DEV_OBJ *)pDrvCtrl, DEVICE_NAME,
	    pDrvCtrl->unit, &tsi108EndFuncTable, "tsi108End Driver") == ERROR)
	goto errorExit;

    tsi108InitMac(pDrvCtrl);

    TSI108_LOG(TSI108_DEBUG_LOAD,
	    ("tsi108end MAC: %02x:%02x:%02x:%02x:%02x:%02x\n"),
	    pDrvCtrl->enetAddr[0],pDrvCtrl->enetAddr[1],pDrvCtrl->enetAddr[2],
	    pDrvCtrl->enetAddr[3],pDrvCtrl->enetAddr[4],pDrvCtrl->enetAddr[5]);

    if (endM2Init(&pDrvCtrl->end, M2_ifType_ethernet_csmacd,
	(u_char *) &pDrvCtrl->enetAddr[0], 6, ETHERMTU, END_SPEED_1000M, 
	IFF_NOTRAILERS | IFF_MULTICAST | IFF_BROADCAST)
		== ERROR) 
	{
	TSI108_LOG(TSI108_DEBUG_ERROR,
		    ("tsi108EndLoad:%d: MIB-II initializations failed\n"),
			pDrvCtrl->unit,0,0,0,0,0);
	goto errorExit;
	}

    bzero ((char *)&pDrvCtrl->endStatsCounters, sizeof(END_IFCOUNTERS));

    pDrvCtrl->endStatsConf.ifPollInterval = sysClkRateGet();
    pDrvCtrl->endStatsConf.ifEndObj = &pDrvCtrl->end;
    pDrvCtrl->endStatsConf.ifWatchdog = NULL;
    pDrvCtrl->endStatsConf.ifValidCounters =
			(END_IFINUCASTPKTS_VALID |
			END_IFINMULTICASTPKTS_VALID |
			END_IFINBROADCASTPKTS_VALID |
			END_IFINOCTETS_VALID |
			END_IFOUTOCTETS_VALID	|
			END_IFOUTUCASTPKTS_VALID    |
			END_IFOUTMULTICASTPKTS_VALID |
			END_IFOUTBROADCASTPKTS_VALID);

    pDrvCtrl->carryRxBytes = 0;
    pDrvCtrl->carryRxPkt = 0;
    pDrvCtrl->carryRxBcastPkt = 0;
    pDrvCtrl->carryRxMcastPkt = 0;
    pDrvCtrl->carryTxBytes = 0;
    pDrvCtrl->carryTxPkt = 0;
    pDrvCtrl->carryTxBcastPkt = 0;
    pDrvCtrl->carryTxMcastPkt = 0;

    END_OBJ_READY(&pDrvCtrl->end,
	IFF_NOTRAILERS | IFF_BROADCAST | IFF_MULTICAST);

    TSI108_LOG(TSI108_DEBUG_LOAD, ("tsi108EndLoad: succeeded!\n"),1,2,3,4,5,6);

    return (&pDrvCtrl->end);

errorExit:

    if (pDrvCtrl != NULL)
        {
        /* Free all previously allocated memory */
        tsi108EndFreeAllMem(pDrvCtrl);
        if (pDrvCtrl->phyLock != NULL)
            semDelete (pDrvCtrl->phyLock);
        if (pDrvCtrl->miscLock != NULL)
            semDelete (pDrvCtrl->miscLock);
	free((char *)pDrvCtrl);
        }

    TSI108_LOG (TSI108_DEBUG_ERROR, ("tsi108EndLoad: failed!\n"),1,2,3,4,5,6);

    return NULL;
    }


#ifdef TSI108END_DBG_SHOW
/*******************************************************************************
* tsi108EndShow - initialize the driver and device
*
* This routine initializes the driver and the device to the operational state.
* All of the device specific parameters are passed in the initString.
*
* RETURNS: Always OK.
*/

STATUS tsi108EndShow (int devNum)
    {
    TSI108_DRV_CTRL * pDrvCtrl;

    if (devNum > 1)
	return ERROR;
	  
    if ((pDrvCtrl = tsi108Ends[devNum]) == NULL)
	return ERROR;

    printf("============ tsi108End Port%d ==============\n",devNum);    
    printf("\n");
    printf("TSI108_DRV_CTRL: 0x%08x\n", pDrvCtrl);
    printf("Device END_OBJ : 0x%08x\n", &pDrvCtrl->end);
    printf("Rx BD Ring Ptr : 0x%08x\n", pDrvCtrl->rxRing);
    printf("RX headptr     : 0x%08x\n", (UINT32)pDrvCtrl->rxRing +
                                  (32 * pDrvCtrl->rxHead));
    printf("RX tailptr     : 0x%08x\n", (UINT32)pDrvCtrl->rxRing +
                                  (32 * pDrvCtrl->rxTail));
    printf("      Rx Queue : tail=%d head=%d free=%d\n",
		    pDrvCtrl->rxTail, pDrvCtrl->rxHead, pDrvCtrl->rxFree);
    printf("    Rx Mblks   : 0x%08x\n", pDrvCtrl->rxMblks);
    printf("    Rx Buffers : 0x%08x\n", pDrvCtrl->rxBufs);
    printf("      Rx Count : %d\n", pDrvCtrl->rxCount);
    printf("Rx Refill Count: %d\n", pDrvCtrl->rxRefillCount);
    printf("       Rx mBlk : 0x%08x\n", pDrvCtrl->pMblk);
    printf("    Rx BufBase : 0x%08x\n", pDrvCtrl->pRxBufBase);
    printf("\n");
    printf("Tx BD Ring Ptr : 0x%08x\n", pDrvCtrl->txRing);
    printf("TX headptr     : 0x%08x\n", (UINT32)pDrvCtrl->txRing +
                                  (32 * pDrvCtrl->txHead));
    printf("TX tailptr     : 0x%08x\n", (UINT32)pDrvCtrl->txRing +
                                  (32 * pDrvCtrl->txTail));
    printf("      Tx Queue : tail=%d head=%d free=%d\n",
		    pDrvCtrl->txTail, pDrvCtrl->txHead, pDrvCtrl->txFree);
    printf("    Tx Buffers : 0x%08x\n", pDrvCtrl->txBufs);
    printf("    Tx Mblocks : 0x%08x\n", (UINT32)pDrvCtrl->txMblks);
    printf("\n");
    printf("     mBlk pool : 0x%08x\n", pDrvCtrl->pMclkArea);
    printf("  Cluster Size : 0x%08x\n", pDrvCtrl->clSz);
    printf("Cluster PoolID : 0x%08x\n", pDrvCtrl->pClPoolId);

    return OK;
    }
#endif /* TSI108END_DBG_SHOW */
