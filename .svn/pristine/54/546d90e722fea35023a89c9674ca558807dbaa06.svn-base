/* flashMem.c - Flash memory device driver */

/*
    Copyright (c) 1993-1994, 1996-1998, 2000-2001, 2005-2007, 2008
    Wind River Systems, Inc.

The right to copy, distribute, modify or otherwise make use
of this software may be licensed only pursuant to the terms
of an applicable Wind River license agreement.
*/

/*
modification history
--------------------
01t,28mar08,rec  add FLASH_29WS064J
01s,23jan06,j_b  add 28FxxxC3 support
01r,28nov05,j_b  add 28FxxxK3/K18 support
01q,24aug04,h_k  returned the second sysFlashDataPoll() arg type to FLASH_DEF
                 (SPR #101284) and added pValTmp to eliminate the GNU
                 dereferencing type-punned pointer warnings.
01p,05may04,h_k  cleaned GNU dereferencing type-punned pointer warnings and
                 constant condition in for loop statement.
01o,15feb01,frf  merged latest contribution, added support for
                 28F128J3A, 28F320J3A, 28F640J3A and 29LV040B.
01n,23mar00,zl   added support for 29LV160B/T. added sysSectorErase().
01m,01feb00,jpd  added support for 28F320. SPR 31293
01l,09sep98,jpd  added support for 28F016SV; minor doc fixes.
01k,20apr98,jpd  moved back into main source tree from local copy in BSPs.
01j,24mar98,jpd  added support for 29LV1024 and 29C040A.
01i,04jul97,jpd  added support for 28F008 device, made overlay buffer dynamic.
01h,25jun96,map  added documentation, and some clean up.
01g,02apr96,tam  cast FLASH_ADRS and FLASH_SIZE to UINT32 to get rid off
                 a compiler warning.
01f,12feb96,kkk  made 1st param of sysFlashDataPoll() volatile to confirm
                 to dzb's change.
01e,26jan96,dzb  added volatile to register pointers.
           +jpb  changed delay parameters to be globals if not defined.
01d,24feb94,dzb  added sysFlashWrite{En,Dis}able(), sysFlashBoardDelay().
01c,15feb94,dzb  added SYS_FLASH_DELAY_SHIFT macro.
01b,07jan94,dzb  added support for 29F0X0 flash devices.
                 cut ties to NVRAM macros.  added support for FLASH_WIDTH.
01a,05oct93,dzb  derived from version 1b of mem/iFlashMem.c.
*/

/*
DESCRIPTION
This library contains routines to manipulate flash memory.  Read and write
routines are included.

The macro values FLASH_ADRS, FLASH_SIZE, and FLASH_WIDTH must be defined to
indicate the address, size (in bytes), and the data-access width (in bytes) of
the flash memory.

If the flash memory needs to be overlaid, and the section of the memory that
needs to be overlaid is less than FLASH_SIZE, then, for efficiency, define
FLASH_SIZE_WRITABLE to the size (in bytes) of the overlay section.

The routine sysFlashDelay() creates a delay for a specified number of
microseconds.  The timing loop can be adjusted on a board-dependent basis by
defining the function sysFlashBoardDelay and values for the following macros,
.iP
SYS_FLASH_DELAY_SHIFT
.iP
SYS_FLASH_DELAY_ADJ
.iP
SYS_FLASH_DELAY_INCR
.LP
To use the routine sysFlashBoardDelay(), the macro SYS_FLASH_BOARD_DELAY
should be defined.

The macro FLASH_NO_OVERLAY should be defined when calls to sysFlashSet()
are expected to erase the flash and reprogram it with only the new data.

The macro FLASH_LOCKED should be defined when block lock/unlock commands
are required to erase the flash or reprogram it with new data.

The macro SYS_FLASH_TYPE should be defined for flash devices that cannot be
auto-selected. This macro should be set to a flash device code defined in the
header files, <drv/mem/flash28.h> and <drv/mem/flash29.h>

To support flash devices that that need to turn on/off write protect features
(special programming voltages or other write-enable features), the macro
SYS_FLASH_WRITE, and the routines, sysFlashWriteEnable() and
sysFlashFlashWriteDisable() should be defined.

INTERNAL:
The FLASH_SIZE_WRITABLE concept doesn't work very well.  It just limits the
amount of flash that is writable, so why bother.  What it was really
intended to address was flash that is only block writable, i.e. you
can only write a complete block at a time.  To properly handle block
memory, you must copy the old block of memory to a buffer, update the
part of the buffer that is to be changed, and then write back the
complete buffer in a single write operation.

The accesses to non-volatile memory, and flash control registers needs
to be abstracted.  Macros should be used for all actual i/o operations.
*/

