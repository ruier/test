/* sysTffsStub.c - BSP stub for TrueFFS Socket Component Driver */

/*
 * Copyright (c) 2000-2002, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01g,21jan13,y_y  added SPI flash TFFS support.
01f,08jan13,x_z  added interface to vxBus Flash driver based
                 wrSbcP5020/sysTffs.c.
01e,22may02,nrv  Adding INCLUDE_MTD_CFIAMD
01d,13nov01,nrv  made use of PRJ_BUILD
01c,09nov01,nrv  merging in T3 version with some cleanup
01b,14dec00,yp   Documentation cleanup
01a,29nov00,yp   derived from ads860 sysTffs.c 01j.
*/

/*
This stub file provides the user with the means to create a TrueFFS compliant
socket component driver for use with any BSP. If This is sample code and it is
expected, even required, to be modified after it is added to a BSP by the
project facility when the component INCLUDE_TFFS is added to a kernel project.
Look for #error and TODO statements in the sample code.

This stub does not provides code, just some procedural macros that the
generic driver code will use.

Note that if INCLUDE_TFFS_VXBFLASH is defined with INCLUDE_TFFS in config.h,
this stub file can be used with vxBus Flash drivers directly.

DESCRIPTION
This library must provide board-specific hardware access routines for TrueFFS.
In effect, these routines comprise the socket component driver (or drivers)
for your flash device.  At socket registration time, TrueFFS stores
the functions provided by this socket component driver in an 'FLSocket'
structure.  When TrueFFS needs to access the flash device, it uses these
functions.

Because this file is, for the most part, a device driver that exports its
functionality by registering function pointers with TrueFFS, very few of the
functions defined here are externally callable.  For the record, the only
external functions are flFitInSocketWindow() and flDelayLoop(), and you should
never have to call them.

However, one of the most important functions defined in this file is neither
referenced in an 'FLSocket' structure, nor is it externally callable.  This
function is sysTffsInit() and it should only be called by TrueFFS.  TrueFFS
calls this function at initialization time to register socket component
drivers for all the flash devices attached to your target.  It is this call
to sysTffs() that results in assigning drive numbers to the flash devices on
your target hardware.  Drive numbers are assigned by the order in which the
socket component drivers are registered. The first to be registered is drive
0, the second is drive 1, and so on up to 4.  As shipped, TrueFFS supports up
to five flash drives.

After registering socket component drivers for a flash device, you may
format the flash medium even though there is not yet a block device driver
associated with the flash (see the reference entry for the tffsDevCreate()
routine).  To format the flash medium for use with TrueFFS,
call tffsDevFormat() or, for some BSPs, sysTffsFormat().

The sysTffsFormat() routine is an optional,BSP-specific helper routine that
can be called externally. Internally, sysTffsFormat() calls tffsDevFormat()
with a pointer to a 'FormatParams' structure that is initialized to values
that leave a space on the flash device for a boot image. This space is outside
the region managed by TrueFFS.  This special region is necessary for boot
images because the normal translation and wear-leveling services of TrueFFS
are incompatible with the needs of the boot program and the boot image it
relies upon.  To write a boot image (or any other data) into this area,
use tffsBootImagePut().

The function sysTffsFormat() is only provided when a Flash SIMM has to have
the TrueFFS file system in some desired fraction of it. It is provided only
for the purpose of simplifying the process of formatting a Flash part that
that should be subdivided.

The Flash SIMM might also be referred to as RFA (Resident Flash Array) in the
following text.

Example implentations of sysTffs.c can be found in the directory

    $(WIND_BASE)/target/src/drv/tffs/sockets

The files sds860-sysTffs.c and pc386-sysTffs.c have support for single and dual
socketed PCMCIA devices as well if that might be useful to you. They both
support multiple sockets.


Finally, this file also contains define statements for symbolic constants
that determine which MTDs, translation layer modules, and other utilities
are ultimately included in TrueFFS.  These defines are as follows:

.IP "INCLUDE_TL_FTL"
To include the NOR-based translation layer module.
.IP "INCLUDE_TL_SSFDC"
To include the SSFDC-appropriate translation layer module.
.IP "INCLUDE_MTD_I28F016"
For Intel 28f016 flash devices.
.IP "INCLUDE_MTD_I28F008"
For Intel 28f008 flash devices.
.IP "INCLUDE_MTD_I28F008_BAJA"
For Intel 28f008 flash devices on the Heurikon Baja 4700.
.IP "INCLUDE_MTD_AMD"
For AMD, Fujitsu: 29F0{40,80,16} 8-bit flash devices.
.IP "INCLUDE_MTD_CFIAMD"
For CFI compliant AMD, Fujitsu flash devices.
.IP "INCLUDE_MTD_CFISCS"
For CFI/SCS flash devices.
.IP "INCLUDE_MTD_WAMD"
For AMD, Fujitsu 29F0{40,80,16} 16-bit flash devices.
.IP "INCLUDE_TFFS_BOOT_IMAGE"
To include tffsBootImagePut() in TrueFFS for Tornado.
.LP
To exclude any of the modules mentioned above, edit sysTffs.c and undefine
its associated symbolic constant.


INCLUDE FILES: flsocket.h, tffsDrv.h
*/

#ifdef INCLUDE_TFFS

/* includes */

#include <vxWorks.h>
#include <taskLib.h>
#include "config.h"
#include "tffs/flsocket.h"
#include "tffs/tffsDrv.h"

#ifdef INCLUDE_TFFS_STUB_VXBFLASH

#include <logLib.h>
#include <tffs/flflash.h>
#include <../src/hwif/h/flash/vxbFlash.h>
#include <../src/hwif/h/flash/vxbFlashCommon.h>
#include <vxBusLib.h>
#include <hwif/util/vxbParamSys.h>

/* defines */

#undef  INCLUDE_MTD_I28F016         /* Intel: 28f016 */
#undef  INCLUDE_MTD_I28F008         /* Intel: 28f008 */
#undef  INCLUDE_MTD_AMD             /* AMD, Fujitsu: 29f0{40,80,16} 8bit */
#undef  INCLUDE_MTD_CFIAMD          /* CFI driver for AMD Flash Part */
#undef  INCLUDE_MTD_CFISCS          /* CFI/SCS */
#undef  INCLUDE_MTD_WAMD            /* AMD, Fujitsu: 29f0{40,80,16} 16bit */
#define INCLUDE_TL_FTL              /* FTL translation layer */
#undef  INCLUDE_TL_SSFDC            /* SSFDC translation layer */
#undef  INCLUDE_SOCKET_SIMM         /* SIMM socket interface */
#undef  INCLUDE_SOCKET_PCMCIA       /* PCMCIA socket interface */

/* define this MACRO to include MTD driver */

#define INCLUDE_MTD_USR
#define MTD_USR_IDENTIFY    sysTffsIdentify

/*
 * Don't define TFFS_PARTn_FLASH_BOOT_OFFSET if you don't use TrueFFS to write
 * your boot image, and tffsBootImagePut() can be used only before TrueFFS
 * file system is mounted.
 */

#if defined(TFFS_PART0_FLASH_BOOT_SIZE) || \
    defined(TFFS_PART1_FLASH_BOOT_SIZE) || \
    defined(TFFS_PART2_FLASH_BOOT_SIZE) || \
    defined(TFFS_PART3_FLASH_BOOT_SIZE) || \
    defined(TFFS_PART4_FLASH_BOOT_SIZE)
#define INCLUDE_TFFS_BOOT_IMAGE
#else /* !TFFS_PARTn_FLASH_BOOT_OFFSET */
#undef  INCLUDE_TFFS_BOOT_IMAGE      /* include tffsBootImagePut() */
#endif /* TFFS_PARTn_FLASH_BOOT_OFFSET */

/*
 * The number of the driver instance must not be larger than the maximum FL
 * socket number.
 */

#define FLASH_TFFS_PART_NUM     DRIVES

/*
 * The size of TrueFFS partition must be aligned with 4K bytes, that is required
 * by TrueFFS.
 */

#define FLASH_TFFS_PART_ALIGN   0x1000
#define FLASH_TFFS_PART_SHIFT   12

#undef FLASH_TFFS_DBG
#ifdef FLASH_TFFS_DBG
#   define TFFS_DBG_MSG(fmt, a, b, c, d, e, f)  \
    do {                    \
    if (_func_logMsg != NULL)       \
        _func_logMsg ("sysTffs: "fmt, \
              (_Vx_usr_arg_t)a, \
              (_Vx_usr_arg_t)b, \
              (_Vx_usr_arg_t)c, \
              (_Vx_usr_arg_t)d, \
              (_Vx_usr_arg_t)e, \
              (_Vx_usr_arg_t)f); \
    } while (FALSE)
#else /* !FLASH_TFFS_DBG */
#   define TFFS_DBG_MSG(fmt, ...)
#endif /* FLASH_TFFS_DBG */

