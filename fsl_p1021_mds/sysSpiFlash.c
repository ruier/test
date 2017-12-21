/* sysSpiFlash.c - Freescale P1021MDS board SPI Flash driver */

/*
* Copyright (c) 2009-2010 Wind River Systems, Inc.
*
* The right to copy, distribute, modify or otherwise make use
* of this software may be licensed only pursuant to the terms
* of an applicable Wind River license agreement.
*/

/*
modification history
--------------------
01b,17dec10,y_y  change the taskDelay with SYS_CLK_RATE
01b,17nov10,y_y  write sysFlashSet and sysFlashGet routine 
01b,17aug10,e_d  fix issue for write data
01a,12jun09,syt  written.
*/

/* includes */

#include <vxWorks.h>
#include <iv.h>
#include <excLib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sysSpiFlash.h"

/* local */

void  spiFlashWriteEnable  (void);
void  spiFlashWriteDisable (void);
UINT8 spiFlashReadStatus   (void);

#define BOOTLINE_MAC_SAVE_ADDR          0x1fffe000

/*******************************************************************************
*
* spiFlashEraseSector -  SPI Flash sector erase
*
* This is the SPI Flash S25FL128P sector erase routine.
*
*
* RETURNS: OK or ERROR if there is an error.
*
* ERRNO
*/

STATUS spiFlashEraseSector
    (
    UINT32  sectorAddr
    )
    {
    UINT8   status, cmd8;
    UINT32  i = 0;
    UINT32  cmd32;

    /* write enable command is required */

    spiFlashWriteEnable ();

    taskDelay(1);

    cmd32 = (FLASH_SE_64K_TYPE << 24) | (sectorAddr & 0xffffff);

    /* send out ERASE sector command to Flash device */

    sysSpiWriteCmd (SPI_CS0, TRANS_ONLY, (UINT8 *)&cmd32,
                    (CMD_LEN + ADDR_LEN), 0x00);

    taskDelay(1);

    /* check Flash sector erase progress */

    cmd8 = FLASH_RDSR;

    do
        {
        status = spiFlashReadStatus ();

        taskDelay (1);

        i++;

        } while((status & WIP) && (i < FLASH_ERASE_TIMEOUT));

    if (i >= FLASH_ERASE_TIMEOUT)
        return ERROR;

    return OK;

    }

/*******************************************************************************
*
* spiFlashEraseChip -  SPI Flash chip erase
*
* This is the SPI Flash S25FL128P chip erase routine.
*
*
* RETURNS: OK or ERROR if there is an error.
*
* ERRNO
*/

STATUS spiFlashEraseChip (void)
    {
    UINT32  count;

    for (count = 0; count < SPI_FLASH_SECTOR_NUMBER; count++)
        {
        if (spiFlashEraseSector (count * SPI_FLASH_SECTOR_SIZE) == ERROR)
            return ERROR;
        taskDelay(SYS_CLK_RATE/2);
        printf ("Formatted %d of %d units = %d.%01d %%\r",
                count, (SPI_FLASH_SECTOR_NUMBER - 1),
                100 * count / (SPI_FLASH_SECTOR_NUMBER - 1),
                1000 * count / (SPI_FLASH_SECTOR_NUMBER - 1) % 10
                );
        }

   if (count == SPI_FLASH_SECTOR_NUMBER)
        printf ("\r\n");

    return OK;
    }

/*******************************************************************************
*
* spiFlashRead -  read SPI Flash data
*
* This is the SPI Flash S25FL128P data read out routine.
*
*
* RETURNS: OK or ERROR if there is an error.
*
* ERRNO
*/

