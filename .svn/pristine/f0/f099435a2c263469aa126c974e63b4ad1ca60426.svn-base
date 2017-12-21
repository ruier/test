/* sysFlashTffs - MTD for AMD S29GL512/256/128N MirrorBit Flash  */

/* Copyright 1984-2003 Wind River Systems, Inc. */
/* Copyright 2005 Motorola, Inc. All Rights Reserved */

#include "copyright_wrs.h"

/*
modification history
--------------------
01a,19aug05,cak	 Ported and modified from 01d,mv6100/strataFlash.c 
*/

/*
DESCRIPTION

This is an AMD S29GL512/256/128N MirrorBit flash specific MTD for TFFS.
It is specifically designed to be used with the sysAmdS29GLxxxN.c 
driver and makes use of the flash driver functions in sysAmdS29GLxxxN.c. 
*/

#include "vxWorks.h"
#include "config.h"
#include "amdS29GLxxxN.h"

#include <stdio.h>
#include "tffs/flflash.h"
#include "tffs/backgrnd.h"

/* defines */

#undef	MTD_DEBUG
#undef	MTD_DEBUG_ERROR

#if (_BYTE_ORDER == _BIG_ENDIAN)

#define CFI_LONGSWAP(x)	LONGSWAP(x)	/* swap everything in a long word */
#define CFI_WORDSWAP(x) WORDSWAP(x)	/* swap the shorts in a long */
#define CFI_BYTESWAP(x) ( ( (x & 0xff) << 8) | ( (x >> 8) & 0xff) ) 

#else

#define CFI_LONGSWAP(x)	(x)		/* pass it as is */
#define CFI_WORDSWAP(x) (x)		/* pass it as is */
#define CFI_BYTESWAP(x) (x)		/* pass it as is */

#endif /* _BYTE_ORDER == _BIG_ENDIAN */

#define CFI_LONG_WRITE(addr, data)  (*(ULONG *)addr = \
				     (ULONG)(CFI_LONGSWAP(data)))
#define CFI_WORD_WRITE(addr, data)  (*(USHORT *)addr = \
				     (USHORT)(CFI_WORDSWAP(data)))
#define CFI_WORD_READ(addr)         (CFI_BYTESWAP(*(USHORT *)addr))
#define CFI_LONG_READ(addr)         (CFI_WORDSWAP(*(ULONG *)addr))

#define READ_ARRAY              0xff

/* 
 * Instance specific CFI data so that a system may have more 
 * than one CFI device. 
 */

typedef struct
    {
    unsigned         commandSetId;       /* id of a specific command set. */
    unsigned         altCommandSetId;    /* id of alternate command set.  */
    FLBoolean        wordMode;           /* TRUE - word mode.             */
    				         /* FALSE - byte mode.            */
    int              multiplier;         /* the number of bytes in 	  */
    					 /* addressing scheme	          */
    int              interleaveFactor;   /* Number of bytes interleaved   */
    unsigned         maxBytesWrite;      /* maximum number of bytes       */
    					 /* in multi-byte write.          */
    FLBoolean        vpp;                /* if = TRUE, need vpp.          */
    long             optionalCommands;   /* optional commands supported   */
					 /* (1 = yes, 0 = no):            */
					 /* bit 0 - chip erase.           */
					 /* bit 1 - suspend erase.        */
					 /* bit 2 - suspend write         */
					 /* bit 3 - lock/unlock.          */
					 /* bit 4 - queued erase.         */
    unsigned         afterSuspend;       /* functions supported after     */
					 /* suspend (1 = yes, 0 = no):    */
					 /* bit 0 - write after erase     */
					 /*         suspend.              */
    } CFI;

LOCAL CFI mtdVars[DRIVES];

/* globals */

UINT32 tffsBankSize = 0x0;
UINT32 flashSectorSize;
int flashNumSectors;

/* externals */

IMPORT void sysFlashSwWriteProtectSet (BOOL);
IMPORT BOOL sysFlashIsHwWriteProtected (void);

/*****************************************************************************
 *
 * sysFlashTFFSWrite - write routine 
 *                                                                     
 * Write a block of bytes to Flash.  This routine will be registered as 
 * the MTD flash.write routine.
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

LOCAL FLStatus sysFlashTFFSWrite
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

    /* clear the SW write protection */

    sysFlashSwWriteProtectSet(FALSE);

    if ( flWriteProtected(vol.socket) )
        return flWriteProtect;

    flashPtr = (FlashDPTR)flMap(vol.socket, address);

    if (flashUtilSectorProgram((vol.socket->window.baseAddress << 12),
			       (UINT32)flashPtr,(UINT8*)buffer,length,
			       flashSectorSize,flashNumSectors)!=OK )
        {
#ifdef MTD_DEBUG_ERROR
        logMsg("Debug: Flash write failed:0x%x, 0x%x\n",(UINT32)flashPtr,
		length,0,0,0,0);
#endif
        return flWriteFault;
        }

    flashPtr = (FlashDPTR) flMap(vol.socket, address);

    /* verify the data */

    if (tffscmp((void FAR0 *) flashPtr,buffer,length))
        {
#ifdef MTD_DEBUG_ERROR
        logMsg("Debug: Flash write failed in verification.\n",0,0,0,0,0,0);
#endif
        status = flWriteFault;
        }
    else
        {
#ifdef MTD_DEBUG
        logMsg("Wrote %d bytes successfully @ 0x%x\n",length,flashPtr,0,0,0,0);
#endif
        }
    return status;
    }

