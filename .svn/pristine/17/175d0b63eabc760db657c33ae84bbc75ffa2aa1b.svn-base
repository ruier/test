/* sysSerial.c -  MPC827x SCC UART BSP serial device initialization */

/* Copyright 1984-2003 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/*
modification history
--------------------
01k,03oct04,dtr  SPR 98762.
01j,23dec03,dtr  modified to use dpram mem alloc library calls and 
                 SCC4 support.
01i,01oct03,dee  removed hardcoding of internal map base, use immrVal
01h,17oct01,jrs  Upgrade to veloce
		 corrected SCC2 initialization in sysSerialHwInit() - SPR #64407
		 removed useless init of baud rate generators - SPR #88989
01g,09sep99,ms_  change bsp name from vads8260 to ads8260
01f,15jul99,ms_  make compliant with our coding standards
01e,19apr99,ms_  adhere to coding conventions
01d,14apr99,ms_  switch from using interrupt vectors to interrupt numbers
01c,08apr99,ms_  upgrade to multiple channels
01b,05apr99,ms_  upgrade to support two channels
01a,21jan99,ms_  adapted from sysSerial.c for ads860 
*/

/*
DESCRIPTION

This library contains routines for M8260 SMC UART BSP serial device
initialization
*/

#include "vxWorks.h"
#include "iv.h"
#include "intLib.h"
#include "config.h"
#include "sysLib.h"
#include "drv/sio/m8260Scc.h"
#include "drv/sio/m8260Sio.h"
#include "drv/parallel/m8260IOPort.h"
#include "drv/sio/m8260Brg.h"

IMPORT    UINT32  vxImmrGet (void);

LOCAL M8260_SCC_CHAN m8260SccChan1;
LOCAL M8260_SCC_CHAN m8260SccChan2;


/******************************************************************************
*
* sysSerialHwInit - initialize the BSP serial devices to a quiesent state
*
* This routine initializes the BSP serial device descriptors and puts the
* devices in a quiesent state.  It is called from sysHwInit() with
* interrupts locked.
*
* Buffers and Buffer Descriptors for the two channels:
* 
* .CS
*                                                   Address per SCC
*                                                ---------------------------
* field                                 size        SCC1            SCC2
* ------                                -------  -----------     -----------
* Receive Buffer Descriptor             8 bytes IMMAP+0x0000    IMMAP+0x0100
* Receive Buffer Status                 2 bytes IMMAP+0x0000    IMMAP+0x0100
* Receive Buffer Length                 2 bytes IMMAP+0x0002    IMMAP+0x0102
* Pointer to Receive Buffer             4 bytes IMMAP+0x0004    IMMAP+0x0104
* Receive Buffer                        1 bytes IMMAP+0x0040    IMMAP+0x0140
*
* Transmit Buffer Descriptor            8 bytes IMMAP+0x0008    IMMAP+0x0108
* Transmit Buffer Status                2 bytes IMMAP+0x0008    IMMAP+0x0108
* Transmit Buffer Length                2 bytes IMMAP+0x000A    IMMAP+0x010A
* Transmit to Receive Buffer            4 bytes IMMAP+0x000C    IMMAP+0x010C
* Transmit Buffer                       1 bytes IMMAP+0x0060    IMMAP+0x0160
* .CE
*
*
*
* RETURNS: N/A
*/ 