#include "drv/mem/flashDev.h"
//#include "tffs/flbase.h"

#define  FLASH_BLOCK_SIZE	0x40000
#ifndef FLASH_NO_OVERLAY
#  include <stdlib.h>
#endif /* FLASH_NO_OVERLAY */

/* defines */

/* Establish default values for DELAY parameters */

#ifndef SYS_FLASH_DELAY_SHIFT
#   define  SYS_FLASH_DELAY_SHIFT 0
#endif /*SYS_FLASH_DELAY_SHIFT*/

#ifndef SYS_FLASH_DELAY_ADJ
#   define  SYS_FLASH_DELAY_ADJ 0
#endif  /* SYS_FLASH_DELAY_ADJ */

#ifndef SYS_FLASH_DELAY_INCR
#   define  SYS_FLASH_DELAY_INCR 1
#endif  /* SYS_FLASH_DELAY_INCR */

/* Names of routines, or null values */

#ifdef  SYS_FLASH_WRITE
#   define SYS_FLASH_WRITE_ENABLE_RTN() sysFlashWriteEnable ()
#   define SYS_FLASH_WRITE_DISABLE_RTN() sysFlashWriteDisable ()
#else
#   define SYS_FLASH_WRITE_ENABLE_RTN()
#   define SYS_FLASH_WRITE_DISABLE_RTN()
#endif  /* SYS_FLASH_WRITE */

#ifdef  SYS_FLASH_BOARD_DELAY
#   define SYS_FLASH_BOARD_DELAY_RTN()  sysFlashBoardDelay ()
#else
#   define SYS_FLASH_BOARD_DELAY_RTN()
#endif  /* SYS_FLASH_BOARD_DELAY */

#ifdef SYS_FLASH_TYPE
#   define FLASH_MEM_TYPE       SYS_FLASH_TYPE
#else
#   define FLASH_MEM_TYPE       0
#endif  /* SYS_FLASH_TYPE */

#ifdef FLASH_SIZE_WRITEABLE
#   define FLASH_MEM_SIZE       FLASH_SIZE_WRITEABLE
#else
#   define FLASH_MEM_SIZE       FLASH_SIZE
#endif  /* FLASH_SIZE_WRITEABLE */

/* Operation status bits for Flash 29Fxxx devices */
/* AMD unlocking sequence */
//#define READ_ARRAY           0x00f000f0
//#define SETUP_ERASE          0x00800080
//#define SETUP_WRITE          0x00a000a0

//#define UNLOCK_1             0x00aa00aa  /*aaaa */
//#define UNLOCK_2             0x00550055  /* 5555   */
//#define UNLOCK_ADDR1         0x555		 /*5555   */
//#define UNLOCK_ADDR2         0x2aa		 /*2aa    */

//#define D5                   0x20    /* Set when programming timeout */
#define Q7(ix)      ((ix & 0x80) >> 7)  /* DQ7 bit */
#define Q5(ix)      ((ix & 0x20) >> 5)  /* DQ5 bit */


  #define wUnlockAddr1 ((FlashDPTR) unlockAddr1)
  #define wUnlockAddr2 ((FlashDPTR) unlockAddr2)

