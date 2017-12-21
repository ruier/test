/* cfiscs.c - MTD for devices that comply with CFI and the Intel SCS */

/*
 * Copyright (c) 2003-2005 Wind River Systems, Inc.
 *
 * The right to copy, distribute or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement. No license to Wind River intellectual property rights
 * is granted herein. All rights not licensed by Wind River are reserved
 * by Wind River.
 */
 
#include "copyright_wrs.h"

/* FAT-FTL Lite Software Development Kit
 * Copyright (C) M-Systems Ltd. 1995-2001       */

/*
modification history
--------------------
01l,23mar05,mdo  Documentation fixes for apigen
01k,23nov04,mil  Fixed compilation warnings.
01j,16nov04,mil  Added 32-bit write and bootrom protection for wrSbc85xx.
01i,16oct03,mil  Added workaround for wrSbc85xx user flash and debug info.
01h,24jun02,nrv  fixing the condition where flashPtr[0x24] is checked twice
01g,12dec01,yp   removing assumption that the InterleavedWordWrite call might
                 have to support 16 bit commands. reworked identification of
                 two 16 bit devices and single 32 bit device. made erase
                 routines mutually exclusive at compile time. removed dataWidth
		 and deviceWidth members from CFI structure. removed unused 
		 MakeCmd function.
01f,11dec01,nrv  fixed build error
01e,29oct01,yp   Converting to WRS coding convention. Adding support for double
		 word width devices. Converting identification process to byte 
		 offsets. Providing alternative write mechanism for none 
		 functional WRITE_TO_BUFFER platforms.
01d,.......,vf   Added for debugging of this MTD.
01c,06apr99,vf   Try to make endian-safe.  LONGSWAP and WORDSWAP are 
		 #define'd in vxWorks.h.
01b,09jan99,an   Added support for machines that require a full 32bit write to 
		 access fall correctly. Also fixed code to correctly compute 
		 total Flash size, virtual block size, and combined write buffer
		 size when interleaved parts are detected.
01a,27may97,msys original version from M-Systems
*/

/* Original CVS history
 * $Log: cfiscs.c,v $
 * Revision 1.1  2000/07/13 01:46:37  caluml
 * Added TFFS and bootParams stuff
 *
 * Revision 1.1.1.1  2000/05/31 00:09:33  caluml
 * T201 Arm Ugl2
 *
 * Revision 1.1  2000/04/14 16:26:14  caluml
 * This version is for the Assabet board with 32Mb of StrataFlash memory
 *
 *
 *    Rev 1.20   11 Nov 1997 15:26:00   ANDRY
 * get rid of compiler warnings
 *
 *    Rev 1.19   06 Oct 1997  9:53:30   danig
 * VPP functions under #ifdef
 *
 *    Rev 1.18   18 Sep 1997 10:05:40   danig
 * Warnings
 *
 *    Rev 1.17   10 Sep 1997 16:31:16   danig
 * Got rid of generic names
 *
 *    Rev 1.16   04 Sep 1997 18:19:34   danig
 * Debug messages
 *
 *    Rev 1.15   31 Aug 1997 14:50:52   danig
 * Registration routine return status
 *
 *    Rev 1.14   27 Jul 1997 15:00:38   danig
 * FAR -> FAR0
 *
 *    Rev 1.13   21 Jul 1997 19:58:24   danig
 * No watchDogTimer
 *
 *    Rev 1.12   15 Jul 1997 19:18:32   danig
 * Ver 2.0
 *
 *    Rev 1.11   09 Jul 1997 10:58:52   danig
 * Fixed byte erase bug & changed identification routines
 *
 *    Rev 1.10   20 May 1997 14:48:02   danig
 * Changed overwrite to mode in write routines
 *
 *    Rev 1.9   18 May 1997 13:54:58   danig
 * JEDEC ID independent
 *
 *    Rev 1.8   13 May 1997 16:43:10   danig
 * Added getMultiplier.
 *
 *    Rev 1.7   08 May 1997 19:56:12   danig
 * Added cfiscsByteSize
 *
 *    Rev 1.6   04 May 1997 14:01:16   danig
 * Changed cfiscsByteErase and added multiplier
 *
 *    Rev 1.4   15 Apr 1997 11:38:52   danig
 * Changed word identification and IDs.
 *
 *    Rev 1.3   15 Jan 1997 18:21:40   danig
 * Bigger ID string buffers and removed unused definitions.
 *
 *    Rev 1.2   08 Jan 1997 14:54:06   danig
 * Changes in specification
 *
 *    Rev 1.1   25 Dec 1996 18:21:44   danig
 * Initial revision
 */

/*
DESCRIPTION
This library provides an MTD for devices that conform to the Common Flash 
Interface (CFI) and the Intel Scalable Command Set (SCS). The library provides 
identification, erase and write routines for 8, 16, and 32 bit devices in 
native or byte mode. Support is also added for word width devices 
interleaved in word width mode where bytes 0 and 1 would be on device one 
and bytes 2 and 3 would be on device two. Support is also present for 
configurations that only permit double word width writes.

The identification process supports byte, word, and double word width devices 
as specified by the CFI query process. Vendor specific extensions are made 
available from an instance specific data structure, referred to in this file 
as this CFI. However, the driver remains vendor neutral in the hope that it 
will be applicable to a larger range of devices.

In addition to supporting a boot code region this driver is also able to 
reserve the last erase block of each device in the array so the vxWorks may
use this region as NVRAM if the macro SAVE_NVRAM_REGION is defined.

Some hardware using word width interleaved devices permit only 32 bit writes.
In these environments the macro INTERLEAVED_MODE_REQUIRES_32BIT_WRITES must 
be defined. The erase routine in particular and some of the write routines are 
impacted by this. 

BSPs tested:

BSP		Device		Socket		Defines
Assabet		StrataFlash	RFA		32_BIT_WRITES
						SAVE_NVRAM
ddb5476		StrataFlash	RFA		
pcPentum3	StrataFlash	PCMCIA
ads860		StrataFlash	PCMCIA
prpm800		StrataFlash	RFA		BUFFER_WRITE_BROKEN
integrator	StrataFlash	RFA		SAVE_NVRAM
(720/920)

INCLUDE FILES:
 
\NOMANUAL
*/

#include "vxWorks.h"
#include "config.h"
#include "stdio.h"
#include "taskLib.h"
#include "logLib.h"

#ifdef INCLUDE_TFFS

#include <stdio.h>
#include "tffs/flflash.h"
#include "tffs/backgrnd.h"


/* Instance specific CFI data so that a system may have more than one CFI
 * device. */
typedef struct {

  unsigned         commandSetId;            /* id of a specific command set. */
  unsigned         altCommandSetId;         /* id of alternate command set.  */
  FLBoolean        wordMode;                /* TRUE - word mode.             */
					    /* FALSE - byte mode.            */
  int              multiplier;              /* the number of bytes in 	     */
					    /* addressing scheme	     */
  int		   interleaveFactor;	    /* Number of bytes interleaved   */	
  unsigned         maxBytesWrite;           /* maximum number of bytes       */
					    /* in multi-byte write.          */
  FLBoolean        vpp;                     /* if = TRUE, need vpp.          */
  long             optionalCommands;        /* optional commands supported   */
					    /* (1 = yes, 0 = no):            */
					    /* bit 0 - chip erase.           */
					    /* bit 1 - suspend erase.        */
					    /* bit 2 - suspend write         */
					    /* bit 3 - lock/unlock.          */
					    /* bit 4 - queued erase.         */
  unsigned         afterSuspend;            /* functions supported after     */
					    /* suspend (1 = yes, 0 = no):    */
					    /* bit 0 - write after erase     */
					    /*         suspend.              */
} CFI;

LOCAL CFI mtdVars[DRIVES];


/* defines */

/* 
 * define the following if the target requires interleaves to be 
 * full 32 bit write accesses when erasing blocks. The Assabet BSP
 * is the only one requiring this so far.
 */
#undef INTERLEAVED_MODE_REQUIRES_32BIT_WRITES

/* Save the last erase block on each device discovered in the array 
 * for NVRAM */
#ifdef  INCLUDE_TFFS_BOOTROM_OVERWRITE	/* wrSbc85xx */
#undef  SAVE_NVRAM_REGION
#else   /* INCLUDE_TFFS_BOOTROM_OVERWRITE */
#define SAVE_NVRAM_REGION
#endif  /* INCLUDE_TFFS_BOOTROM_OVERWRITE */

/* disable debugging */
#undef CFI_DEBUG

#ifdef CFI_DEBUG
#define DEBUG_PRINT    printf
#else
#undef DEBUG_PRINT
#endif

/* define the following if the WRITE_TO_BUFFER command does not work
 * The value of maxBytesWrite is then forced to 1, resulting in either 
 * byte or word writes being used
 */
#define  BUFFERED_WRITE_BROKEN

#ifndef _BYTE_ORDER
#error "Error: _BYTE_ORDER needs to be #define'd.  Try #include \"vxWorks.h\" "
#endif

#if (_BYTE_ORDER == _BIG_ENDIAN)