/* Flash functions */

#define FLASH_DEV_OFFSET(pPartInfo, tffsOffset)    \
        ((pPartInfo)->flashOffset + (tffsOffset))

#define FLASH_TFFS_RST(pPartInfo)    \
        (pPartInfo)->pFlashChip->flashOPs.rst ((pPartInfo)->pFlashChip)

#define FLASH_TFFS_ENA(pPartInfo)    \
        (pPartInfo)->pFlashChip->flashOPs.ena ((pPartInfo)->pFlashChip, \
        TRUE, NULL)
#define FLASH_TFFS_DIS(pPartInfo)    \
        (pPartInfo)->pFlashChip->flashOPs.ena ((pPartInfo)->pFlashChip, \
        FALSE, NULL)

#define FLASH_TFFS_LOCKQRY(pPartInfo, blkOffset, pLock)    \
        (pPartInfo)->pFlashChip->flashOPs.blkLockQry ((pPartInfo)->pFlashChip, \
        FLASH_DEV_OFFSET (pPartInfo, blkOffset), pLock)

#define FLASH_TFFS_LOCK(pPartInfo, blkOffset)    \
        (pPartInfo)->pFlashChip->flashOPs.blkLock ((pPartInfo)->pFlashChip, \
        FLASH_DEV_OFFSET (pPartInfo, blkOffset), TRUE)
#define FLASH_TFFS_UNLOCK(pPartInfo, blkOffset)    \
        (pPartInfo)->pFlashChip->flashOPs.blkLock ((pPartInfo)->pFlashChip, \
        FLASH_DEV_OFFSET (pPartInfo, blkOffset), FALSE)

#define FLASH_TFFS_READ(pPartInfo, offset, bufLen, ppbuf)    \
        (pPartInfo)->pFlashChip->flashOPs.read ((pPartInfo)->pFlashChip,   \
        FLASH_DEV_OFFSET (pPartInfo, offset), 1, bufLen, (ppbuf), NULL)

#define FLASH_TFFS_WRITE(pPartInfo, offset, bufLen, ppbuf)    \
        (pPartInfo)->pFlashChip->flashOPs.write((pPartInfo)->pFlashChip,   \
        FLASH_DEV_OFFSET (pPartInfo, offset), 1, bufLen, (ppbuf), NULL)

#define FLASH_TFFS_ERASE(pPartInfo, blkOffset, blkCnt)    \
        (pPartInfo)->pFlashChip->flashOPs.blkErase((pPartInfo)->pFlashChip,   \
        FLASH_DEV_OFFSET (pPartInfo, blkOffset), blkCnt)

/*
 * set these to board specific values.
 * The values used here are fictional.
 */

#define VCC_DELAY_MSEC      100 /* Millisecs to wait for Vcc ramp up */
#define VPP_DELAY_MSEC      100 /* Millisecs to wait for Vpp ramp up */
#define KILL_TIME_FUNC      ((iz * iz) / (iz + iz)) / ((iy + iz) / (iy * iz))

/* typedefs */

/* TrueFFS partition information */

typedef struct
    {
    /* saved information */

    char *          pPartName;
    char *          pFlashName;
    UINT32          flashUnit;

    /* normal region offset information */

    UINT32          flashBase;
    UINT32          flashOffset;
    UINT32          flashSize;  /* total size */
    UINT32          bootSize;   /* reseved size */

    /* Partition information */

    FLASH_CHIP_ID   pFlashChip;
    UINT32          blkNum;
#ifdef TFFS_BOOT_LOCK    
    BOOL            bootLocked;
#endif /* TFFS_BOOT_LOCK */
    UINT8 *         pLockArray; /* block lock status array */
                                /* 1 - locked 0 - unlocked */
    } TFFS_PART_INFO;

typedef struct 
    {
    UINT8 mapBuffer[SECTOR_SIZE];   /* buffer for map through buffer */
    } Vars;

LOCAL Vars mtdVars[DRIVES];

#define thisVars   ((Vars *) vol.mtdVars)
#define thisBuffer (thisVars->mapBuffer)

/* locals */

LOCAL UINT32 sysTffsMsecLoopCount = 0;

LOCAL FLStatus sysTffsUnlock
    (
    TFFS_PART_INFO *    pPartInfo,
    UINT32              offset,
    UINT32              len
    );

/* Driver utility functions */

LOCAL FLStatus sysTffsWrite
    (
    FLFlash             vol,
    CardAddress         address,
    const void FAR1 *   buffer,
    int                 length,
    int                 overwrite
    );
LOCAL FLStatus sysTffsErase
    (
    FLFlash vol,
    int     firstErasableBlock,
    int     numOfErasableBlocks
    );

LOCAL FLStatus sysTffsRead
    (
    FLFlash             vol,
    CardAddress         address,
    const void FAR1 *   buffer,
    int                 length,
    int                 overwrite
    );

LOCAL  void FAR0 * sysTffsMap 
    (
    FLFlash vol,
    CardAddress address,
    int length
    );

/* Structures */

LOCAL TFFS_PART_INFO tffsPartTbl[FLASH_TFFS_PART_NUM];

/* forward declarations */

LOCAL void      rfaRegister (void);
LOCAL FLStatus  sysTffsIdentify (FLFlash vol);
LOCAL FLBoolean rfaCardDetected (FLSocket vol);
LOCAL void      rfaVccOn (FLSocket vol);
LOCAL void      rfaVccOff (FLSocket vol);
#ifdef  SOCKET_12_VOLTS
LOCAL FLStatus  rfaVppOn (FLSocket vol);
LOCAL void      rfaVppOff (FLSocket vol);
#endif  /* SOCKET_12_VOLTS */
LOCAL FLStatus  rfaInitSocket (FLSocket vol);
LOCAL void      rfaSetWindow (FLSocket vol);
LOCAL void      rfaSetMappingContext (FLSocket vol, unsigned page);
LOCAL FLBoolean rfaGetAndClearCardChangeIndicator (FLSocket vol);
LOCAL FLBoolean rfaWriteProtected (FLSocket vol);

/* forward declarations */

#ifndef DOC
#   include "tffs/tffsConfig.c"
#endif /* DOC */

/*******************************************************************************
*
* sysTffsInit - board level initialization for TFFS
*
* This routine calls the socket registration routines for the socket component
* drivers that will be used with this BSP. The order of registration signifies
* the logical drive number assigned to the drive associated with the socket.
*
* RETURNS: N/A
*/

