/* sysGei82543End.c - Intel Pro1000 F/T Adapter END driver support routines */

/*
 * Copyright 2002, 2004, 2006, 2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 2001 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01i,28aug08,x_z	Moved intEnable from sys82543BoardInit to sys543IntEnable
                (defect WIND00129531)
01h,16jul06,??? PPC Dec Timer vxBus complaint
01g,01nov04,mdo Documentation fixes for apigen
01f,07oct04,jln spr#102216
01e,18may04,jln fix compiler warning
01d,03dec02,scb Fixed warnings flagged by diab compiler.
01c,19nov02,scb Fix for gei buffers in cacheable copyback.
01b,15nov02,scb Reserve non-cached low memory for gei ethernet buffers.
01a,16oct02,scb Ported. (from ver 01a, mcp820/sysGei82543End.c)
*/

/*
DESCRIPTION

This module is BSP support for Intel PRO1000 F/T adaptors.

INCLUDE FILES

SEE ALSO: ifLib,
.I "RS82543GC GIGABIT ETHERNET CONTROLLER NETWORKING SILICON DEVELOPER'S MANUAL"
*/

#include "vxWorks.h"
#include "taskLib.h"
#include "sysLib.h"
#include "config.h"
#include "end.h"
#include "vmLib.h"
#include "drv/pci/pciIntLib.h"
#include "drv/end/gei82543End.h"

#ifdef I82543_DEBUG
#   undef    LOCAL
#   define    LOCAL
#endif    /* I82543_DEBUG */

/* include PCI Library */

#ifndef INCLUDE_PCI
#define INCLUDE_PCI
#endif /* INCLUDE_PCI */

/* PCI configuration type */

#ifndef PCI_CFG_TYPE
#define PCI_CFG_TYPE               PCI_CFG_NONE
#endif

/* Default RX descriptor  */

#ifndef GEI_RXDES_NUM
#define GEI_RXDES_NUM              GEI_DEFAULT_RXDES_NUM
#endif

/* Default TX descriptor  */

#ifndef GEI_TXDES_NUM
#define GEI_TXDES_NUM              GEI_DEFAULT_TXDES_NUM
#endif

/* Default User's flags  */

#ifndef GEI_USR_FLAG
#define GEI_USR_FLAG               GEI_DEFAULT_USR_FLAG
#endif

#define GEI0_MEMBASE0_LOW           0xfd000000    /* mem base for CSR */
#define GEI0_MEMBASE0_HIGH          0x00000000    /* mem base for CSR */
#define GEI0_MEMSIZE0               0x20000       /* mem size - CSR,128KB */
#define GEI0_MEMBASE1               0xfd100000    /* mem base - Flash */
#define GEI0_MEMSIZE1               0x00080000    /* mem size - Flash,512KB */
#define GEI0_INT_LVL                0x0b          /* IRQ 11 */
#define GEI0_INIT_STATE_MASK        (VM_STATE_MASK_FOR_ALL)
#define GEI0_INIT_STATE             (VM_STATE_FOR_PCI)
#define GEI0_SHMEM_BASE             NONE
#define GEI0_SHMEM_SIZE             0
#define GEI0_RXDES_NUM              GEI_RXDES_NUM
#define GEI0_TXDES_NUM              GEI_TXDES_NUM
#define GEI0_USR_FLAG               GEI_USR_FLAG

#define GEI1_MEMBASE0_LOW           0xfd200000    /* mem base for CSR */
#define GEI1_MEMBASE0_HIGH          0x00000000    /* mem base for CSR */
#define GEI1_MEMSIZE0               0x20000       /* mem size - CSR,128KB */
#define GEI1_MEMBASE1               0xfd300000    /* mem base for Flash */
#define GEI1_MEMSIZE1               0x00080000    /* mem size - Flash,512KB */
#define GEI1_INT_LVL                0x05          /* IRQ 5 */
#define GEI1_INIT_STATE_MASK        (VM_STATE_MASK_FOR_ALL)
#define GEI1_INIT_STATE             (VM_STATE_FOR_PCI)
#define GEI1_SHMEM_BASE             NONE
#define GEI1_SHMEM_SIZE             0
#define GEI1_RXDES_NUM              GEI_RXDES_NUM
#define GEI1_TXDES_NUM              GEI_TXDES_NUM
#define GEI1_USR_FLAG               GEI_USR_FLAG

#define GEI2_MEMBASE0_LOW           0xfd400000    /* mem base - CSR */
#define GEI2_MEMBASE0_HIGH          0x00000000    /* mem base - CSR */
#define GEI2_MEMSIZE0               0x20000       /* mem size - CSR, 128KB */
#define GEI2_MEMBASE1               0xfd500000    /* mem base - Flash */
#define GEI2_MEMSIZE1               0x00080000    /* mem size - Flash,512KB */
#define GEI2_INT_LVL                0x0c          /* IRQ 12 */
#define GEI2_INIT_STATE_MASK        (VM_STATE_MASK_FOR_ALL)
#define GEI2_INIT_STATE             (VM_STATE_FOR_PCI)
#define GEI2_SHMEM_BASE             NONE
#define GEI2_SHMEM_SIZE             0
#define GEI2_RXDES_NUM              GEI_RXDES_NUM
#define GEI2_TXDES_NUM              GEI_TXDES_NUM
#define GEI2_USR_FLAG               GEI_USR_FLAG

#define GEI3_MEMBASE0_LOW           0xfd600000   /* mem base - CSR */
#define GEI3_MEMBASE0_HIGH          0x00000000   /* mem base - CSR */
#define GEI3_MEMSIZE0               0x20000      /* mem size - CSR,128KB */
#define GEI3_MEMBASE1               0xfd700000   /* mem base - Flash */
#define GEI3_MEMSIZE1               0x00080000   /* mem size - Flash,512KB */
#define GEI3_INT_LVL                0x09         /* IRQ 9 */
#define GEI3_INIT_STATE_MASK        (VM_STATE_MASK_FOR_ALL)
#define GEI3_INIT_STATE             (VM_STATE_FOR_PCI)
#define GEI3_SHMEM_BASE             NONE
#define GEI3_SHMEM_SIZE             0
#define GEI3_RXDES_NUM              GEI_RXDES_NUM
#define GEI3_TXDES_NUM              GEI_TXDES_NUM
#define GEI3_USR_FLAG               GEI_USR_FLAG

