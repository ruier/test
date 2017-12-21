/* s29glMtd.c - MTD for S29GL parts that comply with CFI */

/*
 * Copyright (c) 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* FAT-FTL Lite Software Development Kit
 * Copyright (C) M-Systems Ltd. 1995-2001       */

/*
modification history
--------------------
01b,18mar07,x_s  Modify to support flash type S29GL256N.
01a,18jan07,b_m  Written based on cfiamd.c
*/

/*
DESCRIPTION
This library provides an MTD for devices that conform to the Common Flash
Interface (CFI) and the AMD/Fujitsu command set. The library provides
identification, erase and write routines for 8 and 16 bit devices in
native or byte mode. Support is also added for word width devices
interleaved word wide where bytes 0 and 1 would be on device one
and bytes 2 and 3 would be on device two, and so on. Support is also present
for configurations that only permit double word width writes.

The identification process supports byte, word, and double word width devices
as specified by the CFI query process. Vendr specific extentions are made
available from an instance specific data structure, referred to in this file
as thisCFI.

In addition to supporting a boot code region through the format process this
driver is also able to reserve the last erase block of each device in the array
so the vxWorks may use this region as NVRAM if the macro SAVE_NVRAM_REGION is
defined.

The low voltage devices that support boot blocks get no special treatment in
this driver. The asymmetric regions are regocgnized from the erase block region
information retreaved during CFI identification. They can, therefore be used by
the the file system just like any other sector, if they are not used for boot
purposes. Since TrueFFS does not support uneven sector distribution, the size of
the largest sector is taken as the erase block size. When an erase request
corresponds to the boot block region, all sectors in the boot block are erased.
As such, the NVRAM area saved on a top boot device is the entire boot block and
not just the last sector as one might expect.

Based on the observation that CFI data on the sector distribution on AMD devices
always describes bottom boot devices, this driver does an "autoselect" to
determine the boot block type for proper interpretation of the device geometry
data. The erase process assumes that bottom boot devices have the boot block
located in sector zero. The first erase block encountered in an erase request
that has a sector size other than vol.erasableBlockSize on top boot devices is
assumed to be the boot block anchor on top boot devices.


NOMANUAL
*/

#include <stdio.h>
#include "tffs/flflash.h"
#include "tffs/backgrnd.h"


#define MAXSECREGS  4
#define	MAXSECTORS  256

typedef enum
    {
    BOOTBLOCK_NONE,
    BOOTBLOCK_TOP,
    BOOTBLOCK_BOTTOM
    }BOOTBLOCK;

typedef union
    {
    UCHAR uchar[4];
    UINT32 uint32;
    } CFI_DWORD;

typedef union
    {
    UCHAR uchar[2];
    USHORT ushort;
    } CFI_WORD;

/* Instance specific CFI data so that a system may have more than one CFI
 * device. */
typedef struct {

  unsigned      commandSetId;           /* id of a specific command set. */
  unsigned      altCommandSetId;        /* id of alternate command set.  */
  FLBoolean     wordMode;               /* TRUE - word mode.             */
                                        /* FALSE - byte mode.            */
  int           multiplier;             /* the number of bytes between   */
					                    /* 1st Q and 1st R in query	     */
  int           interleaveWidth;	    /* 1 - byte 2 - word interleaved */
  unsigned      maxBytesWrite;          /* maximum number of bytes       */
					                    /* in multi-byte write.          */
  FLBoolean     vpp;                    /* if = TRUE, need vpp.          */
  long          optionalCommands;       /* optional commands supported   */
                					    /* (1 = yes, 0 = no):            */
                					    /* bit 0 - chip erase.           */
                					    /* bit 1 - suspend erase.        */
                					    /* bit 2 - suspend write         */
                					    /* bit 3 - lock/unlock.          */
                					    /* bit 4 - queued erase.         */
  unsigned      afterSuspend;           /* functions supported after     */
                					    /* suspend (1 = yes, 0 = no):    */
                					    /* bit 0 - write after erase     */
                					    /*         suspend.              */
  int           sectorDefs;		        /* Number of sector definitions  */
  struct {				                /* sector descriptions as found  */
      long      secSize;		        /* in CFI query. 		         */
      int       numSecs;		        /*				                 */
  }secDesc[MAXSECREGS];				    /* assume 4 is enough	         */
  struct {				                /*				                 */
      long      sectorSize;		        /* sector size 		             */
      UINT32    sectorBaseAdrs;	        /* base address of sector	     */
  } secInfo[MAXSECTORS];		        /* per sector info		         */
  UINT32        unlockAddr1;			/* offset for first unlock	     */
  UINT32        unlockAddr2;			/* offset for 2nd unlock	     */
  int           sectorsInCFI;			/* sector count for device	     */
  int           bootBlockSectors;	    /* sectors that makeup boot block*/
  BOOTBLOCK     bootBlockType;		    /* Top, Bottom or None	         */
} CFI;

LOCAL CFI mtdVars[DRIVES];

/* defines */

/* Save the last erase block on each device discovered in the array
 * for NVRAM */
#undef SAVE_NVRAM_REGION

/* disable debugging */
#undef S29GL_CFI_DEBUG

#ifdef S29GL_CFI_DEBUG
#define DEBUG_PRINT		printf
#else
#undef DEBUG_PRINT
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

