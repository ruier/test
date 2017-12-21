/* sysFlash.c - BSP Intel Strataflash flash driver */

/*
 * Copyright (c) 2002, 2004, 2006, 2007, 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01f,14may10,sye  Fix sector locked error when writing. (WIND00117833)
01e,22sep07,y_w  Fix Flash write slow problem. (WIND00076030)
01d,29aug06,jb3  Fix for WIND00056331 TFFS Building
01c,16jul06,???  PPC Dec Timer vxBus complaint
01b,30jan04,nrv  modified to get it to work with mv5500
01a,08mar02,gtf  new
*/

/*
DESCRIPTION

This library provides NVRAM in flash and general Intel
Strataflash driver support. Strataflash driver functions are
implemented as specified in the Strataflash datasheet. The
functions testFlashIntel() and testFlashBufferIntel() at the
end of this file are provided to illustrate driver usage and
also serve as a test driver.

Note that none of the driver functions are protected for 
simultaneous usage (e.g. with a semaphore) so if this is
intended by the user wrapper functions should be implemented.

Two types of flash write routines are provided one that uses
buffer mode programming versus one that does not. When
programming large buffers it is suggested that the buffer mode
write routine be used.

A comment on the flash functions. Two auxiliary routines,
sysMsrMaskBits() and sysMsrSetBits() were required in order
to turn off EE and relocate IP back to 0x0000000 when any
flash command is given to the flash. The reason for this is
the exception vectors are in flash by default (and jump to
SDRAM locations) so that SDRAM can support two vxWorks images,
one for processor A and one for processor B. When a flash
command is given, any code or data in the flash devices are
no longer readable. Since the exception vectors and boot
code are in the same physical device as the general purpose
flash at 0x7c000000, this approach is necessary.

Finally, the flash write routines assume that the input data
is 32 bit aligned and that a multiple of 32 bits are written.
This means that any functions calling the flash write functions
are required to provide programming data to the flash 
write routines with these restrictions. See sysFlashSet() below
for an example of how this is done.

*/

#include <vxWorks.h>
#include "config.h"
#include "sysFlash.h"
#include <cacheLib.h>
#include <drv/timer/ppcDecTimer.h>

#include <logLib.h>
#include <string.h>
#include <arch/ppc/vxPpcLib.h>
#include <tickLib.h>
#include <stdio.h>

IMPORT void sysUsDelay ( UINT32 delay );

/* NOR FLASH DRIVER FUNCTIONS */

#if FALSE /* This sysFlashGet/sysFlashSet can be used for NVRAM in flash. */
/******************************************************************************
*
* sysFlashGet - return data from the simulated NVRAM in Flash.
* This is a glue function to the nvRamToFlash.c driver.
*
* RETURNS: OK
*
*/

STATUS  sysFlashGet 
    (
    char *string, 
    int strLen, 
    int offset
    )
    {
    bcopy((const char *)(NV_RAM_BASE_ADRS+offset),(char *)string,strLen);
    return OK ;
    }


/******************************************************************************
*
* sysFlashSet - Sets the simulated NVRAM location in flash with the requested data.
* This is a glue function to the nvRamToFlash.c driver. Note that it is not
* necessary to manage the data across sector boundaries as 1 sector of the 29F040
* device is reserved to NVRAM.
*
* RETURNS: OK, or ERROR if unable to program flash.
*
*/

UINT8 progBuf[NV_RAM_SIZE] ;                                         
STATUS  sysFlashSet 
    (
    char *string, 
    int strLen, 
    int offset
    )
    {
    if ( strLen <= 0 )
        {
        logMsg("sysFlashSet: strLen <= 0!\n",1,2,3,4,5,6);
        return ERROR ;
        }
    if ( offset < 0 )
        {
        logMsg("sysFlashSet: offset < 0!\n",1,2,3,4,5,6);
        return ERROR ;
        }

    /* Need a shared memory semaphore here for both sides in case
       they try to write nvram at the same time.
       Use the sysBusTas() used for the pci shared memory backplane.
       Reading at the same time should not be a problem. */

    /* check to see if string is already in flash.. */
    if ( !bcmp((char *)(NV_RAM_BASE_ADRS+offset),(char*)string,strLen) )
        return OK ;

    /* Save current sector contents */
    bcopy((const char *)NV_RAM_BASE_ADRS,(char *)progBuf,sizeof(progBuf));

    /* "Program" the temp buffer */
    bcopy((const char *)string,(char*)(progBuf+offset),strLen) ;

    /* Erase sector contents */
    if ( sysFlashSectorEraseIntel((char *)NV_RAM_BASE_ADRS) != OK )
        {
        logMsg("sysFlashSet: sysFlashSectorErase failure!\n",1,2,3,4,5,6);
        return ERROR ;
        }
    if ( sysFlashSectorWriteIntel((char *)(NV_RAM_BASE_ADRS),(char*)progBuf,sizeof(progBuf)) != OK )
        {
        logMsg("sysFlashSet: sysFlashSectorWrite failure!\n",1,2,3,4,5,6);
        return ERROR ;
        }
    if ( bcmp((char *)NV_RAM_BASE_ADRS,(char*)progBuf,sizeof(progBuf)) )
        {
        logMsg("sysFlashSet: NVRAM not programmed correctly!\n",1,2,3,4,5,6);
        return ERROR ;
        }

    return OK ;
    }
