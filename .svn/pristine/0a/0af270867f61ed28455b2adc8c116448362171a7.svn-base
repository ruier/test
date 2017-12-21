/* simnetEnd.c -  simulator Enhanced Network Driver (END) */

/*
 * Copyright (c) 2003-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement. No license to Wind River intellectual property rights
 * is granted herein. All rights not licensed by Wind River are reserved
 * by Wind River.
 */

/*
modification history
--------------------
01z,06mar12,elp  fixed handling of netTupleGet() failure (CQ:WIND00331093),
		 added IFF_SIMPLEX (CQ:WIND00338583).
01y,04jan11,dbt  Removed build warnings (CQ:WIND00249312).
01x,02jun10,elp  64 bits support.
01w,14dec09,dbt  Added NAT support.
01v,29jan09,elp  fixed unload routine (CQ:WIND00157376).
01u,29aug08,kab  Change _WRS_VX_SMP to _WRS_CONFIG_SMP
01t,14aug08,elp  reinitialize buffer size to available buffer after each pkt.
01s,23may08,elp  fixed simsolaris alignment issue (CQ:WIND124232).
01r,29apr08,elp  fixed polling mode.
01q,11feb08,elp  opacified SIMNET_PKT type.
01p,11jun07,elp  made SMP safe.
01o,05jul07,elp  removed intLock() call.
01n,26oct06,tkf  Remove if_subr.h.
01m,25oct06,elp  include vxsimHostLibP.h rather than vxsimHostLib.h
01m,16oct06,dbt  Add padding for packets smaller than ethernet header size
		 (CQ WIND00069854).
01l,19aug05,elp  correctly disconnect interrupt.
01k,03mar05,elp  fixed alignment problem related to MTU size (SPR #106601).
01j,16nov04,jmp  fixed documentation references.
01i,18aug04,elp  mtu configuration.
01h,16apr04,elp  updated documentation
01g,14apr04,elp  indicate when simulator is listening to mcast addresses. 
01f,01apr04,elp  added RFC_2233 support.
01e,22mar04,jmp  fixed simnetLoad() prototype.
01d,04dec03,elp  fixed DIAB warning.
01c,02oct03,elp  fixed simnetSend() return value,
		 removed useless intLock()/intUnlock().
01b,01sep03,elp  renamed, reworked subnet name configuration,
		 moved cluster blocks configuration in driver structure,
		 fixed multicast and promiscuous modes.
01a,02jun03,elp  created.
*/

/*
DESCRIPTION
This module implements Wind River VxWorks Simulator Ethernet network interface
driver. It requires ithat Wind River Network Daemon (vxsimnetd) was previsouly
started with a network configuration. 

EXTERNAL INTERFACE
The driver provides the standard external interface, simnetLoad(), which takes
a string of colon separated parameters. 

The only optional parameter is subnet name as described in Network Daemon
configuration file.

TARGET-SPECIFIC PARAMETERS

.IP <paramString>
This parameter is passed to the driver via simnetLoad(). It is used to specify
subnet to attach to. If it is set VxSim tries to attach to a matching subnet
configured in running Network Daemon, if subnet name is not specified IP
address is used instead to find a matching subnet, if neither subnet name nor
IP address is specified, VxSim attaches to the first configured subnet.

INCLUDES:
end.h endLib.h etherMultiLib.h

SEE ALSO: ifLib,
.I "Wind River VxWorks Simulator User's Guide : Networking with the VxWorks Simulator" .
*/

/* includes */

#include <vxWorks.h>
#include <vsbConfig.h>
#include <stdlib.h>
#include <cacheLib.h>
#include <intLib.h>
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

#define END_MACROS
#include <end.h>			/* Common END structures. */
#include <endLib.h>

#include <net/mbuf.h>
#include <net/unixLib.h>
#include <net/protosw.h>
#include <net/systm.h>
#include <net/route.h>
#include <netinet/if_ether.h>

#include <sys/times.h>

#include <private/vxsimHostLibP.h>
#include <drv/end/simnetEnd.h>

IMPORT	int endMultiLstCnt (END_OBJ * pEnd);

/* defines */

#define	SIMNET_DRV_NAME		"simnet"
#define SIMNET_DRV_NAME_LEN	(sizeof(SIMNET_DRV_NAME) + 1)
#define	SIMNET_NAT_DRV_NAME	"simnet_nat"
#define SIMNET_NAT_DRV_NAME_LEN	(sizeof(SIMNET_NAT_DRV_NAME) + 1)

/* Configuration items */

/* make sure data after ethernet header is correctly aligned */

#define SIMNET_ALIGN_OFFSET	2	

#define SIMNET_DATASIZ(pDrvCtrl)	\
    (pDrvCtrl->simnetMtu + SIZEOF_ETHERHEADER + 4 + SIMNET_ALIGN_OFFSET)
#define SIMNET_BUFSIZ(pDrvCtrl)		\
    (MEM_ROUND_UP(SIMNET_DATASIZ(pDrvCtrl)))

#define EH_SIZE		(14)
#define END_SPEED_10M	10000000	/* 10Mbs */
#define END_SPEED_100M	100000000	/* 100Mbs */
#define END_SPEED       END_SPEED_10M

#define SIMNET_CLUSTER_NUM 128

/* Cache macros */

#define END_CACHE_INVALIDATE(address, len) \
    CACHE_DRV_INVALIDATE (pDrvCtrl->pCacheFuncs, (address), (len))

#define END_CACHE_VIRT_TO_PHYS(address) \
    CACHE_DRV_VIRT_TO_PHYS (pDrvCtrl->pCacheFuncs, (address))

/*
 * Default macro definitions for BSP interface.
 * These macros can be redefined in a wrapper file, to generate
 * a new module with an optimized interface.
 */

/* A shortcut for getting the hardware address from the MIB II stuff. */

#define END_FLAGS_ISSET(pEnd, setBits)	\
    ((pEnd)->flags & (setBits))

#define END_HADDR(pEnd)			\
    ((pEnd)->mib2Tbl.ifPhysAddress.phyAddress)

#define END_HADDR_LEN(pEnd) 		\
    ((pEnd)->mib2Tbl.ifPhysAddress.addrLength)

/* SIMNET_DRV_CTRL flags access macros */

#ifdef _WRS_CONFIG_SMP
#define SIMNET_DRV_FLAGS_SET(setBits)	(vxAtomicOr (&pDrvCtrl->flags, setBits))
#define SIMNET_DRV_FLAGS_ISSET(setBits)	(vxAtomicGet(&pDrvCtrl->flags) \
					 & setBits)
#define SIMNET_DRV_FLAGS_CLR(clrBits)	(vxAtomicAnd (&pDrvCtrl->flags, \
						      (atomicVal_t)(~clrBits)))
#define SIMNET_DRV_FLAGS_GET()		(vxAtomicGet (&pDrvCtrl->flags))

#define SIMNET_RXPENDING_CLR(pDrvCtrl)	\
    (vxAtomicSet(&pDrvCtrl->simnetRxPending, (atomic_t)0))

#define SIMNET_RXPENDING_TEST_AND_SET(pDrvCtrl)	\
    (!vxAtomicSet(&pDrvCtrl->simnetRxPending, (atomic_t)1))

#define SIMNET_RXPENDING_SET(pDrvCtrl)	\
    (vxAtomicSet(&pDrvCtrl->simnetRxPending, (atomic_t)1))

#define SIMNET_RXPENDING_GET(pDrvCtrl)	\
    (vxAtomicGet(&pDrvCtrl->simnetRxPending))

