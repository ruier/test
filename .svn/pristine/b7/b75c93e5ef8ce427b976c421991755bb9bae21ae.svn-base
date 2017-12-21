/* sysMpc8540Dma.c - Support for the MPC8540 DMA Controller */

/*
 * Copyright (c) 2005, 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2005 Motorola, Inc., All Rights Reserved */

/*
modification history
--------------------
01b,05may09,z_l  Fix error in test routines. (WIND00163030)
01a,06mar05,cak  New. Base on sysMv64360Dma.c features (01e,03feb04,mv6100).
*/

/*
DESCRIPTION

MPC8540 DMA driver functions are provided to support initialization, 
configuration, start, pause, resume, abort and status retrieval of DMA 
operations on a per channel basis. Both block mode and chain mode are 
supported.

The DMA initialization routine must be called once before any other DMA
routine is invoked. Failure to do so will result in status error returns
from all other DMA functions. This initialization is handled by sysHwInit2 
whenever INCLUDE_MPC8540_DMA is defined in config.h. 

By default the bandwidth control is disabled for each DMA channel.  The
routine sysMpc8540DmaSetBandwidth() can be used to configure the
bandwidth (arbitration) for each DMA channel independently.

Source and destination address hold is supported, and can be configured
by way of the DMA_ATTRIBUTES structure.

To start DMA for a particular channel, invoke the start routine with
the desired addresses, byte count, transfer attributes, etc. These 
parameters are specified via separate descriptor (list and link) and 
attribute structures. 

DMA is supported between PCI, the Local Bus Controller (LBC), and the
local address space (including DRAM).  Support for the RapidIO interface
is not included. 

User interrupt handlers can be supplied for each of the three types of
completion interrupts: EOSI (end-of-segments interrupt), EOLNI (end-of-links
interrupt), and EOLSI (end-of-lists interrupt), as well as a handler for
error interrupts (transfer error or programming error).  Interrupt handlers, 
if provided, must conform to the rules for standard VxWorks interrupt 
service routines. They are called with a current channel parameter.
Since these routines are called by the driver interrupt handler, they are 
not responsible for clearing interrupts. Interrupts are cleared by the
driver's interrupt service routine (sysMpc8540DmaHandleInt). 

The driver enables all three of the completion interrupt types, regardless
of the mode the channel is running in.  This allows the user to specify
an interrupt handler for each of the interrupt types.  However, it is not
until the DMA has completed (successfully) that the DMA channel's status
is set to OK, versus ERROR.  This is performed by the driver's interrupt
handler when it detects an interrupt signalling the DMA completion.

Interrupt handlers are specified via the DMA_ATTRIBUTES structure.  The
parameters are: userEosiHandler, userEolniHandler, userEolsiHandler, and
userErrorHandler.

The driver can be configured to return only after the DMA transfer has
completed successfully (waitForCompletion in DMA_ATTRIBUTES is set to
TRUE), or to return immediately to the caller (waitForCompletion in 
DMA_ATTRIBUTES is set to FALSE).  If waitForCompletion is set to FALSE
you will need to poll the channel's status to determine when the DMA has
completed.  In order to do this, a DMA_STATUS structure must be passed 
to the driver's start routine.

The routine sysMpc8540ChannelContinueSet() is provided to support the
"Channel Continue Mode for Cascading Transfer Chains" as described in
section 15.4.1.4 of the MPC8540 Reference Manual (Rev. 1, 07/2004).
The routine takes the channel nunmber as its only argument.  This feature
allows the DMA controller to get started on descriptors that have already
been programmed while software continues to build more descriptors in
memory.

.CS
  sysMpc8540ChannelContinueSet
  sysMpc8540DmaAbort
  sysMpc8540DmaInit
  sysMpc8540DmaPause
  sysMpc8540DmaResume 
  sysMpc8540DmaStart 
  sysMpc8540DmaStatus
  sysMpc8540DmaSetBandwidth
.CE

This driver supports four modes of operation for each channel.  The 
mode is determined by the parameters passed to the sysMpc8540DmaStart()
routines in the DMA_ATTRIBUTES, DMA_LIST_DESCRIPTOR, and 
DMA_LINK_DESCRIPTOR structures.

1. Basic Direct

In basic direct mode descriptors are not utilized, and striding is not
available.  To utilize basic direct mode DMA_LIST_DESCRIPTOR must be NULL
and nextDescPtrAddr in the DMA_LINK_DESCRIPTOR must be set to 0x0. 

In basic direct mode DMA completion is indicated with an EOSI interrupt.

2. Extended Direct Mode

In extended direct mode descriptors are not utilized but striding is
available.  To utilize extended direct mode, DMA_LIST_DESCRIPTOR must be
NULL, DMA_ATTRIBUTES must not be NULL, and either sourceStride or
destStride must not be equal to 0x0.

In extended direct mode DMA completion is indicated with an EOSI interrupt.

3. Basic Chain Mode

In basic chain mode descriptors are utilized and striding is not
available.  To utilize basic chain mode, DMA_LIST_DESCRIPTOR must be 
NULL and nextDescPtrAddr in DMA_LINK_DESCRIPTOR must not be 0x0.

In basic chain mode DMA completion is indicated with an EOLNI interrupt.
The completion of each link descriptor will be indicated with an EOSI interrupt.

4. Extended Chain Mode

In extended chain mode both list and link descriptors are utilized and
striding is available.  To utilize extended chain mode DMA_LIST_DESCRIPTOR
must not be NULL.

In extended chain mode DMA completion is indicated with an EOLSI interrupt.
The completion of each link descriptor will be indicated with an EOSI 
interrupt, and the completion of each list descriptor will be indicated
with an EOLNI interrupt.

Note: Single-write start and external control mode are not supported with
this driver.  In all modes, the channel is activated by clearing and setting
MRn[CS].  This is done by the driver itself in the sysMpc8540DmaStart()
routine.

For code examples of all supported features and modes see the routine
sysMpc8540DmaTest() at the end of this file.

The amount of parameter checking in this driver has been kept to a minimum
in order to allow greater flexibility for the application developer.  It
could be fairly easy to pass paramters to the driver, via the link
decriptors, list descriptors, and attribute structure, that in combination
would prove illegal for the DMA controller.  Ensure that parameters meet
device specifications as outlined in the MPC8540 Reference Manual.  Also,
see the above mentioned test routine for examples of legal configurations.

The DMA driver makes use of the BSP's error handling suite to log DMA errors.
To enable the logging of DMA errors it is necessary to #define
INCLUDE_ERROR_HANDLING in config.h.  The DMA controller detects and reports
two types of errors: (1) Transfer error (TE), and (2) Programming errors 
(PE).  See section 15.4.3 in the MPC8540 Reference Manual for a 
discussion of there errors.  If error handling is enabled and an error 
is detected the error will be logged (ie. saved) in the internal error log. 
*/

/* includes */

#include <vxWorks.h>
#include "config.h"
#include "mpc8540Dma.h"

/* defines */

#undef	INCLUDE_MPC8540_DMA_TEST
#define	DMA_LINK_DESC_SIZE	sizeof(DMA_LINK_DESCRIPTOR)
#define DMA_LIST_DESC_SIZE	sizeof(DMA_LIST_DESCRIPTOR)
#define DMA_CHAN_IS_ACTIVE(_X_) (((_X_) & DMA_DGSR_CB_MASK) == \
                                  DMA_DGSR_CB_MASK)

/* typedefs */

/*
 * The following structure defines the control elements for a given DMA
 * channel. The register address offsets are used to conveniently access a
 * particular channel's DMA descriptor and control registers.
 */

typedef struct mpc8540DmaDrvCntl
    {
    UINT32  dmaModeRegOff;		/* offset DMA mode register */
    UINT32  dmaStatusRegOff;		/* offset DMA status register */
    UINT32  dmaCurrLinkDescAddrRegOff;	/* offset DMA current link */
					/* descriptor address register */
    UINT32  dmaSrcAttrRegOff;		/* offset DMA source attributes reg*/
    UINT32  dmaSrcAddrRegOff;		/* offset DMA source address reg */
    UINT32  dmaDestAttrRegOff;		/* offset DMA destination */
					/* attributes register */
    UINT32  dmaDestAddrRegOff;		/* offset DMA destination address reg */
    UINT32  dmaByteCountRegOff;		/* DMA byte count register */
    UINT32  dmaNextLinkDescAddrRegOff;	/* offset DMA next link descriptor */
					/* address register */
    UINT32  dmaCurrListDescAddrRegOff;	/* offset DMA current list */
					/* descriptor address register */
    UINT32  dmaNextListDescAddrRegOff;	/* offset DMA next list descriptor */
					/* address register */
    UINT32  dmaSrcStrideRegOff;		/* offset DMA source stride reg */
    UINT32  dmaDestStrideRegOff;	/* offset DMA destination */
					/* stride register */
    HANDLER userEosiHandler;		/* user supplied EOSI routine */
    HANDLER userEolniHandler;		/* user supplied EOLNI routine */
    HANDLER userEolsiHandler;		/* user supplied EOLSI routine */
    HANDLER userErrorHandler;		/* User supplied Error routine */
    BOOL    waitForCompletion;		/* flag to wait for DMA completion */
    SEM_ID  semDrvId;			/* driver access semaphore */
    SEM_ID  semIntId;			/* interrupt interface semaphore */
    STATUS  dmaStatus;			/* DMA completion status */
    } DMA_DRV_CNTL;

/* globals */

#ifdef INCLUDE_MPC8540_DMA_TEST
    UINT32 numEolni = 0x0;
    UINT32 numEosi = 0x0;
    UINT32 numEolsi = 0x0;
    UINT32 numError = 0x0;
#endif /* INCLUDE_MPC8540_DMA_TEST */

/* locals */

LOCAL DMA_DRV_CNTL mpc8540DmaCntl[DMA_CHANNEL_COUNT];
LOCAL BOOL isInitialized = FALSE;

/* forward declarations */

/* external references */

IMPORT void sysDelay (void);
#ifdef INCLUDE_ERROR_HANDLING
   IMPORT void sysErrorLogShow (ERROR_ID mask, ERROR_ID compare);
#endif /* INCLUDE_ERROR_HANDLING */

/**************************************************************************
*
* sysMpc8540ChannelContinueSet - set the CC bit for DMA channel 
*
* This function will set the CC bit, of the Mode register, for a given
* DMA channel. 
*
* RETURNS: OK if CC is not already set 
* ERROR is returned if the driver is not initialized, or the channel
* is invalid, or the CC is already set 
*/

STATUS sysMpc8540ChannelContinueSet
    (
    UINT32 chan		/* DMA channel */
    )
    {
    DMA_DRV_CNTL *dmaChanCntl = NULL;
    UINT32 modeReg = 0x0;
    STATUS status = OK;

    if (isInitialized && chan < DMA_CHANNEL_COUNT)
        {
        dmaChanCntl = &mpc8540DmaCntl[chan];

        modeReg = CCSR_READ32(CCSBAR,dmaChanCntl->dmaModeRegOff);
	if (modeReg & DMA_MRn_CC_MASK)
	    status = ERROR;
	else
	    {
	    modeReg |= DMA_MRn_CC_MASK;
            CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaModeRegOff,
                              modeReg);
	    }
        }
    else
        {

        /* Driver not initialized or invalid parameter */

        status = ERROR;
        }

    return (status);
    }

/**************************************************************************
*
* sysMpc8540DmaSetBandwidth - set the bandwidth control for DMA channel 
*
* This function allows the bandwidth control parameter (in the mode 
* register) to be set for the given DMA channel. 
*
* RETURNS: OK if bandwidth control changed successfully
* ERROR is returned if the driver is not initialized, or the channel
* is invalid, or the channel is currently active 
*/

STATUS sysMpc8540DmaSetBandwidth
    (
    UINT32 chan,	/* DMA channel */
    UCHAR bandwidth	/* bandwidth value to program */
    )
    {
    DMA_DRV_CNTL *dmaChanCntl = NULL;
    UINT32 modeReg = 0x0;
    UINT32 dgsr = 0x0;
    STATUS status = OK;

    if (isInitialized && chan < DMA_CHANNEL_COUNT)
        {
        dmaChanCntl = &mpc8540DmaCntl[chan];

        semTake (dmaChanCntl->semDrvId, WAIT_FOREVER);

	dgsr = ((CCSR_READ32(CCSBAR,CCSR_DGSR) << (8 * chan)) & DMA_DGSR_MASK);

        if (!(DMA_CHAN_IS_ACTIVE (dgsr)))  
	    {

            modeReg = CCSR_READ32(CCSBAR,dmaChanCntl->dmaModeRegOff);
            modeReg &= ~DMA_MRn_BWC_MASK;
	    modeReg |= (bandwidth << DMA_MRn_BWC_BIT);
            CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaModeRegOff,
                          modeReg);
	    }

        semGive (dmaChanCntl->semDrvId);
        }
    else
        {

        /* Driver not initialized or invalid parameter */

        status = ERROR;
        }

    return (status);
    }

