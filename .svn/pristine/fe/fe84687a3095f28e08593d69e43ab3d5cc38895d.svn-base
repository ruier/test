/* sysAmdS29GLxxxN.c - driver for AMD S29GLxxxN Mirrorbit FLASH Devices */

/*
 * Copyright (c) 2005, 2008, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2005 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01c,28feb12,mpc  Add sync in flashWrite32Bits/flashRead32Bits. (WIND00335163)
01b,28sep08,y_w  Add delay into flashIdGet to get correct devCode
01a,27aug05,cak  New File.
*/

/*
DESCRIPTION

This is a driver for the AMD S29GL512/256/128N MirrorBit FLASH devices.
Routines are provided to erase and write to the FLASH devices.  Note that
this driver only supports the FLASH configured as 16-bit devices.  It is
also assumed that there are two 16-bit FLASH devices to create a 32-bit
"device", and as such all accesses are 32-bit.

The following routines are provided:

BOOL flashNVSectorProtectPPBIsSet (UINT32 flashBaseAddr, UINT32 sectorSize,
                   int numSectors)

This routine will determine if any of the PPB protection bits are
set in any sector on the device/s.

void flashNVSectorProtectPPBErase (UINT32 flashBaseAddr)

This routine will erase the PPB bits in each sector of the FLASH device.
The PPB bits are the Non-volatile sector protection bits, and it is not
possible to clear only selected bits, they must all be cleared
using the "All PPB Erase" command.

void flashNVSectorProtectPPBProgram (UINT32 flashBaseAddr, UINT32 sectorSize,
                     int numSectors)

This routine will program the PPB bits in each sector of the FLASH device.
This is used as a method of locking and protecting the device.

void flashIdGet (UINT32 flashBaseAddr, UINT32* manCode, UINT32* devCode,
         UINT32* secDevCode)

This routine will obtain the manufacturer and device IDs of the flash
device.

void flashInfoGet (UINT32 flashBaseAddr, UINT32 * sectorSize, int * numSectors)

This routine will determine the size of each sector and the total number
of sectors in the device.

STATUS flashWordProgram (UINT32 flashBaseAddr, UINT32 addr, UINT32 data)

This routine will program the given word (32 bits) to the specified
address.

STATUS flashSectorProgram (UINT32 flashBaseAddr, int sectorNum,
               UINT32 * pData, UINT32 sectorSize,
               int numSectors)

This routine will program the given data to the specified sector.  It is
assumed that the sector has been previously erased.  It is also assumed
that the data buffer is one sector in length.  The sector is specified
by a sector number.

STATUS flashUtilSectorProgram (UINT32 flashBaseAddr, UINT32 sectorAddr,
                       char * pData, int length, UINT32 sectorSize,
                   int numSectors)

This routine will program the given data to the specified sector.  It is
assumed that the sector has been previously erased. The sector is specified
by its start address.

STATUS flashSectorErase (UINT32 flashBaseAddr, int sectorNum,
             UINT32 sectorSize, int numSectors)

This routine will erase the specified sector.  The sector is specified by
its sector number.

STATUS flashUtilSectorErase (UINT32 flashBaseAddr, UINT32 eraseAddr,
                 UINT32 sectorSize, int numSectors)

This routine will erase the specified sector.  The sector is specified by
its start address.

*/

/* includes */

#include "amdS29GLxxxN.h"

/* defines */

#undef  FLASH_DEBUG
#undef  FLASH_DEBUG_ERROR

/* typedefs */

/* globals */

/* locals */

/* forward declarations */

LOCAL void flashWrite32Bits (UINT32 addr, UINT32 data);
LOCAL UINT32 flashRead32Bits (UINT32 addr);
LOCAL void flashUnlock (UINT32 flashBaseAddr);
LOCAL void flashReset (UINT32 flashBaseAddr);

/* externals */

IMPORT void sysUsDelay (UINT32 delay);

/*******************************************************************************
*
* flashWrite32Bits - Write 32 bits to device at specified address.
*
* This routine will write 32 bits to the flash device/s at the specified
* address.
*
* RETURNS: N/A
*/

LOCAL void flashWrite32Bits
    (
    UINT32 addr,    /* device address */
    UINT32 data     /* data to be written */
    )
    {

    /* Write */

    *((volatile UINT32*) addr) = data;
    _WRS_ASM ("sync");
    }