#define CFI_LONGSWAP(x)	LONGSWAP(x)	/* swap everything in a long word */
#define CFI_WORDSWAP(x) WORDSWAP(x)	/* swap the shorts in a long */
#define CFI_BYTESWAP(x) ( ( (x & 0xff) << 8) | ( (x >> 8) & 0xff) )  /* Swap bytes in a short */

#else

#define CFI_LONGSWAP(x)	(x)		/* pass it as is */
#define CFI_WORDSWAP(x) (x)		/* pass it as is */
#define CFI_BYTESWAP(x) (x)		/* pass it as is */

#endif /* _BYTE_ORDER == _BIG_ENDIAN */

#define CFI_LONG_WRITE(addr, data)  (*(ULONG *)addr = (ULONG)(CFI_LONGSWAP(data)))
#define CFI_WORD_WRITE(addr, data)  (*(USHORT *)addr = (USHORT)(CFI_WORDSWAP(data)))
#define CFI_WORD_READ(addr)         (CFI_BYTESWAP(*(USHORT *)addr))
#define CFI_LONG_READ(addr)         (CFI_WORDSWAP(*(ULONG *)addr))

/* JEDEC-IDs */

#define VOYAGER_ID              0x8915
#define KING_COBRA_ID           0xb0d0

/* command set IDs */

#define INTEL_COMMAND_SET      0x0001
#define AMDFUJ_COMMAND_SET     0x0002
#define INTEL_ALT_COMMAND_SET  0x0001
#define AMDFUJ_ALT_COMMAND_SET 0x0004
#define ALT_NOT_SUPPORTED      0x0000


/* CFI identification strings */

#define ID_STR_LENGTH      3
#define QUERY_ID_STR       "QRY"
#define PRIMARY_ID_STR     "PRI"
#define ALTERNATE_ID_STR   "ALT"


/* commands */

#define CONFIRM_SET_LOCK_BIT    0x01
#define PROGRAM_WORD            0x10
#define SETUP_BLOCK_ERASE       0x20
#define SETUP_QUEUE_ERASE       0x28
#define SETUP_CHIP_ERASE        0x30
#define PROGRAM                 0x40
#define CLEAR_STATUS            0x50
#define SET_LOCK_BIT            0x60
#define CLEAR_LOCK_BIT          0x60
#define READ_STATUS             0x70
#define READ_ID                 0x90
#define QUERY                   0x98
#define SUSPEND_WRITE           0xb0
#define SUSPEND_ERASE           0xb0
#define CONFIG                  0xb8
#define CONFIRM_WRITE           0xd0
#define RESUME_WRITE            0xd0
#define CONFIRM_ERASE           0xd0
#define RESUME_ERASE            0xd0
#define CONFIRM_CLEAR_LOCK_BIT  0xd0
#define WRITE_TO_BUFFER         0xe8
#define READ_ARRAY              0xff


/* status register bits */

#define WSM_ERROR               0x3a
#define SR_BLOCK_LOCK           0x02
#define SR_WRITE_SUSPEND        0x04
#define SR_VPP_ERROR            0x08
#define SR_WRITE_ERROR          0x10
#define SR_LOCK_SET_ERROR       0x10
#define SR_ERASE_ERROR          0x20
#define SR_LOCK_RESET_ERROR     0x20
#define SR_ERASE_SUSPEND        0x40
#define SR_READY                0x80


/* optional commands support */

#define CHIP_ERASE_SUPPORT           0x0001
#define SUSPEND_ERASE_SUPPORT        0x0002
#define SUSPEND_WRITE_SUPPORT        0x0004
#define LOCK_SUPPORT                 0x0008
#define QUEUED_ERASE_SUPPORT         0x0010


/* supported functions after suspend */

#define WRITE_AFTER_SUSPEND_SUPPORT  0x0001

#define thisCFI   ((CFI *)vol.mtdVars)

/******************************************************************************
* cfiscsGetChipCount - return the number of chips in this array
*
* Leave the first chip (or set of interleaved chips) in READ_ARRAY
* mode and move forward by chip size (or interleaved chip size) until
* address wrap around is detected. The chip count is updated in 
* vol.noOfChips.
*
* RETURNS:
*
* ERRNO:
*
* \NOMANUAL
*/

LOCAL FLStatus cfiscsGetChipCount
    (
    FLFlash vol           /* Pointer identifying drive */
    )
    {
    UINT32 cmdBuffer;
    int ix;
    char queryIdStr[ID_STR_LENGTH + 1] = QUERY_ID_STR;

    FlashPTR flashPtr = (FlashPTR) flMap(vol.socket, 0);

    /* Switch first (interleaved) chip set to QUERY mode */
    cmdBuffer = 0x0;
    for (ix = 0; ix < thisCFI->multiplier; ix++)
        cmdBuffer |= QUERY << (8 * ix);
    flashPtr[0x55 * thisCFI->multiplier] = cmdBuffer;

    /* NOTE: The address wrap around technique used here works only if the flash
     * size was declared correctly in the socket driver. At some point the MTD
     * should be discovering flash size and exporting it to the socket driver. 
     */

    for (vol.noOfChips = 1 * vol.interleaving;       /* Scan the chips */
        vol.noOfChips < 2000;  /* Big enough ? */
        vol.noOfChips += vol.interleaving)
        {
#ifdef SAVE_NVRAM_REGION
        if (vol.interleaving == 1)  /* wrSbc85xx boot flash only */
            {
            /* Reinstate the NVRAM region */
            flashPtr = (FlashPTR) flMap(vol.socket, vol.noOfChips * 
                                        (vol.chipSize + 
                                         (vol.erasableBlockSize / vol.interleaving)));
            }
        else
            {
            flashPtr = (FlashPTR) flMap(vol.socket, vol.noOfChips * vol.chipSize);
            }
#else
        flashPtr = (FlashPTR) flMap(vol.socket, vol.noOfChips * vol.chipSize);
#endif

#ifdef DEBUG_PRINT
        DEBUG_PRINT("Debug: Looking for Flash device at base 0x%x\n", (UINT32) flashPtr);
#endif
        if ((flashPtr[0x10 * thisCFI->multiplier]) 
            == (unsigned char)queryIdStr[0] &&
            (flashPtr[0x11 * thisCFI->multiplier]) 
            == (unsigned char)queryIdStr[1] &&
            (flashPtr[0x12 * thisCFI->multiplier]) 
            == (unsigned char)queryIdStr[2])
            {
            goto noMoreChips;      /* Wrapped around */
            }

        /* Confirm this is also a CFI part */
        *(UCHAR *)(flashPtr + (0x55 * thisCFI->multiplier))
        = (UCHAR)(QUERY);
        if (flashPtr[0x10 * thisCFI->multiplier]
            != (unsigned char)queryIdStr[0] ||
            flashPtr[0x11 * thisCFI->multiplier]
            != (unsigned char)queryIdStr[1] ||
            flashPtr[0x12 * thisCFI->multiplier]
            != (unsigned char)queryIdStr[2])
            {
            /* Should not happen !! */
            goto noMoreChips;   /* Not a CFI Flash part */
            }

        flashPtr[0x55 * thisCFI->multiplier] 
        = (UCHAR)(CLEAR_STATUS);
        flashPtr[0x55 * thisCFI->multiplier] 
        = (UCHAR)(READ_ARRAY);
        }
    /* We should never get here. */
#ifdef DEBUG_PRINT
    DEBUG_PRINT("Debug: Device count overflow\n");
#endif  /* DEBUG_PRINT */
    return flGeneralFailure;

    noMoreChips:
    flashPtr = (FlashPTR) flMap(vol.socket, 0);
    /* Switch to READ_ARRAY mode */
    cmdBuffer = 0x0;
    for (ix = 0; ix < thisCFI->multiplier; ix++)
        cmdBuffer |= READ_ARRAY << (8 * ix);
    flashPtr[0x55 * thisCFI->multiplier] = cmdBuffer;

    return flOK;
    }

/******************************************************************************
* cfiscsByteWrite - byte write routine
*
* This routine is intended to be registered as the write routine in systems
* that have byte wide devices interleaved or devices that are in byte mode.
*
* RETURNS:
*
* ERRNO:
*
* \NOMANUAL
*/