/* globals */

IMPORT  void sysFlashWriteEnable (void);
IMPORT  void sysFlashWriteDisable (void);
IMPORT  void sysFlashBoardDelay (void);

//extern volatile unsigned long 	flMsecCounter;
int flashDelayShift = SYS_FLASH_DELAY_SHIFT;
int flashDelayAdj   = SYS_FLASH_DELAY_ADJ;
int flashDelayIncr  = SYS_FLASH_DELAY_INCR;

/* forward declarations */

void    sysFlashDelay (int delayCount);
STATUS  sysFlashErase ();
STATUS  sysFlashWrite (UINT32 * pFB, int size, int offset,UINT32 value);
UINT8  sysFlashTypeGet (void);

/******************************************************************************
*
* sysFlashGet - get the contents of flash memory
*
* This routine copies the contents of flash memory into a specified
* string.  The string is terminated with an EOS.
*
* RETURNS: OK, or ERROR if access is outside the flash memory range.
*
* SEE ALSO: sysFlashSet()
*
* INTERNAL
* If multiple tasks are calling sysFlashSet() and sysFlashGet(),
* they should use a semaphore to ensure mutually exclusive access.
*/

STATUS sysFlashGet
    (
    char *  string,     /* where to copy flash memory      */
    int     strLen,     /* maximum number of bytes to copy */
    int     offset      /* byte offset into flash memory   */
    )
    {
    if ((offset < 0) || (strLen < 0) || ((offset + strLen) > FLASH_SIZE))
        return (ERROR);

    bcopyBytes ((char *) (FLASH_ADRS + offset), string, strLen);
    string [strLen] = EOS;

    return (OK);
    }

/******************************************************************************
*
* sysFlashDelay - create a delay for a specified number of microseconds
*
* This routine implements a busy wait for a specified number of microseconds.
* The timing loop can be adjusted on a board-dependent basis by
* defining values for the following macros:
* .iP
* SYS_FLASH_DELAY_SHIFT
* .iP
* SYS_FLASH_DELAY_ADJ
* .iP
* SYS_FLASH_DELAY_INCR
* .LP
* The values SYS_FLASH_DELAY_SHIFT and SYS_FLASH_DELAY_ADJ
* convert microseconds into a board-dependent tick-count.
* This routine can call a user-defined hook, sysFlashBoardDelay(),
* which creates a delay for a number of board-dependent ticks as
* specified by SYS_FLASH_DELAY_INCR.  To use sysFlashBoardDelay(), define
* SYS_FLASH_BOARD_DELAY in config.h.
*
* RETURNS: N/A
*
* SEE ALSO: sysFlashErase(), sysFlashWrite()
*/

void sysFlashDelay
    (
    int delayCount  /* number of uSec to delay */
    )
    {
    int ix;

    delayCount <<= flashDelayShift; /* board-dependent shift */
    delayCount += flashDelayAdj;        /* board-dependent addition */

    for (ix = 0; ix < delayCount; ix += flashDelayIncr)
        SYS_FLASH_BOARD_DELAY_RTN ();
    }

/******************************************************************************
*
* sysSectorErase - erase the contents of a sector
*
* This routine clears the contents of one sector in the flash memory.
*
* Flash 29F\f2xxx\f1 devices are erased by writing the six-byte erase code
* into specific address locations, which sets all byte locations to a high
* value (0xFF).
*
* RETURNS: OK, or ERROR if the contents of sector cannot be erased.
*/

