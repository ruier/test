

#ifndef	INCrapidioReg
#define	INCrapidioReg


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
#define RIO_PORT_N_ERR_STS_CSR(x)	(0x0058 + x*0x20) 
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


#endif /* INCm8641Srio */
