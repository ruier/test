/* sysMtd.c - MTD driver for Spansion S29GL series flash parts */

/*
 * Copyright (c) 2007-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,03apr08,b_m  support both 8-bit & 16-bit op.
01b,26sep07,b_m  make the driver more general.
01a,06jun07,b_m  written.
*/

/*
DESCRIPTION
This file provides the TFFS MTD driver for the Spansion S29GL series flash parts
on various boards (most probably on Freescale boards).  The driver handles the
low level operations like erase and program of the flash.  It also provides an
identify routine to check if it is compatible with the devices.

The macro S29GL_DEBUG_ON could be defined to enable informative debug messages
printing if necessary.

The driver is tested on Freescale MDS8568 and ADS5121e boards.  To make the
driver compatible with other untested boards, the following parameters of the
board flash specific configuration should be taken care of by the developer.

The following parameters should be defined in config.h:

\is

\i <S29GL_FLASH_BIT>
Specifies a single flash operation mode.
0 for 8-bit, 1 for 16-bit.

\i <S29GL_FLASH_NUM>
Specifies total flash number connected to one chip select on the board.

\i <S29GL_FLASH_WIDTH>
Specifies the data width on the flash chip select.
1 for 8-bit, 2 for 16-bit, 4 for 32-bit.

\i <S29GL_SECTOR_SIZE>
Specifies the number of bytes per sector per chip.

\i <S29GL_FLASH_SIZE>
Specifies the number of bytes per chip.

\ie

INCLUDE FILES:

SEE ALSO:
*/

#include <stdio.h>
#include <tffs/flflash.h>
#include <tffs/backgrnd.h>
#include "config.h"
#include "sysUsr.h"
/* externs */

#if 1
IMPORT  void sysUsDelay(UINT32 delay);

/* defines */

/* define this macro to enable informative debug messages */

#undef S29GL_DEBUG_ON

#ifdef S29GL_DEBUG_ON
#ifndef S29GL_DBG_MSG
#define S29GL_DBG_MSG(...)      printf(__VA_ARGS__)
#endif
#else
#define S29GL_DBG_MSG(...)
#endif


/* flash specific params */

#define S29GL_VENDOR_ID             0x01
#define S29GL_DEVICE_ID             0x7e
#define S29GL_FLASH_ID              (S29GL_VENDOR_ID << 8 | S29GL_DEVICE_ID)

#define S29GL_VENDOR_OFFSET         0
#if   (S29GL_FLASH_BIT == 0)        /* 8-bit */
#define S29GL_DEVICE_OFFSET         2
#elif (S29GL_FLASH_BIT == 1)        /* 16-bit */
#define S29GL_DEVICE_OFFSET         1
#endif

#if (S29GL_FLASH_WIDTH == 1)
typedef volatile UINT8 *            S29GL_FLASH_ADRS;
typedef volatile UINT8              S29GL_FLASH_DATA;
#define S29GL_WRITE_BYTE_ONCE       1
#define S29GL_WRITE_ADDR_MASK       0
#endif

#if (S29GL_FLASH_WIDTH == 2)
typedef volatile UINT16 *           S29GL_FLASH_ADRS;
typedef volatile UINT16             S29GL_FLASH_DATA;
#define S29GL_WRITE_BYTE_ONCE       2
#define S29GL_WRITE_ADDR_MASK       1
#endif

#if (S29GL_FLASH_WIDTH == 4)
typedef volatile UINT32 *           S29GL_FLASH_ADRS;
typedef volatile UINT32             S29GL_FLASH_DATA;
#define S29GL_WRITE_BYTE_ONCE       4
#define S29GL_WRITE_ADDR_MASK       3
#endif

/* sector address mask */

#define S29GL_SECTOR_MASK           (~(S29GL_FLASH_NUM * S29GL_SECTOR_SIZE - 1))

/* commands */

#if   (S29GL_FLASH_BIT == 0)        /* 8-bit */
#define S29GL_UNLOCK_ADDR1          0xaaa
#define S29GL_UNLOCK_ADDR2          0x555
#elif (S29GL_FLASH_BIT == 1)        /* 16-bit */
#define S29GL_UNLOCK_ADDR1          0x555
#define S29GL_UNLOCK_ADDR2          0x2aa
#endif

