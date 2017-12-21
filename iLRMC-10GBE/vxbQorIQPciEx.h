/* vxbQorIQPciEx.h - header file for Freescale QorIQ VxBus PCI Express driver */

/*
 * Copyright (c) 2009, 2011-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,18oct12,b_m  add QorIQ T4 support.
01c,12oct11,fao  fix PEX_CCSR_MASK value.
01b,08aug11,b_m  add ccsrBar to PEX_DRV_CTRL.
01a,25sep09,wap  written
*/

#ifndef __INCvxbQorIQPciExh
#define __INCvxbQorIQPciExh

#ifdef __cplusplus
extern "C" {
#endif


#define PEX_CFG_ADDR		0x000	/* PCI config address */
#define PEX_CFG_DATA		0x004	/* PCI config data */
#define PEX_OTBC_TMO		0x00C	/* Outbound completion timeout */
#define PEX_CRTY_TMO		0x010	/* Configuration retry timeout */
#define PEX_CFG			0x014	/* Configuration register */
#define PEX_PMEMES_DR		0x020	/* PME & message direct */
#define PEX_PMEMES_IDR		0x024	/* Interrupt disable */
#define PEX_PMEMES_IER		0x028	/* Interrupt enable */
#define PEX_PMCR		0x02C	/* Power management command */

/* Config addr register */

#define PEX_CFGADDR_EN		0x80000000 /* Enable */
#define PEX_CFGADDR_XREGN	0x0F000000 /* PCIe extended reg space */
#define PEX_CFGADDR_BUSN	0x00FF0000 /* Bus number */
#define PEX_CFGADDR_DEVN	0x0000F800 /* Device number */
#define PEX_CFGADDR_FUNCN	0x00000700 /* Function number */
#define PEX_CFGADDR_REGN	0x000000FC /* Register number */

#define PEX_XREGN(x)		(((x) << 16) & PEX_CFGADDR_XREGN)
#define PEX_BUSN(x)		(((x) << 16) & PEX_CFGADDR_BUSN)
#define PEX_DEVN(x)		(((x) << 11) & PEX_CFGADDR_DEVN)
#define PEX_FUNCN(x)		(((x) << 8) & PEX_CFGADDR_FUNCN)
#define PEX_REGN(x)		((x) & PEX_CFGADDR_REGN)

/* Outbound completion timeout register */

#define PEX_OTBCTMO_TD		0x80000000 /* disable timeout */
#define PEX_OTBCTMO_TC		0x03FFFFFF /* timeout counter */

/* Configuration retry timeout */

#define PEX_CRTYTMO_RD		0x80000000 /* disable retry */
#define PEX_CRTYTMO_TC		0x0FFFFFFF /* timeout counter */

/* IP block revision */

#define PEX_IBLK_1		0xBF8	/* IP block revision 1 */
#define PEX_IBLK_2		0xBFC	/* IP block revision 2 */

#define PEX_IBLK_1_REV_MASK	0xFFFF
#define PEX_IBLK_1_REV_30	0x0300

/* Configuration register */

#define PEX_CFG_OBCK		0x00002000 /* outb. addr check enable */
#define PEX_CFG_SAC		0x00000010 /* Sense ASPM control */
#define PEX_CFG_SP		0x00000002 /* Slot present */
#define PEX_CFG_SCC		0x00000001 /* Slot clock config */

/* Outbound windows */

#define PEX_OTAR0		0xC00	/* Outbound xlate addr 0 */
#define PEX_OTEAR0		0xC04	/* Outbd. extended xlate addr 0 */
#define PEX_OWAR0		0xC10	/* Outbd. attributes 0 */
#define PEX_OTAR1		0xC20	/* Outbound xlate addr 1 */
#define PEX_OTEAR1		0xC24	/* Outbd. extended xlate addr 1 */
#define PEX_OWBAR1		0xC28	/* Outbd. base addr 1 */
#define PEX_OWAR1		0xC30	/* Outbd. attributes 1 */
#define PEX_OTAR2		0xC40	/* Outbound xlate addr 2 */
#define PEX_OTEAR2		0xC44	/* Outbd. extended xlate addr 2 */
#define PEX_OWBAR2		0xC48	/* Outbd. base addr 2 */
#define PEX_OWAR2		0xC50	/* Outbd. attributes 2 */
#define PEX_OTAR3		0xC60	/* Outbound xlate addr 3 */
#define PEX_OTEAR3		0xC64	/* Outbd. extended xlate addr 3 */
#define PEX_OWBAR3		0xC68	/* Outbd. base addr 3 */
#define PEX_OWAR3		0xC70	/* Outbd. attributes 3 */
#define PEX_OTAR4		0xC80	/* Outbound xlate addr 4 */
#define PEX_OTEAR4		0xC84	/* Outbd. extended xlate addr 4 */
#define PEX_OWBAR4		0xC88	/* Outbd. base addr 4 */
#define PEX_OWAR4		0xC90	/* Outbd. attributes 4 */

/* Outbound window translation address */

#define PEX_OTAR_TEA		0xFFF00000 /* Xlate extended addr */
#define PEX_OTAR_TA		0x000FFFFF /* Xlate address */

/* Outbound window extended translation address */

#define PEX_OTEAR_TEA		0x000FFFFF /* Xlate extended addr */

/* Outbound window base address */

#define PEX_OWBAR_WBEA		0x00F00000 /* Window extended addr */
#define PEX_OWBAR_BA		0x000FFFFF /* Window base address */

#define PEX_ADDR(x)		(((x) >> 12) & PEX_OWBAR_BA)

/* Outbound window attributes */

#define PEX_OTAR_EN		0x80000000 /* window enable */
#define PEX_OTAR_DIEN		0x40000000 /* invariance enable */
#define PEX_OTAR_ROE		0x10000000 /* relaxed ordering enable */
#define PEX_OTAR_NS		0x08000000 /* no snoop enable */
#define PEX_OTAR_TC		0x00E00000 /* traffic class */
#define PEX_OTAR_RTT		0x000F0000 /* read transaction type */
#define PEX_OTAR_WTT		0x0000F000 /* write transaction type */
#define PEX_OTAR_OWS		0x0000003F /* outbound window size */

#define PEX_TC(x)		(((x) << 17) & PEX_OTAR_TC)
#define PEX_RTT(x)		(((x) << 16) & PEX_OTAR_RTT)
#define PEX_WTT(x)		(((x) << 12) & PEX_OTAR_WTT)

#define PEX_RTT_CONFIG		0x2
#define PEX_RTT_MEM		0x4
#define PEX_RTT_MEM_SNOOP	0x5
#define PEX_RTT_IO		0x8

#define PEX_WTT_CONFIG		0x2
#define PEX_WTT_MEM		0x4
#define PEX_WTT_MEM_SNOOP	0x5
#define PEX_WTT_MESSAGE		0x5
#define PEX_WTT_IO		0x8

#define PEX_WSIZE_4KB		0x0000000B
#define PEX_WSIZE_8KB		0x0000000C
#define PEX_WSIZE_16KB		0x0000000D
#define PEX_WSIZE_32KB		0x0000000E
#define PEX_WSIZE_64KB		0x0000000F
#define PEX_WSIZE_128KB		0x00000010
#define PEX_WSIZE_256KB		0x00000011
#define PEX_WSIZE_512KB		0x00000012
#define PEX_WSIZE_1MB		0x00000013
#define PEX_WSIZE_2MB		0x00000014
#define PEX_WSIZE_4MB		0x00000015
#define PEX_WSIZE_8MB		0x00000016
#define PEX_WSIZE_16MB		0x00000017
#define PEX_WSIZE_32MB		0x00000018
#define PEX_WSIZE_64MB		0x00000019
#define PEX_WSIZE_128MB		0x0000001A
#define PEX_WSIZE_256MB		0x0000001B
#define PEX_WSIZE_512MB		0x0000001C
#define PEX_WSIZE_1GB		0x0000001D
#define PEX_WSIZE_2GB		0x0000001E
#define PEX_WSIZE_4GB		0x0000001F
#define PEX_WSIZE_8GB		0x00000020
#define PEX_WSIZE_16GB		0x00000021
#define PEX_WSIZE_32GB		0x00000022
#define PEX_WSIZE_64GB		0x00000023
#define PEX_WSIZE_128GB		0x00000024

/* MSI inbound window */

#define PEX_MSIITAR		0xD00	/* MSI inb. xlate addr */
#define PEX_MSIIWBAR		0xD08	/* MSI inb. window base */
#define PEX_MSIIWBEAR		0xD0C	/* MSI inb. window base ext. */
#define PEX_MSIIWAR		0xD10	/* MSI inb. window attrs */

#define PEX_CCSR_MASK		0x000FFFFF
#define PEX_CCSR_SHIFT	12

/* Inbound window */

#define PEX_ITAR3		0xDA0	/* Inb. xlate addr 3 */
#define PEX_IWBAR3		0xDA8	/* Inb. base address 3 */
#define PEX_IWBEAR3		0xDAC	/* Inb. base addr extended 3 */
#define PEX_IWAR3		0xDB0	/* Inb. attrs 3 */
#define PEX_ITAR2		0xDC0	/* Inb. xlate addr 2 */
#define PEX_IWBAR2		0xDC8	/* Inb. base address 2 */
#define PEX_IWBEAR2		0xDCC	/* Inb. base addr extended 2 */
#define PEX_IWAR2		0xDD0	/* Inb. attrs 2 */
#define PEX_ITAR1		0xDE0	/* Inb. xlate addr 1 */
#define PEX_IWBAR1		0xDE8	/* Inb. base address 1 */
#define PEX_IWBEAR1		0xDEC	/* Inb. base addr extended 1 */
#define PEX_IWAR1		0xDF0	/* Inb. attrs 1 */

/* Inbound translation address */

#define PEX_ITAR_TEA		0x00F00000 /* Xlate extended addr */
#define PEX_ITAR_TA		0x000FFFFF /* Xlate addr */

/* Inbound window base address */

#define PEX_IWBAR_WBEA		0xFFF00000 /* Window extended addr */
#define PEX_IWBAR_WBA		0x000FFFFF /* Window base addr */

/* Inbound window extended base address */

#define PEX_IWBEAR_WBEA		0x000FFFFF /* Window extended addr */

/* Inbound window attributes */

#define PEX_ITAR_EN		0x80000000 /* Window enable */
#define PEX_ITAR_DIEN		0x40000000 /* Data invariance enable */
#define PEX_ITAR_PF		0x20000000 /* Prefetchable */
#define PEX_ITAR_TRGT		0x00F00000 /* Target interface */
#define PEX_ITAR_RTT		0x000F0000 /* Read transaction type */
#define PEX_ITAR_WTT		0x0000F000 /* Write transaction type */
#define PEX_ITAR_IWS		0x0000003F /* Inbound window size */

#define PEX_TRGT(x)		(((x) << 20) & PEX_ITAR_TRGT)

#define PEX_TRGT_PEX0		0x0
#define PEX_TRGT_PEX1		0x1
#define PEX_TRGT_PEX2		0x2
#define PEX_TRGT_PEX3		0x3
#define PEX_TRGT_SRIO0		0x8
#define PEX_TRGT_SRIO1		0x9
#define PEX_TRGT_CCSR		0xE
#define PEX_TRGT_MEM		0xF

/* Error management */

#define PEX_ERR_DR		0xE00	/* Error detect register */
#define PEX_ERR_EN		0xE08	/* Error interrupt enable */
#define PEX_ERR_DIS		0xE10	/* Error interrupt disable */
#define PEX_ERR_CAP_STS		0xE20	/* Error capture status */
#define PEX_ERR_CAP0		0xE28	/* Error capture 0 */
#define PEX_ERR_CAP1		0xE2C	/* Error capture 1 */
#define PEX_ERR_CAP2		0xE30	/* Error capture 2 */
#define PEX_ERR_CAP3		0xE34	/* Error capture 3 */

/* Error detect register */

#define PEX_ERRDR_ME		0x80000000 /* Multiple errors */
#define PEX_ERRDR_PCT		0x00800000 /* completion timeout */
#define PEX_ERRDR_PAT		0x00400000 /* ack timeout */
#define PEX_ERRDR_PCAC		0x00200000 /* completion abort */
#define PEX_ERRDR_PNM		0x00100000 /* inbound no map */
#define PEX_ERRDR_CDNSC		0x00080000 /* data completion fail */
#define PEX_ERRDR_CRSNC		0x00040000 /* non-conf completion fail */
#define PEX_ERRDR_ICCA		0x00020000 /* Invalid config access */
#define PEX_ERRDR_IACA		0x00010000 /* Invalid ATMU */
#define PEX_ERRDR_CRST		0x00008000 /* CRS threshold crossed */
#define PEX_ERRDR_MIS		0x00004000 /* Message invalid size */
#define PEX_ERRDR_IOIS		0x00002000 /* I/O invalud size */
#define PEX_ERRDR_CIS		0x00001000 /* Config invalid size */
#define PEX_ERRDR_CIEP		0x00000800 /* Config invalid EP */
#define PEX_ERRDR_IOIEP		0x00000400 /* I/O invalid EP */
#define PEX_ERRDR_OAC		0x00000200 /* Outbound ATMU cross */
#define PEX_ERRDR_IOIA		0x00000100 /* I/O invalid addr */
#define PEX_ERRDR_IMBA		0x00000080 /* mem invalid base addr */
#define PEX_ERRDR_IIOBA		0x00000040 /* I/O invalid base addr */

#define PEX_DBMODE		0xF00	/* Debug mode/status */

/* Debug status register (undocumented) */

#define PEX_DBMODE_GLR		0x00000008 /* Link reset */

/*
 * Configuration space registers
 */

/* Device control register */

#define PEX_CFG_DEVCTL		0x54

#define PEX_CFG_DEVCTL_MAXRD	0x7000 /* Max read size */
#define PEX_CFG_DEVCTL_NSE	0x0800 /* No snoop enable */
#define PEX_CFG_DEVCTL_APE	0x0400 /* AUX PM enable */
#define PEX_CFG_DEVCTL_PFE	0x0200 /* Phantom funcs enable */
#define PEX_CFG_DEVCTL_ETE	0x0100 /* Ext tag field enable */
#define PEX_CFG_DEVCTL_MAXPL	0x00E0 /* Max payload size */
#define PEX_CFG_DEVCTL_RO	0x0010 /* Relaxed ordering */
#define PEX_CFG_DEVCTL_URR	0x0008 /* Unsupported req. reporting */
#define PEX_CFG_DEVCTL_FER	0x0004 /* Fatal error reporting */
#define PEX_CFG_DEVCTL_NFER	0x0002 /* Non-fatal error reporting */
#define PEX_CFG_DEVCTL_CER	0x0001 /* Correctable error reporting */

/* Link training state machine register */

#define PEX_CFG_LTSSM		0x404

#define PEX_CFG_LTSSM_QUIET	0x00
#define PEX_CFG_LTSSM_L0	0x16

/* Control/Status register (existing in PCIe IP >=3.0) */

#define PEX_CSR0		0xF14
#define PEX_CSR1		0xF18

#define PEX_CSR0_LTSSM_SHIFT	2
#define PEX_CSR0_LTSSM_L0	0x11

typedef struct pex_drv_ctrl
    {
    VXB_DEVICE_ID	pexDev;
    void *		pexBar;
    void *		pexHandle;
    void *		ccsrBar;
    BOOL		pexAutoConfig;
    BOOL		pexMsi;
    int			pexNum; 
    BOOL		pexInitialized;
    UINT32		mem32Addr; /* prefetchable */
    UINT32		memIo32Addr; /* non-prefetchable */
    UINT32		io32Addr;
    UINT32		io16Addr;
    struct vxbPciConfig *	pexConfig;
    struct vxbPciInt *		pexIntInfo;
    int			pexMaxBus;
    spinlockIsr_t	pexLock;
    } PEX_DRV_CTRL;

#define TEMPLATE_BAR(p)         ((PEX_DRV_CTRL *)(p)->pDrvCtrl)->pexBar
#define TEMPLATE_HANDLE(p)      ((PEX_DRV_CTRL *)(p)->pDrvCtrl)->pexHandle

#define CSR_READ_4(pDev, addr)                                  \
    vxbRead32 (TEMPLATE_HANDLE(pDev), (UINT32 *)((char *)TEMPLATE_BAR(pDev) + addr))

#define CSR_WRITE_4(pDev, addr, data)                           \
    vxbWrite32 (TEMPLATE_HANDLE(pDev),                             \
        (UINT32 *)((char *)TEMPLATE_BAR(pDev) + addr), data)

#define CSR_READ_2(pDev, addr)                                  \
    vxbRead16 (TEMPLATE_HANDLE(pDev), (UINT16 *)((char *)TEMPLATE_BAR(pDev) + addr))

#define CSR_WRITE_2(pDev, addr, data)                           \
    vxbWrite16 (TEMPLATE_HANDLE(pDev),                             \
        (UINT16 *)((char *)TEMPLATE_BAR(pDev) + addr), data)

#define CSR_READ_1(pDev, addr)                                  \
    vxbRead8 (TEMPLATE_HANDLE(pDev), (UINT8 *)((char *)TEMPLATE_BAR(pDev) + addr))

#define CSR_WRITE_1(pDev, addr, data)                           \
    vxbWrite8 (TEMPLATE_HANDLE(pDev),                              \
        (UINT8 *)((char *)TEMPLATE_BAR(pDev) + addr), data)

#define CSR_SETBIT_1(pDev, offset, val)          \
        CSR_WRITE_1(pDev, offset, CSR_READ_1(pDev, offset) | (val))

#define CSR_CLRBIT_1(pDev, offset, val)          \
        CSR_WRITE_1(pDev, offset, CSR_READ_1(pDev, offset) & ~(val))

#define CSR_SETBIT_2(pDev, offset, val)          \
        CSR_WRITE_2(pDev, offset, CSR_READ_2(pDev, offset) | (val))

#define CSR_CLRBIT_2(pDev, offset, val)          \
        CSR_WRITE_2(pDev, offset, CSR_READ_2(pDev, offset) & ~(val))

#define CSR_SETBIT_4(pDev, offset, val)          \
        CSR_WRITE_4(pDev, offset, CSR_READ_4(pDev, offset) | (val))

#define CSR_CLRBIT_4(pDev, offset, val)          \
        CSR_WRITE_4(pDev, offset, CSR_READ_4(pDev, offset) & ~(val))

#ifdef __cplusplus
}
#endif

#endif /* __INCvxbQorIQPciExh */