#else /* _WRS_CONFIG_SMP */
#define SIMNET_DRV_FLAGS_SET(setBits)	(pDrvCtrl->flags |= (setBits))
#define SIMNET_DRV_FLAGS_ISSET(setBits)	(pDrvCtrl->flags & (setBits))
#define SIMNET_DRV_FLAGS_CLR(clrBits)	(pDrvCtrl->flags &= ~(clrBits))
#define SIMNET_DRV_FLAGS_GET()		(pDrvCtrl->flags)

#define SIMNET_RXPENDING_CLR(pDrvCtrl)		\
    pDrvCtrl->simnetRxPending = 0

#define SIMNET_RXPENDING_TEST_AND_SET(pDrvCtrl)		\
    (vxTas ((void *)&pDrvCtrl->simnetRxPending))

#define SIMNET_RXPENDING_SET(pDrvCtrl)		\
    (pDrvCtrl->simnetRxPending = 1)

#endif /* _WRS_CONFIG_SMP */

/* Definitions for the flags field */

#define SIMNET_PROMISC		0x1
#define SIMNET_POLLING		0x2
#define SIMNET_MCAST		0x4

/* DEBUG MACROS */

#undef	SIMNET_DRV_DEBUG
#ifdef	SIMNET_DRV_DEBUG
#define SIMNET_DRV_DEBUG_OFF		0x0000
#define SIMNET_DRV_DEBUG_RX		0x0001
#define	SIMNET_DRV_DEBUG_TX		0x0002
#define SIMNET_DRV_DEBUG_INT		0x0004
#define	SIMNET_DRV_DEBUG_POLL_RX	0x0008
#define	SIMNET_DRV_DEBUG_POLL_TX	0x0010
#define	SIMNET_DRV_DEBUG_POLL		\
    (SIMNET_DRV_DEBUG_POLL_RX | SIMNET_DRV_DEBUG_POLL_TX)
#define	SIMNET_DRV_DEBUG_LOAD		0x0020
#define SIMNET_DRV_DEBUG_DUMP		0x0080
#define SIMNET_DRV_DEBUG_ERROR		0x0100
#define SIMNET_DRV_DEBUG_POLL_REDIR	0x10000
#define	SIMNET_DRV_DEBUG_LOG_NVRAM	0x20000


int	simnetDebug = SIMNET_DRV_DEBUG_LOAD|SIMNET_DRV_DEBUG_ERROR;

#define SIMNET_DRV_LOG(FLG, X)						\
	if (simnetDebug & FLG)					\
            vxsimHostPrintf X;

#define SIMNET_DRV_DUMP(FLG, buf, len)					\
    if ((simnetDebug & (SIMNET_DRV_DEBUG_DUMP | FLG)) ==		\
	(SIMNET_DRV_DEBUG_DUMP | FLG)) 				\
	packetDump (buf, len, FLG & SIMNET_DRV_DEBUG_TX);

#define SIMNET_DRV_PRINT(FLG,X)					\
	if (simnetDebug & FLG) printf X;

void		packetDump  (unsigned char * buf, int len, int type);

#else /*SIMNET_DRV_DEBUG*/

#define SIMNET_DRV_LOG(DBG_SW, X)
#define SIMNET_DRV_PRINT(DBG_SW,X)
#define SIMNET_DRV_DUMP(DBG_SW,X,Y)

#endif /*SIMNET_DRV_DEBUG*/

/* typedefs */

typedef enum simnet_mode
    {
    SIMNET_NAT		= 1,
    SIMNET_VXSIMNETD	= 2,
    SIMNET_PKT_COPY	= 4
    } SIMNET_MODE;

/* The definition of the driver control structure */

typedef struct simnet_drv_ctrl
    {
    END_OBJ		endObj;		/* The class we inherit from. */
    int			unit;		/* unit number */
    atomic_t		flags;		/* Our local flags. */
    UCHAR		enetAddr[6];	/* ethernet address */
    UCHAR		brdAddr[6];	/* broadcast address */
    char 		paramString[32];/* simnet param string */
    void *		pNetCfg;	/* handle */
    int			intId;		/* interrupt mean */
    CL_POOL_ID		pClPoolId;	/* cluster pool */
    char *		pMclkArea;	/* address of Mclk */
    int			simnetMtu;	/* simnet max mtu */
    SEM_ID		simnetDevSemId;	/* device semaphore */
    JOB_QUEUE_ID	simnetJobQueue;	/* simnet receive job queue */
    QJOB		simnetRxJob;
    atomicVal_t		simnetRxPending;/* rcv task is scheduled */
    int			mode;		/* simnet mode (NAT, vxsimnetd...) */
    char *		pSendingBuffer;
    char *		pPollingBuffer;
    M_BLK_ID  		pErrMblk;	/* error mblk */
    } SIMNET_DRV_CTRL;

/* EXTERNALS */

/* LOCALS */

/* forward static functions */

LOCAL void	simnetInt (SIMNET_DRV_CTRL * pDrvCtrl);
LOCAL void	simnetEndRxHandle (void * pArg);

/* END Specific interfaces. */

/* This is the only externally visible interface. */

LOCAL END_OBJ * simnetLoad2 (BOOL isNat, char * initString);
LOCAL STATUS	simnetStart (SIMNET_DRV_CTRL * pDrvCtrl);
LOCAL STATUS	simnetStop (SIMNET_DRV_CTRL * pDrvCtrl);
LOCAL int	simnetIoctl (SIMNET_DRV_CTRL * pDrvCtrl, int cmd, caddr_t data);
LOCAL STATUS	simnetUnload (SIMNET_DRV_CTRL * pDrvCtrl);
LOCAL STATUS	simnetSend (SIMNET_DRV_CTRL * pDrvCtrl, M_BLK_ID pBuf);

LOCAL STATUS	simnetMCastAdd (SIMNET_DRV_CTRL * pDrvCtrl, char * pAddress);
LOCAL STATUS	simnetMCastDel (SIMNET_DRV_CTRL * pDrvCtrl, char * pAddress);
LOCAL STATUS	simnetMCastGet (SIMNET_DRV_CTRL * pDrvCtrl,
				MULTI_TABLE * pTable);
LOCAL STATUS	simnetPollStart (SIMNET_DRV_CTRL * pDrvCtrl);
LOCAL STATUS	simnetPollStop (SIMNET_DRV_CTRL * pDrvCtrl);
LOCAL STATUS	simnetPollRcv (SIMNET_DRV_CTRL * pDrvCtrl, M_BLK_ID pBuf);

LOCAL STATUS	simnetParse ();
LOCAL STATUS	simnetMemInit ();

/*
 * Declare our function table.  This is static across all device
 * instances.
 */

LOCAL NET_FUNCS simnetFuncTable =
    {
    (FUNCPTR) simnetStart,	/* Function to start the device. */
    (FUNCPTR) simnetStop,	/* Function to stop the device. */
    (FUNCPTR) simnetUnload,	/* Unloading function for the driver. */
    (FUNCPTR) simnetIoctl,	/* Ioctl function for the driver. */
    (FUNCPTR) simnetSend,	/* Send function for the driver. */
    (FUNCPTR) simnetMCastAdd,	/* Multicast add function for the driver. */
    (FUNCPTR) simnetMCastDel,	/* Multicast delete function for the driver. */
    (FUNCPTR) simnetMCastGet,	/* Multicast retrieve function. */
    (FUNCPTR) simnetSend,	/* Polling send function */
    (FUNCPTR) simnetPollRcv,	/* Polling receive function */
    endEtherAddressForm, 	/* put address info into a NET_BUFFER */
    endEtherPacketDataGet,	/* get pointer to data in NET_BUFFER */
    endEtherPacketAddrGet  	/* Get packet addresses. */
    };