/**************************************************************************
*
* sysMpc8540DmaHandleInt - Interrupt handler for DMA controller 
*
* This function handles all DMA related interrupts for a particular
* channel. This handler is invoked for any interrupt (EOSI, EOLNI, EOLSI,
* TE, and PE) that occurs on any of the 4 DMA channels.  The interrupt is 
* verified to have occurred and the channel status set to OK only upon 
* successful completion. The semaphore is given to the DMA start routine 
* if waitForCompletion is set to TRUE.  If a "userHandler" routine is 
* provided for a given interrupt that handler will be invoked with the 
* channel number.
*
* RETURNS: N/A
*/

LOCAL void sysMpc8540DmaHandleInt 
    (
    UINT32 chan		/* Interrupting channel */
    )
    {
    DMA_DRV_CNTL *dmaChanCntl = NULL;
    UINT32 dgsr = 0x0;
    UINT32 modeReg = 0x0;
    UINT32 interrupt = 0x0;
    UINT32 interruptClear = 0x0;
    UINT32 interruptMask = 0x0;
    UINT32 intMask = (DMA_DGSR_EOLNI_MASK | DMA_DGSR_EOSI_MASK |
		      DMA_DGSR_EOLSI_MASK);
    UINT32 intErrorMask = (DMA_DGSR_TE_MASK | DMA_DGSR_PE_MASK);
#ifdef INCLUDE_ERROR_HANDLING
    ERROR_MSG myErrorMsg;
#endif

    dmaChanCntl = &mpc8540DmaCntl[chan];

    /* Check channel to be sure interrupt actually occurred */

    dgsr = ((CCSR_READ32(CCSBAR,CCSR_DGSR) << (8 * chan)) & DMA_DGSR_MASK);

    if (dgsr & (intMask | intErrorMask))
        {
    
	/* handle one interrupt at a time */

	if (dgsr & DMA_DGSR_EOSI_MASK)
	    {
	    interrupt = DMA_DGSR_EOSI_MASK;
	    interruptClear = DMA_SRn_EOSI_MASK;
	    interruptMask = DMA_MRn_EOSIE_MASK;
	    }
	else if (dgsr & DMA_DGSR_EOLNI_MASK)
	    {
	    interrupt = DMA_DGSR_EOLNI_MASK;
	    interruptClear = DMA_SRn_EOLNI_MASK;
	    interruptMask = DMA_MRn_EOLNIE_MASK;
	    }
	else if (dgsr & DMA_DGSR_EOLSI_MASK)
	    {
	    interrupt = DMA_DGSR_EOLSI_MASK;
	    interruptClear = DMA_SRn_EOLSI_MASK;
	    interruptMask = DMA_MRn_EOLSIE_MASK;
	    }
	else if (dgsr & DMA_DGSR_TE_MASK)
	    {
	    interrupt = DMA_DGSR_TE_MASK;
	    interruptClear = DMA_SRn_TE_MASK;
	    interruptMask = DMA_MRn_EIE_MASK;
	    }
	else if (dgsr & DMA_DGSR_PE_MASK)
	    {
	    interrupt = DMA_DGSR_PE_MASK;
	    interruptClear = DMA_SRn_PE_MASK;
	    interruptMask = DMA_MRn_EIE_MASK;
	    }

	/* Invoke user supplied routines */

	if ((interrupt == DMA_DGSR_EOSI_MASK) && 
	    ((dmaChanCntl->userEosiHandler) != NULL))
	    (dmaChanCntl->userEosiHandler) (chan);

	if ((interrupt == DMA_DGSR_EOLNI_MASK) &&
	    ((dmaChanCntl->userEolniHandler) != NULL))
	    (dmaChanCntl->userEolniHandler) (chan);

	if ((interrupt == DMA_DGSR_EOLSI_MASK) &&
	    ((dmaChanCntl->userEolsiHandler) != NULL))
	    (dmaChanCntl->userEolsiHandler) (chan);

	if ((interrupt == (UINT32)DMA_DGSR_TE_MASK) || 
	     (interrupt == (UINT32)DMA_DGSR_PE_MASK)) 
	    {
#ifdef INCLUDE_ERROR_HANDLING

	    myErrorMsg.id = DMA_ERR;

	    myErrorMsg.params[0] = (CCSR_READ32(CCSBAR,CCSR_DGSR));

	    /* log the error */

	    sysMpc8540ErrorLog(&myErrorMsg);

#endif /* INCLUDE_ERROR_HANDLING */

	    if ((dmaChanCntl->userErrorHandler) != NULL)
	        (dmaChanCntl->userErrorHandler) (chan);
	    }

        /*
         * If configured to wait for DMA completion and the DMA has
	 * completed, or received an error, then give the semaphore to the
	 * waiting DMA start.  Also, if the DMA has completed successfully
	 * then set the DMA channel's status to OK. 
         */ 

	modeReg = CCSR_READ32(CCSBAR,dmaChanCntl->dmaModeRegOff);

	/* 
	 * We need to determine what mode we are running in to 
	 * determine if the DMA is complete (signalled by the correct
	 * interrupt).  The mode can be determined by the current values
	 * of [CTM] and [XFE] in the Mode register for the channel.
	 */

	if ((modeReg & DMA_MRn_CTM_MASK)  || 
	    ((!(modeReg & DMA_MRn_CTM_MASK)) && 
	     (!(modeReg & DMA_MRn_XFE_MASK)) &&
	     (interrupt == DMA_DGSR_EOLNI_MASK)) ||
	    ((!(modeReg & DMA_MRn_CTM_MASK)) &&
	     (modeReg & DMA_MRn_XFE_MASK) &&
	     (interrupt == DMA_DGSR_EOLSI_MASK)) ||
	    (dgsr & intErrorMask))
	    {

	    /* Status is OK as long as we did not receive an error */

	    if (!(dgsr & intErrorMask))
		{
		dmaChanCntl->dmaStatus = OK;
		}

	    /* release semaphore if DMA transfer has completed or error */

	    if (dmaChanCntl->waitForCompletion == TRUE)
		{  
		semGive (dmaChanCntl->semIntId);
		}
	    }

	/* Now, clear the interrupt */

	CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaStatusRegOff, interruptClear); 
	EIEIO_SYNC;
        }
    }

/******************************************************************************
*
* sysMpc8540DmaInit - Initialize DMA and attach DMA Interrupt Handler.
*
* This function configures the DMA driver for all available DMA channels.
* The driver tracks information on each channel independently. Each channel
* control structure is set up to allow easy indexing by channel from the 
* driver routines. Register defaults are also initialized.
*
* An interrupt handler is connected to each DMA interrupt number.
*
* This routine must be called once for DMA capability to be properly 
* initialized before any other driver routine is called. 
*
* RETURNS: OK if initialization succeeded or ERROR if an error occurred.
*/

STATUS sysMpc8540DmaInit (void)
    {
    UINT32 chan = 0;
    DMA_DRV_CNTL *dmaChanCntl = NULL; 
    STATUS status = OK;

    if (!isInitialized)
	{

	/* Initialize the DMA control array for all channels */

	while (chan < DMA_CHANNEL_COUNT && status == OK)
            {
            dmaChanCntl = &mpc8540DmaCntl[chan];
            dmaChanCntl->dmaModeRegOff = CCSR_MR0 + (chan * 0x80);   
            dmaChanCntl->dmaStatusRegOff = CCSR_SR0 + (chan * 0x80);   
            dmaChanCntl->dmaCurrLinkDescAddrRegOff = CCSR_CLNDAR0 + 
		(chan * 0x80);
            dmaChanCntl->dmaSrcAttrRegOff = CCSR_SATR0 + (chan * 0x80);   
            dmaChanCntl->dmaSrcAddrRegOff = CCSR_SAR0 + (chan * 0x80);   
            dmaChanCntl->dmaDestAttrRegOff = CCSR_DATR0 + (chan * 0x80);   
            dmaChanCntl->dmaDestAddrRegOff = CCSR_DAR0 + (chan * 0x80);   
            dmaChanCntl->dmaByteCountRegOff = CCSR_BCR0 + (chan * 0x80);
	    dmaChanCntl->dmaNextLinkDescAddrRegOff = CCSR_NLNDAR0 + 
		(chan * 0x80);
	    dmaChanCntl->dmaCurrListDescAddrRegOff = CCSR_CLSDAR0 + 
		(chan * 0x80);
	    dmaChanCntl->dmaNextListDescAddrRegOff = CCSR_NLSDAR0 + 
		(chan * 0x80);
	    dmaChanCntl->dmaSrcStrideRegOff = CCSR_SSR0 + (chan * 0x80);
	    dmaChanCntl->dmaDestStrideRegOff = CCSR_DSR0 + (chan * 0x80); 
            dmaChanCntl->userEosiHandler = NULL;
	    dmaChanCntl->userEolniHandler = NULL;
	    dmaChanCntl->userEolsiHandler = NULL;
	    dmaChanCntl->userErrorHandler = NULL;
	    dmaChanCntl->waitForCompletion = FALSE;
            dmaChanCntl->semDrvId = semBCreate (SEM_Q_PRIORITY, SEM_FULL);
            dmaChanCntl->semIntId = semBCreate (SEM_Q_FIFO, SEM_EMPTY);

	    /* Disable the Bandwidth Control by default */

	    CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaModeRegOff,
                                  DMA_MRn_BWC_MASK);

            /* Clear the error and status information */

            dmaChanCntl->dmaStatus = ERROR;

            /* Connect the same interrupt handler to each interrupt number */

            status = intConnect ((VOIDFUNCPTR *)
		((int)DMA_CHAN0_COMPLETION_INT_VEC + chan), 
		sysMpc8540DmaHandleInt, (int)chan);

            if (status == OK)
                {
                intEnable (DMA_CHAN0_COMPLETION_INT_VEC + chan);
                }

            ++chan;
            }
        isInitialized = (status == OK);
	}
    return (status);
    }

/******************************************************************************
*
* sysMpc8540DmaStart - Configure and start the DMA controller.
*
* This function sets up the DMA controller for one of the four modes
* discussed above. The user must fill out the DMA link and list descriptors 
* and DMA attribute parameters according to device specifications. 
* See the file header above for details regarding the configuation of
* parameters in order to utilize one of the four modes of operation.
*
* RETURNS: 
* OK is returned if the channel started successfully.
* ERROR is returned if the driver is not initialized, or the 
* channel is invalid, or the dmaLinkDesc and dmaListDesc reference 
* are both null, or the channel is already active and busy.
*/