LOCAL void sysTffsInit (void)
    {
    UINT32 ix = 0;
    UINT32 iy = 1;
    UINT32 iz = 2;
    int oldTick;

    /*
     * we assume followings:
     *   - no interrupts except timer is happening.
     *   - the loop count that consumes 1 msec is in 32 bit.
     * it is done in the early stage of usrRoot() in tffsDrv().
     */

    oldTick = tickGet();
    while (oldTick == tickGet())    /* wait for next clock interrupt */
    ;

    oldTick = tickGet();
    while (oldTick == tickGet())    /* loop one clock tick */
    {
    iy = KILL_TIME_FUNC;            /* consume time */
    ix++;                           /* increment the counter */
    }

    sysTffsMsecLoopCount = ix * sysClkRateGet() / 1000;

    /* Set up partition table */

    bzero ((char *) tffsPartTbl, sizeof (TFFS_PART_INFO) * FLASH_TFFS_PART_NUM);

#ifdef TFFS_PART0_NAME
    tffsPartTbl[0].pPartName = TFFS_PART0_NAME;
    tffsPartTbl[0].pFlashName = TFFS_PART0_FLASH_NAME;
    tffsPartTbl[0].flashUnit = TFFS_PART0_FLASH_UNIT;
    tffsPartTbl[0].flashBase = TFFS_PART0_FLASH_BASE_ADRS;
    tffsPartTbl[0].flashOffset = TFFS_PART0_FLASH_OFFSET;
    tffsPartTbl[0].flashSize = TFFS_PART0_FLASH_SIZE;
    tffsPartTbl[0].bootSize = TFFS_PART0_FLASH_BOOT_SIZE;
#endif /* TFFS_PART0_NAME */

#ifdef TFFS_PART1_NAME
    tffsPartTbl[1].pPartName = TFFS_PART1_NAME;
    tffsPartTbl[1].pFlashName = TFFS_PART1_FLASH_NAME;
    tffsPartTbl[1].flashUnit = TFFS_PART1_FLASH_UNIT;
    tffsPartTbl[1].flashBase = TFFS_PART1_FLASH_BASE_ADRS;
    tffsPartTbl[1].flashOffset = TFFS_PART1_FLASH_OFFSET;
    tffsPartTbl[1].flashSize = TFFS_PART1_FLASH_SIZE;
    tffsPartTbl[1].bootSize = TFFS_PART1_FLASH_BOOT_SIZE;
#endif /* TFFS_PART1_NAME */

#ifdef TFFS_PART2_NAME
    tffsPartTbl[2].pPartName = TFFS_PART2_NAME;
    tffsPartTbl[2].pFlashName = TFFS_PART2_FLASH_NAME;
    tffsPartTbl[2].flashUnit = TFFS_PART2_FLASH_UNIT;
    tffsPartTbl[2].flashBase = TFFS_PART2_FLASH_BASE_ADRS;
    tffsPartTbl[2].flashOffset = TFFS_PART2_FLASH_OFFSET;
    tffsPartTbl[2].flashSize = TFFS_PART2_FLASH_SIZE;
    tffsPartTbl[2].bootSize = TFFS_PART2_FLASH_BOOT_SIZE;
#endif /* TFFS_PART2_NAME */

#ifdef TFFS_PART3_NAME
    tffsPartTbl[3].pPartName = TFFS_PART3_NAME;
    tffsPartTbl[3].pFlashName = TFFS_PART3_FLASH_NAME;
    tffsPartTbl[3].flashUnit = TFFS_PART3_FLASH_UNIT;
    tffsPartTbl[3].flashBase = TFFS_PART3_FLASH_BASE_ADRS;
    tffsPartTbl[3].flashOffset = TFFS_PART3_FLASH_OFFSET;
    tffsPartTbl[3].flashSize = TFFS_PART3_FLASH_SIZE;
    tffsPartTbl[3].bootSize = TFFS_PART3_FLASH_BOOT_SIZE;
#endif /* TFFS_PART3_NAME */

#ifdef TFFS_PART4_NAME
    tffsPartTbl[4].pPartName = TFFS_PART4_NAME;
    tffsPartTbl[4].pFlashName = TFFS_PART4_FLASH_NAME;
    tffsPartTbl[4].flashUnit = TFFS_PART4_FLASH_UNIT;
    tffsPartTbl[4].flashBase = TFFS_PART4_FLASH_BASE_ADRS;
    tffsPartTbl[4].flashOffset = TFFS_PART4_FLASH_OFFSET;
    tffsPartTbl[4].flashSize = TFFS_PART4_FLASH_SIZE;
    tffsPartTbl[4].bootSize = TFFS_PART4_FLASH_BOOT_SIZE;
#endif /* TFFS_PART4_NAME */

    /* Call each sockets register routine here */

    rfaRegister ();  /* RFA socket interface register */
    }

/*******************************************************************************
*
* rfaRegister - install routines for the Flash RFA
*
* This routine installs necessary functions for the Resident Flash Array(RFA).
*
* RETURNS: N/A
*/

LOCAL void rfaRegister (void)
    {
    UINT32          i;
    UINT32          j;
    BOOL            lock;
    UINT32          blkAdrs;
    VXB_DEVICE_ID   pFlashDev;
    FLSocket *      pFlSocket;
    FLASH_CHIP_ID   (*flashChipInfoGet) (VXB_DEVICE_ID pDev, UINT32 chipId);

    for (i = 0; (i < FLASH_TFFS_PART_NUM) && (noOfDrives < FLASH_TFFS_PART_NUM);
        i++)
        {
        if (tffsPartTbl[i].pPartName == NULL)
            continue;

        /* Find driver instance */

        pFlashDev =
            vxbInstByNameFind (tffsPartTbl[i].pFlashName,
                               tffsPartTbl[i].flashUnit);
        if (pFlashDev != NULL)
            {

            /* Obtain Flash chip information */

            flashChipInfoGet =
                vxbDevMethodGet (pFlashDev,
                                 DEVMETHOD_CALL (vxbFlashChipInfoGet));

            tffsPartTbl[i].pFlashChip = flashChipInfoGet (pFlashDev, 0);

            /* Check Flash offset and size */

            if ((tffsPartTbl[i].flashOffset >=
                tffsPartTbl[i].pFlashChip->uChipSize) ||
                ((tffsPartTbl[i].flashOffset + tffsPartTbl[i].flashSize) >
                tffsPartTbl[i].pFlashChip->uChipSize))
                {
                TFFS_DBG_MSG ("Normal region is Out of range for %s%d\n",
                               tffsPartTbl[i].pFlashName,
                               tffsPartTbl[i].flashUnit, 0, 0, 0, 0);
                return;
                }

            if (((tffsPartTbl[i].flashOffset &
                (tffsPartTbl[i].pFlashChip->uEraseSize - 1)) != 0) ||
                ((tffsPartTbl[i].flashSize &
                (tffsPartTbl[i].pFlashChip->uEraseSize - 1)) != 0))
                {
                TFFS_DBG_MSG ("Normal region %d is unaligned with block "
                              "size(0x%x) for %s%d\n",
                              tffsPartTbl[i].pFlashChip->uEraseSize,
                              tffsPartTbl[i].pFlashName,
                              tffsPartTbl[i].flashUnit, 0, 0, 0);
                return;
                }

            /* Check boot region */

            if (tffsPartTbl[i].bootSize != 0)
                {
                if (((tffsPartTbl[i].bootSize &
                    (tffsPartTbl[i].pFlashChip->uEraseSize - 1)) != 0))
                    {
                    TFFS_DBG_MSG ("Normal region %d is unaligned with block "
                                  "size(0x%x) for %s%d\n",
                                  tffsPartTbl[i].pFlashChip->uEraseSize,
                                  tffsPartTbl[i].pFlashName,
                                  tffsPartTbl[i].flashUnit, 0, 0, 0);
                    return;
                    }
                }

            tffsPartTbl[i].blkNum =
                tffsPartTbl[i].flashSize / tffsPartTbl[i].pFlashChip->uEraseSize;

            /* Check Flash functions */

            if (((tffsPartTbl[i].pFlashChip->uCapability &
                FLASH_CHIP_CAP_RD) == 0) ||
                ((tffsPartTbl[i].pFlashChip->uCapability &
                FLASH_CHIP_CAP_WR) == 0) ||
                ((tffsPartTbl[i].pFlashChip->uCapability &
                FLASH_CHIP_CAP_BLKERA) == 0))
                {
                TFFS_DBG_MSG ("Invalid Flash device\n", 0, 0, 0, 0, 0, 0);
                return;
                }

            if ((tffsPartTbl[i].pFlashChip->uCapability &
                FLASH_CHIP_CAP_BLKLOCK) != 0)
                {

                /* Allocate block lock status array */

                tffsPartTbl[i].pLockArray =
                    (UINT8 *) malloc (sizeof (UINT8) * tffsPartTbl[i].blkNum);
                if (tffsPartTbl[i].pLockArray == NULL)
                    {
                    TFFS_DBG_MSG ("Failed to allocate block lock status array "
                                  "for Flash region %s%d\n",
                                  tffsPartTbl[i].pFlashName,
                                  tffsPartTbl[i].flashUnit, 0, 0, 0, 0);
                    return;
                    }

                /* Query lock status */

                if (((tffsPartTbl[i].pFlashChip->uCapability &
                    FLASH_CHIP_CAP_BLKLOCKQRY) != 0) &&
                    (FLASH_TFFS_ENA (&tffsPartTbl[i]) == OK))
                    {
                    blkAdrs = 0;
                    for (j = 0; j < tffsPartTbl[i].blkNum; j++)
                        {
                        if (FLASH_TFFS_LOCKQRY (&tffsPartTbl[i], blkAdrs,
                                                &lock) != OK)
                            lock = TRUE;

                        tffsPartTbl[i].pLockArray[j] = lock ? 1 : 0;
                        blkAdrs += tffsPartTbl[i].pFlashChip->uEraseSize;
                        }

                    /* The operation has been finished anyway */

                    FLASH_TFFS_DIS (&tffsPartTbl[i]);
                    }

#ifdef TFFS_BOOT_LOCK
                /* Updated lock status for boot region */

                if (tffsPartTbl[i].bootSize != 0)
                    {
                    tffsPartTbl[i].bootLocked = TRUE;
                    for (j = 0;
                         j < (tffsPartTbl[i].bootSize /
                         tffsPartTbl[i].pFlashChip->uEraseSize);
                         j++)
                        {
                        if (tffsPartTbl[i].pLockArray[j] == 0)
                            {
                            tffsPartTbl[i].bootLocked = FALSE;
                            break;
                            }
                        }
                    }
#endif /* TFFS_BOOT_LOCK */
                }

            /*
             * The size of TrueFFS partition must be aligned with 4K bytes, that
             * is required by TrueFFS.
             */

            if ((tffsPartTbl[i].flashSize & (FLASH_TFFS_PART_ALIGN - 1)) != 0)
                {
                free (tffsPartTbl[i].pLockArray);
                TFFS_DBG_MSG ("TrueFFS partition size(0x%x) is not aligned with"
                              " 0x%x\n",
                              tffsPartTbl[i].flashSize, FLASH_TFFS_PART_ALIGN,
                              0, 0, 0, 0);
                return;
                }

            /* Set up FL socket information */

            pFlSocket = flSocketOf (noOfDrives);
            tffsSocket[noOfDrives] = tffsPartTbl[i].pPartName;
            pFlSocket->serialNo = i;

            /* Just make TrueFFS isRAM() happy... */

            pFlSocket->window.baseAddress =
                (tffsPartTbl[i].flashBase + tffsPartTbl[i].flashOffset) >>
                FLASH_TFFS_PART_SHIFT;

            pFlSocket->cardDetected = rfaCardDetected;
            pFlSocket->VccOn = rfaVccOn;
            pFlSocket->VccOff = rfaVccOff;
#ifdef SOCKET_12_VOLTS
            pFlSocket->VppOn = rfaVppOn;
            pFlSocket->VppOff = rfaVppOff;
#endif /* SOCKET_12_VOLTS */
            pFlSocket->initSocket = rfaInitSocket;
            pFlSocket->setWindow = rfaSetWindow;
            pFlSocket->setMappingContext = rfaSetMappingContext;
            pFlSocket->getAndClearCardChangeIndicator =
                rfaGetAndClearCardChangeIndicator;
            pFlSocket->writeProtected = rfaWriteProtected;
            noOfDrives++;
            }
        }
    }