#define GEI82543_LOAD_FUNC          gei82543EndLoad
#define GEI_X86_OFFSET_VALUE        0x0

/* Alaska PHY's information */

#define MARVELL_OUI_ID              0x5043
#define MARVELL_ALASKA_88E1000      0x5
#define MARVELL_ALASKA_88E1000S     0x4
#define ALASKA_PHY_SPEC_CTRL_REG        0x10
#define ALASKA_PHY_SPEC_STAT_REG        0x11
#define ALASKA_INT_ENABLE_REG           0x12
#define ALASKA_INT_STATUS_REG           0x13
#define ALASKA_EXT_PHY_SPEC_CTRL_REG    0x14
#define ALASKA_RX_ERROR_COUNTER         0x15
#define ALASKA_LED_CTRL_REG             0x18

#define ALASKA_PSCR_ASSERT_CRS_ON_TX    0x0800
#define ALASKA_EPSCR_TX_CLK_25          0x0070

#define ALASKA_PSCR_AUTO_X_1000T        0x0040
#define ALASKA_PSCR_AUTO_X_MODE         0x0060

#define ALASKA_PSSR_DPLX                0x2000
#define ALASKA_PSSR_SPEED               0xC000
#define ALASKA_PSSR_10MBS               0x0000
#define ALASKA_PSSR_100MBS              0x4000
#define ALASKA_PSSR_1000MBS             0x8000

/* assuming 1:1 mapping for virtual:physical address */

#define GEI_SYS_WRITE_REG(unit, reg, value) \
        geiSysWriteReg(unit, reg, value)

#define GEI_SYS_READ_REG(unit, reg) geiSysReadReg(unit, reg)

/* externs */

IMPORT END_TBL_ENTRY    endDevTbl[];    /* end device table */

/* typedefs */

#define EEPROM_ENABLE_BITS              9
#define EEPROM_WRITE_DIS_OPCODE         ((0x4 << 6) | (0x0 << 4))
#define EEPROM_WRITE_EN_OPCODE          ((0x4 << 6) | (0x3 << 4))
#define EEPROM_WRITE_ALL_OPCODE         ((0x4 << 6) | (0x1 << 4))
#define EEPROM_ERASE_ALL_OPCODE         ((0x4 << 6) | (0x2 << 4))

typedef struct geiResource        /* GEI_RESOURCE */
    {
    UINT32 memBaseLow;            /* Base Address LOW */
    UINT32 memBaseHigh;           /* Base Address HIGH */
    UINT32 flashBase;             /* Base Address for FLASH */
    char   irq;                   /* Interrupt Request Level */
    BOOL   adr64;                 /* Indicator for 64-bit support */
    int    boardType;             /* type of LAN board this unit is */
    int    pciBus;                /* PCI Bus number */
    int    pciDevice;             /* PCI Device number */
    int    pciFunc;               /* PCI Function number */
    UINT   memLength;             /* required memory size */
    UINT   initialStateMask;      /* mask parameter to vmStateSet */
    UINT   initialState;          /* state parameter to vmStateSet */
    UINT16 eeprom_icw1;           /* EEPROM initialization control word 1 */
    UINT16 eeprom_icw2;           /* EEPROM initialization control word 2 */
    UCHAR  enetAddr[6];           /* MAC address for this adaptor */
    STATUS iniStatus;             /* initialization perform status */
    UINT32 shMemBase;             /* Share memory address if any */
    UINT32 shMemSize;             /* Share memory size if any */
    UINT32 rxDesNum;              /* RX descriptor for this unit */
    UINT32 txDesNum;              /* TX descriptor for this unit */
    UINT32 usrFlags;              /* user flags for this unit */
    } GEI_RESOURCE;

/* locals */

LOCAL UINT32 sys543LocalToPciBusAdrs (int unit, UINT32 adrs);
LOCAL UINT32 sys543PciBusToLocalAdrs (int unit, UINT32 adrs);
LOCAL UINT32 geiUnits;     /* number of GEIs we found */

LOCAL GEI_RESOURCE geiResources [] =
    {
    {GEI0_MEMBASE0_LOW,GEI0_MEMBASE0_HIGH, GEI0_MEMBASE1, GEI0_INT_LVL,
     FALSE, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN,
     GEI0_MEMSIZE0, GEI0_INIT_STATE_MASK, GEI0_INIT_STATE, 0, 0, {0xff},
     ERROR, (UINT32)GEI0_SHMEM_BASE, GEI0_SHMEM_SIZE, GEI0_RXDES_NUM, GEI0_TXDES_NUM,
     GEI0_USR_FLAG},
    {GEI1_MEMBASE0_LOW, GEI1_MEMBASE0_HIGH, GEI1_MEMBASE1, GEI1_INT_LVL,
     FALSE, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN,
     GEI1_MEMSIZE0, GEI1_INIT_STATE_MASK, GEI1_INIT_STATE, 0, 0, {0xff},
     ERROR, (UINT32)GEI1_SHMEM_BASE, GEI1_SHMEM_SIZE, GEI1_RXDES_NUM, GEI1_TXDES_NUM,
     GEI1_USR_FLAG},
    {GEI2_MEMBASE0_LOW, GEI2_MEMBASE0_HIGH, GEI2_MEMBASE1, GEI2_INT_LVL,
     FALSE,UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN,
     GEI2_MEMSIZE0, GEI2_INIT_STATE_MASK, GEI2_INIT_STATE, 0, 0, {0xff},
     ERROR, (UINT32)GEI2_SHMEM_BASE, GEI2_SHMEM_SIZE, GEI2_RXDES_NUM, GEI2_TXDES_NUM,
     GEI2_USR_FLAG},
    {GEI3_MEMBASE0_LOW,GEI3_MEMBASE0_HIGH, GEI3_MEMBASE1, GEI3_INT_LVL,
     FALSE, UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN,
     GEI3_MEMSIZE0, GEI3_INIT_STATE_MASK, GEI3_INIT_STATE, 0, 0, {0xff},
     ERROR, (UINT32)GEI3_SHMEM_BASE, GEI3_SHMEM_SIZE, GEI3_RXDES_NUM, GEI3_TXDES_NUM,
     GEI3_USR_FLAG},
     };

