
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
#include "vxbTimerLib.h"
#if 1
/* JEDEC ids for this MTD */
#define Am29GL01GP_FLASH     0x017E  /* add by ff 2010.06.11*/
/* commands for AMD flash */
#define SETUP_ERASE          0x00800080
#define SETUP_WRITE          0x00a000a0
#define READ_ID              0x00900090
#define SUSPEND_ERASE        0x00b000b0
#define SECTOR_ERASE         0x00300030
#define RESUME_ERASE         0x00300030
#define READ_ARRAY           0x00f000f0
#define INTEL_READ_ARRAY     0x00ff00ff

/* AMD unlocking sequence */

#define UNLOCK_1             0x00aa00aa  /*aaaa */
#define UNLOCK_2             0x00550055  /* 5555   */
#define UNLOCK_ADDR1         0x555		 /*5555   */
#define UNLOCK_ADDR2         0x2aa		 /*2aa    */

/* AMD flash status bits */
#define D2                   0x04    /* Toggles when erase suspended */
#define D5                   0x20    /* Set when programming timeout */
#define D6                   0x40    /* Toggles when programming     */

#define WRITE_ADDR_MASK			3
#define WRITE_BYTE_ONCE			4
#define DQ5                   0x00200020    /* Set when programming timeout */
#define DQ7                   0x00800080 
#define DQ1					 0x00010001
#define BUFWR_MAX_BYTE	     0x400
#define BUFWR_ADDR_MASK	     0x3FF

#ifdef __cplusplus
  #define wFlashPtr    ((FlashWPTR &) flashPtr)
  #define wBuffer      ((const unsigned short FAR1 * &) buffer)
  #define wUnlockAddr1 ((FlashWPTR &) unlockAddr1)
  #define wUnlockAddr2 ((FlashWPTR &) unlockAddr2)
#else
  #define wFlashPtr    ((FlashDPTR) flashPtr)
  #define wBuffer      ((const unsigned long FAR1 *) buffer)  /*  */
  #define wUnlockAddr1 ((FlashDPTR) unlockAddr1)
  #define wUnlockAddr2 ((FlashDPTR) unlockAddr2)
#endif 

/* add by ff 2010.08.03  begin*/
#define BUF_WRITE
//#define DEBUG_PRINTF
//#define   clkReg   0xe1000064
extern int sysGetCpuID(void);
STATUS getFlashWLock();
STATUS  freeFlashLock();
void bufWrCommand( unsigned int secAddr, unsigned int size);
void bufWrComfirm(unsigned int secAddr);
STATUS waitForReady(FlashDPTR srcAddr,FlashDPTR aligned);
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

extern unsigned int  flash_baseAddr;  /* add by ff 2010.07.27  */
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
    void FAR0* pFlash = (void FAR0*) ( flash_baseAddr+ address);  
	return(pFlash);
    }
