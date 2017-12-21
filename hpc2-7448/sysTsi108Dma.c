/* sysTsi108Dma.c - Tsi108 DMA Interface support */

/* Copyright 2005 Tundra Semiconductor Corp. */

/*
modification history
--------------------
01c,31mar06,dtr  Clean up.
01b,06dec05,avb  clean up to meet vxWorks code style requirements
01a,12sep05,avb  written for Tsi108
*/

/*
DESCRIPTION
This module contains routines that provide support for Tsi108 DMA engine.

This module provides support for direct and linked-list ("chained") mode DMA
transfers.

The descriptors for the chained DMA are built in the local memory .

.SH INITIALIZATION
This driver is initialized from the BSP, usually as part of sysHwInit().
The routine tsi108DmaInit() should be called to initialize the  driver .
*/

/* includes */

#include "vxWorks.h"
#include "semLib.h"
#include "sysLib.h"
#include "errno.h"
#include "errnoLib.h"
#include "cacheLib.h"
#include "intLib.h"
#include "memLib.h"
#include "stdlib.h"
#include "tsi108.h"
#include "sysTsi108Dma.h"

/* defines */

/* static file scope locals */

SEM_ID	semDmaCh[MAX_DMA_PORTS];    /* Semaphores for each DMA channel */

/* forward Declarations */


/*******************************************************************************
*
* tsi108DmaInit - initialize DMA engine and library
*
* NOTE: This routine should be called only once, during hardware initialization,
* before using the DMA support functions.
*
* RETURNS: N/A
*/

void tsi108DmaInit (void)
    {
    int i;
    UINT32 regVal;


    for (i = 0; i < MAX_DMA_PORTS; i++)
	{
	/* Clear status bits and disable interupts for each DMA channel. */

	regVal = TSI108_REG_RD(BL_DMA(i), DMA_CHX_GCSR);
	TSI108_REG_WR(BL_DMA(i), DMA_CHX_GCSR, regVal & ~0x3f);

	/* Create semaphore to control access to DMA channel */

	semDmaCh[i] = semBCreate ((INT32)SEM_FULL, SEM_Q_PRIORITY);
	}
    }


/*******************************************************************************
* tsi108MemFill - initializes and starts Memory Fill 
*
* This routine is used for starting Memory Fill DMA operation.
* This is a Direct Mode DMA data transfer.
*
* RETURNS:  OK, or ERROR (with appropriate errno set ) if invalid parameter
* specified by caller.
*/