#define CFI_LONG_WRITE(addr, data)  (*(ULONG *)addr = (ULONG)(CFI_WORDSWAP(data)))
#define CFI_WORD_WRITE(addr, data)  (*(USHORT *)addr = (USHORT)(CFI_BYTESWAP(data)))
#define CFI_WORD_READ(addr)         (CFI_BYTESWAP(*(USHORT *)addr))
#define CFI_LONG_READ(addr)         (CFI_WORDSWAP(*(ULONG *)addr))

/* command set IDs */

#define INTEL_COMMAND_SET      0x0001
#define AMDFUJ_COMMAND_SET     0x0002
#define INTEL_ALT_COMMAND_SET  0x0001
#define AMDFUJ_ALT_COMMAND_SET 0x0004
#define ALT_NOT_SUPPORTED      0x0000


/* CFI identification strings */

#define ID_STR_LENGTH       3
#define QUERY_ID_STR        "QRY"
#define PRIMARY_ID_STR      "PRI"
#define ALTERNATE_ID_STR    "ALT"


/* commands */

#define AMD_SETUP_ERASE		0x80
#define AMD_SETUP_WRITE		0xa0
#define AMD_READ_ID 		0x90
#define AMD_SUSPEND_ERASE	0xb0
#define AMD_SECTOR_ERASE	0x30
#define AMD_RESUME_ERASE	0x30
#define AMD_READ_ARRAY		0xf0

#define AMD_NO_UNLOCK_ADDR  0xffffffffL

#define AMD_UNLOCK_1		0xaa
#define	AMD_UNLOCK_2		0x55

#define AMD_WW_UNLOCK_ADDR1	0x555u	/* Word wide devices */
#define	AMD_WW_UNLOCK_ADDR2	0x2aau  /* Word wide devices */
#define AMD_BW_UNLOCK_ADDR1	0xaaau  /* Byte wide devices */
#define	AMD_BW_UNLOCK_ADDR2	0x555u  /* Byte wide devices */

#define	AMD_D2		4	    /* Toggles when erase suspended */
#define AMD_D5		0x20	/* Set when programming timeout */
#define	AMD_D6		0x40	/* Toggles when programming */


/* optional commands support */

#define CHIP_ERASE_SUPPORT           0x0001
#define SUSPEND_ERASE_SUPPORT        0x0002
#define SUSPEND_WRITE_SUPPORT        0x0004
#define LOCK_SUPPORT                 0x0008
#define QUEUED_ERASE_SUPPORT         0x0010

#define QUERY       0x98

/* supported functions after suspend */

#define WRITE_AFTER_SUSPEND_SUPPORT  0x0001

#define thisCFI   ((CFI *)vol.mtdVars)

#define FLASH_DEF       UINT32
#define FLASH_CAST      (UINT32 *)

/* JEDEC IDs */
#define S29GL128N_FLASH     0x017e /* 128 128KB sectors */
#define S29GL256N_FLASH     0x037e

IMPORT STATUS sysFlashDataPoll
    (
    volatile FLASH_DEF * pFA,   /* programmed address to poll */
    FLASH_DEF value     /* data programmed to poll address */
    );

/******************************************************************************
* s29GlChipCountGet - return the number of chips in this array
*
* Leave the first chip (or set of interleaved chips) in READ_ARRAY
* mode and move forward by chip size (or interleaved chip size) until
* address wrap around is detected. The chip count is updated in
* vol.noOfChips.
*
* NOMANUAL
*
*/

LOCAL FLStatus s29GlChipCountGet
    (
    FLFlash vol
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
	/* Reinstate the NVRAM region */
	flashPtr = (FlashPTR) flMap(vol.socket, vol.noOfChips *
		(vol.chipSize + (vol.erasableBlockSize / vol.interleaving)));
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
	    goto noMoreChips;	/* Not a CFI Flash part */
	    }

	flashPtr[0x55 * thisCFI->multiplier]
		= (UCHAR)(AMD_READ_ARRAY);
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
	cmdBuffer |= AMD_READ_ARRAY << (8 * ix);
    flashPtr[0x55 * thisCFI->multiplier] = cmdBuffer;

    return flOK;
    }