LOCAL FLStatus cfiscsByteWrite
    (
    FLFlash vol,               /* Pointer identifying drive     */
    CardAddress address,       /* Card address to write to      */
    const void FAR1 *buffer,   /* Address of data to write      */
    int length,                /* Number of bytes to write      */
    int mode                   /* write mode (overwrite yes/no) */
    )
    {
    FLStatus status = flOK;
    FlashPTR flashPtr;
    unsigned int i, from, eachWrite;
    const char FAR1 *temp = (const char FAR1 *)buffer;
    /* Set timeout to 20 seconds from now */
    unsigned long writeTimeout = flMsecCounter + 20000;

    printf ("CFISCS: writing bytes to flash offset 0x%x\n", address);

    if (flWriteProtected(vol.socket))
        return flWriteProtect;

#ifdef SOCKET_12_VOLTS
    if (thisCFI->vpp)
        checkStatus(flNeedVpp(vol.socket));
#endif

    if (thisCFI->maxBytesWrite > 1) /* multi-byte write supported */
        eachWrite = thisCFI->maxBytesWrite * vol.interleaving;
    else
        eachWrite = vol.interleaving;

    for (from = 0; (int)from < length && status == flOK; from += eachWrite)
        {
        int thisLength = length - from;
        FlashPTR currPtr;
        unsigned tailBytes, lengthByte;

        if (thisLength > (int)eachWrite)
            thisLength = eachWrite;

        lengthByte = thisLength / vol.interleaving;
        tailBytes = thisLength % vol.interleaving;

        flashPtr = (FlashPTR) flMap(vol.socket, address + from);

        for (i = 0, currPtr = flashPtr;
            ((int)i < vol.interleaving) && ((int)i < thisLength);
            i++, currPtr++)
            {
            do 
                {
                *currPtr = WRITE_TO_BUFFER;
                } while (!(*currPtr & SR_READY) && 
                         (flMsecCounter < writeTimeout));
            if (!(*currPtr & SR_READY))
                {
#ifdef DEBUG_PRINT
                DEBUG_PRINT("Debug: timeout error in CFISCS write.\n");
#endif
                status = flWriteFault;
                }
            *currPtr = i < tailBytes ? lengthByte : lengthByte - 1;
            }

        tffscpy((unsigned long FAR0 *) flashPtr,temp + from,thisLength);

        for (i = 0, currPtr = flashPtr;
            ((int)i < vol.interleaving) && ((int)i < thisLength);
            i++, currPtr++)
            *currPtr = CONFIRM_WRITE;

        for (i = 0, currPtr = flashPtr;
            ((int)i < vol.interleaving) && ((int)i < thisLength);
            i++, currPtr++)
            {
            while (!(*currPtr & SR_READY) && (flMsecCounter < writeTimeout))
                ;
            if (!(*currPtr & SR_READY))
                {
#ifdef DEBUG_PRINT
                DEBUG_PRINT("Debug: timeout error in CFISCS write.\n");
#endif
                status = flWriteFault;
                }
            if (*currPtr & WSM_ERROR)
                {
#ifdef DEBUG_PRINT
                DEBUG_PRINT("Debug: error in CFISCS write.\n");
#endif
                status = (*currPtr & SR_VPP_ERROR) ? 
                         flVppFailure : flWriteFault;
                *currPtr = CLEAR_STATUS;
                }
            *currPtr = READ_ARRAY;
            }
        }

#ifdef SOCKET_12_VOLTS
    if (thisCFI->vpp)
        flDontNeedVpp(vol.socket);
#endif

    flashPtr = (FlashPTR) flMap(vol.socket, address);
    /* verify the data */
    if (status == flOK && tffscmp((void FAR0 *) flashPtr,buffer,length))
        {
#ifdef DEBUG_PRINT
        DEBUG_PRINT("Debug: CFISCS byte write failed in verification.\n");
#endif
        status = flWriteFault;
        }

    return status;

    }

/*****************************************************************************
* cfiscsWordWrite - write routine for 16 bit devices 
*                                                                     
* Write a block of bytes to Flash in a word-mode.                    
*                                                                   
* This routine will be registered as the MTD flash.write routine   
*                                                                                                                          
* RETURNS:                                                
*      FLStatus        : 0 on success, failed otherwise  
*
* ERRNO:
*
* \NOMANUAL
*/
LOCAL FLStatus cfiscsWordWrite
    (
    FLFlash vol,               /* Pointer identifying drive     */
    CardAddress address,       /* Card address to write to      */
    const void FAR1 *buffer,   /* Address of data to write      */
    int length,                /* Number of bytes to write      */
    int mode                   /* write mode (overwrite yes/no) */
    )
    {
    FLStatus status = flOK;
    FlashWPTR flashPtr;
    int from, eachWrite;
    const char FAR1 *temp = (const char FAR1 *)buffer;
    /* Set timeout to 5 seconds from now */
    unsigned long writeTimeout = flMsecCounter + 5000;

#ifdef DEBUG_PRINT
    DEBUG_PRINT("Debug: entering CFISCS word write routine.\n");
#endif

    if (flWriteProtected(vol.socket))
        return flWriteProtect;

    if ((length & 1) || (address & 1)) /* Only write words on word-boundary */
        return flBadParameter;

#ifdef SOCKET_12_VOLTS
    if (thisCFI->vpp)
        checkStatus(flNeedVpp(vol.socket));
#endif

    if (thisCFI->maxBytesWrite > 1) /* multi-byte write supported */
        eachWrite = thisCFI->maxBytesWrite / 2;   /* we are counting words */
    else
        eachWrite = 1;

    /* we assume that the interleaving is 1. */
    for (from = 0; (from < length / 2) && (status == flOK); from += eachWrite)
        {
        int thisLength = (length / 2) - from;

        if (thisLength > eachWrite)
            thisLength = eachWrite;

        flashPtr = (FlashWPTR)flMap(vol.socket, address + from * 2);

        if (thisLength == 1)
            {
            *flashPtr = PROGRAM ;
            *flashPtr = * ( (FlashWPTR) (temp + from * 2) ) ;
            }
        else
            {
            /* count > 1 so do a multiple write */
            do 
                {
                *flashPtr = WRITE_TO_BUFFER;
                } while (!(*flashPtr & SR_READY) && (flMsecCounter < writeTimeout));
            if (!(*flashPtr & SR_READY))
                {
#ifdef DEBUG_PRINT
                DEBUG_PRINT("Debug: timeout error in CFISCS write.\n");
#endif
                status = flWriteFault;
                }
            *flashPtr = thisLength - 1;

            tffscpyWords((unsigned long FAR0 *) flashPtr, temp + from * 2, 
                         thisLength * 2);

            *flashPtr = CONFIRM_WRITE;
            }
        while (!(*flashPtr & SR_READY) && (flMsecCounter < writeTimeout))
            ;
        if (!(*flashPtr & SR_READY))
            {
#ifdef DEBUG_PRINT
            DEBUG_PRINT("Debug: timeout error in CFISCS write.\n");
#endif
            status = flWriteFault;
            }
        if (*flashPtr & WSM_ERROR)
            {
#ifdef DEBUG_PRINT
            DEBUG_PRINT("Debug: CFISCS write error.\n");
#endif
            status = (*flashPtr & SR_VPP_ERROR) ? flVppFailure : flWriteFault;
            *flashPtr = CLEAR_STATUS;
            }
        *flashPtr = READ_ARRAY;
        }

#ifdef SOCKET_12_VOLTS
    if (thisCFI->vpp)
        flDontNeedVpp(vol.socket);
#endif

    flashPtr = (FlashWPTR) flMap(vol.socket, address);
    /* verify the data */
    if (status == flOK && tffscmp((void FAR0 *) flashPtr,buffer,length))
        {
#ifdef DEBUG_PRINT
        DEBUG_PRINT("Debug: CFISCS word write failed in verification.\n");
#endif
        status = flWriteFault;
        }

    return status;
    }

