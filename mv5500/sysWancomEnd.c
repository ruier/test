/* sysWancomEnd.c - system configuration module for wancomEnd */

/*
 * Copyright 2003-2004, 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2002 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01i,06dec07,y_w  fix the problem: wancom0 doesn't work when changing the value
                 of RAM_LOW_ADRS (WIND00112036)
01h,28jun04,h_k  added cache snoop disabled case in sysWancomEndMallocMemBase.
                 deleted obsolete sysWancomEndFree* functions.
01g,14jun04,h_k  modified for updated wancomEnd.
01f,19may04,h_k  changed the default memSize and the comments for the reception
                 buffer (SPR #95134 and #97010)
01e,01apr04,h_k  changed the parameter string for updated wancomEnd.
                 (SPR #88187)
01d,17nov03,jln  remove "#if 0"
01c,21oct03,jln  include wancomEnd.c; add more CFD/RFDs
01b,28oct02,scb  Fixed minor compiler warnings when pedantic set.
01a,08oct02,efb  Ported from Wind River wancom driver. Added VPD MAC 
                 address retrieval and #defined PHY addresses. Disabled
                 out-of-context code.
*/


/*

DESCRIPTION

This is the WRS-supplied configuration module for the VxWorks
wancomEnd (wancom) END driver.  It performs the dynamic parameterization
of the wancomEnd driver.  This technique of 'just-in-time'
parameterization allows driver parameter values to be declared
as something other than static strings.

*/

/* includes */

#include "vxWorks.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "end.h"
#include "config.h"
#include "taskLib.h"
#include "intLib.h"
#include "sysLib.h"
#include "sysMotVpd.h"
#include "wancomDefs.h"        
#include "drv/end/wancomEnd.h"
#include "private/vmLibP.h"

#undef  WANCOM_MAX_UNITS
#define WANCOM_MAX_UNITS 2         

#undef WANCOMEND_DEBUG   


/* imports */

IMPORT FUNCPTR   wancomEndIntConnect;
IMPORT FUNCPTR   wancomEndIntDisconnect;
IMPORT END_OBJ * wancomEndLoad (char *);

/* defines */

/* unknown values */

#define UNKNOWN         -1

/* wancom driver access routines */

/* typedefs */

/* locals */
LOCAL STATUS sysWancomEndIntEnable(int unit);
LOCAL STATUS sysWancomEndIntDisable(int unit);
LOCAL char * sysWancomEndMallocMemBase (WANCOM_DRV_CTRL *pDrvCtrl);
LOCAL char * sysWancomEndMallocHash (WANCOM_DRV_CTRL *pDrvCtrl);

/* forward declarations */

END_OBJ * sysWancomEndLoad (char *pParamStr, void *unused);
STATUS sysWancomInit (int unit, WANCOM_PORT_INFO *pPort);
LOCAL int       sysWancomEndIntAck     (int unit);
STATUS sysWancomMdioRead (int unit, int reg, UINT16 *data);
STATUS sysWancomMdioWrite (int unit, int reg, UINT16 data);
STATUS sysEnetAddrGet (int unit, UINT8 *adrs);
void sysEnetAddrSet (int unit, 
                     UINT8 byte0, 
                     UINT8 byte1, 
                     UINT8 byte2, 
                     UINT8 byte3, 
                     UINT8 byte4, 
                     UINT8 byte5) ;

IMPORT VPD_PACKET *sysVpdPkts[];      /* board vpd packets */

IMPORT STATUS sysMotVpdPktGet (UCHAR, UINT32, VPD_PACKET **, VPD_PACKET **);

IMPORT FUNCPTR	_func_wancomEndMallocMemBase;
IMPORT FUNCPTR	_func_wancomEndMallocHash;


/***************************************************************************    
*
* sysWancomEndLoad - load wancomEnd () device.
*
* This routine loads the wancom device with initial parameters.
*
* RETURNS: pointer to END object or ERROR.
*
* SEE ALSO: wancomEndLoad()
*/

