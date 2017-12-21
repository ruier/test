/* strataflash.c - MTD for Intel 28F128 */

/*
 * Copyright (c) 2002-2004, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,08sep11,sye  Return both chips to read array mode. (WIND00295145)
01c,30jan04,nrv  Modified for mv5500
01b,28mar03,pmr  Reworked debug facility to use logMsg().
01a,07may02,gtf  New.
*/

/*
DESCRIPTION

This is an Intel 28F128 specific strataflash MTD for TFFS.
It is specifically designed to be used with the sysFlash.c
driver provided for the mv5500 bsp and makes use of the
flash driver functions in sysFlash.c.

This module can be specifically configured to use either
byte write or write buffer modes of the 28F128 strataflash
device by configuring the STRATAFLASH_WRITE_BUFFER_MODE macro
in config.h.


NOMANUAL
*/

#include "vxWorks.h"
#include "config.h"
#include "sysFlash.h"

#include <stdio.h>
#include "tffs/flflash.h"
#include "tffs/backgrnd.h"

/* defines */

/* disable debugging */
#undef CFI_DEBUG

#ifdef CFI_DEBUG
#define DEBUG_MSG(X0, X1, X2, X3, X4, X5, X6)			\
    do {							\
	if (strataDebug)					\
            if (_func_logMsg != NULL)				\
                _func_logMsg (X0, (int)X1, (int)X2, (int)X3,	\
		    	      (int)X4, (int)X5, (int)X6);	\
    } while (0)
#else
#define DEBUG_MSG(X0, X1, X2, X3, X4, X5, X6)
#endif

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


#define READ_ARRAY              0xff


/* Instance specific CFI data so that a system may have more than one CFI
 * device. */
typedef struct
    {

    unsigned         commandSetId;            /* id of a specific command set. */
    unsigned         altCommandSetId;         /* id of alternate command set.  */
    FLBoolean        wordMode;                /* TRUE - word mode.             */
    					      /* FALSE - byte mode.            */
    int              multiplier;              /* the number of bytes in 	     */
    /* addressing scheme	     */
    int          interleaveFactor; /* Number of bytes interleaved   */
    unsigned         maxBytesWrite;           /* maximum number of bytes    */
    					      /* in multi-byte write.       */
    FLBoolean        vpp;                     /* if = TRUE, need vpp.       */
    long             optionalCommands;        /* optional commands supported*/
					/* (1 = yes, 0 = no):            */
					/* bit 0 - chip erase.           */
					/* bit 1 - suspend erase.        */
					/* bit 2 - suspend write         */
					/* bit 3 - lock/unlock.          */
					/* bit 4 - queued erase.         */
    unsigned         afterSuspend;            /* functions supported after  */
					      /* suspend (1 = yes, 0 = no): */
					      /* bit 0 - write after erase  */
					      /*         suspend.           */

    } CFI;

LOCAL CFI mtdVars[DRIVES];

int strataDebug = 0;

/*****************************************************************************
 * strataFlashWrite - write routine for 16 bit devices 
 *                                                                     
 * Write a block of bytes to Flash in a word-mode.                    
 *                                                                   
 * This routine will be registered as the MTD flash.write routine   
 *                                                                 
 * Parameters:                                                    
 *      vol             : Pointer identifying drive              
 *      address         : Card address to write to              
 *      buffer          : Address of data to write             
 *      length          : Number of bytes to write            
 *      mode            : write mode (overwrite yes/no)      
 *                                                          
 * Returns:                                                
 *      FLStatus        : 0 on success, failed otherwise  
 *
 * NOMANUAL
 *
 */

LOCAL FLStatus strataFlashWrite
    (
    FLFlash vol,
    CardAddress address,
    const void FAR1 *buffer,
    int length,
    int mode /* ignored */
    )
    {
    FLStatus status = flOK;
    FlashDPTR flashPtr;

    if (flWriteProtected(vol.socket))
        return flWriteProtect;

    flashPtr = (FlashDPTR)flMap(vol.socket, address);

#ifdef STATAFLASH_WRITE_BUFFER_MODE
    if (sysFlashWriteBufferStrataflash((UINT8*)flashPtr,
                                       (UINT8*)buffer, length) != OK)
#else
    if (sysFlashWriteStrataflash((UINT8*)flashPtr, (UINT8*)buffer, length)!=OK)
#endif /* STATAFLASH_WRITE_BUFFER_MODE */
        {
        DEBUG_MSG("Debug: StrataFlash write failed.\n", 1, 2, 3, 4, 5, 6);
        return flWriteFault;
        }

    flashPtr = (FlashDPTR) flMap(vol.socket, address);
    /* verify the data */
    if (tffscmp((void FAR0 *)flashPtr, buffer, length))
        {
        DEBUG_MSG("Debug: StrataFlash write failed in verification.\n", 1, 2, 3, 4, 5, 6);
        status = flWriteFault;
        }
    else
        {
        DEBUG_MSG("Wrote %d bytes successfully @ 0x%x\n", length, flashPtr, 3, 4, 5, 6);
        }

    return status;
    }