STATUS sysSectorErase
    (
    UINT32 * pFA        /* Sector start address */
    )
    {
	int j,finished;
    STATUS retVal = OK;
	volatile unsigned int * flashBaseAddr=(volatile unsigned int *)0xffe00000;
	  *(flashBaseAddr + 0x555) = 0x00aa00aa;
	  for(j=0;j<10000;j++);
	  *(flashBaseAddr + 0x2aa) = 0x00550055;
	  for(j=0;j<10000;j++);
	  *(flashBaseAddr + 0x555) = 0x00800080;
	  for(j=0;j<10000;j++);
	  *(flashBaseAddr + 0x555) = 0x00aa00aa;
	  for(j=0;j<10000;j++);
	  *(flashBaseAddr + 0x2aa) = 0x00550055;
	  for(j=0;j<10000;j++);
	     *pFA = 0x00300030;
	   for(j=0;j<10000;j++);
//	    while(*pFA!=0xffffffff);
	    do
	    {
	    	finished = TRUE;
		if(*pFA != 0xffffffff)
		{
		   if(((*pFA & 0xffff &0x20)&&((*pFA & 0xffff) != 0xffff))
		       ||
		      ((*pFA & 0xffff0000 & (0x20*0x10000))&&((*pFA & 0xffff0000) != 0xffff0000)))
		   {
		       	*pFA = 0x00f000f0;
		          return -1;
		     }
		   finished = FALSE;
		}	
	    }while (!finished);
	    *flashBaseAddr=0x00f000f0;
    return (retVal);
    }

/******************************************************************************
*
* sysFlashWrite - write data to flash memory
*
* This routine copies specified data of a specified length, <size>, into a
* specified offset, <offset>, in the flash memory.  Data is passed as a string,
* <pFB>, if not NULL.  If NULL, data is taken as a repeated sequence of
* <value>.
* The parameter <flashType> should be set to the flash device code.
* The parameter <offset> must be appropriately aligned for the width of
* the Flash devices in use.
*
* Flash 28F\f2xxx\f1 devices are programmed by a sequence of operations:
* .iP
* set up device to write
* .iP
* perform write
* .iP
* verify the write
* .LP
*
* Flash 29F\f2xxx\f1 devices are programmed by a sequence of operations:
* .iP
* set up device to write
* .iP
* perform write
* .iP
* wait for the write to complete
* .LP
*
* RETURNS: OK, or ERROR if the write operation fails.
*
* SEE ALSO: sysFlashSet()
*/

STATUS sysFlashWrite
    (
    UINT32 * pFB,        /* string to be copied; use <value> if NULL */
    int         size,       /* size to program in bytes */
    int         offset,     /* byte offset into flash memory */
    FLASH_DEF   value       /* value to program */
    )
    {
    volatile UINT32 * pFA;       /* flash address */
//    UINT32 *  addrTemp;
    STATUS retVal = OK;
    int j;
    unsigned long writeTimeout;
    writeTimeout = flMsecCounter + 5000; 

    pFA = (UINT32*)FLASH_ADRS;
    
	  for (j = 0; j < (size / 4); j++,  pFA++)    	 
  	{
				/* Don't bother programming if buffer data == format value */
  	      if (*pFB == 0xffffffff)
  	      {
  	    	 pFB++;
  	          continue;
  	      }
  	      else
  	      {
  	      /* Program 32 bits */   
  	    	*(UINT32*) (FLASH_ADRS + 4*0x555) = UNLOCK_1;
  	    	*(UINT32*)(FLASH_ADRS + 4*0x2aa) = UNLOCK_2;
  	      /* flash command */
  	    	*(UINT32*) (FLASH_ADRS + 4*0x555) = SETUP_WRITE;    	      
  	    	*pFA=*pFB;  
 	      while ((*pFA != *pFB)    && (flMsecCounter < writeTimeout))
  	      {
  	          if( ((*pFA &          D5)  && ((*pFA ^ *pFB) &   0xffff))          
  	              ||
  	             ((*pFA & (0x10000 * D5)) && ((*pFA ^ *pFB) & 0xffff0000))
  	             )
  	          {
  	           *pFA = READ_ARRAY;
 	           retVal = flWriteFault;
  	           return flWriteFault;
  	          }
  	       }    	      
  	     if (retVal !=flOK)
  	     {
  	    	retVal = ERROR;
  	        break; 
  	     }
  	      }    	   
    	    pFB++;
  	}

    return (retVal);
    }