/******************************************************************************
* cfiscsInterleavedWordWrite - write routine 16 bit interleaved devices
*                                                                 
* This routine will be registered as the MTD flash.write routine      
*                                                                                                                                 
* RETURNS:                                                   
*      FLStatus        : 0 on success, failed otherwise     
*
* ERRNO:
*
* \NOMANUAL
*/
LOCAL FLStatus cfiscsInterleavedWordWrite
    (
    FLFlash vol,                /* Pointer identifying drive     */
    CardAddress address,        /* Card address to write to      */
    const void FAR1 *buffer,    /* Address of data to write      */
    int length,                 /* Number of bytes to write      */
    int mode                    /* write mode (overwrite yes/no) */
    )                           
    {
    FLStatus status = flOK;
    FlashWPTR flashPtr;
    unsigned long maxWordsPerWrite,  /* Number of words per WRITE/WRITE_BUFFER 
					command. */
                wordsWritten,      /* total written so far */
                wordsRemaining,    /* number of words left to go */
                wordsThisWrite;    /* number of words */
 
#ifdef DEBUG_PRINT
    DEBUG_PRINT("Debug: entering CFISCS interleaved word write routine.\n");
#endif

    if (flWriteProtected(vol.socket))
        return flWriteProtect;

    if ((length & 1) || (address & 1))  /* Only write whole words, 
                       only on word-boundary */
        return flBadParameter;

#ifdef SOCKET_12_VOLTS
    if (thisCFI->vpp)
        checkStatus(flNeedVpp(vol.socket));
#endif

    if (thisCFI->maxBytesWrite > 1) /* multi-byte write supported? */
        maxWordsPerWrite = thisCFI->maxBytesWrite;
    else
        maxWordsPerWrite = 1;  /* Part can only handle single-byte writes. */

    flashPtr = (FlashWPTR)flMap(vol.socket, address);

    /* Now program the part: */
    if (maxWordsPerWrite > 1)  /* Part can handle writes of multiple 
                  words to buffer */
        {
        wordsWritten = 0;
        wordsRemaining = length / 2;  /* Number of words left to program into 
                     a part, convert bytes to words. */
        while (wordsRemaining > 0)
            {
            /* Figure out how many words we need to write.  This will either 
             * be the max allowed (maxWordsPerWrite), or what's left over at 
             * the end (wordsRemaining).  Make it extra-cryptic with the
             * ?: operator.
             */
            wordsThisWrite = (wordsRemaining > maxWordsPerWrite) 
                             ? maxWordsPerWrite : wordsRemaining;

            /* if we don't have enough aligned data to use a buffered write 
             * we just use a normal write 
             */
            if ( (wordsThisWrite <2 ) && (wordsThisWrite > 0))
                {
                /* data is a 1's mask, so we only try overwriting the 
                 * bytes we desire ie. written a 1 should not change 
                 * the state of an erase bit
                 */
                int byteCount;
                ULONG data = 0xffffffff; 
                /* clear out the bytes we are going to overwrite */
                for (byteCount = 0; byteCount<length; byteCount++)
                    data &= ~(0xff << (byteCount*8));
                /* set the bits we don't want to clear in our data */
                data |= CFI_LONGSWAP(* (ULONG *) buffer);
                CFI_LONG_WRITE(flashPtr, (ULONG) (PROGRAM<<16) |PROGRAM);
                CFI_LONG_WRITE(flashPtr, (ULONG) data);
                /* Flow diagram says to check that status register says 
                 * everything is copacetic. 
                 */
                while ( (!(CFI_WORD_READ(flashPtr) & SR_READY)) ||
                        (!(CFI_WORD_READ((flashPtr + 1)) & SR_READY)) );  
                CFI_LONG_WRITE(flashPtr, (ULONG) (READ_ARRAY<<16) |READ_ARRAY); 

#ifdef DEBUG_PRINT
                DEBUG_PRINT("Debug: mtd: Using NON-buffered write to program %ld"
                            " words at addr 0x%lx\n", 
                            wordsThisWrite, (ULONG) flashPtr);
#endif
                }
            else
                {
                CFI_LONG_WRITE(flashPtr, (ULONG) ((CLEAR_STATUS<<16)
                                                  |CLEAR_STATUS));
                CFI_LONG_WRITE(flashPtr, (ULONG) (WRITE_TO_BUFFER<<16) 
                               |WRITE_TO_BUFFER);  /* both parts */

                /* At this point, we must wait for XSR to indicate a buffer is 
                 * available. We will spin on this, but only for a maximum of 5 
                 * seconds.  We do the timing by taking advantage of the 
                 * ever-incrementing flMsecCounter.
                 */
                while ((!(CFI_WORD_READ((FlashWPTR)(flashPtr)) & SR_READY)) ||
                       (!(CFI_WORD_READ((FlashWPTR)(flashPtr + 1)) & SR_READY)))
                    ;

                /* Check to see if either part timed out: */
                if ( (!(CFI_WORD_READ(&flashPtr[0x0])     & SR_READY)) ||
                     (!(CFI_WORD_READ((&flashPtr[0x1])) & SR_READY)) )
                    {
                    /* Send read-array command to both parts, abort. */
                    CFI_LONG_WRITE(flashPtr, (ULONG) (READ_ARRAY<<16) |READ_ARRAY);
                    return(flWriteFault);
                    }

                /* Now we must write the count value (zero-based, so 0 => 1 word,
                 * 1 => 2 words, etc.) for the number of words we intend to 
                 * write.  Note that this code always assumes an even number of 
                 * words > 0.  Half the words go in the even part, half go in the 
                 * odd part.
                 */
                CFI_LONG_WRITE(flashPtr, (ULONG)(((wordsThisWrite / 2)  - 1)
                                         <<16) | ((wordsThisWrite / 2) - 1));  /* both parts */

                /* Finally, write the data.  This is easy, because now the 
                 * flash parts are seen as memory addresses.
                 */
                /* FIXME: This is suboptimal.  It is only writing half the 
                 * maximum words that could be written to each part in buffered 
                 * write mode. If we double the assignments for wordsWritten & 
                 * wordsThisWrite, and halve the assignment for wordsRemaining 
                 * each iteration, we can probably make this work optimally.
                 * Then we should probably rename some of the variables to 
                 * "longsxxx".
                 */
                /* last argument is expected in bytes */
                tffscpyLongs(&((long *)flashPtr)[wordsWritten/2], 
                             &((long *)buffer)[wordsWritten/2], wordsThisWrite*2);

                /* Now send CONFIRM_WRITE command. */
                CFI_LONG_WRITE(flashPtr, (ULONG) (CONFIRM_WRITE<<16) 
                               | CONFIRM_WRITE);  /* both parts */
                /* Flow diagram says to check that status register says 
                 * everything is copacetic. 
                 */
                while ( (!(CFI_WORD_READ(flashPtr)     & SR_READY)) ||
                        (!(CFI_WORD_READ((flashPtr + 1)) & SR_READY)) ); 

                /* Check to see if either part timed out: */
                if ( (!(CFI_WORD_READ(flashPtr)     & SR_READY)) ||
                     (!(CFI_WORD_READ((flashPtr + 1)) & SR_READY)) )
                    {
                    /* Send read-array command to both parts, abort. */
                    CFI_LONG_WRITE(flashPtr, (ULONG)(READ_ARRAY<<16) 
                                   | READ_ARRAY);
                    return(flWriteFault);
                    }
                else
                    {
                    CFI_LONG_WRITE(flashPtr, (ULONG)(READ_ARRAY<<16) 
                                   | READ_ARRAY);
                    }
                /* Now verify the write. */
                if (tffscmpWords(&((long *)flashPtr)[wordsWritten/2],
                                 &((long *)buffer)[wordsWritten/2], wordsThisWrite*2))
                    {
#ifdef DEBUG_PRINT
                    DEBUG_PRINT("Debug: mtd: Buffer write verification failed "
                                "(interleaved word mode)\n");
#endif
                    return flWriteFault;
                    }
                else
                    {
#ifdef DEBUG_PRINT
                    DEBUG_PRINT("Debug: mtd: Buffer write verification passed "
                                "(interleaved word mode) at addr 0x%lx\n", 
                                (ULONG) flashPtr);
#endif
                    }
                }
            wordsRemaining  -= wordsThisWrite;  /* That many fewer words to go. */
            wordsWritten    += wordsThisWrite;  /* That many more done. */

            }
        }
    else   /* Part cannot handle writes to buffer, must write one byte at a time. */
        {
        }

    /* Put both devices back into read mode: */
    CFI_LONG_WRITE(flashPtr, (ULONG) (READ_ARRAY<<16) | READ_ARRAY); 
    return status;
    }

/***************************************************************************
* cfiscsDWordWrite - Write routine for devices on 32 bit bus.
*
* This routine is instrumented specifically for the large detachable flash
* module on the wrSbc85xx board.  It contains implementation and
* configuration specific code and is not meant for generic use.
*
* RETURNS:
*
* ERRNO:
*
* \NOMANUAL
*/

