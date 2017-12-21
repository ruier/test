/* sysFlashUtil.c - FLASH Driver IF to VxWorks IO System for AMD S29GLxxxN */

/* Copyright 2005 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01a,27aug05,cak  Ported and modified from 01b, mv6100.
*/

/*
DESCRIPTION

This file contains the FLASH driver interface to the VxWorks IO system 
for the AMD S29GL512/256/128N MirrorBit Flash on the MVME3100.  It provides an
interface to the already existing flash driver (sysAmdS29GlxxxN.c) which is
shared with the TrueFFS Flash File System support.
*/

/* includes */

#include "vxWorks.h"
#include "config.h"
#include "string.h"
#include "stdio.h"
#include "sysMotVpd.h"
#include "amdS29GLxxxN.h"
#include "sysFlashUtil.h"

/* defines */

#undef	FLASH_DRV_DEBUG
#undef	FLASH_DEBUG_SHOW
#define MOT_VPD_INIT()		sysMotVpdInit()
#define NUM_FLASH_BANKS		1

/* locals */

LOCAL BOOL flashInitFlag = FALSE;
LOCAL FLASH_DEVICE sysFlashDevs [NUM_FLASH_BANKS];

#ifdef INCLUDE_SHOW_ROUTINES
static char flashShowMsgHeader[] = { "\
Device-Name Base-Address,Size Device-Size,Count Boot Type\r\n"};
static char flashShowMsgEntry[] = { "%s%-6d%08X,%08X %08X,%08X %-4s %s\r\n"};
#endif /* INCLUDE_SHOW_ROUTINES */

/* forward declarations */

LOCAL STATUS sysFlashUtilParamCheck (UINT32, UINT32, UINT32, UINT32);
LOCAL STATUS sysFlashUtilConfig (FLASH_DEVICE *, int); 
LOCAL STATUS sysFlashUtilInit ();
LOCAL STATUS sysFlashErase (FLASH_INSTANCE *, UINT32);
LOCAL int sysFlashUtilErase (FLASH_INSTANCE *, UINT32);
LOCAL STATUS sysFlashWrite (FLASH_INSTANCE *, void *, UINT32);
LOCAL STATUS sysFlashUtilDevCreate (char *, int, int);
LOCAL int sysFlashUtilDrv ();
#ifdef FLASH_DEBUG_SHOW
void sysFlashTest ();
#endif

/* externals */

IMPORT VPD_PACKET * sysVpdPkts[]; /* board vpd packets */
IMPORT void sysFlashSwWriteProtectSet (BOOL);
IMPORT BOOL sysFlashIsHwWriteProtected (void);
IMPORT BOOL sysBootBlkAIsSelected (void);
IMPORT UINT32 getFlashBaseAddress (void);

/*******************************************************************************
*
* sysFlashUtilParamCheck - parameter checking routine
*
* The purpose of this routine is to check for illegal parameters.
*
* RETURNS: OK if parameters are legal, ERROR otherwise.
*/

LOCAL STATUS sysFlashUtilParamCheck
    (
    UINT32 start,	/* start address */
    UINT32 end,		/* end address */
    UINT32 size,	/* size of FLASH bank */
    UINT32 base		/* base address of FLASH bank */
    )
    {
    if (start > (base + (size - 1)))
	{
	printf("sysFlashUtilParamCheck: address beyond flash bank\n");
	return(ERROR);
	}

    if (end > (base + (size - 1)))
	{
	printf("sysFlashUtilParamCheck: address beyond flash bank\n");
	return(ERROR);
	}

    if (start > end)
	{
	printf("sysFlashUtilParamCheck: invalid arguments\n");
	return(ERROR);
	}

    if (start < base)
	{
	printf("sysFlashUtilParamCheck: address beyond flash bank\n");
	return(ERROR);
	}
    return(OK);
    }

/*******************************************************************************
*
* sysFlashUtilConfig - configure FLASH bank 
* 
* The purpose of this routine is to gather information about the given
* FLASH bank, by reading the VPD.
*/

