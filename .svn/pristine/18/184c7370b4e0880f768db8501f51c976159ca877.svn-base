/* vxbIntCtlrLib.h - VxBus Interrupt Controller Support Library */

/*
 * Copyright (C) 2006-2009  Wind River Systems, Inc. All rights are reserved.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01k,16jul09,rgo  Introduce dynamicArg struct to wrap
                 intCtlrHwConf/vectNum(CQ:145042)
01j,20feb09,pmr  interrupt triggering
01i,22oct08,h_k  added intCtlrISRChainEntry.
01h,15feb08,h_k  optimized VXB_INTCTLR_ISR_CALL for PPC.
                 put isr, pArg and pinFlags fields in vxbIntCtlrPin on one
                 cache line.
01g,15jul07,tor  VXB_INTR_DYNAMIC moved to target/h
01f,21jun07,tor  intCtlrTableIsrGet and intCtlrTableArgGet are globals
01f,21jun07,dtr  Add extra flags.
01e,06jun07,dtr  Export intCtlrPinFind API.
01d,01may07,slk  add macros to help search the pin entry list
01c,17apr07,slk  add macros for table and pin element number
01b,15Mar07,tor  update structures
01a,20Sep06,tor  created
*/

#ifndef _INC_vxbIntCtlrLibH
#define _INC_vxbIntCtlrLibH

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/hwMemLib.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <logLib.h>
#include <intLib.h>
#include <stdio.h>
#include <hwif/vxbus/vxbIntrCtlr.h>

/* Interrupt controller input pin flags */

/*
 * we use the low order 16 bits for input pin number
 * and the upper 16 bits for flags
 */

#define VXB_INTCTLR_INPIN_MASK	0x0000ffff

#define VXB_INTCTLR_FLG_ENABLE	0x00010000
#define VXB_INTCTLR_SPECIFIC_1     0x40000000
#define VXB_INTCTLR_SPECIFIC_2     0x80000000

/* Table sizes */

/*
 * To represent input pins, we use a two-level table.  This gives
 * us a good compromise between supporting a large number of inputs
 * and requiring a small amount of memory to hold the table.  The
 * table can be "sparsely" populated, so that if there is a chunk
 * without any inputs, which fills an entire lower-level table,
 * then we don't allocate memory for that table.
 *
 * The choices of table sizes is determined by the allocation strategy
 * used in hwMemLib.  The top-level table can be 250 entries, and
 * still fit within a 1008-byte block, with 4 bytes reserved for
 * a next pointer and 4 bytes reserved for future use.
 * The lower-level tables can be 12 entries and fit within a
 * 496-byte block.  This allows each interrupt controller to service
 * 3000 inputs.  If we ever find an interrupt controller that has
 * more than this, we'll use the next pointer to create a chain.
 */

#define VXB_INTCTLRLIB_TOPLVL_SIZE    496

#define VXB_INTCTLRLIB_LOWLVL_SIZE_POW 3 /*2^x =VXB_INTCTLRLIB_LOWLVL_SIZE */
/* Make this power of 2 */
#define VXB_INTCTLRLIB_LOWLVL_SIZE   ( 1<< VXB_INTCTLRLIB_LOWLVL_SIZE_POW)
   
/* defines to get the tbl and pins index numbers based on input pin number
 * also a define to determine the number of entries in the 
 * vxbIntCtlrPin structure
 */

#define VXB_INTCTLRLIB_NUM_PINS(pEnt)  (NELEMENTS(pEnt->pTop->tbl[0]->pins))

#define VXB_INTCTLRLIB_TBL_IDX(pEnt,inpPin) \
                           (inpPin / VXB_INTCTLRLIB_NUM_PINS(pEnt))
#define VXB_INTCTLRLIB_PINS_IDX(pEnt,inpPin) \
                           (inpPin % VXB_INTCTLRLIB_NUM_PINS(pEnt))

/* The cache line size is multiple of 16-byte on most of the architectures. */