END_OBJ * sysWancomEndLoad
    (
    char *  pParamStr,  /* ptr to initialization parameter string */
    void *  unused ARG_UNUSED     /* unused optional argument */
    )
    {
    /*
     * The wancomEnd driver END_LOAD_STRING should be:
     * "<memBase>:<memSize>:<nCFDs>:<nRFDs>:<flags>" 
     */

    char *  cp;
    char    paramStr [END_INIT_STR_MAX];   /* from end.h */
    END_OBJ *   pEnd;

    if ( strlen (pParamStr) == 0 )
        {
        /*
         * muxDevLoad() calls us twice.  If the string is
         * zero length, then this is the first time through
         * this routine, so we just return.
         */
        pEnd = wancomEndLoad (pParamStr);
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
        cp = strcpy (paramStr, pParamStr); /* cp points to paramStr */

        /* Now, we advance cp, by finding the end the string */
        cp += strlen (paramStr);

        /*
	 * Finish off the initialization parameter string
	 * The memBase should be equal to ETHER_MEM_START_ADDR and
	 * the memSize is ETHER_PORT_MEM_SIZE. See mv5500A.h.
	 */
        sprintf (cp, "0x%x:0x%x:0x40:0x40:0x00", ETHER_MEM_START_ADDR,\
                 ETHER_PORT_MEM_SIZE);

#ifdef WANCOMEND_DEBUG
        logMsg("paramStr - %s\n",paramStr,2,3,4,5,6);
#endif

        if ( (pEnd = wancomEndLoad (paramStr)) == NULL )
            {
            printf ("Error: device failed wancomEndLoad routine.\n");
            taskDelay(sysClkRateGet()*1);
            }
        }

    return(pEnd);
    }

/**************************************************************************
*
* wancomInit - initialize and configure the GT device.
* This routine makes the appropreate preparation to enable the GT to rceive 
* and transmit ethernet packets using any one of its Ethernet port.
* 
* 1) Set Ethernet port multiplex control.
* 2) Set the Ethernet port various configuration and command words 
*
* RETURNS: OK or ERROR.
*
*/

STATUS sysWancomInit
    (
    int unit, 
    WANCOM_PORT_INFO *pPort
    )
    {
    UINT32 temp ;

#ifdef WANCOMEND_DEBUG
    logMsg("sysWancomInit: unit - 0x%x, pPort - 0x%x\n", unit, pPort, 3,4,5,6);
#endif

    /* Get mac address */
    sysEnetAddrGet(unit, pPort->enetAddr);

    if ( unit == 0 )
        {
        pPort->vector = INT_VEC_ETH0 ;
        pPort->level = INT_LVL_ETH0 ;
        /* Setup phy address on port 0 */
        GT64260_REG_RD(ETH_PHY_ADRS, &temp);
        temp &= ~0x1f;
        temp |= ETH_PHY_ADRS_PORT0;
        GT64260_REG_WR(ETH_PHY_ADRS, temp); 
        }
    else if ( unit == 1 )
        {
        pPort->vector = INT_VEC_ETH1 ;
        pPort->level = INT_LVL_ETH1 ;
        /* Setup phy address on port 1 */
        GT64260_REG_RD(ETH_PHY_ADRS, &temp);
        temp &= ~(0x1f << 5);
        temp |= (ETH_PHY_ADRS_PORT1 << 5); 
        GT64260_REG_WR(ETH_PHY_ADRS, temp);
        }

    pPort->baseAddr = GT64260_BASE_ADRS + PORT0_BASE_OFFSET +
		      (ETHERNET_PORTS_DIFFERENCE_OFFSETS*unit);
    pPort->portConfig =        PORT_CONFIG_VALUE;
    pPort->portConfigExtend =  PORT_CONFIG_EXTEND_VALUE;
    pPort->portSdmaConfig =    PORT_SDMA_CONFIG_VALUE;

#ifdef WANCOMEND_DEBUG
    logMsg("baseAddr - 0x%x\n", pPort->baseAddr,2,3,4,5,6);
    logMsg("portConfig - 0x%x\n", pPort->portConfig,2,3,4,5,6);
    logMsg("portConfigExtend - 0x%x\n", pPort->portConfig,2,3,4,5,6);
    logMsg("portSdmaConfig - 0x%x\n", pPort->portSdmaConfig,2,3,4,5,6);
#endif

    pPort->intEnable     = sysWancomEndIntEnable;
    pPort->intDisable    = sysWancomEndIntDisable;
    pPort->intAck        = sysWancomEndIntAck;

    /*
     * Since the receiption buffer must in cache safe area on mv5500
     * set _func_wancomEndMallocMemBase function bind, allocate memory
     * from non-cacheable area and force to use legacy network buffer
     * management scheme (netPoolInit).
     */

    _func_wancomEndMallocMemBase = (FUNCPTR) sysWancomEndMallocMemBase;

#if (USER_D_CACHE_MODE & CACHE_SNOOP_ENABLE)
#  if defined (INCLUDE_MMU_BASIC) || defined (INCLUDE_MMU_FULL)
    _func_wancomEndMallocHash = (FUNCPTR) sysWancomEndMallocHash;
#  else  /* INCLUDE_MMU_BASIC || INCLUDE_MMU_FULL */
#  error	wancomEnd does not support on the system which cache snoop is enabled and mmu is off.
#  endif /* INCLUDE_MMU_BASIC || INCLUDE_MMU_FULL */
#endif	/* USER_D_CACHE_MODE & CACHE_SNOOP_ENABLE */

    return (OK);
    }