/******************************************************************************
* s29GlWrite - Write a block of bytes to Flash
*
* This routine will be registered as the MTD vol.write routine. Given a pointer
* to a buffer that must be written, the write width determined from the
* identification process is used the stepping stone to interate through the buffer.
*
* Parameters:
*	vol		    : Pointer identifying drive
*  address		: Card address to write to
*  buffer		: Address of data to write
*	length		: Number of bytes to write
*	overwrite	: TRUE if overwriting old Flash contents
*			    FALSE if old contents are known to be erased
*
* Returns:
*	FLStatus	: 0 on success, failed otherwise
*/
LOCAL FLStatus s29GlWrite
    (
    FLFlash vol,
    CardAddress address,
    const void FAR1 *buffer,
    int length,
    FLBoolean overwrite
    )
    {
    /* Set timeout to 5 seconds from now */
    unsigned long writeTimeout = flMsecCounter + 5000;
    int cLength;
    CardAddress cAddr = address;
    FlashPTR base;
    FlashPTR flashPtr;
    UINT32 unlock1 = 0;
    UINT32 unlock2 = 0;
    UINT32 setup_write = 0;
    UINT32 read_array = 0;
    int i;
    CFI_DWORD tmpDWord;
    CFI_WORD  tmpWord;

	
#define bFlashPtr  flashPtr
#define bBuffer ((const unsigned char FAR1 *) buffer)
#define wFlashPtr ((FlashWPTR) flashPtr)
#define wBuffer ((const unsigned short FAR1 *) buffer)
#define dFlashPtr ((FlashDPTR) flashPtr)
#define dBuffer ((const unsigned long FAR1 *) buffer)

    /* Setup the commands first */
    for (i = 0; i < thisCFI->multiplier; i++)
	{
	unlock1 	|= AMD_UNLOCK_1 << (i * 8);
	unlock2 	|= AMD_UNLOCK_2 << (i * 8);
	setup_write	|= AMD_SETUP_WRITE << (i * 8);
	read_array	|= AMD_READ_ARRAY << (i * 8);
	}

    if (flWriteProtected(vol.socket))
	return flWriteProtect;

    i = 0;
    while ((thisCFI->secInfo[i].sectorBaseAdrs <= address) && (i < thisCFI->sectorsInCFI))
	i++;
    i--;

    base = (FlashPTR)
		  vol.map(&vol, thisCFI->secInfo[i].sectorBaseAdrs,
		    vol.interleaving * thisCFI->interleaveWidth);

    flashPtr = (FlashPTR)
		  vol.map(&vol, address,
		    vol.interleaving * thisCFI->interleaveWidth);

    cLength = length;
    if (vol.interleaving * thisCFI->interleaveWidth == 1)
	    {
        while (cLength >= 1)
  	    {
	    *(UCHAR *)(base +
		(thisCFI->unlockAddr1 * thisCFI->multiplier)) = unlock1;
    	    *(UCHAR *)(base + 
		(thisCFI->unlockAddr2 * thisCFI->multiplier)) = unlock2;
    	    *(UCHAR *)(base + 
		(thisCFI->unlockAddr1 * thisCFI->multiplier)) = setup_write;
	    *bFlashPtr = *bBuffer;
	    while (bFlashPtr[0] != bBuffer[0] && flMsecCounter < writeTimeout)
	        {
	        if ((bFlashPtr[0] & AMD_D5) && bFlashPtr[0] != bBuffer[0])
		    {
		    bFlashPtr[0] = read_array;
#ifdef DEBUG_PRINT
		    DEBUG_PRINT("Debug: write failed in AMD MTD.\n");
#endif
		    return flWriteFault;
		    }
	        }
	    cLength--;
	    cAddr++;
        buffer = (unsigned char *) buffer + 1;
	    bFlashPtr++;
	    }
        }
    else if (vol.interleaving * thisCFI->interleaveWidth == 2)
	{
        while (cLength >= 2)
	    {
	    *(USHORT *)(base +
		(thisCFI->unlockAddr1 * thisCFI->multiplier)) = unlock1;
    	    *(USHORT *)(base + 
		(thisCFI->unlockAddr2 * thisCFI->multiplier)) = unlock2;
    	    *(USHORT *)(base + 
		(thisCFI->unlockAddr1 * thisCFI->multiplier)) = setup_write;
	    *wFlashPtr = *wBuffer;
	    while ((wFlashPtr[0] != wBuffer[0]) &&
				    (flMsecCounter < writeTimeout))
		{
		if (((wFlashPtr[0] & AMD_D5) &&
		    ((wFlashPtr[0] ^ wBuffer[0]) &   0xff)) ||
		    ((wFlashPtr[0] & (AMD_D5 * 0x100))
		     && ((wFlashPtr[0] ^ wBuffer[0]) & 0xff00)))
		    {
		    wFlashPtr[0] = read_array;
#ifdef DEBUG_PRINT
	            DEBUG_PRINT("Debug: write failed in AMD MTD.\n");
#endif
		    return flWriteFault;
		    }
		}
	    cLength -= 2;
	    cAddr += 2;
        buffer = (unsigned char *)buffer + sizeof(short);
	    flashPtr += 2;
	    }
        if (cLength > 0)
        {
	    /* copy data from flash to tmpWord */
	    tmpWord.ushort = wFlashPtr[0];

	    /* now fill in the left over byte */
	    tmpWord.uchar[0] = *(char *)buffer;

	    *(USHORT *)(base +
		(thisCFI->unlockAddr1 * thisCFI->multiplier)) = unlock1;
    	    *(USHORT *)(base + 
		(thisCFI->unlockAddr2 * thisCFI->multiplier)) = unlock2;
    	    *(USHORT *)(base + 
		(thisCFI->unlockAddr1 * thisCFI->multiplier)) = setup_write;
	    *wFlashPtr = tmpWord.ushort;

	    while ((wFlashPtr[0] != tmpWord.ushort) &&
				    (flMsecCounter < writeTimeout))
		{
		if (((wFlashPtr[0] & AMD_D5) &&
		    ((wFlashPtr[0] ^ tmpWord.ushort) &   0xff)) ||
		    ((wFlashPtr[0] & (AMD_D5 * 0x100))
		     && ((wFlashPtr[0] ^ tmpWord.ushort) & 0xff00)))
		    {
		    wFlashPtr[0] = read_array;
#ifdef DEBUG_PRINT
            DEBUG_PRINT("Debug: write failed in AMD MTD.\n");
#endif
		    return flWriteFault;
		    }
		}
	    cAddr += cLength;
        buffer =  (unsigned char *) buffer + cLength;
	    flashPtr += cLength;
	    cLength = 0;
	    }
	}
	else /* if (vol.interleaving >= 4) */ 
	{
	while (cLength >= 4) 
	    {
	    *(UINT32 *)(base + 
		(thisCFI->unlockAddr1 * thisCFI->multiplier)) = unlock1;
    	    *(UINT32 *)(base + 
		(thisCFI->unlockAddr2 * thisCFI->multiplier)) = unlock2;
    	    *(UINT32 *)(base + 
		(thisCFI->unlockAddr1 * thisCFI->multiplier)) = setup_write;
	    *dFlashPtr = *dBuffer;

	    while ((dFlashPtr[0] != dBuffer[0]) && 
			    (flMsecCounter < writeTimeout)) 
		{
		if (thisCFI-> interleaveWidth == 1)
		    {
		    /* We have 4 8 bit devices */
		    if (((dFlashPtr[0] & AMD_D5) && 
			((dFlashPtr[0] ^ dBuffer[0]) & 0xff)) ||
			((dFlashPtr[0] & (AMD_D5 * 0x100)) && 
			((dFlashPtr[0] ^ dBuffer[0]) & 0xff00)) ||
			((dFlashPtr[0] & (AMD_D5 * 0x10000lu)) && 
			((dFlashPtr[0] ^ dBuffer[0]) & 0xff0000lu)) ||
			((dFlashPtr[0] & (AMD_D5 * 0x1000000lu)) && 
			((dFlashPtr[0] ^ dBuffer[0]) & 0xff000000lu))) 
		        {
#ifdef DEBUG_PRINT
	  DEBUG_PRINT("Debug: write failed in AMD MTD.\n");
#endif
		        dFlashPtr[0] = read_array;
		        return flWriteFault;
		        }
		    }
		else
		    {
		    /* Here we assume that the only other option is 2 16 bit devices */
		    if (((dFlashPtr[0] & AMD_D5) && 
			((dFlashPtr[0] ^ dBuffer[0]) & 0x0000ffff)) ||
		        ((dFlashPtr[0] & (AMD_D5 * 0x10000lu)) && 
			((dFlashPtr[0] ^ dBuffer[0]) & 0xffff0000)))
			{
#ifdef DEBUG_PRINT
	  DEBUG_PRINT("Debug4-1: write failed in AMD MTD.\n");
#endif
		        dFlashPtr[0] = read_array;
		        return flWriteFault;
		        }
		    }		    
		
		}
	    cLength -= 4;
	    cAddr += 4;
            buffer = (unsigned char *)buffer + sizeof(long);
	    flashPtr += 4;
	    }
	if (cLength > 0)
            {

	    tmpDWord.uint32 = 0xffffffff;
	    /* read trailing bytes in to temp dword */
	    for (i = 0; i < cLength; i++)
		tmpDWord.uchar[i] = *((char *)buffer + i);

	    /* fill the rest from flash */
	    for (    ; i < 4; i++)
		tmpDWord.uchar[i] = *(flashPtr + i);

	    *(UINT32 *)(base + 
		(thisCFI->unlockAddr1 * thisCFI->multiplier)) = unlock1;
    	    *(UINT32 *)(base + 
		(thisCFI->unlockAddr2 * thisCFI->multiplier)) = unlock2;
    	    *(UINT32 *)(base + 
		(thisCFI->unlockAddr1 * thisCFI->multiplier)) = setup_write;
	    *dFlashPtr = tmpDWord.uint32;

	    while ((dFlashPtr[0] != tmpDWord.uint32) && 
			    (flMsecCounter < writeTimeout)) 
		{
		if (thisCFI-> interleaveWidth == 1)
		    {
		    /* We have 4 8 bit devices */
		    if (((dFlashPtr[0] & AMD_D5) && 
			((dFlashPtr[0] ^ tmpDWord.uint32) & 0xff)) ||
			((dFlashPtr[0] & (AMD_D5 * 0x100)) && 
			((dFlashPtr[0] ^ tmpDWord.uint32) & 0xff00)) ||
			((dFlashPtr[0] & (AMD_D5 * 0x10000lu)) && 
			((dFlashPtr[0] ^ tmpDWord.uint32) & 0xff0000lu)) ||
			((dFlashPtr[0] & (AMD_D5 * 0x1000000lu)) && 
			((dFlashPtr[0] ^ tmpDWord.uint32) & 0xff000000lu))) 
		        {
#ifdef DEBUG_PRINT
	  DEBUG_PRINT("Debug: write failed in AMD MTD.\n");
#endif
		        dFlashPtr[0] = read_array;
		        return flWriteFault;
		        }
		    }
		else
		    {
		    /* Here we assume that the only other option is 16 devices */
		    if (((dFlashPtr[0] & AMD_D5) && 
			((dFlashPtr[0] ^ tmpDWord.uint32) & 0x0000ffff)) ||
		        ((dFlashPtr[0] & (AMD_D5 * 0x10000lu)) && 
			((dFlashPtr[0] ^ tmpDWord.uint32) & 0xffff0000)))
			{
#ifdef DEBUG_PRINT
	  DEBUG_PRINT("Debug4-2: write failed in AMD MTD.\n");
#endif
		        dFlashPtr[0] = read_array;
		        return flWriteFault;
		        }
		    }		    
		}

	    cAddr += cLength;
            buffer =  (unsigned char *) buffer + cLength;
	    flashPtr += cLength;
	    cLength = 0;
	    }
	    /* cfiAmdLeftoverBytesWrite(&vol, buffer, flashPtr, cLength); */
	}

    flashPtr -= length;
    buffer = (unsigned char *)buffer - length; /* bBuffer -= length; */

    if (tffscmp((void FAR0 *) flashPtr,buffer,length))
	{
#ifdef DEBUG_PRINT
	DEBUG_PRINT("Debug: write failed in AMD MTD on verification.\n");
#endif
	return flWriteFault;
	}

    return flOK;
    }

