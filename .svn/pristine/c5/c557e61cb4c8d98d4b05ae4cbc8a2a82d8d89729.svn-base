/* rioPrivate.h - rapid IO private configuration info.*/

/*
 * Copyright (c) 2012- Prophet Electronic,Inc.
 * 
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Prophet license agreement. 
 */

/*
modification history
-----------------------
10/09/2012,sjy Created the file.
 */

/*
 * DESCRIPTION
 * private Info.
 */

#ifndef __RIO_PRIVATE_H_
#define __RIOP_RIVATE_H_
#ifdef __cplusplus
extern "C" {
#endif

	/* Registers' offset definations */
#define AIDCAR 0x000008      	       /*Assembly identity capability register (AIDCAR)*/
#define PEFCAR 0x000010                /* Processing Element Features CAR */
#define SPICAR 0x000014                /* Swithc Port Information CAR */
#define COMTAGCSR 0x00006C             /* PE component tag CSR */
#define BDIDCSR 0x000060               /* Base Device ID Command and Status Register */
#define SRTECSCSR 0x000070             /* Standard Route Table Entries Configuration destID Select CSR */
#define SRTECPSCSR 0x000074            /* Standard Route Table Entry Configuration Port Select CSR */
#define PGCCSR 0x00013C                /* Port General Control CSR */
#define RPSR 0x10070                   /* Route Port Select Register */
#define HBDLCSR 0x000068               /* Host Base deviceID Lock CSR */
#define LTLEDCSR 0x0608                              /* Logical/Transport layer error detect command and \
                                          status register ()	*/
#define PRTOCCSR 0x0124                /* Port response time-out control command and status register */
#define LTLEECSR 0x060c/*Logical/Transport layer error enable command and status register*/ 
#define RDR      0xf20020              /* RapidIO Domain register */

	/* Port registers's offset definations */
#define PORT_ESCSR    0x000158         /* Port {0..17}Error and Status CSR */

#define	PORT_ERR_STAT_CSR(port)   (PORT_ESCSR+0x20*port)  
#define	PORT_CTL_CSR(port)   (0X15c+0x20*port)
#define PORT_LINK_MAINT_REQ_CSR(port)   (0x140+0x20*port)
#define PORT_LINK_MAINT_RESP_CSR(port)  (0x144+0x20*port)
#define PORT_LOCAL_ACKID_CSR(port)  (0x148+0x20*port)	
    /* from rapidioReg.h*/
	/* Helper macros to parse the Extended Feature Block header */
#define RIO_EFB_PTR_MASK	0xffff0000
#define RIO_EFB_ID_MASK		0x0000ffff
#define RIO_GET_BLOCK_PTR(x)	((x & RIO_EFB_PTR_MASK) >> 16)
#define RIO_GET_BLOCK_ID(x)	(x & RIO_EFB_ID_MASK)

	/* Extended Feature Block IDs */
#define RIO_EFB_PAR_EP_ID	0x0001	/* [IV] LP/LVDS EP Devices */
#define RIO_EFB_PAR_EP_REC_ID	0x0002	/* [IV] LP/LVDS EP Recovery Devices */
#define RIO_EFB_PAR_EP_FREE_ID	0x0003	/* [IV] LP/LVDS EP Free Devices */
#define RIO_EFB_SER_EP_ID	0x0004	/* [VI] LP/Serial EP Devices */
#define RIO_EFB_SER_EP_REC_ID	0x0005	/* [VI] LP/Serial EP Recovery Devices */
#define RIO_EFB_SER_EP_FREE_ID	0x0006	/* [VI] LP/Serial EP Free Devices */

#define RIO_EFB_SW_REC_ID	0x0009	/* [VI] Switch with software recovery capability*/

	/*
	 * Physical 8/16 LP-LVDS
	 * ID=0x0001, Generic End Point Devices
	 * ID=0x0002, Generic End Point Devices, software assisted recovery option
	 * ID=0x0003, Generic End Point Free Devices
	 *
	 * Physical LP-Serial
	 * ID=0x0004, Generic End Point Devices
	 * ID=0x0005, Generic End Point Devices, software assisted recovery option
	 * ID=0x0006, Generic End Point Free Devices

	   ID=0x0009,Switch with software recovery capability

	 */


#define RIO_PORT_N_MNT_REQ_CSR(x)	(0x0040 + x*0x20)	/* 0x0002 */
#define RIO_PORT_N_MNT_RSP_CSR(x)	(0x0044 + x*0x20)	/* 0x0002 */
#define RIO_PORT_N_ACK_STS_CSR(x)	(0x0048 + x*0x20)	/* 0x0002 */
#define RIO_PORT_N_ERR_STS_CSR(x)	(0x058 + x*0x20) 
#define PORT_N_ERR_STS_PORT_OK	0x00000002
#define RIO_PORT_N_CTL_CSR(x)		(0x5c + x*0x20)



#define RIO_DEV_ID_CAR		0x00	/* [I] Device Identity CAR */
#define RIO_DEV_INFO_CAR	0x04	/* [I] Device Information CAR */
#define RIO_ASM_ID_CAR		0x08	/* [I] Assembly Identity CAR */
#define  RIO_ASM_ID_MASK		0xffff0000	/* [I] Asm ID Mask */
#define  RIO_ASM_VEN_ID_MASK		0x0000ffff	/* [I] Asm Vend Mask */

#define RIO_ASM_INFO_CAR	0x0c	/* [I] Assembly Information CAR */
#define  RIO_ASM_REV_MASK		0xffff0000	/* [I] Asm Rev Mask */
#define  RIO_EXT_FTR_PTR_MASK		0x0000ffff	/* [I] EF_PTR Mask */

#define RIO_PEF_CAR		0x10	/* [I] Processing Element Features CAR */
#define  RIO_PEF_SWITCH			0x10000000	/* [I] Switch */

#define RIO_SWP_INFO_CAR	0x14	/* [I] Switch Port Information CAR */


#define RIO_DID_CSR		0x60	/* [III] Base Device ID CSR */
	/* 0x64 *//* Reserved */
#define RIO_HOST_DID_LOCK_CSR	0x68	/* [III] Host Base Device ID Lock CSR */
#define RIO_COMPONENT_TAG_CSR	0x6c	/* [III] Component Tag CSR */

#ifdef __cplusplus
}
#endif
#endif /* __CPS1848RIOPRIVATE_H */