void sysSerialHwInit (void)
    {

    int immrVal = vxImmrGet();
    char * bufferBase;


    /* SCC1 */
    * M8260_IOP_PDDIR(immrVal) &= ~(PD31 | PC30);  
    * M8260_IOP_PDDIR(immrVal) |= (PD30);   	
    * M8260_IOP_PDPAR(immrVal) |= (PD31 | PD30); 
    * M8260_IOP_PDSO(immrVal) &= ~(PD31 | PD30); 	
    * M8260_IOP_PDSO(immrVal) |= PD30; 

    /* SCC 4 */
    * M8260_IOP_PDSO(immrVal) &= ~(PD22 | PC21);
    * M8260_IOP_PDDIR(immrVal) &= ~(PD22);
    * M8260_IOP_PDDIR(immrVal) |= (PD21);
    * M8260_IOP_PDPAR(immrVal) |= (PD22 | PD21);
    

    /* CPM muxs */

    /* for the value of 0x0009121B:
     * field    description     SCC1    SCC2    SCC3    SCC4
     * GR       Grant Support   <-----always asserted------>
     * SC       SCC connection  <---not connected to TSA--->
     * RSxCS    Rcv Clk Source  BRG1    BRG2    BRG3    BRG4
     * TSxCS    Tx Clk Source   BRG1    BRG2    BRG3    BRG4
     */

    * M8260_CMXSCR(immrVal) = 0x0009121B;

    /* enable both RS232 ports on the board */

    *BCSR1 &= ~(BCSR1_RS232EN_1_L);
    *BCSR1 &= ~(BCSR1_RS232EN_2_L);

    /* intialize the two serial channels */

    /* this is in order of the structure contents */

    /* indicate that this is a fresh device */
    m8260SccChan1.channelMode = 0;
    m8260SccChan2.channelMode = 0;

    m8260SccChan1.baudRate = DEFAULT_BAUD;
    m8260SccChan2.baudRate = DEFAULT_BAUD;

    m8260SccChan1.sccNum = 1;
    m8260SccChan2.sccNum = 4;

    m8260SccChan1.immrVal = immrVal;
    m8260SccChan2.immrVal = immrVal;

    bufferBase = (char *)m82xxDpramAlignedMalloc(0x200,0x100);
    
    memset ((char*)bufferBase,0x0,0x200);
    m8260SccChan1.pBdBase = (char *) ((UINT32)bufferBase + 0x000);
    m8260SccChan1.rcvBufferAddr = (char *) ((UINT32)bufferBase + 0x040);
    m8260SccChan1.txBufferAddr = (char *) ((UINT32)bufferBase + 0x060);

    m8260SccChan2.pBdBase = (char *) ((UINT32)bufferBase + 0x100);
    m8260SccChan2.rcvBufferAddr = (char *) ((UINT32)bufferBase + 0x140);
    m8260SccChan2.txBufferAddr = (char *) ((UINT32)bufferBase + 0x160);


    /* disable interrupts from SCC1 and SCC2 */

    m8260IntDisable(INUM_SCC1);

#if (NUM_TTY >= 2)
	m8260IntDisable(INUM_SCC4);
#endif

    /* reset the channel */

    m8260SioDevInit(&m8260SccChan1);

#if (NUM_TTY >= 2)
		m8260SioDevInit(&m8260SccChan2);
#endif

    }

/******************************************************************************
*
* sysSerialHwInit2 - connect BSP serial device interrupts
*
* This routine connects the BSP serial device interrupts.  It is called from
* sysHwInit2().  Serial device interrupts could not be connected in
* sysSerialHwInit() because the kernel memory allocator was not initialized
* at that point, and intConnect() calls malloc().
*
* RETURNS: N/A
*/ 

void sysSerialHwInit2 (void)
    {

    /* connect serial interrupts */

    (void) intConnect (INUM_TO_IVEC(INUM_SCC1), 
		      (VOIDFUNCPTR) m8260SioInt, (int) &m8260SccChan1);
#if (NUM_TTY >= 2)
		(void) intConnect (INUM_TO_IVEC(INUM_SCC4), 
			  (VOIDFUNCPTR) m8260SioInt, (int) &m8260SccChan2);
#endif

    }

/******************************************************************************
*
* sysSerialChanGet - get the SIO_CHAN device associated with a serial channel
*
* This routine gets the SIO_CHAN device associated with a specified serial
* channel.
*
* RETURNS: A pointer to the SIO_CHAN structure for the channel, or ERROR
* if the channel is invalid.
*/

SIO_CHAN * sysSerialChanGet
    (
    int channel		/* serial channel */
    )
    {
    if (channel == 0)
		return ((SIO_CHAN *) &m8260SccChan1); 
    else if (channel == 1)
		return ((SIO_CHAN *) &m8260SccChan2); 
    else 
		return ((SIO_CHAN *) ERROR);
    }

/*******************************************************************************
*
* sysSerialReset - reset the serail device 
*
* This function calls sysSerialHwInit() to reset the serail device
*
* RETURNS: N/A
*
*/

void sysSerialReset (void)
    {
    sysSerialHwInit ();
    }
