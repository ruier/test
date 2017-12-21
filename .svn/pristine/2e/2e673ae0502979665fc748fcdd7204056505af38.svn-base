/* sysMtd.c - MTD driver for Spansion S29GL series flash parts */

/*
 * Copyright (c) 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,01mar12,syt  fixed compile warnings.
01a,14jan12,syt  derived from version 01c of fsl_ads5121e/sysMtd.c.
*/

/*
DESCRIPTION
This file provides the TFFS MTD driver for the Spansion S29GL series flash parts
on various boards (most probably on Freescale boards).  The driver handles the
low level operations like erase and program of the flash.  It also provides an
identify routine to check if it is compatible with the devices.

The macro S29GL_DEBUG_ON could be defined to enable informative debug messages
printing if necessary.

The driver is tested on APM Serengeti boards. To make the driver compatible
with other untested boards, the following parameters of the board flash specific
configuration should be taken care of by the developer.

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

/* externs */

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

#define S29GL_DQ7_MASK              ((S29GL_FLASH_DATA)0x80808080)
#define S29GL_DQ6_MASK              ((S29GL_FLASH_DATA)0x40404040)
#define S29GL_DQ2_MASK              ((S29GL_FLASH_DATA)0x20202020)

#define S29GL_OP_TIMEOUT            5000
#define S29GL_OP_DELAY              10

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
    UINT32  timeout = 0;

    sector = (S29GL_FLASH_ADRS)((UINT32)addr & S29GL_SECTOR_MASK);
    S29GL_PROGRAM(sector, addr, value);

    /* set timeout = 5s */

    timeout = flMsecCounter + S29GL_OP_TIMEOUT;

    do
        {
        if (flMsecCounter >= timeout)
            {
            S29GL_RESET(addr);
            return flTimedOut;
            }
        S29GL_FLASH_DELAY
        } while (*addr != value);

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
    UINT32 timeout = 0;
    int i;

    if (flWriteProtected(vol.socket))
        return flWriteProtect;

    for (i = firstErasableBlock;
         i < firstErasableBlock + numOfErasableBlocks;
         i++)
        {
        sector = (S29GL_FLASH_ADRS)vol.map(&vol, i * vol.erasableBlockSize, 0);

        S29GL_DBG_MSG("Erasing sector#%03d @ 0x%08x ...\r\n", i, (UINT32)sector);

        S29GL_ERASE(sector);

        /* set timeout = 5s */

        timeout = flMsecCounter + S29GL_OP_TIMEOUT;

        do
            {
            if (flMsecCounter >= timeout)
                {
                S29GL_RESET(sector);
                return flTimedOut;
                }
            } while ((*sector & S29GL_DQ7_MASK) == 0);
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

    vol.chipSize = S29GL_FLASH_SIZE;
    vol.noOfChips = S29GL_FLASH_NUM;
    vol.interleaving = S29GL_FLASH_NUM;
    vol.erasableBlockSize = S29GL_SECTOR_SIZE * vol.interleaving;
    vol.write = s29GlWrite;
    vol.erase = s29GlErase;

    S29GL_RESET(base);
    return flOK;
    }