#define S29GL_UNLOCK_DATA1          ((S29GL_FLASH_DATA)0xaaaaaaaa)
#define S29GL_UNLOCK_DATA2          ((S29GL_FLASH_DATA)0x55555555)

#define S29GL_CMD_RESET             ((S29GL_FLASH_DATA)0xF0F0F0F0)
#define S29GL_CMD_READ_ID           ((S29GL_FLASH_DATA)0x90909090)
#define S29GL_CMD_ERASE_SECTOR1     ((S29GL_FLASH_DATA)0x80808080)
#define S29GL_CMD_ERASE_SECTOR2     ((S29GL_FLASH_DATA)0x30303030)
#define S29GL_CMD_PROGRAM           ((S29GL_FLASH_DATA)0xA0A0A0A0)

#define S29GL_DQ6_MASK              ((S29GL_FLASH_DATA)0x40404040)

#define S29GL_OP_TIMEOUT            5000
#define S29GL_OP_DELAY              5


/* flash ops */

#ifdef S29GL_FLASH_OP_DELAY
#define S29GL_FLASH_DELAY   \
    do  \
    {   \
        sysUsDelay(S29GL_OP_DELAY); \
    } while((0));
#else
#define S29GL_FLASH_DELAY
#endif

#define S29GL_RESET(addr)   \
    do  \
    {   \
        *addr = S29GL_CMD_RESET;    \
    } while((0));

#define S29GL_ERASE(addr)   \
    do  \
    {   \
        *(addr + S29GL_UNLOCK_ADDR1) = S29GL_UNLOCK_DATA1;      \
        *(addr + S29GL_UNLOCK_ADDR2) = S29GL_UNLOCK_DATA2;      \
        *(addr + S29GL_UNLOCK_ADDR1) = S29GL_CMD_ERASE_SECTOR1; \
        *(addr + S29GL_UNLOCK_ADDR1) = S29GL_UNLOCK_DATA1;      \
        *(addr + S29GL_UNLOCK_ADDR2) = S29GL_UNLOCK_DATA2;      \
        *addr = S29GL_CMD_ERASE_SECTOR2;    \
       /* S29GL_FLASH_DELAY; */ \
    } while((0));

#define S29GL_PROGRAM(addr1, addr2, value)  \
    do  \
    {   \
        *(addr1 + S29GL_UNLOCK_ADDR1) = S29GL_UNLOCK_DATA1; \
        *(addr1 + S29GL_UNLOCK_ADDR2) = S29GL_UNLOCK_DATA2; \
        *(addr1 + S29GL_UNLOCK_ADDR1) = S29GL_CMD_PROGRAM;  \
        *addr2 = value; \
       /* S29GL_FLASH_DELAY;  */\
    } while((0));

#define S29GL_READ_ID(addr) \
    do  \
    {   \
        *(addr + S29GL_UNLOCK_ADDR1) = S29GL_UNLOCK_DATA1;  \
        *(addr + S29GL_UNLOCK_ADDR2) = S29GL_UNLOCK_DATA2;  \
        *(addr + S29GL_UNLOCK_ADDR1) = S29GL_CMD_READ_ID;   \
        S29GL_FLASH_DELAY;  \
    } while((0));


/*******************************************************************************
*
* S29GLProgram - low level byte programming routine
*
* This routine is called to program byte to the flash part.
*
* RETURNS: flOK or flTimedOut if timeout.
*
* ERRNO: N/A
*/

LOCAL FLStatus s29GlProgram
    (
    S29GL_FLASH_ADRS    addr,
    S29GL_FLASH_DATA    value
    )
    {
    S29GL_FLASH_ADRS sector;
    S29GL_FLASH_DATA data1, data2;
    UINT32  timeout = 0;
	/*printf("addr 0x%x ,value 0x%x\n",addr,value);*/
    sector = (S29GL_FLASH_ADRS)((UINT32)addr & S29GL_SECTOR_MASK);
    S29GL_PROGRAM(sector, addr, value);

    /* set timeout = 5s */

    timeout = flMsecCounter + S29GL_OP_TIMEOUT;

    data1 = *addr;
    do
        {
        data2 = data1;
        data1 = *addr;
        if (flMsecCounter >= timeout)
            {
            S29GL_RESET(addr);
            return flTimedOut;
            }
        } while ((data1 & S29GL_DQ6_MASK) != (data2 & S29GL_DQ6_MASK));

    S29GL_RESET(addr);
    return flOK;
    }

