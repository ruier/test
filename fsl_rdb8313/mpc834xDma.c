/* mpc834xDma.c - Freescale MPC834X Message Passing/Doorbell/DMA driver */

/*
* Copyright 2007 Wind River Systems, Inc.
*
* The right to copy, distribute, modify or otherwise make use
* of this software may be licensed only pursuant to the terms
* of an applicable Wind River license agreement.
*/

/*
modification history
--------------------
01a,11apr07,vf   created
*/

/*
DESCRIPTION
This is a driver for the Message passing, doorbell and DMA engines
present in the Freescale MPC834X CPU.  It is intended for use by
client drivers performing other high level functions.

This library does not support chained-mode DMA, nor does it
support externally controlled DMA.

USAGE

The entire DMA, Message Passing and Doorbell library is initialized
by calling mpc834xDmaInit().

DMA

There are four DMA channels available on the MPC8349 CPU.  At present,
only one client may queue up to use any given channel.  Subsequent
clients wait until the current client is done with the channel.

A DMA channel is reserved for use by calling mpc834xDmaReserveChannel().
Once the channels reserved, a client use the channel with mpc834xDmaMemCopy().

mpc834xDmaCallbackAdd() is used to install a callback routine that is invoked
whenever a DMA transaction has completed.  If desired, the callback routine
can be removed with mpc834xDmaCallbackDelete().

Once the DMA client is done with the channel, call mpc834xDmaReleaseChannel().

During a DMA transaction, mpc834xDmaSuspend() and mpc834xDmaResume() can be called
to suspend and resume the DMA transaction.

MESSAGING

Call mpc834xMsgRegRead() to read a message register.  The register to be read
is passed as a parameter.  Likewise, use mpc834xMsgRegWrite() to write a 
message register.  Again, the register to be written is passed as a parameter.

DOORBELLS

Call mpc834xDoorbellRead() and mpc834xDoorbellWrite() to read and write the
doorbell register.

Use mpc834xMsgDoorbellCallbackAdd() to install callback routines for
message register and doorbell register reads and writes.  Refer to the
manual entry below for mpc834xMsgDoorbellCallbackAdd() usage.

INCLUDE FILES: mpc834xDma.h

*/

/* includes */


#include <vxWorks.h>
#include "config.h"
#include <intLib.h>
#include <iv.h>
#include <vmLib.h>
#include <private/vmLibP.h>
#include <rebootLib.h>
#include <cacheLib.h>

#include <excLib.h>
#include <stdlib.h>
#include <string.h>

#undef DEBUG
#ifdef DEBUG
#include <logLib.h>
#include <stdio.h>
#else
#define NDEBUG
#endif
#include <assert.h>

#include "mpc834xDma.h"

/* prototypes */

IMPORT UINT32 sysInLong (VUINT32 *pAddr);
IMPORT VOID   sysOutLong(VUINT32 *pAddr, UINT32 data);

/* macros */

#ifndef SYS_IN_LONG
#define SYS_IN_LONG  sysInLong
#endif

#ifndef SYS_OUT_LONG
#define SYS_OUT_LONG sysOutLong
#endif

#undef  CCS_BAR
#define CCS_BAR mpc834xDmaImmr

#ifndef EIEO_SYNC
#define EIEIO_SYNC  _WRS_ASM (" eieio; sync")
#endif

/* locals */
LOCAL UINT32 mpc834xDmaImmr = CCSBAR;

LOCAL MPC834XDMA_CHAN mpc834xDmaChannels[MPC834XDMA_NUM_CHANNELS];

LOCAL FUNCPTR mpc834xInMsgCallbackRtn = NULL;
LOCAL UINT32  mpc834xInMsgCallbackArg = 0;
LOCAL FUNCPTR mpc834xOutMsgCallbackRtn = NULL;
LOCAL UINT32  mpc834xOutMsgCallbackArg = 0;
LOCAL FUNCPTR mpc834xInDoorbellCallbackRtn = NULL;
LOCAL UINT32  mpc834xInDoorbellCallbackArg = 0;
LOCAL FUNCPTR mpc834xOutDoorbellCallbackRtn = NULL;
LOCAL UINT32  mpc834xOutDoorbellCallbackArg = 0;

