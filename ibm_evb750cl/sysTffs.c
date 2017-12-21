/* sysTffs.c - Malta system-dependent TrueFFS library */

/* Copyright 2001, 2007-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
 * This file has been developed or significantly modified by the
 * MIPS Center of Excellence Dedicated Engineering Staff.
 * This notice is as per the MIPS Center of Excellence Master Partner
 * Agreement, do not remove this notice without checking first with
 * WR/Platforms MIPS Center of Excellence engineering management.
 */

/* FAT-FTL Lite Software Development Kit
 * Copyright (C) M-Systems Ltd. 1995-1997   */

/*
modification history
--------------------
01f,08aug08,y_w  Add TFFS related definitions (WIND00128734)
01e,12nov07,y_w  Modify sysTffsFormat() to support both 750cl and 750gx BSPs
01d,26jul07,y_w  Clean up and add support for Hackberry Evb.
01c,07dec01,tlc  Update to use TrueFFS with Project Facility.
01b,16jul01,pes  Add CoE Copyright comment
01a,15may01,pes  created from ddb5476 version.
*/

/*
DESCRIPTION
This library provides board-specific hardware access routines for TrueFFS.  
In effect, these routines comprise the socket component driver (or drivers)
for your flash device hardware.  At socket registration time, TrueFFS stores 
pointers to the functions of this socket component driver in an 'FLSocket' 
structure.  When TrueFFS needs to access the flash device, it uses these 
functions.  

Because this file is, for the most part, a device driver that exports its 
functionality by registering function pointers with TrueFFS, very few of the 
functions defined here are externally callable.  For the record, these 
external functions are flFitInSocketWindow() and flDelayLoop().  You should 
never have any need to call these functions.  

However, one of the most import functions defined in this file is neither
referenced in an 'FLSocket' structure, nor is it externally callable.  This
function is sysTffsInit().  TrueFFS calls this function at initialization 
time to register socket component drivers for all the flash devices attached 
to your target.  It is this call to sysTffs() that results in assigning 
drive numbers to the flash devices on your target hardware.  Drive numbers 
are assigned by the order in which the socket component drivers are registered.
The first to be registered is drive 0, the second is drive 1, and so on up to 
4.  As shipped, TrueFFS supports up to five flash drives.  

After registering socket component drivers for a flash device, you may 
format the flash medium even though there is not yet a block device driver
associated with the flash (see the reference entry for the tffsDevCreate() 
routine).  To format the flash medium for use with TrueFFS, 
call tffsDevFormat().

Finally, this file also contains define statements for symbolic constants 
that determine which MTDs, translation layer modules, and other utilities 
are ultimately included in TrueFFS.  These defines are as follows:

.IP "INCLUDE_TL_NFTL"
To include the NAND-based translation layer module.
.IP "INCLUDE_TL_FTL"
To include the NOR-based translation layer module.
.IP "INCLUDE_TL_SSFDC"
To include the SSFDC-appropriate translation layer module.
.IP "INCLUDE_MTD_I28F016"
For Intel 28f016 flash devices.
.IP "INCLUDE_MTD_I28F008"
For Intel 28f008 flash devices.
.IP "INCLUDE_MTD_I28F008_BAJA"
For Intel 28f008 flash devices on the Heurikon Baja 4700.
.IP "INCLUDE_MTD_AMD"
For AMD, Fujitsu: 29F0{40,80,16} 8-bit flash devices.
.IP "INCLUDE_MTD_CDSN"
For Toshiba, Samsung: NAND CDSN flash devices.
.IP "INCLUDE_MTD_DOC2"
For Toshiba, Samsung: NAND DOC flash devices.
.IP "INCLUDE_MTD_CFISCS"
For CFI/SCS flash devices.
.IP "INCLUDE_MTD_WAMD"
For AMD, Fujitsu 29F0{40,80,16} 16-bit flash devices.
.IP "INCLUDE_TFFS_BOOT_IMAGE"
To include tffsBootImagePut() in TrueFFS for Tornado.
.LP
To exclude any of the modules mentioned above, edit sysTffs.c and undefine
its associated symbolic constant.

INCLUDE FILES: flsocket.h, tffsDrv.h
*/