/*******************************************************************************
*
* S29GLWrite - write routine for S29GL flash
*
* This routine is called to write data to the flash part.
*
* RETURNS: flOK or flWriteFault if write failed.
*
* ERRNO: N/A
*/

LOCAL FLStatus s29GlWrite
    (
    FLFlash         vol,
    CardAddress     address,
    const void FAR1 *buffer,
    int             length,
    int             overwrite
    )
    {
    UINT8  *unaligned;
    UINT8  *buf = (UINT8 *)buffer;
    UINT32  left = length;
    S29GL_FLASH_ADRS aligned;
    S29GL_FLASH_DATA data;
    UINT32  num;
    int     i;

    if (flWriteProtected(vol.socket))
        return flWriteProtect;

    /* calculate the program addr, make sure it's aligned */

    unaligned = (UINT8 *)vol.map (&vol, address, 0);
    num = (UINT32)unaligned & S29GL_WRITE_ADDR_MASK;
    aligned = (S29GL_FLASH_ADRS)((UINT32)unaligned - num);

    if (num != 0)
        {
        data = *aligned;

        for (i = num ; i < S29GL_WRITE_BYTE_ONCE; i++)
            {
            data &= ~(0xFF << (((S29GL_WRITE_BYTE_ONCE - 1) - i) * 8));
            data |= ((*(buf + i - num)) << (((S29GL_WRITE_BYTE_ONCE - 1) - i) * 8));
            }

        if (s29GlProgram(aligned, data) != flOK)
            return flWriteFault;

        buf  += (S29GL_WRITE_BYTE_ONCE - num);
        left -= (S29GL_WRITE_BYTE_ONCE - num);
        aligned++;
        }

    while (left >= S29GL_WRITE_BYTE_ONCE)
        {
        data = *(S29GL_FLASH_ADRS)buf;

        if (s29GlProgram(aligned, data) != flOK)
            return flWriteFault;

        buf  += S29GL_WRITE_BYTE_ONCE;
        left -= S29GL_WRITE_BYTE_ONCE;
        aligned++;
        }

    if (left > 0)
        {
        data = *aligned;

        for (i = 0 ; i < left; i++)
            {
            data &= ~(0xFF << (((S29GL_WRITE_BYTE_ONCE - 1) - i) * 8));
            data |= ((*(buf + i)) << (((S29GL_WRITE_BYTE_ONCE - 1) - i) * 8));
            }

        if (s29GlProgram(aligned, data) != flOK)
            return flWriteFault;
        }

    if (tffscmp((void FAR0 *)unaligned, buffer, length))
        {
        S29GL_DBG_MSG("[S29GLWrite]: data double check error @ 0x%08x ...\n",
                      (UINT32)unaligned);
        return flWriteFault;
        }

    return flOK;
    }

/*******************************************************************************
*
* S29GLErase - erase routine for S29GL flash
*
* This routine is called to erase sector(s) of the flash part.
*
* RETURNS: flOK or flTimedOut if timeout.
*
* ERRNO: N/A
*/

LOCAL FLStatus s29GlErase
    (
    FLFlash vol,
    int     firstErasableBlock,
    int     numOfErasableBlocks
    )
    {
    S29GL_FLASH_ADRS sector = NULL;
    S29GL_FLASH_DATA data1, data2;
    UINT32 timeout = 0;
    int i;

    if (flWriteProtected(vol.socket))
        return flWriteProtect;

    for (i = firstErasableBlock;
         i < firstErasableBlock + numOfErasableBlocks;
         i++)
        {
        sector = (S29GL_FLASH_ADRS)vol.map(&vol, i * vol.erasableBlockSize, 0);

        S29GL_DBG_MSG("Erasing sector#%03d @ 0x%08x ...\r", i, (UINT32)sector);
        S29GL_ERASE(sector);

        /* set timeout = 5s */

        timeout = flMsecCounter + S29GL_OP_TIMEOUT;

        data1 = *sector;
        do
            {
            data2 = data1;
            data1 = *sector;
            if (flMsecCounter >= timeout)
                {
                S29GL_RESET(sector);
                return flTimedOut;
                }
            } while ((data1 & S29GL_DQ6_MASK) != (data2 & S29GL_DQ6_MASK));
        }

    S29GL_RESET(sector);
    return flOK;
    }


