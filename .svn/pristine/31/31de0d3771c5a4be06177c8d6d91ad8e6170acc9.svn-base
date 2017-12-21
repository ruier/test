/* spS25flxxSpiFlash.c - Spansion SPI flash driver */

/*
 * Copyright (c) 2011, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,04jul12,d_l  chipErase check status twice.(WIND00359779)
01c,14feb12,y_y  fix the partial write failed in spS25flxxSpiFlashProgram 
                 and change read max length from 0xfffb to 0xfff8. (WIND00334290)
01b,04nov11,y_y  fix gnu build warnings.
01a,12jun11,y_y  written.
*/

/*
DESCRIPTION
This module contains routines for the SPI interface of Spansion S25FLXX serial 
SpiFlash.

SUPPORT
This driver can support the following Spansion SpiFlash devices:

    s25fl12800
    s25fl12801
    s25fl129p0
    s25fl129p1

Other serial SpiFlash devices with identical instruction sets can 
easily be supported by this driver by adding the appropriate device information 
to the list of supported devices in the supportedDevicesTable[].

This module provides the routines  for writing/reading  data to/from
the SPI interface with Spansion S25FLXX serial SpiFlash.

The routine spS25flxxSpiFlashProbe() is used for probing the device at 
the specified chipSelect to determine whether it is a supported Spansion 
SpiFlash part (registered in the supportedDevicesTable[]). 

The routine spS25flxxSpiFlashProgram() is used for writing data to 
a SpiFlash device.

The routine spS25flxxSpiFlashRead() is used for reading data from 
a SpiFlash device.

The routine spS25flxxSpiFlashShow() is used for showing the information of 
Spansion S25FLXX serial SpiFlash.

The routine spS25flxxSpiFlashTest() is used for testing erase, read and write 
functions.

*/

/* includes */

#include <vxWorks.h>
#include <taskLib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "spS25flxxSpiFlash.h"

/* externs */

IMPORT STATUS sysSpiWriteData (UINT32, UINT32, UINT32, UINT8 *, UINT8 *, UINT32);
IMPORT STATUS sysSpiReadData (VUINT32, UINT8 *);

/* debug */

#ifdef  DEBUG

IMPORT FUNCPTR _func_logMsg;
#define SPI_FLASH_DBG(X0, X1, X2, X3, X4, X5, X6)         \
  do { \
      if (_func_logMsg != NULL) \
          { \
          _func_logMsg (X0, (int)X1, (int)X2, (int)X3, (int)X4, (int)X5, (int)X6); \
          } \
    } while (0)
#else
#define SPI_FLASH_DBG(fmt,a,b,c,d,e,f) 
#endif  /* DEBUG */

/* locals */

INT32 spS25flxxSpiFlashRegRead (UINT8, UINT8);
LOCAL STATUS spS25flxxSpiFlashWriteEnable (UINT8, UINT8);
LOCAL STATUS spS25flxxSpiFlashWaitReady (UINT8, UINT32);
LOCAL SPI_FLASH_DEVICE spiFlash[SLAVES_MAX] = {{0}};

/* supported spiFlash table */ 

LOCAL const SPI_FLASH_DEVICE supportedDevicesTable[] = {

/* ManufID        DeviceID    ExtID   pageSize  sectorSize  sectorNum    name */

{SPAN_MANF_ID,  SPAN_DEV_ID,  0x0300,   256,   256 * 1024,    64,   "s25fl12800"},
{SPAN_MANF_ID,  SPAN_DEV_ID,  0x0301,   256,    64 * 1024,   256,   "s25fl12801"},
{SPAN_MANF_ID,  SPAN_DEV_ID,  0x4d00,   256,   256 * 1024,    64,   "s25fl129p0"},
{SPAN_MANF_ID,  SPAN_DEV_ID,  0x4d01,   256,    64 * 1024,   256,   "s25fl129p1"},
};

/*******************************************************************************
*
* spS25flxxSpiFlashProbe - probe the spiFlash device.
*
* This routine probes the deviceID of the slave at the specified chipSelect. If
* the deviceID matched one from the s25flxxxFlashDevices table, it is supported
* and we will get the rest of the relevant device information from the table.
* If the deviceID doesn't match one from the "know" table, the driver will still
* operate, but use a set of default parameters instead. If no device is found at
* the specified chipSelect, return ERROR.
*
* NOTE: This routine should be called before any spiFlash operation.
*
* RETURNS: OK or ERROR if there is an error.
*/