LOCAL FLStatus cfiscsDWordWrite
    (
    FLFlash vol,                /* Pointer identifying drive     */
    CardAddress address,        /* Card address to write to      */
    const void FAR1 *buffer,    /* Address of data to write      */
    int length,                 /* Number of bytes to write      */
    int mode                    /* write mode (overwrite yes/no) */
    )
    {
    FLStatus status = flOK;
    FlashPTR flashPtr;
    unsigned int from, eachWrite;
    const char FAR1 *temp = (const char FAR1 *)buffer;
    /* Set timeout to 5 seconds from now */
    unsigned long writeTimeout = flMsecCounter + 5000;
    int uaoffset = 0, padding = 0;
    void FAR1 *newBuff = 0, *orgBuff = 0;
    int newLength = 0, orgLength = 0;

    printf ("CFISCS: writing %d bytes to flash offset 0x%x\n", length, address);

    /* check for unaligned address and length */
    flashPtr = (FlashPTR) flMap(vol.socket, address);
    if ((uaoffset = ((UINT32)flashPtr & 0x3)) || (length & 0x3))
        {
        padding = 0x3 & (4 - ((uaoffset + length) & 0x3));
        newLength = uaoffset + length + padding;
        newBuff = (void FAR1 *) malloc(newLength+8);
        memset((void*)newBuff, 'A', newLength+8);
#ifdef DEBUG_PRINT
        DEBUG_PRINT("Debug: flashPtr mapped to %x\n", (UINT32) flashPtr);
        DEBUG_PRINT("Debug: uaoffset = %d, padding = %d\n", uaoffset, padding);
        DEBUG_PRINT("Debug: newLength = %d, newBuff = 0x%x\n",
                    newLength, newBuff);
#endif
        flashPtr = (FlashPTR) ((int)flashPtr & ~0x3);
        *(FlashDPTR)flashPtr = (UINT32) ((READ_ARRAY << 24) |
                                         (READ_ARRAY << 16) |
                                         (READ_ARRAY << 8)  |
                                         READ_ARRAY);
        memcpy ((void*)newBuff, (void*)flashPtr, newLength);
        memcpy ((void*)((int)newBuff+uaoffset), (void*)buffer, length);

        orgBuff = (void FAR1 *)buffer;
        orgLength = length;
        temp = newBuff;
        buffer = newBuff;
        length = newLength;
        }

    if (flWriteProtected(vol.socket))
        return flWriteProtect;

#ifdef SOCKET_12_VOLTS
    if (thisCFI->vpp)
        checkStatus(flNeedVpp(vol.socket));
#endif

    if (thisCFI->maxBytesWrite > 1) /* multi-byte write supported */
        eachWrite = thisCFI->maxBytesWrite * vol.interleaving;
    else
        eachWrite = 4;  /* Max bus width */

    for (from = 0; (int)from < length && status == flOK; from += eachWrite)
        {
        int thisLength = length - from;

        if (thisLength > (int)eachWrite)
            thisLength = eachWrite;

        flashPtr = (FlashPTR) flMap(vol.socket, address + from);

        do 
            {
            if ((thisCFI->maxBytesWrite > 1) && 
                ((thisCFI->maxBytesWrite & 0x3) == 0))
                *(FlashDPTR)flashPtr = (UINT32) ((WRITE_TO_BUFFER << 24) |
                                                 (WRITE_TO_BUFFER << 16) |
                                                 (WRITE_TO_BUFFER << 8)  |
                                                 WRITE_TO_BUFFER);
            else
                *(FlashDPTR)flashPtr = (UINT32) ((PROGRAM << 24) |
                                                 (PROGRAM << 16) |
                                                 (PROGRAM << 8)  |
                                                 PROGRAM);

            } while (!(*(FlashDPTR)flashPtr &
                       (SR_READY<<24|SR_READY<<16|SR_READY<<8|SR_READY)) &&
                     (flMsecCounter < writeTimeout));
        if (!(*(FlashDPTR)flashPtr &
              (SR_READY<<24|SR_READY<<16|SR_READY<<8|SR_READY)))
            {
#ifdef DEBUG_PRINT
            DEBUG_PRINT("Debug: timeout error in CFISCS write.\n");
#endif
            status = flWriteFault;
            }

        /* If we are using the WRITE_TO_BUFFER command then we must program
         * the buffer length
         */
        if ((thisCFI->maxBytesWrite > 1) && 
            ((thisCFI->maxBytesWrite & 0x3) == 0))
            *(FlashDPTR)flashPtr = (thisLength << 24 |
                                    thisLength << 16 |
                                    thisLength << 8 |
                                    thisLength);

        tffscpyLongs(flashPtr,temp + from,thisLength);

#if FALSE
        *(FlashDPTR)flashPtr = (UINT32) ((CONFIRM_WRITE << 24) |
                                         (CONFIRM_WRITE << 16) |
                                         (CONFIRM_WRITE << 8)  |
                                         CONFIRM_WRITE);
        *(FlashDPTR)flashPtr = (UINT32) ((CONFIRM_WRITE << 24) |
                                         (CONFIRM_WRITE << 16) |
                                         (CONFIRM_WRITE << 8)  |
                                         CONFIRM_WRITE);
#endif

        while (!((*flashPtr & SR_READY) &&
                 (*(flashPtr+1) & SR_READY) &&
                 (*(flashPtr+2) & SR_READY) &&
                 (*(flashPtr+3) & SR_READY)) &&
               (flMsecCounter < writeTimeout))
            ;

        if (!(flMsecCounter < writeTimeout))
            {
#ifdef DEBUG_PRINT
            DEBUG_PRINT("Debug: timeout error in CFISCS write.\n");
#endif
            status = flWriteFault;
            }
        if (*flashPtr & WSM_ERROR)
            {
#ifdef DEBUG_PRINT
            DEBUG_PRINT("Debug: error in CFISCS write.\n");
#endif
            status = (*flashPtr & SR_VPP_ERROR) ? flVppFailure : flWriteFault;
            *(FlashDPTR)flashPtr = (UINT32) ((CLEAR_STATUS << 24) |
                                             (CLEAR_STATUS << 16) |
                                             (CLEAR_STATUS << 8)  |
                                             CLEAR_STATUS);
            }

        *(FlashDPTR)flashPtr = (UINT32) ((READ_ARRAY << 24) |
                                         (READ_ARRAY << 16) |
                                         (READ_ARRAY << 8)  |
                                         READ_ARRAY);
        }

#ifdef SOCKET_12_VOLTS
    if (thisCFI->vpp)
        flDontNeedVpp(vol.socket);
#endif

    flashPtr = (FlashPTR) flMap(vol.socket, address);
    /* verify the data */

    if (status == flOK &&
        tffscmpWords((void FAR0 *) flashPtr, (void FAR1 *) buffer,length))
        {
#ifdef DEBUG_PRINT
        DEBUG_PRINT("Debug: CFISCS dword write failed in verification.\n");
#endif
        status = flWriteFault;
        }

    if ((uaoffset != 0) || (padding != 0))
        {
        free (newBuff);
        buffer = orgBuff;
        length = orgLength;
        }

    return status;
    }

#ifdef INTERLEAVED_MODE_REQUIRES_32BIT_WRITES
/***************************************************************************
* cfiscsInterleavedWordErase - Erase specified number of blocks
*
* Given the first block and number of contiguous blocks to erase, this 
* routine will erase the content of the specified region. The routine does
* not support features like "Suspend Erase" or "Queued Erase" so the device
* stays busy until all specified blocks have been erased. These features need
* to be supported by the TrueFFS core to be applicable here.
*
* RETURNS:
*
* ERRNO:
*
* \NOMANUAL
*/
LOCAL FLStatus cfiscsInterleavedWordErase
    (
    FLFlash vol,                  /* Pointer identifying drive            */
    int firstEraseBlock,          /* First block to erase                 */
    int numberOfEraseBlocks       /* Number of contiguous blocks to erase */
    )
    {
    FLStatus status = flOK;       /* unless proven otherwise */
    int iBlock;

#ifdef DEBUG_PRINT
    DEBUG_PRINT("Debug: entering CFISCS Interleaved Word Erase routine.\n");
#endif

    if (flWriteProtected(vol.socket))
        return flWriteProtect;

#ifdef SOCKET_12_VOLTS
    if (thisCFI->vpp)
        checkStatus(flNeedVpp(vol.socket));
#endif

    for (iBlock = firstEraseBlock; 
        iBlock < firstEraseBlock + numberOfEraseBlocks && status == flOK;
        iBlock++)
        {
        FLBoolean finished;

        FlashWPTR flashPtr = (FlashWPTR) flMap(vol.socket,
                                               iBlock * vol.erasableBlockSize);

        CFI_LONG_WRITE(flashPtr, (ULONG) (SETUP_BLOCK_ERASE <<16)|SETUP_BLOCK_ERASE);
        CFI_LONG_WRITE(flashPtr, (ULONG) (CONFIRM_ERASE <<16)|CONFIRM_ERASE);

        /*   while (flashPtr[ix * thisCFI->interleaveFactor] != 0x80); */

        do 
            {
            finished = FALSE;

            if ((flashPtr[0] == SR_READY) &&
                (flashPtr[1] == SR_READY))
                {
                finished = TRUE;
                if (CFI_WORD_READ(flashPtr) & WSM_ERROR)
                    {
#ifdef DEBUG_PRINT
                    DEBUG_PRINT("Debug: CFISCS erase error on device one\n");
#endif
                    status = (CFI_WORD_READ(flashPtr) & SR_VPP_ERROR) 
                             ?  flVppFailure : flWriteFault;
                    CFI_LONG_WRITE(flashPtr, (ULONG) CLEAR_STATUS);
                    }
                else if (CFI_WORD_READ(&flashPtr[1]) & WSM_ERROR)
                    {
#ifdef DEBUG_PRINT
                    DEBUG_PRINT("Debug: CFISCS erase error on device two\n");
#endif
                    status = ((CFI_WORD_READ(&flashPtr[1])) & (SR_VPP_ERROR)) 
                             ?  flVppFailure : flWriteFault;
                    CFI_LONG_WRITE(flashPtr, (ULONG) (CLEAR_STATUS <<16) & 0xffff0000);
                    }
                CFI_LONG_WRITE(flashPtr, (ULONG) (READ_ARRAY <<16)|READ_ARRAY);
                }
            } while (!finished);
        }
#ifdef SOCKET_12_VOLTS
    if (thisCFI->vpp)
        flDontNeedVpp(vol.socket);
#endif

    return status;
    }
#else /* !INTERLEAVED_MODE_REQUIRES_32BIT_WRITES */