/*******************************************************************************
*
* flashRead32Bits - Read 32 bits from device at specified address.
*
* This routine will read 32 bits from the flash device/s at the specified
* address.
*
* RETURNS: data at specified address
*/

LOCAL UINT32 flashRead32Bits
    (
    UINT32 addr     /* address to read from */
    )
    {
    UINT32 data;

    data = *((volatile UINT32*) addr);
    _WRS_ASM ("sync");
    return(data);
    }

/*******************************************************************************
*
* flashNVSectorProtectPPBIsSet - determine if any PPB bits are set
*
* This routine will determine if any of the PPB protection bits are
* set in any sector on the device/s.
*
* RETURNS: True if any PPB is set, FALSE otherwise
*/

BOOL flashNVSectorProtectPPBIsSet
    (
    UINT32 flashBaseAddr,   /* base address of flash bank */
    UINT32 sectorSize,      /* flash device sector size */
    int numSectors      /* number of sectors on the flash device */
    )
    {
    int x;
    UINT32 addr;
    UINT32 data;
    BOOL isSet = FALSE;

#ifdef FLASH_DEBUG
    logMsg("starting flashNVSectorProtectPPBIsSet...\n\r",0,0,0,0,0,0);
#endif

    /* Non-Volatile Sector Protection Command Set Entry */

    flashUnlock(flashBaseAddr);
    flashWrite32Bits(flashBaseAddr + ADDR_NV_SECT_PROTECT_ENTRY,
             CMD_NVPROTECT_ENTRY);

    /* check to see if PPB is set for any sector */

    addr = flashBaseAddr;
    for (x = 0; x < numSectors; x++)
    {
    data = flashRead32Bits(addr);

    if (!data)
        {
        isSet = TRUE;
#ifdef FLASH_DEBUG
        logMsg("sector %d,0x%x = 0x%x\n\r", x, addr, data,0,0,0);
#endif
        break;
        }
    addr += sectorSize; /* advance to next sector */
    }

    /* Non-Volatile Sector Protection Command Set Exit */

    flashWrite32Bits(flashBaseAddr + ADDR_NV_SECT_PROTECT_EXIT,
                     CMD_NVPROTECT_EXIT1);
    flashWrite32Bits(flashBaseAddr + ADDR_NV_SECT_PROTECT_EXIT,
                     CMD_NVPROTECT_EXIT2);

#ifdef FLASH_DEBUG
    logMsg("end flashNVSectorProtectPPBIsSet\n\r",0,0,0,0,0,0);
#endif

    return(isSet);
    }

/*******************************************************************************
*
* flashNVSectorProtectPPBErase - Erase all PPB bits
*
* This routine will erase the PPB bits in each sector of the FLASH device.
* The PPB bits are the Non-volatile sector protection bits, and it is not
* possible to clear only selected bits, they must all be cleared
* using the "All PPB Erase" command.
*
* RETURNS: N/A
*/

void flashNVSectorProtectPPBErase
    (
    UINT32 flashBaseAddr    /* base address of the flash bank */
    )
    {
#ifdef FLASH_DEBUG
    logMsg("starting flashNVSectorProtectPPBErase...\n\r",0,0,0,0,0,0);
#endif

    /* Non-Volatile Sector Protection Command Set Entry */

    flashUnlock(flashBaseAddr);
    flashWrite32Bits(flashBaseAddr + ADDR_NV_SECT_PROTECT_ENTRY,
                     CMD_NVPROTECT_ENTRY);

    /* All PPB Erase Command Sequence */

    flashWrite32Bits(flashBaseAddr + ADDR_NV_SECT_PROTECT_PPB_ERASE,
                     CMD_PPBERASE1);
    flashWrite32Bits(flashBaseAddr + ADDR_NV_SECT_PROTECT_PPB_ERASE,
                     CMD_PPBERASE2);

    /*
     * The following delay is necessary to allow the device time to
     * complete the "All PPB Erase" before issuing the exit command
     * sequence.  Failure to do so will leave the device in the
     * Non-Volatile Sector Protection mode.
     */

    sysUsDelay(100000);

    /* Non-Volatile Sector Protection Command Set Exit */

    flashWrite32Bits(flashBaseAddr + ADDR_NV_SECT_PROTECT_EXIT,
                     CMD_NVPROTECT_EXIT1);
    flashWrite32Bits(flashBaseAddr + ADDR_NV_SECT_PROTECT_EXIT,
                     CMD_NVPROTECT_EXIT2);

#ifdef FLASH_DEBUG
    logMsg("end flashNVSectorProtectPPBErase...\n\r",0,0,0,0,0,0);
#endif
    }