LOCAL STATUS sysFlashUtilConfig
    (
    FLASH_DEVICE * flashDev,	/* device structure */
    int bank			/* FLASH bank number */
    )
    {
    VPD_PACKET ** pVpdPkts;     /* board vpd packets */
    UCHAR type;                 /* board vpd packet type */
    UINT32 sectorSize;
    int numSectors;
    UINT32 manCode;
    UINT32 devCode;
    UINT32 secDevCode;

    /*
     * if the first entry in the vpd packet pointer array is null, then
     * the vpd data needs to be read into memory and parsed.
     */

    if (sysVpdPkts[0] == NULL)
        {
        printf ("Initialize VPD packets.\n\r");
        if (MOT_VPD_INIT() != OK )
            {
            printf ("VPD structure invalid.\n");
            return (ERROR);
            }
        }

    /* Initialize the vpd packet pointer and type to the first packet. */

    pVpdPkts = (VPD_PACKET **)&sysVpdPkts[0];
    type = (*pVpdPkts)->type;

    /* Find the VPD packet for the FLASH memory of the appropriate bank. */

    while (type != VPD_PID_TERM && type != VPD_PID_FMC)
        {
        pVpdPkts++;
        type = (*pVpdPkts)->type;
        }

    if (type != VPD_PID_FMC)
        {
        printf ("Cannot read the FLASH memory VPD.\n\r");
        return (ERROR);
        }
    else if ((*pVpdPkts)->data[VPD_FMC_BANK] != bank)
        {
        do
            {
            pVpdPkts++;
            type = (*pVpdPkts)->type;
            }
        while (type != VPD_PID_TERM && type != VPD_PID_FMC);
        if (type != VPD_PID_FMC)
            {
            printf ("Cannot read the FLASH memory VPD.\n\r");
            return (ERROR);
            }
        }

    /*
     * Initialize the entry in the sysFlashDevs[] array with the 
     * VPD information for the appropriate bank.
     */

    /* bank device count */

    flashDev->bankDeviceCount = (*pVpdPkts)->data[VPD_FMC_NOD];

    /* column width (bytes) */

    flashDev->wColumn = (*pVpdPkts)->data[VPD_FMC_CW] / 8;

    /* number of columns */

    flashDev->nColumns = (*pVpdPkts)->data[VPD_FMC_NOC];

    /* device/part size (bytes) */

    flashDev->deviceSize =
             (1 << ((*pVpdPkts)->data[VPD_FMC_SIZE] + 18))
              / (*pVpdPkts)->data[VPD_FMC_NOD];

    /* device/part width (bits) */

    flashDev->deviceWidth = (*pVpdPkts)->data[VPD_FMC_DDW];

    /* device write size (bytes) */

    flashDev->wSize = (*pVpdPkts)->data[VPD_FMC_WEDW] / 8;

    /* device read/write swap */

    flashDev->rwReverse = 0;

#ifdef FLASH_DEBUG_SHOW
    printf("VPD_FMC_NOD:  %d\n\r", (UINT32)(*pVpdPkts)->data[VPD_FMC_NOD]);
    printf("VPD_FMC_CW:   %d\n\r", (UINT32)(*pVpdPkts)->data[VPD_FMC_CW]);
    printf("VPD_FMC_NOC:  %d\n\r", (UINT32)(*pVpdPkts)->data[VPD_FMC_NOC]);
    printf("VPD_FMC_SIZE: %d\n\r", (UINT32)(*pVpdPkts)->data[VPD_FMC_SIZE]);
    printf("VPD_FMC_DDW:  %d\n\r", (UINT32)(*pVpdPkts)->data[VPD_FMC_DDW]);
    printf("VPD_FMC_WEDW: %d\n\r", (UINT32)(*pVpdPkts)->data[VPD_FMC_WEDW]);
#endif

    flashDev->baseAddress = getFlashBaseAddress(); 

    /* gather chip info */

    flashInfoGet(flashDev->baseAddress, &sectorSize, &numSectors);
    flashIdGet(flashDev->baseAddress, &manCode, &devCode, &secDevCode);

#ifdef FLASH_DEBUG_SHOW
    printf("sector size:         0x%x\n\r", sectorSize);
    printf("number of sectors:   0x%x\n\r", numSectors);
    printf("manufacturer ID:     0x%x\n\r", manCode);
    printf("device ID:           0x%x\n\r", devCode);
    printf("secondary device ID: 0x%x\n\r", secDevCode);
#endif

    /* sector size */

    flashDev->sectorSize = sectorSize;

    /* device name */

    switch (secDevCode)
        {
        case FMID_AMD_GL128:
	    flashDev->pDeviceName = "AMD S29GL128N MirrorBit";  
	    break;          
        case FMID_AMD_GL256:
            flashDev->pDeviceName = "AMD S29GL256N MirrorBit";
	    break; 
        default:        /* and FMID_AMD_GL512 */
	    flashDev->pDeviceName = "AMD S29GL512N MirrorBit";
	    break;
        }

    /* allocate semaphore for protecting access to FLASH bank */

    flashDev->semDrvId = semBCreate (SEM_Q_PRIORITY, SEM_FULL);

    /* Unlock the flash devices */

    if (flashNVSectorProtectPPBIsSet(flashDev->baseAddress, sectorSize,
                                     numSectors) == TRUE)
        {
        flashNVSectorProtectPPBErase(flashDev->baseAddress);
        }

    return(OK);
    }