/*******************************************************************************
* s29GlErase - Erase one or more contiguous Flash erasable blocks
*
* This routine will be registered as the MTD vol.erase routine. Sector zero is
* assumed to be the boot block anchor of the boot block on bottom boot devices,
* while the first sector of size not equal to vol.erasableBlockSize is assumed
* to be the anchor for top boot devices.
*
* Parameters:
*	vol		: Pointer identifying drive
*      firstErasableBlock : Number of first block to erase
*	numOfErasableBlocks: Number of blocks to erase
*
* Returns:
*	FLStatus	: 0 on success, failed otherwise
*/

LOCAL  FLStatus s29GlErase
    (
    FLFlash vol,
    int firstErasableBlock,
    int numOfErasableBlocks
    )
    {
    int i;
    UINT32 unlock1 = 0;
    UINT32 unlock2 = 0;
    UINT32 erase_setup = 0;
    UINT32 erase_sector = 0;
    int thisSector = firstErasableBlock;
    int sectorsToErase = numOfErasableBlocks;
    UINT32 * unlockAddr1;
    UINT32 * unlockAddr2;
	STATUS retVal = OK;
	 
    if (flWriteProtected(vol.socket))
	return flWriteProtect;

	/* Setup the commands first */
    for (i = 0; i < thisCFI->multiplier; i++)
	{
	unlock1 	 |= AMD_UNLOCK_1 << (i * 8);
	unlock2 	 |= AMD_UNLOCK_2 << (i * 8);
	erase_setup	 |= AMD_SETUP_ERASE << (i * 8);
	erase_sector |= AMD_SECTOR_ERASE << (i * 8);
	}

    for ( ; thisSector < firstErasableBlock + sectorsToErase; thisSector++)
	{


	FlashPTR flashPtr;


	/* we know we are on a unit boundary so mapBase is not necessary */
	flashPtr = (FlashPTR)
		  vol.map(&vol, thisCFI->secInfo[thisSector].sectorBaseAdrs, 0);

	unlockAddr1 = (UINT32 *)((long)flashPtr + (thisCFI->unlockAddr1 *thisCFI->multiplier));
	unlockAddr2 = (UINT32 *)((long)flashPtr + (thisCFI->unlockAddr2 *thisCFI->multiplier));
	
	*unlockAddr1 = unlock1;
	*unlockAddr2 = unlock2;
	*unlockAddr1 = erase_setup;
	*unlockAddr1 = unlock1;
	*unlockAddr2 = unlock2;
	*(UINT32 *)flashPtr = erase_sector;
    do
    {
    retVal = sysFlashDataPoll ((volatile FLASH_DEF *)flashPtr,  (FLASH_DEF)0xffffffff);
    } while ((*(UINT32 *)flashPtr != (UINT32)0xffffffff) && (retVal == OK));
	if (retVal != OK)
	{
		#ifdef DEBUG_PRINT
			DEBUG_PRINT("Clearing sector %d\n", thisSector);
		#endif
		return flWriteFault;
	}
	}
    return flOK;
    }