/*******************************************************************************
*
* rfaCardDetected - detect if a card is present (inserted)
*
* This routine detects if a card is present (inserted).
* Always return TRUE in RFA environments since device is not removable.
*
* RETURNS: TRUE, or FALSE if the card is not present.
*/

LOCAL FLBoolean rfaCardDetected
    (
    FLSocket vol        /* pointer identifying drive */
    )
    {
    return (TRUE);
    }

/*******************************************************************************
*
* rfaVccOn - turn on Vcc (3.3/5 Volts)
*
* This routine turns on Vcc (3.3/5 Volts).  Vcc must be known to be good
* on exit. Assumed to be ON constantly in RFA environment.
*
* RETURNS: N/A
*/

LOCAL void rfaVccOn
    (
    FLSocket vol        /* pointer identifying drive */
    )
    {
    }

/*******************************************************************************
*
* rfaVccOff - turn off Vcc (3.3/5 Volts)
*
* This routine turns off Vcc (3.3/5 Volts) (PCMCIA). Assumed to be ON
* constantly in RFA environment.
*
* RETURNS: N/A
*/

LOCAL void rfaVccOff
    (
    FLSocket vol        /* pointer identifying drive */
    )
    {
    }

#ifdef SOCKET_12_VOLTS

/*******************************************************************************
*
* rfaVppOn - turns on Vpp (12 Volts)
*
* This routine turns on Vpp (12 Volts). Vpp must be known to be good on exit.
* Assumed to be ON constantly in RFA environment. This is not optional and
* must always be implemented.
*
* RETURNS: flOK always.
*/

LOCAL FLStatus rfaVppOn
    (
    FLSocket vol        /* pointer identifying drive */
    )
    {
    return (flOK);
    }

/*******************************************************************************
*
* rfaVppOff - turns off Vpp (12 Volts)
*
* This routine turns off Vpp (12 Volts). Assumed to be ON constantly
* in RFA environment.This is not optional and must always be implemented.
*
* RETURNS: N/A
*/

LOCAL void rfaVppOff
    (
    FLSocket vol        /* pointer identifying drive */
    )
    {
    }

#endif  /* SOCKET_12_VOLTS */

/*******************************************************************************
*
* rfaInitSocket - perform all necessary initializations of the socket
*
* This routine performs all necessary initializations of the socket.
*
* RETURNS: flOK always.
*/
LOCAL FLStatus rfaInitSocket
    (
    FLSocket vol        /* pointer identifying drive */
    )
    {
    return (flOK);
    }

/*******************************************************************************
*
* rfaSetWindow - set current window attributes, base address, size, etc
*
* This routine sets current window hardware attributes: Base address, size,
* speed and bus width.  The requested settings are given in the 'vol.window'
* structure.  If it is not possible to set the window size requested in
* 'vol.window.size', the window size should be set to a larger value,
* if possible. In any case, 'vol.window.size' should contain the
* actual window size (in 4 KB units) on exit.
*
* RETURNS: N/A
*/
LOCAL void rfaSetWindow
    (
    FLSocket vol        /* pointer identifying drive */
    )
    {

    /* Physical base as a 4K page */

    flSetWindowSize (&vol,
                    tffsPartTbl[vol.serialNo].flashSize >>
                    FLASH_TFFS_PART_SHIFT);
    }

/*******************************************************************************
*
* rfaSetMappingContext - sets the window mapping register to a card address
*
* This routine sets the window mapping register to a card address.
* The window should be set to the value of 'vol.window.currentPage',
* which is the card address divided by 4 KB. An address over 128MB,
* (page over 32K) specifies an attribute-space address. On entry to this
* routine vol.window.currentPage is the page already mapped into the window.
* (In otherwords the page that was mapped by the last call to this routine.)
* The page to map is guaranteed to be on a full window-size boundary.
* This is meaningful only in environments that use sliding window mechanism
* to view flash memory, like in PCMCIA. Not common in RFA environments.
*
* RETURNS: N/A
*/

LOCAL void rfaSetMappingContext
    (
    FLSocket vol,       /* pointer identifying drive */
    unsigned page       /* page to be mapped */
    )
    {
    }

/*******************************************************************************
*
* rfaGetAndClearCardChangeIndicator - return the hardware card-change indicator
*
* This routine returns TRUE if the card has been changed and FALSE if not. It
* also clears the "card-changed" indicator if it has been set.
* Always return FALSE in RFA environments since device is not removable.
*
* RETURNS: FALSE, or TRUE if the card has been changed
*/

LOCAL FLBoolean rfaGetAndClearCardChangeIndicator
    (
    FLSocket vol        /* pointer identifying drive */
    )
    {
    return (FALSE);
    }

/*******************************************************************************
*
* rfaWriteProtected - return the write-protect state of the media
*
* This routine returns the write-protect state of the media
*
* RETURNS: FALSE, or TRUE if the card is write-protected
*/

LOCAL FLBoolean rfaWriteProtected
    (
    FLSocket vol        /* pointer identifying drive */
    )
    {
    return (FALSE);
    }

/*******************************************************************************
*
* flFitInSocketWindow - check whether the flash array fits in the socket window
*
* This routine checks whether the flash array fits in the socket window.
*
* RETURNS: A chip size guaranteed to fit in the socket window.
*/

long int flFitInSocketWindow
    (
    long int chipSize,      /* size of single physical chip in bytes */
    int      interleaving,  /* flash chip interleaving (1,2,4 etc) */
    long int windowSize     /* socket window size in bytes */
    )
    {
    if (chipSize * interleaving > windowSize) /* doesn't fit in socket window */
        {
        int  roundedSizeBits;

        /* fit chip in the socket window */

        chipSize = windowSize / interleaving;

        /* round chip size at powers of 2 */

        for (roundedSizeBits = 0; (0x1L << roundedSizeBits) <= chipSize;
             roundedSizeBits++)
        ;

        chipSize = (0x1L << (roundedSizeBits - 1));
        }

    return (chipSize);
    }

/*******************************************************************************
*
* flDelayMsecs - wait for specified number of milliseconds
*
* This routine waits for the specified number of milliseconds.
*
* RETURNS: N/A
*
* NOMANUAL
*/

void flDelayMsecs
    (
    unsigned milliseconds       /* milliseconds to wait */
    )
    {
    UINT32 ix;
    UINT32 iy = 1;
    UINT32 iz = 2;

    /* it doesn't count time consumed in interrupt level */

    for (ix = 0; ix < milliseconds; ix++)
        for (ix = 0; ix < sysTffsMsecLoopCount; ix++)
        {
        tickGet ();             /* dummy */
        iy = KILL_TIME_FUNC;    /* consume time */
        }
    }

/*******************************************************************************
*
* flDelayLoop - consume the specified time
*
* This routine delays for the specified time.
*
* RETURNS: N/A
*/

void flDelayLoop
    (
    int  cycles
    )
    {
    while (--cycles)
    ;
    }

#ifdef TFFS_FORMAT_PRINT
/*******************************************************************************
*
* sysTffsProgressCb - print progess information
*
* This routine prints the progess information.
*
* RETURNS: flOK always.
*
* ERRNO: N/A
*/

int sysTffsProgressCb
    (
    int totalUnitsToFormat,
    int totalUnitsFormattedSoFar
    )
    {
    printf("Formatted %d of %d units = %d.%01d %%\r",
            totalUnitsFormattedSoFar, totalUnitsToFormat,
            100 * totalUnitsFormattedSoFar / totalUnitsToFormat,
            1000 * totalUnitsFormattedSoFar / totalUnitsToFormat % 10);

    if (totalUnitsFormattedSoFar == totalUnitsToFormat)
        printf ("\n");
    return (flOK);
    }

