/* tsecMdio.h - header file for TSEC MDIO driver */

/*
 * Copyright (c) 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,29jun07,wap  written
*/

#ifndef __INCvxbProphetM8640dMdioh
#define __INCvxbProphetM8640dMdioh

#ifdef __cplusplus
extern "C" {
#endif

	
#define ETSEC_MIIMCFG                        0x520
#define ETSEC_MIIMCOM                        0x524
#define ETSEC_MIIMADD                        0x528
#define ETSEC_MIIMCON                        0x52C
#define ETSEC_MIIMSTAT                       0x530
#define ETSEC_MIIMIND                        0x534
	
#define ETSEC_MIIMCFG_MCS_28		0x00000007
	
/* MII management command register */

#define ETSEC_MIIMCOM_SCAN		0x00000002
#define ETSEC_MIIMCOM_READ		0x00000001
	
/* MII management indicator register */

#define ETSEC_MIIMIND_NOT_VALID		0x00000004
#define ETSEC_MIIMIND_SCAN		0x00000002
#define ETSEC_MIIMIND_BUSY		0x00000001
	
#define M8640DMDIO_TIMEOUT	1000000
	
	
IMPORT void vxbProphetM8640dMdioRegister (void);




/* Private instance context */

typedef struct m8640d_mdio_drv_ctrl
    {
    VXB_DEVICE_ID	m8640dMdioDev;
    SEM_ID		m8640dMdioMiiSem;
    } M8640D_MDIO_DRV_CTRL;


    
#define M8640_MDIO_CSR_READ_4(pDev, addr)                                  \
        *(volatile UINT32 *)((UINT32)pDev->pRegBase[0] + addr)

#define M8640_MDIO_CSR_WRITE_4(pDev, addr, data)                           \
        do {                                                    \
            volatile UINT32 *pReg =                             \
                (UINT32 *)((UINT32)pDev->pRegBase[0] + addr);   \
            *(pReg) = (UINT32)(data);                           \
            WRS_ASM("eieio");                                   \
        } while ((0))
#define M8640_MDIO_CSR_SETBIT_4(pDev, offset, val)          \
		M8640_MDIO_CSR_WRITE_4(pDev, offset, CSR_READ_4(pDev, offset) | (val))

#define M8640_MDIO_CSR_CLRBIT_4(pDev, offset, val)          \
		M8640_MDIO_CSR_WRITE_4(pDev, offset, CSR_READ_4(pDev, offset) & ~(val))



#ifdef __cplusplus
}
#endif

#endif /* __INCtsecMdioh */