/***************************************************************************
 * s29GlIdentify - Identification routine for devices conforming to CFI/SC
 *
 * Identifies media based on CFI and the AMD/Fujitsu command set and registers
 * as an MTD for such. This routine should be placed on the MTD list in flcustom.h.
 * It must be an extern routine. On successful identification, the Flash
 * structure is filled out and the write and erase routines registered.
 *
 * Parameters:
 *      vol             : Pointer identifying drive
 *
 * Returns:
 *      FLStatus        : 0 on positive identificaion, failed otherwise
 *
 * http://www.amd.com/us-en/FlashMemory/ProductInformation/0,,37_1447_1451_1780%5E1834%5E1955,00.html
 *
 *
 * NOMANUAL
 *
 */
FLStatus s29GlIdentify
    (
    FLFlash vol
    )
    {
    FlashPTR flashPtr = (FlashPTR) flMap(vol.socket, 0);
    FlashWPTR flashWPtr = (FlashWPTR) flashPtr;
    FlashDPTR flashDPtr = (FlashDPTR) flashPtr;
    UINT32 unlock1 = 0;
    UINT32 unlock2 = 0;
    UINT32 readID = 0;
    UINT32 readArray = 0;

    int i;

    unsigned primaryTable, secondaryTable;
    FLBoolean eightBitMode = FALSE;
    int thisSector = 0;		/* running count of sectors for this CFI */
    CardAddress sectorBaseAdrs = 0;	/* base address of this sector */
    int ix = 0;
	
#ifdef DEBUG_PRINT
    DEBUG_PRINT("Debug: Entering S29GL identification routine %d.\n");
#endif

    vol.mtdVars = &mtdVars[flSocketNoOf(vol.socket)];
    thisCFI->bootBlockSectors = 0;
    flashWPtr[0x55] = (USHORT) ((AMD_READ_ARRAY << 8) | AMD_READ_ARRAY); 
    flashDPtr[0x55] = (ULONG) (QUERY << 24| QUERY <<16 | QUERY << 8 | QUERY); 

    /* A 32 bit device in 8 bit mode and two 16 bit deivec in 8 bit mode will
     * appear the same to the querry process. The only way to make the
     * distinction is to try and set one device back to read array mode while
     * the other remains in query mode */
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
	flashPtr[0x154] = AMD_READ_ARRAY;
	if ((flashPtr[0x40] != 0x51) && (flashPtr[0x41] == 0x51))
	    {	/* Turned one device off successfully */
	    flashPtr[0x154] = QUERY; 	/* turn it back on to querry mode */
	    vol.interleaving = 2;
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
	    }
       	thisCFI->multiplier = 4;
	thisCFI->interleaveWidth = 1;
	eightBitMode = TRUE;
	goto getCFI;
	}

    /* Is it a single 32 bit device */
    if (flashDPtr[0x10] == 'Q' &&
        flashDPtr[0x11] == 'R' &&
        flashDPtr[0x12] == 'Y')
        {
#ifdef DEBUG_PRINT
        DEBUG_PRINT ("Debug: detected a 32 bit device in 32 bit mode\n");
#endif
	thisCFI->multiplier = 4;
	thisCFI->interleaveWidth = 1;
	vol.interleaving = 1;
	goto getCFI;
	}

    /* Two 16 bit devices in 16 bit mode */
    if (flashWPtr[0x20] == 'Q' &&
        flashWPtr[0x21] == 'Q' &&
        flashWPtr[0x22] == 'R' &&
        flashWPtr[0x23] == 'R' &&
        flashWPtr[0x24] == 'Y' &&
        flashWPtr[0x25] == 'Y')
        {
#ifdef DEBUG_PRINT
        DEBUG_PRINT ("Debug: Detected two 16 bit devices in 16 bit mode(3)\n");
#endif
	thisCFI->multiplier = 4;
	thisCFI->interleaveWidth = 2;
	vol.interleaving = 2;
	goto getCFI;
	}

    /* If we get here the device is probably not CFI */