#include <vxWorks.h>
#include <taskLib.h>
#include "config.h"

#include <tffs/flsocket.h>
#include <tffs/tffsDrv.h>

#ifdef INCLUDE_TFFS

/* defines */

#define FLASH_28F128J3          0x8918
#define TFFS_BASE_ADDR          FLASH_ADRS      /* use upper 2meg of bootrom */
#define TFFS_WIN_SIZE           0x00800000      /* 8 meg size */
#define TFFS_FLASH_SIZE         0x00800000      /* 8 meg size */
#define TFFS_ERASE_BLK_SIZE     0x0040000l      /* 64K*2 block size */
#define TFFS_FLASH_TYPE         FLASH_28F128J3

#define INCLUDE_TL_FTL          /* FTL translation layer */
#define INCLUD_TFFS_SHOW

#define INCLUDE_MTD_USR
#define MTD_USR_IDENTIFY        maltaRfaMTDIdentify

#define FLASH_WRITE_CMD         0x40404040
#define FLASH_BLOCK_ERASE_CMD   0x20202020
#define FLASH_ERASE_CONF_CMD    0xd0d0d0d0
#define FLASH_CLR_STS_CMD       0x50505050
#define FLASH_READ_ARRAY_CMD    0xffffffff

#define FLASH_COMPLETE          0x80008000
#define FLASH_ERASE_ERROR       0xb000b000

/* forward declarations */

LOCAL FLBoolean rfaCardDetected (FLSocket vol);
LOCAL void      rfaVccOn (FLSocket vol);
LOCAL void      rfaVccOff (FLSocket vol);

#ifdef  SOCKET_12_VOLTS
LOCAL FLStatus  rfaVppOn (FLSocket vol);
LOCAL void      rfaVppOff (FLSocket vol);
#endif  /* SOCKET_12_VOLTS */

LOCAL FLBoolean rfaGetAndClearCardChangeIndicator (FLSocket vol);
LOCAL FLBoolean rfaWriteProtected (FLSocket vol);
LOCAL void      rfaSetWindow (FLSocket vol);
LOCAL void      rfaSetMappingContext (FLSocket vol, unsigned page);
LOCAL FLStatus  rfaSocketInit (FLSocket vol);
LOCAL FLStatus  rfaRegister (void);

#ifndef DOC
#include "tffs/tffsConfig.c"
#endif /* DOC */


/*******************************************************************************
*
* sysTffsInit - board level initialization for TFFS
*
* This routine calls the socket registration routines for the socket component
* drivers that will be used with this BSP. The order of registration signifies
* the logical drive number given to the drive associated with the socket.
*
* RETURNS: N/A
*/

LOCAL void sysTffsInit (void)
    {
    rfaRegister ();
    }

/*******************************************************************************
*
* rfaRegister - install routines for the RFA
*
* This routine installs necessary functions for the RFA.
*
* RETURNS: flOK, or flTooManyComponents if there're too many drives
*/

LOCAL FLStatus  rfaRegister (void)
    {
    FLSocket  vol = flSocketOf (noOfDrives);

    if (noOfDrives >= DRIVES)
        return (flTooManyComponents);

    tffsSocket[noOfDrives] = "RFA";
    noOfDrives++;

    vol.serialNo = 0;
    vol.window.baseAddress = TFFS_BASE_ADDR >> 12;

    /* fill in function pointers */
    vol.cardDetected      = rfaCardDetected;
    vol.VccOn             = rfaVccOn;
    vol.VccOff            = rfaVccOff;
#ifdef SOCKET_12_VOLTS
    vol.VppOn             = rfaVppOn;
    vol.VppOff            = rfaVppOff;
#endif /* SOCKET_12_VOLTS */
    vol.initSocket        = rfaSocketInit;
    vol.setWindow         = rfaSetWindow;
    vol.setMappingContext = rfaSetMappingContext;
    vol.getAndClearCardChangeIndicator =
                          rfaGetAndClearCardChangeIndicator;
    vol.writeProtected    = rfaWriteProtected;

    return (flOK);
    }