/*******************************************************************************
*
* flashNVSectorProtectPPBProgram - Program all PPB bits
*
* This routine will program the PPB bits in each sector of the FLASH device.
* This is used as a method of locking and protecting the device.
*
* RETURNS: N/A
*/

void flashNVSectorProtectPPBProgram
    (
    UINT32 flashBaseAddr,   /* base address of flash bank */
    UINT32 sectorSize,      /* size of sectors on flash device */
    int    numSectors       /* number of sectors on flash device */
    )
    {
    int x;
    UINT32 addr;

#ifdef FLASH_DEBUG
    logMsg("starting flashNVSectorProtectPPBProgram...\n\r",0,0,0,0,0,0);
#endif

    /* Non-Volatile Sector Protection Command Set Entry */

    flashUnlock(flashBaseAddr);
    flashWrite32Bits(flashBaseAddr + ADDR_NV_SECT_PROTECT_ENTRY,
                     CMD_NVPROTECT_ENTRY);

    flashWrite32Bits(flashBaseAddr + ADDR_NV_SECT_PROTECT_PPB_PRGM,
                     CMD_PPBPROGRAM1);

    /* Set the PPB bit in each sector */

    addr = flashBaseAddr;
    for (x = 0; x < numSectors; x++)
    {
    flashWrite32Bits(addr, CMD_PPBPROGRAM2);
    addr += sectorSize; /* advance to next sector */
    }

    /* Non-Volatile Sector Protection Command Set Exit */

    flashWrite32Bits(flashBaseAddr + ADDR_NV_SECT_PROTECT_EXIT,
                     CMD_NVPROTECT_EXIT1);
    flashWrite32Bits(flashBaseAddr + ADDR_NV_SECT_PROTECT_EXIT,
                     CMD_NVPROTECT_EXIT2);

    sysUsDelay(100);

#ifdef FLASH_DEBUG
    logMsg("end flashNVSectorProtectPPBProgram...\n\r",0,0,0,0,0,0);
#endif
    }

/*******************************************************************************
*
* flashIdGet - Get flash manufacturer and device IDs.
*
* This routine will obtain the manufacturer and device IDs of the flash
* device.
*
* RETURNS: N/A
*/

void flashIdGet
    (
    UINT32 flashBaseAddr,   /* base address of flash bank */
    UINT32* manCode,        /* manufacturer ID */
    UINT32* devCode,        /* primary device ID */
    UINT32* secDevCode      /* secondary device ID */
    )
    {
#ifdef FLASH_DEBUG
    logMsg("starting flashIdGet...\n\r",0,0,0,0,0,0);
#endif

    flashUnlock(flashBaseAddr);
    flashWrite32Bits(flashBaseAddr + ADDR_AUTOSELECT,
                     CMD_AUTOSELECT);
    *manCode = flashRead32Bits(flashBaseAddr +
                               ADDR_MAN_ID);
    *devCode = flashRead32Bits(flashBaseAddr +
                               ADDR_DEV_ID);
    *secDevCode = flashRead32Bits(flashBaseAddr +
                                  ADDR_SEC_DEV_ID);

    /* put the devices back into read mode */

    flashReset(flashBaseAddr);

    sysUsDelay(1000);

#ifdef FLASH_DEBUG
    logMsg("end flashIdGet...\n\r",0,0,0,0,0,0);
#endif
    }

/*******************************************************************************
*
* flashInfoGet - Get flash sector information.
*
* This routine will determine the size of each sector and the total number
* of sectors in the device.
*
* RETURNS: N/A
*/