static void flUpdateUnlockAddrs (FLFlash    vol,
                                 FlashDPTR  basePtr)
{
    thisVars->unlockAddr1 = (FlashDPTR)(((long)basePtr & -0x10000l)) + 0x555;
    thisVars->unlockAddr2 = (FlashDPTR)(((long)basePtr & -0x10000l)) + 0x2aa;
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
                          baseAddress + 2 * UNLOCK_ADDR1,
                          vol.interleaving) = UNLOCK_1;
    *(FlashDPTR) vol.map (&vol,
                          baseAddress + 2 * UNLOCK_ADDR2,
                          vol.interleaving) = UNLOCK_2;

    /* write flash command */
    *(FlashDPTR) vol.map (&vol,
                          baseAddress + 2 * UNLOCK_ADDR1,
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
  unsigned long writeTimeout;
  int           cLength; 
 unsigned int  sectorAddr;
 int i=0,j=0 ,left=0;
 UINT32 WC=0,WC1=0;
 
 UINT8  *unaligned;
 FlashDPTR aligned;
  UINT32 data;
  UINT32  num;
  UINT8 * buf  = (UINT8 *)buffer;

  STATUS status=flOK;
  if ( flWriteProtected(vol.socket) )
   {
 		printf("flwAmdMTDWrite:flWriteProtect\n");
     return flWriteProtect;
   }

  /* Set timeout to 5 seconds from now */
  writeTimeout = flMsecCounter + 5000;  
  
#ifdef DEBUG    	
    	printf("flashPtr=0x%08x,address=0x%08x,length=0x%08x\n",flashPtr,address,length); 
#endif
    	
    cLength = length;
    unaligned = (UINT8*) vol.map (&vol, address, length);  
    num = (UINT32)unaligned & WRITE_ADDR_MASK;
    aligned = (FlashDPTR)((UINT32)unaligned - num);
#ifdef DEBUG_PRINTF
   printf("aligned=0x%08x,address=0x%08x,length=0x%08x\n",aligned,address,length); 
#endif	 
    sectorAddr = (UINT32)aligned&0xFFFFC000;
    num = (UINT32)aligned & WRITE_ADDR_MASK;
	 if (num != 0)  //地址非4byte对齐，调整
	 {
		 printf("num=%d,aligned = 0x%x\n",num,aligned);
	      data = *aligned;

	      for (i = num ; i < WRITE_BYTE_ONCE; i++)
	      {
	            data &= ~(0xFF << (((WRITE_BYTE_ONCE - 1) - i) * 8));
	            data |= ((*(buf + i - num)) << (((WRITE_BYTE_ONCE - 1) - i) * 8));
	      }
	      cLength  +=  num;
	  }
	  left = cLength%4;   
	  if(cLength >= WRITE_BYTE_ONCE)
	  {
		if(((UINT32)aligned& BUFWR_ADDR_MASK)!=0)  //非1KB地址对齐
		{
			if((((UINT32)aligned & BUFWR_ADDR_MASK) +  cLength)<=BUFWR_MAX_BYTE)  //在同一line
			{
				if(left !=0)
				{
					 WC1 = (cLength - left)/4-1;
					cLength = left; 
				}
				else
					WC1 = cLength/4-1;
				
	#ifdef DEBUG_PRINTF
			 printf("in same Line,WC1 = 0x%x Byte,startAddress = 0x%x\n",WC1,aligned);
	#endif
		
			WC = (UINT32)((WC1<<16)|WC1);
				/* buffer write command */
			bufWrCommand(sectorAddr,WC); 
			while(1)
			{		
				if(num!=0)  //地址非4B对齐时，第一个byte是时做过处理的，非直接由源数据传来，需单独处理
				{
					printf("num!=0;WC=0x%x\n",WC);
				    num = 0;
					*aligned = data;
					aligned ++;
					buf += (WRITE_BYTE_ONCE - num);
					if(WC1==0) 
					{
						bufWrComfirm(sectorAddr);//goto comfirm;
						break;
					}
					WC1--;
					}
				else
				{
					*aligned = *(FlashDPTR)buf;	
					aligned ++;
					buf += 4; 
					if(WC1==0) 
					{
						bufWrComfirm(sectorAddr);
						break;
					}
					else
					 WC1--;
				}
			}
				/* waite finished*/
				if(waitForReady((FlashDPTR)buf,(FlashDPTR)aligned)!=OK) //goto waitForReady;	
					return ERROR;
				
				if(left!=0)   //剩余小于4B的数据，单独处理（字操作）
				{
					cLength = 0;
					goto singleWord;
				}
				
				return OK;
			}
			else  //非同一Line
			{	

				/* buffer write command */
				cLength = cLength -(BUFWR_MAX_BYTE- ((UINT32)aligned& BUFWR_ADDR_MASK));
				 WC1 = (BUFWR_MAX_BYTE-((UINT32)aligned&BUFWR_ADDR_MASK))/4-1;
				 WC = (UINT32)((WC1<<16)|WC1);
	#ifdef DEBUG_PRINTF
			 printf("not the same Line,write size = 0x%x,startAddress=0x%x,sectorAddr=0x%x,WC = 0x%x\n",
					 BUFWR_MAX_BYTE-((UINT32)aligned&BUFWR_ADDR_MASK),aligned,sectorAddr,WC);
	#endif
				bufWrCommand(sectorAddr,WC);
				
				/* write data to buffer*/
				while(1)
				{		if(num!=0)  //地址非4B对齐时，第一个byte是时做过处理的，非直接由源数据传来，需单独处理
						{
	//						printf("num!=0;WC=0x%x\n",WC);
							num = 0;
							*aligned = data;
							aligned ++;
							buf += (WRITE_BYTE_ONCE - num);
							if(WC1==0) 
							{
								bufWrComfirm(sectorAddr);//goto comfirm;
								break;
							}
							WC1--;
						}
						else
						{
						//	printf("num==0;WC=0x%x\n",WC);
							*aligned = *(FlashDPTR)buf;	
							aligned ++;
							buf += 4; 
							if(WC1==0) 
							{
								bufWrComfirm(sectorAddr);//goto comfirm;
								break;
							}
							else
							 WC1--;
	//						goto loop;
						}
				}
				/* waite finished*/
				if(waitForReady((FlashDPTR)buf,aligned)!=OK) //goto waitForReady;	
					return ERROR;
				sectorAddr = (UINT32)aligned&0xFFFFC000;
			}
		}

				while(cLength>=BUFWR_MAX_BYTE)   //1KB地址对齐 ,满buf操作
				{
	#ifdef DEBUG_PRINTF
			 printf("1k address aligned,startAddress=0x%x,sectorAddr=0x%x\n",
					 aligned,sectorAddr);
	#endif	
					/* buffer write command */
					 WC1 = BUFWR_MAX_BYTE/4-1;
					 WC = (UINT32)((WC1<<16)|WC1);
					bufWrCommand(sectorAddr,WC);  //BUFWR_MAX_BYTE/4-1
					/* write data to buffer*/
					while(1)
					{		
						if(num!=0)  //地址非4B对齐时，第一个byte是时做过处理的，非直接由源数据传来，需单独处理
						{
							printf("num!=0;WC=0x%x\n",WC);
							num = 0;
							*aligned = data;
							aligned ++;
							buf += (WRITE_BYTE_ONCE - num);
							if(WC1==0) 
							{
								bufWrComfirm(sectorAddr);//goto comfirm;
								break;
							}
							WC1--;
						}
						else
						{
							*aligned = *(FlashDPTR)buf;	
							aligned ++;
							buf += 4; 
							if(WC1==0) 
							{
								bufWrComfirm(sectorAddr);//goto comfirm;
								break;
							}
							else
								WC1--;
						}
					}
								/* waite finished*/
					if(waitForReady((FlashDPTR)buf,aligned)!=OK) //goto waitForReady;	
							return ERROR;
					cLength = cLength-BUFWR_MAX_BYTE;
					sectorAddr = (UINT32)aligned&0xFFFFC000;
	#ifdef DEBUG_PRINTF
			 printf("cLength = 0x%x,sectorAddr =0x%x\n", cLength,sectorAddr);
	#endif
				}
				if(cLength != left)  //remainBytes>left
				{	
	#ifdef DEBUG_PRINTF
			 printf("remainBytes != left,cLength = 0x%x,sectorAddr =0x%x\n", cLength,sectorAddr);
	#endif
						/* buffer write command */
					WC1 = (cLength-left)/4-1;
				 	WC = (UINT32)((WC1<<16)|WC1);
//					printf("WC =0x%x\n",WC);
					bufWrCommand(sectorAddr,WC);
						/* write data to buffer*/
					while(1)
					{		
						if(num!=0)  //地址非4B对齐时，第一个byte是时做过处理的，非直接由源数据传来，需单独处理
						{
	//						printf("num!=0;WC=0x%x\n",WC);
							num = 0;
							*aligned = data;
							aligned ++;
							buf += (WRITE_BYTE_ONCE - num);
							if(WC1==0) 
							{
								bufWrComfirm(sectorAddr);//goto comfirm;
								break;
							}
							WC1--;
						}
						else
						{
	//						printf("num==0;WC=0x%x,aligned=0x%x,srcAddr=0x%x\n",WC,aligned,srcAddr);
							*aligned = *(FlashDPTR)buf;	
							aligned ++;
							buf += 4; 
							if(WC1==0) 
							{
								bufWrComfirm(sectorAddr);//goto comfirm;
								break;
							}
							else
							 WC1--;
						}
					}
					/* waite finished*/
					if(waitForReady((FlashDPTR)buf,aligned)!=OK) //goto waitForReady;	
					return ERROR;
					cLength = left;
				}
	   }

	 if(left!=0)
		goto singleWord;
	 else
		 return OK;
		singleWord:
		{
					data = *aligned;
	#ifdef DEBUG_PRINTF
			 printf("singleWord:data1 = 0x%x,sectorAddr =0x%x\n", data,sectorAddr);
	#endif
					for (i = 0 ; i < left; i++)
					{
							 data &= ~(0xFF << (((4 - 1) - i) * 8));
							 data |= ((*(buf + i)) << (((4 - 1) - i) * 8));
					}
	#ifdef DEBUG_PRINTF
			 printf("singleWord:data2 = 0x%x,sectorAddr =0x%x\n", data,sectorAddr);
	#endif
					/* Program 32 bits */   
					*((UINT32*) sectorAddr + 0x555) = 0x00aa00aa;
					*((UINT32*) sectorAddr + 0x2aa) = 0x00550055;
					/* flash command */
					*((UINT32*) sectorAddr + 0x555) = 0x00a000a0;    	      
					*aligned = data; 
				      while ((*aligned != data)    && (flMsecCounter < writeTimeout))
				      {
				          if( ((*aligned &          D5)  && ((*aligned ^ data) &   0xffff))          
				              ||
				             ((*aligned & (0x10000 * D5)) && ((*aligned ^ data) & 0xffff0000))
				             )
				          {
				           *aligned = READ_ARRAY;
				          status = flWriteFault;
				           return flWriteFault;
				          }
				       } 

		}  
		return OK;
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
  FlashDPTR  flashPtr;
  FLBoolean  finished;
  int        iBlock;
//  int i;
  if ( flWriteProtected(vol.socket) )
  {
    return flWriteProtect;
  }
  for (iBlock = 0;  iBlock < numOfBlocks;  iBlock++)
  {
    flashPtr = (FlashDPTR) vol.map( &vol,
                                       (firstBlock + iBlock) * vol.erasableBlockSize,
                                        vol.interleaving);
    flUpdateUnlockAddrs (&vol, flashPtr);
    flwAmdCommand (&vol, 0, SETUP_ERASE);
    flwAmdCommand (&vol, 0, SECTOR_ERASE);
    do {
#ifdef BACKGROUND
      while (flForeground(1) == BG_SUSPEND) { /* suspend */
        for (i = 0; i < interleave; i++) {
	  flashPtr[i] = SUSPEND_ERASE;
	  /* Wait for D6 to stop toggling */
	  while ((flashPtr[i] ^ flashPtr[i]) & D6)
	    ;
	}
      } /* while (foreground..) */
#endif

      finished = TRUE;
 	  if (*wFlashPtr != 0xffffffff)
      {
        if( ((*wFlashPtr &   0xffff &          D5)  && ((*wFlashPtr &   0xffff) !=   0xffff))           
            ||
            ((*wFlashPtr & 0xffff0000 & (0x10000 * D5)) && ((*wFlashPtr & 0xffff0000) != 0xffff0000))
            )
        {
          *wFlashPtr = READ_ARRAY;
          return flWriteFault;
        }
        finished = FALSE;
//        for(i=0;i<0x1000;i++);
    }      
    } while (!finished);
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
#if 0
  unsigned long  resetCmd = amdCmdRoutine ? READ_ARRAY : INTEL_READ_ARRAY;
  FlashDPTR       flashPtr = (FlashDPTR) vol.map (&vol, idOffset, 0);
  if (amdCmdRoutine)             /* use AMD unlock sequence      */
  {
    flUpdateUnlockAddrs(&vol, flashPtr);
    amdCmdRoutine (&vol, idOffset, READ_ID);
  }
  else
    flashPtr[0] = READ_ID;       /* use Intel-style              */

  /* We left the first chip in Read ID mode, so that we can      */
  /* discover an address wraparound.                             */
  for (vol.noOfChips = 1;  vol.noOfChips < 2000;  vol.noOfChips++)
  {
    flashPtr = (FlashDPTR)
                    vol.map (&vol, vol.noOfChips * vol.chipSize + idOffset, 0);
    if ((FlashType)(((((flashPtr[0]&0x00ff)<<8)) | (flashPtr[1] & 0xff)) == vol.type))   
          break;
    /* Check if chip displays the same JEDEC id and interleaving */
    if (amdCmdRoutine)           /* AMD: use unlock sequence     */
    {
      flUpdateUnlockAddrs (&vol, flashPtr);
      amdCmdRoutine (&vol,
                     vol.noOfChips * vol.chipSize + idOffset,
                     READ_ID);
    }
    else
      flashPtr[0] = READ_ID;     /* use Intel-style              */  
    	if ((FlashType) ((((flashPtr[0]&0x00ff)<<8) & 0xff00) | (flashPtr[1] & 0xff)) != vol.type)
    	{
    		/* This "chip" doesn't respond correctly, so we're done */
    		break;
    	 }

    flashPtr[0] = resetCmd;
  }
  flashPtr = (FlashDPTR) vol.map (&vol, idOffset, 0);
  flashPtr[0] = resetCmd;        /* reset the original chip      */
#endif
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

  flSetWindowBusWidth (vol.socket,32);      /* use 16-bits */
  flSetWindowSpeed (vol.socket,120);        /* 120 nsec.   */
  
  /* place to store all the AMD-specific flash parametes */
  vol.mtdVars = &mtdVars[flSocketNoOf(vol.socket)];

  thisVars->unlockAddr1 = NULL;

  vol.interleaving = 2;

  flwAmdIdentify (&vol, flwAmdCommand, 0);
  
  if (vol.type ==Am29GL01GP_FLASH )
  {
	 vol.chipSize = SINGLE_FLASH_AVAILABLE_SIZE * vol.interleaving; 
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

  vol.erasableBlockSize = 0x20000l * vol.interleaving; 
  vol.map = (void FAR0 *)amd29glMap;
  vol.flags            |= SUSPEND_FOR_WRITE;

  /* Register our flash handlers */
  vol.write = flwAmdMTDWrite;
  vol.erase = flwAmdMTDErase;
  return flOK;
}
#endif
STATUS getFlashWLock()
{
	int cpuID;
	UINT32 regValue;
	cpuID = sysGetCpuID();
	regValue = (cpuID+1);
	
	*(UINT8*)(FLASH_LOCK_REG) = (cpuID+1);
	regValue = *(UINT8*)(FLASH_LOCK_REG);
	if(regValue == (cpuID+1)) return OK;
	else return ERROR;
}
STATUS freeFlashLock()
{
	int cpuID;
	UINT32 regValue;
	cpuID = sysGetCpuID();
	regValue = *(UINT8*)(FLASH_LOCK_REG) ;
	if(regValue == 0) return OK;
	else
	{
		*(UINT8*)(FLASH_LOCK_REG) = (cpuID+1);
		regValue = *(UINT8*)(FLASH_LOCK_REG) ;
		if(regValue == 0) return OK;
		else return ERROR;
	}
}
void bufWrCommand( unsigned int secAddr, unsigned int size)
{
	*(( unsigned int *)secAddr + 0x555) = 0x00aa00aa;
	*(( unsigned int *)secAddr + 0x2aa) = 0x00550055;
	*( unsigned int *)secAddr = 0x00250025;
	*( unsigned int *)secAddr = size;
}
void bufWrComfirm(unsigned int secAddr)
{
	*( unsigned int *)secAddr = 0x00290029;
}
STATUS waitForReady(FlashDPTR srcAddr,FlashDPTR aligned)
{
	UINT32 srcdata,data,i;
	unsigned long writeTimeout;
	  writeTimeout = flMsecCounter + 5000; 
/* wait for ready */
//   	printf("waitForReady:aligned = 0x%x,srcAddr = 0x%x\n",(aligned-1),(srcAddr-1));
//	  vxbMsDelay(1);
	  for(i=0;i<10000;i++);
	srcdata = *(srcAddr-1); //*(UINT32*)(srcAddr-4);
#if 0
    while ((*(aligned-1) != srcdata)    && (flMsecCounter < writeTimeout))
    {
        if( ((*(aligned-1) &          D5)  && ((*(aligned-1) ^srcdata) &   0xffff))          
            ||
           ((*(aligned-1) & (0x10000 * D5)) && ((*(aligned-1) ^ srcdata) & 0xffff0000))
           )
        {
         *(aligned-1) = READ_ARRAY;
         //         status = flWriteFault;
         return flWriteFault;
        }
     } 
#else
	while(1)
	{
		data = *(aligned-1);
   	//	if ((data & DQ7)==(srcdata&DQ7))
		if(data==srcdata)
   			return OK;
   		else 
   		{
   			if((data & DQ5) == DQ5)
   			{
//   				if ((*(UINT32*)(aligned-4) & D7) == (srcdata&D7)) 
   				if (*(aligned-1) == srcdata) 
   					return OK;
   				
   				else
   				{        			
   					*(aligned-1) = 0x00f000f0;
   					printf(" 1:buffer write failed,data = 0x%x,srcdata=0x%x\n",*(UINT32*)(aligned-1),srcdata);
   					return ERROR;
   				}
   			}	
   			else
   			{
   				if ((*(aligned-1) & DQ1) == DQ1) 
   				{
   				//	if ((*(UINT32*)(aligned-4) & D7) == (srcdata&D7)) 
   					if (*(aligned-1) == srcdata)
   						return OK;
   				
   					else
   					{
   						*(aligned-1) = 0x00f000f0;
   						printf(" 2:buffer write failed,data = 0x%x,srcdata = 0x%x\n",*(UINT32*)(aligned-1),srcdata);
   						return ERROR;
   					}
   				}

   			}
   		}
}
#endif
}

    void bufWrAbort(unsigned int secAddr)
    {
    	*(( unsigned int *)secAddr + 0x555) = 0x00aa00aa;
    	*(( unsigned int *)secAddr + 0x2aa) = 0x00550055;
    	*(( unsigned int *)secAddr + 0x555) = 0x00f000f0;
    }


#if	FALSE
/*----------------------------------------------------------------------*/
/*                 f l R e g i s t e r W A M D M T D                    */
/*									*/
/* Registers this MTD for use						*/
/*									*/
/* Parameters:                                                          */
/*	None								*/
/*                                                                      */
/*----------------------------------------------------------------------*/

FLStatus  flRegisterWAMDMTD (void)
{
  if (noOfMTDs >= MTDS)
    return flTooManyComponents;

  mtdTable[noOfMTDs++] = flwAmdMTDIdentify;

  return flOK;
}
#endif	/* FALSE */
