/* at24cEeprom.c - Atmel AT24C01B EEPROM driver */

/*
 * Copyright (c) 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,23dec10,x_s  created from amcc_kilauea/at24c16Mem.c v01a.
*/

/*
DESCRIPTION
This driver provides routines to read and write a I2C AT24C01B (or similar)
EEPROM. It is typically included by the BSP file sysLib.c.

The macro INCLUDE_I2C must be defined to include the IIC bus interface support
in config.h before define the macro INCLUDE_AT24C_MEM in config.h to include
this driver. The size of the entire chip is 128 bytes.

The more details about the I2C bus can be obtained from sysLsiI2c.c.

INCLUDE FILES: sysLib.h ctype.h string.h stdioLib.h stdlib.h config.h

SEE ALSO:
\tb "ATMEL AT24C01B Two-wire Serial EEPROM Data Sheet(5156E¨CSEEPR¨C10/08)",
\tb "APP350 ARM I2C Interface",
\tb "Axxia Communication Processor (ACP3400) Technical Guide",
\tb "ACP3400 Validation Board (Mission) Architecture",
\tb "LSI ACP34XX Board Schematics".
*/

/* includes */

#include <vxWorks.h>
#include <sysLib.h>
#include <ctype.h>
#include <string.h>
#include <stdioLib.h>
#include <stdlib.h>
#include "config.h"

/* defines */

/* globals */

#define  AT24C_MEM_DBG

/*******************************************************************************
*
* at24cMemSet - write the AT24C memory
*
* This routine copies a specified buffer into the AT24C memory.
*
* RETURNS: OK, or ERROR if the parameters are invalid or the operation fails.
*/

STATUS at24cMemSet
    (
    char *  pSrc,   /* source buffer */
    UINT32  nbytes, /* number of bytes to copy */
    UINT32  offset  /* byte offset into AT24C */
    )
    {
    STATUS  status;

    if (((nbytes + offset) > AT24C_MEM_SIZE) || !pSrc)
        return (ERROR);
            
    while (nbytes > 0)
        {
        /* every time transfer two bytes: offset & data */    
        
        status = sysI2cSend(AT24C_I2C_ADRS, offset, pSrc, 2);

        if (status != OK)
            return (status);

        pSrc++;
        offset++;
        nbytes--;
        }

    return (OK);
    }

/*******************************************************************************
*
* at24cMemGet - get the contents of AT24C memory
*
* This routine copies the contents of the AT24C memory into a specified
* buffer.
*
* RETURNS: OK, or ERROR if the parameters are invalid or the operation fails.
*/

STATUS at24cMemGet
    (
    char *  pDst,   /* destination buffer */
    INT32   nbytes, /* number of bytes to copy */
    UINT32  offset  /* byte offset into AT24C */
    )
    {
    STATUS  status = OK;

    if (((nbytes + offset) > AT24C_MEM_SIZE) || !pDst)
        return (ERROR);

   while (nbytes > 0)
       {
        /* first transfer one byte: offset address */

        status = sysI2cSend(AT24C_I2C_ADRS, offset, 0, 1);
        if (status != OK)
            return (status);

        /* receive one bytes every time */

        status = sysI2cRcv(AT24C_I2C_ADRS, pDst);
        if (status != OK)
            return (status);

        nbytes--;
        offset++;
        pDst++;
        }

    return (status);
    }

#ifdef AT24C_MEM_DBG
/*******************************************************************************
*
* at24cMemDump - dump the contents of AT24C memory
*
* This routine dumps the contents of the AT24C memory.
*
* RETURNS: N/A
*/

void at24cMemDump
    (
    UINT32  nbytes, /* number of bytes to dump */
    UINT32  offset   /* byte offset into AT24C */

    )
    {
    char *  pBuf;
    int     bufSize;
    int     i;

    if ((nbytes + offset) > AT24C_MEM_SIZE)
        return;

    bufSize = ((nbytes - 1) / 8 + 1) * 8;
    pBuf    = malloc (bufSize);
    if (pBuf == NULL)
        return;

    if (OK != at24cMemGet (pBuf, nbytes, offset))
        return;

    for(i = 0 ; i < nbytes; i += 8)
        {
        printf ("%03x: %02x %02x %02x %02x %02x %02x %02x %02x",
                i,
                pBuf[i + 0],
                pBuf[i + 1],
                pBuf[i + 2],
                pBuf[i + 3],
                pBuf[i + 4],
                pBuf[i + 5],
                pBuf[i + 6],
                pBuf[i + 7]);
        printf (" *%c%c%c%c%c%c%c%c*\n",
                isprint (pBuf[i + 0]) ? pBuf[i + 0]:'.',
                isprint (pBuf[i + 1]) ? pBuf[i + 1]:'.',
                isprint (pBuf[i + 2]) ? pBuf[i + 2]:'.',
                isprint (pBuf[i + 3]) ? pBuf[i + 3]:'.',
                isprint (pBuf[i + 4]) ? pBuf[i + 4]:'.',
                isprint (pBuf[i + 5]) ? pBuf[i + 5]:'.',
                isprint (pBuf[i + 6]) ? pBuf[i + 6]:'.',
                isprint (pBuf[i + 7]) ? pBuf[i + 7]:'.');
        }
    }
#endif /* AT24C_MEM_DBG */