LOCAL STATUS spS25flxxSpiFlashProbe
    (
    UINT8 chipSelect
    )
    {
    UINT8 id[DEV_ID_LEN];
    UINT8 cmd = CMD_RDID;
    UINT8 i = 0;

    /* 
     * A single master with multiple slaves uses up to 
     * four chip select signals to selectively enable slaves 
     */
    
    if (chipSelect >= SLAVES_MAX)
        return ERROR;

    /* Check if the slave device has been probed */

    if (spiFlash[chipSelect].manufId != 0)
        /* previously probed and found device at this chip select */
        return OK;

    if (sysSpiWriteData (chipSelect, 1, DEV_ID_LEN, &cmd, NULL, NORMAL) != OK)
        return ERROR;

    if (sysSpiReadData (DEV_ID_LEN, (UINT8 *) id) != OK)
        return ERROR;

    /* If no device, return */

    if (id[0] == 0xff)
        {
        return ERROR;
        }

    /* Check for slave's ID in table of "known" flash devices */

    for (i = 0; i < NELEMENTS (supportedDevicesTable); i++)
        {
        if (supportedDevicesTable[i].manufId     == id[0]                 &&
            supportedDevicesTable[i].deviceId    == (id[1] << 8 | id[2])  &&
            supportedDevicesTable[i].extendDevId == (id[3] << 8 | id[4]))
            break;
        }

    /* 
     * If the slave's ID doesn't match one from the supported table,  
     * default parameters will be used. 
     */

    if ((NELEMENTS (supportedDevicesTable)) == i)
        {
        spiFlash[chipSelect].manufId  = id[0];
        spiFlash[chipSelect].deviceId = (id[1] << 8 | id[2]);
        spiFlash[chipSelect].extendDevId = (id[3] << 8 | id[4]);
        spiFlash[chipSelect].pageSize = 256;
        spiFlash[chipSelect].sectorNum = 256;
        spiFlash[chipSelect].sectorSize = 1024 *
        ((spiFlash[chipSelect].extendDevId & 0x00ff) == 0) ? 256 : 64;
        spiFlash[chipSelect].name = "Unknown";
        }
    else
        /* Save the device's info */
        spiFlash[chipSelect] = supportedDevicesTable[i];

    return OK;
    }

/*******************************************************************************
*
* spS25flxxSpiFlashProgram - SPI flash page program
*
* This is the spansion SPI flash page program routine.
*
* RETURNS: OK or ERROR if there is an error.
*/

STATUS spS25flxxSpiFlashProgram
    (
    UINT8  chipSelect,
    UINT32 startAddr,
    UINT32  len,
    UINT8 * buf         /* program data buf */
    )
    {
    UINT32 cmd32;
    UINT32 dataLen = 0;
    UINT32 offset = 0;

    /* Probe the Spansion device */

    if (spS25flxxSpiFlashProbe (chipSelect) != OK)
        return ERROR;

    while (offset < len)
        {
        
        /* Compute the number of data ready to write for one write cycle */
        
        dataLen = min (len - offset, spiFlash[chipSelect].pageSize -
                       ((startAddr + offset) % spiFlash[chipSelect].pageSize));

        cmd32 = (CMD_PP << 24) | ((offset + startAddr) & 0xffffff);

        if (spS25flxxSpiFlashWriteEnable (chipSelect, CMD_WREN) != OK)
            return ERROR;

        if (sysSpiWriteData (chipSelect, 4, dataLen, (UINT8*)&cmd32, 
                             buf + offset, TRANSONLY) != OK)
            return ERROR;

        if (spS25flxxSpiFlashWaitReady (chipSelect, TIMEOUT) != OK)
            return ERROR;

        offset += dataLen;
        }

    return OK;
    }

/*******************************************************************************
*
* spS25flxxSpiFlashRead - read SPI flash data
*
* This is the spansion SPI flash data read out routine.
*
* RETURNS: OK or ERROR if there is an error.
*/