/*******************************************************************************
*
* sysFlashUtilInit - populate sysFlashDevs[] array
*
* This routine populates the sysFlashDevs[] array with FLASH bank information.
*
* RETURNS: OK if initialization succeeded, ERROR otherwise.
*/

LOCAL STATUS sysFlashUtilInit (void)
    {
    int bank;
    FLASH_DEVICE *pFlashDev;
    STATUS status = OK;

    if (sysFlashIsHwWriteProtected() == TRUE)
        {
        printf("Flash bank is hardware write protected!\n\r");
        return (ERROR);
        }

    /* clear the SW write protection */

    sysFlashSwWriteProtectSet(FALSE);
    
    if (! flashInitFlag)
	{
	for (bank = 0; bank < NUM_FLASH_BANKS; bank++)
	    {
	    pFlashDev = &sysFlashDevs[bank];
   
	    if (sysFlashUtilConfig(pFlashDev,bank) != OK)
		status = ERROR;
	    }
	flashInitFlag = TRUE;
	}

#ifndef INCLUDE_TFFS

    /* enable SW write protection */

    sysFlashSwWriteProtectSet(TRUE);
#endif /* INCLUDE_TFFS */

    return (status);
    }

#ifdef INCLUDE_SHOW_ROUTINES
/*******************************************************************************
*
* sysFlashShow - Display FLASH bank information
*
* This routine displays information for each of the FLASH banks.
*
* RETURNS: N/A
*/

void sysFlashShow (void)
    {
    int bank;
    BOOL bootBank;

    if (! flashInitFlag)
	{
	if (sysFlashUtilInit() != OK)
	    {
	    printf("sysFlashShow: unable to initialize interface\n");
	    return;
	    }
	}

    /* display header */

    printf(flashShowMsgHeader);

    for (bank = 0; bank < NUM_FLASH_BANKS; bank++)
	{
	bootBank = sysBootBlkAIsSelected();
 
	printf(
	    flashShowMsgEntry,
	    "/flash",
	    bank,
	    sysFlashDevs[bank].baseAddress,
	    sysFlashDevs[bank].bankDeviceCount * sysFlashDevs[bank].deviceSize,
	    sysFlashDevs[bank].deviceSize,
	    sysFlashDevs[bank].bankDeviceCount,
	    bootBank ? "A" : "B",
	    sysFlashDevs[bank].pDeviceName 
	    );
	}
    }
#endif /* INCLUDE_SHOW_ROUTINES */

/*******************************************************************************
*
* sysFlashUtilOpen - driver interface to `open' routine
*
* This routine is the interface between the IO system's `open' routine
* and the FLASH driver.  Each time sysFlashUtilOpen() is called by the IO
* system, a new FLASH_INSTANCE structure is malloc'ed, and the address of
* this structure is returned to the IO system to be stored in the 
* FD (File Descriptor) table.
*
* RETURNS: Address of FLASH_INSTANCE structure for this `file' to be
*          stored in the FD table by the IO system.
*/