#   define  PROGRESS_CB sysTffsProgressCb
#else /* !TFFS_FORMAT_PRINT */
#   define  PROGRESS_CB NULL
#endif /* TFFS_FORMAT_PRINT */

/*******************************************************************************
*
* sysTffsFormat - format the flash memory above an offset
*
* This routine formats the flash memory.  Because this function defines
* the symbolic constant, HALF_FORMAT, the lower half of the specified flash
* memory is left unformatted.  If the lower half of the flash memory was
* previously formated by TrueFFS, and you are trying to format the upper half,
* you need to erase the lower half of the flash memory before you format the
* upper half.  To do this, you could use:
* .CS
* tffsRawio(0, 3, 0, 8)
* .CE
* The first argument in the tffsRawio() command shown above is the TrueFFS
* drive number, 0.  The second argument, 3, is the function number (also
* known as TFFS_PHYSICAL_ERASE).  The third argument, 0, specifies the unit
* number of the first erase unit you want to erase.  The fourth argument, 8,
* specifies how many erase units you want to erase.
*
* RETURNS: OK, or ERROR if it fails.
*/

STATUS sysTffsFormat
    (
    int tffsDriveNo
    )
    {
    STATUS          status;
    FLSocket *      pFlSocket = flSocketOf (tffsDriveNo);

    /* Note: cfiamd keeps track of room to save at end of flash as required */

    tffsDevFormatParams params = {
        {0, 99, 1, 0x10000l, PROGRESS_CB, {0,0,0,0}, NULL, 2, 0, NULL},
        FTL_FORMAT
        };

    /* format device */

    params.formatParams.bootImageLen =
        tffsPartTbl[pFlSocket->serialNo].bootSize;
    status = tffsDevFormat (tffsDriveNo, (int) &params);

    return (status);
    }

#ifdef TFFS_BOOT_LOCK
/*******************************************************************************
*
* sysTffsBootLock - lock boot region
*
* This routine locks the boot region.
*
* RETURNS: flOK or flDriveNotAvailable for device busy, flIncomplete for failure
* to lock the boot region, flUnknownCmd if unsupported by HW.
*/

FLStatus sysTffsBootLock
    (
    int tffsDriveNo
    )
    {
    UINT32              i;
    UINT32              blkOffset = 0;
    FLSocket *          pFlSocket = flSocketOf (tffsDriveNo);
    TFFS_PART_INFO *    pPartInfo = &tffsPartTbl[pFlSocket->serialNo];
    FLStatus            retVal = flOK;

    if (pPartInfo->pLockArray == NULL)
        {
        TFFS_DBG_MSG ("Lock function is unsupported by HW\n",
                      0, 0, 0, 0, 0, 0);
        return (flUnknownCmd);
        }

    if (pPartInfo->bootLocked)
        return (flOK);

    /* Enable Flash */

    if (FLASH_TFFS_ENA (pPartInfo) != OK)
        {
        TFFS_DBG_MSG ("Failed to enable Flash\n", 0, 0, 0, 0, 0, 0);
        return (flDriveNotAvailable);
        }

    pPartInfo->bootLocked = TRUE;
    for (i = 0; i < pPartInfo->bootSize / pPartInfo->pFlashChip->uEraseSize;
        i++, blkOffset += pPartInfo->pFlashChip->uEraseSize)
        {
        if (pPartInfo->pLockArray[i] != 0)
            continue;

        if (FLASH_TFFS_LOCK (pPartInfo, blkOffset) != OK)
            {
            TFFS_DBG_MSG ("Failed to unlock boot region @ 0x%x\n",
                          blkOffset, 0, 0, 0, 0, 0);
            retVal = flIncomplete;
            pPartInfo->bootLocked = FALSE;
            break;
            }

        pPartInfo->pLockArray[i] = TRUE;
        }

    /* The lock operation has been finished anyway */

    FLASH_TFFS_DIS (pPartInfo);

    return (retVal);
    }

/*******************************************************************************
*
* sysTffsBootUnLock - unlock boot region
*
* This routine unlocks the boot region.
*
* RETURNS: flOK or flDriveNotAvailable for device busy, flIncomplete for failure
* to lock the boot region.
*/

FLStatus sysTffsBootUnLock
    (
    int tffsDriveNo
    )
    {
    UINT32              i;
    UINT32              blkOffset = 0;
    FLSocket *          pFlSocket = flSocketOf (tffsDriveNo);
    TFFS_PART_INFO *    pPartInfo = &tffsPartTbl[pFlSocket->serialNo];
    FLStatus            retVal = flOK;

    if (pPartInfo->pLockArray == NULL)
        {
        TFFS_DBG_MSG ("Unlock function is unsupported by HW\n",
                      0, 0, 0, 0, 0, 0);
        return (flUnknownCmd);
        }

    if (!pPartInfo->bootLocked)
        return (flOK);

    /* Enable Flash */

    if (FLASH_TFFS_ENA (pPartInfo) != OK)
        {
        TFFS_DBG_MSG ("Failed to enable Flash\n", 0, 0, 0, 0, 0, 0);
        return (flDriveNotAvailable);
        }

    pPartInfo->bootLocked = FALSE;
    for (i = 0; i < pPartInfo->bootSize / pPartInfo->pFlashChip->uEraseSize;
        i++, blkOffset += pPartInfo->pFlashChip->uEraseSize)
        {
        if (pPartInfo->pLockArray[i] == 0)
            continue;

        if (FLASH_TFFS_UNLOCK (pPartInfo, blkOffset) != OK)
            {
            TFFS_DBG_MSG ("Failed to unlock boot region @ 0x%x\n",
                          blkOffset, 0, 0, 0, 0, 0);
            retVal = flIncomplete;
            pPartInfo->bootLocked = TRUE;
            break;
            }

        pPartInfo->pLockArray[i] = FALSE;
        }

    /* The unlock operation has been finished anyway */

    FLASH_TFFS_DIS (pPartInfo);

    return (retVal);
    }
#endif /* TFFS_BOOT_LOCK */

/*******************************************************************************
*
* sysTffsUnlock - unlock blocks if necessary
*
* This routine unlocks the blocks if necessary.
*
* RETURNS: flOK or flBadParameter for invalid parameters, flDriveNotAvailable
* for device busy, flWriteFault for failure to unlock the blocks.
*
* ERRNO: N/A
*/

LOCAL FLStatus sysTffsUnlock
    (
    TFFS_PART_INFO *    pPartInfo,
    UINT32              offset,
    UINT32              len
    )
    {
    UINT32  blkOffset;
    UINT32  blkIndex;

    if (pPartInfo->pLockArray == NULL)
        return (flOK);

    for (blkOffset =
            offset - (offset & (pPartInfo->pFlashChip->uEraseSize - 1));
        blkOffset < (offset + len);
        blkOffset += pPartInfo->pFlashChip->uEraseSize)
        {
        blkIndex = blkOffset / pPartInfo->pFlashChip->uEraseSize;
        if (pPartInfo->pLockArray[blkIndex] != 0)
            {
            if (FLASH_TFFS_UNLOCK (pPartInfo, blkOffset) != OK)
                {
                TFFS_DBG_MSG ("Failed to unlock block @ 0x%x\n",
                              blkOffset, 0, 0, 0, 0, 0);
                return (flWriteFault);
                }
            else
                pPartInfo->pLockArray[blkIndex] = 0;
            }
        }

    return (flOK);
    }

/*******************************************************************************
*
* sysTffsWrite - write data to flash
*
* This routine writes the data to the Flash.
*
* RETURNS: flOK or flBadParameter for invalid parameters, flDriveNotAvailable
* for device busy, flWriteFault for failure to write data.
*
* ERRNO: N/A
*/