void flashInfoGet
    (
    UINT32 flashBaseAddr,   /* base address of flash bank */
    UINT32 * sectorSize,    /* sector size of flash device */
    int * numSectors        /* number of sectors on flash device */
    )
    {
    UINT32 manCode;
    UINT32 devCode;
    UINT32 secDevCode;

#ifdef FLASH_DEBUG
    logMsg("starting flashInfoGet...\n\r",0,0,0,0,0,0);
#endif

    flashIdGet(flashBaseAddr, &manCode, &devCode, &secDevCode);

    if (devCode == FMID_AMD_MIRROR_BIT)
        *sectorSize = (2 * AMD_SECTOR_SIZE_16BIT); /* 2 16-bit devices */
    else
        {
	*sectorSize = 0;
	*numSectors = 0;

#ifdef FLASH_DEBUG
        logMsg("invalid flash device code...\n\r",0,0,0,0,0,0);
#endif
	return;
        }

    switch (secDevCode)
    {
    case FMID_AMD_GL128:
        *numSectors = AMD_GL128_NUM_SECTORS;
        break;
    case FMID_AMD_GL256:
        *numSectors = AMD_GL256_NUM_SECTORS;
        break;
    default:    /* and FMID_AMD_GL512 */
        *numSectors = AMD_GL512_NUM_SECTORS;
        break;
    }

#ifdef FLASH_DEBUG
    logMsg("sectorSize = 0x%x\n\r", (UINT32)*sectorSize,0,0,0,0,0);
    logMsg("numSectors = %d\n\r", (int)*numSectors,0,0,0,0,0);
    logMsg("end flashInfoGet...\n\r",0,0,0,0,0,0);
#endif
    }

/*******************************************************************************
*
* flashUnlock - Write unlock sequence.
*
* This routine will perform the unlock sequence that is needed for most
* command sequences.
*
* RETURNS: N/A
*/

LOCAL void flashUnlock
    (
    UINT32 flashBaseAddr    /* base address of flash bank */
    )
    {
    flashWrite32Bits(flashBaseAddr + ADDR_UNLOCK_1,
                     CMD_UNLOCK_DATA_1);
    flashWrite32Bits(flashBaseAddr + ADDR_UNLOCK_2,
                     CMD_UNLOCK_DATA_2);
    }

/*******************************************************************************
*
* flashReset - Write reset sequence.
*
* This routine will perform the reset sequence which is used to put the
* devices back into read mode.
*
* RETURNS: N/A
*/

LOCAL void flashReset
    (
    UINT32 flashBaseAddr    /* base address of flash bank */
    )
    {
    flashWrite32Bits(flashBaseAddr + ADDR_RESET,
                     CMD_READRESET);
    }

/*******************************************************************************
*
* flashWordProgram - Program one word (32 bits).
*
* This routine will program the given word (32 bits) to the specified
* address.
*
* RETURNS: OK if word was successfully programmed, ERROR otherwise.
*/

STATUS flashWordProgram
    (
    UINT32 flashBaseAddr,   /* base address of flash bank */
    UINT32 addr,        /* address to be programmed */
    UINT32 data         /* data to be programmed */
    )
    {
    BOOL programmed = FALSE;
    int timeout = 10000000;

    /* perform "word program" command sequence */

    flashUnlock(flashBaseAddr);
    flashWrite32Bits(flashBaseAddr + ADDR_PROGRAM,
                     CMD_WORDPROGRAM);

    /* write word to device */

    flashWrite32Bits(addr, data);

    /* verify that the word was written */

    for (; timeout > 0; timeout-- )
        {
        if (flashRead32Bits(addr) == data)
            {
            programmed = TRUE;
            break;
            }
        sysUsDelay(10);
        }

    if (!programmed)
        {
#ifdef FLASH_DEBUG_ERROR
        logMsg("flashWordProgram Timeout: 0x%x\n\r",addr,0,0,0,0,0);
#endif
    return(ERROR);
        }
    return(OK);
    }

/*******************************************************************************
*
* flashSectorProgram - Program an entire sector.
*
* This routine will program the given data to the specified sector.  It is
* assumed that the sector has been previously erased.  It is also assumed
* that the data buffer is one sector in length.
*
* RETURNS: OK if sector successfully programmed, ERROR otherwise.
*/