#endif


/******************************************************************************
*
* sysFlashDelay - introduce a small delay.
*
* RETURNS: N/A 
*
*/

void sysFlashDelay(void)
    {

    /* need at least a 210 nsec delay..., @ 400MHz or 2.5nsec per cycle*/

    sysUsDelay(1);
    }

/******************************************************************************
*
* sysFlashReadIdIntel - Read mfgr id and device id from the Strataflash.
*
* RETURNS: OK, or ERROR 
*
*/
LOCAL UINT32 i28f128found = 0 ;
LOCAL UINT32 i28f640found = 0 ;
LOCAL UINT32 i28f320found = 0 ;

/* chips are hooked up x16 on mv5500, and sectors are 128K */

LOCAL UINT32 i28f128SectorSize = INTEL28F128_SECTOR_SZ ;
LOCAL UINT32 i28f640SectorSize = INTEL28F640_SECTOR_SZ ;
LOCAL UINT32 i28f320SectorSize = INTEL28F320_SECTOR_SZ ;

LOCAL UINT32 i28f128NumSectors = INTEL28F128_NUM_SECTORS ;
LOCAL UINT32 i28f640NumSectors = INTEL28F640_NUM_SECTORS ;
LOCAL UINT32 i28f320NumSectors = INTEL28F320_NUM_SECTORS ;

LOCAL UINT32 strataFlashDeviceId = 0 ;
LOCAL UINT32 strataFlashNumSectors = 0 ;
LOCAL UINT32 strataFlashSectorSize = 0 ;

STATUS sysFlashReadIdIntel
    (
    volatile FlashWord *adrs
    )
    {
    FlashWord mfgrId, deviceId ;

    adrs->l = 0x00ff00ff ;
    sysFlashDelay();
    adrs->l = 0x00900090 ;
    sysFlashDelay();

    mfgrId.l = adrs[0].l ;
    deviceId.l = adrs[1].l ;

    switch ( deviceId.l )
        {
        case INTEL28F320_DEVICE_ID: /* 32 Mbit */
            if ( deviceId.l == INTEL28F320_DEVICE_ID )
                {
                i28f320found = 1 ;
                strataFlashDeviceId = deviceId.c[1] ;
                strataFlashNumSectors = i28f320NumSectors ;
                strataFlashSectorSize = i28f320SectorSize ;
#ifdef FLASH_DEBUG
                printf("sysFlashReadIdIntel: Intel 28F320 Strataflash found!\n");
#endif
                }
            break;
        case INTEL28F640_DEVICE_ID: /* 64 Mbit */
            if ( deviceId.l == INTEL28F640_DEVICE_ID )
                {
                i28f640found = 1 ;
                strataFlashDeviceId = deviceId.c[1] ;
                strataFlashNumSectors = i28f640NumSectors ;
                strataFlashSectorSize = i28f640SectorSize ;
#ifdef FLASH_DEBUG
                printf("sysFlashReadIdIntel: Intel 28F640 Strataflash found!\n");
#endif
                }
            break;
        case INTEL28F128_DEVICE_ID: /* 128 Mbit */
            if ( deviceId.l == INTEL28F128_DEVICE_ID )
                {
                i28f128found = 1 ;
                strataFlashDeviceId = deviceId.c[1] ;
                strataFlashNumSectors = i28f128NumSectors ;
                strataFlashSectorSize = i28f128SectorSize ;
#ifdef FLASH_DEBUG
                printf("sysFlashReadIdIntel: Intel 28F128 Strataflash found!\n");
#endif
                }
            break;
        default:
            printf("sysFlashReadIdIntel: invalid deviceId - 0x%x\n",(unsigned)deviceId.c[1]);
            adrs->l = 0x00ff00ff ;
            sysFlashDelay();
            return ERROR ;
        }

    if ( mfgrId.l != INTEL_MFGR_ID )
        {
        adrs->l = 0x00ff00ff ;
        sysFlashDelay();
#ifdef FLASH_DEBUG
        printf("sysFlashReadIdIntel: Intel Strataflash not found!\n");
#endif
        return ERROR ;
        }
    else if ( strataFlashDeviceId < 0x16 || strataFlashDeviceId > 0x18 )
        {
        adrs->l = 0x00ff00ff ;
        sysFlashDelay();
#ifdef FLASH_DEBUG
        printf("sysFlashReadIdIntel: Intel Strataflash not found!\n");
#endif
        return ERROR ;
        }

    adrs->l = 0x00ff00ff ;
    sysFlashDelay();

    return OK ;
    }