/*******************************************************************************
*
* simnetLoad - initialize the simnet driver and device in vxsimnetd mode
*
* This routine initializes the driver and the device to the operational state.
* The unit number that is automatically added is parsed, and then subnet name
* to attach to is expected. If no subnet name is provided, an empty string is
* used, simulator will then use IP address if any to attach to first matching
* subnet.
*
* This routine can be called in two modes.  If it is called with an empty but
* allocated string, it places the name of this device into the <initString>
* and returns 0.
*
* If the string is allocated and not empty, the routine attempts to load
* the driver using the values specified in the string.
*
* RETURNS: An END object pointer, or NULL on error, or 0 and the name of the
* device if the <initString> was empty.
*/

END_OBJ * simnetLoad
    (
    char * initString,		/* String to be parsed by the driver. */
    void * unused
    )
    {
    return (simnetLoad2 (FALSE, initString));
    }

/*******************************************************************************
*
* simnetNatLoad - initialize the simnet driver and device in NAT mode
*
* This routine initializes the driver and the device to the operational state.
* The unit number that is automatically added is parsed, and then subnet name
* to attach to is expected. If no subnet name is provided, an empty string is
* used, simulator will then use IP address if any to attach to first matching
* subnet.
*
* This routine can be called in two modes.  If it is called with an empty but
* allocated string, it places the name of this device into the <initString>
* and returns 0.
*
* If the string is allocated and not empty, the routine attempts to load
* the driver using the values specified in the string.
*
* RETURNS: An END object pointer, or NULL on error, or 0 and the name of the
* device if the <initString> was empty.
*/

END_OBJ * simnetNatLoad
    (
    char * initString,		/* String to be parsed by the driver. */
    void * unused
    )
    {
    return (simnetLoad2 (TRUE, initString));
    }

/*******************************************************************************
*
* simnetLoad2 - initialize the driver and device in NAT or vxsimnetd NODE
*
* This routine initializes the driver and the device to the operational state.
* The unit number that is automatically added is parsed, and then subnet name
* to attach to is expected. If no subnet name is provided, an empty string is
* used, simulator will then use IP address if any to attach to first matching
* subnet.
*
* This routine can be called in two modes.  If it is called with an empty but
* allocated string, it places the name of this device into the <initString>
* and returns 0.
*
* If the string is allocated and not empty, the routine attempts to load
* the driver using the values specified in the string.
*
* RETURNS: An END object pointer, or NULL on error, or 0 and the name of the
* device if the <initString> was empty.
*/

LOCAL END_OBJ * simnetLoad2
    (
    BOOL	isNat,		/* simnet driver is in NAT mode? */
    char *	initString	/* String to be parsed by the driver. */
    )
    {
    SIMNET_DRV_CTRL *	pDrvCtrl;
    char *		ipAddr = NULL;
    UINT32		endFlags;
    char *		simnetDrvName;
    size_t		simnetDrvNameLength;

    if (initString == NULL)
        return NULL;

    if (isNat)
    	{
    	simnetDrvName = (char *) SIMNET_NAT_DRV_NAME;
	simnetDrvNameLength = SIMNET_NAT_DRV_NAME_LEN;
	}
    else
    	{
    	simnetDrvName = (char *) SIMNET_DRV_NAME;
	simnetDrvNameLength = SIMNET_DRV_NAME_LEN;
	}
    
    if (initString[0] == EOS)
        {
        bcopy (simnetDrvName, initString, simnetDrvNameLength);
        return NULL;
        }

    /* else initString is not blank, pass two ... */

    /* allocate the device structure */

    pDrvCtrl = (SIMNET_DRV_CTRL *)calloc (sizeof (SIMNET_DRV_CTRL), 1);
    if (pDrvCtrl == NULL)
	goto errorExit;

    pDrvCtrl->simnetDevSemId = semMCreate (SEM_Q_PRIORITY | SEM_DELETE_SAFE |
    					   SEM_INVERSION_SAFE);
    if (pDrvCtrl->simnetDevSemId == NULL)
	goto errorExit;

    /*
     * parse the init string, possibly get the subnet name we should try to
     * attach to.
     */

    if (simnetParse (pDrvCtrl, initString) == ERROR)
	goto errorExit;

    /*
     * add IP address found in boot parameters for boot devices or 
     * in additional interfaces 
     */

    if ((strcmp (sysBootParams.bootDev, simnetDrvName) == 0) &&
    	(sysBootParams.unitNum == pDrvCtrl->unit))
	{
	ipAddr = sysBootParams.ead;
	}
    else 
    	{
	int	ix;
	int	unit;
	char *	dev;
	char *	devStr;
	char *	ipMask;
	
	for (ix = 0;
	     vxsimHostNetAddIfGet (ix, &dev, &unit, &devStr, &ipAddr, &ipMask)
	     == OK;
	     ix++)
	     {
	     if (strcmp (dev, simnetDrvName) == 0 && unit == pDrvCtrl->unit)
		 break;
	     ipAddr = NULL;
	     }
	}

    if (vxsimHostSimnetAttach (simnetDrvName, pDrvCtrl->paramString,
    				ipAddr,  pDrvCtrl->unit,
				&pDrvCtrl->pNetCfg, &pDrvCtrl->intId) != OK)
	{
        SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_LOAD,
			("simnetLoad: Attach failure \r\n"));
	goto errorExit;
	}

    /* get simulated network MTU */

    if (vxsimHostSimnetParamGet (pDrvCtrl->pNetCfg, SIMNET_SUBNET_MTU,
				 (UINT8 *)&pDrvCtrl->simnetMtu, 
				 sizeof(pDrvCtrl->simnetMtu)) != OK)
	{
	SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_LOAD,
			("simnetLoad: Failed to get MTU\r\n"));
	goto errorExit;
	}

    SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_LOAD,
		    ("simnetLoad: got MTU %d\r\n", pDrvCtrl->simnetMtu));

    if (isNat)
    	{
	/*
	 * Indicate this is a NAT driver. NAT driver requires to work in
	 * packet copy mode.
	 */

	pDrvCtrl->mode = SIMNET_NAT|SIMNET_PKT_COPY;
	}
    else
    	{
    	pDrvCtrl->mode = SIMNET_VXSIMNETD;

#ifdef _WRS_CONFIG_SMP
	/*
	 * On SMP every CPUs do not map simnet shared memory thus can not
	 * directly write packet from mbuf. It is 1st mandatory copy
	 * mblk to sending buffer and then perform host call that puts is
	 * into shared memory before sending it out.
	 */

	pDrvCtrl->mode |= SIMNET_PKT_COPY;
#endif
	}

    if ((pDrvCtrl->mode & SIMNET_PKT_COPY) == SIMNET_PKT_COPY)
    	{
	pDrvCtrl->pSendingBuffer = malloc (SIMNET_DATASIZ(pDrvCtrl));
	pDrvCtrl->pPollingBuffer = malloc (SIMNET_DATASIZ(pDrvCtrl));

	if (pDrvCtrl->pSendingBuffer == NULL ||
		pDrvCtrl->pPollingBuffer == NULL)
	    {
	    SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_LOAD,
		    ("simnetLoad: Failed to allocate sending buffers\r\n"));
	    goto errorExit;
	    }
	}

    /* get ethernet address. */

    memset ((char *)pDrvCtrl->brdAddr, 0xff, 6);
    bzero ((char *)pDrvCtrl->enetAddr, 6);
    vxsimHostSimnetPhysAddrGet (pDrvCtrl->pNetCfg, pDrvCtrl->enetAddr,
			        sizeof(pDrvCtrl->enetAddr));

    /* initialize the END and MIB2 parts of the structure */

    /*
     * The M2 element must come from m2Lib.h
     * Type is set to other so that above layers do not try to use ethernet
     * addresses.
     */

    if (END_OBJ_INIT (&pDrvCtrl->endObj, NULL, simnetDrvName,
                      pDrvCtrl->unit, &simnetFuncTable,
                      "SIM END Driver.") == ERROR)
	goto errorExit;