#ifdef DEBUG_PRINT
    DEBUG_PRINT ("Debug: Failed to identify CFI... \n Exiting \n");
#endif

    flashDPtr[0x55] = (ULONG) (AMD_READ_ARRAY << 24 | AMD_READ_ARRAY << 16
			     | AMD_READ_ARRAY << 8 | AMD_READ_ARRAY);

    return flUnknownMedia;

getCFI:

    if (!eightBitMode)
        {
        thisCFI->wordMode = TRUE; /* limitting ourselves to 8 an 16 bit devs */
        thisCFI->unlockAddr1 = AMD_WW_UNLOCK_ADDR1;
        thisCFI->unlockAddr2 = AMD_WW_UNLOCK_ADDR2;
#ifdef DEBUG_PRINT
	DEBUG_PRINT("WORD MODE !!\n");
#endif
	}
    else
    	{
    	thisCFI->wordMode = FALSE;
    	thisCFI->unlockAddr1 = AMD_BW_UNLOCK_ADDR1;
    	thisCFI->unlockAddr2 = AMD_BW_UNLOCK_ADDR2;
#ifdef DEBUG_PRINT
	DEBUG_PRINT("BYTE MODE !!\n");
#endif
	}

    /* check the command set ID */
    /* NOTE: Not swapped */
	
	thisCFI->commandSetId = *(USHORT *)(&flashPtr[0x13 * thisCFI->multiplier]);
	
#ifdef DEBUG_PRINT
    DEBUG_PRINT ("Debug: Commandset ID is 0x%x\n", thisCFI->commandSetId);
#endif

    /* Only support the AMD/Fujitsu Command set */
    if ( thisCFI->commandSetId != AMDFUJ_COMMAND_SET)
        {
#ifdef DEBUG_PRINT
        DEBUG_PRINT("Debug: did not recognize command set.\n");
#endif
        return flUnknownMedia;
        }

    /* get address for primary algorithm extended table. */
    primaryTable = CFI_WORD_READ(&flashPtr[0x15 * thisCFI->multiplier]);

    /* check alternate command set ID. */
    /* NOTE: not swapped */
    thisCFI->altCommandSetId = *(USHORT *)
				(&flashPtr[0x17 * thisCFI->multiplier]);
    if (thisCFI->altCommandSetId != AMDFUJ_ALT_COMMAND_SET &&
        thisCFI->altCommandSetId != ALT_NOT_SUPPORTED)
        {
#ifdef DEBUG_PRINT
        DEBUG_PRINT("Debug: did not recognize alternate command set.\n");
#endif
        return flUnknownMedia;
        }

    /* get address for secondary algorithm extended table. */
    secondaryTable = CFI_WORD_READ(&flashPtr[0x19 * thisCFI->multiplier]);

    thisCFI->vpp = flashPtr[0x1d * thisCFI->multiplier];

    /*Get the number of erase block descriptions for thei CFI*/
    if (eightBitMode)
	thisCFI->sectorDefs = (int) (*(UCHAR *)
				(&flashPtr[0x2c * thisCFI->multiplier]));
    else
	thisCFI->sectorDefs = (int) (*(USHORT *)
				    (&flashPtr[0x2c * thisCFI->multiplier]));

    /* We should bail out if this is greater than 8 */