/* forward declarations */

LOCAL STATUS mpc834xDmaSubmitJob (MPC834XDMA_CHAN *pChan, MPC834XDMA_DESCRIPTOR *pJob);
LOCAL void mpc834xDmaExcJob (MPC834XDMA_CHAN *pChan);
LOCAL void mpc834xDmaIsr (void);
LOCAL void mpc834xDmaRebootHook (int startType);
LOCAL UINT32 mpc834xDmaRead (MPC834XDMA_CHAN *pChan, int offset);
LOCAL void mpc834xDmaWrite (MPC834XDMA_CHAN *pChan, int offset, UINT32 value);
LOCAL void mpc834xDoorbellMessageIsr(void);

#ifdef DEBUG
UINT32 doorbellMsgIntCnt = 0;
UINT32 dmaIntCnt = 0;
#endif



/**********************************************************************
*
* mpc834xDmaInit - Initialize MPC834XDMA/messaging/doorbell driver
*
* This routine performs driver and hardware initialization required
* by the MPC834XDMA driver.  <dmaINum> and <msgINum> are passed
* to intConnect() and represent the associated interrupt numbers
* for DMA and messaging/doorbell interrupts, respectively.  <immBar>
* is the value of the IMMRBAR register (typically 0xff400000).
*
* RETURNS: OK, or ERROR on any invocation after the first one.
*
* SEE ALSO: mpc834xDmaMemCopy
*
*/

STATUS mpc834xDmaInit
    (
    UINT32 immrBar,    /* Value of IMMR register */
    UINT32 dmaINum,    /* interrupt number for DMA */
    UINT32 msgINum     /* interrupt number for message/doorbell */
    )
    {
    static BOOL mpc834xDma_initialized = FALSE;
    UINT32 i;

    if (mpc834xDma_initialized)
        return ERROR;

    mpc834xDma_initialized = TRUE;

    mpc834xDmaImmr = immrBar;

    for (i = 0; i < MPC834XDMA_NUM_CHANNELS; i++)
        {
        mpc834xDmaChannels[i].base      = (VUINT32 *)(CCS_BAR + DMAMSG_REG_BA + 0x100 + (i * 0x80));
        mpc834xDmaChannels[i].chanId    = i;
        mpc834xDmaChannels[i].dmamr     = MPC83XX_DEFAULT_DMAMR;
        mpc834xDmaChannels[i].mutex     = semMCreate(SEM_Q_FIFO);
        mpc834xDmaChannels[i].callback  = NULL;
        mpc834xDmaChannels[i].available = TRUE;
        mpc834xDmaChannels[i].user_data = 0;
        }

    intConnect(INUM_TO_IVEC(dmaINum), (VOIDFUNCPTR)mpc834xDmaIsr, 0);
    intConnect(INUM_TO_IVEC(msgINum),  (VOIDFUNCPTR)mpc834xDoorbellMessageIsr, 0);

    intEnable(dmaINum);
    intEnable(msgINum);

    SYS_OUT_LONG(M83XX_OMIMR(CCS_BAR), 0);  /* Enable outbound message/doorbell interrupts in OMIMR register*/
    SYS_OUT_LONG(M83XX_IMIMR(CCS_BAR), 0);  /* Enable inbound  message/doorbell interrupts, no machine checks in IMIMR register */

    /* Install hook to stop DMA in case of reboot */
    rebootHookAdd ((FUNCPTR)mpc834xDmaRebootHook);

    return OK;
    }