#ifdef INCLUDE_RFC_2233
	
    /* Initialize MIB-II entries (for RFC 2233 ifXTable) */
    pDrvCtrl->endObj.pMib2Tbl = m2IfAlloc (M2_ifType_ethernet_csmacd,
    					   &pDrvCtrl->enetAddr[0], 6,
					   pDrvCtrl->simnetMtu, END_SPEED,
					   simnetDrvName,
					   pDrvCtrl->unit);
    if (pDrvCtrl->endObj.pMib2Tbl == NULL)
    	{
	logMsg ("%s%d - MIB-II initializations failed\n",
		simnetDrvName, pDrvCtrl->unit);
	goto errorExit;
	}

    /*
     * Set the RFC2233 flag bit in the END object flags field and
     * install the counter update routines.
     */
    m2IfPktCountRtnInstall(pDrvCtrl->endObj.pMib2Tbl, m2If8023PacketCount);

    /*
     * Make a copy of the data in mib2Tbl struct as well. We do this
     * mainly for backward compatibility issues. There might be some
     * code that might be referencing the END pointer and might
     * possibly do lookups on the mib2Tbl, which will cause all sorts
     * of problems.
     */

    bcopy ((char *)&pDrvCtrl->endObj.pMib2Tbl->m2Data.mibIfTbl,
    	   (char *)&pDrvCtrl->endObj.mib2Tbl, sizeof (M2_INTERFACETBL));

    /* Mark the device ready */

    endFlags = IFF_NOTRAILERS | IFF_MULTICAST | IFF_BROADCAST | END_MIB_2233;
#else
    /* Old RFC 1213 mib2 interface */

    if (END_MIB_INIT (&pDrvCtrl->endObj, M2_ifType_ethernet_csmacd,
                      &pDrvCtrl->enetAddr[0], 6, pDrvCtrl->simnetMtu,
                      END_SPEED) == ERROR)
	{
	goto errorExit;
	}
    endFlags = IFF_NOTRAILERS | IFF_MULTICAST | IFF_BROADCAST;
#endif /* INCLUDE_RFC_2233 */
    endFlags |= IFF_SIMPLEX;

    /* Perform memory allocation/distribution */

    if (simnetMemInit (pDrvCtrl) == ERROR)
	goto errorExit;

    /* set the flags to indicate readiness */

    END_OBJ_READY (&pDrvCtrl->endObj, endFlags);

    SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_LOAD,
    		    ("Done loading %s%d...\n", simnetDrvName,
		    pDrvCtrl->unit));

    return (&pDrvCtrl->endObj);

errorExit:
    if (pDrvCtrl != NULL)
	{
	if (pDrvCtrl->simnetDevSemId != NULL)
	    semDelete (pDrvCtrl->simnetDevSemId);
	if (pDrvCtrl->pSendingBuffer != NULL)
	    free (pDrvCtrl->pSendingBuffer);
	if (pDrvCtrl->pPollingBuffer != NULL)
	    free (pDrvCtrl->pPollingBuffer);
	free ((char *)pDrvCtrl);
	}

    return NULL;
    }

/*******************************************************************************
*
* simnetParse - parse the init string
*
* Parse the input string.  Fill in values in the driver control structure.
*
* The muxLib.o module automatically prepends the unit number to the user's
* initialization string from the BSP (configNet.h).
*
* RETURNS: OK or ERROR for invalid arguments.
*
* NOMANUAL
*/

LOCAL STATUS simnetParse
    (
    SIMNET_DRV_CTRL * pDrvCtrl,	/* device pointer */
    char * initString		/* information string */
    )
    {
    char *	tok;
    char *	pHolder = NULL;

    /* Get unit number. */

    tok = strtok_r (initString, ":", &pHolder);
    if (tok == NULL)
	return ERROR;
    pDrvCtrl->unit = atoi (tok);

    /* Get simnet parameter string */

    bzero (pDrvCtrl->paramString, sizeof (pDrvCtrl->paramString));

    if (strncmp (pHolder, "(null)", 6) != 0)
	strncpy (pDrvCtrl->paramString, pHolder,
		 min (strlen (pHolder), sizeof (pDrvCtrl->paramString) - 1));

    return OK;
    }

/*******************************************************************************
*
* simnetMemInit - initialize memory for the chip
*
* This routines allocates descriptor buffers separate from the data buffers.
* Use the same buffering scheme for Rx and Tx.
*
* RETURNS: OK or ERROR.
*
* NOMANUAL
*/

LOCAL STATUS simnetMemInit
    (
    SIMNET_DRV_CTRL * pDrvCtrl	/* device to be initialized */
    )
    {
    M_CL_CONFIG simnetMclBlkConfig = 	/* cluster blocks configuration */
	{
	/*
	no. mBlks		no. clBlks	memArea		memSize
	-----------		----------	-------		-------
	*/
	0, 			0, 		NULL, 		0
	};
    				
    CL_DESC simnetClDescTbl [] = 	/* cluster blocks config table */
	{
	/*
	clusterSize		num	memArea		memSize
	-----------		----	-------		-------
	*/
	{0,			SIMNET_CLUSTER_NUM,	NULL,		0}
	};

    int simnetClDescTblNumEnt = (NELEMENTS(simnetClDescTbl));

    simnetClDescTbl[0].clSize = SIMNET_BUFSIZ(pDrvCtrl);

    /*
     * This is how we would set up an END netPool using netBufLib(1).
     * This code is pretty generic.
     */

    if ((pDrvCtrl->endObj.pNetPool = malloc (sizeof(NET_POOL))) == NULL)
        return (ERROR);

	
    /* number of driver Descriptor/data buffers */

    simnetClDescTbl[0].clNum = SIMNET_CLUSTER_NUM;

    /* Setup mbuf/cluster block pool with same number of mbufs and clBlks */

    simnetMclBlkConfig.clBlkNum = simnetClDescTbl[0].clNum;
    simnetMclBlkConfig.mBlkNum = simnetMclBlkConfig.clBlkNum;

    /* Calculate the total memory for all the M-Blks and CL-Blks. */

    simnetMclBlkConfig.memSize =
    	(simnetMclBlkConfig.mBlkNum * (M_BLK_SZ + sizeof (void *))) +
	(simnetMclBlkConfig.clBlkNum * CL_BLK_SZ);

    /* allocate mbuf/Cluster blocks from normal memory */

    if ((simnetMclBlkConfig.memArea =
    	 (char *) memalign (sizeof(long), simnetMclBlkConfig.memSize)) == NULL)
        return (ERROR);

    pDrvCtrl->pMclkArea = simnetMclBlkConfig.memArea;

    /* Calculate the memory size of all the clusters. */

    simnetClDescTbl[0].memSize =
    	(simnetClDescTbl[0].clNum * (simnetClDescTbl[0].clSize +
				     sizeof(void *)));

    /* Allocate the memory for the clusters. */

    simnetClDescTbl[0].memArea = (char *) malloc (simnetClDescTbl[0].memSize);

    if (simnetClDescTbl[0].memArea == NULL)
        {
        SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_LOAD, ("system memory unavailable\n"));
        return (ERROR);
        }

    /* Initialize the memory pool. */

    if (netPoolInit(pDrvCtrl->endObj.pNetPool, &simnetMclBlkConfig,
                    &simnetClDescTbl[0], simnetClDescTblNumEnt,
		    NULL) == ERROR)
        {
        SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_LOAD,
			("Could not init buffering %#x\n", errno));
        return (ERROR);
        }

     /* Store the cluster pool id as others need it later. */

    pDrvCtrl->pClPoolId = clPoolIdGet (pDrvCtrl->endObj.pNetPool,
				       SIMNET_BUFSIZ(pDrvCtrl),
     				       FALSE);

    /* Reserve  one cluster for error case */

    if ((pDrvCtrl->pErrMblk = netTupleGet (pDrvCtrl->endObj.pNetPool,
					      SIMNET_DATASIZ(pDrvCtrl),
					      M_DONTWAIT, MT_DATA, TRUE))
	 == NULL)
    	{
	SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_LOAD,
			("Could not reserve error cluster\n"));
	return ERROR;
	}
    pDrvCtrl->pErrMblk->m_data += SIMNET_ALIGN_OFFSET;

    SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_LOAD,
    		    ("Memory setup complete, pNetPool=%x\n",
		    pDrvCtrl->endObj.pNetPool));

    return OK;
    }