STATUS tsi108MemFill
    (
    UINT32 	chNum,		    /* Channel Number */
    UINT32 	fillPatternH,	    /* Fill Pattern - Upper 32 bits */
    UINT32 	fillPatternL,	    /* Fill Pattern - Lower 32 bits */
    UCHAR * 	dstAddress,	    /* Destinantion Address */
    UINT32 	dstPortNum,	    /* Destinantion Port Number */
    UINT32 	numOfBytes	    /* Number of Bytes */
    )
    {
    UINT32 regVal;
    UINT32 dmaStat;

    if (chNum < MAX_DMA_PORTS)
        {
	if (numOfBytes != 0) /* number of bytes is non-zero */
            {
            semTake (semDmaCh[chNum], WAIT_FOREVER); /* Take the semaphore */

            /* Wait until the DMA channel completes prev operation */

	    while ((dmaStat = TSI108_REG_RD(BL_DMA(chNum), DMA_CHX_GCSR)) & 
	    			DMA_CHX_GCSR_DACT);

            /* Program the Source Address Registers with Fill Pattern */

	    TSI108_REG_WR(BL_DMA(chNum), DMA_CHX_SRC_ADDR_M, fillPatternH);
	    TSI108_REG_WR(BL_DMA(chNum), DMA_CHX_SRC_ADDR_L, fillPatternL);

            /* Program the Destination Address Registers */

	    if (dstPortNum == TSI108_PORT_PBM)
		TSI108_REG_WR(BL_DMA(chNum), DMA_CHX_DST_ADDR_M, (1 << 30));
	    else
		TSI108_REG_WR(BL_DMA(chNum), DMA_CHX_DST_ADDR_M, 0);

	    TSI108_REG_WR(BL_DMA(chNum),
	    		  DMA_CHX_DST_ADDR_L, (UINT32)dstAddress);

            /* Program the source and destination ports */

	    regVal = TSI108_REG_RD(BL_DMA(chNum), DMA_CHX_TCR1);
	    regVal &= ~(DMA_CHX_TCR1_SRC_PORT | DMA_CHX_TCR1_DST_PORT);
	    regVal |= (dstPortNum << 8) & DMA_CHX_TCR1_DST_PORT;
	    TSI108_REG_WR(BL_DMA(chNum), DMA_CHX_TCR1, regVal);

            /* Program the number of bytes to transfer */

	    regVal = TSI108_REG_RD(BL_DMA(chNum), DMA_CHX_TCR2);
            regVal &=  ~DMA_CHX_TCR2_BC;
            regVal |=  (numOfBytes & DMA_CHX_TCR2_BC);
	    TSI108_REG_WR(BL_DMA(chNum), DMA_CHX_TCR2, regVal);

            /* Set optype MEM_FILL and clear CHAIN and error/status bits */
            dmaStat &= ~(DMA_CHX_GCSR_GO |
			    DMA_CHX_GCSR_CHAIN | DMA_CHX_GCSR_OP_CMD);
	    TSI108_REG_WR(BL_DMA(chNum),
			    DMA_CHX_GCSR, dmaStat | (TSI108_OP_MFILL << 28));

	    /* Read back and issue the GO command */
	    dmaStat = TSI108_REG_RD(BL_DMA(chNum), DMA_CHX_GCSR);

	    TSI108_REG_WR(BL_DMA(chNum),
			DMA_CHX_GCSR, dmaStat | DMA_CHX_GCSR_GO);

            semGive (semDmaCh[chNum]); /* Release the semaphore */
            }
        else
            {
            errnoSet (EINVAL);
            return (ERROR);
            }
        }
    else    /* Invalid Channel Number */
	{
	errnoSet (EINVAL);
	return (ERROR);
	}

    return (OK) ;
    }


/*******************************************************************************
* tsi108DmaDirect - start DMA data transfer in Direct Mode
*
* This routine is used for starting Direct Mode DMA data transfer.
*
* RETURNS:  OK, or ERROR if invalid parameters specified by caller.
*/