/**********************************************************************
*
* mpc834xDmaMemCopy - perform DMA copy from one address to another
*
* mpc834xDmaMemCopy() copies <size> bytes from <src> to <dst> according to the
* parameters set forth by the <dmamr> argument.  The <dmamr> value will be
* used to set the DMA mode register.  Do not set the CS bit, this routine
* will handle that.
*
* The final argument to mpc834xDmaMemCopy() is a 32-bit value that is passed to the
* client's callback routine.  It is intended to manage or track completion of
* mpc834xDmaMemCopy().  Possible uses for user_data included semaphore identifiers
* (SEM_ID), an array index, or as a general purpose unique identifier.
* Its use is optional.
*
* RETURNS: OK if a descriptor is available to fullfill the request
* or ERROR if not.
*
* SEE ALSO: mpc834xDmaMemInit, mpc834xDmaReserveChannel, mpc834xDmaReleaseChannel,
*
*/

STATUS mpc834xDmaMemCopy
    (
    MPC834XDMA_CHAN *pChan,  /* channel over which to perform DMA */
    UINT32 dmamr,            /* value for DMA mode register to use */
    PHYS_ADDR src,           /* source address */
    PHYS_ADDR dst,           /* destination addreess */
    UINT32 size,             /* number of bytes to transfer */
    UINT32 user_data         /* user-defined data (optional) supplied to callback routine */
    )
    {
    MPC834XDMA_DESCRIPTOR *pDescriptor;

    semTake (pChan->mutex, WAIT_FOREVER);

    pDescriptor = memalign (MPC834XDMA_DESCRIPTOR_ALIGNMENT, sizeof (MPC834XDMA_DESCRIPTOR));
    if (pDescriptor == NULL)
        return ERROR;

    pDescriptor->sourceAddr = (UINT32)src;
    pDescriptor->destAddr = (UINT32)dst;
    pDescriptor->byteCount = size;
    pChan->dmamr = dmamr;
    pChan->user_data = user_data;
    mpc834xDmaSubmitJob (pChan, pDescriptor);
    free(pDescriptor);

    semGive (pChan->mutex);
    return OK;
    }


/**********************************************************************
*
* mpc834xDmaReserveChannel - Reserve an MPC834XDMA channel
*
* mpc834xDmaReserveChannel() returns pointer to an MPC834XDMA_CHAN structure which
* is supplied by client software to the MPC834XDMA driver as a handle for
* further channel activities.  mpc834xDmaReserveChannel() is normally followed by
* calls to mpc834xDmaCallbackAdd() and mpc834xDmaMemCopy().
*
* RETURNS: A pointer to a channel to use for DMA, or NULL if none are available.
*
* SEE ALSO: mpc834xDmaMemCopy()
*
*/

MPC834XDMA_CHAN *mpc834xDmaReserveChannel(void)
    {
    MPC834XDMA_CHAN *pChan = NULL;
    int i;

    for (i = 0; i < MPC834XDMA_NUM_CHANNELS; i++)
        {
        if (mpc834xDmaChannels[i].available == TRUE)
            {
            mpc834xDmaChannels[i].available = FALSE;
            pChan = &mpc834xDmaChannels[i];
            break;
            }
        }
    return pChan;
    }


/**********************************************************************
*
* mpc834xDmaReleaseChannel - Release a reserved channel back to the driver
*
* mpc834xDmaReleaseChannel() releases channel back to the MPC834XDMA driver
* that was allocated with mpc834xDmaReserveChannel().
*
* NOTE: The client that calls this routine must be called from the
* same task as the one that called mpc834xDmaReserveChannel(), as task-
* specific resources are freed up here.
*
* RETURNS: OK or ERROR if passed an invalid channel or the channel is busy.
*
* SEE ALSO: mpc834xDmaReserveChannel()
*
*/

STATUS mpc834xDmaReleaseChannel
    (
    MPC834XDMA_CHAN *pChannel
    )
    {
    if (NULL == pChannel)
        return ERROR;

    pChannel->available = TRUE;
    return OK;
    }