/*******************************************************************************
*
* s29GlIdentify - identify routine for S29GL flash
*
* This routine is called to probe the flash part.
*
* RETURNS: flOK or flUnknownMedia if unknown flash probed.
*
* ERRNO: N/A
*/

FLStatus s29GlIdentify
    (
    FLFlash vol
    )
    {
    S29GL_FLASH_ADRS base = (S29GL_FLASH_ADRS)vol.map(&vol, 0, 0);
    S29GL_FLASH_DATA vendor, device;

    S29GL_DBG_MSG("Entering s29GlIdentify routine @ base address 0x%08x ...\n",
                  (UINT32)base);

    /* check the flash id */

    S29GL_READ_ID(base);
    vendor = (UINT8)(*(base + S29GL_VENDOR_OFFSET));
    device = (UINT8)(*(base + S29GL_DEVICE_OFFSET));
    if ((vendor == S29GL_VENDOR_ID) && (device == S29GL_DEVICE_ID))
        {
        vol.type = S29GL_FLASH_ID;
        }
    else
        {
        S29GL_DBG_MSG("Unknown flash device: vendor = %x, device = %x\n",
                      vendor, device);
        S29GL_RESET(base);
        return flUnknownMedia;
        }

    vol.chipSize = /*S29GL_FLASH_SIZE*/TFFS_FLASH_SIZE;
    vol.noOfChips = S29GL_FLASH_NUM;
    vol.interleaving = S29GL_FLASH_NUM;
    vol.erasableBlockSize = S29GL_SECTOR_SIZE * vol.interleaving;
    vol.write = s29GlWrite;
    vol.erase = s29GlErase;

    S29GL_RESET(base);
    return flOK;
    }
#else
/* JEDEC ids for this MTD */
#define Am29GL01GP_FLASH     0x017E  /* add by ff 2010.06.11*/
/* commands for AMD flash */
#define SETUP_ERASE          0x0080
#define SETUP_WRITE          0x00a0
#define READ_ID              0x0090
#define SUSPEND_ERASE        0x00b0
#define SECTOR_ERASE         0x0030
#define RESUME_ERASE         0x0030
#define READ_ARRAY           0x00f0
#define INTEL_READ_ARRAY     0x00ff

/* AMD unlocking sequence */

#define UNLOCK_1             0x00aa  /*aaaa */
#define UNLOCK_2             0x0055  /* 5555   */
#define UNLOCK_ADDR1         0x555		 /*5555   */
#define UNLOCK_ADDR2         0x2aa		 /*2aa    */

/* AMD flash status bits */
#define D2                   0x04    /* Toggles when erase suspended */
#define D5                   0x20    /* Set when programming timeout */
#define D6                   0x40    /* Toggles when programming     */

#define VUINT16 volatile unsigned short
#define wFlashPtr    ((VUINT16*) flashPtr)
#define wBuffer      ((const VUINT16 *) buffer)  /*  */
#define wUnlockAddr1 ((VUINT16*) unlockAddr1)
#define wUnlockAddr2 ((VUINT16*) unlockAddr2)

/* add by ff 2010.08.03  begin*/
/*#define TEST*/
#define   clkReg    0xe1000064
extern int sysGetCpuID(void);
/*int getFlashWLock();
void  freeFlashLock();*/
/* add by ff 2010.08.03  end*/

/* -------------------------------------------------------------------- */
/* AMD Flash specific data. Pointed by FLFlash.mtdVars                  */
/* -------------------------------------------------------------------- */
        typedef struct
                {
                  FlashDPTR  unlockAddr1;
                  FlashDPTR  unlockAddr2;                   
                } Vars;
                
static Vars  mtdVars[DRIVES];

#define thisVars   ((Vars *) vol.mtdVars)


static FLStatus flwAmdMTDErase (FLFlash  vol,
                                int      firstBlock,
                                int      numOfBlocks);
