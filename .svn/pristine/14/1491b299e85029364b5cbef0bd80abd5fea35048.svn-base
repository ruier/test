/* sysFlash.c - BSP Intel Strataflash flash driver */

/*
 * Copyright (c) 2002, 2004, 2007, 2008, 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2002-2004 Motorola, Inc. */

/*
modification history
--------------------
01i,09nov10,sye  Add flash software write protect mechanism. (WIND00235333)
01h,21may08,fao  Fixed mem leak (WIND00123625)
01g,27apr08,y_w  Fixed gnu compiler warnings and make man error and code cleanup.
01f,20mar07,y_w  Support new flash type: INTEL 28F128J3
01e,01nov04,cak  Fixed diab compiler errors.
01d,11may04,cak  Removed a misplaced return(ERROR) statement.
01c,15apr04,cak  Modified for MVME6100.
01b,30jan04,nrv  modified to get it to work with mv5500
01a,08mar02,gtf  new
*/

/*
DESCRIPTION

This library provides general Intel Strataflash driver support. 
Strataflash driver functions are implemented as specified in 
the Strataflash datasheet. 

Note that none of the driver functions are protected for 
simultaneous usage (e.g. with a semaphore) so if this is
intended by the user wrapper functions should be implemented.

Two types of flash write routines are provided one that uses
buffer mode programming versus one that does not. When
programming large buffers it is suggested that the buffer mode
write routine be used.

Finally, the flash write routines assume that the input data
is 32 bit aligned and that a multiple of 32 bits are written.
This means that any functions calling the flash write functions
are required to provide programming data to the flash 
write routines with these restrictions. 
*/

/* includes */

#include <vxWorks.h>
#include "config.h"
#include "sysFlash.h"
#include <cacheLib.h>

#include <logLib.h>
#include <string.h>
#include <tickLib.h>
#include <stdio.h>

/* globals */

LOCAL UINT32 i28f128found = 0 ;
LOCAL UINT32 i28f640found = 0 ;
LOCAL UINT32 i28f320found = 0 ;
LOCAL UINT32 i28f256found = 0 ;

/* chips are hooked up x16 on mv6100, and sectors are 256K */

LOCAL UINT32 i28f128SectorSize = INTEL28F128_SECTOR_SZ ;
LOCAL UINT32 i28f640SectorSize = INTEL28F640_SECTOR_SZ ;
LOCAL UINT32 i28f320SectorSize = INTEL28F320_SECTOR_SZ ;
LOCAL UINT32 i28f256SectorSize = INTEL28F256_SECTOR_SZ ;

LOCAL UINT32 i28f128NumSectors = INTEL28F128_NUM_SECTORS ;
LOCAL UINT32 i28f640NumSectors = INTEL28F640_NUM_SECTORS ;
LOCAL UINT32 i28f320NumSectors = INTEL28F320_NUM_SECTORS ;
LOCAL UINT32 i28f256NumSectors = INTEL28F256_NUM_SECTORS ;

LOCAL UINT32 strataFlashDeviceId = 0 ;
LOCAL UINT32 strataFlashNumSectors = 0 ;
LOCAL UINT32 strataFlashSectorSize = 0 ;


IMPORT void sysUsDelay (UINT);

/*******************************************************************************
*
* sysFlashDelay - introduce a small delay.
*
* RETURNS: OK, or ERROR 
*
*/

void sysFlashDelay(void)
    {

    /* need at least a 210 nsec delay */

    sysUsDelay(1); 
    }

/*******************************************************************************
*
* sysFlashSWPOn - set flash software write protect on
*
* This routine sets flash software write protect on for board MVME6100. The 
* flash software write protect bits of the system control and status register 2 
* protect whole flash bank A and the boot block of flash bank B from inadvertent
* erasing or programming.
*
* RETURNS: N/A
*/

LOCAL void sysFlashSWPOn(void)
{       
    *(UCHAR *)(SYSTEM_STATUS_REG_2) |=  (SYSTEM_STATUS_REG_2_FLASHA_WP_MASK |
                                         SYSTEM_STATUS_REG_2_FBOOTB_WP_MASK);
}

/*******************************************************************************
*
* sysFlashSWPOff - set flash software write protect off
*
* This routine sets flash software write protect off for board MVME6100. The 
* flash software write protect bits of the system control and status register 2 
* protect whole flash bank A and the boot block of flash bank B from inadvertent
* erasing or programming.
*
* RETURNS: N/A
*/

LOCAL void sysFlashSWPOff(void)
{       
    *(UCHAR *)(SYSTEM_STATUS_REG_2) &=  ~(SYSTEM_STATUS_REG_2_FLASHA_WP_MASK |
                                          SYSTEM_STATUS_REG_2_FBOOTB_WP_MASK);
}

