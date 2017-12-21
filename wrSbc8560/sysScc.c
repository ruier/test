/* sysScc.c -  M8260 SCC UART BSP serial device initialization */

/* Copyright 1984-2002 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/*
modification history
--------------------
01j,26aug03,dtr  Using 82xx DramLib for buffer allocation.
01i,01aug03,dtr  Using CCSBAR for DPRAM base adrs.
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
#include "wrSbc85xx.h"

#define SCC_BASE (CCSBAR + 0x80000)

M8260_SCC_CHAN m8260SccChan1;
M8260_SCC_CHAN m8260SccChan2;
char * bufferBase;

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
*                                               ---------------------------
* field                                 size    SCC1            SCC2
* ------                                ------- -----------     -----------
* Receive Buffer Descriptor             8 bytes 0x0470_0000     0x0470_0100
* Receive Buffer Status                 2 bytes 0x0470_0000     0x0470_0100
* Receive Buffer Length                 2 bytes 0x0470_0002     0x0470_0102
* Pointer to Receive Buffer             4 bytes 0x0470_0004     0x0470_0104
* Receive Buffer                        1 bytes 0x0470_0040     0x0470_0140
*
* Transmit Buffer Descriptor            8 bytes 0x0470_0008     0x0470_0108
* Transmit Buffer Status                2 bytes 0x0470_0008     0x0470_0108
* Transmit Buffer Length                2 bytes 0x0470_000A     0x0470_010A
* Transmit to Receive Buffer            4 bytes 0x0470_000C     0x0470_010C
* Transmit Buffer                       1 bytes 0x0470_0060     0x0470_0160
* .CE
*
*
*
* RETURNS: N/A
*/ 

void sysSerialHwInit (void)
    {

    UINT32 immrVal = SCC_BASE;

    * M8260_IOP_PCDIR(immrVal) &= 0xFFFDFFFF;
    * M8260_IOP_PCDIR(immrVal) &= ~0x000A0000; /* clr bits 12, 13 for CD in */
    * M8260_IOP_PCPAR(immrVal) |= 0x00020000;
    * M8260_IOP_PCSO(immrVal) &= 0xFFFDFFFF;
    * M8260_IOP_PCODR(immrVal) &= 0xFFFDFFFF;
    * M8260_IOP_PCDAT(immrVal) = 0x00000000;
    * M8260_IOP_PDDIR(immrVal) &= 0xFFFFFFFC;   	/* clear first */
    * M8260_IOP_PDDIR(immrVal) |= 0x00000002;   	/* TXD pin 30| RXD pin 31 */
    * M8260_IOP_PDPAR(immrVal) |= 0x00000003;   	/* TXD pin 30| RXD pin 31 */
    * M8260_IOP_PDPAR(immrVal) |= 0x00000004;   	/* set bit 29 for SCC2*/
    * M8260_IOP_PDSO(immrVal) &= 0xFFFFFFF8; 	/* clear first */
    * M8260_IOP_PDSO(immrVal) |= 0x00000002; 	/* TXD pin 30 and RXD pin 31 */

if (NUM_TTY>=2)
    {
    * M8260_IOP_PCPAR(immrVal) |= 0x000A0000; /* set bits 12 for CD in */
    * M8260_IOP_PCSO(immrVal) &= ~0x000A0000; /* clr bits 14 for CD in */
    * M8260_IOP_PDDIR(immrVal) |= 0x00000010;   	/* set bit 27 for SCC2 */
    * M8260_IOP_PDDIR(immrVal) &= ~0x00000008;   	/* clear bit 28 for SCC2 */
    * M8260_IOP_PDPAR(immrVal) |= 0x00000018;   	/* set bits 27, 28 for SCC2*/
    * M8260_IOP_PDPAR(immrVal) |= 0x00000020;   	/* set bit 26 for SCC2*/
    * M8260_IOP_PDSO(immrVal) &= ~0x00000018; 	/* clear bits 27, 28 for SCC2 */
    * M8260_IOP_PDSO(immrVal) &= ~0x00000020; 	/* clear bit 26 for SCC2 */
    * M8260_IOP_PDODR(immrVal) &= 0xFFFFFFED;	/* TXD pin 28 | RXD pin 27 */
    * M8260_IOP_PDDAT(immrVal) = 0x00000000;

    }
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


/* intialize the two serial channels */

/* this is in order of the structure contents */

    /* indicate that this is a fresh device */
    m8260SccChan1.channelMode = 0;
    m8260SccChan2.channelMode = 0;

    m8260SccChan1.baudRate = DEFAULT_BAUD;
    m8260SccChan2.baudRate = DEFAULT_BAUD;

    m8260SccChan1.sccNum = 1;
    m8260SccChan2.sccNum = 2;

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

    m85xxCpmIntDisable(INUM_CPM_SCC1);

    if (NUM_TTY >= 2)
	m85xxCpmIntDisable(INUM_CPM_SCC2);

    /* reset the channel */

    m8260SioDevInit(&m8260SccChan1);

    if (NUM_TTY >= 2)
	m8260SioDevInit(&m8260SccChan2);

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

    (void) m85xxCpmIntConnect (INUM_CPM_SCC1, 
		      (VOIDFUNCPTR) m8260SioInt, (int*) &m8260SccChan1);
    if (NUM_TTY >= 2)
	(void) m85xxCpmIntConnect (INUM_CPM_SCC2, 
			  (VOIDFUNCPTR) m8260SioInt, (int*) &m8260SccChan2);

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