/*******************************************************************************
*
* rfaCardDetected - detect if a card is present (inserted)
*
* This routine detects if a card is present (inserted).
*
* RETURNS: TRUE, or FALSE if the card is not present.
*/

LOCAL FLBoolean rfaCardDetected 
    (
    FLSocket vol        /* pointer identifying drive */
    )
    {
    return (TRUE);
    }

/*******************************************************************************
*
* rfaVccOn - turn on Vcc (3.3/5 Volts)
*
* This routine turns on Vcc (3.3/5 Volts).  Vcc must be known to be good
* on exit.
*
* RETURNS: N/A
*/

LOCAL void  rfaVccOn 
    (
    FLSocket vol        /* pointer identifying drive */
    )
    {
    }

/*******************************************************************************
*
* rfaVccOff - turn off Vcc (3.3/5 Volts)
*
* This routine turns off Vcc (3.3/5 Volts). 
*
* RETURNS: N/A
*/

LOCAL void  rfaVccOff 
    (
    FLSocket vol        /* pointer identifying drive */
    )
    {
    }

#ifdef SOCKET_12_VOLTS

/*******************************************************************************
*
* rfaVppOn - turns on Vpp (12 Volts)
*
* This routine turns on Vpp (12 Volts). Vpp must be known to be good on exit.
*
* RETURNS: flOK always
*/

LOCAL FLStatus rfaVppOn 
    (
    FLSocket vol        /* pointer identifying drive */
    )
    {
    return (flOK);
    }

/*******************************************************************************
*
* rfaVppOff - turns off Vpp (12 Volts)
*
* This routine turns off Vpp (12 Volts).
*
* RETURNS: N/A
*/

LOCAL void  rfaVppOff 
    (
    FLSocket vol        /* pointer identifying drive */
    ) 
    {
    }

#endif  /* SOCKET_12_VOLTS */

/*******************************************************************************
*
* rfaSocketInit - perform all necessary initializations of the socket
*
* This routine performs all necessary initializations of the socket.
*
* RETURNS: flOK always
*/

LOCAL FLStatus  rfaSocketInit 
    (
    FLSocket vol        /* pointer identifying drive */
    )
    {
    vol.cardChanged = FALSE;

    /* enable memory window and map it at address 0 */

    rfaSetWindow (&vol);

    return (flOK);
    }

/*******************************************************************************
*
* rfaSetWindow - set current window attributes, Base address, size, etc
*
* This routine sets current window hardware attributes: Base address, size,
* speed and bus width.  The requested settings are given in the 'vol.window' 
* structure.  If it is not possible to set the window size requested in
* 'vol.window.size', the window size should be set to a larger value, 
* if possible. In any case, 'vol.window.size' should contain the 
* actual window size (in 4 KB units) on exit.
*
* RETURNS: N/A
*/

LOCAL void  rfaSetWindow 
    (
    FLSocket vol        /* pointer identifying drive */
    )
    {
    /* Physical base as a 4K page */

    vol.window.baseAddress = TFFS_BASE_ADDR >> 12;

    flSetWindowSize (&vol, TFFS_WIN_SIZE >> 12);
    }

/*******************************************************************************
*
* rfaSetMappingContext - sets the window mapping register to a card address
*
* This routine sets the window mapping register to a card address.
* The window should be set to the value of 'vol.window.currentPage',
* which is the card address divided by 4 KB. An address over 128MB,
* (page over 32K) specifies an attribute-space address.
*
* The page to map is guaranteed to be on a full window-size boundary.
*
* RETURNS: N/A
*/