#ifdef DEBUG_PRINT
    DEBUG_PRINT ("Debug: Number of erase block descriptions is 0x%x\n",
					    thisCFI->sectorDefs);
#endif
    if (thisCFI->sectorDefs > MAXSECREGS)
        {
#ifdef DEBUG_PRINT
        DEBUG_PRINT ("Debug: Number of erase block descriptions overrun!\n");
#endif
        return flUnknownMedia;
        }

    vol.erasableBlockSize = 0;
    thisCFI->sectorsInCFI = 0;

    for ( ix = 0; ix < thisCFI->sectorDefs; ix++)
	{
	thisCFI->secDesc[ix].numSecs = (int) (*(USHORT *)
		(&flashPtr[(0x2d + (ix * 4)) * thisCFI->multiplier]));
	thisCFI->secDesc[ix].numSecs++;
#ifdef DEBUG_PRINT
    DEBUG_PRINT ("Debug: Num sectors in %d - %d\n", ix, thisCFI->secDesc[ix].numSecs);
#endif

	thisCFI->secDesc[ix].secSize = (long) ((*(USHORT *)
		(&flashPtr[(0x2f + (ix * 4)) * thisCFI->multiplier])) +
		(*(USHORT *)(&flashPtr[(0x30 + (ix * 4)) * 
			       thisCFI->multiplier])) * 0x100L);
#ifdef DEBUG_PRINT
    DEBUG_PRINT ("Debug: Sector size is 0x%x\n",
			(UINT32) thisCFI->secDesc[ix].secSize);
#endif

	/* TrueFFS does not support the flexible sector architecture so we
	 * take the largest sector size to be the erase block size.
	 */
	if (vol.erasableBlockSize < thisCFI->secDesc[ix].secSize)
	    vol.erasableBlockSize = thisCFI->secDesc[ix].secSize;

	/* Also track the number of sector on this device */
	thisCFI->sectorsInCFI += thisCFI->secDesc[ix].numSecs;
	}

    if (vol.erasableBlockSize == 0x00)
	vol.erasableBlockSize = 0x80L;
    else
	vol.erasableBlockSize *= 256;

    /* Get Device Size */
    if (eightBitMode)
	vol.chipSize = (1L << flashPtr[0x27 * thisCFI->multiplier]);
    else
	vol.chipSize = (1L << *(USHORT*)(&flashPtr[0x27 * thisCFI->multiplier]));

    vol.erasableBlockSize *= vol.interleaving;

#ifdef SAVE_NVRAM_REGION
    /* Top boot block devices will loose their entire boot block to NVRAM. Since
     * TrueFFS cares not about the subdivisions we reserve an entire erase block
     * on each of the interleaved devices.
     */
    vol.chipSize -= vol.erasableBlockSize;
#endif

#ifdef DEBUG_PRINT
    DEBUG_PRINT("Debug: %d sectors on device\n", thisCFI->sectorsInCFI);
    DEBUG_PRINT("Debug: Erasable block size is 0x%lx\n", vol.erasableBlockSize);
    DEBUG_PRINT("Debug: Chip size is 0x%lx\n", vol.chipSize);