/******************************************************************************
*
* sysFlashBlockLockStatusIntel - retrieve the lock status of the given block.
*
* RETURNS: OK - block locked, or ERROR  - block not locked.
*
*/

STATUS sysFlashBlockLockStatusIntel
    (
    FlashWord * adrs
    )
    {
    FlashWord * ptr ;
    volatile UINT32 temp ;
    volatile UINT32 s1 ;
    STATUS status ;

    temp = (volatile UINT32) adrs ;
    temp &= ~(INTEL28F128_SECTOR_SZ-1) ;
    ptr = (FlashWord *)temp ;

    ptr->l = 0x00980098 ;
    ptr++ ;
    ptr++ ;
    sysFlashDelay();
    s1 = ptr->l ; /* ? find correct location of lock bit in x32 mode */

#if FLASH_DEBUG
    printf("lock status: s1 - 0x%8.8x\n", s1);
#endif

    /* return chips to read mode */
    ptr->l = 0x00ff00ff ;
    sysFlashDelay() ;

    if ( s1&0x00010001 )
        status = OK ;
    else
        status = ERROR ;

    return status ;
    }


/******************************************************************************
*
* checkStatusIntel - check the status word from flash.
*
* RETURNS: OK, or ERROR 
*
*/

int checkStatusIntel
    (
    UINT32 status1
    )
    {
    if ( status1&0x00200000 || 
         status1&0x00000020 )
        {
        printf("sysFlashPollDataIntel: block erase error.\n");
        return -1 ;
        }
    if ( status1&0x00080000 || 
         status1&0x00000008 ) /* Vpp low */
        {
        printf("sysFlashPollDataIntel Vpp low.\n");
        return -1 ;
        }
    if ( status1&0x00020000 || 
         status1&0x00000002 ) /* block lock bit detected */
        {
        printf("sysFlashPollDataIntel: block lock bit detected.\n");
        return -1 ;
        }
    if ( !(status1&0x00040004) )  /* program completed */
        return 0 ;
    else
        return 1 ;
    }


/******************************************************************************
*
* sysFlashPollDataIntel - poll the flash status register for completion.
*
* RETURNS: OK, or ERROR 
*
*/

STATUS sysFlashPollDataIntel
    (
    FlashWord * adrs
    )
    {
    FlashWord * ptr = adrs ;
    UINT32 s1 ;
    int status ;

    while ( TRUE )
        {
        ptr->l = 0x00500050 ; /* clear status reg */
        sysFlashDelay();
        ptr->l = 0x00700070 ; /* read status reg */
        sysFlashDelay();

        s1 = ptr->l ;

        if ( !((s1 & 0x00000080)&&
               (s1 & 0x00800000)) )
            continue; /* wsm not ready */

        status = checkStatusIntel(s1);
        if ( status == -1 )
            {
            ptr->l = 0x00ff00ff;
            sysFlashDelay();
            return ERROR ;
            }
        else if ( status == 1 )
            continue;
        else if ( status == 0 )
            break;
        }

    ptr->l = 0x00ff00ff ;
    sysFlashDelay();

    return OK ;
    }


/******************************************************************************
*
* sysFlashSectorEraseIntel - erase the requested Intel Strataflash sector.
*
* RETURNS: OK, or ERROR if unable to erase the sector.
*
*/