/**********************************************************************
*
* mpc834xDmaCallbackAdd - Register a callback function for a channel
*
* mpc834xDmaCallbackAdd() registers a callback function supplied by client
* software.  The callback function must be of the form:
* \cs
* void callback(MPC834XDMA_CHAN *pChan, UINT32 user_data);
* \ce
*
* The callback will be called during a DMA interrupt.
*
* user_data will contain the identifier passed as user_data to a 
* matching call to mpc834xDmaMemCopy().
*
* RETURNS: ERROR if pChan is invalid, otherwise OK.
*
* SEE ALSO: mpc834xDmaMemCopy(), mpc834xDmaCallbackDelete()
*
*/

STATUS mpc834xDmaCallbackAdd
    (
    MPC834XDMA_CHAN *pChan,  /* DMA channel for which to install callback routine */
    FUNCPTR pCallback        /* pointer to callback routine */
    )
    {
    if (pChan == NULL)
        return ERROR;

    if (semTake (pChan->mutex, WAIT_FOREVER) == ERROR)
        return ERROR;

    pChan->callback = pCallback;
    semGive (pChan->mutex);

    return OK;
    }

    
/**********************************************************************
*
* mpc834xDmaCallbackDelete - Unregister a DMA callback routine
*
* Unregister a DMA callback routine.
*
* RETURNS: OK if pChan is not NULL, otherwise ERROR
*
* SEE ALSO: mpc834xDmaMemCopy(), mpc834xDmaCallbackAdd()
*
*/

STATUS mpc834xDmaCallbackDelete
    (
    MPC834XDMA_CHAN *pChan   /* DMA channel for which to delete callback routine */
    )
    {
    if (pChan == NULL)
        return ERROR;

    semTake (pChan->mutex, WAIT_FOREVER);
    pChan->callback = NULL;
    semGive (pChan->mutex);
    return OK;
    }    


/**********************************************************************
*
* mpc834xDmaSubmitJob - Initiate DMA operations with a new descriptor chain
* or append to the descriptor chain of any currently running DMA job
*
* mpc834xDmaSubmitJob() initiates DMA operations with a new descriptor chain
* or appends to the descriptor chain of any currently running DMA job
* if the channel is already active.
*
* RETURNS: OK if enough descriptors are available from the descriptor
* pool and all other sanity checks clear, otherwise ERROR.
*
* SEE ALSO: mpc834xDmaMemCopy(), mpc834xDmaReserveChannel()
*
* NOMANUAL
*
*/

LOCAL STATUS mpc834xDmaSubmitJob
    (
    MPC834XDMA_CHAN * pChan,        /* channel on which to perform DMA */
    MPC834XDMA_DESCRIPTOR* pJob     /* job descriptor */
    )
    {
    UINT32 dmasr;
    
    /* check for invalid arguments */
    if ((NULL == pChan) || (NULL == pJob))
        return ERROR;
    
    /* check for invalid arguments */        
    if ((UINT32)pJob % MPC834XDMA_DESCRIPTOR_ALIGNMENT)
        return ERROR;
        
    /* check if channel is active or not */
    dmasr = mpc834xDmaRead (pChan, (UINT32)M83XX_DMASR_OFFSET);
    
    if (dmasr & M83XX_DMASR_CB)
        /*
         * We do not support chaining, as we cannot stop a direct mode
         * transfer and change it to a chained transfer.
         */
        return ERROR; 
    else
        {        
        /* set source address */
        mpc834xDmaWrite (pChan, M83XX_DMASAR_OFFSET, (UINT32)pJob->sourceAddr);

        /* set destination address */
        mpc834xDmaWrite (pChan, M83XX_DMADAR_OFFSET, (UINT32)pJob->destAddr);

        /* set byte count address */
        mpc834xDmaWrite (pChan, M83XX_DMABCR_OFFSET, (UINT32)pJob->byteCount);

        /* first, clear the start bit */
        pChan->dmamr &= ~M83XX_DMAMR_CS;
        mpc834xDmaWrite (pChan, M83XX_DMAMR_OFFSET, pChan->dmamr);

        /* set the start bit and away we go */
        mpc834xDmaWrite (pChan, M83XX_DMAMR_OFFSET, (pChan->dmamr | M83XX_DMAMR_CS));
        }

        return OK;
    }