STATUS spS25flxxSpiFlashRead
    (
    UINT8  chipSelect,
    UINT32 startAddr,
    UINT32 dataLen,
    UINT8 * dest
    )
    {
    UINT32 cmd32;
    UINT32 len = 0;
    UINT32 count = dataLen / READ_MAX_LEN + (dataLen % READ_MAX_LEN ? 1 : 0);

    while (count --)
        {
        
        /* Compute the number of data ready to read for one read cycle */
        
        len = min (dataLen, READ_MAX_LEN);

        cmd32 = (CMD_READ << 24) | (startAddr & 0xffffff);
        
        /* Send the read command */
        
        if (sysSpiWriteData (chipSelect, CMD_READ_LEN, len, (UINT8*)&cmd32, 
                             NULL, NORMAL) != OK)
            return ERROR;

        if (sysSpiReadData (len, dest) != OK)
            return ERROR;

        startAddr += len;
        dest += len;
        dataLen -= len;
        }

    return OK;
    }

/*******************************************************************************
* spS25flxxSpiFlashRegRead - read Spansion register
*
* This is the SPI flash status /config register read out routine.
*
* RETURNS: status register value.
*/

INT32 spS25flxxSpiFlashRegRead
    (
    UINT8 chipSelect,
    UINT8 cmd
    )
    {
    UINT8 buffer;

    if (sysSpiWriteData (chipSelect, sizeof (cmd), 1, &cmd, NULL, NORMAL) != OK)
        return ERROR;    

    if (sysSpiReadData (sizeof (cmd), &buffer) != OK)
        return ERROR;

    return (buffer);
    }

/*******************************************************************************
* spS25flxxSpiFlashWaitReady - wait the status ready
*
* This routine reads the status register, according to the Write-In-Process bit
* to judge the transfer finished or not. If the status still didn't finish in 
* timeout, return ERROR.
*
* RETURNS: OK or ERROR if there is an error.
*/

LOCAL STATUS spS25flxxSpiFlashWaitReady
    (
    UINT8 chipSelect,
    UINT32 timeout
    )
    {
    while (timeout--)
        {
        if ((spS25flxxSpiFlashRegRead (chipSelect, CMD_RDSR) & SR_WIP) == 0)
            break;

        sysUsDelay (1); /* a minor fixed delay */
        } 

    /* Timed out */

    if (timeout == 0)
        return ERROR;

    return OK;
    }

/*******************************************************************************
* spS25flxxSpiFlashRegWrite - write Spansion register
*
* This is the SPI flash status register write routine.
*
* RETURNS: OK or ERROR if there is an error.
*/

STATUS spS25flxxSpiFlashRegWrite
    (
    UINT8 chipSelect,
    UINT8 status
    )
    {
    UINT8 buffer;
    UINT8 cmd = CMD_WRSR;

    buffer = status;

    /* Write enable is required */
    if (spS25flxxSpiFlashWriteEnable (chipSelect, CMD_WREN) != OK)
        return ERROR;

    if (sysSpiWriteData (chipSelect, sizeof (cmd), 1, &cmd, &buffer, NORMAL) != OK)
        return ERROR;

    if (spS25flxxSpiFlashWaitReady (chipSelect, TIMEOUT) != OK)
        return ERROR;

    return OK;
    }

/*******************************************************************************
* spS25flxxSpiFlashWriteEnable - Spansion write enable
*
* The Write Enable (WREN) sets the Write Enable Latch (WEL) bit to a 1, which
* enables the device to accept a Write Status Register, program, or erase 
* command. The WEL bit must be set prior to every Page Program (PP), 
* Quad Page Program (QPP), Parameter Sector Erase (P4E, P8E), Erase(SE or BE), 
* Write Registers (WRR) and OTP Program (OTPP) command
*
* RETURNS: OK or ERROR if there is an error.
*/

LOCAL STATUS spS25flxxSpiFlashWriteEnable
    (
    UINT8  chipSelect,
    UINT8  cmd
    )
    {
    /* Send the WREN command */
    
    if (sysSpiWriteData (chipSelect, sizeof (cmd), 0, &cmd, NULL, NORMAL) != OK)
        return ERROR;
    
    if (spS25flxxSpiFlashWaitReady (chipSelect, TIMEOUT) != OK)
        return ERROR;

    return OK;
    }

/*******************************************************************************
* spS25flxxSpiFlashChipErase - Spansion chip erase.
*
* This is the Spansion chip erase routine. The Bulk Erase (BE) 
* command sets all the bits within the entire memory array to logic 1. A WREN 
* command is required prior to writing the BE command. The typical chip erase
* time is 128s. The maximum time is 256s.
*
* RETURNS: OK or ERROR if there is an error.
*/