#endif
    /* Since boot blocks happen to be subdevided erase blocks and TrueFFS does
     * not support the sub devision of erase blocks the MTD needs to be able
     * to detect the "sectors" that are sub devisions and erase all of them
     * (assuming that they are contiguous) together. Since the boot block can
     * appear either at the top or the bottom, the only way to keep track of
     * them is to read all sector descriptions and remember them for cross
     * reference. The size of the largest erase block is registered as the
     * eraseBlockSize for the volume.
     */

    /* Setup the commands first */
    for (i = 0; i < thisCFI->multiplier; i++)
	{
	unlock1 	 |= AMD_UNLOCK_1   << (i * 8);
	unlock2 	 |= AMD_UNLOCK_2   << (i * 8);
	readID	 	 |= AMD_READ_ID    << (i * 8);
	readArray 	 |= AMD_READ_ARRAY << (i * 8);
	}

    /* Get JEDEC ID so we know if this is a top or bottom boot device */

    *(UINT32 *)(flashPtr + (thisCFI->unlockAddr1 * thisCFI->multiplier)) = unlock1;
    *(UINT32 *)(flashPtr + (thisCFI->unlockAddr2 * thisCFI->multiplier)) = unlock2;
    *(UINT32 *)(flashPtr + (thisCFI->unlockAddr1 * thisCFI->multiplier)) = readID;

    if (eightBitMode)
        vol.type = flashPtr[0] << 8 | flashPtr[thisCFI->interleaveWidth];
    else
        vol.type = (*(USHORT *)(&flashPtr[0]) << 8) |
		    (*(USHORT *)(&flashPtr[thisCFI->multiplier]) & 0x00ff);

    /* Determine boot block type */
    if (vol.type == S29GL128N_FLASH || vol.type == S29GL256N_FLASH)
	thisCFI->bootBlockType = BOOTBLOCK_NONE;
    else
	{
	/* Probably an unregistered device that needs to be added here */
#ifdef DEBUG_PRINT
	DEBUG_PRINT ("Unknown device of type 0x%x \n", vol.type);
#endif
	/* set device back to read array */
	*(UINT32 *) flashPtr = AMD_READ_ARRAY;

	return flUnknownMedia;
	}

    /* We now have all of the CFI info that we need. Now lets build up
     * the sector map and register the erase block sizes.
     */

    if (thisCFI->bootBlockType == BOOTBLOCK_NONE)
	{
#ifdef DEBUG_PRINT
	DEBUG_PRINT("Debug: Uniform device, no boot block\n");
#endif

	/* Simplest case is the uniform sector device. Has only one
	 * sector desc.
	 */
	for (; thisSector < thisCFI->secDesc[0].numSecs; thisSector++)
	    {
	    thisCFI->secInfo[thisSector].sectorSize = vol.erasableBlockSize;
	    thisCFI->secInfo[thisSector].sectorBaseAdrs = sectorBaseAdrs;
	    sectorBaseAdrs += (CardAddress) vol.erasableBlockSize;
	    }
	}
    else if (thisCFI->bootBlockType == BOOTBLOCK_BOTTOM)
	{
#ifdef DEBUG_PRINT
	DEBUG_PRINT("Debug: Bottom boot block device\n");
#endif
	/* This is a hack but when looking at the data sheets for all of
	 * these devices what comes across is that the CFI data always 
	 * describes a botom boot device. There for we read the sector 
	 * info in the exact order that it is provided in.
	 */
    	for ( ix = 0; ix < thisCFI->sectorDefs; ix++)
	    {
	    int iy;

            for (iy = 0; iy < thisCFI->secDesc[ix].numSecs; iy++, thisSector++)
                {
	    	if (thisCFI->secDesc[ix].secSize != 0x00)
	            thisCFI->secInfo[thisSector].sectorSize = 
				    thisCFI->secDesc[ix].secSize * 0x100L;
	    	else
	            thisCFI->secInfo[thisSector].sectorSize = 0x80L;
		
		/* count boot blocks */
		if (thisCFI->secInfo[thisSector].sectorSize != vol.erasableBlockSize)
		    thisCFI->bootBlockSectors++;
		
	    	thisCFI->secInfo[thisSector].sectorBaseAdrs = sectorBaseAdrs;
	    	sectorBaseAdrs += (CardAddress) thisCFI->secInfo[thisSector].sectorSize;
	    	}
	    }
	}
    else	/* Top boot device */
	{
	int secCount;
#ifdef DEBUG_PRINT
	DEBUG_PRINT("Debug: Top boot block device\n");
#endif
	/* First load the descriptions of the uniform sectors and 
	 * then add the sub divisions 
	 */
    	for ( ix = 0; ix < thisCFI->sectorDefs; ix++)
	    {
	    int iy;
	    /* We assume that the unform sectors will never be of the 
	     * variety 128 bytes 
	     */
	    if (vol.erasableBlockSize == thisCFI->secDesc[ix].secSize * 0x100L * vol.interleaving)
		{
            	for (iy = 0; iy < thisCFI->secDesc[ix].numSecs; 
							iy++, thisSector++)
                    {
		    thisCFI->secInfo[thisSector].sectorSize = 
							vol.erasableBlockSize;
		    thisCFI->secInfo[thisSector].sectorBaseAdrs = 
							sectorBaseAdrs;
	    	    sectorBaseAdrs += (CardAddress) 
		    		thisCFI->secInfo[thisSector].sectorSize;
		    }
		break;
	    	}
	    }
	for ( ix = 0, secCount = 0; 
		thisCFI->secDesc[ix].secSize * 0x100L * vol.interleaving
			 != vol.erasableBlockSize; 
		ix++)
	    {
	    int iy;

	    for (iy = 0; iy < thisCFI->secDesc[ix].numSecs; iy++, secCount++)
		{
	    	thisCFI->secInfo[(thisCFI->sectorsInCFI - 1) - secCount].sectorSize = 
						thisCFI->secDesc[ix].secSize * 0x100L * vol.interleaving;
		}
	    }

	for (; thisSector <= (thisCFI->sectorsInCFI -1); thisSector++)
	    {
	    thisCFI->secInfo[thisSector].sectorBaseAdrs = sectorBaseAdrs;
    	    sectorBaseAdrs += (CardAddress) thisCFI->secInfo[thisSector].sectorSize;
	    thisCFI->bootBlockSectors++;
	    }
	}
#ifdef DEBUG_PRINT
    DEBUG_PRINT("Debug: Number of boot block sectors is %d\n", thisCFI->bootBlockSectors);
#endif

    /* Make sure this comes at the end since the device is set to READ_ARRAY
     * mode prior to the return
     */
    if (s29GlChipCountGet(&vol) != flOK)
	return flUnknownMedia;
#ifdef DEBUG_PRINT
    DEBUG_PRINT ("Debug: No of chips detected is %d\n", vol.noOfChips);
#if 0
    for (ix = 0; ix < thisCFI->sectorsInCFI; ix++)
	DEBUG_PRINT ("%2d:0x%x	---  0x%lx\n", ix,
			thisCFI->secInfo[ix].sectorBaseAdrs,
			thisCFI->secInfo[ix].sectorSize);
#endif
#endif

    vol.erase = s29GlErase;
    vol.write = s29GlWrite;

    /* Might have to do this on a bus width basis but for now it seems to
     * work with Intel devices.
     */
    flashPtr[0] = AMD_READ_ARRAY;
    flashPtr[1] = AMD_READ_ARRAY;
    flashPtr[2] = AMD_READ_ARRAY;
    flashPtr[3] = AMD_READ_ARRAY;

    return flOK;
    }