/***************************************************************************
 * strataFlashErase - Erase specified number of blocks
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
 * NOMANUAL
 *
 */

LOCAL FLStatus strataFlashErase
    (
    FLFlash vol,
    int firstEraseBlock,
    int numberOfEraseBlocks
    )
    {
    FLStatus status = flOK;       /* unless proven otherwise */
    int iBlock;

    if ( flWriteProtected(vol.socket) )
        return flWriteProtect;

    for ( iBlock = firstEraseBlock; 
        iBlock < firstEraseBlock + numberOfEraseBlocks && status == flOK;
        iBlock++ )
        {
        FlashPTR flashPtr = (FlashPTR) flMap(vol.socket,
                                             iBlock * vol.erasableBlockSize);
        DEBUG_MSG("Erasing block @ 0x%x\n", flashPtr, 2, 3, 4, 5, 6);
        if ( sysFlashEraseStrataflash((UINT8*)flashPtr)!=OK )
            {
            return flWriteFault ;
            }
        status = flOK ;
        }

    return status;
    }


/*----------------------------------------------------------------------
 *                strataFlashMTDMap      
 *                                  
 * Map through buffer. This routine will be registered as the map  
 * routine for this MTD.                 
 *                                  
 * Parameters:                     
 *  vol     : Pointer identifying drive
 *  address : Flash address to be mapped. 
 *  length  : number of bytes to map.    
 *                                  
 * Returns:
 *  Pointer to the buffer data was mapped to. 
 *                                  
 *----------------------------------------------------------------------*/

static void FAR0 * strataFlashMTDMap 
    ( 
    FLFlash vol,
    CardAddress address,
    int length 
    )
    {
    /* CardAddress is typedef - unsigned long */
    UINT32 adrs=0 ;
    adrs = FLASH_BASE_ADRS+address ;
            
    return(void FAR0 *)adrs;
    }


/***************************************************************************
 * strataFlashIdentify - Identification routine for Intel 28F640
 *
 *
 * Parameters:
 *      vol             : Pointer identifying drive 
 *
 * Returns:
 *      FLStatus        : 0 on positive identificaion, failed otherwise 
 *
 * NOMANUAL
 *
 */

FLStatus strataFlashIdentify
    (
    FLFlash vol
    )
    {
    FlashPTR flashPtr = (FlashPTR) flMap(vol.socket, 0);
    FlashDPTR flashDPtr = (FlashDPTR) flashPtr;

    DEBUG_MSG("Debug: entering strataFlash identification routine.\n vol = 0x%x, flashPtr = 0x%x\n", (int)&vol, (int)flashPtr, 3, 4, 5, 6);

    flSetWindowBusWidth(vol.socket, 16);/* use 16-bits */
    flSetWindowSpeed(vol.socket, 120);  /* 120 nsec. */
    flSetWindowSize(vol.socket, 2);       /* 8 KBytes */

    if ( sysFlashReadIdIntel((FlashWord*)flashPtr) != OK )
        {
        printf("Can't find Intel Strataflash!\n");
        return flGeneralFailure ;
        }

    vol.write = strataFlashWrite ;
    vol.erase = strataFlashErase ;
    vol.type = 0 ; /* JEDEC ID - ?, not needed. */

    /* make flash look like 1 big flash part */

    vol.erasableBlockSize = TFFS_DEVICE_SECTOR_SZ ;
    vol.noOfChips = 1 ;

    vol.chipSize = FLASH_SIZE;
     
    vol.interleaving = 1 ;
    vol.map = strataFlashMTDMap ;

    /* place to store all the flash parameters */

    vol.mtdVars = &mtdVars[flSocketNoOf(vol.socket)];

    DEBUG_MSG("Debug: No of chips detected is %d\n", vol.noOfChips, 2, 3, 4, 5, 6);

    /* return both flash chips to read array mode */

    flashDPtr[0] = (READ_ARRAY << 16) | READ_ARRAY;

    return flOK;
    }