#define VXB_INTCTLRLIB_MIN_CACHE_LINE_SIZE	16

struct intCtlrMoreDrvName
    {
    struct intCtlrMoreDrvName * pNext;
    char *			drvName;
    int				drvUnit;
    int				pinIndex;
    };

struct vxbIntCtlrPin
    {
    struct intCtlrMoreDrvName * pMoreNames;
    char *                      drvName;
    int                         drvUnit;
    int                         intIndex;

    /*
     * Maintain isr, pArg and pinFlags fields on one cache line to read them
     * from interrupt handler effectively.
     * The cache line size is multiple of 16-byte on most of the architectures,
     * so put them within 16-byte.
     */

    void                        (*isr)(void * pArg, int inputPin);
    void *                      pArg;
    int                         pinFlags;
    UINT16                      pinPriority;
    UINT8			pinTrigger;
    UINT8			resbyte;
    UINT32                      pinOutput;
    UINT32                      pinCpu;
    int				reserved [2];
    };

struct vxbIntCtlrInput
    {
    struct vxbIntCtlrPin      pins[VXB_INTCTLRLIB_LOWLVL_SIZE];
    };

struct vxbIntCtlrInpTop
    {
    struct vxbIntCtlrInpTop * pNext;          /* not implemented */
    struct vxbIntCtlrInput *  tbl[VXB_INTCTLRLIB_TOPLVL_SIZE];
    UINT32                    reserved;
    };

/* helper routines */
void intCtlrStrayISR
    (
    void *	pArg,
    int		inputPin
    );

#define VXB_INTCTLR_ISR_CALL(ent, inpPin)                          \
    {                                                              \
    FAST int tblIndx = (inpPin >> VXB_INTCTLRLIB_LOWLVL_SIZE_POW); \
    FAST int pinIndx = inpPin & (VXB_INTCTLRLIB_LOWLVL_SIZE - 1);  \
    FAST struct vxbIntCtlrPin * pPin =                             \
                    &((ent)->pTop->tbl[tblIndx]->pins[pinIndx]);   \
    FAST void * pArg = pPin->pArg;                                 \
    FAST int pinFlags = pPin->pinFlags;                            \
    /*                                                             \
     * In order to generate minimum instructions from              \
     * ppcIntCtlrISR_INTR() for PPC with -Xlocals-on-stack option  \ 
     * by the current Diab compiler, Ver5.6.0.0, don't assign pIsr \
     * to a regsiter.                                              \
     */                                                            \
    VOIDFUNCPTR pIsr = pPin->isr;                                  \
    (*pIsr)(pArg, pinFlags);                                       \
    }

#define VXB_INTCTLR_PINENTRY_ENABLED(pEnt,inpPin,flag)        \
    {                                                         \
    int tblIndx = VXB_INTCTLRLIB_TBL_IDX(pEnt,inpPin);        \
    int pinIndx = VXB_INTCTLRLIB_PINS_IDX(pEnt,inpPin);       \
    FAST struct vxbIntCtlrPin * pPin;                         \
    flag = FALSE;                                             \
    if (pEnt->pTop->tbl[tblIndx] != NULL)                     \
        {                                                     \
        pPin = &((pEnt)->pTop->tbl[tblIndx]->pins[pinIndx]);  \
        if (pPin->pinFlags & VXB_INTCTLR_FLG_ENABLE)          \
            flag = TRUE;                                      \
        }                                                     \
    }

#define VXB_INTCTLR_PINENTRY_ALLOCATED(pEnt,inpPin,flag)      \
    {                                                         \
    int tblIndx = VXB_INTCTLRLIB_TBL_IDX(pEnt,inpPin);        \
    flag = FALSE;                                             \
    if (pEnt->pTop->tbl[tblIndx] != NULL)                     \
        {                                                     \
        flag = TRUE;                                          \
        }                                                     \
    }

typedef struct intCtlrHwConf INTCTLR_HWCONF;

