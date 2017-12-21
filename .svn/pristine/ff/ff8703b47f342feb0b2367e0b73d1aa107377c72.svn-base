/* mv88E1xxxphy.h - register definitions for the Marvell 88E1xxx PHY chips */

/*
 * Copyright (c) 2005 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01e,17mar08,wap  Add support for 88E1111 with mode selection (RGMII or GMII)
01d,01aug07,wap  Add support for 88E1011S (WIND00100585)
01c,14jul07,wap  Add support for fiber operation
01b,02nov06,wap  Add 88E1145 support
01a,15sep05,wap  written
*/

#ifndef __PHY_DEPEND_H__
#define __PHY_DEPEND_H__


IMPORT void mvPhyRegister (void);

#define MII_OUI_MARVELL 0x005043        /* Marvell Semiconductor */

#define MII_MODEL_MARVELL_E1011		    0x0002
#define MII_MODEL_MARVELL_E1011S	    0x0006
#define MII_MODEL_xxMARVELL_E1000       0x0005
#define MII_MODEL_MARVELL_E1111		    0x000C
#define MII_MODEL_MARVELL_E1145		    0x000D

#define MV88E_CDS8548_BSP_MOD 1

#define MV88E_MODE_COPPER	            0
#define MV88E_MODE_FIBER	            1
#define MV88E_MODE_RGMII	            2
#define MV88E_MODE_RGMII_FIXUP	        3
#define MV88E_MODE_GMII		            4
#define MV88E_MODE_RGMII_TO_FIBER		6

/* Calibration override, register 30, page 3 */
#define MV_CALOV	30
#define MV_CALOV_RESTART	0x8000	/* Calibration restart */

/* PHY specific control register */
#define MV_CR		16

#define MV_CR_125CLK_DIS	0x0010	/* Disable 125CLK */

/* Extended PHY specific control register */
#define MV_ECR		20

#define MV_ECR_RGMII_RX_DELAY	0x0080	/* 88E114x only */
#define MV_ECR_RGMII_TX_DELAY	0x0002	/* 88E114x only */

/* Extended PHY specific status register */
#define MV_ESR		27

#define MV_ESR_FC_AUTO_DIS	0x8000	/* Fiber/copper autoselect disable */
#define MV_ESR_FC_AUTO_REG	0x4000	/* Fiber/copper auto register select */
#define MV_ESR_FC_RESULT	0x2000	/* Fiber/copper resolution */
#define MV_ESR_HWCFG_MODE	0x000F	/* hardware config mode */

#define MV_HWCFG_GMII_TO_COPPER		15 /* GMII to copper */
#define MV_HWCFG_RGMII_TO_COPPER	11 /* RGMII/modified MII to copper */
#define MV_HWCFG_RGMII_TO_FIBER		3  /* GMII to fiber */
#define MV_HWCFG_TBI_TO_FIBER		5  /* TBI to fiber */

/* Miscellaneous control, register 30, page 18 */
#define MV_MISCCTL	30

#define MV_MISCCTL_PMOS_IMP	0x0E00	/* Calibration PMOS target impedance */
#define MV_MISCCTL_NMOS_IMP	0x01C0	/* Calibration NMOS target impedance */

#define MV_NMOSIMP_1111_41OHM	0x0180
#define MV_NMOSIMP_39OHM	    0x0080

/* Page select register */
#define MV_PAGESEL	29

#define MV_PMOSIMP_1111_41OHM	0x0C00
#define MV_PMOSIMP_36OHM	    0x0400

/* PHY specific status register */
#define MV_STS		17

#define MV_STS_DUPLEX		0x2000	/* current duplex state */
#define MV_STS_LINK		    0x0400	/* link status */
#define MV_STS_SPEED		0xC000	/* current link speed */

#define MV_SPEED_1000		0x8000	/* 1000Mbps */
#define MV_SPEED_100		0x4000	/* 100Mbps */
#define MV_SPEED_10		    0x0000	/* 10Mbps */


#endif /* __PHY_DEPEND_H__ */