/**********************************************************************
*
* mpc834xDmaSuspend - Suspend a DMA channel
*
* Suspend a DMA channel by setting the suspend bit in the DMA
* Control Register
*
* RETURNS: OK if successful, ERROR if the suspend request failed
*
* SEE ALSO: mpc834xDmaResume()
*
*/

STATUS mpc834xDmaSuspend
    (
    MPC834XDMA_CHAN *pChan  /* DMA channel */
    )
    {
    UINT32 dmamr = mpc834xDmaRead (pChan, M83XX_DMAMR_OFFSET);
    mpc834xDmaWrite (pChan, M83XX_DMAMR_OFFSET, (dmamr & ~(M83XX_DMAMR_CS)));

    /* check that DMA is suspended */
    if (mpc834xDmaRead (pChan, M83XX_DMASR_OFFSET) & M83XX_DMASR_CB)
        return ERROR;
    else
        return OK;
    }


/**********************************************************************
*
* mpc834xDmaResume - Resume a DMA channel
*
* Resume a DMA channel previously suspended by mpc834xDmaSuspend()
*
* RETURNS: OK if successful, ERROR if the resume request failed
*
* SEE ALSO: mpc834xDmaSuspend()
*
*/

STATUS mpc834xDmaResume
    (
    MPC834XDMA_CHAN* pChan  /* DMA channel */
    )
    {
    UINT32 dmamr = mpc834xDmaRead (pChan, M83XX_DMAMR_OFFSET);
    mpc834xDmaWrite (pChan, M83XX_DMAMR_OFFSET, (dmamr  | M83XX_DMAMR_CS));

    /* check that DMA is running */
    if (mpc834xDmaRead (pChan, M83XX_DMASR_OFFSET) & M83XX_DMASR_CB)
        return OK;
    else
        return ERROR;
    }


/**********************************************************************
*
* mpc834xDmaRebootHook - Routine called when system is rebooted
*
* mpc834xDmaRebootHook() calls mpc834xDmaSuspend() for each DMA channel which prevents
* undesirable bus transactions during the reboot process.
*
* SEE ALSO: rebootHookAdd()
*
* NOMANUAL
*
*/

LOCAL void mpc834xDmaRebootHook
    (
    int startType  /* passed in to us by reboot() */
    )
    {
    MPC834XDMA_CHAN channel;
    int i;

    for (i = 0; i < MPC834XDMA_NUM_CHANNELS; i++)
        {
        channel.chanId = i;
        mpc834xDmaSuspend (&channel);
        }
    }


/**********************************************************************
*
* mpc834xDmaRead - Read a DMA channel register
*
* RETURNS: contents of a channel register as a UINT32
*
* SEE ALSO: mpc834xDmaWrite(), mpc834xDmaRead()
*
* NOMANUAL
*
*/

LOCAL UINT32 mpc834xDmaRead
    (
    MPC834XDMA_CHAN *pChan,  /* DMA channel */
    int offset               /* register offset */
    )
    {
    volatile UINT32 *addr;

    addr = (UINT32 *)((UINT32)pChan->base + (UINT32)offset);  /* cast to avoid pointer arithmetic */
    return SYS_IN_LONG(addr);
    }


/**********************************************************************
*
* mpc834xDmaWrite - Write a UINT32 value to a channel register
*
* SEE ALSO: mpc834xDmaRead()
*
* NOMANUAL
*/

LOCAL void mpc834xDmaWrite
    (
    MPC834XDMA_CHAN* pChan,  /* pointer to DMA channel */
    int offset,              /* offset of register to write */
    UINT32 value             /* value to write */
    )
    {
    volatile UINT32 *addr;

    addr = (UINT32 *)((UINT32)pChan->base + (UINT32)offset);  /* cast to avoid pointer arithmetic */
    SYS_OUT_LONG(addr, value);
    EIEIO_SYNC;
    }