/***************************************************************************
* cfiscsErase - Erase specified number of blocks
*
* Given the first block and number of contiguous blocks to erase, this 
* routine will erase the content of the specified region. The routine does
* not support features like "Suspend Erase" or "Queued Erase" so the device
* stays busy until all specified blocks have been erased. These features need
* to be supported by the TrueFFS core to be applicable here.
*
* Each interleaved device is erased independently. The core does not
* compensate for interleaving and expects interleaving to be handled here.
*
* RETURNS:
*
* ERRNO:
*
* \NOMANUAL
*/
LOCAL FLStatus cfiscsErase
    (
    FLFlash vol,                  /* Pointer identifying drive            */
    int firstEraseBlock,          /* First block to erase                 */
    int numberOfEraseBlocks       /* Number of contiguous blocks to erase */
    )
    {
    FLStatus status = flOK;       /* unless proven otherwise */
    int iBlock;

    printf ("CFISCS: erasing logical block # %d, count = %d\n",
            firstEraseBlock, numberOfEraseBlocks);

    if (flWriteProtected(vol.socket))
        {
#ifdef DEBUG_PRINT
        DEBUG_PRINT("Debug: Write protect error word erasing device.\n");
#endif
        return flWriteProtect;
        }

#ifdef SOCKET_12_VOLTS
    if (thisCFI->vpp)
        checkStatus(flNeedVpp(vol.socket));
#endif

    for (iBlock = firstEraseBlock; 
        iBlock < firstEraseBlock + numberOfEraseBlocks && status == flOK;
        iBlock++)
        {
        int ix;
        FLBoolean finished;

        FlashPTR flashPtr = (FlashPTR) flMap(vol.socket,
                                             iBlock * vol.erasableBlockSize);
        for (ix=0; ix < vol.interleaving; ix++)
            {
            flashPtr[ix * thisCFI->interleaveFactor] = SETUP_BLOCK_ERASE;
            flashPtr[ix * thisCFI->interleaveFactor] = CONFIRM_ERASE;
            flashPtr[ix * thisCFI->interleaveFactor] = CONFIRM_ERASE;

            while (flashPtr[ix * thisCFI->interleaveFactor] != 0x80);

            do 
                {
                finished = TRUE;

                if (!(flashPtr[ix * thisCFI->interleaveFactor] & SR_READY))
                    finished = FALSE;
                else
                    {
                    if (flashPtr[ix * thisCFI->interleaveFactor] & WSM_ERROR)
                        {
#ifdef DEBUG_PRINT
                        DEBUG_PRINT("Debug: CFISCS erase error on device %d.\n", ix);
#endif
                        status = (flashPtr[ix * thisCFI->interleaveFactor] 
                                  & SR_VPP_ERROR) ?  flVppFailure : flWriteFault;
                        flashPtr[ix * thisCFI->interleaveFactor] = CLEAR_STATUS;
                        }
                    flashPtr[ix * thisCFI->interleaveFactor] = READ_ARRAY;
                    }
                } while (!finished);
            }
        if (numberOfEraseBlocks > 1)
            printf (".");
        }
#ifdef SOCKET_12_VOLTS
    if (thisCFI->vpp)
        flDontNeedVpp(vol.socket);
#endif

    if (numberOfEraseBlocks > 1)
        printf ("\n");
    return status;
    }

/***************************************************************************
* cfiscsDWordErase - Erase routine for devices on 32 bit bus.
*
* This routine is instrumented specifically for the large detachable flash
* module on the wrSbc85xx board.  It contains implementation and
* configuration specific code and is not meant for generic use.
*
* RETURNS:
*
* ERRNO:
*
* \NOMANUAL
*/

LOCAL FLStatus cfiscsDWordErase
    (
    FLFlash vol,                  /* Pointer identifying drive            */
    int firstEraseBlock,          /* First block to erase                 */
    int numberOfEraseBlocks       /* Number of contiguous blocks to erase */
    )
    {
    FLStatus status = flOK;       /* unless proven otherwise */
    int iBlock;

    printf ("CFISCS: erasing logical block # %d, count = %d\n",
            firstEraseBlock, numberOfEraseBlocks);

    if (flWriteProtected(vol.socket))
        return flWriteProtect;

#ifdef SOCKET_12_VOLTS
    if (thisCFI->vpp)
        checkStatus(flNeedVpp(vol.socket));
#endif

    for (iBlock = firstEraseBlock;
        iBlock < firstEraseBlock + numberOfEraseBlocks && status == flOK;
        iBlock++)
        {
        int ix;
        FLBoolean finished;

        FlashDPTR flashDPtr = (FlashDPTR) flMap(vol.socket,
                                                iBlock * vol.erasableBlockSize);
        for (ix=0; ix < vol.interleaving; ix++)
            {
            flashDPtr[ix * thisCFI->interleaveFactor] = (SETUP_BLOCK_ERASE <<24) | 
                                                        (SETUP_BLOCK_ERASE <<16) | 
                                                        (SETUP_BLOCK_ERASE <<8)  | 
                                                         SETUP_BLOCK_ERASE;
            flashDPtr[ix * thisCFI->interleaveFactor] = (unsigned long) ((CONFIRM_ERASE<<24) |
                                                                         (CONFIRM_ERASE<<16) | 
                                                                         (CONFIRM_ERASE<<8)  |
                                                                          CONFIRM_ERASE);
            flashDPtr[ix * thisCFI->interleaveFactor] = (unsigned long) ((CONFIRM_ERASE<<24) |
                                                                         (CONFIRM_ERASE<<16) | 
                                                                         (CONFIRM_ERASE<<8)  | 
                                                                         CONFIRM_ERASE);

            while (flashDPtr[ix * thisCFI->interleaveFactor] != 0x80808080);

            do
                {
                finished = TRUE;

                if (!(flashDPtr[ix * thisCFI->interleaveFactor] & SR_READY))
                    finished = FALSE;
                else
                    {
                    if (flashDPtr[ix * thisCFI->interleaveFactor] & WSM_ERROR)
                        {
#ifdef DEBUG_PRINT
                        DEBUG_PRINT("Debug: CFISCS erase error on device %d.\n", ix);
#endif
                        status = (flashDPtr[ix * thisCFI->interleaveFactor]
                                  & SR_VPP_ERROR) ?  flVppFailure : flWriteFault;
                        flashDPtr[ix * thisCFI->interleaveFactor] = (CLEAR_STATUS<<24)|
                                                                    (CLEAR_STATUS<<16)|
                                                                    (CLEAR_STATUS<<8) |
                                                                     CLEAR_STATUS;
                        }
                    flashDPtr[ix * thisCFI->interleaveFactor] = (unsigned long) ((READ_ARRAY<<24)|
                                                                                 (READ_ARRAY<<16)|
                                                                                 (READ_ARRAY<<8) |
                                                                                  READ_ARRAY);
                    }
                } while (!finished);
            }
        if (numberOfEraseBlocks > 1)
            printf (".");
        }
#ifdef SOCKET_12_VOLTS
    if (thisCFI->vpp)
        flDontNeedVpp(vol.socket);
#endif

    if (numberOfEraseBlocks > 1)
        printf ("\n");
    return status;
    }

#endif /* INTERLEAVED_MODE_REQUIRES_32BIT_WRITES */