STATUS flashSectorProgram
    (
    UINT32 flashBaseAddr,   /* base address of flash bank */
    int sectorNum,      /* sector to be programmed */
    UINT32 * pData,     /* pointer to data to be programmed */
    UINT32 sectorSize,      /* sector size of flash device */
    int numSectors      /* number of sectors on flash device */
    )
    {
    UINT32 addr;
    int x;

#ifdef FLASH_DEBUG
    logMsg("starting flashSectorProgram...\n\r",0,0,0,0,0,0);
#endif

    addr = (flashBaseAddr + (sectorSize * sectorNum));

#ifdef FLASH_DEBUG
    logMsg("addr = 0x%x\n\r", addr,0,0,0,0,0);
    logMsg("pData = 0x%x\n\r", (UINT32)pData,0,0,0,0,0);
    logMsg("loops = 0x%x\n\r", sectorSize/4,0,0,0,0,0);
#endif

    for (x = 0; x < (int)sectorSize/4; x++)
    {
    if (flashWordProgram(flashBaseAddr,addr,*pData) == ERROR)
        return(ERROR);
    addr += 4;  /* advance device address */
    pData ++;   /* advance data buffer */
    }

#ifdef FLASH_DEBUG
    logMsg("addr = 0x%x\n\r", addr,0,0,0,0,0);
    logMsg("pData = 0x%x\n\r", (UINT32)pData,0,0,0,0,0);
    logMsg("end flashSectorProgram...\n\r",0,0,0,0,0,0);
#endif
    return(OK);
    }

/*******************************************************************************
*
* flashUtilSectorProgram - Program an entire sector.
*
* This routine will program the given data to the specified sector.  It is
* assumed that the sector has been previously erased.
*
* RETURNS: OK if sector successfully programmed, ERROR otherwise.
*/

STATUS flashUtilSectorProgram
    (
    UINT32 flashBaseAddr,   /* base address of flash bank */
    UINT32 sectorAddr,      /* address to be programmed */
    char * pData,       /* pointer to data to be programmed */
    int length,         /* number of bytes to program */
    UINT32 sectorSize,      /* sector size of flash device */
    int numSectors      /* number of sectors on flash device */
    )
    {
    int x;
    UINT32 newSectorAddr = 0;
    int newLength = 0;
    UINT8 * memBuf;
    int alignment;
    UINT32 * pNewData;

#ifdef FLASH_DEBUG
    logMsg("starting flashUtilSectorProgram...\n\r",0,0,0,0,0,0);
#endif

    /* programming address must be on word boundary */

    switch (sectorAddr & 0x3)
        {
        case 0:
            newSectorAddr = sectorAddr;
            newLength = length;
            break;
        case 1:
            newSectorAddr = sectorAddr - 1;
            newLength = length + 1;
            break;
        case 2:
            newSectorAddr = sectorAddr - 2;
            newLength = length + 2;
            break;
        case 3:
            newSectorAddr = sectorAddr - 3;
            newLength = length + 3;
            break;
        }

    /* make length a multiple of 4 */

    switch (newLength % 4)
        {
        case 0:
            break;
        case 1:
            newLength += 3;
            break;
        case 2:
            newLength += 2;
            break;
        case 3:
            newLength += 1;
            break;
        }

#ifdef FLASH_DEBUG
    logMsg("newLength = 0x%x\n\r", newLength,0,0,0,0,0);
#endif

    memBuf = (UINT8 *)cacheDmaMalloc(newLength);

    /* copy existing sector data to memBuf */

    memcpy((void*)memBuf,(const void*)newSectorAddr,newLength);

    alignment = sectorAddr & 0x3;

    /* copy buffer data to memBuf */

    memcpy(&memBuf[alignment],pData,length);

    pNewData = (UINT32 *)memBuf;

#ifdef FLASH_DEBUG
    logMsg("sectorAddr = 0x%x\n\r", sectorAddr,0,0,0,0,0);
    logMsg("pData = 0x%x\n\r", (UINT32)pData,0,0,0,0,0);
    logMsg("newSectorAddr = 0x%x\n\r", newSectorAddr,0,0,0,0,0);
    logMsg("memBuf = 0x%x\n\r", (UINT32)memBuf,0,0,0,0,0);
    logMsg("pNewData = 0x%x\n\r", (UINT32)pNewData,0,0,0,0,0);
    logMsg("loops = 0x%x\n\r", newLength/4,0,0,0,0,0);
#endif

    for (x = 0; x < (int)newLength/4; x++)
    {
    if (flashWordProgram(flashBaseAddr,newSectorAddr,*pNewData) == ERROR)
        return(ERROR);
    newSectorAddr += 4;  /* advance device address */
    pNewData ++;   /* advance data buffer */
    }

#ifdef FLASH_DEBUG
    logMsg("newSectorAddr = 0x%x\n\r", newSectorAddr,0,0,0,0,0);
    logMsg("pNewData = 0x%x\n\r", (UINT32)pNewData,0,0,0,0,0);
#endif

    /* free memory buffer */

    if (memBuf != NULL)
    cacheDmaFree(memBuf);

#ifdef FLASH_DEBUG
    logMsg("end flashUtilSectorProgram...\n\r",0,0,0,0,0,0);
#endif

    return(OK);
    }