int sysFlashUtilOpen
    (
    FLASH_DEVICE * pFlashDev,	  /* device structure */
    char * remainder ARG_UNUSED,  /* unused, but required by IO system */
    int mode			  /* mode */
    )
    {
    FLASH_INSTANCE * pFlashInst;

    pFlashInst = (FLASH_INSTANCE *) malloc (sizeof(FLASH_INSTANCE));
    pFlashInst->flashDev = pFlashDev;
    pFlashInst->offset = 0x0;

    /* 
     * Technically speaking the mode is unused, though we do store it
     * here, as the mode is required in the IO system `open' call.
     * It could be used to restrict the type of access to the FLASH
     * parts on a per `file' basis, but this is not implememted.
     */

    pFlashInst->mode = mode;

#ifdef FLASH_DEBUG_SHOW
    printf("sysFlashUtilOpen: pFlashInst = 0x%x\n", (UINT32)pFlashInst);
#endif

    return((int)pFlashInst);
    }

/*******************************************************************************
*
* sysFlashUtilClose - driver interface to `close' routine
*
* This routine is the interface between the IO system's `close' routine
* and the FLASH driver.  The IO system passes the address of the
* FLASH_INSTANCE structure, which was malloc'ed via the open routine,
* which will subsequently be free'ed. 
* 
* RETURNS: OK always, as the free routine does not return a status. 
*/

STATUS sysFlashUtilClose
    (
    FLASH_INSTANCE * pFlashInst 	/* `file' structure */
    )
    {
#ifdef FLASH_DEBUG_SHOW
    printf("sysFlashUtilClose: pFlashInst = 0x%x\n", (UINT32)pFlashInst);
#endif
    free (pFlashInst);
    return(OK);
    }

/*******************************************************************************
*
* sysFlashUtilRead - driver interface to `read' routine
*
* This routine is the interface for the IO system's `read' routine.
* It simply performs a `memcpy', since reading the FLASH devices does
* not require the use of special driver routines.
*
* RETURNS: The number of bytes read.
*/

int sysFlashUtilRead
    (
    FLASH_INSTANCE * pFlashInst,	/* `file' structure */
    void * buffer,			/* destination buffer */
    UINT32 numBytes			/* number of bytes to read */
    )
    {
    UCHAR *address = NULL;
    FLASH_DEVICE * pFlashDev;
    UINT32 flashSize;
    UINT32 endAddr;

    pFlashDev = pFlashInst->flashDev;

    flashSize = pFlashDev->bankDeviceCount * pFlashDev->deviceSize;
    address = (UCHAR *)(pFlashDev->baseAddress + pFlashInst->offset);
    endAddr = (((UINT32)address + numBytes) - 1);

    /* error checking */

    if (sysFlashUtilParamCheck((UINT32)address, endAddr, flashSize,
			       pFlashDev->baseAddress) == ERROR)
	{
	return(ERROR);
	}

#ifdef FLASH_DEBUG_SHOW
    printf("sysFlashUtilRead: numBytes = 0x%x\n", numBytes);
    printf("sysFlashUtilRead: buffer = 0x%x\n", (UINT32)buffer);
    printf("sysFlashUtilRead: start address = 0x%x\n", (UINT32)address);
    printf("sysFlashUtilRead: end address = 0x%x\n", endAddr);
#endif

    memcpy(buffer, address, numBytes);

    return(numBytes);
    }

/*******************************************************************************
*
* sysFlashErase - routine for erasing the FLASH 
*
* This routine will erase a portion (or all) of a FLASH device. 
*
* RETURNS: OK if erase successful, ERROR otherwise. 
*/