/*******************************************************************************
*
* sysFlashReadIdIntel - Read mfgr id and device id from the Strataflash.
*
* RETURNS: OK, or ERROR 
*
*/

STATUS sysFlashReadIdIntel
    (
    volatile FlashWord *adrs
    )
    {
    FlashWord mfgrId, deviceId ;

    sysFlashSWPOff();

    adrs->l = 0x00ff00ff ;
    sysFlashDelay();
    adrs->l = 0x00900090 ;

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
        case INTEL28F256_DEVICE_ID: /* 256 Mbit */
            if ( deviceId.l == INTEL28F256_DEVICE_ID )
                {
                i28f256found = 1 ;
                strataFlashDeviceId = deviceId.c[1] ;
                strataFlashNumSectors = i28f256NumSectors ;
                strataFlashSectorSize = i28f256SectorSize ;
#ifdef FLASH_DEBUG
                printf("sysFlashReadIdIntel: Intel 28F256 Strataflash found!\n");
#endif
                }
            break;
        case INTEL28F128J3_DEVICE_ID: /* 256 Mbit (dual die)*/
            if ( deviceId.l == INTEL28F128J3_DEVICE_ID )
                {
                i28f256found = 1 ;
                strataFlashDeviceId = deviceId.c[1] ;
                strataFlashNumSectors = i28f256NumSectors ;
                strataFlashSectorSize = i28f256SectorSize ;
#ifdef FLASH_DEBUG
                printf("sysFlashReadIdIntel: Intel 28F128J3 Strataflash found!\n");
#endif
                }
            break;
        default:
            printf("sysFlashReadIdIntel: invalid deviceId - 0x%x\n",
            (unsigned)deviceId.c[1]);
            adrs->l = 0x00ff00ff ;
            sysFlashDelay();
            sysFlashSWPOn();
            return ERROR ;
        }

    if ( mfgrId.l != INTEL_MFGR_ID )
        {
        adrs->l = 0x00ff00ff ;
        sysFlashDelay();
#ifdef FLASH_DEBUG
        printf("sysFlashReadIdIntel: Intel Strataflash not found!\n");
#endif
        sysFlashSWPOn();
        return ERROR ;
        }

    adrs->l = 0x00ff00ff ;
    sysFlashDelay();
    sysFlashSWPOn();
    return OK ;
    }

/*******************************************************************************
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
    temp &= ~(strataFlashSectorSize-1) ;
    ptr = (FlashWord *)temp ;

    ptr->l = 0x00980098 ;
    ptr++ ;
    ptr++ ;

    sysDelay(); /* needs a bit longer delay here */

    s1 = ptr->l ; /* ? find correct location of lock bit in x32 mode */

#ifdef FLASH_DEBUG
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

/*******************************************************************************
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

/*******************************************************************************
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

    for(;;)
        {
        ptr->l = 0x00500050 ; /* clear status reg */
        ptr->l = 0x00700070 ; /* read status reg */

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

/*******************************************************************************
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

    sysFlashSWPOff();

    if ( sysFlashBlockLockStatusIntel((FlashWord*)ptr) == OK )
        {
#ifdef FLASH_DEBUG
        printf("sysFlashSectorEraseIntel: sectorAdrs 0x%x is locked.\n", (unsigned)sectorAdrs);
#endif
        if ( sysFlashSectorUnlockIntel((char*)ptr) != OK)
            { 
#ifdef FLASH_DEBUG
            printf("sysFlashSectorEraseIntel: Sector can't be unlocked.\n");
#endif
            sysFlashSWPOn();
            return ERROR ;
            }
        }
    
    ptr->l = 0x00200020 ;
    ptr->l = 0x00d000d0 ;

    if ( sysFlashPollDataIntel(ptr) != OK )
        {
        printf("sysFlashSectorEraseIntel: sysFlashPollDataIntel failure.\n");
        sysFlashSWPOn();
        return ERROR ;
        }

    ptr->l = 0x00ff00ff ;
    sysFlashDelay();
    sysFlashSWPOn();

    return OK ;
    }

/*******************************************************************************
*
* sysFlashChipEraseIntel - Erase the entire Intel Strataflash chip.
*
* RETURNS: OK, or ERROR if unable to erase the entire chip.
*
*/

STATUS sysFlashChipEraseIntel
    (
    UINT32 baseAdrs	/* base address of FLASH bank */
    )
    {
    int i ;
    FlashWord * ptr = (FlashWord *)baseAdrs;

    /* 
     * no chip erase - loop through all blocks, note ptr math : 
     * FlashWord is 4 bytes so incrementing ptr will advance 
     * the address +4. That's why strataFlashSectorSize 
     * is adjusted by dividing by 4 since it defines how many 
     * bytes are in a sector. 
     */

    for (i=0; i<strataFlashNumSectors; i++, ptr+=(strataFlashSectorSize/4))
        {
#ifdef FLASH_DEBUG
        printf("sysFlashChipEraseIntel: erasing sector %d, adrs - 0x%x\n",
           i,(unsigned)ptr) ;
#else
        printf(".") ;
#endif
        if ( sysFlashSectorEraseIntel((char*)ptr)!=OK )
            {
            printf("sysFlashChipEraseIntel: Unable to erase sector @ 0x%x\n",
           (unsigned)ptr);
            return ERROR ;
            }
        }

    return OK ;
    }