LOCAL void rfaSetMappingContext 
    (
    FLSocket vol,       /* pointer identifying drive */
    unsigned page       /* page to be mapped */
    )
    {
    }

/*******************************************************************************
*
* rfaGetAndClearCardChangeIndicator - return the hardware card-change indicator
*
* This routine returns the hardware card-change indicator and clears it if set.
*
* RETURNS: FALSE, or TRUE if the card has been changed
*/

LOCAL FLBoolean rfaGetAndClearCardChangeIndicator 
    (
    FLSocket vol        /* pointer identifying drive */
    )
    {
    return (FALSE);
    }

/*******************************************************************************
*
* rfaWriteProtected - return the write-protect state of the media
*
* This routine returns the write-protect state of the media
*
* RETURNS: FALSE, or TRUE if the card is write-protected
*/

LOCAL FLBoolean rfaWriteProtected 
    (
    FLSocket vol        /* pointer identifying drive */
    )
    {
    return (FALSE);
    }

/*******************************************************************************
*
* flDelayLoop - consume the specified time
*
* This routine consumes the specified time.
*
* RETURNS: N/A
*/

void flDelayLoop 
    (
    int cycles          /* loop count to be consumed */
    )
    {
    /* No need for delay on this board */

    return;
    }

/*
 * The remainder of this file implements the functionality of an MTD for
 * the resident flash array on the Malta board, consisting of a pair of 
 * Intel DT28F160 flash chips.
 */

typedef union
    {
    UCHAR   uchar[4];
    UINT32  uint32;
    } FL_DWORD;

/*******************************************************************************
*
* sysTffsFormat - format the flash memory above an offset
*
* This routine formats the flash memory.  The lower 1M (ROM_SIZE) of the flash
* memory is left unformatted.  If the lower 1M of the flash memory was
* previously formated by TrueFFS, and you are trying to format the rest,
* you need to erase the lower 1M of the flash memory before you re-format it.
* To do this, you could use:
* .CS
* tffsRawio(0, 3, 0, 8)
* .CE
* The first argument in the tffsRawio() command shown above is the TrueFFS
* drive number, 0.  The second argument, 3, is the function number (also
* known as TFFS_PHYSICAL_ERASE).  The third argument, 0, specifies the unit
* number of the first erase unit you want to erase.  The fourth argument, 8,
* specifies how many erase units you want to erase.
*
* RETURNS: OK, or ERROR if it fails.
*/

STATUS sysTffsFormat (void)
    {
    STATUS status;
    tffsDevFormatParams params = 
    {
    /* ROM_SIZE is reserved for boot image */

    {ROM_SIZE + 1, 99, 1, 0x10000l, NULL, {0,0,0,0}, NULL, 2, 0, NULL},
    FTL_FORMAT
    };

    status = tffsDevFormat (0, (int)&params);

    return (status);
    }

/******************************************************************************
*
* maltaRfaMTDWriteDword - write one dword to the flash
*
* This routine writes one dword to the flash using the I28F640J5 programming
* algorithm.
*
* NOMANUAL
*/

static FLStatus maltaRfaMTDWriteDword
    (
    volatile UINT32 *   flashPtr,
    UINT32      value
    )
    {
    /* Issue Write Word Command to Address */

    *flashPtr = FLASH_WRITE_CMD;
    
    /* Write data to Address */

    *flashPtr = value;

    while(*flashPtr != FLASH_COMPLETE);           /* Wait for completion */

    /* Issue Read Array Command to Reset */

    *flashPtr = FLASH_READ_ARRAY_CMD;

    return flOK;
    }

/******************************************************************************
*
* maltaRfaMTDWrite - Write a block of bytes to Flash
*
* This routine writes a block of bytes to the flash. It is registered as 
* the MTD's vol.write routine.
*
* RETURNS: flOK, or error if it fails
*/