LOCAL STATUS sysFlashErase
    (
    FLASH_INSTANCE * pFlashInst,	/* `file' structure */
    UINT32 numBytes			/* number of bytes to erase */
    )
    {
    UINT32 startAddr;
    UINT32 endAddr;
    UINT32 flashSize;
    FLASH_DEVICE * pFlashDev;

    pFlashDev = pFlashInst->flashDev;

    startAddr = pFlashDev->baseAddress + pFlashInst->offset;
    endAddr = ((startAddr + numBytes) - 1);
    flashSize = pFlashDev->bankDeviceCount * pFlashDev->deviceSize;

#ifdef FLASH_DEBUG_SHOW
    printf("sysFlashErase: initial start = 0x%x\n", startAddr);
    printf("sysFlashErase: initial end = 0x%x\n", endAddr);
    printf("sysFlashErase: flash size = 0x%x\n", flashSize);
#endif

    /* error checking */

    if (sysFlashUtilParamCheck(startAddr, endAddr, flashSize,
			       pFlashDev->baseAddress) == ERROR)
	return (ERROR);

    /* start and end addresses must be on sector boundaries */

    startAddr = ((startAddr/pFlashDev->sectorSize) * pFlashDev->sectorSize);
    endAddr = ((endAddr/pFlashDev->sectorSize) * pFlashDev->sectorSize);

#ifdef FLASH_DEBUG_SHOW
    printf("sysFlashErase: adjusted start = 0x%x\n", startAddr);
    printf("sysFlashErase: adjusted end = 0x%x\n", endAddr);
#endif

#ifndef INCLUDE_TFFS

    /* clear the SW write protection */

    sysFlashSwWriteProtectSet(FALSE);
#endif /* INCLUDE_TFFS */

    do
	{
#ifdef FLASH_DEBUG_SHOW
	printf("sysFlashErase: erase address = 0x%x\n", startAddr);
#endif
	if ( flashUtilSectorErase(pFlashDev->baseAddress, startAddr,
				  pFlashDev->sectorSize, 
				  flashSize/pFlashDev->sectorSize)!=OK )
            {
            return(ERROR);
            }
#ifdef FLASH_DEBUG_SHOW
	printf("Erase: base address = 0x%x\n", pFlashDev->baseAddress);
	printf("Erase: eraseAddr = 0x%x\n", startAddr);
	printf("Erase: sector size = 0x%x\n", pFlashDev->sectorSize);
	printf("Erase: num sectors = %d\n", (flashSize/pFlashDev->sectorSize));
#endif
	startAddr += pFlashDev->sectorSize;
	}
    while ((startAddr <= endAddr) && (startAddr != 0x0));

#ifndef INCLUDE_TFFS

    /* enable the SW write protection */

    sysFlashSwWriteProtectSet(TRUE);
#endif /* INCLUDE_TFFS */

    return(OK);
    }

/*******************************************************************************
*
* sysFlashUtilErase - driver interface to `ioctl' (FIODISKFORMAT) routine
*
* This routine will erase a portion (or all) of a FLASH device via the
* IO system's `ioctl' routine with the FIODISKFORMAT function code.
*
* RETURNS: Number of bytes erased.
*/

LOCAL int sysFlashUtilErase
    (
    FLASH_INSTANCE * pFlashInst,	/* `file' structure */
    UINT32 numBytes			/* number of bytes to erase */
    )
    {
    STATUS status;
    FLASH_DEVICE * pFlashDev;

    pFlashDev = pFlashInst->flashDev;

    /* Protect access to this resource via semaphore "take" */

    semTake (pFlashDev->semDrvId, WAIT_FOREVER);

    /* erase FLASH */

    status = sysFlashErase(pFlashInst, numBytes);

    /* "free" resource */

    semGive (pFlashDev->semDrvId);

    if (status == ERROR)
	return (ERROR);
    else
	return (numBytes);
    }

/*******************************************************************************
*
* sysFlashWrite - routine to write to the FLASH devices
*
* The FLASH driver write routines are used to write one sector at a 
* time to the FLASH device.
*
* RETURNS: OK if write successful, ERROR otherwise. 
*/