/**********************************************************************
*
* mpc834xDmaExcJob - maintenance function called after DMA events
*
* mpc834xDmaExcJob() performs descriptor maintenance and calls client software
* callback functions whenever polled by mpc834xDmaPollStatus() or an interrupt
* occurs enabled in the DMAMR register.
*
* SEE ALSO: mpc834xDmaCallbackAdd(), mpc834xDmaPollStatus()
*
* NOMANUAL
*
*/

LOCAL void mpc834xDmaExcJob
    (
    MPC834XDMA_CHAN *pChan  /* pointer to DMA channel */
    )
    {
    if (pChan == NULL)
        return;
        
    if (pChan->callback != NULL)
        pChan->callback (pChan, pChan->user_data);
    }


/**********************************************************************
*
* mpc834xDmaIsr - mpc834xDma driver interrupt service routine
*
* mpc834xDmaIsr() is called from interrupt context, determines if a DMA
* channel needs attention, and places a job in the tExec task
* job queue for later servicing from task context. mpc834xDmaIsr() is called
* if a channel's interrupt enable bits are set in the DMAMR field
* for the current DMA descriptor.
*
*
* SEE ALSO: mpc834xDmaMemCopy(), mpc834xDmaSubmitJob(), mpc834xDmaPollStatus()
*
* NOMANUAL
*
*/

LOCAL void mpc834xDmaIsr(void)
    {
    UINT32 dmasr, i;

#ifdef DEBUG
    dmaIntCnt++;
#endif

    for (i = 0; i < MPC834XDMA_NUM_CHANNELS; i++)
        {
        dmasr = mpc834xDmaRead(&mpc834xDmaChannels[i], M83XX_DMASR_OFFSET);
#ifdef DEBUG
        logMsg("dmasr #%d = 0x%x\n", i, dmasr, 0, 0, 0, 0);
#endif
        if ((dmasr & (M83XX_DMASR_TE | M83XX_DMASR_EOCDI)))
            {
            mpc834xDmaWrite (&mpc834xDmaChannels[i], M83XX_DMASR_OFFSET, dmasr);  /* Clear bits */
            excJobAdd (mpc834xDmaExcJob, (int)&mpc834xDmaChannels[i], 0,0,0,0,0);
            }
        }
    }


/**********************************************************************
*
* mpc834xDoorbellWrite - set outbound doorbell register
*
* This routine writes the value <doorbelValue> to the outbound doorbell
* register.
*
* RETURNS: N/A
*
*/

void mpc834xDoorbellWrite
    (
    UINT32 doorbellValue  /* value to write to doorbell register */
    )
    {
    doorbellValue &= 0x1fffffff;  /* Only bits 0-28 are allowed, others must be zero */
    SYS_OUT_LONG(M83XX_ODR(CCS_BASE), doorbellValue);
    }


/**********************************************************************
*
* mpc834xDoorbellRead - read inbound doorbell register
*
* This routine reads the value of the inbound doorbell
* register, including the Inbound Machine Check bit (bit 31).
*
* RETURNS: Value of inbound doorbell register
*
*/

UINT32 mpc834xDoorbellRead(void)
    {
    return SYS_IN_LONG((UINT32 *)M83XX_IDR(CCS_BASE));
    }


/**********************************************************************
*
* mpc834xMsgRegRead - read MPC834X message register contents
*
* This routine returns the contents of the specified MPC834X message
* register.  Legal values for <whichMsgReg> are MPC834XDMA_MSG_REG0
* and MPC834XDMA_MSG_REG1.
*
* RETURNS: OK, or ERROR if <whichMsgReg> is not a legal value.
* In that case, <message> is not altered, otherwise
* <message> is populated with the value in the specified message
* register.
*
*/

STATUS mpc834xMsgRegRead
    (
    MPC834XDMA_MSG_REG whichMsgReg,  /* message register to read */
    UINT32 *message                  /* pointer to 32-bit message value that was read */
    )
    {
        switch (whichMsgReg)
            {
            case MPC834XDMA_MSG_REG0:
               *message = SYS_IN_LONG(M83XX_IMR0(CCS_BASE));
               break;
            case MPC834XDMA_MSG_REG1:
               *message = SYS_IN_LONG(M83XX_IMR1(CCS_BASE));
               break;
            default:
               return ERROR;
               break;
            }

    return OK;
    }