/*******************************************************************************
*
* sysFlashSectorWriteBufferIntel - sector buffer write of Intel flash
*
* This write routine uses write to buffer programming - faster than byte at a
* time programming. Note: max number of bytes per buffer is 16 for x16 mode. 
* The caller must guarantee that the data doesn't exceed flash sector boundaries.
*
* RETURNS: OK, or ERROR if unable to program the requested sector.
*
*/

#define INTEL_28F256_WRITE_TO_BUFFER_COUNT 16 /* in x16 mode */
#define INTEL_28F256_WRITE_TO_BUFFER_TIMEOUT 5 /* 5 seconds */
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
    printf("sysFlashSectorWriteBufferIntel: programming @ 0x%x.\n",
	   (unsigned)adrs);
#endif

    sysFlashSWPOff();

    longsToWrite = ((len-1)/4)+1 ;

    /* check to see if block is locked */

    if ( sysFlashBlockLockStatusIntel((FlashWord*)adrs) == OK )
        {
#ifdef FLASH_DEBUG
        printf("sysFlashSectorWriteBufferIntel: sectorAdrs 0x%x is locked.\n",
               (unsigned)adrs);
#endif
        if ( sysFlashSectorUnlockIntel((char*)adrs) != OK)
            { 
#ifdef FLASH_DEBUG
            printf("sysFlashSectorWriteBufferIntel: Can't unlock sector.\n");
#endif
            sysFlashSWPOn();
            return ERROR ;
            }
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
        sysFlashSWPOn();
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
        for(;;)
            {
            do
                {
                ptr->l = 0x00500050 ; /* clear status register */
                ptr->l = 0x00700070 ;
                s1 = ptr->l ;
                } while ( !(s1&0x00800000) || !(s1&0x00000080) ) ;

            ptr->l = 0x00e800e8 ; /* set write to buffer mode */
            s1 = ptr->l ;   /* read extended status register */

            if ( tickGet() < endTicks )
                {
                if ( !(s1&0x00800000) || !(s1&0x00000080) )
                    {

            /* 
             * continue waiting for buffer, not available, 
             * and timeout not expired 
             */

                    continue; 
                    }
                }
            else if ( tickGet() >= endTicks )
                {

                /* timeout waiting for buffer to be available */

                do
                    {
                    ptr->l = 0x00500050 ; /* clear status register */
                    ptr->l = 0x00700070 ;
                    s1 = ptr->l ;
                    } while ( !(s1&0x00800000) || !(s1&0x00000080) ) ;
                printf("sysFlashSectorWriteBufferIntel: timeout trying to get write buffer!\n");
                ptr->l = 0x00ff00ff ;
                sysFlashDelay();
                cacheDmaFree(memBuf);
                sysFlashSWPOn();
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

            for ( i=0 ; (i < INTEL_28F256_WRITE_TO_BUFFER_COUNT) && 
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
        ptr->l = 0x00700070 ;
        s1 = ptr->l ;
        } while ( !(s1&0x00800000) || !(s1&0x00000080) ) ;

    ptr->l = 0x00ff00ff ;
    sysFlashDelay();
    sysFlashSWPOn();
    
    cacheDmaFree(memBuf);

    return OK ;
    }

/*******************************************************************************
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

    sysFlashSWPOff();

    ptr->l = 0x00600060 ;
    ptr->l = 0x00010001 ;

    do
        {
        s1 = ptr->l ;
        } while ( !(s1 & 0x00800000) ||
                  !(s1 & 0x00000080) );

    ptr->l = 0x00700070 ;
    s1 = ptr->l ;

    if ( (s1 & 0x00100000) ||
         (s1 & 0x00000010) )
        {
        printf("sysFlashSectorLockIntel: Unable to set lock bit for sector 0x%x\n", (unsigned)adrs);

        ptr->l = 0x00500050 ;
        ptr->l = 0x00ff00ff ;
        sysFlashDelay();
        sysFlashSWPOn();
        return ERROR ;
        }

    ptr->l = 0x00ff00ff ;
    sysFlashDelay();
    sysFlashSWPOn();
    return OK ;
    }

/*******************************************************************************
*
* sysFlashSectorUnlockIntel - unlock the requested Intel Strataflash sector.
*
* RETURNS: OK, or ERROR if unable to unlock sector.
*
*/

STATUS sysFlashSectorUnlockIntel
    (
    char *adrs
    )
    {
    FlashWord * ptr = (FlashWord *)adrs;
    volatile UINT32 s1 ;

    ptr->l = 0x00600060 ;
    ptr->l = 0x00d000d0 ;

    do
        {
        s1 = ptr->l ;

        } while ( !(s1 & 0x00800000) ||
                  !(s1 & 0x00000080) );

    ptr->l = 0x00700070 ;
    s1 = ptr->l ;

    if ( (s1 & 0x00200000) ||
         (s1 & 0x00000020) )
        {
        printf("sysFlashSectorUnlockIntel: Unable to clear lock bits\n");
        ptr->l = 0x00500050 ;
        ptr->l = 0x00ff00ff ;
        sysFlashDelay();
        return ERROR ;
        }

    ptr->l = 0x00ff00ff ;
    sysFlashDelay();
    return OK ;
    }

/*******************************************************************************
*
* sysFlashSectorWriteIntel - sector write of Intel flash
*
* This write routine uses byte programming.to program an Intel Strataflash 
* sector. Allows any alignment for adrs and buf. Assumes that the flash region
* has been erased before this function is called.
*
* RETURNS: OK, or ERROR 
*
*/

STATUS sysFlashSectorWriteIntel
    (
    char *  adrs, 
    char *  buf, 
    int     len
    )
    {
    int         i ;
    FlashWord * ptr=NULL ;
    FlashWord * dst ;
    FlashWord * src ;
    int         newLen=0 ;
    UINT8 *     memBuf ;
    int         alignment ;
    int         nBytes ;

#ifdef FLASH_DEBUG
    printf("sysFlashSectorWriteIntel: programming @ 0x%x.\n",(unsigned)adrs);
#endif

    sysFlashSWPOff();

    if ( sysFlashBlockLockStatusIntel((FlashWord *)adrs) == OK )
        {
        if ( sysFlashSectorUnlockIntel((char*)adrs) != OK)
            { 
#ifdef FLASH_DEBUG
            printf("sysFlashSectorWriteIntel: Sector can't be unlocked.\n");
#endif
            sysFlashSWPOn();
            return ERROR ;
            }
        }

    /* programming address must begin on word boundary */

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
        sysFlashSWPOn();
        return ERROR ;
        }

    /* copy existing sector data to memBuf */

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
        dst->l = src->l ;

        if ( sysFlashPollDataIntel(ptr) != OK )
            {
            ptr->l = 0x00ff00ff ;
            sysFlashDelay();
            printf("sysFlashSectorWriteIntel: sysFlashPollDataIntel failure.\n");
            if ( memBuf != NULL )
                cacheDmaFree(memBuf);
            sysFlashSWPOn();
            return ERROR ;
            }
        }

    if ( memBuf != NULL )
        cacheDmaFree(memBuf);

    ptr->l = 0x00ff00ff ;
    sysFlashDelay();
    sysFlashSWPOn();

    return OK ;
    }