STATUS sysFlashSectorEraseIntel
    (
    char * sectorAdrs
    )
    {
    FlashWord * ptr = (FlashWord *)sectorAdrs ;

    ptr->l = 0x00200020 ;
    sysFlashDelay();
    ptr->l = 0x00d000d0 ;
    sysFlashDelay();

    if ( sysFlashPollDataIntel(ptr) != OK )
        {
        printf("sysFlashSectorEraseIntel: sysFlashPollDataIntel failure.\n");
        return ERROR ;
        }

    ptr->l = 0x00ff00ff ;
    sysFlashDelay();

    return OK ;
    }


/******************************************************************************
*
* sysFlashChipEraseIntel - Erase the entire Intel Strataflash chip.
*
* RETURNS: OK, or ERROR if unable to erase the entire chip.
*
*/

STATUS sysFlashChipEraseIntel(void)
    {
    int i ;
    FlashWord * ptr = (FlashWord *)INTEL28F128_BASE_ADRS ;

    /* no chip erase - loop through all blocks, note ptr math : FlashWord is 4 bytes
       so incrementing ptr will advance the address +4. That's why INTEL28F128_SECTOR_SZ
       is adjusted by dividing by 4 since it defines how many bytes are in a sector. */
    for ( i=0 ; i<INTEL28F128_NUM_SECTORS ; i++, ptr+=(INTEL28F128_SECTOR_SZ/4) )
        {
#if FLASH_DEBUG
        printf("sysFlashChipEraseIntel: erasing sector %d, adrs - 0x%x\n",i,(unsigned)ptr) ;
#else
        printf(".") ;
#endif
        if ( sysFlashSectorEraseIntel((char*)ptr)!=OK )
            {
            printf("sysFlashChipEraseIntel: Unable to erase sector @ 0x%x\n",(unsigned)ptr);
            return ERROR ;
            }
        }

    return OK ;
    }


/******************************************************************************
*
* sysFlashSectorWriteBufferIntel - this write routine uses write to buffer
* programming - faster than byte at a time programming. Note: max number of
* bytes per buffer is 16 for x16 mode. The caller must guarantee that the 
* data doesn't exceed flash sector boundaries.
*
* RETURNS: OK, or ERROR if unable to program the requested sector.
*
*/