/*******************************************************************************
*
* simnetStart - start the device
*
* This function connects interrupts and start the device running in interrupt
* mode.
*
* RETURNS: OK or ERROR
*
* NOMANUAL
*/

LOCAL STATUS simnetStart
    (
    SIMNET_DRV_CTRL * pDrvCtrl	/* device ID */
    )
    {
    END_OBJ *		pEnd = &pDrvCtrl->endObj;

    SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_LOAD, ("simnetStart %x\n", pDrvCtrl));

    if (semTake (pDrvCtrl->simnetDevSemId, WAIT_FOREVER) == ERROR)
    	return ERROR;

    END_TX_SEM_TAKE (pEnd, WAIT_FOREVER);

    if (pEnd->flags & IFF_UP)
    	{
	END_TX_SEM_GIVE (pEnd);
	semGive (pDrvCtrl->simnetDevSemId);
	return (OK);
	}

    /* Initialize job queues */

    pDrvCtrl->simnetJobQueue = netJobQueueId;

    pDrvCtrl->simnetRxJob.func = simnetEndRxHandle;

    if (intConnect ((VOIDFUNCPTR *)(long) pDrvCtrl->intId,
    		    &simnetInt, (_Vx_usr_arg_t)pDrvCtrl) == ERROR)
	{
	END_TX_SEM_GIVE (pEnd);
	semGive (pDrvCtrl->simnetDevSemId);
	return ERROR;
	}

    vxsimHostSimnetIntEnable (pDrvCtrl->pNetCfg, pDrvCtrl->intId, TRUE);
    SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_LOAD, ("interrupt enabled.\n"));

    END_FLAGS_SET (pEnd, IFF_UP | IFF_RUNNING);

    END_TX_SEM_GIVE (pEnd);
    semGive (pDrvCtrl->simnetDevSemId);
    return (OK);
    }


/*******************************************************************************
*
* simnetInt - handle controller interrupt
*
* This routine is called at interrupt level in response to an interrupt from
* the controller.
*
* RETURNS: N/A.
*
* NOMANUAL
*/

LOCAL void simnetInt
    (
    SIMNET_DRV_CTRL  * pDrvCtrl	/* interrupting device */
    )
    {
    SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_INT, ("Got an interrupt!\n"));

    /* clear interrupt source */

    vxsimHostSimnetIntClear (pDrvCtrl->pNetCfg, pDrvCtrl->intId);

    /* Have netTask handle any input packets */

    if (SIMNET_RXPENDING_TEST_AND_SET(pDrvCtrl))
	{
	jobQueuePost (pDrvCtrl->simnetJobQueue, &pDrvCtrl->simnetRxJob);
        }
    else	
	{
	SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_RX,
			("interrupt already processing!\n"));
	}
    }


/*******************************************************************************
*
* simnetPacketGet - get next received message
*
* Get next received message.  Returns -1 if none are ready.
* This routines also filters out packet that are not for this target based on
* packet destination ethernet address and MAC address/or multicast addresses.
*
* RETURNS: remain qlen on success, -1 on failure/no pkt.
*
* NOMANUAL
*/

LOCAL int simnetPacketGet
    (
    SIMNET_DRV_CTRL *	pDrvCtrl,	/* device structure */
    char *		buffer,
    size_t *		pBufSize	
    )
    {
    int			qlen;
    ETHER_MULTI *	pCurr;
    int			availableBufSize = *pBufSize;

again:
    *pBufSize = availableBufSize;
    qlen = vxsimHostSimnetPktReceive (pDrvCtrl->pNetCfg, buffer, pBufSize);
    if (qlen != -1)
    	{
	if (SIMNET_DRV_FLAGS_ISSET(SIMNET_PROMISC))
	    {
	    return qlen;	/* any packet is received */
	    }
	
	if ((memcmp (buffer, pDrvCtrl->enetAddr, 6) == 0) ||
	    (memcmp (buffer, (char *)pDrvCtrl->brdAddr, 6) == 0))
	    {
	    return qlen;	/* either broadcast or right destination */
	    }
	else
	    {
	    if (SIMNET_DRV_FLAGS_ISSET(SIMNET_MCAST))
	    	{
		pCurr = END_MULTI_LST_FIRST(&pDrvCtrl->endObj);
		while (pCurr != NULL)
		    {
		    if (memcmp (buffer, (char *)pCurr->addr, 6) == 0)
			{
			return qlen;	/* match : process packet */
			}
		    pCurr = END_MULTI_LST_NEXT(pCurr);
		    }
		}
	    goto again;		/* not for us, try another one */
	    }
	}
    return qlen;
    }

/*******************************************************************************
*
* simnetEndRxHandle - task level interrupt service for input packets
*
* This function is scheduled by the ISR to run in the context of tNetTask
* whenever an RX interrupt is received.
* The double loop is to protect against a race condition where the interrupt
* code see <simnetRxPending> as TRUE, but it is then turned off by task code.
* This race is not fatal, but does cause occassional delays until a second
* packet is received and then triggers the netTask to call this routine again.
*
* RETURNS: N/A.
*
* NOMANUAL
*/