STATUS sysMpc8540DmaStart
    (
    UINT32 chan,			/* DMA channel to start */
    DMA_LINK_DESCRIPTOR * dmaLinkDesc,	/* Link Descriptor */
    DMA_LIST_DESCRIPTOR * dmaListDesc,	/* List Descriptor */
    DMA_ATTRIBUTES * dmaAttrib,		/* Configuration options */
    DMA_STATUS * dmaStatus		/* DMA status output */
    )
    {
    DMA_DRV_CNTL *dmaChanCntl = NULL;
    UINT32 modeReg = 0;
    STATUS status = OK;
    UINT32 dgsr = 0;

    if (isInitialized && chan < DMA_CHANNEL_COUNT &&
	(dmaLinkDesc != NULL || dmaListDesc != NULL))
        {
        dmaChanCntl = &mpc8540DmaCntl[chan]; 

        semTake (dmaChanCntl->semDrvId, WAIT_FOREVER);
    
        /* Make sure channel is not active */

	dgsr = ((CCSR_READ32(CCSBAR,CCSR_DGSR) << (8 * chan)) & DMA_DGSR_MASK);

        if (!DMA_CHAN_IS_ACTIVE (dgsr) && (dmaLinkDesc != NULL ||
	     dmaListDesc != NULL))  
            {

	    /* Preserve the Bandwidth control setting */

	    modeReg = CCSR_READ32(CCSBAR,dmaChanCntl->dmaModeRegOff);
    	    modeReg &= DMA_MRn_BWC_MASK;

	    /* enable all interrupts */

	    modeReg |= (DMA_MRn_EOSIE_MASK | DMA_MRn_EOLNIE_MASK | 
		       DMA_MRn_EOLSIE_MASK | DMA_MRn_EIE_MASK);

        /* Set the configuration options */

        if (dmaAttrib != NULL)
            {
            dmaChanCntl->userEosiHandler = dmaAttrib->userEosiHandler;
            dmaChanCntl->userEolniHandler = dmaAttrib->userEolniHandler;
            dmaChanCntl->userEolsiHandler = dmaAttrib->userEolsiHandler;
            dmaChanCntl->userErrorHandler = dmaAttrib->userErrorHandler;
            dmaChanCntl->waitForCompletion = dmaAttrib->waitForCompletion;

            if (dmaAttrib->holdSourceAddr)
                {
                modeReg |= DMA_MRn_SAHE_MASK;
                modeReg |= ((dmaAttrib->holdSourceAddrSize & 0x3) <<
                             DMA_MRn_SAHTS_BIT);
                }        
            if (dmaAttrib->holdDestAddr)
                {
                modeReg |= DMA_MRn_DAHE_MASK;
                modeReg |= ((dmaAttrib->holdDestAddrSize & 0x3) <<
                             DMA_MRn_DAHTS_BIT);
                }
            }
        else
            {
            dmaChanCntl->userEosiHandler = NULL;
            dmaChanCntl->userEolniHandler = NULL;
            dmaChanCntl->userEolsiHandler = NULL;
            dmaChanCntl->userErrorHandler = NULL;
            dmaChanCntl->waitForCompletion = FALSE;
            }

        /*
         * Reset the channel's error state. Status is error until 
         * successful completion.
         */

        dmaChanCntl->dmaStatus = ERROR;

	    /* clear any errors reported for this channel */

	    CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaStatusRegOff,
                          (DMA_SRn_TE_MASK | DMA_SRn_PE_MASK));

	    /* Initialize Registers */

	    CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaSrcAttrRegOff,
		     	  0x00000000);
	    CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaSrcAddrRegOff,
		          0x00000000);
	    CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaDestAttrRegOff,
		          0x00000000);
	    CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaDestAddrRegOff,
		          0x00000000);
	    CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaByteCountRegOff,
	      	          0x00000000);
	    CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaSrcStrideRegOff,
	      	          0x00000000);
	    CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaDestStrideRegOff,
	      	          0x00000000);
	    CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaCurrLinkDescAddrRegOff,
			  0x00000000);
	    CCSR_WRITE32(CCSBAR,dmaChanCntl->dmaCurrListDescAddrRegOff,
                          0x00000000); 

	    /* 
	     * Is this Basic Direct, Extended Direct, Basic Chaining, or
	     * Extended Chaining mode?
	     *
	     * Configure the DMA controller according to the mode
	     * that is desired, as specified by the parameters passed
	     * into the start routine.
	     */

	    if (dmaListDesc == NULL)
		{

		/* Basic Direct, Basic Chaining, or Extended Direct? */

		if (dmaAttrib != NULL && 
		    (dmaAttrib->sourceStride != 0x00000000 ||
                     dmaAttrib->destStride != 0x00000000))
		    {
		    
		    /* Entended Direct */

	            CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaSrcAttrRegOff,
			     	  dmaLinkDesc->sourceAttr);
	            CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaSrcAddrRegOff,
			          dmaLinkDesc->sourceAddr);
	            CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaDestAttrRegOff,
			          dmaLinkDesc->destinAttr);
	            CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaDestAddrRegOff,
			          dmaLinkDesc->destinAddr);
	       	    CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaByteCountRegOff,
		      	          dmaLinkDesc->byteCount);
	       	    CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaSrcStrideRegOff,
		      	          dmaAttrib->sourceStride);
	       	    CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaDestStrideRegOff,
		      	          dmaAttrib->destStride);

		    /* set CTM and XFE */

		    modeReg |= (DMA_MRn_CTM_MASK | DMA_MRn_XFE_MASK);
		    CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaModeRegOff,
			          modeReg);
		    }
		else
		    {

		    /* Basic Direct or Basic Chaining? */

	    	    if (dmaLinkDesc->nextDescPtrAddr == 0x0)
		        {

			/* Basic Direct Mode */

	        	CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaSrcAttrRegOff,
			       	      dmaLinkDesc->sourceAttr);
	        	CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaSrcAddrRegOff,
			      	      dmaLinkDesc->sourceAddr);
	        	CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaDestAttrRegOff,
			              dmaLinkDesc->destinAttr);
	        	CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaDestAddrRegOff,
			              dmaLinkDesc->destinAddr);
	        	CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaByteCountRegOff,
			      	      dmaLinkDesc->byteCount);
	       	        CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaSrcStrideRegOff,
		      	              0x00000000);
	       	        CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaDestStrideRegOff,
		      	              0x00000000);

			/* Basic Direct Mode set CTM bit in Mode Register */

			modeReg |= DMA_MRn_CTM_MASK;
			CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaModeRegOff,
				      modeReg);
			}
	    	    else
		        {
	
			/* 
			 * Basic Chain Mode 
			 *
			 * We need only program the Current Link
			 * Descriptor Address Register, the other
			 * registers will be programmed by the controller
			 * based on the values in the link descriptors.
			 */

	        	CCSR_WRITE32(CCSBAR,
				     dmaChanCntl->dmaCurrLinkDescAddrRegOff,
			      	     (UINT32)dmaLinkDesc);
			CCSR_WRITE32(CCSBAR,dmaChanCntl->dmaModeRegOff,
				     modeReg);
			}
		    }
		}
	    else
		{

		/* 
		 * Extended Chaining 
		 *
		 * We need only program the Current List Descriptor
		 * Address register.  The rest of the registers will
		 * automatically be programmed by the controller based
		 * on the values in the list and link descriptors.
		 */

		CCSR_WRITE32(CCSBAR,dmaChanCntl->dmaCurrListDescAddrRegOff,
			     (UINT32)dmaListDesc);

		/* set XFE */

	 	modeReg |= DMA_MRn_XFE_MASK;
		CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaModeRegOff,
			      modeReg);
		}

            /* 
	     * Start the transfer
	     *
	     * The channel is activated by clearing (this is done above)
	     * and setting MRn[CS].  Single-write start mode is not
	     * utilized, and external control mode is not supported.
	     */

	    modeReg = CCSR_READ32(CCSBAR,dmaChanCntl->dmaModeRegOff);
	    modeReg |= DMA_MRn_CS_MASK;		/* set CS bit */
	    CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaModeRegOff,modeReg);

            if (dmaChanCntl->waitForCompletion == TRUE)
                {
                semTake (dmaChanCntl->semIntId, WAIT_FOREVER);

                if (dmaStatus != NULL)
                    {
                    sysMpc8540DmaStatus (chan, dmaStatus);
                    }
                }
            }
        else
            {
            status = ERROR;
            }

        /* Release the device instance semaphore. */

        semGive (dmaChanCntl->semDrvId);
        }
    else
        {
        status = ERROR;
        }
    return (status);
    }

/******************************************************************************
*
* sysMpc8540DmaStatus - Read and return DMA status.
*
* For the given channel, provide the following DMA status information:
*
* DMA Mode Register
* DMA Status Register
* DMA Current Link Descriptor Address Register
* DMA Source Attributes Register
* DMA Source Address Register
* DMA Destination Attributes Register
* DMA Destination Address Register
* DMA Byte Count Register
* DMA Next Link Descriptor Address Register
* DMA Current List Descriptor Address Register
* DMA Next List Descriptor Address Register
* DMA Source Stride Register
* DMA Destination Stride Register
*
* The status information is placed into a DMA status
* structure referenced by dmaStatus along with a DMA status (dmaStatus)
* summary of either OK or ERROR. 
*
* If status is obtained while DMA is in progress, the status
* summary will be ERROR and there will be stale values for the above 
* mentioned registers.  If DMA status is OK, the DMA transfer completed 
* successfully. If status is ERROR and the DMA has stopped, this indicates 
* an error occurred and the DMA was unable to complete.  The state of the 
* registers at the time of the error is captured. 
*
* RETURNS: 
* OK is returned if the dmaStatus is valid.
* ERROR is returned if the driver is not initialized, or the 
* channel is invalid or the dmaStatus reference is null. The 
* dmaStatus contents will not be valid.
*/

STATUS sysMpc8540DmaStatus
    (
    UINT32 chan,		/* Channel to get status */
    DMA_STATUS * dmaStatus	/* Current status of channel */
    )
    {
    DMA_DRV_CNTL * dmaChanCntl = NULL;
    STATUS status = OK;

    if (isInitialized && chan < DMA_CHANNEL_COUNT && dmaStatus != NULL)
        {
        dmaChanCntl = &mpc8540DmaCntl[chan]; 

        dmaStatus->dmaStatus = dmaChanCntl->dmaStatus;

	/* Capture the current state of the channel registers */

	dmaStatus->modeReg = CCSR_READ32(CCSBAR,dmaChanCntl->dmaModeRegOff);
	dmaStatus->statusReg = CCSR_READ32(CCSBAR,dmaChanCntl->dmaStatusRegOff);
	dmaStatus->currLinkDescAddrReg = CCSR_READ32(CCSBAR,
	    dmaChanCntl->dmaCurrLinkDescAddrRegOff);
	dmaStatus->srcAttrReg = CCSR_READ32(CCSBAR,
	    dmaChanCntl->dmaSrcAttrRegOff);
	dmaStatus->srcAddrReg = CCSR_READ32(CCSBAR,
	    dmaChanCntl->dmaSrcAddrRegOff);
	dmaStatus->destAttrReg = CCSR_READ32(CCSBAR,
	    dmaChanCntl->dmaDestAttrRegOff);
	dmaStatus->destAddrReg = CCSR_READ32(CCSBAR,
	    dmaChanCntl->dmaDestAddrRegOff);
	dmaStatus->byteCountReg = CCSR_READ32(CCSBAR,
	    dmaChanCntl->dmaByteCountRegOff);
	dmaStatus->nextLinkDescAddrReg = CCSR_READ32(CCSBAR,
	    dmaChanCntl->dmaNextLinkDescAddrRegOff);
	dmaStatus->currListDescAddrReg = CCSR_READ32(CCSBAR,
	    dmaChanCntl->dmaCurrListDescAddrRegOff);
	dmaStatus->nextListDescAddrReg = CCSR_READ32(CCSBAR,
	    dmaChanCntl->dmaNextListDescAddrRegOff);
	dmaStatus->srcStrideReg = CCSR_READ32(CCSBAR,
	    dmaChanCntl->dmaSrcStrideRegOff);
	dmaStatus->destStrideReg = CCSR_READ32(CCSBAR,
	    dmaChanCntl->dmaDestStrideRegOff);
        }
    else
        {
        status = ERROR;
        }
    return (status);
    }

/******************************************************************************
*
* sysMpc8540DmaAbort - Initiate an abort of the current DMA operation.
*
* For the given valid channel, if the channel is active, the channel's
* abort bit is set in its mode register. The function waits for the
* abort to complete before returning.
*
* RETURNS:
* OK is returned if abort succeeded.
* ERROR is returned if the driver is not initialized or the
* channel number is invalid or should the abort fail.
*/