/* forward declarations */

STATUS sys543PciInit (void);
#if 0
LOCAL void      sys543LoadStrCompose (int unit);
#endif

STATUS          sys82543BoardInit(int unit, ADAPTOR_INFO *pBoard);
LOCAL UINT16    sys543eepromReadBits(int unit, int bitsNum);
LOCAL void      sys543eepromWriteBits(int unit, UINT16 value, UINT16 bitNum);
LOCAL UINT16    sys543eepromReadWord (int unit, UINT32 index);
LOCAL STATUS    sys543EtherAdrGet (int unit);
LOCAL STATUS    sys543eepromCheckSum (int unit);
LOCAL void      sys543PhySpecRegsInit(PHY_INFO *, UINT8 phyAddr);
LOCAL int       sys543IntAck     (int unit);
LOCAL UINT32    sys543LocalToPciBusAdrs(int unit, UINT32 adrs);
LOCAL UINT32    sys543PciBusToLocalAdrs(int unit, UINT32 adrs);
LOCAL int       sys543IntEnable  (int unit);
LOCAL int       sys543IntDisable (int unit);
void            sys543Show (UINT32 unit);
UINT16          sys543eepromWriteWord(int unit, UINT32 index, UINT16 value);
STATUS          sys543EepromDump(int unit, char *adr);
LOCAL void      sys1000NsDelay (void);
LOCAL void      geiSysWriteReg (int unit, UINT32 reg, UINT32 value);
LOCAL UINT32    geiSysReadReg (int unit, UINT32 reg);

/*****************************************************************************
*
* sys543PciInit - Initialize and get the PCI configuration for 82543 Chips
*
* This routine finds out PCI device, and maps its memory and IO address.
* It must be done prior to initializing of 82543 chips.  Also
* must be done prior to MMU initialization, usrMmuInit().
*
* RETURNS: N/A
*
* ERRNO
*/