#define INTEL_28F128_WRITE_TO_BUFFER_COUNT 16 /* in x16 mode */
#define INTEL_28F128_WRITE_TO_BUFFER_TIMEOUT 5 /* 5 seconds */
STATUS sysFlashSectorWriteBufferIntel
    (
    FlashWord *adrs, 
    char * buf, 
    int len
    )
    {
    int i ;
    int longsToWrite ;
    /* assumes that adrs is 32 bit aligned */
    volatile FlashWord * ptr=NULL ;
    volatile FlashWord *dst ;
    volatile FlashWord *src ;
    volatile UINT32 s1 ;
    UINT32 endTicks ;
    FlashWord f ;
    int newLen=0 ;
    UINT8 *memBuf ;
    int alignment ;
    int nBytes ;

#ifdef FLASH_DEBUG
    printf("sysFlashSectorWriteBuffeBufferIntel: programming @ 0x%x.\n",(unsigned)adrs);
#endif

    longsToWrite = ((len-1)/4)+1 ;

    /* check to see if block is locked */
    if ( sysFlashBlockLockStatusIntel((FlashWord*)adrs) == OK )
        {
        printf("sysFlashSectorWriteBufferIntel: sectorAdrs 0x%x is locked.\n",(unsigned)adrs);
        return ERROR ;
        }

    /* programming address must being on word boundary */
    switch ( ((UINT32)adrs&0x3) )
        {
        case 0:
            ptr = (FlashWord*)adrs ;
            newLen = len ;
            break;
        case 1:
            ptr = (FlashWord*)((UINT32)adrs-1) ;
            newLen = len+1 ;
            break;
        case 2:
            ptr = (FlashWord*)((UINT32)adrs-2) ;
            newLen = len+2 ;
            break;
        case 3:
            ptr = (FlashWord*)((UINT32)adrs-3) ;
            newLen = len+3 ;
            break;
        }

    /* make length a multiple of 4 */
    switch ( newLen%4 )
        {
        case 0:
            break;
        case 1:
            newLen += 3 ;
            break;
        case 2:
            newLen += 2 ;
            break;
        case 3:
            newLen += 1 ;
            break;
        }
    /* assumes that sector has been erased */

    memBuf = (UINT8*)cacheDmaMalloc(newLen) ;
    if ( memBuf == NULL )
        {
        logMsg("Unable to allocate memory size %d bytes.\n",newLen,2,3,4,5,6);
        return ERROR ;
        }

    /* copy existing sector data to memBuf */
    memcpy((void*)memBuf,(const void*)ptr,newLen);

    alignment = (UINT32)adrs & 0x3 ;
    /* copy buffer data to memBuf */
    memcpy(&memBuf[alignment],buf,len);
    nBytes = newLen ;
    longsToWrite = nBytes / 4 ;

    src = (FlashWord *)memBuf ;
    dst = ptr ;

    while ( longsToWrite > 0 )
        {
        /* we want about 5 second timeout */
        endTicks = tickGet() + (sysClkRateGet()*5);
        while ( TRUE )
            {
            do
                {
                ptr->l = 0x00500050 ; /* clear status register */
                sysFlashDelay();
                ptr->l = 0x00700070 ;
                sysFlashDelay();
                s1 = ptr->l ;
                } while ( !(s1&0x00800000) || !(s1&0x00000080) ) ;

            ptr->l = 0x00e800e8 ; /* set write to buffer mode */
            sysFlashDelay();
            s1 = ptr->l ;   /* read extended status register */

            if ( tickGet() < endTicks )
                {
                if ( !(s1&0x00800000) || !(s1&0x00000080) )
                    {
                    /* continue waiting for buffer, not available, and timeout not expired */
                    continue; 
                    }
                }
            else if ( tickGet() >= endTicks )
                {
                /* timeout waiting for buffer to be available */
                do
                    {
                    ptr->l = 0x00500050 ; /* clear status register */
                    sysFlashDelay();
                    ptr->l = 0x00700070 ;
                    sysFlashDelay();
                    s1 = ptr->l ;
                    } while ( !(s1&0x00800000) || !(s1&0x00000080) ) ;
                printf("sysFlashSectorWriteBufferIntel: timeout trying to get write buffer!\n");
                ptr->l = 0x00ff00ff ;
                sysFlashDelay();
                return ERROR ;
                }
#ifdef DEBUG
            /* write word count (0 = 1) */
            printf("longs to write - 0%d\n",longsToWrite) ;
#endif
            /* load word count into each x16 chip */
            f.s[0] = f.s[1] = (longsToWrite > 16) ? 15 : longsToWrite-1 ;
            ptr->l = f.l ;

            /* buffer is available */
            for ( i=0 ; (i < INTEL_28F128_WRITE_TO_BUFFER_COUNT) && 
                (longsToWrite > 0) ; i++, src++, dst++, longsToWrite-- )
                dst->l = src->l ;

            ptr->l = 0x00d000d0 ; /* confirm */
            break;
            }
        }

    /* read reset */
    do
        {
        ptr->l = 0x00500050 ; /* clear status register */
        sysFlashDelay();
        ptr->l = 0x00700070 ;
        sysFlashDelay();
        s1 = ptr->l ;
        } while ( !(s1&0x00800000) || !(s1&0x00000080) ) ;

    ptr->l = 0x00ff00ff ;
    sysFlashDelay();

    return OK ;
    }




/******************************************************************************
*
* sysFlashSectorLockIntel - Lock the requested Intel Stratflash sector.
*
* RETURNS: OK, or ERROR if unable to lock the sector.
*
*/

STATUS sysFlashSectorLockIntel
    (
    char *adrs
    )
    {
    FlashWord * ptr = (FlashWord *)adrs;
    volatile UINT32 s1 ;

    ptr->l = 0x00600060 ;
    sysFlashDelay();
    ptr->l = 0x00010001 ;
    sysFlashDelay();

    do
        {
        s1 = ptr->l ;
        } while ( !(s1 & 0x00800000) ||
                  !(s1 & 0x00000080) );

    ptr->l = 0x00700070 ;
    sysFlashDelay();
    s1 = ptr->l ;

    if ( (s1 & 0x00100000) ||
         (s1 & 0x00000010) )
        {
        printf("sysFlashSectorLockIntel: Unable to set lock bit for sector 0x%x\n", (unsigned)adrs);

        ptr->l = 0x00500050 ;
        sysFlashDelay();
        ptr->l = 0x00ff00ff ;
        sysFlashDelay();
        return ERROR ;
        }

    ptr->l = 0x00ff00ff ;
    sysFlashDelay();
    return OK ;
    }

/******************************************************************************
*
* sysFlashSectorUnlockIntel - unlock the requested Intel Strataflash sector.
*
* RETURNS: OK, or ERROR if unable to unlock sector.
*
*/