static FLStatus maltaRfaMTDWrite 
    (
    FLFlash         vol,
    CardAddress     address,
    const void FAR1 *buffer,
    int             length,
    FLBoolean       overwrite
    )
    {
    int         i;
    int         cLength;
    int         tmp;
    UCHAR *     flashCPtr;
    UINT32 *    flashPtr;
    UCHAR *     bufCPtr;
    FL_DWORD    dword;

#ifdef TFFS_DEBUG
    logMsg("maltaRfaMTDWrite vol, %x, %x, %x, %x\n", (int)address, \
           (int)buffer, (int)length, (int)overwrite,0,0);
#endif /* TFFS_DEBUG */

    if (flWriteProtected (vol.socket))
    	return flWriteProtect;

    /* Initialize pointers */

    bufCPtr   = (UCHAR *) buffer;
    flashCPtr = (UCHAR *) vol.map (&vol, address, 0);
    flashPtr  = (UINT32 *)((UINT32)flashCPtr & ~0x3);
    cLength   = length;

    /* first take care of unaligned flashPtr */

    if ((tmp = ((UINT32)flashCPtr & 0x3)) != 0)
        {
        /* assemble a dword and write to flash */

        for (i = 0; i < tmp; i++)
            dword.uchar[i] = *(flashCPtr - tmp + i);

        for (     ; i < 4; i++)
            dword.uchar[i] = *(bufCPtr + i - tmp);
                 
        if (maltaRfaMTDWriteDword (flashPtr, dword.uint32) != flOK)
            {
            return flWriteFault;
            }

        cLength -= (4 - tmp);
        bufCPtr += (4 - tmp);
        flashPtr++;
        }

    /* Write all properly aligned 32-bit data */

    while (cLength >=4)
        {
        for (i = 0; i < 4; i++)
            dword.uchar[i] = *bufCPtr++;

        if (maltaRfaMTDWriteDword (flashPtr, dword.uint32) != flOK)
            {
            return flWriteFault;
            }

        flashPtr++;
        cLength -=4;
        }

    /* take care of remaining bytes */

    if (cLength > 0)
        {
        /* Assemble the last dword and write to flash */

        flashCPtr = (UCHAR*) flashPtr;

        for (i = 0; i < cLength; i++)
            dword.uchar[i] = *(bufCPtr + i);

        for (     ; i < 4; i++)
            dword.uchar[i] = *(flashCPtr + i);

        if (maltaRfaMTDWriteDword (flashPtr, dword.uint32) != flOK)
            {
            return flWriteFault;
            }
        }

    /* Verify by comparing data */

    flashCPtr = (UCHAR *) vol.map (&vol, address, 0);

    if (tffscmp ((void FAR0 *)flashCPtr, buffer, length))
        {
        return flWriteFault;
        }

    return flOK;
    }

/******************************************************************************
*
* maltaRfaMTDErase - erase the contents of one or more sectors
*
* This routine erases one or more contiguous Flash erasable blocks. It is 
* registered as the MTD's vol.erase routine.
*
* RETURNS: flOK, or error if it fails
*/