/***************************************************************************    
*
* sysWancomEndIntEnable - enable wancom ethernet interrupts
*
* This routine enables wancom ethernet interrupts.  This may involve operations on
* interrupt control hardware.
*
* RETURNS: OK or ERROR for invalid arguments.
*/

LOCAL STATUS sysWancomEndIntEnable 
    (
    int unit
    )
    {
    int level ;

    if ( unit==0 )
        level = INT_LVL_ETH0 ;
    else if ( unit==1 )
        level = INT_LVL_ETH1 ;
    else
        return ERROR ;

    return(intEnable(level));
    }

/*******************************************************************************
*
* sysWancomIntDisable - disable wancom ethernet interrupts
*
* This routine disables wancom ethernet interrupts.  This may involve operations on
* interrupt control hardware.
*
* RETURNS: OK or ERROR for invalid arguments.
*/

LOCAL STATUS sysWancomEndIntDisable 
    (
    int unit
    )
    {
    int level ;

    if ( unit==0 )
        level = INT_LVL_ETH0 ;
    else if ( unit==1 )
        level = INT_LVL_ETH1 ;
    else
        return ERROR ;

    return(intDisable(level));
    }

/***************************************************************************
*
* sysWancomEndIntAck - acknowledge an wancom ethernet interrupt
*
* This routine performs any wancom ethernet interrupt acknowledge that may be
* required.  This typically involves an operation to some interrupt
* control hardware.
*
* This routine gets called from the wancom driver's interrupt handler.
*
* This routine assumes that the PCI configuration information has already
* been setup.
*
* RETURNS: OK, or ERROR if the interrupt could not be acknowledged.
*/

LOCAL STATUS sysWancomEndIntAck
    (
    int unit
    )
    {
    int level ;
    if ( unit == 0 )
        level = INT_LVL_ETH0 ;
    else
        level = INT_LVL_ETH1 ;

    return(OK);
    }

/***************************************************************************
*
* sysWancomMdioRead - read the LXT972 mii register.
*
* RETURNS: OK, or ERROR.
*/

STATUS sysWancomMdioRead
    (
    int unit, 
    int reg, 
    UINT16 *data
    )
    {
    UINT32 temp ;
    UINT32 phyAdrs=0 ;

    if ( reg > 30 )
        return ERROR ;
    if ( unit < 0 || unit > 1 )
        return ERROR ;

    if ( unit == 0 )
        phyAdrs = ETH_PHY_ADRS_PORT0 ; 
    else if ( unit == 1 )
        phyAdrs = ETH_PHY_ADRS_PORT1 ; 

    temp = 0 ;
    temp = phyAdrs << 16 ;
    temp |= (reg << 21) ;
    temp |= (1 << 26) ; /* read bit */
    GT64260_REG_WR(ETH_SMIR, temp);
    {
        int i ;
        for ( i=0 ; i < 0x10000; i++ ) ;
    }
    while (TRUE)
        {
        GT64260_REG_RD(ETH_SMIR, &temp);
        if ( !(temp & (1 << 27)) )
            continue;
        else
            break;
        }
    *data = (UINT16)temp ;
    return OK ;
    }



/***************************************************************************
*
* sysWancomMdioWrite - write the LXT972 mii register.
*
* RETURNS: OK, or ERROR.
*/