LOCAL FLStatus sysTffsWrite
    (
    FLFlash             vol,
    CardAddress         address,
    const void FAR1 *   buffer,
    int                 length,
    int                 overwrite
    )
    {
    FLSocket *          pFlSocket = vol.socket;
    TFFS_PART_INFO *    pPartInfo = &tffsPartTbl[pFlSocket->serialNo];
    UINT8 *             ppbuf[1];
    UINT8 *             pBuf = (UINT8 *) buffer;
    UINT32              len;
    FLStatus            retVal = flOK;

    if (pBuf== NULL)
        {
        TFFS_DBG_MSG ("NULL buffer pointer\n", 0, 0, 0, 0, 0, 0);
        return flBadParameter;
        }

    /* 0 may be passed from TrueFFS really... */

    if (length <= 0)
        {
        TFFS_DBG_MSG ("Invalid length\n", length, 0, 0, 0, 0, 0);
        return (flOK);
        }

    if ((address >= pPartInfo->flashSize) ||
        ((address + length) > pPartInfo->flashSize))
        {
        TFFS_DBG_MSG ("[0x%x, 0x%x] is out of range(0x%x) for write\n",
                      address, address + length - 1, pPartInfo->flashSize,
                      0, 0, 0);
        return flBadParameter;
        }

    if ((address < pPartInfo->bootSize) &&
        ((address + length) > pPartInfo->bootSize))
        {
        TFFS_DBG_MSG ("[0x%x, 0x%x] is out of boot range(0x%x) for write\n",
                      address, address + length - 1, pPartInfo->bootSize,
                      0, 0, 0);
        return flBadParameter;
        }

    /* Enable Flash */

    if (FLASH_TFFS_ENA (pPartInfo) != OK)
        {
        TFFS_DBG_MSG ("Failed to enable Flash\n", 0, 0, 0, 0, 0, 0);
        return (flDriveNotAvailable);
        }

#ifdef TFFS_BOOT_LOCK
    /* Check lock status for boot region */

    if ((address < pPartInfo->bootSize) && (pPartInfo->bootLocked))
        {
        FLASH_TFFS_DIS (pPartInfo);
        TFFS_DBG_MSG ("Boot region is locked\n", 0, 0, 0, 0, 0, 0);
        return (flWriteFault);
        }
#endif /* TFFS_BOOT_LOCK */

    /* Unlock boot region if boot region needn't be protected */

    retVal = sysTffsUnlock (pPartInfo, address, length);
    if (retVal != flOK)
        {
        FLASH_TFFS_DIS (pPartInfo);
        return (retVal);
        }

    ppbuf[0] = pBuf;
    if (FLASH_TFFS_WRITE (pPartInfo, address, length, ppbuf) != OK)
        {
        FLASH_TFFS_DIS (pPartInfo);
        TFFS_DBG_MSG ("Failed to write @ 0x%x\n",
                      address, 0, 0, 0, 0, 0);
        return (flWriteFault);
        }

    FLASH_TFFS_DIS (pPartInfo);
    return (retVal);
    }

/*******************************************************************************
*
* sysTffsErase - erase blocks
*
* This routine erases the specified blocks for the Flash.
*
* RETURNS: flOK or flBadParameter for invalid parameters, flDriveNotAvailable
* for device busy, flWriteFault for failure to erase the blocks.
*
* ERRNO: N/A
*/

LOCAL FLStatus sysTffsErase
    (
    FLFlash vol,
    int     firstErasableBlock,
    int     numOfErasableBlocks
    )
    {
    FLSocket *          pFlSocket = vol.socket;
    TFFS_PART_INFO *    pPartInfo = &tffsPartTbl[pFlSocket->serialNo];
    UINT32              blkOffset;
    UINT32              len;
    FLStatus            retVal = flOK;

    /* 0 may be passed from TrueFFS really... */

    if (numOfErasableBlocks == 0)
        {
        TFFS_DBG_MSG ("Invalid block count\n",
                      numOfErasableBlocks, 0, 0, 0, 0, 0);
        return (flOK);
        }

    blkOffset = firstErasableBlock * pPartInfo->pFlashChip->uEraseSize;
    len = numOfErasableBlocks * pPartInfo->pFlashChip->uEraseSize;
    if ((blkOffset >= pPartInfo->flashSize) ||
        ((blkOffset + len) > pPartInfo->flashSize))
        {
        TFFS_DBG_MSG ("[0x%x, 0x%x] is out of range(0x%x) for erase\n",
                      blkOffset, blkOffset + len - 1, pPartInfo->flashSize,
                      0, 0, 0);
        return flBadParameter;
        }

    if ((blkOffset < pPartInfo->bootSize) &&
        ((blkOffset + len) > pPartInfo->bootSize))
        {
        TFFS_DBG_MSG ("[0x%x, 0x%x] is out of boot range(0x%x) for erase\n",
                      blkOffset, blkOffset + len - 1, pPartInfo->bootSize,
                      0, 0, 0);
        return flBadParameter;
        }

    /* Enable Flash */

    if (FLASH_TFFS_ENA (pPartInfo) != OK)
        {
        TFFS_DBG_MSG ("Failed to enable Flash\n", 0, 0, 0, 0, 0, 0);
        return (flDriveNotAvailable);
        }

#ifdef TFFS_BOOT_LOCK
    /* Check lock status for boot region */

    if ((blkOffset < pPartInfo->bootSize) && (pPartInfo->bootLocked))
        {
        FLASH_TFFS_DIS (pPartInfo);
        TFFS_DBG_MSG ("Boot region is locked\n", 0, 0, 0, 0, 0, 0);
        return (flWriteFault);
        }
#endif /* TFFS_BOOT_LOCK */

    /* Unlock boot region if boot region needn't be protected */

    retVal = sysTffsUnlock (pPartInfo, blkOffset, len);
    if (retVal != flOK)
        {
        FLASH_TFFS_DIS (pPartInfo);
        return (retVal);
        }

    if (FLASH_TFFS_ERASE (pPartInfo, blkOffset, numOfErasableBlocks) != OK)
        {
        FLASH_TFFS_DIS (pPartInfo);
        TFFS_DBG_MSG ("Failed to erase @ 0x%x\n",
                      blkOffset, 0, 0, 0, 0, 0);
        return (flWriteFault);
        }

    FLASH_TFFS_DIS (pPartInfo);
    return (flOK);
    }

/*******************************************************************************
*
* sysTffsRead - flash read routine
*
* This routine reads the data from the Flash.
*
* Parameters:
* vol:       Pointer identifying drive
* address  : Card address to read
* buffer   : Area to read into
* length   : Length to read
*
* RETURNS: flOK or flBadParameter for invalid parameters, flDriveNotAvailable
* for device busy, flWriteFault for failure to write data.
*
* ERRNO: N/A
*/

LOCAL FLStatus sysTffsRead
    (
    FLFlash             vol,
    CardAddress         address,
    const void FAR1 *   buffer,
    int                 length,
    int                 mode
    )
    {
    FLSocket *          pFlSocket = vol.socket;
    TFFS_PART_INFO *    pPartInfo = &tffsPartTbl[vol.socket->serialNo];
    UINT8 *             ppbuf[1];
    UINT8 *             pBuf = (UINT8 *) buffer;
    UINT32              len;
    FLStatus            retVal = flOK;

    if (pBuf== NULL)
        {
        TFFS_DBG_MSG ("NULL buffer pointer\n", 0, 0, 0, 0, 0, 0);
        return flBadParameter;
        }

    /* 0 may be passed from TrueFFS really... */

    if (length <= 0)
        {
        TFFS_DBG_MSG ("Invalid length\n", length, 0, 0, 0, 0, 0);
        return (flOK);
        }

    if ((address >= pPartInfo->flashSize) ||
        ((address + length) > pPartInfo->flashSize))
        {
        TFFS_DBG_MSG ("[0x%x, 0x%x] is out of range(0x%x) for read\n",
                      address, address + length - 1, pPartInfo->flashSize,
                      0, 0, 0);
        return flBadParameter;
        }

    if ((address < pPartInfo->bootSize) && 
        ((address + length) > pPartInfo->bootSize))
        {
        TFFS_DBG_MSG ("[0x%x, 0x%x] is out of boot range(0x%x) for read\n",
                      address, address + length - 1, pPartInfo->bootSize,
                      0, 0, 0);
        return flBadParameter;
        }

    /* Enable Flash */

    if (FLASH_TFFS_ENA (pPartInfo) != OK)
        {
        TFFS_DBG_MSG ("Failed to enable Flash\n", 0, 0, 0, 0, 0, 0);
        return (flDriveNotAvailable);
        }

    ppbuf[0] = pBuf;
    if (FLASH_TFFS_READ (pPartInfo, address, length, ppbuf) != OK)
        {
        FLASH_TFFS_DIS (pPartInfo);
        TFFS_DBG_MSG ("Failed to read @ 0x%x\n", 
                      address, 0, 0, 0, 0, 0);
        return (flWriteFault);
        }  

    FLASH_TFFS_DIS (pPartInfo);
    return (retVal);
    }

/*******************************************************************************
*
* sysTffsMap - Map through buffer
*
* This routine will be registered as the map routine for this MTD.
*
* Parameters:
* vol     : Pointer identifying drive.
* address : Flash address to be mapped.
* length  : number of bytes to map.
*
* RETURNS: Pointer to the buffer data was mapped to.
*
* ERRNO: N/A
*/

LOCAL void FAR0 * sysTffsMap
    (
    FLFlash vol,
    CardAddress address,
    int length
    )
    {
    address = flMap(vol.socket, address);

    sysTffsRead(&vol, address, thisBuffer, length, 0);

    vol.socket->remapped = TRUE;

    return (void FAR0 *)thisBuffer;
    }