/* -------------------------------------------------------------------- */
/*               f l U p d a t e U n l o c k A d d r s                  */
/*									*/
/* Updates unlock addresses for AMD flash media.                        */
/*									*/
/*									*/
/* Parameters:                                                          */
/*	vol		: Pointer identifying drive			*/
/*      basePtr         : flash pointer, could point anywhere in the    */
/*                        memory window                                 */
/*									*/
/* -------------------------------------------------------------------- */
LOCAL void FAR0* amd29glMap
	(
	FLFlash* vol,
    CardAddress address,
    int length
    )
    {
    UINT32 flashBaseAddr = (vol->socket->window.baseAddress<< 12);     
    void FAR0* pFlash = (void FAR0*) ( FLASH_TFFS_ADRS + address);   
	return(pFlash);
    }
static void flUpdateUnlockAddrs (FLFlash    vol,VUINT16*  basePtr)
{
    thisVars->unlockAddr1 = (VUINT16*)(((long)basePtr & -0x10000l)) + 0x555;
    thisVars->unlockAddr2 = (VUINT16*)(((long)basePtr & -0x10000l)) + 0x2aa;
}
/*----------------------------------------------------------------------*/
/*                   f l w A m d C o m m a n d                          */
/*									*/
/* Writes an AMD command with the required unlock sequence		*/
/*									*/
/* Parameters:                                                          */
/*	vol		: Pointer identifying drive			*/
/*      address		: Card address at which to write command	*/
/*	command		: command to write				*/
/*                                                                      */
/*----------------------------------------------------------------------*/
static void flwAmdCommand (FLFlash         vol,
                           CardAddress     address,
                           unsigned long  command)
{    
  if (thisVars->unlockAddr1)
  {
    thisVars->unlockAddr1[0] = UNLOCK_1;
    thisVars->unlockAddr2[0] = UNLOCK_2;
    thisVars->unlockAddr1[0] = command;
  }
  else
  {
    CardAddress  baseAddress  = address & (-0x10000l);

    /* write AMD unlock sequence */
    
    *(FlashDPTR) vol.map (&vol,
                          baseAddress + 1 * UNLOCK_ADDR1,
                          vol.interleaving) = UNLOCK_1;
    *(FlashDPTR) vol.map (&vol,
                          baseAddress + 1 * UNLOCK_ADDR2,
                          vol.interleaving) = UNLOCK_2;

    /* write flash command */
    *(FlashDPTR) vol.map (&vol,
                          baseAddress + 1 * UNLOCK_ADDR1,
                          vol.interleaving) = command;
    vol.map (&vol, address, vol.interleaving);
  }

}
/*----------------------------------------------------------------------*/
/*                f l w A m d M T D W r i t e                           */
/*									*/
/* Write a block of bytes to Flash					*/
/*									*/
/* This routine will be registered as FLFlash.write routine             */
/*									*/
/* Parameters:                                                          */
/*	vol		: Pointer identifying drive			*/
/*      address		: Card address to write to			*/
/*      buffer		: Address of data to write			*/
/*	length		: Number of bytes to write			*/
/*	overwrite	: TRUE if overwriting old Flash contents	*/
/*			  FALSE if old contents are known to be erased	*/
/*                                                                      */
/* Returns:                                                             */
/*      FLStatus        : 0 on success, failed otherwise                */
/*----------------------------------------------------------------------*/
static FLStatus flwAmdMTDWrite (FLFlash           vol,
                                CardAddress       address,
                                const void FAR1  *buffer,   
                                int               length,
                                FLBoolean         overwrite)
{
  UINT32     uiDlyTime = 0;
  UINT32     uiLoop = 0;
  int           cLength;
  static STATUS   status=flOK;
  VUINT16 *flashPtr = NULL;
  VUINT16 *unlockAddr1 = NULL;
  VUINT16 *unlockAddr2 = NULL;


  if ( flWriteProtected(vol.socket) )
   {
        return flWriteProtect;
   }

    if(flWriteFault == status)
    {
        return flWriteFault;
    }
  unlockAddr1 = (VUINT16*)thisVars->unlockAddr1;
  unlockAddr2 = (VUINT16*)thisVars->unlockAddr2;
    
  flashPtr = (VUINT16*) vol.map (&vol, address, length);
  /* set addresses for AMD unlock sequence */
  flUpdateUnlockAddrs (&vol, flashPtr);
  unlockAddr1 = thisVars->unlockAddr1;
  unlockAddr2 = thisVars->unlockAddr2;

#if 1   
  printf("flashPtr=0x%08x,address=0x%08x,length=0x%08x\n",flashPtr,address,length); 
#endif
  /* write the data */
  cLength = length;
  while (cLength >= 2)
  {
    /* AMD unlock sequence */
    uiDlyTime = 0;

	*wUnlockAddr1 = UNLOCK_1;
    *wUnlockAddr2 = UNLOCK_2;
    /* flash command */
    *wUnlockAddr1 = SETUP_WRITE;
    /* write user data word */
    *wFlashPtr    = *wBuffer;  

    /* wait for ready */
    while (*wFlashPtr != *wBuffer)
    {
      if(uiDlyTime > 800000)
      {
        *wFlashPtr = READ_ARRAY;
        return flWriteFault;
      }
      for(uiLoop = 0;uiLoop < 10;uiLoop++);
      uiDlyTime++;
    }
    
    cLength -= 2;
    buffer = (unsigned char *)buffer + sizeof(unsigned short);
    flashPtr = flashPtr + 1;
  }
  /* verify the written data */
  flashPtr -= length / 2;
  buffer = (unsigned short *)buffer - (length/2);
  
  if (tffscmp(flashPtr, buffer, length))
  {
     status = flWriteFault;
     return status;
  }
  
  return flOK;
}