STATUS spiFlashRead
    (
    UINT32          flashSrcAddr,
    UINT8 *         pData,
    UINT32          dataNum
    )
    {
    UINT32  cmd32;

    if ((flashSrcAddr > 0xFFFFFF) || ((flashSrcAddr + dataNum) > 0xFFFFFF))
        return ERROR;

    while (dataNum / SPI_TRANS_LEN_MAX)
        {
        cmd32 = (FLASH_READ << 24) | (flashSrcAddr & 0xffffff);

        sysSpiWriteCmd (SPI_CS0, NORMAL, (UINT8 *)&cmd32,
                        (CMD_LEN + ADDR_LEN), SPI_TRANS_LEN_MAX);

        sysSpiRead (pData, SPI_TRANS_LEN_MAX);

        dataNum      -= SPI_TRANS_LEN_MAX;
        flashSrcAddr += SPI_TRANS_LEN_MAX;
        pData        += SPI_TRANS_LEN_MAX;
        }

    if ((dataNum % SPI_TRANS_LEN_MAX) == 0)
        {
        return OK;
        }
    else
        {
        cmd32 = (FLASH_READ << 24) | (flashSrcAddr & 0xffffff);

        sysSpiWriteCmd (SPI_CS0, NORMAL, (UINT8 *)&cmd32,
                        (CMD_LEN + ADDR_LEN), dataNum);

        sysSpiRead (pData, dataNum);

        return OK;

        }
    }

/*******************************************************************************
*
* spiFlashByteProgram -  SPI Flash Byte program
*
* This is the SPI Flash S25FL128P byte program routine.
*
*
* RETURNS: OK or ERROR if there is an error.
*
* ERRNO
*/

LOCAL STATUS spiFlashByteProgram
    (
    UINT32  destAddr,
    UINT8   data
    )
    {
    UINT32  cmd32, i;
    UINT8   cmd8;

    /* write enable command is required */

    spiFlashWriteEnable ();

    cmd32 = (FLASH_PP << 24) | (destAddr & 0xffffff);

    sysSpiWriteCmd (SPI_CS0, TRANS_ONLY, (UINT8 *)&cmd32,
                    (CMD_LEN + ADDR_LEN), 1);

    sysSpiWriteData (&data, 1);

    /* check Flash sector erase progress */

    cmd8 = FLASH_RDSR;

    i = 0;

    do
        {
        taskDelay (1);
        i++;
        if ((spiFlashReadStatus () & WIP)== 0)
	  break;
        }while ( i < 200);

    if (i >= 200)
       {
       printf("SF: SPANSION page programming timed out\n");
       return ERROR;
        }


    return OK;
    }

/*******************************************************************************
*
* spiFlashPageProgram -  SPI Flash page program
*
* This is the SPI Flash S25FL128P page program routine.
*
*
* RETURNS: OK or ERROR if there is an error.
*
* ERRNO
*/

LOCAL STATUS spiFlashPageProgram
    (
    UINT32  destAddr,       /* SPI Flash dest address */
    UINT8 * pData,          /* data source address pointer */
    UINT32  byteNumber      /* number of bytes */
    )
    {
    UINT32  cmd32, i;
    UINT8   cmd8;

    if (byteNumber > PP_LEN)
        return ERROR;

    /* write enable command is required */

    spiFlashWriteEnable ();

    cmd32 = (FLASH_PP << 24) | (destAddr & 0xffffff);


    sysSpiWriteCmd (SPI_CS0, TRANS_ONLY, (UINT8 *)&cmd32,
                    (CMD_LEN + ADDR_LEN), byteNumber);

    sysSpiWriteData (pData, byteNumber);

    /* check Flash sector erase progress */

    cmd8 = FLASH_RDSR;
    i = 0;

    do
        {
        taskDelay (1);
        i++;
        
        if ((spiFlashReadStatus () & WIP)== 0)
	  break;
        }while ( i < 200);

    if (i >= 200)
       {
       printf("SF: SPANSION page programming timed out\n");
       return ERROR;
        }
    return OK;
    }


/*******************************************************************************
* spiFlashWrite - write data into SPI Flash
*
* This is the SPI Flash S25FL128P write data routine.
*
*
* RETURNS: OK or ERROR if there is an error.
*
* ERRNO
*/