STATUS spS25flxxSpiFlashChipErase
    (
    UINT8 chipSelect
    )
    {
    UINT8 cmd = CMD_BE;
    UINT8 status = 0;
    UINT8 checkStatusTimes = 0;
    UINT32 count = 0;
    UINT32 timeout = 0x100;

    /* Check whether the status is right */
    while (timeout--)
        {
        if (spS25flxxSpiFlashRegRead(chipSelect, CMD_RDSR) == 0)
            break;
        else
            {
            /* try to recover the status */
            spS25flxxSpiFlashRegWrite(chipSelect, 0);
            logMsg("Flash status wrong, try to recover..\r\n",1,2,3,4,5,6);
            }
        };

    if (spS25flxxSpiFlashWriteEnable (chipSelect, CMD_WREN) != OK)
        return ERROR;

    if (sysSpiWriteData (chipSelect, sizeof (cmd), 0, &cmd, NULL, NORMAL) != OK)
        return ERROR;

    /* Check Flash sector erase progress */

    do
        {
        /* Read the status register */
        
        status = spS25flxxSpiFlashRegRead (chipSelect, CMD_RDSR);

        SPI_FLASH_DBG ("BulkErase: %d s, Status: 0x%x(%s) \r", count, status,
                            ((status & SR_WIP) ? "Busy" : "Idle"), 0, 0, 0);

        /* Delay 1 second */

        taskDelay (sysClkRateGet ()); 

        /* Write in progress */

        /* 
         * The WIP bit is 1 during the Bulk Erase operation, and is 0 when the
         * operation is completed. But on P2041RDB board, it is found that 
         * reading status after Bulk Erase command will return 0 immediately,
         * then it return 3(busy) in next 80+ seconds, and finally it return 0
         * again. So check status twice to make sure operation is completed.
         */

        if ((status & SR_WIP) == 0)
            {
            checkStatusTimes++;
            if (checkStatusTimes >= MAX_CHECKSTATUS)
                break;
            }
        
        count++;
        }
    while (count < MAX_ERASE_TIME);

    /* Timeout */

    if (count >= MAX_ERASE_TIME)
        return ERROR;

    return OK;
    }

/*******************************************************************************
*
* spS25flxxSpiFlashSectorErase - Spansion sector erase
*
* This is the Spansion sector erase routine. The Sector Erase (SE) 
* command sets all bits at all addresses within a specified sector to a logic 1. 
* A WREN command is required prior to writing the SE command.
*
* RETURNS: OK or ERROR if there is an error.
*/

STATUS spS25flxxSpiFlashSectorErase
    (
    UINT8  chipSelect,
    UINT32 sectorNum
    )
    {
    UINT32 cmd32 = 0;
    UINT32 timeout = 0x100;

    /* Probe the Spansion device */

    if (spS25flxxSpiFlashProbe (chipSelect) != OK)
        return ERROR;

    /* Check whether the status is right */

    while (timeout--)
        {
        if (spS25flxxSpiFlashRegRead(chipSelect, CMD_RDSR) == 0)
            break;
        else
            {
            /* try to recover the status */
            spS25flxxSpiFlashRegWrite(chipSelect, 0);
            logMsg("Flash status wrong, try to recover..\r\n",1,2,3,4,5,6);
            }
        };

    cmd32 = ((UINT32) CMD_SE << 24) |
            ((sectorNum * spiFlash[chipSelect].sectorSize) & 0xffffff);

    /* Write enable */

    if (spS25flxxSpiFlashWriteEnable (chipSelect, CMD_WREN) != OK)
        return ERROR;

     /* Send the SE command */
     
    if (sysSpiWriteData (chipSelect, sizeof (cmd32), 0, (UINT8*)&cmd32, NULL, NORMAL) != OK)
        return ERROR;

    if (spS25flxxSpiFlashWaitReady (chipSelect, TIMEOUT) != OK)
        return ERROR;

    return OK;
    }

#ifdef INCLUDE_SHOW_ROUTINES
/*******************************************************************************
*
* spS25flxxSpiFlashShow -  Spansion SPI flash device show routine
*
* This routine shows Spansion SPI flash information.
*
* RETURNS: OK or ERROR if there is an error.
*/