/*******************************************************************************
*
* flashSectorErase - Erase specified sector.
*
* This routine will erase the specified sector.
*
* RETURNS: OK if sector successfully erased, ERROR otherwise.
*/

STATUS flashSectorErase
    (
    UINT32 flashBaseAddr,   /* base address of flash bank */
    int sectorNum,          /* sector to be addressed */
    UINT32 sectorSize,      /* sector size of flash device */
    int numSectors          /* number of sectors on flash device */
    )
    {
    BOOL erased = FALSE;
    int timeout = 10000000;
    UINT32 eraseAddr;
    UINT32 data;

#ifdef FLASH_DEBUG
    logMsg("starting flashSectorErase...\n\r",0,0,0,0,0,0);
#endif

    eraseAddr = (flashBaseAddr + (sectorSize * sectorNum));

    /* perform the sector erase command sequence */

    flashUnlock(flashBaseAddr);
    flashWrite32Bits(flashBaseAddr + ADDR_SECTOR_ERASE, CMD_SECTORERASE1);
    flashUnlock(flashBaseAddr);
    flashWrite32Bits(eraseAddr, CMD_SECTORERASE2);

    /* verify that the sector was indeed erased */

    for (; timeout > 0; timeout-- )
        {
    data = flashRead32Bits(eraseAddr);
#ifdef FLASH_DEBUG_ERROR
    logMsg("data = 0x%x\n\r", data,0,0,0,0,0);
#endif
        if (data == 0xffffffff)
            {
            erased = TRUE;
            break;
            }
    sysUsDelay(300);
        }

    if (!erased)
        {
#ifdef FLASH_DEBUG_ERROR
        logMsg("flashSectorErase timeout: %d\n\r",sectorNum,0,0,0,0,0);
#endif
    return(ERROR);
        }

#ifdef FLASH_DEBUG
    logMsg("end flashSectorErase...\n\r",0,0,0,0,0,0);
#endif

    return(OK);
    }

/*******************************************************************************
*
* flashUtilSectorErase - Erase specified sector by address.
*
* This routine will erase the specified sector.
*
* RETURNS: OK if sector successfully erased, ERROR otherwise.
*/

STATUS flashUtilSectorErase
    (
    UINT32 flashBaseAddr,   /* base address of flash bank */
    UINT32 eraseAddr,       /* address of sector to be erased */
    UINT32 sectorSize,      /* sector size of flash device */
    int numSectors      /* number of sectors on flash device */
    )
    {
    BOOL erased = FALSE;
    int timeout = 10000000;
    UINT32 data;

#ifdef FLASH_DEBUG
    logMsg("starting flashUtilSectorErase...\n\r",0,0,0,0,0,0);
#endif

    /* perform the sector erase command sequence */

    flashUnlock(flashBaseAddr);
    flashWrite32Bits(flashBaseAddr + ADDR_SECTOR_ERASE, CMD_SECTORERASE1);
    flashUnlock(flashBaseAddr);
    flashWrite32Bits(eraseAddr, CMD_SECTORERASE2);

    /* verify that the sector was indeed erased */

    for (; timeout > 0; timeout-- )
        {
    data = flashRead32Bits(eraseAddr);
#ifdef FLASH_DEBUG_ERROR
    logMsg("data = 0x%x\n\r", data,0,0,0,0,0);
#endif
        if (data == 0xffffffff)
            {
            erased = TRUE;
            break;
            }
    sysUsDelay(300);
        }

    if (!erased)
        {
#ifdef FLASH_DEBUG_ERROR
        logMsg("flashUtilSectorErase timeout: %d\n\r",numSectors,0,0,0,0,0);
#endif
    return(ERROR);
        }

#ifdef FLASH_DEBUG
    logMsg("end flashUtilSectorErase...\n\r",0,0,0,0,0,0);
#endif

    return(OK);
    }