STATUS sysFlashSectorUnlockIntel(void)
    {
    FlashWord * ptr =  (FlashWord *)INTEL28F128_BASE_ADRS ;
    volatile UINT32 s1 ;

    ptr->l = 0x00600060 ;
    sysFlashDelay();
    ptr->l = 0x00d000d0 ;
    sysFlashDelay();
    do
        {
        s1 = ptr->l ;

        } while ( !(s1 & 0x00800000) ||
                  !(s1 & 0x00000080) );

    ptr->l = 0x00700070 ;
    sysFlashDelay();
    s1 = ptr->l ;

    if ( (s1 & 0x00200000) ||
         (s1 & 0x00000020) )
        {
        printf("sysFlashSectorUnlockIntel: Unable to clear lock bits\n");
        ptr->l = 0x00500050 ;
        sysFlashDelay();
        ptr->l = 0x00ff00ff ;
        sysFlashDelay();
        return ERROR ;
        }

    ptr->l = 0x00ff00ff ;
    sysFlashDelay();
    return OK ;
    }



/******************************************************************************
*
* sysFlashSectorWriteIntel - this write routine uses byte programming.
* to program an Intel Strataflash sector. Allows any alignment for
* adrs and buf. Assumes that the flash region has been erased before
* this function is called.
*
* RETURNS: OK, or ERROR 
*
*/

STATUS sysFlashSectorWriteIntel
    (
    char * adrs, 
    char * buf, 
    int len
    )
    {
    int i ;
    FlashWord * ptr=NULL ;
    FlashWord *dst ;
    FlashWord *src ;
    int newLen=0 ;
    UINT8 *memBuf ;
    int alignment ;
    int nBytes ;

#ifdef FLASH_DEBUG
    printf("sysFlashSectorWriteIntel: programming @ 0x%x.\n",(unsigned)adrs);
#endif

    /* programming address must being on word boundary */
    switch ( ((UINT32)adrs&0x3) )
        {
        case 0:
            ptr = (FlashWord*)adrs ;
            newLen = len ;
            break;
        case 1:
            ptr = (FlashWord*)((UINT32)adrs-1) ;
            newLen = len+1 ;
            break;
        case 2:
            ptr = (FlashWord*)((UINT32)adrs-2) ;
            newLen = len+2 ;
            break;
        case 3:
            ptr = (FlashWord*)((UINT32)adrs-3) ;
            newLen = len+3 ;
            break;
        }

    /* make length a multiple of 4 */
    switch ( newLen%4 )
        {
        case 0:
            break;
        case 1:
            newLen += 3 ;
            break;
        case 2:
            newLen += 2 ;
            break;
        case 3:
            newLen += 1 ;
            break;
        }
    /* assumes that sector has been erased */

    memBuf = (UINT8*)cacheDmaMalloc(newLen) ;
    if ( memBuf == NULL )
        {
        logMsg("Unable to allocate memory size %d bytes.\n",newLen,2,3,4,5,6);
        return ERROR ;
        }

    /* copy existing secdtor data to memBuf */
    memcpy(memBuf,ptr,newLen);

    alignment = (UINT32)adrs & 0x3 ;
    /* copy buffer data to memBuf */
    memcpy(&memBuf[alignment],buf,len);
    nBytes = newLen ;

    src = (FlashWord *)memBuf ;
    dst = ptr ;

    for ( i=0 ; i < (nBytes/4); i++, src++, dst++ )
        {
        if ( src->l == 0xffffffff )
            continue;

        ptr->l = 0x00400040 ;
        sysFlashDelay();
        dst->l = src->l ;

        if ( sysFlashPollDataIntel(ptr) != OK )
            {
            ptr->l = 0x00ff00ff ;
            sysFlashDelay();
            printf("sysFlashSectorWriteIntel: sysFlashPollDataIntel failure.\n");
            if ( memBuf != NULL )
                cacheDmaFree(memBuf);
            return ERROR ;
            }
        }

    if ( memBuf != NULL )
        cacheDmaFree(memBuf);

    ptr->l = 0x00ff00ff ;
    sysFlashDelay();

    return OK ;
    }


/******************************************************************************
*
* sysFlashEraseStrataflash - erase the requested Intel Strataflash sector.
*
* RETURNS: OK, or ERROR if input argument error failure to erase sector.
*
*/