STATUS sysMpc8540DmaAbort
    (
    UINT32   chan		/* The active DMA channel to abort */
    )
    {
    DMA_DRV_CNTL *dmaChanCntl = NULL;
    UINT32 modeReg = 0x0;
    UINT32 dgsr = 0x0;
    STATUS status = OK;
    UINT32 msWait = 10;         /* millisecond wait for abort time */

    if (isInitialized && chan < DMA_CHANNEL_COUNT)
        {
        dmaChanCntl = &mpc8540DmaCntl[chan];

        semTake (dmaChanCntl->semDrvId, WAIT_FOREVER);

	dgsr = ((CCSR_READ32(CCSBAR,CCSR_DGSR) << (8 * chan)) & DMA_DGSR_MASK);

        if (DMA_CHAN_IS_ACTIVE (dgsr))
            {

	    /* DMA channel is active */

	    /* Abort the transfer by setting MRn[CA] */

	    modeReg = CCSR_READ32(CCSBAR,dmaChanCntl->dmaModeRegOff);
	    modeReg |= DMA_MRn_CA_MASK;
	    CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaModeRegOff,
                          modeReg);

            /* 
	     * Wait for abort to complete 
	     *
	     * An abort is signalled by MRn[CA] = 1 and DGSR[CB] = 0 
	     */

            while ((!((CCSR_READ32(CCSBAR,dmaChanCntl->dmaModeRegOff) & 
		       DMA_MRn_CA_MASK) && (!(CCSR_READ32(CCSBAR,CCSR_DGSR) & 
					      DMA_DGSR_CB_MASK))))
                   && msWait > 0)
                {
                sysDelay ();
                --msWait;
                }

            if (msWait == 0)
                {

                /*
                 * Channel became inactive prior to abort command or
                 * abort could not complete.
                 */

                status = ERROR;
                }
            }
        else
            {

            /* Channel not active */

            status = ERROR;
            }

        semGive (dmaChanCntl->semDrvId);
        }
    else
        {

        /* Driver not initialized or invalid parameter */

        status = ERROR;
        }

    return (status);
    }

/******************************************************************************
*
* sysMpc8540DmaPause - Initiate a pause of the current DMA operation.
*
* For the given valid channel, if the channel is busy, clear MRn[CS] 
* to pause the channel. Monitor SRn[CH] to ensure the channel has been
* paused. 
*
* RETURNS: 
* OK is returned if pause succeeded.
* ERROR is returned if the driver is not initialized, or the channel
* is invalid, or the channel was not active.
*/

STATUS sysMpc8540DmaPause
    (
    UINT32   chan		/* Channel to be paused */
    )
    {
    DMA_DRV_CNTL *dmaChanCntl = NULL;
    UINT32 dgsr = 0x0;
    UINT32 modeReg = 0x0;
    STATUS status = OK;
    UINT32 msWait = 10;         /* millisecond wait for abort time */

    if (isInitialized && chan < DMA_CHANNEL_COUNT)
        {
        dmaChanCntl = &mpc8540DmaCntl[chan];

        semTake (dmaChanCntl->semDrvId, WAIT_FOREVER);

	dgsr = ((CCSR_READ32(CCSBAR,CCSR_DGSR) << (8 * chan)) & DMA_DGSR_MASK);

        if ((DMA_CHAN_IS_ACTIVE (dgsr)) && 
	    (!(CCSR_READ32(CCSBAR,dmaChanCntl->dmaModeRegOff) & 
	       DMA_MRn_CTM_MASK)))
	    {

	    /* 
	     * If the channel is active and we are in chain mode, 
	     * pause the channel by clearing MRn[Cs]. 
	     */

            modeReg = CCSR_READ32(CCSBAR,dmaChanCntl->dmaModeRegOff);
            modeReg &= ~DMA_MRn_CS_MASK;
            CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaModeRegOff,
                          modeReg);


            /* Wait for channel to pause */

            while (!(CCSR_READ32(CCSBAR,CCSR_DGSR) & DMA_DGSR_CH_MASK) &&
                   msWait > 0)
                {
                sysDelay ();
                --msWait;
                }

            if (msWait == 0)
                {

                /* Channel did not become inactive */

                status = ERROR;
                }
            }
        else
            {

            /* Channel already stopped */

            status = ERROR;
            }

        semGive (dmaChanCntl->semDrvId);
        }
    else
        {

        /* Driver not initialized or invalid parameter */

        status = ERROR;
        }

    return (status);
    }

/******************************************************************************
*
* sysMpc8540DmaResume - Resume a previously paused DMA operation.
*
* For a given valid channel, verify that the channel is not active and that
* there are bytes remaining to transfer (a non-zero byte count). If so, 
* set MRn[CS]. The channel will continue the transfer.
*
* RETURNS: 
* OK is returned if the resume succeeded.
* ERROR is returned if the driver is not initialized, or the channel
* is invalid, or the channel is not paused (i.e. inactive with a 
* a non-zero byte count descriptor register).
*/

STATUS sysMpc8540DmaResume
    (
    UINT32   chan		/* Channel to be resumed */
    )
    {
    DMA_DRV_CNTL *dmaChanCntl = NULL;
    UINT32 dgsr = 0x0;
    UINT32 modeReg = 0x0;
    UINT32 byteCount = 0;
    STATUS status = OK;

    if (isInitialized && chan < DMA_CHANNEL_COUNT)
        {
        dmaChanCntl = &mpc8540DmaCntl[chan]; 
 
        semTake (dmaChanCntl->semDrvId, WAIT_FOREVER);
 
	dgsr = ((CCSR_READ32(CCSBAR,CCSR_DGSR) << (8 * chan)) & DMA_DGSR_MASK);
        modeReg = CCSR_READ32(CCSBAR,dmaChanCntl->dmaModeRegOff);
        byteCount = CCSR_READ32(CCSBAR, dmaChanCntl->dmaByteCountRegOff);

	if ((!(modeReg & DMA_MRn_CS_MASK)) && (dgsr & DMA_DGSR_CB_MASK) &&
	    (byteCount > 0))
            {

	    /* 
	     * If the channel is halted and there are bytes left 
	     * to transfer then resume the transfer.
	     */

	    modeReg |= DMA_MRn_CS_MASK;
            CCSR_WRITE32 (CCSBAR,dmaChanCntl->dmaModeRegOff,
                          modeReg);
            }
        else
            {
            status = ERROR;
            }

        semGive (dmaChanCntl->semDrvId);
        }
    else
        {
        status = ERROR;
        }

    return (status);
    }

#ifdef INCLUDE_MPC8540_DMA_TEST
/******************************************************************************
*
* userEosiHandleInt - handler to test "user-supplied" handlers
*
* This routine is used to test "user-supplied" interrupt handlers.
*
* RETURNS: N/A
*/

LOCAL void userEosiHandleInt
    (
    int chan    /* channel number */
    )
    {

    /* increment counter */

    numEosi += 1;
    }

/******************************************************************************
*
* userEolniHandleInt - handler to test "user-supplied" handlers
*
* This routine is used to test "user-supplied" interrupt handlers.
*
* RETURNS: N/A
*/

LOCAL void userEolniHandleInt
    (
    int chan    /* channel number */
    )
    {

    /* increment counter */

    numEolni += 1;
    }

/******************************************************************************
*
* userEolsiHandleInt - handler to test "user-supplied" handlers
*
* This routine is used to test "user-supplied" interrupt handlers.
*
* RETURNS: N/A
*/

LOCAL void userEolsiHandleInt
    (
    int chan    /* channel number */
    )
    {

    /* increment counter */

    numEolsi += 1;
    }

/******************************************************************************
*
* userErrorHandleInt - handler to test "user-supplied" handlers
*
* This routine is used to test "user-supplied" interrupt handlers.
*
* RETURNS: N/A
*/

LOCAL void userErrorHandleInt
    (
    int chan    /* channel number */
    )
    {

    /* increment counter */

    numError += 1;
    }

/**************************************************************************
*
* displayStatus - display the contents of a DMA_STATUS structure
*
* This routine is used to display the contents of a DMA_STATUS structure
* with a standard format.
*
* RETURNS: N/A
*/

void displayStatus
    (
    DMA_STATUS * dmaStatus
    )
    {
    printf("  Status = %08x\n\r", dmaStatus->dmaStatus);
    printf("  MRn      SRn      CLNDARn  SATRn    SARn     DATRn    DARn      \n\r");
    printf("  %08x %08x %08x %08x %08x %08x %08x \n\r", dmaStatus->modeReg, dmaStatus->statusReg, dmaStatus->currLinkDescAddrReg, dmaStatus->srcAttrReg, dmaStatus->srcAddrReg, dmaStatus->destAttrReg, dmaStatus->destAddrReg);
    printf("  BCRn     NLNDARn  CLSDARn  NLSDARn  SSRn     DSRn    \n\r");
    printf("  %08x %08x %08x %08x %08x %08x \n\r", dmaStatus->byteCountReg, dmaStatus->nextLinkDescAddrReg, dmaStatus->currListDescAddrReg, dmaStatus->nextListDescAddrReg, dmaStatus->srcStrideReg, dmaStatus->destStrideReg);
    printf("\n\r");
    }

/**************************************************************************
*
* dataFill - fill source buffer with a known pattern
*
* This routine is used to fill the DMA source buffer with a known pattern.
*
* RETURNS: N/A
*/

void dataFill
    (
    UCHAR *buffAddr,    /* address of buffer */
    UINT32 count        /* size of buffer (number of bytes) */
    )
    {
    UINT32 x;

    for (x = 0 ; x < count; ++x)
        {
        *buffAddr = (UCHAR)((x + 0xee) & 0xFF);
        ++buffAddr;
        }
    }

/**************************************************************************
*
* dataCompare -  compare buffers when striding not enabled
*
* This routine is used to compare buffers when striding is not enabled.
*
* RETURNS: OK, if comparison succeeded, ERROR otherwise
*/

STATUS dataCompare
    (
    UCHAR *expAddr,        /* Expected values */
    UCHAR *compAddr,       /* Compare values */
    UINT32 count           /* Number of bytes to compare */
    )
    {
    BOOL mismatch = FALSE;

    while (!mismatch && count > 0 )
        {
        mismatch = (*expAddr != *compAddr);

        expAddr += 1;
        compAddr += 1;

        count -= 1;
        }

    return (mismatch ? ERROR : OK);
    }

/**************************************************************************
*
* holdDataCompare - compare buffers when source/destination holding enabled
*
* This is a special routine that will compare data buffers when source
* or destination holding is enabled to ensure that they are as they should 
* be.
*
* RETURNS: OK, if comparison succeeded, ERROR otherwise
*/

STATUS holdDataCompare
    (
    BOOL holdSrcAddr,
    BOOL holdDestAddr,
    UCHAR holdSrcAddrSize,
    UCHAR holdDestAddrSize,
    UINT32 byteCount,
    UCHAR * srcBuffer,
    UCHAR * destBuffer
    )
    {
    STATUS match = OK;
    UCHAR * holdSrcPtr;
    UCHAR * holdDestPtr;
    int x;
    int y;
    int z;
    UCHAR * tempSrcPtr;
    UCHAR * tempDestPtr;

    x = 1;
    y = 1;
    holdSrcPtr = tempSrcPtr = srcBuffer;
    holdDestPtr = tempDestPtr = destBuffer;

    if (holdDestAddr)
        {
        holdDestPtr = destBuffer + holdDestAddrSize;
        for (z = 0; z < byteCount - holdDestAddrSize; z++)
            {
            match = (*holdDestPtr != 0x00000000);
            holdDestPtr += 1;
            }
        }
    else if (holdSrcAddr)
	{
	for (z = 0; z < byteCount; z++)
            {

            /* compare a byte */

            match = dataCompare(tempSrcPtr, tempDestPtr, 1);

            /* increment buffer pointers */

            tempDestPtr += 1;

            if (holdSrcAddr == TRUE)
                {
                if (y != holdSrcAddrSize)
                    {
                    tempSrcPtr += 1;
                    y++;
                    }
                else
                    {
                    tempSrcPtr = srcBuffer;
                    holdSrcPtr = tempSrcPtr;
                    y = 1;
                    }
                }
            else
                tempSrcPtr += 1;
            }
	}

    return (match);
    }
/**************************************************************************
*
* strideDataCompare - compare buffers when striding enabled
*
* This is a special routine that will compare data buffers when striding
* is available to ensure that they are as they should be.
*
* RETURNS: OK, if comparison succeeded, ERROR otherwise
*/