/*******************************************************************************
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
    if ( sysFlashSectorEraseIntel((char*)sectorAdrs) != OK )
        {
        printf("sysFlashEraseStrataflash: sysFlashSectorEraseIntel failure.\n");
        return ERROR ;
        }

    return OK ;
    }

/*******************************************************************************
*
* sysFlashWriteStrataflash - sector write of Strata flash
*
* This routine program the requested sector using the byte program algorithm. 
* The data buffer must fit in the requested Intel Strataflash sector.
*
* RETURNS: OK, or ERROR if input argument error failure to program sector.
*/

STATUS sysFlashWriteStrataflash
    (
    UINT8 * sectorAdrs,
    UINT8 * buffer,
    int     length
    )
    {
    if ( sysFlashSectorWriteIntel((char *)sectorAdrs, (char *)buffer, length)!= OK )
        {
        printf("sysFlashWriteStrataflash: sysFlashSectorWriteIntel failure.\n");
        return ERROR ;
        }

    return OK ;
    }

/*******************************************************************************
*
* sysFlashWriteBufferStrataflash - sector buffer write of Strata flash
*
* This routine program the Intel strataflash sector by using the write buffer 
* algorithm. The data buffer must fit in the requested sector.
*
* RETURNS: OK, or ERROR if input argument error failure to program sector.
*/

STATUS sysFlashWriteBufferStrataflash
    (
    UINT8 * sectorAdrs, 
    UINT8 * buffer, 
    int length
    )
    {
    if ( sysFlashSectorWriteBufferIntel((FlashWord*)(sectorAdrs), 
         (char *)buffer, length)!= OK )
        {
        printf("sysFlashWriteBufferStrataflash: sysFlashSectorWriteIntel failure.\n");
        return ERROR ;
        }
    return OK ;
    }