STATUS spiFlashWrite
    (
    UINT32          destAddr,
    UINT8 *         pData,
    UINT32          dataNum
    )
    {
    UINT8 * pMem = NULL;
    UINT32  i;

    /* check validity of dest address and data number */

    if ((destAddr > 0xFFFFFF) || ((destAddr + dataNum) > 0xFFFFFF))
        return ERROR;

    if (dataNum >= PP_SIZE)
        {
        if ((pMem = malloc (PP_SIZE)) == NULL)
            return ERROR;

        memset (pMem, 0x00, PP_SIZE);

        if ( pMem == NULL)
            return ERROR;
        }

    /*
     * if data number larger than page program length, then use SPI Flash
     * page program function
     */

    while (dataNum / PP_SIZE)
        {

        if (spiFlashPageProgram (destAddr, pData, PP_SIZE) == ERROR)
        

            return ERROR;
         

         spiFlashRead (destAddr, pMem, PP_SIZE);

        if (memcmp (pData, pMem, PP_SIZE) != OK)
            {
            free (pMem);
     
            return ERROR;
            }

        destAddr += PP_SIZE;
        pData    += PP_SIZE;
        dataNum  -= PP_SIZE;

        }

    free (pMem);

    if((dataNum % PP_SIZE) == 0)
        return OK;

    /*
     * if data number samll than page program length, then use SPI Flash
     * byte program function
     */

    for (i = 0; i < (dataNum % PP_SIZE); i++)
        spiFlashByteProgram ((destAddr + i), *(pData + i));

    if ((pMem = malloc (dataNum % PP_SIZE)) == NULL)
        return ERROR;

    memset (pMem, 0x00, (dataNum % PP_SIZE));

    spiFlashRead(destAddr, pMem, (dataNum % PP_SIZE));

    if (memcmp (pData, pMem, (dataNum % PP_SIZE)) != OK)
        {
        free (pMem);

        return ERROR;
        }

    free (pMem);

    return OK;
    }

/*******************************************************************************
* spiFlashReadStatus - Read SPI Flash Status register
*
* This is the SPI Flash Status register read out routine.
*
*
* RETURNS: Status register value.
*
* ERRNO
*/

UINT8 spiFlashReadStatus (void)
    {
    UINT8 status, cmd8;

    status = 0x0;

    cmd8 = FLASH_RDSR;

    sysSpiWriteCmd (SPI_CS0, NORMAL, (UINT8 *)&cmd8, CMD_LEN, 1);
    
    sysSpiRead (&status, 1);

    return status;
    }

/*******************************************************************************
* spiFlashWriteEnable - write enable command
*
* This is the SPI Flash write wnable command routine.
*
*
* RETURNS: N/A.
*
* ERRNO
*/

void spiFlashWriteEnable (void)
    {
    UINT8 cmd8;

    cmd8 = FLASH_WREN;

    sysSpiWriteCmd (SPI_CS0, TRANS_ONLY, (UINT8 *)&cmd8, CMD_LEN, 0);
    }

/*******************************************************************************
* spiFlashWriteDisable - write disable command
*
* This is the SPI Flash write disable command routine.
*
*
* RETURNS: N/A.
*
* ERRNO
*/

 void spiFlashWriteDisable (void)
    {
    UINT8 cmd8;

    cmd8 = FLASH_WRDI;

    sysSpiWriteCmd (SPI_CS0, TRANS_ONLY, (UINT8 *)&cmd8, CMD_LEN, 0);
    }

#ifdef  SPI_FLASH_DEBUG
/*******************************************************************************
*
* spiFlashDevIdShow -  show SPI Flash device inforation
*
* This is the SPI Flash S25FL128P device information routine.
*
*
* RETURNS: N/A.
*
* ERRNO
*/
void spiFlashDevIdShow (void)
    {
    UINT8   cmd8;
    UINT8 * pDevId;

    pDevId = malloc (RDID_DATA_LEN);

    cmd8 = FLASH_RDID;

    sysSpiWriteCmd (SPI_CS0, NORMAL, (UINT8 *)&cmd8, CMD_LEN, RDID_DATA_LEN);

    sysSpiRead (pDevId, RDID_DATA_LEN);

    printf("Flash Manufacturer ID    -> 0x%02x\r\n", *((UINT8 *)pDevId));
    printf("Flash Device ID          -> 0x%04x\r\n", *((UINT16 *)(pDevId + 1)));
    printf("Flash Extended Device ID -> 0x%04x\r\n", *((UINT16 *)(pDevId + 3)));

    free (pDevId);

    }

#define NUM_SECTOR  2
/*******************************************************************************
*
* spiFlashTest -  test SPI Flash device driver
*
* This is the SPI Flash S25FL128P device test routine.
*
*
* RETURNS: OK or ERROR.
*
* ERRNO
*/