INT32 flwWait(VUINT16 *puiSectorAddr,UINT16 uiVal)
{
	UINT32  uiLoop = 0;
    UINT32  uiTime = 0;
	
	for(uiLoop = 0;uiLoop < 0x200000; uiLoop++)
	{
        if(*puiSectorAddr == uiVal)
        {
            return OK;
        }
        for(uiTime = 0;uiTime < 100;uiTime++);
	}

    return ERROR;
}


/*----------------------------------------------------------------------*/
/*               f l w A m d M T D E r a s e                            */
/*									*/
/* Erase one or more contiguous Flash erasable blocks			*/
/*									*/
/* This routine will be registered as FLFlash.erase routine             */
/*									*/
/* Parameters:                                                          */
/*      vol                : Pointer identifying drive                  */
/*      firstBlock         : Number of first block to erase             */
/*      numOfBlocks        : Number of blocks to erase                  */
/*                                                                      */
/* Returns:                                                             */
/*      FLStatus           : 0 on success, failed otherwise             */
/*----------------------------------------------------------------------*/

static FLStatus flwAmdMTDErase (FLFlash  vol,
                                int      firstBlock,
                                int      numOfBlocks)
{
    int        iBlock;
    VUINT16   *puiBaseAddr = (VUINT16*)FLASH_BASE_ADRS;
    VUINT16   *puiSectorAddr = NULL;
    FlashDPTR  flashPtr;
    
    for (iBlock = 0;  iBlock < numOfBlocks;  iBlock++)
    {
        flashPtr = (FlashDPTR) vol.map( &vol,
                                (firstBlock + iBlock) * vol.erasableBlockSize,
                                 vol.interleaving);
        puiSectorAddr =(VUINT16*)flashPtr;
   
        *(puiBaseAddr + 0x555) = 0x00aa;
        *(puiBaseAddr + 0x2aa) = 0x0055;
        *(puiBaseAddr + 0x555) = 0x0080;
        *(puiBaseAddr + 0x555) = 0x00aa;
        *(puiBaseAddr + 0x2aa) = 0x0055;
	
        *puiSectorAddr = 0x0030;
    
        if(OK != flwWait(puiSectorAddr,0xffff))
        {
            return ERROR;
        }
    	
        *puiBaseAddr = 0x00f0;
    }
  return flOK;
}


/*----------------------------------------------------------------------*/
/*                     f l w A m d I d e n t i f y                      */
/*									*/
/* Identify the AMD Flash media.                                        */
/* Sets the value of flash.type (JEDEC id) & FLFlash.interleaving.	*/
/*									*/
/* Parameters:                                                          */
/*      vol             : Pointer identifying drive                     */
/*	amdCmdRoutine	: Routine to read-id AMD/Fujitsu style at	*/
/*			  a specific location. If null, Intel procedure	*/
/*			  is used.                                      */
/*      idOffset	: Chip offset to use for identification		*/
/*                                                                      */
/*----------------------------------------------------------------------*/
void flwAmdIdentify (FLFlash      vol,
                            void (*amdCmdRoutine)(FLFlash vol, CardAddress, unsigned long),
                            CardAddress  idOffset)
{ 
  vol.type = 0x017e;
}


