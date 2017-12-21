/* vxbDtsecMdio.h - header file for dTSEC MDIO driver */

/*
 * Copyright (c) 2009, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,16mar11,b_m  Add BSP hook to enable mdio mux.
01c,24sep09,wap  Set up GPIO support for FSL P4080DS board
01b,09sep09,wap  Add register definitions
01a,28jul09,wap  written
*/

#ifndef __INCvxbDtsecMdioh
#define __INCvxbDtsecMdioh

#ifdef __cplusplus
extern "C" {
#endif

IMPORT void dtmRegister (void);

#define DTM_TIMEOUT 10000

#define DTM_MIIMCFG           0x0120  /* MII mgmt cfg */
#define DTM_MIIMCOM           0x0124  /* MII mgmt command */
#define DTM_MIIMADD           0x0128  /* MII mgmt address */
#define DTM_MIIMCON           0x012C  /* MII mgmt control */
#define DTM_MIIMSTAT          0x0130  /* MII mgmt status */
#define DTM_MIIMIND           0x0134  /* MII mgmt indication */

/* MII management configuration register */

#define DTM_MIIMCFG_RESET             0x80000000
#define DTM_MIIMCFG_NO_PRE            0x00000010
#define DTM_MIIMCFG_MGMT_CLK          0x0000000F

#define DTM_MIIMCFG_MCS_2             0x00000000
#define DTM_MIIMCFG_MCS_4             0x00000001
#define DTM_MIIMCFG_MCS_6             0x00000002
#define DTM_MIIMCFG_MCS_8             0x00000003
#define DTM_MIIMCFG_MCS_10            0x00000004
#define DTM_MIIMCFG_MCS_14            0x00000005
#define DTM_MIIMCFG_MCS_20            0x00000006
#define DTM_MIIMCFG_MCS_28            0x00000007

/* MII management command register */

#define DTM_MIIMCOM_SCAN              0x00000002
#define DTM_MIIMCOM_READ              0x00000001

/* MII management address register */

#define DTM_MIIMADD_PHYADDR           0x0000001F
#define DTM_MIIMADD_REGADDR           0x00001F00

/* MII management control register */

#define DTM_MIIMCON_WRDATA            0x0000FFFF

/* MII management status register */

#define DTM_MIIMSTAT_RDDATA           0x0000FFFF

/* MII management indicator register */

#define DTM_MIIMIND_NOT_VALID         0x00000004
#define DTM_MIIMIND_SCAN              0x00000002
#define DTM_MIIMIND_BUSY              0x00000001

/* Private instance context */

typedef struct dtm_drv_ctrl
    {
    VXB_DEVICE_ID	dtmDev;
    SEM_ID		dtmMiiSem;
    UINT32 *		dtmShared;
    UINT32		dtmDummy;
    VXB_DEVICE_ID	dtmGpioDev;
    UINT8		dtmFm0Gpio;
    UINT8		dtmFm1Gpio;
    FUNCPTR		dtmMuxEnable;
    } TM_DRV_CTRL;

#undef CSR_READ_4
#undef CSR_WRITE_4

#define CSR_READ_4(pDev, addr)                                  \
        *(volatile UINT32 *)((UINT32)pDev->pRegBase[0] + addr)

#define CSR_WRITE_4(pDev, addr, data)                           \
        do {                                                    \
            volatile UINT32 *pReg =                             \
                (UINT32 *)((UINT32)pDev->pRegBase[0] + addr);   \
            *(pReg) = (UINT32)(data);                           \
            WRS_ASM("eieio");                                   \
        } while ((0))

#define CSR_SETBIT_4(pDev, offset, val)          \
        CSR_WRITE_4(pDev, offset, CSR_READ_4(pDev, offset) | (val))

#define CSR_CLRBIT_4(pDev, offset, val)          \
        CSR_WRITE_4(pDev, offset, CSR_READ_4(pDev, offset) & ~(val))


#ifdef __cplusplus
}
#endif

#endif /* __INCvxbDtsecMdioh */