STATUS tsi108DmaDirect
    (
    UINT32 	chNum,      /* Channel Number */
    UCHAR * 	srcAddress, /* Source Address */
    UINT32 	srcPortNum, /* Source Port */
    UCHAR * 	dstAddress, /* Destinantion Address */
    UINT32 	dstPortNum, /* Destinantion Port */
    UINT32 	numOfBytes  /* Number of Bytes */
    )
    {
    UINT32 regVal;
    UINT32 dmaStat;

    if (chNum < MAX_DMA_PORTS)
        {

	if (numOfBytes != 0) /* number of bytes is non-zero */
            {
            semTake (semDmaCh[chNum], WAIT_FOREVER); /* Take the semaphore */

            /* Wait until the DMA channel completes prev operation */

	    while ((dmaStat = TSI108_REG_RD(BL_DMA(chNum), DMA_CHX_GCSR)) &
	    		DMA_CHX_GCSR_DACT);

            /* Program the Source Address Registers */

	    if (srcPortNum == TSI108_PORT_PBM)
		TSI108_REG_WR(BL_DMA(chNum), DMA_CHX_SRC_ADDR_M, (1 << 30));
	    else
		TSI108_REG_WR(BL_DMA(chNum), DMA_CHX_SRC_ADDR_M, 0);

	    TSI108_REG_WR(BL_DMA(chNum),
	    		  DMA_CHX_SRC_ADDR_L, (UINT32)srcAddress);

            /* Program the Destination Address Registers */

	    if (dstPortNum == TSI108_PORT_PBM)
		TSI108_REG_WR(BL_DMA(chNum), DMA_CHX_DST_ADDR_M, (1 << 30));
	    else
		TSI108_REG_WR(BL_DMA(chNum), DMA_CHX_DST_ADDR_M, 0);

	    TSI108_REG_WR(BL_DMA(chNum),
	    		  DMA_CHX_DST_ADDR_L, (UINT32)dstAddress);

            /* Program the source and destination ports */

	    regVal = TSI108_REG_RD(BL_DMA(chNum), DMA_CHX_TCR1);
	    regVal &= ~(DMA_CHX_TCR1_SRC_PORT | DMA_CHX_TCR1_DST_PORT);
	    regVal |= ((srcPortNum << 16) & DMA_CHX_TCR1_SRC_PORT) |
		((dstPortNum << 8) & DMA_CHX_TCR1_DST_PORT);

	    TSI108_REG_WR(BL_DMA(chNum), DMA_CHX_TCR1, regVal);

	    /* Program the number of bytes to transfer */

	    regVal = TSI108_REG_RD(BL_DMA(chNum), DMA_CHX_TCR2);
            regVal &=  ~DMA_CHX_TCR2_BC;
            regVal |=  (numOfBytes & DMA_CHX_TCR2_BC);
	    TSI108_REG_WR(BL_DMA(chNum), DMA_CHX_TCR2, regVal);

            /* Set operation type DMA and clear CHAIN and error/status bits */
            dmaStat &= ~(DMA_CHX_GCSR_GO |
			    DMA_CHX_GCSR_CHAIN | DMA_CHX_GCSR_OP_CMD);
	    TSI108_REG_WR(BL_DMA(chNum), DMA_CHX_GCSR,
			    dmaStat | (TSI108_OP_DMA << 28));

	    /* Read back and issue the GO command */
	    dmaStat = TSI108_REG_RD(BL_DMA(chNum), DMA_CHX_GCSR);

	    TSI108_REG_WR(BL_DMA(chNum),
	    		  DMA_CHX_GCSR, dmaStat | DMA_CHX_GCSR_GO);

            semGive (semDmaCh[chNum]); /* Release the semaphore */
            }
        else
            {
            errnoSet (EINVAL);
            return (ERROR);
            }
        }
    else    /* Invalid Channel Number */
	{
	errnoSet (EINVAL);
	return (ERROR);
	}

    return (OK) ;
    }


/*******************************************************************************
* tsi108DmaChain - start DMA data transfer in Chain Mode
*
* This routine is used for starting linked-list (chained) mode DMA transfer.
*
* RETURNS:  OK, or ERROR if invalid parameters are specified.
*/

STATUS tsi108DmaChain
    (
    UINT32 chNum,
    TSI108_DMA_DESCRIPTOR *descAddress,  /* ptr to the first DMA Descriptor */
    UINT32 descPort
    )
    {
    UINT32 regVal;
    UINT32 dmaStat;

    /* NOTE: This library supports DMA descriptors in SDRAM only */
    if ((chNum < MAX_DMA_PORTS) && 
	(descPort == TSI108_PORT_PBM || descPort == TSI108_PORT_SDC)
	)
	{
	semTake (semDmaCh[chNum], WAIT_FOREVER); /* Take the semaphore */

	/* Wait until the DMA channel completes prev operation */

	while ((dmaStat = TSI108_REG_RD(BL_DMA(chNum), DMA_CHX_GCSR)) &
			DMA_CHX_GCSR_DACT);

	/* Program address of the first descriptor in the chain */

	if (descPort == TSI108_PORT_PBM)
	    TSI108_REG_WR(BL_DMA(chNum), DMA_CHX_ND_ADDR_M, (1 << 30));
	else
	    TSI108_REG_WR(BL_DMA(chNum), DMA_CHX_ND_ADDR_M, 0);

	TSI108_REG_WR(BL_DMA(chNum), DMA_CHX_ND_ADDR_L, (UINT32)descAddress);

	/* Set descriptor port number */
	regVal = TSI108_REG_RD(BL_DMA(chNum), DMA_CHX_ND_TCR);
	regVal &= ~(DMA_CHX_ND_TCR_ND_SWAP | DMA_CHX_ND_TCR_ND_PORT);

	TSI108_REG_WR(BL_DMA(chNum), DMA_CHX_ND_TCR,
			regVal | (3 << 30) |
			((descPort << 24) & DMA_CHX_ND_TCR_ND_PORT));

        /* Set byte count to zero */
	regVal = TSI108_REG_RD(BL_DMA(chNum), DMA_CHX_TCR2);
        regVal &= ~DMA_CHX_TCR2_BC;
	TSI108_REG_WR(BL_DMA(chNum), DMA_CHX_TCR2, regVal);

        /* Set OP type DMA CHAINed Transfer and clear err/stat bits */
        dmaStat &= ~(DMA_CHX_GCSR_GO | DMA_CHX_GCSR_OP_CMD);
	TSI108_REG_WR(BL_DMA(chNum), DMA_CHX_GCSR,
			dmaStat | DMA_CHX_GCSR_CHAIN | (TSI108_OP_DMA << 28));

	/* Read back and issue the GO command */
	dmaStat = TSI108_REG_RD(BL_DMA(chNum), DMA_CHX_GCSR);
	TSI108_REG_WR(BL_DMA(chNum), DMA_CHX_GCSR, dmaStat | DMA_CHX_GCSR_GO);
        semGive (semDmaCh[chNum]); /* Release the semaphore */
	}
    else    /* Invalid Channel Number */
	{
	errnoSet (EINVAL);
	return (ERROR);
	}

    return (OK);
    }