STATUS sys543PciInit (void)
    {
    GEI_RESOURCE *pReso;         /* chip resources */
    int pciBus;                  /* PCI bus number */
    int pciDevice;               /* PCI device number */
    int pciFunc;                 /* PCI function number */
    int unit;                    /* unit number */
    int pro1000TDevUnit=0;       /* count of Intel Pro1000T */
    int pro1000DevUnit =0;       /* count of Intel Pro1000F/T */
    int pro1000_544DevUnit = 0;  /* count of Intel Pro1000_544 */
    BOOL duplicate;              /* BOOL flag for duplicate chip */
    UINT32 bar0;                 /* PCI BAR_0 */
    UINT32 memBaseLow;           /* mem base low */
    UINT32 memBaseHigh;          /* mem base High */
    UINT32 flashBase;            /* flash base */
    UINT16 boardId =0;           /* adaptor Id */
    UINT8  irq;                  /* irq number */
    int    ix;                   /* index */

    for (unit = 0; unit < (int)NELEMENTS(geiResources); unit++)
        {
        boardId = (UINT16)UNKNOWN;

        /*
         * The following PCI information is based on adapters (T/F)
         * from Intel.
         * PCI Device ID    SUB-SYSTEM ID    ADAPTOR
         *  0x1001           0x1003          INTEL PRO1000 F
         *  0x1001           0x1004          INTEL PRO1000 T
         *  0x1004             X             INTEL PRO1000 T
         *  0x1008             X             INTEL PRO1000 544
         */

        if ((pciFindDevice (PRO1000_PCI_VENDOR_ID, PRO1000_544_BOARD,
                            pro1000TDevUnit, &pciBus, &pciDevice, &pciFunc)
             == OK))
            {
            pro1000_544DevUnit++;

            boardId = PRO1000_544_BOARD;
            }

        else if (pciFindDevice (PRO1000_PCI_VENDOR_ID, PRO1000T_PCI_DEVICE_ID,
                                pro1000TDevUnit, &pciBus, &pciDevice, &pciFunc)
                 == OK)
            {
            pro1000TDevUnit++;

            /* Distinguish Pro1000T and Pro1000F adapter by SUB_SYSTEM_ID */

            boardId = PRO1000T_BOARD;
            }

        /* Detect possible PRO1000T/F adaptors */

        else if (pciFindDevice (PRO1000_PCI_VENDOR_ID, PRO1000_PCI_DEVICE_ID,
                                pro1000DevUnit, &pciBus, &pciDevice,
                                &pciFunc) == OK)
            {

            pro1000DevUnit++;

            /* Distinguish Pro1000T and Pro1000F adapter by SUB_SYSTEM_ID */

            pciConfigInWord(pciBus, pciDevice, pciFunc,
                            PCI_CFG_SUB_SYSTEM_ID, &boardId);

            if (boardId == PRO1000T_PCI_SUBSYSTEM_ID)
                boardId = PRO1000T_BOARD;
            else if (boardId == PRO1000F_PCI_SUBSYSTEM_ID)
                boardId = PRO1000F_BOARD;
            }
        else
            break;

        /* check the duplicate */

        pReso     = &geiResources [0];
        duplicate = FALSE;

        for (ix = 0; ix < (int)NELEMENTS(geiResources); ix++, pReso++)
            {
            if ((ix != unit) && (pReso->pciBus == pciBus) &&
                (pReso->pciDevice == pciDevice) && (pReso->pciFunc == pciFunc))
                duplicate = TRUE;
            }

        if (duplicate)
            continue;

        /* we found the right one */

        pReso = &geiResources [unit];
        pReso->boardType = boardId;
        pReso->pciBus    = pciBus;
        pReso->pciDevice = pciDevice;
        pReso->pciFunc   = pciFunc;

        /*
         * BAR0: [32:17]: memory base
         *       [16:4] : read as "0";
         *       [3]    : 0 - device is not prefetchable
         *       [2:1]  : 00b - 32-bit address space, or
         *                01b - 64-bit address space
         *       [0]    : 0 - memory map decoded
         *
         * BAR1: if BAR0[2:1] == 00b, optional flash memory base
         *       if BAR0[2:1] == 01b, high portion of memory base
         *                            for 64-bit address space
         *
         * BAR2: if BAR0[2:1] == 01b, optional flash memory base
         *       if BAR0[2:1] == 00b, behaves as BAR-1 when BAR-0 is
         *                            a 32-bit value
         */

        pciConfigInLong (pReso->pciBus, pReso->pciDevice, pReso->pciFunc,
                         PCI_CFG_BASE_ADDRESS_0, &bar0);

        pReso->adr64 = ((bar0 & BAR0_64_BIT) == BAR0_64_BIT)? TRUE : FALSE;

        /* If configured, set the PCI Configuration manually */

#if (PCI_CFG_TYPE == PCI_CFG_FORCE)
        {
            pciConfigOutLong (pReso->pciBus, pReso->pciDevice, pReso->pciFunc,
                              PCI_CFG_BASE_ADDRESS_0, pReso->memBaseLow);
            if (pReso->adr64)
                {
                pciConfigOutLong (pReso->pciBus, pReso->pciDevice,
                                  pReso->pciFunc,PCI_CFG_BASE_ADDRESS_1,
                                  pReso->memBaseHigh);

                pciConfigOutLong (pReso->pciBus, pReso->pciDevice,
                                  pReso->pciFunc, PCI_CFG_BASE_ADDRESS_2,
                                  pReso->flashBase);
                }
            else
                {
                pciConfigOutLong (pReso->pciBus, pReso->pciDevice,
                                  pReso->pciFunc, PCI_CFG_BASE_ADDRESS_1,
                                  pReso->flashBase);

                pciConfigOutLong (pReso->pciBus, pReso->pciDevice,
                                  pReso->pciFunc, PCI_CFG_BASE_ADDRESS_2,
                                  pReso->flashBase);
                }

            pciConfigOutByte (pReso->pciBus, pReso->pciDevice, pReso->pciFunc,
                              PCI_CFG_DEV_INT_LINE, pReso->irq);
        }
#endif /* (PCI_CFG_TYPE == PCI_CFG_FORCE) */

        /* read back memory base address and IO base address */

        pciConfigInLong (pReso->pciBus, pReso->pciDevice, pReso->pciFunc,
                         PCI_CFG_BASE_ADDRESS_0, &memBaseLow);
        if (pReso->adr64)
            {
            pciConfigInLong (pReso->pciBus, pReso->pciDevice, pReso->pciFunc,
                             PCI_CFG_BASE_ADDRESS_1, &memBaseHigh);
            pciConfigInLong (pReso->pciBus, pReso->pciDevice, pReso->pciFunc,
                             PCI_CFG_BASE_ADDRESS_2, &flashBase);
            }
        else
            {
            pciConfigInLong (pReso->pciBus, pReso->pciDevice, pReso->pciFunc,
                             PCI_CFG_BASE_ADDRESS_1, &flashBase);
            memBaseHigh = 0x0;
            }

        pciConfigInByte (pReso->pciBus, pReso->pciDevice, pReso->pciFunc,
                         PCI_CFG_DEV_INT_LINE, &irq);

        memBaseLow &= PCI_MEMBASE_MASK;
        flashBase  &= PCI_MEMBASE_MASK;

        /* over write the resource table with read value */

        pReso->memBaseLow   = memBaseLow;
        pReso->memBaseHigh  = memBaseHigh;
        pReso->flashBase    = flashBase;
        pReso->irq          = irq;

        /* enable mapped memory and IO addresses */

        pciConfigOutWord (pReso->pciBus, pReso->pciDevice, pReso->pciFunc,
                          PCI_CFG_COMMAND, PCI_CMD_IO_ENABLE |
                          PCI_CMD_MEM_ENABLE | PCI_CMD_MASTER_ENABLE);

        /* compose loadString in endDevTbl for this unit */

        geiUnits++;
        }

    return OK;
    }

/***************************************************************************
* sysGei8254xEndLoad - create load string and load a gei driver.
*
* This routine will be invoked by the MUX for the purpose of loading an
* gei82543End (gei) device with initial parameters.  This routine is
* constructed as an interface wrapper for the driver load routine.  Thus,
* the arguments and return values are consistent with any xxxEndLoad()
* routine defined for an END driver and the MUX API.
*
* RETURNS: An END object pointer, or NULL on error, or 0 and the name of the
* device if the <pParamStr> was NULL.
*
* ERRNO
*
* SEE ALSO: gei82543EndLoad()
*/

END_OBJ * sysGei8254xEndLoad
    (
    char *    pParamStr,   /* ptr to initialization parameter string */
    void *    unused       /* unused optional argument */
    )
    {
    END_OBJ * pEnd;
    char      paramStr [END_INIT_STR_MAX];

    if (strlen (pParamStr) == 0)
        {
        /* PASS (1)
         * The driver load routine returns the driver name in <pParamStr>.
         */

        pEnd = gei82543EndLoad (pParamStr);
        }
    else
        {
        /* PASS (2)
         * The END <unit> number is prepended to <pParamStr>.  Construct
         * the rest of the driver load string based on physical devices.
         */

        char * holder = NULL;
        int    unit   = atoi (strtok_r (pParamStr, ":", &holder));

        /* is there a PCI resource associated with this END unit ? */

        if (unit >= geiUnits)
            {
            return NULL;
            }

        sprintf (paramStr,
                 "%d:0x%x:0x%x:0x%x:0x%x:0x%x:%d",
                      unit,
                      geiResources [unit].shMemBase, /* share memory base */
                      geiResources [unit].shMemSize, /* share memory size */
                      geiResources [unit].rxDesNum,  /* RX Descriptor Number*/
                      geiResources [unit].txDesNum,  /* TX Descriptor Number*/
                      geiResources [unit].usrFlags,  /* user's flags */
                      GEI_X86_OFFSET_VALUE           /* offset value */
                     );
        if ((pEnd = gei82543EndLoad (paramStr)) == (END_OBJ *)NULL)
            {
            printf ("ERROR: sysGei8254xEndLoad fails to load gei %d\n", unit);
            }
        }

    return (pEnd);
    }