STATUS spiFlashTest (void)
    {
    UINT8 * pData = NULL;
    UINT8   j;
    UINT32  i;
  
    for (i = 0; i < NUM_SECTOR; i++)
        {
        spiFlashEraseSector (i * SPI_FLASH_SECTOR_SIZE);

        taskDelay(SYS_CLK_RATE/2);
        }
    
        pData = malloc(NUM_SECTOR * SPI_FLASH_SECTOR_SIZE);

    for (i = 0, j = 0; i < (2 * SPI_FLASH_SECTOR_SIZE); i++, j++)
        *(pData + i) = j + 1;

    if (pData == NULL)
        {
        printf("spiFlashTest: memery allocate failure \r\n");

        return ERROR;
        }
    
    if ((spiFlashWrite (0x00, pData, (NUM_SECTOR * SPI_FLASH_SECTOR_SIZE))) == ERROR)
        {
        printf("spiFlashTest: Flash write failure \r\n");

        free (pData);

        return ERROR;
        }

    free (pData);

    printf("spiFlashTest: Test successed!\r\n");

    return OK;
    }
#endif  /* SPI_FLASH_DEBUG */

/* Note: this Flash have 256 bank in all */

#define SECTOR_NUM   100  /* save MAC and bootline in which SPI flash bank */
/******************************************************************************
*
* sysFlashGet - get the contents of spi flash
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
    char *  string,     /* string to be copied into flash memory */
    int     strLen,     /* maximum number of bytes to copy       */
    int     offset      /* byte offset into flash memory         */
    )
    {
    if ((offset < 0) || (strLen < 0))
        return (ERROR);
          
    spiFlashRead ((SECTOR_NUM*SPI_FLASH_SECTOR_SIZE+offset), \
                 (UINT8 *)BOOTLINE_MAC_SAVE_ADDR+offset, (strLen+4-(strLen%4)));
     
    bcopyBytes ((char *)(BOOTLINE_MAC_SAVE_ADDR+offset), string, strLen);

    string [strLen] = EOS;

    return (OK);
    }

/*******************************************************************************
*
* sysFlashSet - write to spi flash 
*
* This routine copies a specified string into spi flash .
*
* RETURNS: OK, or ERROR if the write fails or the input parameters are
* out of range.
*
* SEE ALSO: sysFlashGet(), sysFlashWrite()
*
* INTERNAL
* If multiple tasks are calling sysFlashSet() and sysFlashGet(), they should
* use a semaphore to ensure mutually exclusive access to flash.
*/

STATUS sysFlashSet
    (
    char *  string,     /* string to be copied into flash memory */
    int     strLen,     /* maximum number of bytes to copy       */
    int     offset      /* byte offset into flash memory         */
    )
    {

    UINT8 * tempBuffer;

    /* limited to one sector */

    if ((offset < 0) || (strLen < 0) || (strLen >256)|| \
       ((offset + strLen) > SPI_FLASH_SECTOR_SIZE) )
        return (ERROR);

    /* see if contents are actually changing */

    if (bcmp ((char *)BOOTLINE_MAC_SAVE_ADDR+offset, string, strLen) == 0)
       {
        return (OK);
        }
    /* first read existing data */

    if ((tempBuffer = malloc (0x200)) == NULL)
        return (ERROR);
        
        spiFlashRead ((SECTOR_NUM*SPI_FLASH_SECTOR_SIZE),tempBuffer, 0x200);
       
        bcopyBytes (string, (char *)tempBuffer + offset, strLen);
      
    if (spiFlashEraseSector (SPI_FLASH_SECTOR_SIZE*SECTOR_NUM) == ERROR)
        {
        free (tempBuffer);
        printf("spiFlashEraseSector failed!\r\n");
        return (ERROR);
        }
        
    /* the typical sector erase time is 0.5s, so delay 0.5s to finish erase */

          taskDelay(SYS_CLK_RATE/2);

    if ((spiFlashWrite((SECTOR_NUM*SPI_FLASH_SECTOR_SIZE), \
                        tempBuffer, 2*PP_SIZE)) == ERROR)
        {
        printf("spiFlashWrite  failed!\r\n");
        free (tempBuffer);
        return (ERROR);
        }
       
    spiFlashRead ((SECTOR_NUM*SPI_FLASH_SECTOR_SIZE), \
                 (UINT8 *)BOOTLINE_MAC_SAVE_ADDR, 2*PP_SIZE);
    
    free (tempBuffer);

    return (OK);
    }