/*******************************************************************************
*
* tsi108AddDmaDescriptor - initialize linked-list (chain) of DMA descriptors
*
* This routine is used to initialize chain of DMA descriptors or add
* a new DMA descriptor into an existing chain.
*
* Before a linked-list DMA transfer can be started, a set of descriptors has
* to be setup by calling this routine as many number of times as required
* with the appropriate parameters. The routine tsi108DmaChain() should be
* called after the linked-list is built.
*
* RETURNS:  OK, or ERROR if invalid parameters are specified by caller.
*/

STATUS tsi108AddDmaDescriptor
    (
    UCHAR *	srcAddress,     /* Pointer to  DMA start Address  */
    UINT32 	srcPortNum,     /* Source Port */
    UCHAR *	dstAddress,	/* Pointer to DMA destination address */
    UINT32 	dstPortNum,	/* Destinantion Port */
    UINT32	numOfBytes,     /* Number of bytes to transfer */
    UINT32 	descPort,	/* descriptor port: 2 or 4  */
    TSI108_DMA_DESCRIPTOR **pFirstDesc
    )
    {
    TSI108_DMA_DESCRIPTOR *	pNewDescriptor;
    TSI108_DMA_DESCRIPTOR *	pTempDescriptor;

    /* NOTE: This library supports DMA descriptors in SDRAM only */
    if ((numOfBytes != 0) &&
	(descPort == TSI108_PORT_PBM || descPort == TSI108_PORT_SDC)
	)
	{
	/* allocate aligned memory for new dma descriptor */

	pNewDescriptor = (TSI108_DMA_DESCRIPTOR *)
	    memalign(TSI108_DMA_DESC_ALIGN, sizeof(TSI108_DMA_DESCRIPTOR));

	if (pNewDescriptor == NULL)
	    {
	    errnoSet (ENOMEM);
	    return (ERROR);
	    }
	
	/* Fill DMA descriptor fields */

	pNewDescriptor->srcAddressLow = (UINT32)srcAddress;

	if (srcPortNum == TSI108_PORT_PBM)
	    pNewDescriptor->srcAddressHigh = (1 << 30);
	else
	    pNewDescriptor->srcAddressHigh = 0;
	
	pNewDescriptor->dstAddressLow = (UINT32)dstAddress;
	
	if (dstPortNum == TSI108_PORT_PBM)
	    pNewDescriptor->dstAddressHigh = (1 << 30);
	else
	    pNewDescriptor->dstAddressHigh = 0;
	
	pNewDescriptor->tcr1 = ((srcPortNum << 16) & DMA_CHX_TCR1_SRC_PORT) |
				((dstPortNum << 8) & DMA_CHX_TCR1_DST_PORT);
	pNewDescriptor->tcr2 = numOfBytes & DMA_CHX_TCR2_BC;
	pNewDescriptor->nextDescAddressLow = DMA_CHX_ND_ADDR_L_LAST;
	pNewDescriptor->nextDescAddressHigh = 0;
	pNewDescriptor->nextDescTCR = 0;
	pNewDescriptor->crcAddressLow = 0;
	pNewDescriptor->crcAddressHigh = 0;

	/* Flush the data cache */
	cacheFlush(DATA_CACHE,pNewDescriptor,sizeof(TSI108_DMA_DESCRIPTOR));

	/* Add new DMA descriptor into the chain */
	if (*pFirstDesc == NULL)
	    {
	    /* This is the first BD in the chain: simply report it back */
	    *pFirstDesc = pNewDescriptor;	
	    }
	else
	    {
	    /* Find last descriptor in the existing chain */
	    pTempDescriptor = *pFirstDesc;

            while (
		0 == (pTempDescriptor->nextDescAddressLow &
		      DMA_CHX_ND_ADDR_L_LAST)
		)
                {
                pTempDescriptor = (TSI108_DMA_DESCRIPTOR *)
		    (pTempDescriptor->nextDescAddressLow &
		     DMA_CHX_ND_ADDR_L_ND_ADDR_LB);

                }
	    /* 
	     * Add the new descriptor to the end of chain (clearing LAST bit)
	     */
	    pTempDescriptor->nextDescAddressLow = (UINT32)pNewDescriptor;

	    if (descPort == TSI108_PORT_PBM)
		pTempDescriptor->nextDescAddressHigh = (1 << 30);
	    else
		pTempDescriptor->nextDescAddressHigh = 0;

	    pTempDescriptor->nextDescTCR = (3 << 30) |
	    			((descPort << 24) & DMA_CHX_ND_TCR_ND_PORT);

	    }
	}
    else
        {
        errnoSet (EINVAL);
        return (ERROR);
        }

    return (OK);
    }