STATUS strideDataCompare
    (
    UINT32 destStrideSize,
    UINT32 destStrideDist,
    UINT32 srcStrideSize,
    UINT32 srcStrideDist,
    UINT32 byteCount,
    UCHAR * srcBuffer,
    UCHAR * destBuffer
    )
    {
    STATUS match = OK;
    BOOL destStride;
    BOOL srcStride;
    UCHAR * strideSrcPtr;
    UCHAR * strideDestPtr;
    int x;
    int y;
    int z;
    UCHAR * tempSrcPtr;
    UCHAR * tempDestPtr;

    destStride = ((destStrideSize != 0x0) || (destStrideDist != 0x0));
    srcStride = ((srcStrideSize != 0x0) || (srcStrideDist != 0x0));

    x = 1;
    y = 1;
    strideSrcPtr = tempSrcPtr = srcBuffer;
    strideDestPtr = tempDestPtr = destBuffer;

    for (z = 0; z < byteCount; z++)
        {

        /* compare a byte */

        match = dataCompare(tempSrcPtr, tempDestPtr, 1);

        /* increment buffer pointers */

        if (destStride)
            {
            if (x != destStrideSize)
                {
                tempDestPtr += 1;
                x++;
                }
            else
                {
                tempDestPtr = strideDestPtr + destStrideDist;
                strideDestPtr = tempDestPtr;
                x = 1;
                }
            }
        else
            tempDestPtr += 1;

        if (srcStride)
            {
            if (y != srcStrideSize)
                {
                tempSrcPtr += 1;
                y++;
                }
            else
                {
                tempSrcPtr = strideSrcPtr + srcStrideDist;
                strideSrcPtr = tempSrcPtr;
                y = 1;
                }
            }
        else
            tempSrcPtr += 1;
        }
    return (match);
    }

/******************************************************************************
*
* sysMpc8540DmaTest - Test for the MPC8540 DMA driver
*
* This test is used to verify the functionality of the MPC8540 DMA
* driver in all of its modes.  The tests are performed using the onboard
* DRAM only.
*
* RETURNS: OK, if all tests passed, ERROR otherwise
*/