/*****************************************************************************
*
* sys82543BoardInit - Adaptor initialization for 82543 chip
*
* This routine is expected to perform any adapter-specific or target-specific
* initialization that must be done prior to initializing the 82543 chip.
*
* The 82543 driver calls this routine from the driver load routine before
* any other routines.
*
* RETURNS: OK or ERROR
*
* ERRNO
*/

STATUS sys82543BoardInit
    (
    int    unit,                /* unit number */
    ADAPTOR_INFO  *pBoard       /* board information for the GEI driver */
    )
    {
    GEI_RESOURCE *pReso = &geiResources [unit];

    /* sanity check */

    if (unit >= (int)geiUnits)
        return (ERROR);

    if ((pReso->boardType !=  PRO1000_544_BOARD) &&
    (pReso->boardType !=  PRO1000F_BOARD) &&
    (pReso->boardType !=  PRO1000T_BOARD))
           return ERROR;

    /* perform EEPROM checksum */

    if (sys543eepromCheckSum (unit) != OK)
        {
        printf ("GEI82543:unit=%d, EEPROM checksum Error!\n", unit);
        }

    /* get the Ethernet Address from eeprom */

    if (sys543EtherAdrGet (unit) != OK)
        {
        printf ("GEI82543:unit=%d, Invalid Ethernet Address!\n", unit);
        }

    /* get the initialization control word 1 (ICW1) in EEPROM */

    geiResources[unit].eeprom_icw1 = sys543eepromReadWord (unit, EEPROM_ICW1);

    /* get the initialization control word 2 (ICW2) in EEPROM */

    geiResources[unit].eeprom_icw2 = sys543eepromReadWord (unit, EEPROM_ICW2);

    /* initializes the board information structure */

    pBoard->boardType     = pReso->boardType;
    pBoard->vector        = pReso->irq + INT_NUM_IRQ0;

    pBoard->regBaseLow    = pReso->memBaseLow;
    pBoard->regBaseHigh   = pReso->memBaseHigh;
    pBoard->flashBase     = pReso->flashBase;
    pBoard->adr64         = pReso->adr64;

    pBoard->intEnable     = sys543IntEnable;
    pBoard->intDisable    = sys543IntDisable;
    pBoard->intAck        = sys543IntAck;

    if ((pBoard->boardType == PRO1000_544_BOARD) ||
        (pBoard->boardType == PRO1000T_BOARD))
        {
        pBoard->phyType   = GEI_PHY_GMII_TYPE;
        }

    pBoard->phySpecInit   = sys543PhySpecRegsInit;
    pBoard->delayFunc     = (FUNCPTR) sys1000NsDelay;
    pBoard->delayUnit     = 1000;   /* sys1000NsDelay() delays 1000ns */
    pBoard->sysLocalToBus = sys543LocalToPciBusAdrs;
    pBoard->sysBusToLocal = sys543PciBusToLocalAdrs;

    /* specify the interrupt connect/disconnect routines to be used */

    pBoard->intConnect    = (FUNCPTR) intConnect;
    pBoard->intDisConnect = (FUNCPTR) intDisable;

    /* get the ICW1 and ICW2 */

    pBoard->eeprom_icw1   = geiResources[unit].eeprom_icw1;
    pBoard->eeprom_icw2   = geiResources[unit].eeprom_icw2;

    /* copy Ether address */

    memcpy (&pBoard->enetAddr[0], &geiResources[unit].enetAddr[0],
            ETHER_ADDRESS_SIZE);

    /* we finished adaptor initialization */

    pReso->iniStatus = OK;

    return (OK);
    }

/*************************************************************************
*
* sys543eepromReadBits - read bits from EEPROM
*
* This routine reads bit data from the EEPROM
*
* RETURNS: value in WORD size
*
* ERRNO
*/

LOCAL UINT16 sys543eepromReadBits
    (
    int          unit,
    int          bitsNum
    )
    {
    int   ix;
    UINT16   val = 0;

    for (ix = 0; ix < bitsNum; ix++)
        {
        /* raise the clk */

        GEI_SYS_WRITE_REG(unit, INTEL_82543GC_EECD,
                          (EECD_CS_BIT | EECD_SK_BIT));
        /* wait 2000ns */

        sysUsDelay(2);

        val = ( val << 1) |
              ((GEI_SYS_READ_REG(unit, INTEL_82543GC_EECD) & EECD_DO_BIT)? 1 : 0);

        /* lower the clk */

        GEI_SYS_WRITE_REG(unit, INTEL_82543GC_EECD, EECD_CS_BIT);

        /* wait 2000 ns */

        sysUsDelay(2);
        }

    return (val);
    }

/*************************************************************************
*
* sys543eepromWriteBits - write bits out to EEPROM
*
* This routine writes bits out to the EEPROM
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void sys543eepromWriteBits
    (
    int          unit,
    UINT16       value,
    UINT16       bitNum
    )
    {
    volatile UINT16 data;

    if (bitNum == 0)
        return;

    while (bitNum--)
        {
        data = (value & (0x1 << bitNum )) ? EECD_DI_BIT : 0;

        data |=  EECD_CS_BIT;

        /* write the data */

        GEI_SYS_WRITE_REG(unit, INTEL_82543GC_EECD, data);

        /* wait 1000ns */

        sysUsDelay(1);

        /* raise the clk */

        GEI_SYS_WRITE_REG(unit, INTEL_82543GC_EECD, (data | EECD_SK_BIT));

        /* wait 1000ns */

        sysUsDelay(1);

        /* lower the clk */

        GEI_SYS_WRITE_REG(unit, INTEL_82543GC_EECD, data);

        /* wait 1000ns */

        sysUsDelay(1);
        }

    return;
    }