STATUS sysFlashEraseStrataflash
    (
    UINT8 * sectorAdrs
    )
    {
    /* validate input arg */
    if ( ((UINT32)sectorAdrs) < INTEL28F128_BASE_ADRS )
        {
        printf("sysFlashEraseStrataflash: invalid sector adrs - 0x%x\n",(unsigned)sectorAdrs);
        return ERROR ;
        }
    if ( (((UINT32)sectorAdrs)) > (INTEL28F128_BASE_ADRS+INTEL28F128_SIZE) )
        {
        printf("sysFlashEraseStrataflash: invalid sector adrs - 0x%x\n",(unsigned)sectorAdrs);
        return ERROR ;
        }
    /* check device */
    if ( sysFlashReadIdIntel((FlashWord*)INTEL28F128_BASE_ADRS) != OK )
        {
        printf("sysFlashEraseStrataflash: can't read id from device\n");
        return ERROR ;
        }

    if ( sysFlashSectorEraseIntel((char*)sectorAdrs) != OK )
        {
        printf("sysFlashEraseStrataflash: sysFlashSectorEraseIntel failure.\n");
        return ERROR ;
        }

    return OK ;
    }


/******************************************************************************
*
* sysFlashWriteStrataflash - program the requested sector using the byte program
* algorithm. The data buffer must fit in the requested Intel Strataflash sector.
*
* RETURNS: OK, or ERROR if input argument error failure to program sector.
*
*/

STATUS sysFlashWriteStrataflash
    (
    UINT8 * sectorAdrs, 
    UINT8 * buffer, 
    int length
    )
    {
    /* validate input arg */
    if ( ((UINT32)sectorAdrs) < INTEL28F128_BASE_ADRS )
        {
        printf("sysFlashWriteStrataflash: invalid sector adrs - 0x%x\n",(unsigned)sectorAdrs);
        return ERROR ;
        }
    if ( (((UINT32)sectorAdrs)+length) > (INTEL28F128_BASE_ADRS+INTEL28F128_SIZE) )
        {
        printf("sysFlashWriteStrataflash: invalid sector adrs - 0x%x\n",(unsigned)sectorAdrs);
        return ERROR ;
        }
    /* check device */
    if ( sysFlashReadIdIntel((FlashWord*)INTEL28F128_BASE_ADRS) != OK )
        {
        printf("sysFlashWriteStrataflash: can't read id from device\n");
        return ERROR ;
        }
    if ( sysFlashSectorWriteIntel(sectorAdrs, buffer, length)!=OK )
        {
        printf("sysFlashWriteStrataflash: sysFlashSectorWriteIntel failure.\n");
        return ERROR ;
        }
    return OK ;
    }

/******************************************************************************
*
* sysFlashWriteBufferStrataflash - program using the write buffer algorithm
* the Intel strataflash sector. The data buffer must fit in the requested sector.
*
* RETURNS: OK, or ERROR if input argument error failure to program sector.
*
*/

STATUS sysFlashWriteBufferStrataflash
    (
    UINT8 * sectorAdrs, 
    UINT8 * buffer, 
    int length
    )
    {
    /* validate input arg */
    if ( ((UINT32)sectorAdrs) < INTEL28F128_BASE_ADRS )
        {
        printf("sysFlashWriteBufferStrataflash: invalid sector adrs - 0x%x\n",(unsigned)sectorAdrs);
        return ERROR ;
        }
    if ( (((UINT32)sectorAdrs)+length) > (INTEL28F128_BASE_ADRS+INTEL28F128_SIZE) )
        {
        printf("sysFlashWriteBufferStrataflash: invalid sector adrs - 0x%x\n",(unsigned)sectorAdrs);
        return ERROR ;
        }
    /* check device */
    if ( sysFlashReadIdIntel((FlashWord*)INTEL28F128_BASE_ADRS) != OK )
        {
        printf("sysFlashWriteBufferStrataflash: can't read id from device\n");
        return ERROR ;
        }

    if ( sysFlashSectorWriteBufferIntel((FlashWord*)(sectorAdrs), buffer, length)!=OK )
        {
        printf("sysFlashWriteBufferStrataflash: sysFlashSectorWriteIntel failure.\n");
        return ERROR ;
        }
    return OK ;
    }


