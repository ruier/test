/* sysMtd.c - MTD driver for flash on MDS8568 */

/*
 * Copyright (c) 2007-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,18nov09,x_z  fix compiler warnings for mds837x.
01b,26jan08,b_m  fix compiler warnings.
01a,06jun07,b_m  written.
*/

/*
DESCRIPTION
This file provides the TFFS MTD driver for the 16-bit 32MB Spansion S29GL256
flash on the MDS8568 board. The driver handles the low level operations like
erase and program of the flash. It also provides an identify routine to check
if it is compatible with the devices.

The macro DEBUG_PRINT must be defined as 'printf' to provide informative debug
messages if necessary.

The driver is only compatible and tested with MDS8568 board. DO NOT use this
driver on any other boards unless the board flash configuration is absolutely
the same.

INCLUDE FILES:

SEE ALSO:
*/

#include <stdio.h>
#include <tffs/flflash.h>
#include <tffs/backgrnd.h>

/* externs */

IMPORT  void sysUsDelay(UINT32 delay);

/* defines */

/* define this macro to 'printf' to provide informative debug messages */
#undef  DEBUG_PRINT

#ifndef S29GL_FLASH_SIZE
#   define S29GL_FLASH_SIZE     0x02000000
#endif /* S29GL_FLASH_SIZE */

#define S29GL_FLASH_NUM         1
#define S29GL_SECTOR_SIZE       0x20000
#define S29GL_SECTOR_MASK       0xfffe0000

#define S29GL_VENDOR_OFFSET     0
#define S29GL_DEVICE_OFFSET     1

#define S29GL_VENDOR_ID         0x0001
#define S29GL_DEVICE_ID         0x227e
#define S29GL_FLASH_ID          0x017e

#define S29GL_UNLOCK_ADDR1      0x555
#define S29GL_UNLOCK_ADDR2      0x2aa

#define S29GL_UNLOCK_DATA1      0xaa
#define S29GL_UNLOCK_DATA2      0x55

#define S29GL_CMD_RESET             0xF0
#define S29GL_CMD_READ_ID           0x90
#define S29GL_CMD_ERASE_SECTOR1     0x80
#define S29GL_CMD_ERASE_SECTOR2     0x30
#define S29GL_CMD_PROGRAM           0xA0

#define S29GL_DQ6_MASK              0x40

#define S29GL_OP_TIMEOUT            5000

#define S29GL_FLASH_DELAY   \
    do  \
    {   \
        sysUsDelay(10); \
    } while((0));

#define S29GL_RESET(addr)   \
    do  \
    {   \
        *addr = S29GL_CMD_RESET;    \
    } while((0));

#define S29GL_ERASE(addr)   \
    do  \
    {   \
        *(addr + S29GL_UNLOCK_ADDR1) = S29GL_UNLOCK_DATA1;  \
        *(addr + S29GL_UNLOCK_ADDR2) = S29GL_UNLOCK_DATA2;  \
        *(addr + S29GL_UNLOCK_ADDR1) = S29GL_CMD_ERASE_SECTOR1; \
        *(addr + S29GL_UNLOCK_ADDR1) = S29GL_UNLOCK_DATA1;  \
        *(addr + S29GL_UNLOCK_ADDR2) = S29GL_UNLOCK_DATA2;  \
        *addr = S29GL_CMD_ERASE_SECTOR2;   \
        S29GL_FLASH_DELAY;  \
    } while((0));