/***************************************************************************
 *
 * sysFlashTFFSErase - Erase specified number of blocks
 *
 * Given the first block and number of contiguous blocks to erase, this 
 * routine will erase the content of the specified region. 
 *
 * Parameters:                     
 *      vol                  : Pointer identifying drive
 *      firstEraseblock      : first block to be erased 
 *      numberOfEraseBlocks  : number of blocks to erase  
 *  
 * Returns:
 *      FLStatus        : 0 if erase succeeded, failed otherwise
 * 
 * NOMANUAL
 *
 */

LOCAL FLStatus sysFlashTFFSErase
    (
    FLFlash vol,
    int firstEraseBlock,
    int numberOfEraseBlocks
    )
    {
    FLStatus status = flOK;       /* unless proven otherwise */
    int iBlock;

    /* Clear the SW write protection */

    sysFlashSwWriteProtectSet(FALSE);

    if ( flWriteProtected(vol.socket) )
        return flWriteProtect;

    for ( iBlock = firstEraseBlock; 
          iBlock < firstEraseBlock + numberOfEraseBlocks && status == flOK;
          iBlock++ )
        {
#ifdef MTD_DEBUG
        logMsg("Erasing block %d\n",iBlock,0,0,0,0,0);
#endif
        if (flashSectorErase(vol.socket->window.baseAddress<<12, iBlock,
			     flashSectorSize, flashNumSectors)!=OK )
            {
#ifdef MTD_DEBUG_ERROR
	    logMsg("Debug: Flash erase failed: %d\n\r",iBlock,0,0,0,0,0);
#endif
            return flWriteFault ;
            }
        status = flOK ;
        }

    return status;
    }

/***************************************************************************
 * 
 * sysFlashTFFSMTDMap - map routine for MTD     
 *                                  
 * Map through buffer. This routine will be registered as the map  
 * routine for this MTD.                 
 *                                  
 * Parameters:                     
 *      vol     : Pointer identifying drive
 *      address : Flash address to be mapped. 
 *      length  : number of bytes to map.    
 *                                  
 * Returns:
 *  Pointer to the buffer data was mapped to. 
 *                                  
 */

static void FAR0 * sysFlashTFFSMTDMap 
    ( 
    FLFlash vol,
    CardAddress address,
    int length 
    )
    {

    /* CardAddress is typedef - unsigned long */

    UINT32 adrs=0 ;
    adrs = ((vol.socket->window.baseAddress<<12) + address) ;
            
    return(void FAR0 *)adrs;
    }

/***************************************************************************
 *
 * sysFlashTFFSIdentify - Identification routine for AMD S29GL512/256/128N
 * MirrorBit Flash Devices
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

FLStatus sysFlashTFFSIdentify
    (
    FLFlash vol
    )
    {
    UINT32 sectorSize;
    int numSectors;
    UINT32 flashBaseAddr = vol.socket->window.baseAddress<<12;

    FlashPTR flashPtr = (FlashPTR) flMap(vol.socket, 0);
    FlashDPTR flashDPtr = (FlashDPTR) flashPtr;

    if (sysFlashIsHwWriteProtected() == TRUE)
	{
	printf("Flash bank is hardware write protected!\n\r");
        return flWriteFault;
	}

    /* clear the SW write protection */

    sysFlashSwWriteProtectSet(FALSE);

#ifdef MTD_DEBUG
    logMsg("base address = 0x%x\n\r", flashBaseAddr,0,0,0,0,0);
#endif

    flashInfoGet(flashBaseAddr, &sectorSize, &numSectors);
    flashSectorSize = sectorSize;
    flashNumSectors = numSectors;
#ifdef MTD_DEBUG
    logMsg("sectorSize = 0x%x\n\r", flashSectorSize,0,0,0,0,0);
    logMsg("numSectors = 0x%x\n\r", flashNumSectors,0,0,0,0,0);
#endif

    flSetWindowBusWidth(vol.socket, 16);  /* use 16-bits */
    flSetWindowSpeed(vol.socket, 120);    /* 120 nsec. */
    flSetWindowSize(vol.socket, 2);       /* 8 KBytes */

    vol.write = sysFlashTFFSWrite ;
    vol.erase = sysFlashTFFSErase ;
    vol.type = 0 ; 

    /* Make flash look like 1 big flash part. */

    vol.erasableBlockSize = sectorSize;
    vol.noOfChips = 1;

    /* can only use half of the flash bank - to protect the boot blocks */

    tffsBankSize = (flashSectorSize * flashNumSectors) / 2;
    vol.chipSize = tffsBankSize;

#ifdef MTD_DEBUG
    logMsg("tffsBankSize = 0x%x\n\r", tffsBankSize,0,0,0,0,0);
#endif
     
    vol.interleaving = 1 ;
    vol.map = sysFlashTFFSMTDMap ;

    /* place to store all the flash parameters */

    vol.mtdVars = &mtdVars[flSocketNoOf(vol.socket)];

    flashDPtr[0] = READ_ARRAY;

    /* Unlock the flash devices */

    if (flashNVSectorProtectPPBIsSet(flashBaseAddr, flashSectorSize,
                                     flashNumSectors) == TRUE)
        {
        flashNVSectorProtectPPBErase(flashBaseAddr);
        }
    return flOK;
    }