/***************************************************************************
* cfiscsIdentify - Identification routine for devices conforming to CFI/SC
*
* Identifies media based on SCS/CFI and registers as an MTD for such.
* This routine will be placed on the MTD list in custom.h. It must be an 
* extern routine. On successful identification, the Flash structure is filled 
* out and the write and erase routines registered. 
*
* RETURNS:
*      FLStatus        : 0 on positive identification, failed otherwise 
*
* ERRNO:
*
* URL:http://developer.intel.com/design/flcomp/applnots/29220404.pdf 
*
* \NOMANUAL
*/
FLStatus cfiscsIdentify
    (
    FLFlash vol       /* Pointer identifying drive */
    )
    {
    volatile FlashPTR flashPtr = (FlashPTR) flMap(vol.socket, 0);
    volatile FlashWPTR flashWPtr = (FlashWPTR) flashPtr;
    volatile FlashDPTR flashDPtr = (FlashDPTR) flashPtr;

    unsigned primaryTable, secondaryTable;
    char priIdStr[ID_STR_LENGTH + 1] = PRIMARY_ID_STR;

#ifdef DEBUG_PRINT
    DEBUG_PRINT("Debug: entering CFISCS identification routine.\n");
#endif

    flSetWindowBusWidth(vol.socket, 16);/* use 16-bits */
    flSetWindowSpeed(vol.socket, 120);  /* 120 nsec. */
    flSetWindowSize(vol.socket, 2);       /* 8 KBytes */

    vol.mtdVars = &mtdVars[flSocketNoOf(vol.socket)];

    /* Is this an 8 bit device */
    flashPtr[0x55] = READ_ARRAY;
    flashPtr[0x55] = QUERY;

    if (flashPtr[0x10] == 0x51 && /* 'Q' */
        flashPtr[0x11] == 0x52 && /* 'R' */
        flashPtr[0x12] == 0x59)   /* 'Y' */
        {
#ifdef DEBUG_PRINT
        DEBUG_PRINT ("Debug: detected single 8 bit device\n");
#endif
        thisCFI->multiplier = 1;
        thisCFI->interleaveFactor = 1;
        vol.interleaving = 1;
        vol.type = (FlashType) ((flashPtr[0] << 8) | flashWPtr[1]);
        vol.write = cfiscsByteWrite;
        vol.erase = cfiscsErase;

        goto getCFI;
        }

    /* Reset to READ_ARRAY and retry. Maybe 16 bit addressing 
     */
    flashPtr[0x55] = READ_ARRAY;
    flashWPtr[0x55] = (USHORT) ((QUERY << 8) | QUERY); 

#if 0

    /* Check for two interleaved 8 bit parts */
    if (flashPtr[0x20] == 0x51 && /* 'Q' */
        flashPtr[0x21] == 0x51 && /* 'Q' */
        flashPtr[0x22] == 0x52 && /* 'R' */
        flashPtr[0x23] == 0x52 && /* 'R' */
        flashPtr[0x24] == 0x59 && /* 'Y' */
        flashPtr[0x25] == 0x59)   /* 'Y' */
        {
        /* Let's try turning one off */
        CFI_WORD_WRITE((FlashWPTR)(&flashPtr[0x55]), READ_ARRAY);
        if (flashPtr[0x20] != 0x51) /* Turned off successfully */
            {
            thisCFI->multiplier = 2;
            thisCFI->interleaveFactor = 1;
            vol.interleaving =2;
            vol.type = (FlashType) ((flashWPtr[0] << 8) | flashWPtr[1]);
            vol.write = cfiscsByteWrite;
            vol.erase = cfiscsErase;
#ifdef DEBUG_PRINT
            DEBUG_PRINT ("Debug: detected two 8 bit devices\n");
#endif
            goto getCFI;
            }
        }
#endif  /* 0 */

    /* a 16 bit device in 16 bit mode */
    if (flashPtr[0x20] == 0x51 && /* 'Q' */
        flashPtr[0x21] == 0x00 && /* NUL */
        flashPtr[0x22] == 0x52 && /* 'R' */
        flashPtr[0x23] == 0x00 && /* NUL */
        flashPtr[0x24] == 0x59 && /* 'Y' */
        flashPtr[0x25] == 0x00)   /* NUL */
        {
        thisCFI->multiplier = 2;
        thisCFI->interleaveFactor = 1;
        vol.interleaving = 1;
        vol.type = (FlashType) ((flashWPtr[0] << 8) | (flashWPtr[1]));
        vol.write = cfiscsWordWrite;
        vol.erase = cfiscsErase;
#ifdef DEBUG_PRINT
        DEBUG_PRINT ("Debug: detected one 16 bit device\n");
#endif
        goto getCFI;
        }

    /* If we have a 16 bit device in 8 bit mode it should ID correctly */
    if (flashPtr[0x20] == 0x51 && /* 'Q' */
        flashPtr[0x21] == 0x51 && /* 'Q' */
        flashPtr[0x22] == 0x52 && /* 'R' */
        flashPtr[0x23] == 0x52 && /* 'R' */
        flashPtr[0x24] == 0x59 && /* 'Y' */
        flashPtr[0x25] == 0x59)   /* 'Y' */
        {
        thisCFI->multiplier = 2;
#ifdef DEBUG_PRINT
        DEBUG_PRINT ("Debug: detected a 16 bit device in 8 bit mode\n");
#endif
        vol.interleaving =1;
        thisCFI->interleaveFactor = 1;
        vol.type = (FlashType) ((flashWPtr[0] << 8) | flashWPtr[1]);
        vol.write = cfiscsByteWrite;
        vol.erase = cfiscsErase;
        goto getCFI;
        }

    /* Reset to READ_ARRAY and retry. Maybe 32 bit addressing 
     */
    flashWPtr[0x55] = (USHORT) ((READ_ARRAY << 8) | READ_ARRAY); 
    flashDPtr[0x55] = (ULONG) (QUERY << 24| QUERY <<16 | QUERY << 8 | QUERY); 

    /* A 32 bit device in 8 bit mode and two 16 bit device in 8 bit mode will
     * appear the same to the query process. The only way to make the
     * distinction is to try and set one device back to read array mode while
     * the other remains in query mode 
     */
    if (flashPtr[0x40] == 0x51 && /* 'Q' */
        flashPtr[0x41] == 0x51 && /* 'Q' */
        flashPtr[0x42] == 0x51 && /* 'Q' */
        flashPtr[0x43] == 0x51 && /* 'Q' */
        flashPtr[0x44] == 0x52 && /* 'R' */
        flashPtr[0x45] == 0x52 && /* 'R' */
        flashPtr[0x46] == 0x52 && /* 'R' */
        flashPtr[0x47] == 0x52 && /* 'R' */
        flashPtr[0x48] == 0x59 && /* 'Y' */
        flashPtr[0x49] == 0x59 && /* 'Y' */
        flashPtr[0x4a] == 0x59 && /* 'Y' */
        flashPtr[0x4b] == 0x59)   /* 'Y' */
        {
        /* See if we can turn one off */
        flashPtr[0x154] = READ_ARRAY;
        if ((flashPtr[0x40] != 0x51) && (flashPtr[0x41] == 0x51))
            {   /* Turned one device off successfully */
            flashPtr[0x154] = QUERY;    /* turn it back on to query mode */
            vol.interleaving = 2;
            vol.type = (FlashType) ((flashWPtr[0] << 8) | (flashWPtr[4]));
#ifdef DEBUG_PRINT
            DEBUG_PRINT ("Debug: detected two 16 bit device, 8 bit interleaved(2)\n");
#endif
            }
        else
            {
            vol.interleaving =1;
#ifdef DEBUG_PRINT
            DEBUG_PRINT ("Debug: detected a 32 bit device in 8 bit mode\n");
#endif
            vol.type = (FlashType) ((flashDPtr[0] << 8) | flashDPtr[1]);
            }
        thisCFI->multiplier = 4;
        thisCFI->interleaveFactor = 1;
        vol.write = cfiscsByteWrite;
        vol.erase = cfiscsErase;

        goto getCFI;
        }

    /* Is it a single 32 bit device */
    if (flashPtr[0x40] == 0x51 && /* 'Q' */
        flashPtr[0x41] == 0x00 && /* NUL */
        flashPtr[0x42] == 0x00 && /* NUL */
        flashPtr[0x43] == 0x00 && /* NUL */
        flashPtr[0x44] == 0x52 && /* 'R' */
        flashPtr[0x45] == 0x00 && /* NUL */
        flashPtr[0x46] == 0x00 && /* NUL */
        flashPtr[0x47] == 0x00 && /* NUL */
        flashPtr[0x48] == 0x59 && /* 'Y' */
        flashPtr[0x49] == 0x00 && /* NUL */
        flashPtr[0x4a] == 0x00 && /* NUL */
        flashPtr[0x4b] == 0x00)   /* NUL */
        {
#ifdef DEBUG_PRINT
        DEBUG_PRINT ("Debug: detected a 32 bit device in 32 bit mode\n");
#endif
        thisCFI->multiplier = 4;
        thisCFI->interleaveFactor = 1;
        vol.interleaving = 1;
        vol.type = (FlashType) ((flashDPtr[0] << 8) | flashDPtr[1]);
        vol.write = cfiscsDWordWrite;
        vol.erase = cfiscsErase;
        goto getCFI;
        }
    /* Two 16 bit devices in 16 bit mode */
    if (flashPtr[0x40] == 0x51 && /* 'Q' */
        flashPtr[0x41] == 0x00 && /* NUL */
        flashPtr[0x42] == 0x51 && /* 'Q' */
        flashPtr[0x43] == 0x00 && /* NUL */
        flashPtr[0x44] == 0x52 && /* 'R' */
        flashPtr[0x45] == 0x00 && /* NUL */
        flashPtr[0x46] == 0x52 && /* 'R' */
        flashPtr[0x47] == 0x00 && /* NUL */
        flashPtr[0x48] == 0x59 && /* 'Y' */
        flashPtr[0x49] == 0x00 && /* NUL */
        flashPtr[0x4a] == 0x59 && /* 'Y' */
        flashPtr[0x4b] == 0x00)   /* NUL */
        {
#ifdef DEBUG_PRINT
        DEBUG_PRINT ("Debug: Detected two 16 bit devices in 16 bit mode(3)\n");
#endif
        thisCFI->multiplier = 4;
        thisCFI->interleaveFactor = 2;
        vol.interleaving = 2;
        vol.type = (FlashType) (flashPtr[0] << 8 | 
                                flashPtr[thisCFI->multiplier]);
        vol.write = cfiscsInterleavedWordWrite;
        vol.erase = cfiscsErase;
        goto getCFI;
        }

    /* Reset to READ_ARRAY and retry. Maybe 64 bit addressing */

    flashDPtr[0x55] = (ULONG) ((READ_ARRAY << 24) | (READ_ARRAY << 16) |
                               (READ_ARRAY << 8) | READ_ARRAY);
    flashDPtr[0xaa] = (ULONG) (QUERY << 24| QUERY <<16 | QUERY << 8 | QUERY);

    /* Four 16 bit device in 8 bit mode or two 16 bit device in 16 bit mode
     * among others, will appear the same. Let's not tell them apart
     * for the moment, just because we know what is there. 
     */

    taskDelay (10);
    /* Is it four interleaved 8 bit devices */
    if (flashPtr[0x80] == 0x51 && /* 'Q' */
        flashPtr[0x81] == 0x51 && /* 'Q' */
        flashPtr[0x82] == 0x51 && /* 'Q' */
        flashPtr[0x83] == 0x51 && /* 'Q' */
        flashPtr[0x84] == 0x51 && /* 'Q' */
        flashPtr[0x85] == 0x51 && /* 'Q' */
        flashPtr[0x86] == 0x51 && /* 'Q' */
        flashPtr[0x87] == 0x51 && /* 'Q' */
        flashPtr[0x88] == 0x52 && /* 'R' */
        flashPtr[0x89] == 0x52 && /* 'R' */
        flashPtr[0x8a] == 0x52 && /* 'R' */
        flashPtr[0x8b] == 0x52 && /* 'R' */
        flashPtr[0x8c] == 0x52 && /* 'R' */
        flashPtr[0x8d] == 0x52 && /* 'R' */
        flashPtr[0x8e] == 0x52 && /* 'R' */
        flashPtr[0x8f] == 0x52 && /* 'R' */
        flashPtr[0x90] == 0x59 && /* 'Y' */
        flashPtr[0x91] == 0x59 && /* 'Y' */
        flashPtr[0x92] == 0x59 && /* 'Y' */
        flashPtr[0x93] == 0x59 && /* 'Y' */
        flashPtr[0x94] == 0x59 && /* 'Y' */
        flashPtr[0x95] == 0x59 && /* 'Y' */
        flashPtr[0x96] == 0x59 && /* 'Y' */
        flashPtr[0x97] == 0x59)   /* 'Y' */
        {

        /* workaround for wrSbc85xx */

#ifdef DEBUG_PRINT
        DEBUG_PRINT ("Debug: detected four 8 bit devices in 32 bit mode\n");
#endif
        thisCFI->multiplier = 8;
        thisCFI->interleaveFactor = 4;
        vol.interleaving = 4;
        vol.type = (FlashType) ((flashDPtr[0] << 8) | flashDPtr[1]);
        vol.write = cfiscsDWordWrite;
        vol.erase = cfiscsDWordErase;
        goto getCFI;
        }

    /* If we get here the device is probably not CFI */

#ifdef DEBUG_PRINT
    DEBUG_PRINT ("Debug: Failed to identify CFI... \n Exiting \n");
#endif

    flashDPtr[0xaa] = (ULONG) (READ_ARRAY << 24| READ_ARRAY <<16 
                               | READ_ARRAY << 8 | READ_ARRAY); 

    return flUnknownMedia;

    getCFI:

    /* check the command set ID */
    thisCFI->commandSetId = flashPtr[0x13 * thisCFI->multiplier] | 
                            ((unsigned)flashPtr[0x14 * thisCFI->multiplier] << 8);
    if (thisCFI->commandSetId != INTEL_COMMAND_SET &&
        thisCFI->commandSetId != AMDFUJ_COMMAND_SET)
        {
#ifdef DEBUG_PRINT
        DEBUG_PRINT("Debug: did not recognize command set.\n");
#endif
        return flUnknownMedia;
        }
    /* get address for primary algorithm extended table. */
    primaryTable = flashPtr[0x15 * thisCFI->multiplier] |
                   ((unsigned)flashPtr[0x16 * thisCFI->multiplier] << 8);

    /* check alternate command set ID. */
    thisCFI->altCommandSetId = flashPtr[0x17 * thisCFI->multiplier] | 
                               ((unsigned)flashPtr[0x18 * thisCFI->multiplier] << 8);
    if (thisCFI->altCommandSetId != INTEL_ALT_COMMAND_SET &&
        thisCFI->altCommandSetId != AMDFUJ_ALT_COMMAND_SET &&
        thisCFI->altCommandSetId != ALT_NOT_SUPPORTED)
        return flUnknownMedia;

    /* get address for secondary algorithm extended table. */
    secondaryTable = flashPtr[0x19 * thisCFI->multiplier] |
                     ((unsigned)flashPtr[0x1a * thisCFI->multiplier] << 8);

    thisCFI->vpp = flashPtr[0x1d * thisCFI->multiplier];

    vol.erasableBlockSize = (flashPtr[0x2f * thisCFI->multiplier] |
                             ((unsigned)flashPtr[0x30 * thisCFI->multiplier]) << 8) *
                            0x100L;

    /* we reserve one erase block for NVRAM usage */
#ifdef SAVE_NVRAM_REGION
    if (vol.interleaving == 1)  /* wrSbc85xx boot flash only */
        {
        /* save 1MB rounded to next blk */
        vol.chipSize = (1L << flashPtr[0x27 * thisCFI->multiplier]) - 
                       vol.erasableBlockSize * (1+(0xfffff/vol.erasableBlockSize));
        }
    else
        {
        vol.chipSize = (1L << flashPtr[0x27 * thisCFI->multiplier]);
        }
#else
    vol.chipSize = (1L << flashPtr[0x27 * thisCFI->multiplier]);
#endif
    vol.erasableBlockSize *= vol.interleaving;

#ifdef BUFFERED_WRITE_BROKEN
    thisCFI->maxBytesWrite = 1;
#else
    thisCFI->maxBytesWrite = 1L << (flashPtr[0x2a * thisCFI->multiplier] |
                                    ((unsigned)flashPtr[0x2b * thisCFI->multiplier] << 8));
#endif /* BUFFERED_WRITE_BROKEN */

    /* In this part we access the primary extended table implemented by Intel.
     * If the device uses a different extended table, it should be accessed
     * according to the vendor specifications. 
     */
    if ((primaryTable) && (thisCFI->commandSetId == INTEL_COMMAND_SET))
        {
        /* look for the primary table identification string "PRI" */
        if (priIdStr[0] != flashPtr[primaryTable * thisCFI->multiplier] ||
            priIdStr[1] != flashPtr[(primaryTable + 1) * thisCFI->multiplier] ||
            priIdStr[2] != flashPtr[(primaryTable + 2) * thisCFI->multiplier])
            {
#ifdef DEBUG_PRINT
            DEBUG_PRINT ("Debug: No primary ID @ offset 0x%x\n", primaryTable);
#endif
            return flUnknownMedia;
            }

        thisCFI->optionalCommands = flashPtr[(primaryTable + 5) * thisCFI->multiplier] |
                                    ((long)flashPtr[(primaryTable + 6) *
                                                    thisCFI->multiplier] << 8) |
                                    ((long)flashPtr[(primaryTable + 7) *
                                                    thisCFI->multiplier] << 16) |
                                    ((long)flashPtr[(primaryTable + 8) *
                                                    thisCFI->multiplier] << 24);

        thisCFI->afterSuspend = flashPtr[(primaryTable + 9) * thisCFI->multiplier];
        }
    else
        {
        thisCFI->optionalCommands = 0;
        thisCFI->afterSuspend = 0;
        }
    /* Make sure this comes at the end since the device is set to READ_ARRAY
     * mode prior to the return
     */
    if (cfiscsGetChipCount(&vol) != flOK)
        return flUnknownMedia;
#ifdef DEBUG_PRINT
    DEBUG_PRINT ("Debug: No of chips detected is %d\n", vol.noOfChips);
#endif

#ifdef INTERLEAVED_MODE_REQUIRES_32BIT_WRITES
    vol.erase = cfiscsInterleavedWordErase;
/*
#else  / * !INTERLEAVED_MODE_REQUIRES_32BIT_WRITES * /
    vol.erase = cfiscsErase;
*/
#endif /* INTERLEAVED_MODE_REQUIRES_32BIT_WRITES */

#ifdef DEBUG_PRINT
    DEBUG_PRINT ("Debug: flash cmd set = %s\n",
                 ((thisCFI->commandSetId) == INTEL_COMMAND_SET)?
                 "Intel" : "AMD");
    DEBUG_PRINT ("Debug: primaryTable = %d\n", primaryTable);
    DEBUG_PRINT ("Debug: alt cmd set = %s\n",
                 ((thisCFI->altCommandSetId) == 0)?
                 "Not supported" :  "Supported");
    DEBUG_PRINT ("Debug: secondaryTable = %d\n", secondaryTable);
    DEBUG_PRINT ("Debug: vpp = %d\n", thisCFI->vpp);
    DEBUG_PRINT ("Debug: erase size = %d\n", vol.erasableBlockSize);
    DEBUG_PRINT ("Debug: opt cmd = %d\n", thisCFI->optionalCommands);
    DEBUG_PRINT ("Debug: after suspend = %d\n", thisCFI->afterSuspend);
    DEBUG_PRINT ("Debug: end\n");
#endif

    /* Might have to do this on a bus width basis but for now it seems to
     * work with Intel devices.
     */
    if (vol.interleaving == 4)
        {
        flashDPtr[0xaa] = (ULONG) (READ_ARRAY << 24| READ_ARRAY <<16 |
                                   READ_ARRAY << 8 | READ_ARRAY); 
        }
    else
        if (vol.interleaving == 2)
        {
        flashWPtr[0] = (USHORT) (READ_ARRAY<<8 | READ_ARRAY);
        }
    else
        {
        flashPtr[0] = READ_ARRAY;
        flashPtr[1] = READ_ARRAY;
        flashPtr[2] = READ_ARRAY;
        flashPtr[3] = READ_ARRAY;
        flashPtr[4] = READ_ARRAY;
        flashPtr[5] = READ_ARRAY;
        flashPtr[6] = READ_ARRAY;
        flashPtr[7] = READ_ARRAY;
        }

    return flOK;
    }

#else  /* INCLUDE_TFFS */

void tffsShow(void)
    {
    printf("INCLUDE_TFFS is not defined for this image build.\n");
    }

#endif  /* INCLUDE_TFFS */