LOCAL void simnetEndRxHandle
    (
    void *	pArg
    )
    {
    QJOB *		pJob = (QJOB *)pArg;
    SIMNET_DRV_CTRL *	pDrvCtrl = member_to_object(pJob, SIMNET_DRV_CTRL,
    						    simnetRxJob);
    M_BLK_ID		pNewMblk;
    int			qlen = 1;
    char *		pPkt;
    size_t		pktSize = SIMNET_DATASIZ(pDrvCtrl);
    int			loopCnt = 0;

    FOREVER
    	{
	pNewMblk = netTupleGet (pDrvCtrl->endObj.pNetPool,
				SIMNET_DATASIZ(pDrvCtrl),
				M_DONTWAIT, MT_DATA, TRUE);
	if (pNewMblk == NULL)
	    {
	    SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_ERROR,
	    		    ("netTupleGet failure\n"));
	    qlen = simnetPacketGet (pDrvCtrl,
	    			    pDrvCtrl->pErrMblk->m_data,
				    &pktSize);
	    if (qlen < 0)
		{
		SIMNET_RXPENDING_CLR (pDrvCtrl);

		/*
		 * test again to prevent a race condition
		 * an interrupt could occur between previous test (qlen < 0)
		 * and before simnetRxPending is cleared
		 */

		qlen = simnetPacketGet (pDrvCtrl, pDrvCtrl->pErrMblk->m_data,
					&pktSize);
		if (qlen < 0)
		    return;

		SIMNET_RXPENDING_SET (pDrvCtrl);
		}
	    
	    /*
	     * Drop the packet and let time to the application/tNetTask
	     * to consume existing packets and release memory
	     */

	    END_ERR_ADD (&pDrvCtrl->endObj, MIB2_IN_ERRS, +1);
	    jobQueuePost (pDrvCtrl->simnetJobQueue, &pDrvCtrl->simnetRxJob);
	    return;
	    }

	/* Adjust the mblk to align payload on longword boundary */

	pNewMblk->m_data += SIMNET_ALIGN_OFFSET;

	/*
	 * get a packet. Pass MTU to check that data does not exceed 
	 * allocated size (extra bytes for headers are always allocated) 
	 */

	pktSize = SIMNET_DATASIZ(pDrvCtrl);
	qlen = simnetPacketGet (pDrvCtrl, pNewMblk->m_data, &pktSize);

	SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_RX,
			("simnetPacketGet done %d\n", qlen));
    	
	if (qlen < 0)	/* no packet */
	    {
	    SIMNET_RXPENDING_CLR (pDrvCtrl);

	    /*
	     * test again to prevent a race condition
	     * an interrupt could occur between previous test (qlen < 0)
	     * and before simnetRxPending is cleared
	     */

	    pktSize = SIMNET_DATASIZ(pDrvCtrl);
	    qlen = simnetPacketGet (pDrvCtrl, pNewMblk->m_data, &pktSize);
	    if (qlen < 0)
		{
		netTupleFree (pNewMblk);
		return;
		}
	    SIMNET_RXPENDING_SET (pDrvCtrl);
	    }

	pPkt = (char *)pNewMblk->m_data;
	pNewMblk->mBlkHdr.mFlags |= M_PKTHDR;
	pNewMblk->mBlkHdr.mLen = pktSize;
	pNewMblk->mBlkPktHdr.len =  pNewMblk->mBlkHdr.mLen;
	END_ERR_ADD (&pDrvCtrl->endObj, MIB2_IN_UCAST, +1);
	SIMNET_DRV_DUMP(SIMNET_DRV_DEBUG_RX,
			(unsigned char *)pNewMblk->m_data, pktSize);
	
	/* Give the frame to the stack. */

	END_RCV_RTN_CALL (&pDrvCtrl->endObj, pNewMblk);

	if (loopCnt++ > 16)
	    {
	    jobQueuePost (pDrvCtrl->simnetJobQueue, &pDrvCtrl->simnetRxJob);
	    return;
	    }
	}
    }

/*******************************************************************************
*
* simnetSend - the driver send routine
*
* This routine takes a M_BLK_ID sends off the data in the M_BLK_ID.
* The buffer must already have the addressing information properly installed
* in it.  This is done by a higher layer.  
* This routine works both in NORMAL and POLLED mode.
*
* RETURNS: OK, ERROR, or END_ERR_BLOCK.
*
* NOMANUAL
*/

LOCAL STATUS simnetSend
    (
    SIMNET_DRV_CTRL *	pDrvCtrl,	/* device ptr */
    M_BLK_ID     	pMblk		/* data to send */
    )
    {
    STATUS		stat = OK;
    BOOL		polling = SIMNET_DRV_FLAGS_ISSET(SIMNET_POLLING);
    size_t		size;
    char *		buffer = NULL;
    void *		pPkt = NULL;

    SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_TX, ("simnetSend \n"));
    /*
     * Obtain exclusive access to transmitter.  This is necessary because
     * we might have more than one stack transmitting at once.
     */
    
    if (!polling)
	END_TX_SEM_TAKE (&pDrvCtrl->endObj, WAIT_FOREVER);

    if ((pDrvCtrl->mode & SIMNET_PKT_COPY) == SIMNET_PKT_COPY)
    	{
	if (!polling)
	    buffer = pDrvCtrl->pSendingBuffer;
	else
	    buffer = pDrvCtrl->pPollingBuffer;
	}
    else
    	{
	/*
	 * Get a empty slot to put the packet. Ask for a packet of the size of
	 * the MTU since we do not know yet how big will be this packet
	 * (we only know that it will be smaller than the MTU). Room for
	 * headers is always allocated.
	 */

	pPkt = vxsimHostSimnetPktAlloc (pDrvCtrl->pNetCfg, &buffer, &size);
	}

    if (buffer != NULL)
	{
	size = netMblkToBufCopy (pMblk, (char *)buffer, NULL);

	/* pad to minimum ethernet length */

	if (size < ETHERSMALL)
	    {
	    bzero ((char *)buffer + size, ETHERSMALL - size);
	    size = ETHERSMALL;
	    }

	SIMNET_DRV_DUMP(SIMNET_DRV_DEBUG_TX, (unsigned char *) buffer, size);

	/* send the packet : send interrupt to notify destination */

	if ((pDrvCtrl->mode & SIMNET_PKT_COPY) == SIMNET_PKT_COPY)
	    stat = vxsimHostSimnetBufSend (pDrvCtrl->pNetCfg, buffer, size);
	else
	    stat = vxsimHostSimnetPktSend (pDrvCtrl->pNetCfg, pPkt, size); 
	}
    else
    	{
	stat = ERROR;
	}

    /* Advance our management index(es) */

    if (!polling)
	{
	/*
	 * Cleanup.  As the driver copied the data from the mblks to a different
	 * buffer, it is possible to free the mblks now.  Otherwise, free the
	 * mblk chain after the device is finished with the TFD.
	 */

	netMblkClChainFree (pMblk);

	END_TX_SEM_GIVE (&pDrvCtrl->endObj);
	}

    /* Bump the statistics counters. */

    if (stat == OK)
	{
	END_ERR_ADD (&pDrvCtrl->endObj, MIB2_OUT_UCAST, +1);
	}
    else
    	{
	END_ERR_ADD (&pDrvCtrl->endObj, MIB2_OUT_ERRS, +1);
	}

    return (stat);
    }

/*******************************************************************************
*
* simnetIoctl - the driver I/O control routine
*
* Process an ioctl request.
*
* RETURNS: A command specific response, usually OK or ERROR.
*
* NOMANUAL
*/