LOCAL STATUS sysFlashWrite
    (
    FLASH_INSTANCE * pFlashInst,	/* `file' structure */
    void * buffer,			/* source buffer */
    UINT32 numBytes			/* number of bytes to write */
    )
    {
    UINT32 startAddr;
    UINT32 sectorStartAddr;
    UINT32 *sectorBuff;
    UINT32 numBytesToBuffer;
    UCHAR * bufferAddr;
    UINT32 bytes;
    UINT32 loopCount;
    UCHAR * tempPtr;
    FLASH_DEVICE * pFlashDev;
    UINT32 x;
    UINT32 flashSize;

    bytes = numBytes;
    bufferAddr = buffer;
    pFlashDev = pFlashInst->flashDev;
    sectorBuff = malloc(pFlashDev->sectorSize);
    memset (sectorBuff, 0x0, pFlashDev->sectorSize);
    startAddr = pFlashDev->baseAddress + pFlashInst->offset;
    sectorStartAddr = ((startAddr/pFlashDev->sectorSize) * 
			pFlashDev->sectorSize);
    numBytesToBuffer = startAddr - sectorStartAddr;
    flashSize = pFlashDev->bankDeviceCount * pFlashDev->deviceSize;

#ifndef INCLUDE_TFFS

    /* Disable SW write protection */

    sysFlashSwWriteProtectSet(FALSE);
#endif /* INCLUDE_TFFS */

    /* Write to the FLASH device */

    do
	{
	tempPtr = (UCHAR *)sectorBuff;

	/* clear the buffer */

	memset(tempPtr, 0x0, pFlashDev->sectorSize);

	if (numBytesToBuffer > 0)
	    {
	    tempPtr += numBytesToBuffer;

	    if (bytes >= (pFlashDev->sectorSize - numBytesToBuffer))
		{
		loopCount = (pFlashDev->sectorSize - numBytesToBuffer);
		bytes -= (pFlashDev->sectorSize - numBytesToBuffer);
		}
	    else
		{
		loopCount = bytes;
		bytes = 0;
		}

	    numBytesToBuffer = 0;
	    }
	else
	    {
	    if (bytes >= pFlashDev->sectorSize)
		{
		loopCount = pFlashDev->sectorSize; 
		bytes -= pFlashDev->sectorSize;
		}
	    else
		{
		loopCount = bytes;
		bytes = 0;
		}
	    }

	/* transfer data from source buffer to sector buffer */

	for (x = 0; x < loopCount; ++x)
	    {
	    *tempPtr = *bufferAddr;
	    ++tempPtr;
	    ++bufferAddr;
	    }

	/* write one sector to FLASH parts */

	flashUtilSectorProgram(pFlashDev->baseAddress,
			       sectorStartAddr, (char *)sectorBuff, 
			       pFlashDev->sectorSize, pFlashDev->sectorSize,
			       flashSize/pFlashDev->sectorSize);

#ifdef FLASH_DEBUG_SHOW
        printf("sysFlashWrite: sector adrs = 0x%x\n", sectorStartAddr);
	printf("sysFlashWrite: buffer adrs = 0x%x\n", (UINT32)sectorBuff);
#endif
 
	sectorStartAddr += pFlashDev->sectorSize;
	}
    while (bytes > 0);

#ifndef INCLUDE_TFFS

    /* enable SW write protection */

    sysFlashSwWriteProtectSet(TRUE);
#endif /* INCLUDE_TFFS */

    free (sectorBuff);

    return(OK);
    }

/*******************************************************************************
*
* sysFlashUtilWrite - driver interface to `write' routine
*
* This routine is the interface for the IO system's `write' routine.
*
* RETURNS: number of bytes written.
*/

int sysFlashUtilWrite
    (
    FLASH_INSTANCE * pFlashInst,	/* `file' structure */
    void * buffer,			/* source buffer */
    UINT32 numBytes			/* number of bytes to write */
    )
    {
    STATUS status;
    FLASH_DEVICE * pFlashDev;

    pFlashDev = pFlashInst->flashDev;

    /* Protect access to this resource via semaphore "take" */

    semTake (pFlashDev->semDrvId, WAIT_FOREVER);

    /* write to FLASH, erase first */

    if (sysFlashErase(pFlashInst, numBytes) == OK)
	{
	status = sysFlashWrite(pFlashInst, buffer, numBytes);
	}
    else
	{
	printf("sysFlashUtilWrite: Unable to erase\n");
	status = ERROR;
	}

    /* "free" resource */

    semGive (pFlashDev->semDrvId);

    if (status == ERROR)
	return (ERROR);
    else
	return (numBytes);
    }

/*******************************************************************************
*
* sysFlashUtilIoctl - driver interface to `ioctl' routine
*
* This routine is the interface for the IO system's `ioctl' routine.
* This is used to erase the device and to set an `offset' into the FLASH
* bank for performing reads, writes, and erases.
*
* Valid function codes are:
*
* FIODISKFORMAT - to erase
* FIOSEEK - to specify an offset into the FLASH bank
* FIONREAD - return size of the FLASH bank
*
* RETURNS: OK if operation succeeds , ERROR otherwise or if 
* unsupported function code is specified.
*/