/*******************************************************************************
*
* sysTffsIdentify - identify Flash partition
*
* This routine probes the Flash partition.
*
* RETURNS: flOK or flUnknownMedia if unknown Flash probed.
*
* ERRNO: N/A
*/

LOCAL FLStatus  sysTffsIdentify
    (
    FLFlash     vol
    )
    {
    FLSocket *          pFlSocket = vol.socket;
    FLStatus            retVal = flOK;
    TFFS_PART_INFO *    pPartInfo = &tffsPartTbl[pFlSocket->serialNo];
    
    vol.chipSize = pPartInfo->flashSize;
    vol.noOfChips = 1;
    vol.interleaving = 1;
    vol.erasableBlockSize =
        pPartInfo->pFlashChip->uEraseSize;
    vol.write = sysTffsWrite;
    vol.erase = sysTffsErase;

    /* Lock boot region */

#ifdef TFFS_BOOT_LOCK
    retVal = sysTffsBootLock (pFlSocket->volNo);
#endif /* TFFS_BOOT_LOCK */

    if (pPartInfo->pFlashChip->uFlashType == FLASH_CHIP_TYPE_NOR)
        {
        /* Enable Flash */

        if (FLASH_TFFS_ENA (pPartInfo) != OK)
            {
            TFFS_DBG_MSG ("Failed to enable Flash\n", 0, 0, 0, 0, 0, 0);
            return (flDriveNotAvailable);
            }

        /* Reset to read array mode */

        FLASH_TFFS_RST (pPartInfo);

        FLASH_TFFS_DIS (pPartInfo);
        }
    else if (pPartInfo->pFlashChip->uFlashType == FLASH_CHIP_TYPE_SPI)
        {
        vol.mtdVars = &mtdVars[flSocketNoOf(vol.socket)];

        vol.map  = sysTffsMap;
        vol.read = sysTffsRead;
        }
    else
        retVal = flBadParameter;

    return (retVal);
    }

#ifdef INCLUDE_SHOW_ROUTINES
/*******************************************************************************
*
* sysTffsShow - show TrueFFS driver information
*
* This routine prints the TrueFFS driver information.
*
* RETURNS: OK always
*/

void sysTffsShow (void)
    {
    UINT32              i;

    for (i = 0; i < FLASH_TFFS_PART_NUM; i++)
        {
        if (tffsPartTbl[i].pPartName == NULL)
            continue;

        printf ("TrueFFS Partition %d - %s\n",
                i, tffsPartTbl[i].pPartName);
        printf ("\tMapped Flash Device: %s %d\n",
                tffsPartTbl[i].pFlashName, tffsPartTbl[i].flashUnit);
        printf ("\tMapped Flash region: 0x%08x - 0x%08x\n",
                tffsPartTbl[i].flashBase + tffsPartTbl[i].flashOffset,
                tffsPartTbl[i].flashBase + tffsPartTbl[i].flashOffset +
                tffsPartTbl[i].flashSize - 1);

        if (tffsPartTbl[i].bootSize != 0)
            {
            printf ("\t");
#ifdef TFFS_BOOT_LOCK
            if (tffsPartTbl[i].bootLocked)
                printf ("Locked ");
#endif /* TFFS_BOOT_LOCK */
            printf ("Reserved Boot region: 0x%08x - 0x%08x\n",
                    tffsPartTbl[i].flashBase + tffsPartTbl[i].flashOffset,
                    tffsPartTbl[i].flashBase + tffsPartTbl[i].flashOffset +
                    tffsPartTbl[i].bootSize - 1);
            }
        }

    return;
    }
#endif /* INCLUDE_SHOW_ROUTINES */
#else /* TFFS to normal Flash interface */

#error "sysTffs : Verify system macros and function before first use"

/* defines */

#ifndef PRJ_BUILD
#define INCLUDE_MTD_I28F016             /* Intel: 28f016 */
#define INCLUDE_MTD_I28F008             /* Intel: 28f008 */
#define INCLUDE_MTD_AMD                 /* AMD, Fujitsu: 29f0{40,80,16} 8bit */
#undef  INCLUDE_MTD_CFIAMD              /* CFI driver for AMD Flash Part */
#undef  INCLUDE_MTD_CFISCS              /* CFI/SCS */
#undef  INCLUDE_MTD_WAMD                /* AMD, Fujitsu: 29f0{40,80,16} 16bit */
#define INCLUDE_TL_FTL                  /* FTL translation layer */
#undef  INCLUDE_TL_SSFDC                /* SSFDC translation layer */
#define INCLUDE_SOCKET_SIMM             /* SIMM socket interface */
#define INCLUDE_SOCKET_PCMCIA           /* PCMCIA socket interface */
#endif  /* PRJ_BUILD */

/* TODO :
 * If you don't use TrueFFS to write your boot image you might want
 * to undefine this.
 */

#define INCLUDE_TFFS_BOOT_IMAGE		/* include tffsBootImagePut() */

/* TODO :
 * set these to board specific values.
 * The values used here are fictional.
 */

#define	FLASH_BASE_ADRS		0x02800000	/* Flash memory base address */
#define	FLASH_SIZE		0x00200000	/* Flash memory size */
#define VCC_DELAY_MSEC		100	/* Millisecs to wait for Vcc ramp up */
#define VPP_DELAY_MSEC		100	/* Millisecs to wait for Vpp ramp up */
#define KILL_TIME_FUNC	 ((iz * iz) / (iz + iz)) / ((iy + iz) / (iy * iz))


/* locals */

LOCAL UINT32 sysTffsMsecLoopCount = 0;


/* forward declarations */

LOCAL FLBoolean		rfaCardDetected (FLSocket vol);
LOCAL void		rfaVccOn (FLSocket vol);
LOCAL void		rfaVccOff (FLSocket vol);
#ifdef	SOCKET_12_VOLTS
LOCAL FLStatus		rfaVppOn (FLSocket vol);
LOCAL void		rfaVppOff (FLSocket vol);
#endif	/* SOCKET_12_VOLTS */
LOCAL FLStatus		rfaInitSocket (FLSocket vol);
LOCAL void		rfaSetWindow (FLSocket vol);
LOCAL void		rfaSetMappingContext (FLSocket vol, unsigned page);
LOCAL FLBoolean		rfaGetAndClearCardChangeIndicator (FLSocket vol);
LOCAL FLBoolean		rfaWriteProtected (FLSocket vol);
LOCAL void		rfaRegister (void);


#ifndef DOC
#include "tffs/tffsConfig.c"
#endif /* DOC */

/*******************************************************************************
*
* sysTffsInit - board level initialization for TFFS
*
* This routine calls the socket registration routines for the socket component
* drivers that will be used with this BSP. The order of registration signifies
* the logical drive number assigned to the drive associated with the socket.
*
* RETURNS: N/A
*/

LOCAL void sysTffsInit (void)
    {
    UINT32 ix = 0;
    UINT32 iy = 1;
    UINT32 iz = 2;
    int oldTick;

    /* we assume followings:
     *   - no interrupts except timer is happening.
     *   - the loop count that consumes 1 msec is in 32 bit.
     * it is done in the early stage of usrRoot() in tffsDrv().  */

    oldTick = tickGet();
    while (oldTick == tickGet())	/* wait for next clock interrupt */
	;

    oldTick = tickGet();
    while (oldTick == tickGet())	/* loop one clock tick */
	{
	iy = KILL_TIME_FUNC;		/* consume time */
	ix++;				/* increment the counter */
	}

    sysTffsMsecLoopCount = ix * sysClkRateGet() / 1000;

    /* TODO:
     * Call each sockets register routine here
     */

    rfaRegister ();			/* RFA socket interface register */

    }

/*******************************************************************************
*
* rfaRegister - install routines for the Flash RFA
*
* This routine installs necessary functions for the Resident Flash Array(RFA).
*
* RETURNS: N/A
*/

LOCAL void rfaRegister (void)
    {
    FLSocket vol = flSocketOf (noOfDrives);

    tffsSocket[noOfDrives] =	"RFA";
    vol.window.baseAddress =	FLASH_BASE_ADRS >> 12;
    vol.cardDetected =		rfaCardDetected;
    vol.VccOn =			rfaVccOn;
    vol.VccOff =		rfaVccOff;
#ifdef SOCKET_12_VOLTS
    vol.VppOn =			rfaVppOn;
    vol.VppOff =		rfaVppOff;
#endif
    vol.initSocket =		rfaInitSocket;
    vol.setWindow =		rfaSetWindow;
    vol.setMappingContext =	rfaSetMappingContext;
    vol.getAndClearCardChangeIndicator = rfaGetAndClearCardChangeIndicator;
    vol.writeProtected =	rfaWriteProtected;
    noOfDrives++;
    }