LOCAL int simnetIoctl
    (
    SIMNET_DRV_CTRL * pDrvCtrl,	/* device receiving command */
    int cmd,			/* ioctl command code */
    caddr_t data		/* command argument */
    )
    {
    int error = 0;
    long value;

    if ((cmd != EIOCPOLLSTART) && (cmd != EIOCPOLLSTOP))
    	{
	if (semTake (pDrvCtrl->simnetDevSemId, WAIT_FOREVER) == ERROR)
	    return ERROR;
	}

    switch (cmd)
        {
        case EIOCSADDR:		/* set MAC address */
	    SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_LOAD,
	    		    ("simnetIoctl EIOCSADDR \n"));
	    if (data == NULL)
		return (EINVAL);
	    if (vxsimHostSimnetPhysAddrSet (pDrvCtrl->pNetCfg,
	    				    (UINT8 *)data, 6) == OK)
		{
		bcopy ((char *)data, (char *)pDrvCtrl->enetAddr, 6);
		bcopy ((char *)data, (char *)END_HADDR(&pDrvCtrl->endObj),
		       END_HADDR_LEN(&pDrvCtrl->endObj));
		}
	    else
	    	return (EINVAL);
            break;

        case EIOCGADDR:		/* get MAC address */
	    SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_LOAD,
	    		    ("simnetIoctl EIOCGADDR \n"));
	    if (data == NULL)
		return (EINVAL);
            bcopy ((char *)END_HADDR(&pDrvCtrl->endObj), (char *)data,
		    END_HADDR_LEN(&pDrvCtrl->endObj));
            break;

        case EIOCSFLAGS:	/* set (or clear) flags */
	    SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_LOAD,
	    		    ("simnetIoctl EIOCSFLAGFS \n"));
	    value = (long)data;
	    if (value < 0)
		{
		value = -value;
		value--;
		END_FLAGS_CLR (&pDrvCtrl->endObj, value);
		}
	    else
		{
		END_FLAGS_SET (&pDrvCtrl->endObj, value);
		}

	    /* handle IFF_PROMISC flag */

	    if (END_FLAGS_ISSET (&pDrvCtrl->endObj, IFF_PROMISC))
	    	{
		if (!SIMNET_DRV_FLAGS_ISSET(SIMNET_PROMISC))
		    {
		    /* turn on promiscuous mode */
		    SIMNET_DRV_FLAGS_SET(SIMNET_PROMISC);
		    vxsimHostSimnetPromiscModeSet (pDrvCtrl->pNetCfg, TRUE);
		    }
		}
	    else
	    	{
		if (SIMNET_DRV_FLAGS_ISSET(SIMNET_PROMISC))
		    {
		    /* turn off promiscuous mode */
		    SIMNET_DRV_FLAGS_CLR(SIMNET_PROMISC);
		    vxsimHostSimnetPromiscModeSet (pDrvCtrl->pNetCfg, FALSE);
		    }
		}
            break;

        case EIOCGFLAGS:	/* get flags */
	    SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_LOAD,
	    		    ("simnetIoctl EIOCSFLAGFS \n"));
	    *(int *)data = END_FLAGS_GET(&pDrvCtrl->endObj);
            break;

	case EIOCPOLLSTART:	/* Begin polled operation */
	    SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_LOAD,
	    		    ("simnetIoctl EIOCPOLLSTART \n"));
	    simnetPollStart (pDrvCtrl);
	    break;

	case EIOCPOLLSTOP:	/* End polled operation */
	    SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_LOAD,
	    		    ("simnetIoctl EIOCPOLLSTOP \n"));
	    simnetPollStop (pDrvCtrl);
	    break;

#ifdef INCLUDE_RFC_2233
        case EIOCGMIB2233:
	    SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_LOAD,
	    		    ("simnetIoctl EIOCGMIB2233 \n"));
	    if ((data == NULL) || (pEndObj->pMib2Tbl == NULL))
		error = EINVAL;
	    else
		*((M2_ID **)data) = pEndObj->pMib2Tbl;
	    break;
#endif /* INCLUDE_RFC_2233 */

        case EIOCGMIB2:		/* return MIB information */
	    SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_LOAD,
	    		    ("simnetIoctl EIOCGMIB2 \n"));
            if (data == NULL)
                return (EINVAL);
            bcopy((char *)&pDrvCtrl->endObj.mib2Tbl, (char *)data,
                  sizeof(pDrvCtrl->endObj.mib2Tbl));
            break;

        case EIOCGHDRLEN:
	    SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_LOAD,
	    		    ("simnetIoctl EIOCGHDRLEN \n"));
            if (data == NULL)
                return (EINVAL);
            *(int *)data = EH_SIZE;
            break;

	case EIOCMULTIADD:
	    SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_LOAD,
	    		    ("simnetIoctl EIOCMULTIADD \n"));
	    error = simnetMCastAdd (pDrvCtrl, (char *)data);
	    break;

	case EIOCMULTIDEL:
	    SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_LOAD,
	    		    ("simnetIoctl EIOCMULTIDEL \n"));
	    error = simnetMCastDel (pDrvCtrl, (char *)data);
	    break;

	case EIOCMULTIGET:
	    SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_LOAD,
	    		    ("simnetIoctl EIOCMULTIGET \n"));
	    error = simnetMCastGet (pDrvCtrl, (MULTI_TABLE *)data);
	    break;

        default:		/* unknown request */
            error = EINVAL;
        }

    if ((cmd != EIOCPOLLSTART) && (cmd != EIOCPOLLSTOP))
    	semGive (pDrvCtrl->simnetDevSemId);

    return (error);
    }

/*******************************************************************************
*
* simnetPollRcv - routine to receive a packet in polled mode.
*
* Polled mode operation takes place without any kernel or other OS
* services available.  Use extreme care to insure that this code does not
* call any kernel services.  Polled mode is only for WDB system mode use.
* Kernel services, semaphores, tasks, etc, are not available during WDB
* system mode.
*
* The WDB agent polls the device constantly looking for new data.  This routine
* examines the device for any new data and copies it to the provided mblk.
* The concern here is to keep the device supplied with empty buffers at all
* time.
*
* RETURNS: OK upon success.  EAGAIN is returned when no packet is available.
* A return of ERROR indicates a hardware fault or no support for polled mode
* at all.
*
* NOMANUAL
*/

LOCAL STATUS simnetPollRcv
    (
    SIMNET_DRV_CTRL *	pDrvCtrl,	/* device to be polled */
    M_BLK_ID		pMblk		/* ptr to buffer */
    )
    {
    char *	pPkt = NULL;
    size_t	size = SIMNET_DATASIZ(pDrvCtrl);

    pMblk->mBlkHdr.mData += SIMNET_ALIGN_OFFSET;
    pPkt = (char *)pMblk->mBlkHdr.mData;
    if ((pMblk->mBlkHdr.mData == NULL) || !(pMblk->mBlkHdr.mFlags & M_EXT))
	{
	SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_ERROR, ("simnetPollRcv bad mBlk\n"));
	return (EAGAIN);
	}

    /* try to get a packet */
    
    if (simnetPacketGet (pDrvCtrl, (char *)pPkt, &size) == -1)
	{
	SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_ERROR,
			("simnetPollRcv nothing to read\n"));
    	return EAGAIN;
	}
    pMblk->mBlkHdr.mData = (char *)pPkt;
    pMblk->mBlkHdr.mFlags |= M_PKTHDR;
    pMblk->mBlkHdr.mLen = size;
    pMblk->mBlkPktHdr.len =  pMblk->mBlkHdr.mLen;
    return OK;
    }

/*****************************************************************************
*
* simnetMCastAdd - add a multicast address for the device
*
* RETURNS: OK or ERROR.
*
* NOMANUAL
*/

LOCAL STATUS simnetMCastAdd
    (
    SIMNET_DRV_CTRL *	pDrvCtrl,	/* device pointer */
    char *		pAddress	/* new address to add */
    )
    {
    END_OBJ *	pEnd = &pDrvCtrl->endObj;
    int		retVal;

    if (semTake (pDrvCtrl->simnetDevSemId, WAIT_FOREVER) == ERROR)
    	return (ERROR);
    
    if (!(pEnd->flags & IFF_UP))
    	{
	semGive (pDrvCtrl->simnetDevSemId);
	return (OK);
	}

    retVal = etherMultiAdd (&pDrvCtrl->endObj.multiList, pAddress);

    if (retVal == ENETRESET)
    	{
	SIMNET_DRV_FLAGS_SET(SIMNET_MCAST);
	vxsimHostSimnetMCastModeSet (pDrvCtrl->pNetCfg, TRUE);
	retVal = OK;
	}

    semGive (pDrvCtrl->simnetDevSemId);
    return ((retVal == OK) ? OK : ERROR);
    }

/*****************************************************************************
*
* simnetMCastDel - delete a multicast address for the device
*
* RETURNS: OK or ERROR.
*
* NOMANUAL
*/