int sysFlashUtilIoctl
    (
    FLASH_INSTANCE * pFlashInst,	/* `file' structure */
    UINT32 func,			/* function code */
    UINT32 arg				/* argument */
    )
    {
    UINT32 flashSize;
    FLASH_DEVICE * pFlashDev;
    UINT32 * pFlashSize = NULL;

    pFlashDev = pFlashInst->flashDev;

    flashSize = pFlashDev->bankDeviceCount * pFlashDev->deviceSize;

    switch (func)
	{
	case FIODISKFORMAT:

	    /* erase flash device */

	    if(sysFlashUtilErase(pFlashInst,arg) == ERROR)
		{
		printf("sysFlashUtilIoctl: error erasing device\n");
		return(ERROR);
		}

	    break;

	case FIOSEEK:

	    /* set offset into flash device */

	    if(arg >= flashSize)
		{
		printf("sysFlashUtilIoctl: offset exceeds bank size\n");
		return(ERROR);
		}

	    pFlashInst->offset = arg;

	    break;

	case FIONREAD:

	    /* return size of flash device */

	    pFlashSize = (UINT32 *)arg;
	    *pFlashSize = flashSize;

	    break;

	default:

	    printf("sysFlashUtilIoctl: unsupported function code\n");
	    return(ERROR);

	}
    return(OK);
    }

/*******************************************************************************
*
* sysFlashUtilDevCreate - Add a device to the VxWorks device list
*
* This routine adds a device (FLASH bank) to the VxWorks device list,
* to be serviced by this FLASH driver, via the iosDevAdd() routine.
* Each device has an entry in the sysFlashDevs[] array, which stores
* device specific information for each FLASH bank.
*
* RETURNS: OK if device was successfully added, ERROR otherwise.
*/

LOCAL STATUS sysFlashUtilDevCreate
    (
    char * name,	/* device name */
    int bank,		/* FLASH bank number */
    int flashDrvNum	/* index into device table */
    )
    {
    FLASH_DEVICE *pFlashDev;

    if (sysFlashUtilInit() != OK)
	return (ERROR);

    pFlashDev = &sysFlashDevs[bank];

    return (iosDevAdd(&pFlashDev->devHdr, name, flashDrvNum));
    }

/*******************************************************************************
*
* sysFlashUtilDrv - Add FLASH driver to VxWorks driver table
*
* This routine adds the FLASH driver to the VxWorks driver table via
* iosDrvInstall().  iosDrvInstall() returns an integer which is an index
* into the driver table.  This routine only needs to be called one time.
*
* RETURNS: Index into driver table if driver was successfully added, 
* ERROR otherwise.
*/

LOCAL int sysFlashUtilDrv (void)
    {
    int flashDrvNum;

    flashDrvNum = iosDrvInstall(sysFlashUtilOpen, sysFlashUtilClose, 
	sysFlashUtilOpen, sysFlashUtilClose, sysFlashUtilRead,
	sysFlashUtilWrite, sysFlashUtilIoctl);

    return (flashDrvNum == ERROR ? ERROR : flashDrvNum);
    }

/*******************************************************************************
*
* sysFlashDevsMount - mount the FLASH devices
*
* This routine mounts the FLASH devices and makes them available to the
* VxWorks IO system.
*
* RETURNS: OK if mount is successful, ERROR otherwise.
*/

STATUS sysFlashDevsMount (void)
    {
    int flashDrvNum;

    if ((flashDrvNum = sysFlashUtilDrv()) == ERROR)
	return (ERROR);
    else
	{
	if (sysFlashUtilDevCreate("/flash0",0, flashDrvNum) != OK)
	    return (ERROR);
	}

    return (OK);
    }

#ifdef FLASH_DRV_DEBUG
/*******************************************************************************
*
* sysFlashTest - routine for testing driver interface
*
* This routine is intended to test the driver interface.
*
* RETURNS: N/A
*/