/*************************************************************************
*
* sys543eepromReadWord - Read a word from EEPROM
*
* This routine reads a word from the EEPROM
*
* RETURNS: value in WORD size
*
* ERRNO
*/

LOCAL UINT16 sys543eepromReadWord
    (
    int          unit,
    UINT32       index
    )
    {
    UINT16 val;
    UINT32 tmp;

    if (index >= EEPROM_WORD_SIZE)
        {
        printf ("Invalid index:%d to EEPROM\n", index);
        return 0;
        }

    tmp = GEI_SYS_READ_REG(unit, INTEL_82543GC_EECD);

    GEI_SYS_WRITE_REG(unit, INTEL_82543GC_EECD, EECD_CS_BIT);

    /* wait 1000ns */

    sysUsDelay(1);

    /* write the opcode out */

    sys543eepromWriteBits (unit, EEPROM_READ_OPCODE, EEPROM_CMD_BITS);

    /* write the index out */

    sys543eepromWriteBits (unit, index, EEPROM_INDEX_BITS);

    tmp = GEI_SYS_READ_REG(unit, INTEL_82543GC_EECD);

    /* read the data */

    val = sys543eepromReadBits (unit, EEPROM_DATA_BITS);

    /* clean up access to EEPROM */

    tmp &= ~(EECD_DI_BIT | EECD_DO_BIT | EECD_CS_BIT);

    GEI_SYS_WRITE_REG(unit, INTEL_82543GC_EECD, tmp);

    return val;
    }

/*************************************************************************
*
* sys543EtherAdrGet - Get Ethernet address from EEPROM
*
* This routine gets an Ethernet address from the EEPROM.
*
* RETURNS: OK or ERROR
*
* ERRNO
*/

LOCAL STATUS sys543EtherAdrGet
    (
    int unit
    )
    {
    UINT32 ix, count = 0 ;
    UINT16 val;
    UCHAR  adr [ETHER_ADDRESS_SIZE];

    for (ix = 0; ix < ETHER_ADDRESS_SIZE / sizeof(UINT16); ix++)
        {
        /* get word i from EEPROM */

        val = sys543eepromReadWord (unit, (UINT16)(EEPROM_IA_ADDRESS + ix));

        adr [count++] = (UCHAR)val;

        adr [count++] = (UCHAR) (val >> 8);
        }

    /* check IA is UCAST  */

    if (adr[0] & 0x1)
        return (ERROR);

    memcpy (&geiResources[unit].enetAddr[0], adr, ETHER_ADDRESS_SIZE);

    return (OK);
    }

/**************************************************************************
*
* sys543eepromCheckSum - calculate checksum
*
* This routine performs the EEPROM checksum calculation.
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL STATUS sys543eepromCheckSum
    (
    int unit
    )
    {
    UINT16 checkSum = 0 ;
    UINT32 ix;

    for (ix = 0; ix < EEPROM_WORD_SIZE; ix++)
        checkSum += sys543eepromReadWord (unit, ix);

    if (checkSum == (UINT16)EEPROM_SUM)
        return OK;

    return ERROR;
    }

/**************************************************************************
*
* sys543PhySpecRegsInit - Initialize PHY specific registers
*
* This routine initializes the PHY specific registers.
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void sys543PhySpecRegsInit
    (
    PHY_INFO * pPhyInfo,    /* PHY's info structure pointer */
    UINT8     phyAddr       /* PHY's bus number */
    )
    {
    UINT16 regVal;          /* register value */
    UINT16 phyId1;          /* phy Id 1 */
    UINT16 phyId2;          /* phy ID 2 */
    UINT32 retVal;          /* return value */
    UINT32 phyOui = 0;      /* PHY's manufacture ID */
    UINT32 phyMode;         /* PHY mode number */

    /* Intel Pro1000T adaptor uses Alaska transceiver */

    /* read device ID to check Alaska chip available */

    MII_READ (phyAddr, MII_PHY_ID1_REG, &phyId1, retVal);

    MII_READ (phyAddr, MII_PHY_ID2_REG, &phyId2, retVal);

    phyOui =  phyId1 << 6 | phyId2 >> 10;

    phyMode = (phyId2 & MII_ID2_MODE_MASK) >> 4;

    if (phyOui == MARVELL_OUI_ID && (phyMode == MARVELL_ALASKA_88E1000 ||
                                     phyMode == MARVELL_ALASKA_88E1000S))
        {
         /* This is actually a Marvell Alaska 1000T transceiver */

         /* disable PHY's interrupt */

         MII_READ (phyAddr, ALASKA_INT_ENABLE_REG, &regVal, retVal);
         regVal = 0;
         MII_WRITE (phyAddr, ALASKA_INT_ENABLE_REG, regVal, retVal);

         /* CRS assert on transmit */

         MII_READ (phyAddr, ALASKA_PHY_SPEC_CTRL_REG, &regVal, retVal);
         regVal |= ALASKA_PSCR_ASSERT_CRS_ON_TX;
         MII_WRITE (phyAddr, ALASKA_PHY_SPEC_CTRL_REG, regVal, retVal);

        /* set the clock rate when operate in 1000T mode */

         MII_READ (phyAddr, ALASKA_EXT_PHY_SPEC_CTRL_REG, &regVal, retVal);
         regVal |= ALASKA_EPSCR_TX_CLK_25;
         MII_WRITE (phyAddr, ALASKA_EXT_PHY_SPEC_CTRL_REG, regVal, retVal);
        }

     /* other PHYS .... */

     return;
     }

/*****************************************************************************
*
* sys543IntAck - acknowledge an 82543 interrupt
*
* This routine performs any 82543 interrupt acknowledge that may be
* required.  This typically involves an operation to some interrupt
* control hardware.
*
* This routine gets called from the 82543 driver's interrupt handler.
*
* This routine assumes that the PCI configuration information has already
* been setup.
*
* RETURNS: OK, or ERROR if the interrupt could not be acknowledged.
*
* ERRNO
*/