STATUS sysMpc8540DmaTest(void)
    {
    UINT32 * buffer1;
    UINT32 * buffer2;
    UINT32 * buffer3;
    UINT32 * buffer4;
    UINT32 * tempBuf1;
    UINT32 * tempBuf2;
    DMA_LINK_DESCRIPTOR * descPtr;
    DMA_LINK_DESCRIPTOR * midPointPtr;
    DMA_LINK_DESCRIPTOR * descPtrChain;
    DMA_LIST_DESCRIPTOR * descPtrList;
    DMA_LINK_DESCRIPTOR * tempPtr;
    DMA_LIST_DESCRIPTOR * tempListPtr;
    DMA_STATUS dmaStatus;
    DMA_ATTRIBUTES dmaAttrib;
    DMA_DRV_CNTL *dmaChanCntl = NULL;
    int x;
    int y;
    UINT32 strideSize = 0x004;
    UINT32 strideDistance = 0x008;
    UINT32 byteCount = 0x1000;          /* 4k */
    STATUS passed = OK;
    int chan;

    /* allocate test buffers */

    buffer1 = (UINT32*)memalign(0x20,byteCount);
    buffer2 = (UINT32*)memalign(0x20,byteCount);
    descPtr = (DMA_LINK_DESCRIPTOR*)memalign(DMA_LINK_DESC_SIZE,
               DMA_LINK_DESC_SIZE);
    buffer3 = (UINT32*)memalign(0x20,8*byteCount);
    buffer4 = (UINT32*)memalign(0x20,8*byteCount);
    descPtrChain = (DMA_LINK_DESCRIPTOR*)memalign(DMA_LINK_DESC_SIZE,
                    8 * DMA_LINK_DESC_SIZE);
    descPtrList = (DMA_LIST_DESCRIPTOR*)memalign(DMA_LIST_DESC_SIZE,
                   2 * DMA_LIST_DESC_SIZE);

    printf("General Test Configuration Parameters:\n\r");
    printf(" Direct mode source buffer address........0x%08x\n\r", (UINT32)buffer1);
    printf(" Direct mode destination buffer address...0x%08x\n\r", (UINT32)buffer2);
    printf(" Chain mode source buffer address.........0x%08x\n\r", (UINT32)buffer3);
    printf(" Chain mode destination buffer address....0x%08x\n\r", (UINT32)buffer4);
    printf("\n\r");

    for (y = 0; y < 2; y++)
        {
        memset(&dmaAttrib, 0, sizeof(DMA_ATTRIBUTES));
        if (y == 0)
            dmaAttrib.waitForCompletion = TRUE;
        else
            dmaAttrib.waitForCompletion = FALSE;

    for (chan = 0; chan < DMA_CHANNEL_COUNT; chan++)
        {
        printf("Channel %d...\n\r", chan);

        /* Test DMA driver in Basic Direct Mode */

        printf(" Basic Direct Mode......................................");

        dataFill ((UCHAR *)buffer1,byteCount);
        memset(buffer2,0x0,byteCount);   /* clear destination buffer */
        memset(descPtr,0x0,DMA_LINK_DESC_SIZE);

        descPtr->sourceAttr = 0x00050000;       /* snoop local processor */
        descPtr->sourceAddr = (UINT32)buffer1;
        descPtr->destinAttr = 0x00050000;       /* snoop local processor */
        descPtr->destinAddr = (UINT32)buffer2;
        descPtr->nextDescPtrAddr = 0x00000000;
        descPtr->byteCount = byteCount;

        dmaAttrib.sourceStride = 0x00000000;
        dmaAttrib.destStride = 0x00000000;
        dmaAttrib.userEosiHandler = (HANDLER)userEosiHandleInt;
        dmaAttrib.userEolniHandler = (HANDLER)userEolniHandleInt;
        dmaAttrib.userEolsiHandler = (HANDLER)userEolsiHandleInt;
        dmaAttrib.userErrorHandler = (HANDLER)userErrorHandleInt;

        /* start the DMA - Basic Direct Mode */

        numEosi = 0x0;
        numEolni = 0x0;
        numEolsi = 0x0;
        numError = 0x0;

        sysMpc8540DmaStart(chan,descPtr,NULL,&dmaAttrib,&dmaStatus);

        if (dmaAttrib.waitForCompletion == FALSE)
            {
    	    sysMpc8540DmaStatus(chan, &dmaStatus);
            while (dmaStatus.dmaStatus != OK)
                {
                sysDelay();
                sysMpc8540DmaStatus(chan, &dmaStatus);
                }
            }

        if ((dataCompare((UCHAR *)buffer1,(UCHAR *)buffer2,byteCount) == OK) 
	     && (numEosi == 1) && (numEolni == 0x0) && (numEolsi == 0x0) && 
	     (numError == 0x0))
            printf("PASS\n\r");
        else
            {
            printf("FAIL\n\r");
            passed = ERROR;
            }

        printf(" No link or list descriptors.\n\r");
        printf(" Total bytes transferred = 0x%08x\n\r", byteCount);
        if (dmaAttrib.waitForCompletion == TRUE)
            printf(" Wait for DMA completion.\n\r");
        else
            printf(" Poll for DMA completion.\n\r");

        numEosi = 0x0;
        numEolni = 0x0;
        numEolsi = 0x0;
        numError = 0x0;

        displayStatus(&dmaStatus);

        /* Test Extended Direct (Striding Capability) */

        printf(" Extended Direct Mode (destination striding)............");

        memset(buffer2,0x0,byteCount);   /* clear destination buffer */
        memset(descPtr,0x0,DMA_LINK_DESC_SIZE);  /* Clear descriptor */

        descPtr->sourceAttr = 0x00050000;
        descPtr->sourceAddr = (UINT32)buffer1;
        descPtr->destinAttr = 0x01050000;
        descPtr->destinAddr = (UINT32)buffer2;
        descPtr->nextDescPtrAddr = 0x00000000;
        descPtr->byteCount = (byteCount / (strideDistance/strideSize));

        dmaAttrib.sourceStride = 0x00000000;   /* no source striding */
        dmaAttrib.destStride = ((strideSize << DMA_DSRn_DSS_BIT) |
                                 (strideDistance << DMA_DSRn_DSD_BIT));
        dmaAttrib.userEosiHandler = (HANDLER)userEosiHandleInt;
        dmaAttrib.userEolniHandler = (HANDLER)userEolniHandleInt;
        dmaAttrib.userEolsiHandler = (HANDLER)userEolsiHandleInt;
        dmaAttrib.userErrorHandler = (HANDLER)userErrorHandleInt;

        /* start the DMA - Extended Direct */

        sysMpc8540DmaStart(chan,descPtr,NULL,&dmaAttrib,&dmaStatus);

        if (dmaAttrib.waitForCompletion == FALSE)
            {
    	    sysMpc8540DmaStatus(chan, &dmaStatus);
            while (dmaStatus.dmaStatus != OK)
                {
                sysDelay();
                sysMpc8540DmaStatus(chan, &dmaStatus);
                }
            }

        if ((strideDataCompare(strideSize,strideDistance,0x0,0x0,
            (byteCount / (strideDistance/strideSize)),(UCHAR *)buffer1,
	    (UCHAR *)buffer2) == OK) && (numEosi == 0x1) && 
	    (numEolni == 0x0) && (numEolsi == 0x0) && (numError == 0x0))
            printf("PASS\n\r");
        else
            {
            printf("FAIL\n\r");
            passed = ERROR;
            }

        printf(" No link or list descriptors.\n\r");
        printf(" Total bytes transferred = 0x%08x\n\r", byteCount/2);
        if (dmaAttrib.waitForCompletion == TRUE)
            printf(" Wait for DMA completion.\n\r");
        else
            printf(" Poll for DMA completion.\n\r");
        printf(" Destination stride size = 0x%08x\n\r", strideSize);
        printf(" Destination stride size = 0x%08x\n\r", strideDistance);

        numEosi = 0x0;
        numEolni = 0x0;
        numEolsi = 0x0;
        numError = 0x0;

        displayStatus(&dmaStatus);

        /* Test Extended Direct (Striding Capability) */

        printf(" Extended Direct Mode (source striding).................");

        memset(buffer2,0x0,byteCount);   /* clear destination buffer */
        memset(descPtr,0x0,DMA_LINK_DESC_SIZE);  /* Clear descriptor */

        descPtr->sourceAttr = 0x01050000;
        descPtr->sourceAddr = (UINT32)buffer1;
        descPtr->destinAttr = 0x00050000;
        descPtr->destinAddr = (UINT32)buffer2;
        descPtr->nextDescPtrAddr = 0x00000000;
        descPtr->byteCount = (byteCount / (strideDistance/strideSize));

        dmaAttrib.sourceStride = ((strideSize << DMA_SSRn_SSS_BIT) |
                                   (strideDistance << DMA_SSRn_SSD_BIT));
        dmaAttrib.destStride = 0x00000000;     /* no destination striding */
        dmaAttrib.userEosiHandler = (HANDLER)userEosiHandleInt;
        dmaAttrib.userEolniHandler = (HANDLER)userEolniHandleInt;
        dmaAttrib.userEolsiHandler = (HANDLER)userEolsiHandleInt;
        dmaAttrib.userErrorHandler = (HANDLER)userErrorHandleInt;

        /* start the DMA - Extended Direct */

        sysMpc8540DmaStart(chan,descPtr,NULL,&dmaAttrib,&dmaStatus);

        if (dmaAttrib.waitForCompletion == FALSE)
            {
    	    sysMpc8540DmaStatus(chan, &dmaStatus);
            while (dmaStatus.dmaStatus != OK)
                {
                sysDelay();
                sysMpc8540DmaStatus(chan, &dmaStatus);
                }
            }

        if ((strideDataCompare(0x0,0x0,strideSize,strideDistance,
            (byteCount / (strideDistance/strideSize)),(UCHAR *)buffer1,
	    (UCHAR *)buffer2) == OK) && (numEosi == 0x1) && 
	    (numEolni == 0x0)  && (numEolsi == 0x0) && (numError == 0x0))
            printf("PASS\n\r");
        else
            {
            printf("FAIL\n\r");
            passed = ERROR;
            }

        printf(" No link or list descriptors.\n\r");
        printf(" Total bytes transferred = 0x%08x\n\r", byteCount/2);
        if (dmaAttrib.waitForCompletion == TRUE)
            printf(" Wait for DMA completion.\n\r");
        else
            printf(" Poll for DMA completion.\n\r");
        printf(" Source stride size = 0x%08x\n\r", strideSize);
        printf(" Source stride size = 0x%08x\n\r", strideDistance);

        numEosi = 0x0;
        numEolni = 0x0;
        numEolsi = 0x0;
        numError = 0x0;

        displayStatus(&dmaStatus);

        /* Test Extended Direct (Striding Capability) */

        printf(" Extended Direct Mode (source & destination striding)...");

        memset(buffer2,0x0,byteCount);   /* clear destination buffer */
        memset(descPtr,0x0,DMA_LINK_DESC_SIZE);  /* Clear descriptor */

        descPtr->sourceAttr = 0x01050000;
        descPtr->sourceAddr = (UINT32)buffer1;
        descPtr->destinAttr = 0x01050000;
        descPtr->destinAddr = (UINT32)buffer2;
        descPtr->nextDescPtrAddr = 0x00000000;
        descPtr->byteCount = (byteCount / (strideDistance/strideSize));

        dmaAttrib.sourceStride = ((strideSize << DMA_SSRn_SSS_BIT) |
                                   (strideDistance << DMA_SSRn_SSD_BIT));
        dmaAttrib.destStride = ((strideSize << DMA_DSRn_DSS_BIT) |
                                 (strideDistance << DMA_DSRn_DSD_BIT));
        dmaAttrib.userEosiHandler = (HANDLER)userEosiHandleInt;
        dmaAttrib.userEolniHandler = (HANDLER)userEolniHandleInt;
        dmaAttrib.userEolsiHandler = (HANDLER)userEolsiHandleInt;
        dmaAttrib.userErrorHandler = (HANDLER)userErrorHandleInt;

        /* start the DMA - Extended Direct */

        sysMpc8540DmaStart(chan,descPtr,NULL,&dmaAttrib,&dmaStatus);

        if (dmaAttrib.waitForCompletion == FALSE)
            {
    	    sysMpc8540DmaStatus(chan, &dmaStatus);
            while (dmaStatus.dmaStatus != OK)
                {
                sysDelay();
                sysMpc8540DmaStatus(chan, &dmaStatus);
                }
            }

        if ((strideDataCompare(strideSize,strideDistance,strideSize,
            strideDistance, (byteCount / (strideDistance/strideSize)),
            (UCHAR *)buffer1,(UCHAR *)buffer2) == OK)  && 
	    (numEosi == 0x1) && (numEolni == 0x0) && (numEolsi == 0x0) && 
	    (numError == 0x0))
            printf("PASS\n\r");
        else
            {
            printf("FAIL\n\r");
            passed = ERROR;
            }

        printf(" No link or list descriptors.\n\r");
        printf(" Total bytes transferred = 0x%08x\n\r", byteCount/2);
        if (dmaAttrib.waitForCompletion == TRUE)
            printf(" Wait for DMA completion.\n\r");
        else
            printf(" Poll for DMA completion.\n\r");
        printf(" Source & Destination stride size = 0x%08x\n\r", strideSize);
        printf(" Source & Destination stride size = 0x%08x\n\r", strideDistance);

        numEosi = 0x0;
        numEolni = 0x0;
        numEolsi = 0x0;
        numError = 0x0;

        displayStatus(&dmaStatus);

        /* Test DMA driver in Basic Chain Mode - 8 descriptors */

        printf(" Basic Chain Mode.......................................");        

        dataFill ((UCHAR *)buffer3,8*byteCount);
        memset(buffer4,0x0,8*byteCount);   /* clear destination buffer */
        memset(descPtrChain,0x0,8 * DMA_LINK_DESC_SIZE);

        tempPtr = descPtrChain;

        tempBuf1 = buffer3;
        tempBuf2 = buffer4;

        for (x = 0; x < 7; x++)
            {
            tempPtr->sourceAttr = 0x00050000;
            tempPtr->sourceAddr = (UINT32)tempBuf1+(x*byteCount);
            tempPtr->destinAttr = 0x00050000;
            tempPtr->destinAddr = (UINT32)tempBuf2+(x*byteCount);
            tempPtr->nextDescPtrAddr = (((UINT32)tempPtr + DMA_LINK_DESC_SIZE) |
                                        DMA_NLNDARn_NDEOSIE_MASK);
            tempPtr->byteCount = byteCount;
            tempPtr++;
            }

        /* Last Descriptor */

        tempPtr->sourceAttr = 0x00050000;
        tempPtr->sourceAddr = (UINT32)tempBuf1+(x*byteCount);
        tempPtr->destinAttr = 0x00050000;
        tempPtr->destinAddr = (UINT32)tempBuf2+(x*byteCount);
        tempPtr->nextDescPtrAddr = (0x00000000 | DMA_NLNDARn_EOLND_MASK |
                                    DMA_NLNDARn_NDEOSIE_MASK);
        tempPtr->byteCount = byteCount;

        dmaAttrib.sourceStride = 0x00000000;
        dmaAttrib.destStride = 0x00000000;
        dmaAttrib.userEosiHandler = (HANDLER)userEosiHandleInt;
        dmaAttrib.userEolniHandler = (HANDLER)userEolniHandleInt;
        dmaAttrib.userEolsiHandler = (HANDLER)userEolsiHandleInt;
        dmaAttrib.userErrorHandler = (HANDLER)userErrorHandleInt;

        /* start the DMA */

        sysMpc8540DmaStart(chan,descPtrChain,NULL,&dmaAttrib,&dmaStatus);

        if (dmaAttrib.waitForCompletion == FALSE)
            {
    	    sysMpc8540DmaStatus(chan, &dmaStatus);
            while (dmaStatus.dmaStatus != OK)
                {
                sysDelay();
                sysMpc8540DmaStatus(chan, &dmaStatus);
                }
            }

        if ((dataCompare((UCHAR *)buffer3, (UCHAR *)buffer4, 8 * byteCount) == OK)
             && (numEolni == 0x1) && (numEolsi == 0x0) && (numError == 0x0))
            printf("PASS\n\r");
        else
            {
            printf("FAIL\n\r");
            passed = ERROR;
            }

        printf(" Number of link descriptors = 0x%08x\n\r", 0x8);
        printf(" No list descriptors.\n\r");
        printf(" Number of bytes per descriptor = 0x%08x\n\r", byteCount);
        printf(" Total bytes transferred = 0x%08x\n\r", 8*byteCount);
        if (dmaAttrib.waitForCompletion == TRUE)
            printf(" Wait for DMA completion.\n\r");
        else
            printf(" Poll for DMA completion.\n\r");

        numEosi = 0x0;
        numEolni = 0x0;
        numEolsi = 0x0;
        numError = 0x0;

        displayStatus(&dmaStatus);

        /*
         * Test DMA driver in Extended Chain Mode
         * 8 link, 2 list descriptors
         */

        printf(" Extended Chain Mode (no striding)......................");

        memset(buffer4,0x0,8*byteCount);   /* clear destination buffer */
        memset(descPtrChain,0x0,8 * DMA_LINK_DESC_SIZE);
        memset(descPtrList,0x0,2 * DMA_LIST_DESC_SIZE);

        tempPtr = descPtrChain;
        tempListPtr = descPtrList;
        tempBuf1 = buffer3;
        tempBuf2 = buffer4;

        tempListPtr->firstLinkDescPrtAddr = (UINT32)tempPtr;
        tempListPtr->nextListDescPtrAddr = ((UINT32)descPtrList +
                                            DMA_LIST_DESC_SIZE);
        tempListPtr++;

        for (x = 0; x < 3; x++)
            {
            tempPtr->sourceAttr = 0x00050000;
            tempPtr->sourceAddr = (UINT32)tempBuf1+(x*byteCount);
            tempPtr->destinAttr = 0x00050000;
            tempPtr->destinAddr = (UINT32)tempBuf2+(x*byteCount);
            tempPtr->nextDescPtrAddr = (UINT32)tempPtr + DMA_LINK_DESC_SIZE;
            tempPtr->byteCount = byteCount;
            tempPtr++;
            }

        /* Last Descriptor */

        tempPtr->sourceAttr = 0x00050000;
        tempPtr->sourceAddr = (UINT32)tempBuf1+(x*byteCount);
        tempPtr->destinAttr = 0x00050000;
        tempPtr->destinAddr = (UINT32)tempBuf2+(x*byteCount);
        tempPtr->nextDescPtrAddr = 0x00000000 | DMA_NLNDARn_EOLND_MASK;
        tempPtr->byteCount = byteCount;

        x++;
        tempPtr++;
        tempListPtr->firstLinkDescPrtAddr = (UINT32)tempPtr;
        tempListPtr->nextListDescPtrAddr = 0x00000000 | DMA_NLSDARn_EOLSD_MASK;

        for (x = 4; x < 7; x++)
            {
            tempPtr->sourceAttr = 0x00050000;
            tempPtr->sourceAddr = (UINT32)tempBuf1+(x*byteCount);
            tempPtr->destinAttr = 0x00050000;
            tempPtr->destinAddr = (UINT32)tempBuf2+(x*byteCount);
            tempPtr->nextDescPtrAddr = (UINT32)tempPtr + DMA_LINK_DESC_SIZE;
            tempPtr->byteCount = byteCount;
            tempPtr++;
            }

        /* Last Descriptor */

        tempPtr->sourceAttr = 0x00050000;
        tempPtr->sourceAddr = (UINT32)tempBuf1+(x*byteCount);
        tempPtr->destinAttr = 0x00050000;
        tempPtr->destinAddr = (UINT32)tempBuf2+(x*byteCount);
        tempPtr->nextDescPtrAddr = 0x00000000 | DMA_NLNDARn_EOLND_MASK;
        tempPtr->byteCount = byteCount;

        dmaAttrib.sourceStride = 0x00000000;
        dmaAttrib.destStride = 0x00000000;
        dmaAttrib.userEosiHandler = (HANDLER)userEosiHandleInt;
        dmaAttrib.userEolniHandler = (HANDLER)userEolniHandleInt;
        dmaAttrib.userEolsiHandler = (HANDLER)userEolsiHandleInt;
        dmaAttrib.userErrorHandler = (HANDLER)userErrorHandleInt;

        /* start the DMA */

        sysMpc8540DmaStart(chan,descPtrChain,descPtrList,&dmaAttrib,&dmaStatus);

        if (dmaAttrib.waitForCompletion == FALSE)
            {
    	    sysMpc8540DmaStatus(chan, &dmaStatus);
            while (dmaStatus.dmaStatus != OK)
                {
                sysDelay();
                sysMpc8540DmaStatus(chan, &dmaStatus);
                }
            }

        if ((dataCompare((UCHAR *)buffer3, (UCHAR *)buffer4, 8 * byteCount) == OK)
             && (numEolni == 0x2) && (numEolsi == 0x1) && (numError == 0x0))
            printf("PASS\n\r");
        else
            {
            printf("FAIL\n\r");
            passed = ERROR;
            }

        printf(" Number of link descriptors = 0x%08x\n\r", 0x8);
        printf(" Number of list descriptors = 0x%08x\n\r", 0x2);
        printf(" Number of bytes per descriptor = 0x%08x\n\r", byteCount);
        printf(" Number of link descriptors per list = 0x%08x\n\r", 0x4);
        printf(" Total bytes transferred = 0x%08x\n\r", 8*byteCount);
        if (dmaAttrib.waitForCompletion == TRUE)
            printf(" Wait for DMA completion.\n\r");
        else
            printf(" Poll for DMA completion.\n\r");
        printf(" No source or destination striding.\n\r");

        numEosi = 0x0;
        numEolni = 0x0;
        numEolsi = 0x0;
        numError = 0x0;

        displayStatus(&dmaStatus);

        /*
         * Test DMA driver in Extended Chain Mode
         * 8 link, 2 list descriptors
         */

        printf(" Extended Chain Mode (destination striding).............");

        memset(buffer4,0x0,8*byteCount);   /* clear destination buffer */
        memset(descPtrChain,0x0,8 * DMA_LINK_DESC_SIZE);
        memset(descPtrList,0x0,2 * DMA_LIST_DESC_SIZE);

        tempPtr = descPtrChain;
        tempListPtr = descPtrList;
        tempBuf1 = buffer3;
        tempBuf2 = buffer4;

        tempListPtr->firstLinkDescPrtAddr = ((UINT32)tempPtr |
					     DMA_NLNDARn_NDEOSIE_MASK);
        tempListPtr->nextListDescPtrAddr = ((UINT32)descPtrList +
                                             DMA_LIST_DESC_SIZE);
        tempListPtr->sourceStride = 0x00000000;
        tempListPtr->destStride = ((strideSize << DMA_DSRn_DSS_BIT) |
                                   (strideDistance << DMA_DSRn_DSD_BIT));

        tempListPtr++;

        for (x = 0; x < 3; x++)
            {
            tempPtr->sourceAttr = 0x00050000;
            tempPtr->sourceAddr = (UINT32)tempBuf1+(x*byteCount);
            tempPtr->destinAttr = 0x01050000;
            tempPtr->destinAddr = (UINT32)tempBuf2+(x*byteCount);
            tempPtr->nextDescPtrAddr = (((UINT32)tempPtr + DMA_LINK_DESC_SIZE) |
					 DMA_NLNDARn_NDEOSIE_MASK);
            tempPtr->byteCount = (byteCount / (strideDistance/strideSize));
            tempPtr++;
            }

        /* Last Descriptor */

        tempPtr->sourceAttr = 0x00050000;
        tempPtr->sourceAddr = (UINT32)tempBuf1+(x*byteCount);
        tempPtr->destinAttr = 0x01050000;
        tempPtr->destinAddr = (UINT32)tempBuf2+(x*byteCount);
        tempPtr->nextDescPtrAddr = (0x00000000 | DMA_NLNDARn_EOLND_MASK |
				    DMA_NLNDARn_NDEOSIE_MASK);
        tempPtr->byteCount = (byteCount / (strideDistance/strideSize));

        tempPtr++;
        tempListPtr->firstLinkDescPrtAddr = ((UINT32)tempPtr |
					     DMA_NLNDARn_NDEOSIE_MASK);
        tempListPtr->nextListDescPtrAddr = 0x00000000 | DMA_NLSDARn_EOLSD_MASK;
        tempListPtr->sourceStride = 0x00000000;
        tempListPtr->destStride = ((strideSize << DMA_DSRn_DSS_BIT) |
                                   (strideDistance << DMA_DSRn_DSD_BIT));

        for (x = 4; x < 7; x++)
            {
            tempPtr->sourceAttr = 0x00050000;
            tempPtr->sourceAddr = (UINT32)tempBuf1+(x*byteCount);
            tempPtr->destinAttr = 0x01050000;
            tempPtr->destinAddr = (UINT32)tempBuf2+(x*byteCount);
            tempPtr->nextDescPtrAddr = (((UINT32)tempPtr + DMA_LINK_DESC_SIZE) |
					 DMA_NLNDARn_NDEOSIE_MASK);
            tempPtr->byteCount = (byteCount / (strideDistance/strideSize));
            tempPtr++;
            }

        /* Last Descriptor */

        tempPtr->sourceAttr = 0x00050000;
        tempPtr->sourceAddr = (UINT32)tempBuf1+(x*byteCount);
        tempPtr->destinAttr = 0x01050000;
        tempPtr->destinAddr = (UINT32)tempBuf2+(x*byteCount);
        tempPtr->nextDescPtrAddr = (0x00000000 | DMA_NLNDARn_EOLND_MASK |
				    DMA_NLNDARn_NDEOSIE_MASK);
        tempPtr->byteCount = (byteCount / (strideDistance/strideSize));

        dmaAttrib.sourceStride = 0x00000000;
        dmaAttrib.destStride = 0x00000000;
        dmaAttrib.userEosiHandler = (HANDLER)userEosiHandleInt;
        dmaAttrib.userEolniHandler = (HANDLER)userEolniHandleInt;
        dmaAttrib.userEolsiHandler = (HANDLER)userEolsiHandleInt;
        dmaAttrib.userErrorHandler = (HANDLER)userErrorHandleInt;

        /* start the DMA */

        sysMpc8540DmaStart(chan,descPtrChain,descPtrList,&dmaAttrib,&dmaStatus);

        if (dmaAttrib.waitForCompletion == FALSE)
            {
    	    sysMpc8540DmaStatus(chan,&dmaStatus);
            while (dmaStatus.dmaStatus != OK)
                {
                sysDelay();
                sysMpc8540DmaStatus(chan,&dmaStatus);
                }
            }

        if ((strideDataCompare(strideSize,strideDistance,0x0,0x0,
            ((8 * byteCount) / (strideDistance/strideSize)),
            (UCHAR *)buffer3,(UCHAR *)buffer4)== OK) && 
	    (numEosi == 0x8) && (numEolni == 0x2) && (numEolsi == 0x1) && 
	    (numError == 0x0))
            printf("PASS\n\r");
        else
            {
            printf("FAIL\n\r");
            passed = ERROR;
            }

        printf(" Number of link descriptors = 0x%08x\n\r", 0x8);
        printf(" Number of list descriptors = 0x%08x\n\r", 0x2);
        printf(" Number of bytes per descriptor = 0x%08x\n\r", byteCount);
        printf(" Number of link descriptors per list = 0x%08x\n\r", 0x4);
        printf(" Total bytes transferred = 0x%08x\n\r", 4*byteCount);
        if (dmaAttrib.waitForCompletion == TRUE)
            printf(" Wait for DMA completion.\n\r");
        else
            printf(" Poll for DMA completion.\n\r");
        printf(" Destination stride size = 0x%08x\n\r", strideSize);
        printf(" Destination stride distance = 0x%08x\n\r", strideDistance);
        printf(" No source striding.\n\r");

        displayStatus(&dmaStatus);

        printf("\n\r");
    } /* mode loop */
    } /* completion loop */

    /* Free buffer memory */

    free(buffer1);
    free(buffer2);
    free(descPtr);
    free(buffer3);
    free(buffer4);
    free(descPtrChain);
    free(descPtrList);

    printf("Testing ABORT...........................................");

    buffer1 = (UINT32*)memalign(0x20,0x01000000);
    buffer2 = (UINT32*)memalign(0x20,0x01000000);
    descPtr = (DMA_LINK_DESCRIPTOR*)memalign(DMA_LINK_DESC_SIZE,
               DMA_LINK_DESC_SIZE);

    dataFill ((UCHAR *)buffer1,0x01000000);
    memset(buffer2,0x0,0x01000000);   /* clear destination buffer */
    memset(descPtr,0x0,DMA_LINK_DESC_SIZE);    

    descPtr->sourceAttr = 0x00050000;       /* snoop local processor */
    descPtr->sourceAddr = (UINT32)buffer1;
    descPtr->destinAttr = 0x00050000;       /* snoop local processor */
    descPtr->destinAddr = (UINT32)buffer2;
    descPtr->nextDescPtrAddr = 0x00000000;
    descPtr->byteCount = 0x01000000;

    dmaAttrib.sourceStride = 0x00000000;
    dmaAttrib.destStride = 0x00000000;
    dmaAttrib.userEosiHandler = (HANDLER)userEosiHandleInt;
    dmaAttrib.userEolniHandler = (HANDLER)userEolniHandleInt;
    dmaAttrib.userEolsiHandler = (HANDLER)userEolsiHandleInt;
    dmaAttrib.userErrorHandler = (HANDLER)userErrorHandleInt;
    dmaAttrib.waitForCompletion = FALSE;

    /* start the DMA - Basic Direct Mode */

    numEosi = 0x0;
    numEolni = 0x0;
    numEolsi = 0x0;
    numError = 0x0;

    sysMpc8540DmaStart(0,descPtr,NULL,&dmaAttrib,&dmaStatus);
    if ((sysMpc8540DmaAbort(0) == OK) &&
	(dataCompare((UCHAR *)buffer1 + 0x00F00000,
		     (UCHAR *)buffer2 + 0x00F00000, 0x100000) != OK))
	printf(" PASS\n\r");
    else
	{
	passed = ERROR;
	printf(" FAIL\n\r");
	}

    sysMpc8540DmaStatus(0,&dmaStatus);
    displayStatus(&dmaStatus);

    printf("Testing PAUSE...........................................");

    buffer3 = (UINT32*)memalign(0x20,0x01000000);
    buffer4 = (UINT32*)memalign(0x20,0x01000000);
    descPtrChain = (DMA_LINK_DESCRIPTOR*)memalign(DMA_LINK_DESC_SIZE,
                    8 * DMA_LINK_DESC_SIZE);

    dataFill ((UCHAR *)buffer3,0x01000000);
    memset(buffer4,0x0,0x01000000);   /* clear destination buffer */
    memset(descPtrChain,0x0,8 * DMA_LINK_DESC_SIZE);

    tempPtr = descPtrChain;

    tempBuf1 = buffer3;
    tempBuf2 = buffer4;

    for (x = 0; x < 7; x++)
        {
        tempPtr->sourceAttr = 0x00050000;
        tempPtr->sourceAddr = (UINT32)tempBuf1+(x*0x00200000);
        tempPtr->destinAttr = 0x00050000;
        tempPtr->destinAddr = (UINT32)tempBuf2+(x*0x0020000);
        tempPtr->nextDescPtrAddr = (((UINT32)tempPtr + DMA_LINK_DESC_SIZE) |
                                    DMA_NLNDARn_NDEOSIE_MASK);
        tempPtr->byteCount = 0x00200000;
        tempPtr++;
        }

    /* Last Descriptor */

    tempPtr->sourceAttr = 0x00050000;
    tempPtr->sourceAddr = (UINT32)tempBuf1+(x*0x00200000);
    tempPtr->destinAttr = 0x00050000;
    tempPtr->destinAddr = (UINT32)tempBuf2+(x*0x00200000);
    tempPtr->nextDescPtrAddr = (0x00000000 | DMA_NLNDARn_EOLND_MASK |
                                DMA_NLNDARn_NDEOSIE_MASK);
    tempPtr->byteCount = 0x00200000;

    dmaAttrib.sourceStride = 0x00000000;
    dmaAttrib.destStride = 0x00000000;
    dmaAttrib.userEosiHandler = (HANDLER)userEosiHandleInt;
    dmaAttrib.userEolniHandler = (HANDLER)userEolniHandleInt;
    dmaAttrib.userEolsiHandler = (HANDLER)userEolsiHandleInt;
    dmaAttrib.userErrorHandler = (HANDLER)userErrorHandleInt;

    numEosi = 0x0;
    numEolni = 0x0;
    numEolsi = 0x0;
    numError = 0x0;

    /* start the DMA */

    sysMpc8540DmaStart(0,descPtrChain,NULL,&dmaAttrib,&dmaStatus);

    if ((sysMpc8540DmaPause(0) == OK) &&
	(dataCompare((UCHAR *)buffer3 + 0x00F00000,
                     (UCHAR *)buffer4 + 0x00F00000, 0x100000) != OK))
	{
	printf("PASS\n\r");
	}
    else
	{
	passed = ERROR;
	printf("FAIL\n\r");
	}

    sysMpc8540DmaStatus(0,&dmaStatus);
    displayStatus(&dmaStatus);

    printf("Testing RESUME...........................................");

    passed = sysMpc8540DmaResume(0);

    sysMpc8540DmaStatus(0,&dmaStatus);

    while (dmaStatus.dmaStatus != OK)
	   {
       sysDelay();
	   sysMpc8540DmaStatus(0,&dmaStatus);
	   }

    if ( dataCompare((UCHAR *)buffer3 + 0x00F00000,
                     (UCHAR *)buffer4 + 0x00F00000, 0x100000) == OK)
        {
        printf("PASS\n\r");
        }
    else
        {
        passed = ERROR;
        printf("FAIL\n\r");
        }

    sysMpc8540DmaStatus(0,&dmaStatus);
    displayStatus(&dmaStatus);

    numEosi = 0x0;
    numEolni = 0x0;
    numEolsi = 0x0;
    numError = 0x0;

    free (buffer1);
    free (buffer2);
    free (buffer3);
    free (buffer4);
    free (descPtrChain);

    printf("Testing Source Address Hold..............................");

    buffer1 = (UINT32*)memalign(0x4,0x00000100);
    buffer2 = (UINT32*)memalign(0x4,0x00000100);
    descPtr = (DMA_LINK_DESCRIPTOR*)memalign(DMA_LINK_DESC_SIZE,
               DMA_LINK_DESC_SIZE);
    dataFill ((UCHAR *)buffer1,0x00000100);
    memset(buffer2,0x0,0x00000100);   /* clear destination buffer */
    memset(descPtr,0x0,DMA_LINK_DESC_SIZE);

    descPtr->sourceAttr = 0x00050000;       /* snoop local processor */
    descPtr->sourceAddr = (UINT32)buffer1;
    descPtr->destinAttr = 0x00050000;       /* snoop local processor */
    descPtr->destinAddr = (UINT32)buffer2;
    descPtr->nextDescPtrAddr = 0x00000000;
    descPtr->byteCount = 0x00000100;

    dmaAttrib.sourceStride = 0x00000000;
    dmaAttrib.destStride = 0x00000000;
    dmaAttrib.userEosiHandler = (HANDLER)userEosiHandleInt;
    dmaAttrib.userEolniHandler = (HANDLER)userEolniHandleInt;
    dmaAttrib.userEolsiHandler = (HANDLER)userEolsiHandleInt;
    dmaAttrib.userErrorHandler = (HANDLER)userErrorHandleInt;
    dmaAttrib.waitForCompletion = TRUE;
    dmaAttrib.holdSourceAddr = TRUE;
    dmaAttrib.holdSourceAddrSize = 0x2;	/* set for 4 bytes */
    dmaAttrib.holdDestAddr = FALSE;
    dmaAttrib.holdDestAddrSize = 0x0;

    /* start the DMA - Basic Direct Mode */

    numEosi = 0x0;
    numEolni = 0x0;
    numEolsi = 0x0;
    numError = 0x0;
    
    sysMpc8540DmaSetBandwidth(0,0x2);	/* set BWC to 4 bytes */

    if ((sysMpc8540DmaStart(0,descPtr,NULL,&dmaAttrib,&dmaStatus) == OK) &&
	(holdDataCompare(TRUE, FALSE, 0x4,0x0,0x100,(UCHAR *)buffer1,
	 (UCHAR *)buffer2) == OK))
	printf(" PASS\n\r");
    else
	{
	passed = ERROR;
	printf(" FAIL\n\r");
	}

    displayStatus(&dmaStatus);

    printf("Testing Destination Address Hold.........................");

    memset(buffer2,0x0,0x00000100);   /* clear destination buffer */
    memset(descPtr,0x0,DMA_LINK_DESC_SIZE);

    descPtr->sourceAttr = 0x00050000;       /* snoop local processor */
    descPtr->sourceAddr = (UINT32)buffer1;
    descPtr->destinAttr = 0x00050000;       /* snoop local processor */
    descPtr->destinAddr = (UINT32)buffer2;
    descPtr->nextDescPtrAddr = 0x00000000;
    descPtr->byteCount = 0x00000100;

    dmaAttrib.sourceStride = 0x00000000;
    dmaAttrib.destStride = 0x00000000;
    dmaAttrib.userEosiHandler = (HANDLER)userEosiHandleInt;
    dmaAttrib.userEolniHandler = (HANDLER)userEolniHandleInt;
    dmaAttrib.userEolsiHandler = (HANDLER)userEolsiHandleInt;
    dmaAttrib.userErrorHandler = (HANDLER)userErrorHandleInt;
    dmaAttrib.waitForCompletion = TRUE;
    dmaAttrib.holdSourceAddr = FALSE;
    dmaAttrib.holdSourceAddrSize = 0x0;	
    dmaAttrib.holdDestAddr = TRUE;
    dmaAttrib.holdDestAddrSize = 0x2; /* set for 4 bytes */

    /* start the DMA - Basic Direct Mode */

    numEosi = 0x0;
    numEolni = 0x0;
    numEolsi = 0x0;
    numError = 0x0;
    
    if ((sysMpc8540DmaStart(0,descPtr,NULL,&dmaAttrib,&dmaStatus) == OK) &&
	(holdDataCompare(FALSE, TRUE, 0x0,0x4,0x100,(UCHAR *)buffer1,
	 (UCHAR *)buffer2) == OK))
	printf(" PASS\n\r");
    else
	{
	passed = ERROR;
	printf(" FAIL\n\r");
	}

    displayStatus(&dmaStatus);

    printf("Testing DMA from LBC (FLASH Bank).......................");

    memset(buffer2,0x0,0x00000100);   /* clear destination buffer */
    memset(descPtr,0x0,DMA_LINK_DESC_SIZE);

    descPtr->sourceAttr = 0x00050000;       /* snoop local processor */
    descPtr->sourceAddr = (UINT32)0xFFF00100;  /* boot block - boot code */
    descPtr->destinAttr = 0x00050000;       /* snoop local processor */
    descPtr->destinAddr = (UINT32)buffer2;
    descPtr->nextDescPtrAddr = 0x00000000;
    descPtr->byteCount = 0x00000100;

    dmaAttrib.sourceStride = 0x00000000;
    dmaAttrib.destStride = 0x00000000;
    dmaAttrib.userEosiHandler = (HANDLER)userEosiHandleInt;
    dmaAttrib.userEolniHandler = (HANDLER)userEolniHandleInt;
    dmaAttrib.userEolsiHandler = (HANDLER)userEolsiHandleInt;
    dmaAttrib.userErrorHandler = (HANDLER)userErrorHandleInt;
    dmaAttrib.waitForCompletion = TRUE;
    dmaAttrib.holdSourceAddr = FALSE;
    dmaAttrib.holdSourceAddrSize = 0x0;	
    dmaAttrib.holdDestAddr = FALSE;
    dmaAttrib.holdDestAddrSize = 0x0; /* set for 4 bytes */

    /* start the DMA - Basic Direct Mode */

    numEosi = 0x0;
    numEolni = 0x0;
    numEolsi = 0x0;
    numError = 0x0;
    
    if ((sysMpc8540DmaStart(0,descPtr,NULL,&dmaAttrib,&dmaStatus) == OK) &&
	(dataCompare((UCHAR *)0xFFF00100,(UCHAR *)buffer2, 0x100) == OK))
	printf(" PASS\n\r");
    else
	{
	passed = ERROR;
	printf(" FAIL\n\r");
	}

    displayStatus(&dmaStatus);

#ifdef INCLUDE_ERROR_HANDLING

    printf("Testing Error Handling..................................");

    memset(buffer2,0x0,0x00000100);   /* clear destination buffer */
    memset(descPtr,0x0,DMA_LINK_DESC_SIZE);

    descPtr->sourceAttr = 0x00050000;       /* snoop local processor */
    descPtr->sourceAddr = (UINT32)0xFFF00100;  /* boot block - boot code */
    descPtr->destinAttr = 0x00050000;       /* snoop local processor */
    descPtr->destinAddr = (UINT32)buffer2;
    descPtr->nextDescPtrAddr = 0x00000000;
    descPtr->byteCount = 0x00000000;

    dmaAttrib.sourceStride = 0x00000000;
    dmaAttrib.destStride = 0x00000000;
    dmaAttrib.userEosiHandler = (HANDLER)userEosiHandleInt;
    dmaAttrib.userEolniHandler = (HANDLER)userEolniHandleInt;
    dmaAttrib.userEolsiHandler = (HANDLER)userEolsiHandleInt;
    dmaAttrib.userErrorHandler = (HANDLER)userErrorHandleInt;
    dmaAttrib.waitForCompletion = TRUE;
    dmaAttrib.holdSourceAddr = FALSE;
    dmaAttrib.holdSourceAddrSize = 0x0;	
    dmaAttrib.holdDestAddr = FALSE;
    dmaAttrib.holdDestAddrSize = 0x0; /* set for 4 bytes */

    /* start the DMA - Basic Direct Mode */

    numEosi = 0x0;
    numEolni = 0x0;
    numEolsi = 0x0;
    numError = 0x0;

   sysMpc8540DmaStart(0,descPtr,NULL,&dmaAttrib,&dmaStatus);

   printf(" PASS\n\r");
#ifdef INCLUDE_SHOW_ROUTINES
   sysErrorLogShow(0,0);
#endif /* INCLUDE_SHOW_ROUTINES */
   displayStatus(&dmaStatus);
#endif /* INCLUDE_ERROR_HANDLING */

    free(buffer1);
    free(buffer2);
    free(descPtr);

    printf("Testing Channel Continue..................................");

    buffer3 = (UINT32*)memalign(0x20,8*0x20);	/* 32 bytes per desc */
    buffer4 = (UINT32*)memalign(0x20,8*0x20);
    descPtrChain = (DMA_LINK_DESCRIPTOR*)memalign(DMA_LINK_DESC_SIZE,
                    8 * DMA_LINK_DESC_SIZE);

    dataFill ((UCHAR *)buffer3,8*0x20);
    memset(buffer4,0x0,8*0x20);   /* clear destination buffer */
    memset(descPtrChain,0x0,8 * DMA_LINK_DESC_SIZE);

    tempPtr = descPtrChain;

    tempBuf1 = buffer3;
    tempBuf2 = buffer4;

    for (x = 0; x < 3; x++)
        {
        tempPtr->sourceAttr = 0x00050000;
        tempPtr->sourceAddr = (UINT32)tempBuf1+(x*0x20);
        tempPtr->destinAttr = 0x00050000;
        tempPtr->destinAddr = (UINT32)tempBuf2+(x*0x20);
        tempPtr->nextDescPtrAddr = (((UINT32)tempPtr + DMA_LINK_DESC_SIZE) |
                                    DMA_NLNDARn_NDEOSIE_MASK);
        tempPtr->byteCount = 0x20;
        tempPtr++;
        }

    /* Last Descriptor */

    tempPtr->sourceAttr = 0x00050000;
    tempPtr->sourceAddr = (UINT32)tempBuf1+(x*0x20);
    tempPtr->destinAttr = 0x00050000;
    tempPtr->destinAddr = (UINT32)tempBuf2+(x*0x20);
    tempPtr->nextDescPtrAddr = (((UINT32)tempPtr + DMA_LINK_DESC_SIZE) | 
				(DMA_NLNDARn_EOLND_MASK |
                                DMA_NLNDARn_NDEOSIE_MASK));
    tempPtr->byteCount = 0x20;

    midPointPtr = tempPtr;	/* save address of this descriptor */

    tempPtr++;

    for (x = 4; x < 7; x++)
        {
        tempPtr->sourceAttr = 0x00050000;
        tempPtr->sourceAddr = (UINT32)tempBuf1+(x*0x20);
        tempPtr->destinAttr = 0x00050000;
        tempPtr->destinAddr = (UINT32)tempBuf2+(x*0x20);
        tempPtr->nextDescPtrAddr = (((UINT32)tempPtr + DMA_LINK_DESC_SIZE) |
                                    DMA_NLNDARn_NDEOSIE_MASK);
        tempPtr->byteCount = 0x20;
        tempPtr++;
        }

    /* Last Descriptor */

    tempPtr->sourceAttr = 0x00050000;
    tempPtr->sourceAddr = (UINT32)tempBuf1+(x*0x20);
    tempPtr->destinAttr = 0x00050000;
    tempPtr->destinAddr = (UINT32)tempBuf2+(x*0x20);
    tempPtr->nextDescPtrAddr = (0x00000000 | DMA_NLNDARn_EOLND_MASK |
                                DMA_NLNDARn_NDEOSIE_MASK);
    tempPtr->byteCount = 0x20;

    dmaAttrib.sourceStride = 0x00000000;
    dmaAttrib.destStride = 0x00000000;
    dmaAttrib.userEosiHandler = NULL;
    dmaAttrib.userEolniHandler = NULL;
    dmaAttrib.userEolsiHandler = NULL;
    dmaAttrib.userErrorHandler = NULL;
    dmaAttrib.waitForCompletion = TRUE;
    dmaAttrib.holdSourceAddr = FALSE;
    dmaAttrib.holdSourceAddrSize = 0x0;
    dmaAttrib.holdDestAddr = FALSE;
    dmaAttrib.holdDestAddrSize = 0x0;

    /* start the DMA */

    sysMpc8540DmaStart(0,descPtrChain,NULL,&dmaAttrib,&dmaStatus);

    if (dataCompare((UCHAR *)buffer3,(UCHAR *)buffer4,8*0x20) != OK)
	{

	/* only half the data was transferred as expected */

	/* clear end-of-links bit in order to continue with the DMA */

	midPointPtr->nextDescPtrAddr &= ~DMA_NLNDARn_EOLND_MASK;

	/* "restart" the DMA by setting CC in the Mode register */

	sysMpc8540ChannelContinueSet(0);

    dmaChanCntl = &mpc8540DmaCntl[0];
    semTake (dmaChanCntl->semIntId, WAIT_FOREVER);
    sysMpc8540DmaStatus (0, &dmaStatus);

	/* compare buffers again */

        if (dataCompare((UCHAR *)buffer3,(UCHAR *)buffer4,8*0x20) == OK) 
            printf("PASS\n\r");
        else
            {
            printf("FAIL\n\r");
            passed = ERROR;
            }
	}
    else
	{
        printf("FAIL\n\r");
        passed = ERROR;
	}

    displayStatus(&dmaStatus);

    free (descPtrChain);
    free (buffer3);
    free (buffer4);

    return (passed);
    }

#endif /* INCLUDE_MPC8540_DMA_TEST */