LOCAL STATUS simnetMCastDel
    (
    SIMNET_DRV_CTRL *	pDrvCtrl,	/* device pointer */
    char *		pAddress	/* address to be deleted */
    )
    {
    END_OBJ *	pEnd = &pDrvCtrl->endObj;
    int		retVal;
    char	mCastAddr[6];
    MULTI_TABLE	mTable;

    if (semTake (pDrvCtrl->simnetDevSemId, WAIT_FOREVER) == ERROR)
    	return ERROR;

    if (!(pEnd->flags & IFF_UP))
    	{
	semGive (pDrvCtrl->simnetDevSemId);
	return (OK);
	}

    /* remove a multicast address from the list */
    
    retVal = etherMultiDel (&pDrvCtrl->endObj.multiList, pAddress);

    if (retVal == ENETRESET)
    	{
	/* check if there are still multicast address */
	
	mTable.len = 6;
	mTable.pTable = (char *)&mCastAddr;
	etherMultiGet (&pDrvCtrl->endObj.multiList, &mTable);
	if (mTable.len == 0)
	    {
	    SIMNET_DRV_FLAGS_CLR(SIMNET_MCAST);
	    vxsimHostSimnetMCastModeSet (pDrvCtrl->pNetCfg, FALSE);
	    }
	retVal = OK;
	}

    semGive (pDrvCtrl->simnetDevSemId);
    return (retVal);
    }

/*****************************************************************************
*
* simnetMCastGet - get the multicast address list for the device
*
* RETURNS: OK or ERROR.
*
* NOMANUAL
*/

LOCAL STATUS simnetMCastGet
    (
    SIMNET_DRV_CTRL *	pDrvCtrl,	/* device pointer */
    MULTI_TABLE *	pTable		/* address table to be filled in */
    )
    {
    STATUS status;

    if (semTake (pDrvCtrl->simnetDevSemId, WAIT_FOREVER) == ERROR)
    	return ERROR;
    status = etherMultiGet (&pDrvCtrl->endObj.multiList, pTable);
    semGive (pDrvCtrl->simnetDevSemId);
    return (status);
    }

/*******************************************************************************
*
* simnetStop - stop the device
*
* This function calls BSP functions to disconnect interrupts and stop
* the device from operating in interrupt mode.
*
* RETURNS: OK or ERROR.
*
* NOMANUAL
*/

LOCAL STATUS simnetStop
    (
    SIMNET_DRV_CTRL *	pDrvCtrl	/* device to be stopped */
    )
    {
    STATUS		result = OK;
    END_OBJ *		pEnd = &pDrvCtrl->endObj;

    if (semTake (pDrvCtrl->simnetDevSemId, WAIT_FOREVER) == ERROR)
	return (ERROR);

    if (!(pEnd->flags & IFF_UP))
    	{
	semGive (pDrvCtrl->simnetDevSemId);
	return (OK);
	}

    SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_LOAD, ("simnetStop %#x\n", pDrvCtrl));
    END_FLAGS_CLR (&pDrvCtrl->endObj, IFF_UP | IFF_RUNNING);

    /* stop/disable the device: disconnect interrupt */

    result = intDisconnect ((VOIDFUNCPTR *)(long)pDrvCtrl->intId, &simnetInt,
			    (_Vx_usr_arg_t)pDrvCtrl);

    if (result == ERROR)
	{
	SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_ERROR,
			("Could not disconnect interrupt!\n"));
	}

    semGive (pDrvCtrl->simnetDevSemId);
    return (result);
    }

/******************************************************************************
*
* simnetUnload - unload a driver from the system
*
* This function first brings down the device, and then frees any
* stuff that was allocated by the driver in the load function.
*
* RETURNS: OK or ERROR.
*
* NOMANUAL
*/

LOCAL STATUS simnetUnload
    (
    SIMNET_DRV_CTRL *	pDrvCtrl	/* device to be unloaded */
    )
    {
    END_OBJ *	pEnd = &pDrvCtrl->endObj;

    SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_LOAD, ("simnetUnload\n"));

    /* device must be stopped before it can be unloaded */

    if (semTake (pDrvCtrl->simnetDevSemId, WAIT_FOREVER) == ERROR)
	return (ERROR);

    if (pEnd->flags & IFF_UP)
    	{
	semGive (pDrvCtrl->simnetDevSemId);
    	return (ERROR);
	}

    vxsimHostSimnetDetach (&pDrvCtrl->pNetCfg);

    netTupleFree (pDrvCtrl->pErrMblk);
    END_OBJECT_UNLOAD (&pDrvCtrl->endObj);

    semDelete (pDrvCtrl->simnetDevSemId);

    /* Free the memory allocated for mBlks and clBlks */

    if (pDrvCtrl->pMclkArea != NULL)
    	free (pDrvCtrl->pMclkArea);

    /* Free the memory allocated for driver pool structure */

    if (pDrvCtrl->endObj.pNetPool != NULL)
    	free (pDrvCtrl->endObj.pNetPool);
    if (pDrvCtrl->pSendingBuffer != NULL)
    	free (pDrvCtrl->pSendingBuffer);
    if (pDrvCtrl->pPollingBuffer != NULL)
    	free (pDrvCtrl->pPollingBuffer);

    cfree ((char *)pDrvCtrl);
    return (OK);
    }

/*******************************************************************************
*
* simnetPollStart - start polled mode operations
*
* RETURNS: OK or ERROR.
*
* NOMANUAL
*/

LOCAL STATUS simnetPollStart
    (
    SIMNET_DRV_CTRL * pDrvCtrl	/* device to be polled */
    )
    {
    /* turn off interrupts */

    vxsimHostSimnetIntEnable (pDrvCtrl->pNetCfg, pDrvCtrl->intId, FALSE);

    /* set polling flag */

    SIMNET_DRV_FLAGS_SET(SIMNET_POLLING);

    SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_POLL, ("Polling STARTED\n"));

    return (OK);
    }

/*******************************************************************************
*
* simnetPollStop - stop polled mode operations
*
* This function terminates polled mode operation.  The device returns to
* interrupt mode.
*
* The device interrupts are enabled, the current mode flag is switched
* to indicate interrupt mode and the device is then reconfigured for
* interrupt operation.
*
* RETURNS: OK or ERROR.
*
* NOMANUAL
*/

LOCAL STATUS simnetPollStop
    (
    SIMNET_DRV_CTRL * pDrvCtrl	/* device to be polled */
    )
    {
    /* clear polling mode and re-enable interrupts */

    SIMNET_DRV_FLAGS_CLR(SIMNET_POLLING);
    vxsimHostSimnetIntEnable (pDrvCtrl->pNetCfg, pDrvCtrl->intId, TRUE);
    SIMNET_DRV_LOG (SIMNET_DRV_DEBUG_POLL, ("Polling STOPPED\n"));

    return (OK);
    }

#ifdef SIMNET_DRV_DEBUG
/******************************************************************************
*
* simnetPoolShow - show device pool statistics
* 
* This routine requires INCLUDE_NET_SHOW component.
*
* RETURNS: N/A
*
* NOMANUAL
*/

void simnetPoolShow
    (
    int unit
    )
    {
    extern void	netPoolShow (NET_POOL_ID pNetPool);
    SIMNET_DRV_CTRL * pDrvCtrl;
    
    pDrvCtrl = (SIMNET_DRV_CTRL *)endFindByName ("simnet", unit);

    netPoolShow (pDrvCtrl->endObj.pNetPool);
    }

/*****************************************************************************
*
* packetDump - prints packet content
*
* RETURNS: N/A
*
* NOMANUAL
*/

void packetDump  
    (
    unsigned char *	buf,
    int			len,
    int			type
    )
    {
    int ix = 0;

    printf ("%s:", type == 0 ? "rcv" : "sdn");
    for (ix = 0; ix < len; ix++)
    	{
	if ((ix == 0) || (ix % 16) != 0)
	    printf ("%02x ", buf[ix]);
	else 
	    printf ("\n%s:%02x ", type == 0 ? "rcv" : "snd", buf[ix]);
	}
    printf ("\n");
    }
#endif /* SIMNET_DRV_DEBUG */