/**********************************************************************
*
* mpc834xMsgRegWrite - write MPC834X message register contents
*
* This routine writes the contents of the specified MPC834X message
* register.  Legal values for <whichMsgReg> are MPC834XDMA_MSG_REG0
* and MPC834XDMA_MSG_REG1.
*
* RETURNS: OK, or ERROR if <whichMsgReg> is not a legal value.
* In that case, the outgoing message register is not altered, otherwise
* the register is populated with the value in the specified by <message>.
*
*/

STATUS mpc834xMsgRegWrite
    (
    MPC834XDMA_MSG_REG whichMsgReg,  /* message register to write */
    UINT32 message                   /* message value to write */
    )
    {
        switch (whichMsgReg)
            {
            case MPC834XDMA_MSG_REG0:
               SYS_OUT_LONG(M83XX_OMR0(CCS_BASE), message);
               break;
            case MPC834XDMA_MSG_REG1:
               SYS_OUT_LONG(M83XX_OMR1(CCS_BASE), message);
               break;
            default:
               return ERROR;
               break;
            }

    return OK;
    }


/**********************************************************************
*
* mpc834xMsgDoorbellCallbackAdd - install message and doorbell callback routines
*
* This routine registers user function pointers as routines to be called each
* time a message or doorbell interrupt arrives.
*
* APIs for the handlers should be as follows:
*
* \cs
* void inboundMsgHandler(MPC834XDMA_MSG_REG messageReg, UINT32 message, UINT32 userArg);
* void outboundMsgHandler(MPC834XDMA_MSG_REG messageReg, UINT32 message, UINT32 userArg);
* void inDoorbellHandler(UINT32 doorbellValue, userArg);
* void outDoorbellHandler(UINT32 doorbellValue, userArg);
* \ce
* 
* For message interrupts, <messageReg> is passed to the API to indicate
* which message register is responsible for the interrupt (MPC834XDMA_MSG_REG0
* or MPC834XDMA_MSG_REG0), and <message> represents the message data itself.
* 
* For doorbell interrupts, <doorbellValue> is passed to the API to indicate
* the value of the corresponding doorbell register.
*
* Note that the handlers must be ISR-safe.  It may only execute code
* listed as safe to run from interrupt context, as desribed in the
* VxWorks Programmer's Guide.
*
* A NULL value for an handler argument indicates that no routine should
* be called for the corresponding interrupt type, and is the default
* value.
*
* RETURNS: OK, always
*
*/

STATUS mpc834xMsgDoorbellCallbackAdd
    (
    FUNCPTR inMsgHandler,        /* inbound message handler */
    UINT32  inMsgUserArg,        /* parameter to be passed back to user handler */
    FUNCPTR outMsgHandler,       /* outbound message handler */
    UINT32  outMsgUserArg,       /* parameter to be passed back to user handler */
    FUNCPTR inDoorbellHandler,   /* inbound doorbell handler */
    UINT32  inDoorbellUserArg,   /* parameter to be passed back to user handler */
    FUNCPTR outDoorbellHandler,  /* outbound doorbell handler */
    UINT32  outDoorbellUserArg   /* parameter to be passed back to user handler */
    )
    {
    mpc834xInMsgCallbackRtn = inMsgHandler;
    mpc834xInMsgCallbackArg = inMsgUserArg;

    mpc834xOutMsgCallbackRtn = outMsgHandler;
    mpc834xOutMsgCallbackArg = outMsgUserArg;

    mpc834xInDoorbellCallbackRtn = inDoorbellHandler;
    mpc834xInDoorbellCallbackArg = inDoorbellUserArg;

    mpc834xOutDoorbellCallbackRtn = outDoorbellHandler;
    mpc834xOutDoorbellCallbackArg = outDoorbellUserArg;

    return OK;
    }