static FLStatus maltaRfaMTDErase
    (
    FLFlash     vol,
    int         firstBlock,
    int         numOfBlocks
    )
    {
    volatile UINT32 *   flashPtr;
    int                 iBlock;

#ifdef TFFS_DEBUG
    logMsg("\rFLFlash(addr = 0x%x) Erasing sector ... %d\t\t",&vol,firstBlock,0,0,0,0);
#endif /* TFFS_DEBUG */

    if (flWriteProtected(vol.socket))
        {
#ifdef TFFS_DEBUG
        logMsg("maltaRfaMTDErase: Write Protect\n",0,0,0,0,0,0);
#endif /* TFFS_DEBUG */

        return flWriteProtect;
        }
    
    for (iBlock = firstBlock;  iBlock < firstBlock + numOfBlocks;  iBlock++)
        {
        flashPtr = vol.map (&vol, iBlock * vol.erasableBlockSize, 0);

#ifdef TFFS_DEBUG
        logMsg("maltaRfaMTDErase: iBlock = 0x%x, flashPtr = 0x%x\n",
               (int)iBlock, (int)flashPtr,0,0,0,0);
        logMsg("FLASH_BLOCK_ERASE_CMD = 0x%x\n",
               FLASH_BLOCK_ERASE_CMD,0,0,0,0,0);
        logMsg("FLASH_ERASE_CONF_CMD = 0x%x\n",
               FLASH_ERASE_CONF_CMD,0,0,0,0,0);
        logMsg("FLASH_READ_ARRAY_CMD = 0x%x\n",
               FLASH_READ_ARRAY_CMD,0,0,0,0,0);
        logMsg("FLASH_COMPLETE = 0x%x\n",
               FLASH_COMPLETE,0,0,0,0,0);
        logMsg("FLASH_ERASE_ERROR = 0x%x\n",
               FLASH_ERASE_ERROR,0,0,0,0,0);
        logMsg("FLASH_CLR_STS_CMD = 0x%x\n",
               FLASH_CLR_STS_CMD,0,0,0,0,0);
#endif /* TFFS_DEBUG */

        /* Issue Block Erase Command */

        *flashPtr = FLASH_BLOCK_ERASE_CMD;

        /* Issue Confirmation Command */

        *flashPtr = FLASH_ERASE_CONF_CMD;

        while(*flashPtr != FLASH_COMPLETE)         /* Wait for completion */
            {
            if (*flashPtr == FLASH_ERASE_ERROR)
                {
                /* Issue Read Array Command to Reset */

                *flashPtr = FLASH_READ_ARRAY_CMD;

                /* Issue Clear Status Register Command */

                *flashPtr = FLASH_CLR_STS_CMD;

#ifdef TFFS_DEBUG
                logMsg("maltaRfaMTDErase write fault\n",0,0,0,0,0,0);
#endif /* TFFS_DEBUG */
                return flWriteFault;
                }

#ifdef TFFS_DEBUG
            logMsg("*flashPtr = 0x%x\n", (int)(*flashPtr),0,0,0,0,0);
#endif /* TFFS_DEBUG */
            }

        /* Issue Read Array Command to Reset */

        *flashPtr = FLASH_READ_ARRAY_CMD;
        }

#ifdef TFFS_DEBUG
    logMsg("maltaRfaMTDErase: complete\n", 0,0,0,0,0,0);
#endif /* TFFS_DEBUG */
    return flOK;
    }


/******************************************************************************
*
* maltaRfaMTDIdentify - Identifies flash media on the Malta board
*
* This routine fills out the Flash structure and registers the write and
* erase routines.
*
* RETURNS: flOK, or error if it fails
*/

FLStatus maltaRfaMTDIdentify
    (
    FLFlash vol
    )
    {
    /* Issue read array command */

    *(UINT32 *)TFFS_BASE_ADDR = FLASH_READ_ARRAY_CMD;

    flSetWindowBusWidth (vol.socket,32);      /* use 32-bits */
    flSetWindowSpeed (vol.socket,120);        /* 120 nsec.   */

    /* Flash configuration */

    vol.type              = TFFS_FLASH_TYPE;
    vol.interleaving      = 2;
    vol.chipSize          = TFFS_FLASH_SIZE / 2;
    vol.noOfChips         = 2;
    vol.erasableBlockSize = TFFS_ERASE_BLK_SIZE;
    vol.flags            |= SUSPEND_FOR_WRITE;

    /* Register our flash handlers */

    vol.write             = maltaRfaMTDWrite;
    vol.erase             = maltaRfaMTDErase;

    return flOK;
    }

#endif /* INCLUDE_TFFS */