LOCAL STATUS sys543IntAck
    (
    int    unit        /* unit number */
    )
    {
    GEI_RESOURCE *pReso = &geiResources [unit];

    switch (pReso->boardType)
    {
    case PRO1000_544_BOARD:
    case TYPE_PRO1000F_PCI:        /* handle PRO1000F/T LAN Adapter */
    case TYPE_PRO1000T_PCI:
        /* no addition work necessary for the PRO1000F/T */
        break;

    default:
        return (ERROR);
    }

    return (OK);
    }

/******************************************************************************
*
* sys543LocalToPciBusAdrs - convert a local address to a bus address
*
* This routine returns a PCIbus address for the LOCAL bus address.
*
* RETURNS: PCIbus address
*
* ERRNO
*/

LOCAL UINT32 sys543LocalToPciBusAdrs
    (
    int unit ARG_UNUSED,
    UINT32      adrs    /* Local Address */
    )
    {
    UINT32 busAdrs; /* Holds converted address */
    char * tmp = (char *)&busAdrs;

    if (sysLocalToBusAdrs(PCI_SPACE_MEM_PRI, (char *) adrs,
                          (char **)tmp) != OK)

        logMsg("ERROR:sys543LocalToPciBusAdrs: Local Address 0x%x is outside of PCI DRAM WINDOW\n",
               (int)adrs,0,0,0,0,0);
    return(busAdrs);
    }

/******************************************************************************
*
* sys543PciBusToLocalAdrs - convert a bus address to a local address
*
* This routine returns a local address that is used to access the PCIbus.
* The bus address that is passed into this routine is the PCIbus address
* as it would be seen on the local bus.
*
* RETURNS: local bus address
*
* ERRNO
*/

LOCAL UINT32 sys543PciBusToLocalAdrs
    (
    int unit ARG_UNUSED,
    UINT32      adrs    /* PCI Address */
    )
    {
    volatile UINT32 localAdrs;  /* Holds converted address */
    char * tmp = (char *)&localAdrs;

    if (sysBusToLocalAdrs(PCI_SPACE_MEM_PRI, (char *) adrs,
                          (char **)tmp) != OK)
        logMsg("ERROR:sys543PciBusToLocalAdrs: Address 0x%x is outside of PCI DRAM WINDOW\n",
               (int)localAdrs,0,0,0,0,0);

    return(localAdrs);
    }

/*****************************************************************************
*
* sys543IntEnable - enable 82543 interrupts
*
* This routine enables 82543 interrupts.  This may involve operations on
* interrupt control hardware.
*
* The 82543 driver calls this routine throughout normal operation to terminate
* critical sections of code.
*
* This routine assumes that the PCI configuration information has already
* been setup.
*
* RETURNS: OK, or ERROR if interrupts could not be enabled.
*
* ERRNO
*/

LOCAL STATUS sys543IntEnable
    (
    int    unit ARG_UNUSED       /* unit number */
    )
    {
    return (intEnable(geiResources[unit].irq));
    }

/*****************************************************************************
*
* sys543IntDisable - disable 82543 interrupts
*
* This routine disables 82543 interrupts.  This may involve operations on
* interrupt control hardware.
*
* The 82543 driver calls this routine throughout normal operation to enter
* critical sections of code.
*
* This routine assumes that the PCI configuration information has already
* been setup.
*
* RETURNS: OK, or ERROR if interrupts could not be disabled.
*
* ERRNO
*/

LOCAL STATUS sys543IntDisable
    (
    int    unit ARG_UNUSED       /* unit number */
    )
    {
    return (intDisable(geiResources[unit].irq));
    }

/*****************************************************************************
*
* sys543Show - shows 82543 configuration
*
* This routine shows (Intel Pro 1000F/T) adapters configuration
*
* RETURNS: N/A
*
* ERRNO
*/

void sys543Show
    (
    UINT32 unit        /* unit number */
    )
    {
    int ix;
    GEI_RESOURCE *pReso = &geiResources [unit];

    if (unit >= geiUnits)
        {
        printf ("invalid unit number: %d\n", unit);
        return;
        }

    if (pReso->boardType == PRO1000_544_BOARD)
        printf ("********* Intel PRO1000 F Adapter ***********\n");

    else if (pReso->boardType == PRO1000F_BOARD)
        printf ("********* Intel PRO1000 F Adapter ***********\n");

    else if (pReso->boardType == PRO1000T_BOARD)
        printf ("********* Intel PRO1000 T Adapter ***********\n");

    else
        printf ("********* UNKNOWN Adaptor ************ \n");

    printf ("  CSR PCI Membase address = 0x%x\n", pReso->memBaseLow);

    printf ("  Flash PCI Membase address = 0x%x\n", pReso->flashBase);

    printf ("  PCI bus no.= 0x%x, device no.= 0x%x, function no.= 0x%x\n",
            pReso->pciBus, pReso->pciDevice, pReso->pciFunc);

    printf ("  IRQ = %d\n", pReso->irq);

    if (pReso->iniStatus == ERROR)
        return;

    printf ("  Adapter Ethernet Address");

    for (ix = 0; ix < 6; ix ++)
        printf (":%2.2X", (UINT32)pReso->enetAddr[ix]);

    printf ("\n");

    printf ("  EEPROM Initialization Control Word 1 = 0x%4.4X\n",
            pReso->eeprom_icw1);

    printf ("  EEPROM Initialization Control Word 2 = 0x%4.4X\n",
            pReso->eeprom_icw2);

    printf ("*********************************************\n");

    return;
    }

/*************************************************************************
*
* sys543eepromWriteWord - Write a word to EEPROM
*
* This routine writes a word to the EEPROM.
*
* RETURNS: value in WORD size
*
* ERRNO
*/