struct intCtlrHwConf
    {
    VXB_DEVICE_ID	  ctlrDev;	  /* VXB_DEVICE_ID of int ctlr */
    struct vxbIntCtlrInpTop * pTop;       /* top-level table of entries */
    struct vxbLock         vxbIntCtlrLibLock;
    };

struct dynamicArg
    {
    struct intCtlrHwConf isrHandle;
    UINT32 vectNum;
    };

struct intCtlrISRChainEntry
    {
    struct intCtlrISRChainEntry * pNext;
    void			  (*isr)(void * pArg, int inputPin);
    void *			  pArg;
    UINT32			  flags;
    };

struct isrRerouteInfo
    {
    struct vxbIntCtlrPin *      pPin;
    int				cpuNo;
    };

IMPORT void * intCtlrTableIsrGet
    (
    struct intCtlrHwConf *      pInputs,
    int                         inputPin
    );
IMPORT void * intCtlrTableArgGet
    (
    struct intCtlrHwConf *      pInputs,
    int                         inputPin
    );
IMPORT void intCtlrChainISR
    (
    void *      pArg,
    int         inputPin        /* unused */
    );
IMPORT BOOL intCtlrISRDisable
    (
    struct intCtlrHwConf *      pEntries,
    int                         inputPin,
    void                        (*isr),
    void *                      pArg
    );
IMPORT STATUS intCtlrISREnable
    (
    struct intCtlrHwConf *      pEntries,
    int                         inputPin,
    void                        (*isr),
    void *                      pArg
    );
IMPORT STATUS intCtlrISRRemove
    (
    struct intCtlrHwConf *      pEntries,
    int                         inputPin,
    void                        (*isr),
    void *                      pArg
    );
IMPORT STATUS intCtlrISRAdd
    (
    struct intCtlrHwConf *      pEntries,
    int                         inputPin,
    void                        (*isr),
    void *                      pArg
    );
IMPORT int intCtlrDevCheck
    (
    VXB_DEVICE_ID               pDev,
    int                         pinIndex,
    struct intCtlrHwConf *      pConf
    );
IMPORT STATUS intCtlrHwConfGet
    (
    VXB_DEVICE_ID               pInst,
    HCF_DEVICE *                pHcf,
    struct intCtlrHwConf *      pEntries
    );
IMPORT STATUS intCtlrHwConfShow
    (
    struct intCtlrHwConf *      pEntries
    );

LOCAL __inline__ struct vxbIntCtlrPin * vxbIntCtlrPinEntryGet
    (
    struct intCtlrHwConf *    pEnt,
    int                               inpPin
    )
    {
    int tblIndx = VXB_INTCTLRLIB_TBL_IDX(pEnt,inpPin);
    int pinIndx = VXB_INTCTLRLIB_PINS_IDX(pEnt,inpPin);
    return(&(pEnt->pTop->tbl[tblIndx]->pins[pinIndx]));
    }

extern int intCtlrPinFind
    (
    VXB_DEVICE_ID               pDev,
    int                         pinIndex,
    VXB_DEVICE_ID               pIntCtlrCaller,
    struct intCtlrHwConf *      pConf
    );

extern void * intCtlrTableArgGet
    (
    struct intCtlrHwConf *	pInputs,
    int				inputPin
    );

STATUS intCtlrTableCreate
    (
    struct intCtlrHwConf *	pInputs,
    int				inputPin
    );

extern void intCtlrTableUserSet
    (
    struct intCtlrHwConf *	pInputs,
    int				inputPin,
    char *			drvName,
    int				drvUnit,
    int				drvIndex
    );

extern void intCtlrTableFlagsSet
    (
    struct intCtlrHwConf *	pInputs,
    int				inputPin,
    UINT32			flagValue
    );

extern UINT32 intCtlrTableFlagsGet
    (
    struct intCtlrHwConf *	pInputs,
    int				inputPin
    );

#endif /* _INC_vxbIntCtlrLibH */
