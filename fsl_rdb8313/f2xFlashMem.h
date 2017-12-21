/* f2xFlashMem.h - Intel28 and AMD29-style Flash driver header */

/*
 * Copyright (c) 2000, 2002-2004 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,02dec04,tjf upped version number to match f2xFlashMem.c
01c,10oct03,tjf modifications for v01g f2xFlashMem.c
01b,16aug02,tjf modifications for v01d f2xFlashMem.c
01a,19oct00,tjf written by Ted Friedl, Wind River PS, Madison, WI
*/

#ifndef __INCf2xFlashMemh
#define __INCf2xFlashMemh

#ifdef __cplusplus
extern "C" {
#endif

#include "vxWorks.h"
#include "semLib.h"
#include "private/semLibP.h"
#include "sysLib.h"

/* types */

#define F28_TYPE                28
#define F29_8BIT_TYPE           298
#define F29_16BIT_TYPE          2916
#define F29_32BIT_TYPE          2932

#define F2X_TYPE_MASK           0xfff     /* The decimal numbers above fit in
                                           * 12 bits.  This mask and its inverse
                                           * are used to separate these numbers
                                           * from the enhanced feature bits
                                           * below.
                                           */
/* enhanced features (part of type) */

#define F2X_WB32_TYPE           0x00001000

/* soon to be obsolte types */

#define F28_WB32_TYPE           (F2X_WB32_TYPE | F28_TYPE)  /* was 2832 */

/* commands */

#define F28_CMD_CLEAR_STATUS    0x50
#define F28_CMD_READ_ARRAY      0xff
#define F28_CMD_BLOCK_ERASE     0x20
#define F28_CMD_CONFIRM         0xd0
#define F28_CMD_PROGRAM         0x40
#define F28_CMD_WRITE_BUFFER    0xe8

#define F29_CMD_RESET           0xf0
#define F29_CMD_UNLOCK1         0xaa
#define F29_CMD_UNLOCK2         0x55
#define F29_CMD_PROGRAM_BUFFER  0x29
#define F29_CMD_BLOCK_ERASE     0x80
#define F29_CMD_ERASE_CONFIRM   0x30
#define F29_CMD_PROGRAM         0xa0
#define F29_CMD_WRITE_BUFFER    0x25

#define F29_OFF_RESET           0x000
#define F29_OFF_UNLOCK1         0x555
#define F29_OFF_UNLOCK2         0x2aa
#define F29_OFF_BLOCK_ERASE     0x555
#define F29_OFF_PROGRAM         0x555

/* status */

#define F28_STAT_WSMS           (1 << 7)  /* write state machine status       */
                                          /* 1 = ready                        */
                                          /* 0 = busy                         */
#define F28_STAT_ESS            (1 << 6)  /* erase suspend status             */
                                          /* 1 = erase suspended              */
                                          /* 0 = erase in progress/complete   */
#define F28_STAT_ES             (1 << 5)  /* erase status                     */
                                          /* 1 = error in block erase         */
                                          /* 0 = successful block erase       */
#define F28_STAT_PS             (1 << 4)  /* program status                   */
                                          /* 1 = error in programming         */
                                          /* 0 = successful programming       */
#define F28_STAT_VPPS           (1 << 3)  /* Vpp status                       */
                                          /* 1 = Vpp low, operation abort     */
                                          /* 0 = Vpp OK                       */
#define F28_STAT_PSS            (1 << 2)  /* program suspend status           */
                                          /* 1 = program suspended            */
                                          /* 0 = program in progress/complete */
#define F28_STAT_BLS            (1 << 1)  /* block lock status                */
                                          /* 1 = prog/erase on a locked       */
                                          /*     block; operation aborted     */
                                          /* 0 = No operation to locked block */
#define F28_STAT_R              (1 << 0)  /* reserved                         */


#define F29_STAT_DQ7            (1 << 7)  /* data polling bit                 */
                                          /* data = success                   */
                                          /* !data = busy or error            */
#define F29_STAT_DQ5            (1 << 5)  /* program/erase status             */
                                          /* 1 = error in programming/erasure */
                                          /* 0 = successful program/erasure   */

#define F29_STATUS_OK           0
#define F29_STATUS_BUSY         1
#define F29_STATUS_ERROR        2

/* driver defines */

#define F2X_MAX_REGION_WIDTH    8         /* bytes */
#define F2X_TIMEOUT             15        /* seconds */

typedef int                     F29_STATUS;

/* driver structures */

typedef struct                  /* F2X_RDESC */
    {
    /* user input */

    char *      baseAdrs;       /* base address of flash region */
    int         regionWidth;    /* width of flash region in bytes */
    int         regionSize;     /* entire size of flash region in bytes */
    int         chipWidth;      /* width of single flash part in bytes */
    int         chipBlockSize;  /* block size of a single flash chip in bytes */
    int         writeWidth;     /* width of a region write in bytes */
    int         type;           /* F2x_xxx_TYPE - see f2xFlashMem.h */
    VOIDFUNCPTR pageFunc;       /* page function (or NULL) */
    int         pageId;         /* argument to page function (or NONE) */

    /* f28 command words */

    char        cmdClearStatus[F2X_MAX_REGION_WIDTH];
    char        cmdReadArray[F2X_MAX_REGION_WIDTH];

    /* f29 command words */

    char        cmdReset[F2X_MAX_REGION_WIDTH];
    char        cmdUnlock1[F2X_MAX_REGION_WIDTH];
    char        cmdUnlock2[F2X_MAX_REGION_WIDTH];
    char        cmdProgramBuffer[F2X_MAX_REGION_WIDTH];

    /* common command words */

    char        cmdBlockErase[F2X_MAX_REGION_WIDTH];
    char        cmdConfirm[F2X_MAX_REGION_WIDTH];
    char        cmdProgram[F2X_MAX_REGION_WIDTH];
    char        cmdWriteBuffer[F2X_MAX_REGION_WIDTH];

    /* f29 command offsets */

    int         offReset;
    int         offUnlock1;
    int         offUnlock2;
    int         offBlockErase;
    int         offProgram;

    /* control */

    FUNCPTR     programRtn;     /* programming routine */
    FUNCPTR     blockEraseRtn;  /* block erase routine */
    SEMAPHORE   semaphore;      /* storage for semaphore (may be shared) */
    SEM_ID      semId;          /* serializes read/write/erase operations */
    } F2X_RDESC;

/* object references */

typedef F2X_RDESC * F2X_RID;
typedef void *      F2X_GID;   /* Note: This file could be included by a driver
                                * released "object only."  F2X_GID is
                                * therefore set to "void *" because of its
                                * dependence on the BSP define F2X_MAX_REGIONS.
                                */

#if defined(__STDC__) || defined(__cplusplus)

IMPORT F2X_RID f2xRegionCreate (char * baseAdrs, int regionWidth,
                                int regionSize, int chipWidth,
                                int chipBlockSize, int writeWidth,
                                int type, F2X_RID dependRid,
                                VOIDFUNCPTR pageFunc, int pageId);

IMPORT F2X_GID f2xGroupCreate (F2X_RID rid);
IMPORT STATUS  f2xGroupAdd (F2X_GID gid, F2X_RID rid);

IMPORT STATUS f2xGroupSet (F2X_GID id, char * pSrc, int nbytes, int offset);
IMPORT STATUS f2xGroupGet (F2X_GID id, char * pDst, int nbytes, int offset);
IMPORT STATUS f2xGroupErase (F2X_GID id);
IMPORT int    f2xGroupSize (F2X_GID id);

IMPORT STATUS f2xGroupProgram (F2X_GID id, char * pSrc, int nbytes, int offset);
IMPORT char * f2xGroupMap (F2X_GID id, int offset);
IMPORT STATUS f2xGroupBlockErase (F2X_GID id, int offset);
IMPORT int    f2xGroupBlockSize (F2X_GID id);

IMPORT STATUS sysFlashAdd (F2X_RID rid);

IMPORT STATUS sysFlashSet (char * pSrc, int nbytes, int offset);
IMPORT STATUS sysFlashGet (char * pDst, int nbytes, int offset);
IMPORT STATUS sysFlashErase (void);
IMPORT int    sysFlashSize (void);

IMPORT STATUS sysNvRamAdd (F2X_RID rid);

#else /* __STDC__ */

IMPORT F2X_RID f2xRegionCreate();
IMPORT F2X_GID f2xGroupCreate();
IMPORT STATUS  f2xGroupAdd();

IMPORT STATUS f2xGroupSet();
IMPORT STATUS f2xGroupGet();
IMPORT STATUS f2xGroupErase();
IMPORT int    f2xGroupSize();

IMPORT STATUS f2xGroupProgram();
IMPORT char * f2xGroupMap();
IMPORT STATUS f2xGroupBlockErase();
IMPORT int    f2xGroupBlockSize();

IMPORT STATUS sysFlashAdd();

IMPORT STATUS sysFlashSet();
IMPORT STATUS sysFlashGet();
IMPORT STATUS sysFlashErase();
IMPORT int    sysFlashSize();

IMPORT STATUS sysNvRamAdd();

#endif /* __STDC__ */

#ifdef __cplusplus
}
#endif

#endif /* __INCf2xFlashMemh */