/*----------------------------------------------------------------------*/
/*                       f l w A m d S i z e                            */
/*									*/
/* Identify the card size for AMD Flash.                                */
/* Sets the value of flash.noOfChips.					*/
/*									*/
/* Parameters:                                                          */
/*	vol		: Pointer identifying drive			*/
/*	amdCmdRoutine	: Routine to read-id AMD/Fujitsu style at	*/
/*			  a specific location. If null, Intel procedure	*/
/*			  is used.                                      */
/*      idOffset	: Chip offset to use for identification		*/
/*                                                                      */
/* Returns:                                                             */
/*      FLStatus        : 0 = OK, otherwise failed (invalid Flash array)*/
/*----------------------------------------------------------------------*/
FLStatus flwAmdSize (FLFlash      vol,
                   void (*amdCmdRoutine)(FLFlash vol, CardAddress, unsigned long),
                   CardAddress  idOffset)
{
  vol.noOfChips = 1;
  return (vol.noOfChips == 0) ? flUnknownMedia : flOK;
}

/*----------------------------------------------------------------------*/
/*              f l w A m d M T D I d e n t i f y                       */
/*									*/
/* Identifies AMD flash media accessible in 16-bit mode only.           */
/*									*/
/* This routine will be placed on the MTD list in CUSTOM.C. It must be  */
/* an extern routine.							*/
/*									*/
/* On successful identification, the Flash structure is filled out and	*/
/* the write and erase routines registered.				*/
/*									*/
/* Parameters:                                                          */
/*	vol		: Pointer identifying drive			*/
/*                                                                      */
/* Returns:                                                             */
/*      FLStatus        : 0 = OK, otherwise failed (invalid Flash array)*/
/*----------------------------------------------------------------------*/

FLStatus s29GlIdentify (FLFlash vol)
{    
  FlashDPTR  baseFlashPtr;
  flSetWindowBusWidth (vol.socket,16);      /* use 16-bits */
  flSetWindowSpeed (vol.socket,120);        /* 120 nsec.   */
  
  /* place to store all the AMD-specific flash parametes */
  vol.mtdVars = &mtdVars[flSocketNoOf(vol.socket)];

  thisVars->unlockAddr1 = NULL;

  vol.interleaving = 1;

  flwAmdIdentify (&vol, flwAmdCommand, 0);
  
  if (vol.type ==Am29GL01GP_FLASH )
  {
	 vol.chipSize = 0x4000000l * vol.interleaving; /*0x7c00000l*/
  }
  else
  {	  
    return flUnknownMedia;
  }

  /* find where the window is */
  baseFlashPtr = (FlashDPTR)
                    vol.map (&vol, (CardAddress)0, vol.interleaving);
  flUpdateUnlockAddrs (&vol, baseFlashPtr);

  checkStatus( flwAmdSize(&vol, flwAmdCommand, 0) );

  if (vol.type ==Am29GL01GP_FLASH )   /* add by ff 2010.06.12 */ 
  {
  vol.erasableBlockSize = 0x20000l * vol.interleaving; 
  vol.map = (void FAR0 *)amd29glMap;
  }
  else	
	  vol.erasableBlockSize = 0x10000l * vol.interleaving; 
  vol.flags            |= SUSPEND_FOR_WRITE;

  /* Register our flash handlers */
  vol.write = flwAmdMTDWrite;
  vol.erase = flwAmdMTDErase;
  
  return flOK;
}

#endif
#if 0
INT32 sysFlashTest(UINT32 len )
{
	UINT32 uiLoop = 0;
	S29GL_FLASH_ADRS sector = NULL;
	UINT32 time =0;

	sector = 0xf2000000;
	
	for(uiLoop = 0; uiLoop < len/0x20000;uiLoop ++)
	{
		sector += 0x20000;
		S29GL_ERASE(sector);
	}
}
#endif