/******************************************************************************
*
* sysFlashSet - write to flash memory
*
* This routine copies a specified string into flash memory after calling
* sysFlashErase() and clearing flash memory.
*
* If FLASH_NO_OVERLAY is defined, the parameter <offset> must be
* appropriately aligned for the Flash devices in use (device width,
* sector size etc.).
*
* If the specified string must be overlaid on the contents of flash memory,
* undefine FLASH_NO_OVERLAY.
*
* RETURNS: OK, or ERROR if the write fails or the input parameters are
* out of range.
*
* SEE ALSO: sysFlashErase(), sysFlashGet(), sysFlashTypeGet(), sysFlashWrite()
*
* INTERNAL
* If multiple tasks are calling sysFlashSet() and sysFlashGet(),
* they should use a semaphore to ensure mutually exclusive access to flash
* memory.
*/

STATUS sysFlashSet
    (
    char *  string,     /* string to be copied into flash memory */
    int     strLen,     /* maximum number of bytes to copy       */
    int     offset      /* byte offset into flash memory         */
    )
    {
#ifndef FLASH_NO_OVERLAY
    char *tempBuffer;
#endif  /* FLASH_NO_OVERLAY */
    if ((offset < 0) || (strLen < 0) || ((offset + strLen) > FLASH_MEM_SIZE))
        return (ERROR);

    /* see if contents are actually changing */

    if (bcmp ((char *) (FLASH_ADRS + offset), string, strLen) == 0)
        return (OK);

#ifndef FLASH_NO_OVERLAY
    /* first read existing data */

    if (tempBuffer = malloc(FLASH_BLOCK_SIZE), tempBuffer == 0)
        return (ERROR);

    bcopyBytes ((char *) FLASH_ADRS, tempBuffer, FLASH_BLOCK_SIZE);

    bcopyBytes (string, (char*)(tempBuffer + offset), strLen);
#endif  /* FLASH_NO_OVERLAY */

    /* erase the sector */

     if (sysSectorErase ((UINT32*)FLASH_ADRS) == ERROR)
     {
#ifndef FLASH_NO_OVERLAY
        free (tempBuffer);
#endif
       return (ERROR);
      }
#ifndef FLASH_NO_OVERLAY                /* program device */
    if (sysFlashWrite ((UINT32*)tempBuffer, FLASH_SIZE_WRITEABLE, 0, 0)
        == ERROR)
    {
    	free (tempBuffer);
#else   /* FLASH_NO_OVERLAY */
    if (sysFlashWrite (FLASH_CAST (string), strLen, offset, flashType, 0) ==
    ERROR)
    {
#endif  /* FLASH_NO_OVERLAY */
    return (ERROR);
    }

#ifndef FLASH_NO_OVERLAY
    free (tempBuffer);
#endif

    return (OK);
    }
    
STATUS  sysEraseTffsFlash()
{
    unsigned int Addr = FLASH_BASE_ADRS + RESERVER_FOR_IMAGE_SIZE;
    int i,num,status=OK;
    num = (LOGICPD_FLASH_BANK0_SIZE - RESERVER_FOR_IMAGE_SIZE)/FLASH_BLOCK_SIZE;
    printf("first sector address: 0x%x, sector num = %d\n",Addr,num);
    for(i=0;i<num;i++)
    {
    	if(sysSectorErase((UINT32 *)Addr)!=OK)
    	{
    		status = FALSE;
    		printf("erase %d sector error,sector address=0x%x\n",num,Addr);
    	}
    	Addr += 0x40000; 
    	if((i+1) == num/2)
    		printf("50%%");
    	else if((i%6) == 0)
    		printf(".");
    }
    printf("100%%.\n");
    return status;
}