/**********************************************************************
*
* mpc834xDoorbellMessageIsr - mpc834x doorbell & incoming message ISR
*
* mpc834xDoorbellMessageIsr is called from interrupt context, and is
* invoked when a doorbell interrupt or an incoming message interrupt
* arrives.It calls the the user's registered handler(s) (if any).
*
* NOMANUAL
*
*/

LOCAL void mpc834xDoorbellMessageIsr(void)
    {
    UINT32 imisr = 0, omisr = 0;
    UINT32 inboundMessage0  = 0, inboundMessage1  = 0, inboundDoorbellValue  = 0;
    UINT32 outboundMessage0 = 0, outboundMessage1 = 0, outboundDoorbellValue = 0;

#ifdef DEBUG
    doorbellMsgIntCnt++;
#endif

    imisr = SYS_IN_LONG(M83XX_IMISR(CCS_BASE));  /* fetch inbound message/doorbell interrupt status register */
    omisr = SYS_IN_LONG(M83XX_OMISR(CCS_BASE));  /* fetch outbound message/doorbell interrupt status register */

    SYS_OUT_LONG(M83XX_IMISR(CCS_BASE), imisr);  /* Clear inbound interrupt notification bits */
    SYS_OUT_LONG(M83XX_OMISR(CCS_BASE), omisr);  /* Clear outbound interrupt notification bits */

    /* Check for incoming message interrupts, call message callback routines */
    if (mpc834xInMsgCallbackRtn != NULL)
        {
        if (imisr & M83XX_MSG_REG_MGS0)
            {
            inboundMessage0 = SYS_IN_LONG(M83XX_IMR0(CCS_BASE))
            *(mpc834xInMsgCallbackRtn)(MPC834XDMA_MSG_REG0, inboundMessage0, mpc834xInMsgCallbackArg);
            }
        if (imisr & M83XX_MSG_REG_MGS1)
            {
            inboundMessage1 = SYS_IN_LONG(M83XX_IMR1(CCS_BASE))
            *(mpc834xInMsgCallbackRtn)(MPC834XDMA_MSG_REG1, inboundMessage1, mpc834xInMsgCallbackArg);
            }
        }

    /* Check for incoming doorbell interrupt, call doorbell callback routine */
    if (mpc834xInDoorbellCallbackRtn != NULL)
        {
        if (imisr & M83XX_MSG_REG_DOORBELL)
            {
            inboundDoorbellValue = SYS_IN_LONG(M83XX_IDR(CCS_BASE));
            (*mpc834xInDoorbellCallbackRtn)(inboundDoorbellValue, mpc834xInDoorbellCallbackArg);
            }
        }

    /* Check for outgoing message interrupts, call message callback routines */
    if (mpc834xOutMsgCallbackRtn != NULL)
        {
        if (omisr & M83XX_MSG_REG_MGS0)
            {
            outboundMessage0 = SYS_IN_LONG(M83XX_OMR0(CCS_BASE))
            *(mpc834xOutMsgCallbackRtn)(MPC834XDMA_MSG_REG0, outboundMessage0, mpc834xOutMsgCallbackArg);
            }
        if (omisr & M83XX_MSG_REG_MGS1)
            {
            outboundMessage1 = SYS_IN_LONG(M83XX_OMR1(CCS_BASE))
            *(mpc834xOutMsgCallbackRtn)(MPC834XDMA_MSG_REG1, outboundMessage1, mpc834xOutMsgCallbackArg);
            }
        }

    /* Check for outgoing doorbell interrupt, call doorbell callback routine */
    if (mpc834xOutDoorbellCallbackRtn != NULL)
        {
        if (omisr & M83XX_MSG_REG_DOORBELL)
            {
            outboundDoorbellValue = SYS_IN_LONG(M83XX_ODR(CCS_BASE));
            (*mpc834xOutDoorbellCallbackRtn)(outboundDoorbellValue, mpc834xOutDoorbellCallbackArg);
            }
        }

    }