STATUS sysWancomMdioWrite
    (
    int unit,  
    int reg, 
    UINT16 data
    )
    {
    UINT32 temp ;
    UINT32 phyAdrs=0 ;

    if ( reg > 30 )
        return ERROR ;
    if ( unit < 0 || unit > 1 )
        return ERROR ;

    if ( unit == 0 )
        phyAdrs = ETH_PHY_ADRS_PORT0; 
    else if ( unit == 1 )
        phyAdrs = ETH_PHY_ADRS_PORT1; 

    temp = 0 ;
    temp = phyAdrs << 16 ;
    temp |= (reg << 21) ;
    temp |= ((UINT32)data&0xffff) ;
    GT64260_REG_WR(ETH_SMIR, temp);

    return OK ;
    }

/***************************************************************************
*
* sysEnetAddrSet - write MAC address to NVRAM
*
* RETURNS: N/A 
*/

void sysEnetAddrSet 
    (
    int unit ARG_UNUSED,  
    UINT8 byte0 ARG_UNUSED, 
    UINT8 byte1 ARG_UNUSED, 
    UINT8 byte2 ARG_UNUSED, 
    UINT8 byte3 ARG_UNUSED, 
    UINT8 byte4 ARG_UNUSED, 
    UINT8 byte5 ARG_UNUSED
    )
    {
    /* not supported yet */

    return; 

    }

/***************************************************************************
*
* sysEnetAddrGet - get MAC address from NVRAM
*
* RETURNS: OK, or ERROR.
*/

STATUS sysEnetAddrGet
    (
    int unit,
    UINT8 *adrs
    )
    {
    STATUS      status = OK;
    VPD_PACKET *etherVpd = NULL;

    if (sysMotVpdPktGet (VPD_PID_EA, unit, sysVpdPkts, &etherVpd) != ERROR)
        {
        bcopy ((char *)etherVpd->data, (char *)adrs, etherVpd->size);
        }
    else
        {

        /* VPD get failed */

        status = ERROR;
        }

    return (status);
    }

/***************************************************************************
*
* sysWancomEndMallocMemBase - BSP specific memory allocation for descriptors
			      and Rx clusters
*
* RETURNS: allocated address, or NULL if fails
*/

LOCAL char * sysWancomEndMallocMemBase
    (
    WANCOM_DRV_CTRL	*pDrvCtrl
    )
    {
    char * memBase;
#if (USER_D_CACHE_MODE & CACHE_SNOOP_ENABLE)
    UINT32 alignSize;
    UINT32 memSize = pDrvCtrl->memSize + _CACHE_ALIGN_SIZE;

    if ((alignSize = VM_PAGE_SIZE_GET()) == ERROR)
	alignSize = _CACHE_ALIGN_SIZE;

    memBase = memalign (alignSize, ROUND_UP (memSize, alignSize));

    if (memBase == NULL)
	return (NULL);

    VM_STATE_SET (NULL, (void *) memBase,
		  ROUND_UP (memSize, alignSize),
		  VM_STATE_MASK_CACHEABLE, VM_STATE_CACHEABLE_NOT);
#else
    memBase = (char *) cacheDmaMalloc (pDrvCtrl->memSize + _CACHE_ALIGN_SIZE);
#endif	/* USER_D_CACHE_MODE & CACHE_SNOOP_ENABLE */

    return (memBase);
    }

/***************************************************************************
*
* sysWancomEndMallocHash - BSP specific memory allocation for hash table
*
* RETURNS: allocated address, or NULL if fails
*/

LOCAL char * sysWancomEndMallocHash
    (
    WANCOM_DRV_CTRL	*pDrvCtrl
    )
    {
    UINT32 alignSize;
    char * addressTableMalloc;
    UINT32 memSize = (((pDrvCtrl->addressTableHashSize == 0) ?
			EIGHT_K : HALF_K) * MAC_ENTRY_SIZE);

    if ((alignSize = VM_PAGE_SIZE_GET()) == ERROR)
	{
	alignSize = MAC_ENTRY_SIZE;
	}

    addressTableMalloc = memalign (alignSize, ROUND_UP (memSize, alignSize));

    if (addressTableMalloc == NULL)
	return (NULL);

    VM_STATE_SET (NULL, (void *) addressTableMalloc,
		  ROUND_UP (memSize, alignSize),
		  VM_STATE_MASK_CACHEABLE, VM_STATE_CACHEABLE_NOT);

    return (addressTableMalloc);
    }