void sysFlashTest (void)
    {
    UINT32 *readBuffer = NULL;
    UINT32 numBytes = 0x100000; /* 1MB */
    UINT32 offset = 0x20;
    int fd;
    int bytes;
    UINT32 count;
    UINT32 *writeBuffer = NULL;
    UCHAR * pFill;
    BOOL mismatch = FALSE;
    UINT32 compareCount;
    UINT32 *restoreBuffer = NULL;
    UINT32 * pRead;
    UINT32 * pWrite;

    printf("Testing FLASH driver interface...\n\r");
    printf("\n");

    /* allocate read, write, and restore buffers */

    readBuffer = (UINT32 *) malloc (numBytes);
    writeBuffer = (UINT32 *) malloc (numBytes);
    restoreBuffer = (UINT32 *) malloc (numBytes*2);

    memset(readBuffer, 0x0, numBytes);
    memset(writeBuffer, 0x0, numBytes);
    memset(restoreBuffer, 0x0, numBytes*2);

    printf("sysFlashTest: readBuffer = 0x%x\n", (UINT32)readBuffer);
    printf("sysFlashTest: writeBuffer = 0x%x\n", (UINT32)writeBuffer);
    printf("sysFlashTest: restoreBuffer = 0x%x\n", (UINT32)restoreBuffer);

    /* fill a source buffer to be written to FLASH banks */

    pFill = (UCHAR *)writeBuffer;
    for (count = 0; count < numBytes; ++count)
	{
	*pFill = (UINT32)((count + 0xee) & 0xFF);
	++pFill;
	}

    /* mount the FLASH devices */

    printf("sysFlashTest: Mounting FLASH devices...\n\r");
    sysFlashDevsMount();

    /* Display FLASH bank configuration information */

#ifdef INCLUDE_SHOW_ROUTINES
    sysFlashShow();
#endif /* INCLUDE_SHOW_ROUTINES */

    /* `Open' file to FLASH bank 0 */

    printf("sysFlashTest: Opening FLASH bank 0...\n\r");

    fd = open ("/flash0", O_RDWR, 0644);

    /* save contents of FLASH bank to be restored at end of test */

    printf("sysFlashTest: Reading FLASH bank 0 (to restore later)...\n\r");

    bytes = read (fd, (char *)restoreBuffer, numBytes*2);

    /* test "seek" functionality for setting an offset into FLASH bank */

    printf("sysFlashTest: Testing seek functionality...\n\r");

    ioctl (fd, FIOSEEK, offset);

    /* test "erase" functionality */

    printf("sysFlashTest: Testing erase functionality...\n\r");

    ioctl (fd, FIODISKFORMAT, numBytes);

    /* test "write" functionality */

    printf("sysFlashTest: Testing write functionality...\n\r");

    bytes = write (fd, (char *)writeBuffer, numBytes);

    /* test "read" functionality */

    printf("sysFlashTest: Testing read functionality...\n\r");

    bytes = read (fd, (char *)readBuffer, numBytes);

    /* compare "read" and "write" buffers for data integrity */

    printf("sysFlashTest: Checking for data integrity...\n\r");

    compareCount = numBytes;
    pRead = readBuffer;
    pWrite = writeBuffer;

    while (!mismatch && compareCount > 0 )
        {
        mismatch = (*pRead != *pWrite);

        if (mismatch)
            {
            printf ("Mismatch @ 0x%08x, expected 0x%x, found 0x%x\r\n",
                    (UINT32)compareCount, *pRead, *pWrite);
            }
        pRead++;
        pWrite++;

        compareCount -= 4;
        }

    /* restore FLASH contents */

    printf("sysFlashTest: Restoring contents of FLASH bank 0...\n\r");

    ioctl (fd, FIOSEEK, 0x0);
    bytes = write (fd, (char *)restoreBuffer, numBytes*2);

    /* `close' file to FLASH bank 0 */

    printf("sysFlashTest: Closing FLASH bank 0...\n\r");

    close (fd);

    /* clear read buffer */

    memset(readBuffer, 0x0, numBytes);

    /* free read & write buffers to clean up after test */

    free (restoreBuffer);
    free (writeBuffer);
    free (readBuffer);
    printf("sysFlashTest: Test Complete\n\r");
    }
#endif /* FLASH_DRV_DEBUG */