#define S29GL_PROGRAM(addr1, addr2, value)  \
    do  \
    {   \
        *(addr1 + S29GL_UNLOCK_ADDR1) = S29GL_UNLOCK_DATA1; \
        *(addr1 + S29GL_UNLOCK_ADDR2) = S29GL_UNLOCK_DATA2; \
        *(addr1 + S29GL_UNLOCK_ADDR1) = S29GL_CMD_PROGRAM;  \
        *addr2 = value; \
        S29GL_FLASH_DELAY;  \
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
* This routine is called to program byte to the flash part on MDS8568 board.
*
* RETURNS: flOK or flTimedOut if timeout.
*
* ERRNO: N/A
*/

LOCAL FLStatus s29GlProgram
    (
    UINT16 *    addr,
    UINT16      value
    )
    {
    UINT16 *sector;
    UINT16  data1, data2;
    UINT32  timeout = 0;

    sector = (UINT16 *)((UINT32)addr & S29GL_SECTOR_MASK);
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
* This routine is called to write data to the flash part on MDS8568 board.
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
    UINT16 *aligned;
    UINT16  data;
    UINT32  num;
    int     i;

    if (flWriteProtected(vol.socket))
        return flWriteProtect;

    /* calculate the program addr, make sure it's 16-bit aligned */
    unaligned = (UINT8 *)vol.map (&vol, address, 0);
    num = (UINT32)unaligned & 0x1;
    aligned = (UINT16 *)((UINT32)unaligned - num);

    if (num != 0)
        {
        data = *aligned;

        for (i = num ; i < 2; i++)
            {
            data &= ~(0xFF << ((1 - i) * 8));
            data |= ((*(buf + i - num)) << ((1 - i) * 8));
            }

        if (s29GlProgram(aligned, data) != flOK)
            return flWriteFault;

        buf  += (2 - num);
        left -= (2 - num);
        aligned++;
        }

    while (left >= 2)
        {
        data = *(UINT16 *)buf;

        if (s29GlProgram(aligned, data) != flOK)
            return flWriteFault;

        buf  += 2;
        left -= 2;
        aligned++;
        }

    if (left > 0)
        {
        data = *aligned;

        for (i = 0 ; i < left; i++)
            {
            data &= ~(0xFF << ((1 - i) * 8));
            data |= ((*(buf + i)) << ((1 - i) * 8));
            }

        if (s29GlProgram(aligned, data) != flOK)
            return flWriteFault;
        }

    if (tffscmp((void FAR0 *)unaligned, buffer, length))
        {
#ifdef DEBUG_PRINT
        DEBUG_PRINT("[S29GLWrite]: data double check error @ 0x%08x ...\n", (UINT32)unaligned);
#endif
        return flWriteFault;
        }

    return flOK;
    }

/*******************************************************************************
*
* S29GLErase - erase routine for S29GL flash
*
* This routine is called to erase sector(s) of the flash part on MDS8568 board.
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
    UINT16 * sector = NULL;
    UINT16 data1, data2;
    UINT32 timeout = 0;
    int i;

    if (flWriteProtected(vol.socket))
        return flWriteProtect;

    for (i = firstErasableBlock; i < firstErasableBlock + numOfErasableBlocks; i++)
        {
        sector = (UINT16 *)vol.map(&vol, i * vol.erasableBlockSize, 0);

#ifdef DEBUG_PRINT
        DEBUG_PRINT("Erasing sector#%03d @ 0x%08x ...\r", i, (UINT32)sector);
#endif

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
* This routine is called to probe the flash part on MDS8568 board.
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
    UINT16 * base = (UINT16 *)vol.map(&vol, 0, 0);
    UINT16   vendor, device;

#ifdef DEBUG_PRINT
    DEBUG_PRINT("Entering s29GlIdentify routine @ base address 0x%08x ...\n", (UINT32)base);
#endif

    /* check the flash id */
    S29GL_READ_ID(base);
    vendor = *(base + S29GL_VENDOR_OFFSET);
    device = *(base + S29GL_DEVICE_OFFSET);
    if ((vendor == S29GL_VENDOR_ID) && (device == S29GL_DEVICE_ID))
        {
        vol.type = S29GL_FLASH_ID;
        }
    else
        {
        S29GL_RESET(base);
        return flUnknownMedia;
        }

    vol.chipSize = S29GL_FLASH_SIZE;
    vol.noOfChips = S29GL_FLASH_NUM;
    vol.interleaving = S29GL_FLASH_NUM;
    vol.erasableBlockSize = S29GL_SECTOR_SIZE * vol.interleaving;
    vol.write = s29GlWrite;
    vol.erase = s29GlErase;

    S29GL_RESET(base);
    return flOK;
    }