/*******************************************************************************
* tsi108FreeDescChain - frees entire chain of DMA descriptors
*
* This routine is used to free a linked-list of DMA descriptors
*
* RETURNS:  OK, or ERROR if invalid parameter(s) is specified.
*/

STATUS tsi108FreeDescChain
    (
    TSI108_DMA_DESCRIPTOR **pFirstDesc
    )
    {
    TSI108_DMA_DESCRIPTOR *pTempDescriptor;
    TSI108_DMA_DESCRIPTOR *pTempFirstDescriptor;

    /* NOTE: This library supports DMA descriptors in SDRAM only */
    
    if (*pFirstDesc != NULL)
	{
	pTempFirstDescriptor = *pFirstDesc;

	while ((void*)((UINT32)pTempFirstDescriptor &
		      DMA_CHX_ND_ADDR_L_ND_ADDR_LB ) != NULL )
	    {
	    pTempDescriptor = pTempFirstDescriptor;

	    pTempFirstDescriptor =
	    (TSI108_DMA_DESCRIPTOR*)pTempFirstDescriptor->nextDescAddressLow;

	    free ((void *)((UINT32)pTempDescriptor & 
		   DMA_CHX_ND_ADDR_L_ND_ADDR_LB));

	    }
	}
    else
        {
        errnoSet (EINVAL);
        return (ERROR);
        }

    return (OK);
    }

/*******************************************************************************
*
* tsi108DmaStatus  -  query DMA status register for specified channel
*
* This  routine purpose is to query the  specified DMA channel for DMA
* transfer status and DMA transfer mode.
*
* RETURNS :
* Contents of GCSR for specified channel
*/

UINT32 tsi108DmaStatus
    (
    ULONG chNum           /* Channel Number */
    )
    {
    UINT32 dmaStat = 0;

    if (chNum < MAX_DMA_PORTS)
        {
	dmaStat = TSI108_REG_RD(BL_DMA(chNum), DMA_CHX_GCSR);
	}
    else
	{
	errnoSet (EINVAL);
	}

    return (dmaStat);
    }