void spS25flxxSpiFlashShow
    (
    UINT8 chipSelect
    )
    {
    UINT32 temp = 0;

    if (chipSelect >= SLAVES_MAX)
        {
        printf ("ERROR: chipSelect should be less than 4. \n");
        return;
        } 

    /* Probe the Spansion device */

    if (spS25flxxSpiFlashProbe (chipSelect) != OK)
        {
        printf ("ERROR: probe the Spansion device failed. \n");
        return;
        }

    /* Info show */

    temp = spiFlash[chipSelect].extendDevId & 0x00ff;

    printf ("Manufacturer:  0x%02x(%s) \n",
            spiFlash[chipSelect].manufId,
            (spiFlash[chipSelect].manufId == SPAN_MANF_ID) ? "Spansion" : "Others");
    printf ("Device ID:     0x%04x \n", spiFlash[chipSelect].deviceId);
    printf ("Extend ID:     0x%04x \n", spiFlash[chipSelect].extendDevId);
    printf ("Type:          \"%s\"%s \r\n", spiFlash[chipSelect].name, 
           ((strcmp(spiFlash[chipSelect].name, "Unknown") == 0) ? 
            " (default parameters used)" : ""));
    printf ("PageSize:      %dB  \r\nSectorSize:    %d(KB)\r\n",
            spiFlash[chipSelect].pageSize,
            ((temp == 0) ? 256 : 64));
    printf ("SectorsNum:    %d  \r\nCapacity:      %d(MB)\r\n",
            spiFlash[chipSelect].sectorNum,
            spiFlash[chipSelect].sectorNum * ((temp == 0) ? 256 : 64) / 1024);
    return;
    }

#ifdef  DEBUG

#define NUM_SECTOR  16  /* test 16 sectors */

/*******************************************************************************
*
* spS25flxxSpiFlashTest - test SPI flash device driver
*
* This is the SPI flash S25FL12XX device test routine.
*
* RETURNS: OK or ERROR if there is an error.
*/

void spS25flxxSpiFlashTest
    (
    UINT8 chipSelect
    )
    {
    UINT8 * pData;
    UINT8 * pDest;
    UINT32  i, j;

    /* Probe the Spansion device */

    if (spS25flxxSpiFlashProbe (chipSelect) != OK)
        {
        printf ("ERROR: probe the Spansion device failure. \n");
        return;
        }

    /* Sector erase */
    
    for (i = 1; i <= NUM_SECTOR; i++)
        {
        if (spS25flxxSpiFlashSectorErase (chipSelect, (i-1)) != OK)
            {
            printf ("ERROR: sector erase failure. \n");
            return;
            }
        printf ("Formatted %d of %d sectors = %d.%01d %%\r", i, NUM_SECTOR,
                 100 * i / (NUM_SECTOR),
                 1000 * i / (NUM_SECTOR) % 10
        );
        }

    if ((pData = malloc (NUM_SECTOR * spiFlash[chipSelect].sectorSize)) == NULL)
        {
        printf ("ERROR: pData memory allocate failure \r\n");
        return;
        }

    if ((pDest = malloc (NUM_SECTOR * spiFlash[chipSelect].sectorSize)) == NULL)
        {
        printf ("ERROR: pDest memory allocate failure \r\n");
        free (pData);
        return;
        }

    for (i = 0, j = 0; i < (NUM_SECTOR * spiFlash[chipSelect].sectorSize); i++, j++)
        {
        *(pData + i) = j + 1;
        }

    /* Program pData to SPI flash */

    if (spS25flxxSpiFlashProgram (chipSelect, 0, NUM_SECTOR *
                                  spiFlash[chipSelect].sectorSize, pData) != OK)
        {
        printf ("ERROR: spiFlash program failure. \r\n");
        goto Exit;
        }

    /* read the data to pDest */
    
    if (spS25flxxSpiFlashRead (chipSelect, 0, NUM_SECTOR *
                               spiFlash[chipSelect].sectorSize, pDest) != OK)
        {
        printf ("ERROR: spiFlash data read failure. \r\n");
        goto Exit;
        }

    if (memcmp (pData, pDest, NUM_SECTOR * spiFlash[chipSelect].sectorSize) != OK)
        {
        printf ("ERROR: memory compare failure. \r\n");
        goto Exit;
        }

    if (spS25flxxSpiFlashRegRead(chipSelect, CMD_RDSR) != 0)
        {
        printf ("ERROR: status failure. \r\n");
        goto Exit;
        }

    printf ("\r\nSpansion SPI flash test succeeded!\r\n");

Exit:
    free (pData);
    free (pDest);

    return;
    }
#endif  /* DEBUG */
#endif  /* INCLUDE_SHOW_ROUTINES */


