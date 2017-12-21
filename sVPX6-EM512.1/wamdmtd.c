
/************************************************************************/
/*                                                                      */
/*		FAT-FTL Lite Software Development Kit			*/
/*		Copyright (C) M-Systems Ltd. 1995-1996			*/
/*									*/
/************************************************************************/
/*                                                                      */
/*                    16-bit AMD MTD                                    */
/*                                                                      */
/* This MTD is for flash media which is accessible in 16-bit mode only. */
/* The following Flash technologies are supported:                      */
/*                                                                      */
/* - AMD     Am29F080   8-mbit devices                                  */
/* - AMD     Am29LV080  8-mbit devices                                  */
/* - AMD     Am29F016  16-mbit devices                                  */
/* - Fujitsu MBM29F080  8-mbit devices                                  */
/*									*/
/************************************************************************/

/*
 * $Log:   V:/wamdmtd.c_v  $
 * 
 *    Rev 1.1   07 Oct 1997 14:58:32   ANDRY
 * get rid of generic names, registration routine returns status
 *
 *    Rev 1.0   25 Aug 1997 12:00:00   ANDRY
 * Initial revision.
 */

#include "tffs\flflash.h"
#include "tffs\backgrnd.h"
#include "stdio.h"


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

#if 0    
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

FLStatus flwAmdMTDIdentify (FLFlash vol)
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