/*******************************************************************************
*
* rfaCardDetected - detect if a card is present (inserted)
*
* This routine detects if a card is present (inserted).
* Always return TRUE in RFA environments since device is not removable.
*
* RETURNS: TRUE, or FALSE if the card is not present.
*/

LOCAL FLBoolean rfaCardDetected
    (
    FLSocket vol		/* pointer identifying drive */
    )
    {
    return (TRUE);
    }

/*******************************************************************************
*
* rfaVccOn - turn on Vcc (3.3/5 Volts)
*
* This routine turns on Vcc (3.3/5 Volts).  Vcc must be known to be good
* on exit. Assumed to be ON constantly in RFA environment.
*
* RETURNS: N/A
*/

LOCAL void rfaVccOn
    (
    FLSocket vol		/* pointer identifying drive */
    )
    {
    }

/*******************************************************************************
*
* rfaVccOff - turn off Vcc (3.3/5 Volts)
*
* This routine turns off Vcc (3.3/5 Volts) (PCMCIA). Assumed to be ON
* constantly in RFA environment.
*
* RETURNS: N/A
*/

LOCAL void rfaVccOff
    (
    FLSocket vol		/* pointer identifying drive */
    )
    {
    }

#ifdef SOCKET_12_VOLTS

/*******************************************************************************
*
* rfaVppOn - turns on Vpp (12 Volts)
*
* This routine turns on Vpp (12 Volts). Vpp must be known to be good on exit.
* Assumed to be ON constantly in RFA environment. This is not optional and
* must always be implemented.
*
* RETURNS: flOK always.
*/

LOCAL FLStatus rfaVppOn
    (
    FLSocket vol		/* pointer identifying drive */
    )
    {
    return (flOK);
    }

/*******************************************************************************
*
* rfaVppOff - turns off Vpp (12 Volts)
*
* This routine turns off Vpp (12 Volts). Assumed to be ON constantly
* in RFA environment.This is not optional and must always be implemented.
*
* RETURNS: N/A
*/

LOCAL void rfaVppOff
    (
    FLSocket vol		/* pointer identifying drive */
    )
    {
    }

#endif	/* SOCKET_12_VOLTS */

/*******************************************************************************
*
* rfaInitSocket - perform all necessary initializations of the socket
*
* This routine performs all necessary initializations of the socket.
*
* RETURNS: flOK always.
*/

    /* TODO:
     * This function is always board specific.
     * Please set this to your specific needs.
     */

LOCAL FLStatus rfaInitSocket
    (
    FLSocket vol		/* pointer identifying drive */
    )
    {
    return (flOK);
    }

/*******************************************************************************
*
* rfaSetWindow - set current window attributes, base address, size, etc
*
* This routine sets current window hardware attributes: Base address, size,
* speed and bus width.  The requested settings are given in the 'vol.window'
* structure.  If it is not possible to set the window size requested in
* 'vol.window.size', the window size should be set to a larger value,
* if possible. In any case, 'vol.window.size' should contain the
* actual window size (in 4 KB units) on exit.
*
* RETURNS: N/A
*/

    /* TODO: set this to your specific needs */

LOCAL void rfaSetWindow
    (
    FLSocket vol		/* pointer identifying drive */
    )
    {
    /* Physical base as a 4K page */

    vol.window.baseAddress = FLASH_BASE_ADRS >> 12;

    flSetWindowSize (&vol, FLASH_SIZE >> 12);
    }

/*******************************************************************************
*
* rfaSetMappingContext - sets the window mapping register to a card address
*
* This routine sets the window mapping register to a card address.
* The window should be set to the value of 'vol.window.currentPage',
* which is the card address divided by 4 KB. An address over 128MB,
* (page over 32K) specifies an attribute-space address. On entry to this
* routine vol.window.currentPage is the page already mapped into the window.
* (In otherwords the page that was mapped by the last call to this routine.)
* The page to map is guaranteed to be on a full window-size boundary.
* This is meaningful only in environments that use sliding window mechanism
* to view flash memory, like in PCMCIA. Not common in RFA environments.
*
* RETURNS: N/A
*/

LOCAL void rfaSetMappingContext
    (
    FLSocket vol,		/* pointer identifying drive */
    unsigned page		/* page to be mapped */
    )
    {
    }

/*******************************************************************************
*
* rfaGetAndClearCardChangeIndicator - return the hardware card-change indicator
*
* This routine returns TRUE if the card has been changed and FALSE if not. It
* also clears the "card-changed" indicator if it has been set.
* Always return FALSE in RFA environments since device is not removable.
*
* RETURNS: FALSE, or TRUE if the card has been changed
*/

LOCAL FLBoolean rfaGetAndClearCardChangeIndicator
    (
    FLSocket vol		/* pointer identifying drive */
    )
    {
    return (FALSE);
    }

/*******************************************************************************
*
* rfaWriteProtected - return the write-protect state of the media
*
* This routine returns the write-protect state of the media
*
* RETURNS: FALSE, or TRUE if the card is write-protected
*/

LOCAL FLBoolean rfaWriteProtected
    (
    FLSocket vol		/* pointer identifying drive */
    )
    {
    return (FALSE);
    }

/*******************************************************************************
*
* flFitInSocketWindow - check whether the flash array fits in the socket window
*
* This routine checks whether the flash array fits in the socket window.
*
* RETURNS: A chip size guaranteed to fit in the socket window.
*/

long int flFitInSocketWindow
    (
    long int chipSize,		/* size of single physical chip in bytes */
    int      interleaving,	/* flash chip interleaving (1,2,4 etc) */
    long int windowSize		/* socket window size in bytes */
    )
    {
    if (chipSize*interleaving > windowSize) /* doesn't fit in socket window */
        {
        int  roundedSizeBits;

        /* fit chip in the socket window */
        chipSize = windowSize / interleaving;

        /* round chip size at powers of 2 */
        for (roundedSizeBits = 0; (0x1L << roundedSizeBits) <= chipSize;
             roundedSizeBits++)
	    ;

        chipSize = (0x1L << (roundedSizeBits - 1));
        }

    return (chipSize);
    }

/*******************************************************************************
*
* flDelayMsecs - wait for specified number of milliseconds
*
* This routine waits for the specified number of milliseconds.
*
* RETURNS: N/A
*
* NOMANUAL
*/

void flDelayMsecs
    (
    unsigned milliseconds       /* milliseconds to wait */
    )
    {
    UINT32 ix;
    UINT32 iy = 1;
    UINT32 iz = 2;

    /* it doesn't count time consumed in interrupt level */

    for (ix = 0; ix < milliseconds; ix++)
        for (ix = 0; ix < sysTffsMsecLoopCount; ix++)
	    {
	    tickGet ();			/* dummy */
	    iy = KILL_TIME_FUNC;	/* consume time */
	    }
    }

/*******************************************************************************
*
* flDelayLoop - consume the specified time
*
* This routine delays for the specified time.
*
* RETURNS: N/A
*/

void flDelayLoop
    (
    int  cycles
    )
    {
    while (--cycles)
	;
    }
#if FALSE
/*******************************************************************************
*
* sysTffsFormat - format the flash memory above an offset
*
* This routine formats the flash memory.  Because this function defines
* the symbolic constant, HALF_FORMAT, the lower half of the specified flash
* memory is left unformatted.  If the lower half of the flash memory was
* previously formated by TrueFFS, and you are trying to format the upper half,
* you need to erase the lower half of the flash memory before you format the
* upper half.  To do this, you could use:
* .CS
* tffsRawio(0, 3, 0, 8)
* .CE
* The first argument in the tffsRawio() command shown above is the TrueFFS
* drive number, 0.  The second argument, 3, is the function number (also
* known as TFFS_PHYSICAL_ERASE).  The third argument, 0, specifies the unit
* number of the first erase unit you want to erase.  The fourth argument, 8,
* specifies how many erase units you want to erase.
*
* RETURNS: OK, or ERROR if it fails.
*/

STATUS sysTffsFormat (void)
    {
    STATUS status;
    tffsDevFormatParams params =
	{
#define	HALF_FORMAT	/* lower 0.5MB for bootimage, upper 1.5MB for TFFS */
#ifdef	HALF_FORMAT
	{0x80000l, 99, 1, 0x10000l, NULL, {0,0,0,0}, NULL, 2, 0, NULL},
#else
	{0x000000l, 99, 1, 0x10000l, NULL, {0,0,0,0}, NULL, 2, 0, NULL},
#endif	/* HALF_FORMAT */
	FTL_FORMAT_IF_NEEDED
	};

    /* we assume that the drive number 0 is RFA */

    status = tffsDevFormat (0, (int)&params);
    return (status);
    }
#endif /* FALSE */
#endif /* INCLUDE_TFFS_VXBFLASH */
#endif /* INCLUDE_TFFS */

