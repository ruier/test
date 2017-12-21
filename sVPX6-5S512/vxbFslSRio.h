/* vxbFslSRio.h - FreeScale Serial RapdIO Controller header file */

/*
 * Copyright (c) 2012, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
DESCRIPTION
This file contains the definitions for FreeScale SRIO module.

*/

/*
modification history
--------------------
01c,15may13,j_z  add slave share memory support. (WIND00418128)
01b,18mar13,j_z  add T4240 support.
01a,03sep12,j_z  written
*/

#ifndef __INCvxbFslSRioH
#define __INCvxbFslSRioH

#ifdef __cplusplus
extern "C" {
#endif

#include <../src/hwif/h/vxbus/vxbRapidIO.h>

#define FSL_RIO_HOST          "fslSRio"

#define FSL_SRIO_IPBRR1        0x10bf8
#define FSL_SRIO_IPBRR2        0x10bfc

#define TXXX_SRIO_IP           0x01c00200

/* RapidIO outbound register */

/* RapidIO outbound window base address register */

#define RIO_ROWBARn(port, win) (0x10C08 + (port) * 0x200 + ((win) * 0x20))

/* RapidIO outbound window attributes register */

#define RIO_ROWARn(port, win) (0x10C10 + (port) * 0x200 + ((win) * 0x20))

/* RapidIO outbound window translation address register */

#define RIO_ROWTARn(port, win) (0x10C00 + (port) * 0x200 + ((win) * 0x20))


/* RapidIO inbound register */

/* RapidIO inbound window attributes register */

#define RIO_RIWBARn(port, win) (0x10D68 + (port) * 0x200 + (0x80 - ((win) * 0x20)))

/* RapidIO inbound window translation address register */

#define RIO_RIWTARn(port, win) (0x10D60 + (port) * 0x200 + (0x80 - ((win) * 0x20)))

/* RapidIO inbound window attributes register */

#define RIO_RIWARn(port, win)  (0x10D70 + (port) * 0x200 + (0x80 - ((win) * 0x20)))

/* Port n Pass-Through/Accept-All Configuration  */

#define PnPTAACR(n) (0x10120 + ((n) * 0x80))
#define PnPTAACR_ACC_ALL                0x00000001 /* accept all packet */

#define RIO_WIN_ENABLE                  0x80000000
#define RIO_WIN_PROTECT                 0x40000000
#define RIO_WIN_PCI_ORDER               0x02000000
#define RIO_WIN_LOCAL_MEM_TGTIF         0xF
#define RIO_WIN_LOCAL_MEM_TGTIF_SHIFT   20
#define RIO_WIN_ADDR_4KALIGN            12

#define RIO_RxWAR_EN                    0x80000000
#define RIO_RIWAR_TGINT_MSK             0xf
#define RIO_RIWAR_PCI                   0x00
#define RIO_RIWAR_PCI2                  0x01
#define RIO_RIWAR_PCIEX                 0x02

#define RIO_RxWAR_TYPE_MSK              0xf
#define RIO_RxWAR_TYPE_READ(val)        (val << 16)
#define RIO_RxWAR_TYPE_WRITE(val)       (val << 12)

#define RIO_RIWAR_IO_TYPE_R             4
#define RIO_RIWAR_IO_TYPE_W             4

#define RIO_RIWAR_LOCAL_R_NO_SNOOP      4
#define RIO_RIWAR_LOCAL_R_SNOOP_CORE    5
#define RIO_RIWAR_LOCAL_R_UNLOCK_L2     7
#define RIO_RIWAR_LOCAL_R_ENHANCE_SNOOP 13

#define RIO_RIWAR_LOCAL_W_NO_SNOOP      4
#define RIO_RIWAR_LOCAL_W_SNOOP_CORE    5
#define RIO_RIWAR_LOCAL_W_ALLOC_L2      6
#define RIO_RIWAR_LOCAL_W_LOCK_L2       7
#define RIO_RIWAR_LOCAL_W_ENHANCE_SNOOP 13

#define RIO_ROWAR_RDTYPE_IO_READ_HOME   0x2
#define RIO_ROWAR_RDTYPE_NREAD          0x4
#define RIO_ROWAR_RDTYPE_MAINT_READ     0x7
#define RIO_ROWAR_RDTYPE_ATOMIC_INC     0xc
#define RIO_ROWAR_RDTYPE_ATOMIC_DEC     0xd
#define RIO_ROWAR_RDTYPE_ATOMIC_SET     0xe
#define RIO_ROWAR_RDTYPE_ATOMIC_CLR     0xf

#define RIO_ROWAR_WRTYPE_FLUSH          0x1
#define RIO_ROWAR_WRTYPE_SWRITE         0x3
#define RIO_ROWAR_WRTYPE_NWRITE         0x4
#define RIO_ROWAR_WRTYPE_NWRITE_R       0x5
#define RIO_ROWAR_WRTYPE_MAINT_WRITE    0x7

#define RIO_RxWAR_SIZE_4K               11
#define RIO_RxWAR_SIZE_8K               12
#define RIO_RxWAR_SIZE_1MB              19
#define RIO_RxWAR_SIZE_2MB              20
#define RIO_RxWAR_SIZE_4MB              21
#define RIO_RxWAR_SIZE_8MB              22
#define RIO_RxWAR_SIZE_16MB             23
#define RIO_RxWAR_SIZE_32MB             24
#define RIO_RxWAR_SIZE_64MB             25
#define RIO_RxWAR_SIZE_128MB            26
#define RIO_RxWAR_SIZE_256MB            27
#define RIO_RxWAR_SIZE_512MB            28

#define RIO_MAINT_WIN_SIZE              RIO_RxWAR_SIZE_4MB

#define RIO_85XX_LAWAR_TGTIF             0x00c00000

/* out channel */

#define RIO_MAINT_CHANNEL                1 
#define RIO_HOST_SM_OUT_CHANNEL          2

/* in channel */

#define RIO_HOST_SM_IN_CHANNEL           1

/* out channel */

#define RIO_SLAVE_SM_OUT_CHANNEL         1
#define RIO_SLAVE_TAS_SET_OUT_CHANNEL    2
#define RIO_SLAVE_TAS_CLEAR_OUT_CHANNEL  3

/* in channel */

#define RIO_SLAVE_SM_IN_CHANNEL          1

typedef struct rio_drv_ctrl
    {
    VXB_DEVICE_ID       pDev;
    void *              riohandle;
    void *              rioBar;
    void *              rioDevBase;
    UINT32              rioLawSize;
    SEM_ID              rioSem;
    UINT32              rioHost;
    UINT32              rioSmWinSize;
    void *              rioSmHostBase;
    void *              rioSmSlaveBase;
    void *              rioSmBusBase;
    UINT32              rioBusSize;
    UINT32              rioPortNum;
    BOOL                rioMaintInit;
    UINT32              rioAutoCfg;
    UINT32              rioForceDevId;
    BOOL                rioSlvNoWait;
    } RIO_DRV_CTRL;

#define RIO_BAR(p)      ((RIO_DRV_CTRL *)(p)->pDrvCtrl)->rioBar
#define RIO_HANDLE(p)   ((RIO_DRV_CTRL *)(p)->pDrvCtrl)->riohandle

#undef CSR_READ_4
#define CSR_READ_4(pDev, addr)                                  \
    vxbRead32 (RIO_HANDLE(pDev), (UINT32 *)((UINT32)RIO_BAR(pDev)+((UINT32)(addr))))

#undef CSR_WRITE_4
#define CSR_WRITE_4(pDev, addr, data) \
    vxbWrite32 (RIO_HANDLE(pDev),                             \
        (UINT32 *)((UINT32)RIO_BAR(pDev)+((UINT32)(addr))), data)

#define CSR_SETBIT_4(pDev, offset, val) \
    CSR_WRITE_4(pDev, offset, CSR_READ_4(pDev, offset) | (val))

#define CSR_CLRBIT_4(pDev, offset, val) \
    CSR_WRITE_4(pDev, offset, CSR_READ_4(pDev, offset) & ~(val))

IMPORT STATUS fslRioUsrIntGen (int, int, int); /* SM_NET user int gen smUtilUser1Rtn */

#ifdef __cplusplus
}
#endif

#endif /* __INCvxbFslSRioH */

