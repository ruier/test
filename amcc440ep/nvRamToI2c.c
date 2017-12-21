/* nvRamToEprom.c - non-volatile RAM to EEPROM  memory routine mapping */

/* Copyright (c) 2006-2007 Wind River Systems, Inc. 
*
* The right to copy, distribute, modify or otherwise make use
* of this software may be licensed only pursuant to the terms
* of an applicable Wind River license agreement.
*/ 

/*
modification history
--------------------
01c,05jul07,l_z  String returned from sysNvRamGet() should be terminated
                 with EOS(WIND00063183). 
01b,16mar06,mmi  Changed the mechanism of determining the iic device address.
01a,08mar06,mmi  created.
*/

/*
DESCRIPTION
This library contains non-volatile RAM manipulation routines for targets
lacking non-volatile RAM, but that do have EEPROM memory.  Read and write
wrappers are provided for the EEPROM memory routines iicReadi() and iicWrite()
*/

/* includes */

#include "drv/mem/memDev.h"

/* Imports */

IMPORT STATUS iicRead (UINT32 iicBaseAddr, 
                       UCHAR devAddr, 
                       UINT32 count, 
                       UCHAR * data);
                       
IMPORT STATUS iicWrite (UINT32 iicBaseAddr, 
                        UCHAR devAddr, 
                        UCHAR devSubAddr, 
                        UINT32 count, 
                        UCHAR * data);

/******************************************************************************
*
* sysNvRamGet - get the contents of non-volatile RAM
*
* This routine copies the contents of non-volatile memory to the specified
* destination indicated by string.
*
* RETURNS: OK or ERROR
*
* SEE ALSO: sysNvRamSet(), iicRead(), iicWrite
*/

STATUS sysNvRamGet
    (
    char * string,    /* Destination for retrieved data    */
    int    strLen,    /* number of bytes to copy           */
    int    offset     /* byte offset into non-volatile RAM */
    )
    {
    STATUS status;
    UCHAR devAddr;    /* device address */

    /* validate arguments */

    if ((offset < 0) || (strLen < 0) || ((offset + strLen) > NV_RAM_SIZE))
        return (ERROR);
        
    /* determine device address based on offset */

    devAddr = IIC0_BOOTLINE0_ADRS;
    if (offset >= IIC_NVRAM_PAGE_BLOCK_SIZE)
        {
        offset -= IIC_NVRAM_PAGE_BLOCK_SIZE;
        devAddr = IIC0_ENET_MAC_ADRS;
        }

    /* offset into the device */

    status = iicWrite(IIC0_BASE, devAddr, offset, 0, NULL) ;

    if (status == OK)
        {

        /* get the information */

        status = iicRead (IIC0_BASE, devAddr, strLen, string);
        }
    string [strLen] = EOS;
    return (status);
    }

/*******************************************************************************
*
* sysNvRamSet - write to non-volatile RAM
*
* This routine calls iicWrite() to copy a specified string into
* EEPROM device.
*
* NOTE: This routine uses EEPROM  since, there is no NVRAM on the AMCC440EP 
* EVB.
*
* RETURNS: The return value of iicWrite().
*
* SEE ALSO: sysNvRamGet(), iicWrite()
*/

STATUS sysNvRamSet
    (
    char *string,     /* string to be copied into non-volatile RAM */
    int strLen,       /* maximum number of bytes to copy           */
    int offset        /* byte offset into non-volatile RAM         */
    )
    {
    UCHAR devAddr;    /* device address */
    
    /* validate arguments */

    if ((offset < 0) || (strLen < 0) || ((offset + strLen) > NV_RAM_SIZE))
        return (ERROR);

    /* determine device address based on offset */

    devAddr = IIC0_BOOTLINE0_ADRS;
    if (offset >= IIC_NVRAM_PAGE_BLOCK_SIZE)
        {
        offset -= IIC_NVRAM_PAGE_BLOCK_SIZE;
        devAddr = IIC0_ENET_MAC_ADRS;
        }

    /* check that we don't wrap around within the page block */

    if ((offset + strLen) > IIC_NVRAM_PAGE_BLOCK_SIZE )
        return (ERROR);

    return (iicWrite (IIC0_BASE, devAddr, offset, strLen, (unsigned char *)string));
    }

