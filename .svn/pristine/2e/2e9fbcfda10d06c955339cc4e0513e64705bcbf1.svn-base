/* sysFlashUtil.h - Flash Utility Header File */

/*
 * Copyright (c) 2005 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2005 Motorola, Inc., All Rights Reserved */

/*
modification history
--------------------
01a,27aug05,cak  Created.
*/

/*
DESCRIPTION

This file contains definitions for the flash utility.
*/

#ifndef INCsysFlashUtilh
#define INCsysFlashUtilh

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

/* typedefs */

typedef struct flashDevice
    {
    DEV_HDR     devHdr;         /* I/O device header */
    UINT32 baseAddress;         /* base address of FLASH bank */
    UINT32 bankDeviceCount;     /* device count */
    UINT32 wColumn;             /* column width */
    UINT32 nColumns;            /* number of columns */
    UINT32 deviceSize;          /* device size */
    UINT32 deviceWidth;         /* device width */
    UINT32 wSize;               /* write size */
    UINT32 rwReverse;           /* read/write swap */
    UINT32 sectorSize;          /* sector size */
    UCHAR * pDeviceName;        /* device name */
    SEM_ID  semDrvId;           /* driver access semaphore */
    } FLASH_DEVICE;

typedef struct flashInstance
    {
    FLASH_DEVICE * flashDev;
    UINT32 offset;              /* offset into FLASH bank */
    int mode;                   /* mode */
    } FLASH_INSTANCE;

#ifdef INCLUDE_SHOW_ROUTINES
void sysFlashShow ();
#endif /* INCLUDE_SHOW_ROUTINES */
int sysFlashUtilOpen (FLASH_DEVICE *, char *, int);
STATUS sysFlashUtilClose (FLASH_INSTANCE *);
int sysFlashUtilRead (FLASH_INSTANCE *, void *, UINT32);
int sysFlashUtilWrite (FLASH_INSTANCE *, void *, UINT32);
int sysFlashUtilIoctl (FLASH_INSTANCE *, UINT32, UINT32);
STATUS sysFlashDevsMount ();

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* INCsysFlashUtilh */