#ifdef FLASH_TEST
/* The following are test routines to illustrate flash driver function usage. */
STATUS testFlashIntel(UINT32 adrs)
    {
    char buffer[256];

    if ( adrs < FLASH_BASE_ADRS || adrs > ROM_BASE_ADRS )
        {
        printf("Attempting to test an invalid flash address 0x%x\n",adrs);
        return ERROR ;
        }

    memset(buffer,0xff,256);
    strcpy(buffer, "The quick brown fox jumps over the lazy dog.\n");

    if ( sysFlashSectorLockIntel((char*)adrs) != OK )
        {
        printf("1a-unable to lock block\n");
        return ERROR ;
        }
    if ( sysFlashSectorLockIntel((char*)adrs) != OK )
        {
        printf("1b-unable to lock block\n");
        }

    if ( sysFlashEraseStrataflash((char *)adrs)!=OK )
        {
        printf("1-can't erase sector\n");
        }
    if ( sysFlashWriteStrataflash((char*)adrs,buffer,sizeof(buffer))!=OK )
        {
        printf("1-can't write chip\n");
        }

    if ( sysFlashSectorUnlockIntel() != OK )
        {
        printf("1c-unable to unlock blocks\n");
        return ERROR ;
        }
    if ( sysFlashSectorUnlockIntel() != OK )
        {
        printf("1d-unable to unlock blocks\n");
        }
    if ( sysFlashChipEraseIntel()!= OK )
        {
        printf("2-can't erase chip\n");
        return ERROR ;
        }
    if ( sysFlashEraseStrataflash((char *)adrs)!=OK )
        {
        printf("2-can't erase sector\n");
        return ERROR ;
        }
    if ( sysFlashWriteStrataflash((char*)adrs,buffer,sizeof(buffer))!=OK )
        {
        printf("2-can't write chip\n");
        return ERROR ;
        }
    if ( sysFlashWriteStrataflash((char*)(adrs+0x41),buffer,sizeof(buffer))!=OK )
        {
        printf("2-can't write chip\n");
        return ERROR ;
        }
    if ( sysFlashWriteStrataflash((char*)(adrs+0x82),buffer,sizeof(buffer))!=OK )
        {
        printf("2-can't write chip\n");
        return ERROR ;
        }
    if ( sysFlashWriteStrataflash((char*)(adrs+0xc3),buffer,sizeof(buffer))!=OK )
        {
        printf("2-can't write chip\n");
        return ERROR ;
        }

    return OK ;
    }


STATUS testFlashBufferIntel(UINT32 adrs)
    {
    char buffer[256];

    if ( adrs < FLASH_BASE_ADRS || adrs > ROM_BASE_ADRS )
        {
        printf("Attempting to test an invalid flash address 0x%x\n",adrs);
        return ERROR ;
        }

    memset(buffer,0xff,256);
    strcpy(buffer, "The quick brown fox jumps over the lazy dog. The rain in Spain falls mainly on the plain.\n");

    if ( sysFlashSectorLockIntel((char*)adrs) != OK )
        {
        printf("1a-unable to lock block\n");
        return ERROR ;
        }
    if ( sysFlashSectorLockIntel((char*)adrs) != OK )
        {
        printf("1b-unable to lock block\n");
        }
    if ( sysFlashEraseStrataflash((char *)adrs)!=OK )
        {
        printf("1-can't erase sector\n");
        }
    if ( sysFlashWriteBufferStrataflash((char*)adrs,buffer,sizeof(buffer))!=OK )
        {
        printf("1-can't write chip\n");
        }
    if ( sysFlashSectorUnlockIntel() != OK )
        {
        printf("1c-unable to unlock blocks\n");
        return ERROR ;
        }
    if ( sysFlashSectorUnlockIntel() != OK )
        {
        printf("1d-unable to unlock blocks\n");
        }
    printf("all blocks unlocked...\n");

    /* test chip erase */

    if ( sysFlashChipEraseIntel()!= OK )
        {
        printf("2-can't erase chip\n");
        return ERROR ;
        }
    if ( sysFlashEraseStrataflash((char *)adrs)!=OK )
        {
        printf("2-can't erase sector\n");
        return ERROR ;
        }
    if ( sysFlashWriteBufferStrataflash((char*)adrs,buffer,sizeof(buffer))!=OK )
        {
        printf("2-can't write chip\n");
        return ERROR ;
        }
    if ( sysFlashWriteBufferStrataflash((char*)(adrs+0x101),buffer,sizeof(buffer))!=OK )
        {
        printf("2-can't write chip\n");
        return ERROR ;
        }
    if ( sysFlashWriteBufferStrataflash((char*)(adrs+0x202),buffer,sizeof(buffer))!=OK )
        {
        printf("2-can't write chip\n");
        return ERROR ;
        }
    if ( sysFlashWriteBufferStrataflash((char*)(adrs+0x303),buffer,sizeof(buffer))!=OK )
        {
        printf("2-can't write chip\n");
        return ERROR ;
        }

    return OK ;
    }
#endif