UINT16 sys543eepromWriteWord
    (
    int          unit,      /* device unit to be accessed. */
    UINT32       index,     /* index in EEPROM address range. */
    UINT16       value      /* value to write. */
    )
    {
    UINT32 tmp;
    UINT32 timeout = 10000;

    if (index >= EEPROM_WORD_SIZE)
        {
        printf ("Invalid index:%d to EEPROM\n", index);
        return 0;
        }

    tmp = GEI_SYS_READ_REG(unit, INTEL_82543GC_EECD);

    /* Enable writing */

    GEI_SYS_WRITE_REG(unit, INTEL_82543GC_EECD, EECD_CS_BIT);

    /* wait 1000ns */

    sysUsDelay(1);

    /* write the write enable opcode out */

    sys543eepromWriteBits (unit, EEPROM_WRITE_EN_OPCODE, EEPROM_ENABLE_BITS);

    /* clean up access to EEPROM and disable chip select */

    tmp &= ~(EECD_DI_BIT | EECD_DO_BIT | EECD_CS_BIT);

    GEI_SYS_WRITE_REG(unit, INTEL_82543GC_EECD, tmp);

    /* wait 1000ns */

    sysUsDelay(1);

    /* write the data out, enable the chip select */

    GEI_SYS_WRITE_REG(unit, INTEL_82543GC_EECD, EECD_CS_BIT);

    /* wait 1000ns */

    sysUsDelay(1);

    /* write the opcode out */

    sys543eepromWriteBits (unit, EEPROM_WRITE_OPCODE, EEPROM_CMD_BITS);

    /* write the index out */

    sys543eepromWriteBits (unit, index, EEPROM_INDEX_BITS);

    /* write the data */

    sys543eepromWriteBits (unit, value, EEPROM_DATA_BITS);

    /* clean up access to EEPROM and disable chip select */

    tmp &= ~(EECD_DI_BIT | EECD_DO_BIT | EECD_CS_BIT);

    GEI_SYS_WRITE_REG(unit, INTEL_82543GC_EECD, tmp);

    /* wait 1000ns */

    sysUsDelay(1);

    /* Complete write cycle */

    GEI_SYS_WRITE_REG(unit, INTEL_82543GC_EECD, EECD_CS_BIT);

    /* wait 10ms for write cycle */

    sysUsDelay(10000);

    while(!((tmp = GEI_SYS_READ_REG(unit, INTEL_82543GC_EECD)) & EECD_DO_BIT)
          && (timeout-- > 0))
    sysUsDelay(1);

    /* clean up access to EEPROM and disable chip select */

    tmp &= ~(EECD_DI_BIT | EECD_DO_BIT | EECD_CS_BIT);

    GEI_SYS_WRITE_REG(unit, INTEL_82543GC_EECD, tmp);

    /* Disable writing */

    GEI_SYS_WRITE_REG(unit, INTEL_82543GC_EECD, EECD_CS_BIT);

    /* wait 1000ns */

    sysUsDelay(1);

    /* write the write enable opcode out */

    sys543eepromWriteBits (unit, EEPROM_WRITE_DIS_OPCODE, EEPROM_ENABLE_BITS);

    /* clean up access to EEPROM and disable chip select */

    tmp &= ~(EECD_DI_BIT | EECD_DO_BIT | EECD_CS_BIT);

    GEI_SYS_WRITE_REG(unit, INTEL_82543GC_EECD, tmp);

    return (OK);
    }

/*************************************************************************
*
* sys543EepromDump - Dump contents of EEPROM
*
* This routine dumps the 82543 EEPROM
*
* RETURNS: OK or ERROR
*
* ERRNO
*/

STATUS sys543EepromDump
    (
    int unit,           /* device unit to be accessed. */
    char *adr           /* address of buffer to store data. */
    )
    {
    UINT32 ix;
    UINT32 count = 0;
    UINT32 checkSum = 0;
    UINT16 val;

    printf("sys543EepromDump:\n\r");

    for (ix = 0; ix < EEPROM_WORD_SIZE; ix++)
        {
        /* get word i from EEPROM */

        val = sys543eepromReadWord (unit, (UINT16)(EEPROM_IA_ADDRESS + ix));
        checkSum += val;

        adr [count++] = (UCHAR)val;

        adr [count++] = (UCHAR) (val >> 8);
        }

    for (ix = 0; ix < EEPROM_WORD_SIZE * 2; ix++)
        {
        printf("%x: 0x%x\n\r", ix, adr[ix]);
        }

    printf("sys543EepromDump: checksum: 0x%x\n\r", checkSum);

    return (OK);
    }

/*************************************************************************
*
* sys1000NsDelay - Call sysUsDelay with a value of 1000 (ns).
*
* This routine delays for 1000 ns.
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void sys1000NsDelay (void)
    {
    sysUsDelay (1);
    return;
    }

/*************************************************************************
*
* geiSysWriteReg - Write to device register.
*
* Given bus address of device register this function writes a 32-bit
* value to that register.
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void geiSysWriteReg
    (
    int unit  ARG_UNUSED,
    UINT32 reg,
    UINT32 value
    )
    {
    UINT32 addr;
    char * tmp = (char *)&addr;

    sysBusToLocalAdrs(PCI_SPACE_MEM_PRI,
              (char *)(reg + geiResources[(unit)].memBaseLow),
              (char **)tmp);
#if (_BYTE_ORDER == _BIG_ENDIAN)
    *(volatile UINT32 *)addr = LONGSWAP(value);
#else
    *(volatile UINT32 *)addr = (value);
#endif
    }

/*************************************************************************
*
* geiSysWriteReg - Read from device register.
*
* Given bus address of device register this function reads a 32-bit
* value from that register.
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL UINT32 geiSysReadReg
    (
    int unit  ARG_UNUSED,
    UINT32 reg
    )
    {
    UINT32 val;
    UINT32 addr;
    char * tmp = (char *)&addr;

    sysBusToLocalAdrs(PCI_SPACE_MEM_PRI,
              (char *)(reg + geiResources[(unit)].memBaseLow),
              (char **)tmp);
#if (_BYTE_ORDER == _BIG_ENDIAN)
    val = LONGSWAP(*(UINT32 *)addr);
#else
    val = *(UINT32 *)addr;
#endif
    return (val);
    }
