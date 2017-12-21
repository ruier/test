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

#ifndef __INCmv88E1xxxPhyh
#define __INCmv88E1xxxPhyh

IMPORT void mvPhyRegister (void);

#define MII_OUI_MARVELL 0x005043        /* Marvell Semiconductor */
#define MII_OUI_xxMARVELL       0x000ac2        /* Marvell Semiconductor */

#define MII_MODEL_MARVELL_E1000		0x0000
#define MII_MODEL_MARVELL_E1011		0x0002
#define MII_MODEL_MARVELL_E1011S	0x0006
#define MII_MODEL_xxMARVELL_E1000       0x0005
#define MII_MODEL_MARVELL_E1111		0x000C
#define MII_MODEL_MARVELL_E1145		0x000D

#define MV88E_NO_BSP_MOD      0
#define MV88E_CDS8548_BSP_MOD 1

#define MV88E_MODE_COPPER	            0
#define MV88E_MODE_FIBER	            1
#define MV88E_MODE_RGMII	            2
#define MV88E_MODE_RGMII_FIXUP	        3
#define MV88E_MODE_GMII		            4
#define MV88E_MODE_SGMII_TO_RGMII		5
#define MV88E_MODE_RGMII_TO_FIBER		6
#define MV88E_MODE_SGMII_TO_COPPER		7


/*
 * The following constants describe the vendor-specific registers
 * in the Marvell 88E1xxx copper gigabit PHY chips. These apply to
 * both the 88E1011 and the 88E1111 in copper mode. We don't have
 * any boards than use them in fiber mode.
 */

/* PHY specific control register */

#define MV_CR		16

#define MV_CR_TXFIFO		0xC000	/* TX FIFO depth */
#define MV_CR_RXFIFO		0x3000	/* RX FIFO depth */
#define MV_CR_CRS_ON_TX		0x0800	/* assert CRS on transmit */
#define MV_CR_FORCELINK		0x0400	/* force link good */
#define MV_CR_ENERGY_DET_MODE	0x0300	/* energy detect */
#define MV_CR_EXT_CABLE_LEN	0x0080	/* extended cable length mode */
#define MV_CR_MDIX		0x0060	/* MDIX mode */ 
#define MV_CR_125CLK_DIS	0x0010	/* Disable 125CLK */
#define MV_CR_MACIFACE_PDOWN	0x0008	/* MAC interface power down */
#define MV_CR_SQE		0x0004	/* enable SQE test */
#define MV_CR_POLARITY_DIS	0x0002	/* disable polarity reversal */
#define MV_CR_JABBER_DIS	0x0001	/* jabber disable */

/* PHY specific status register */

#define MV_STS		17

#define MV_STS_SPEED		0xC000	/* current link speed */
#define MV_STS_DUPLEX		0x2000	/* current duplex state */
#define MV_STS_PGRX		0x1000	/* page received */
#define MV_STS_SPDDPX_OK	0x0800	/* speed and duplex resolved */
#define MV_STS_LINK		0x0400	/* link status */
#define MV_STS_CABLE_LENGTH	0x0380	/* cable length */
#define MV_STS_MDIX		0x0040	/* MDI crossover status */
#define MV_STS_DOWNSHIFT	0x0020	/* downshift status */
#define MV_STS_ENERGY		0x0010 	/* energy detect status */
#define MV_STS_TXPAUSE		0x0008	/* TX pause enabled */
#define MV_STS_RXPAUSE		0x0004	/* RX pause enabled */
#define MV_STS_POLARITY		0x0002	/* link polarity status */
#define MV_STS_JABBER		0x0001	/* jabber status */

#define MV_SPEED_1000		0x8000	/* 1000Mbps */
#define MV_SPEED_100		0x4000	/* 100Mbps */
#define MV_SPEED_10		0x0000	/* 10Mbps */

#define MV_CABLELEN_50		0x0000	/* less than 50 meters */
#define MV_CABLELEN_50_80	0x0080	/* 50 - 80 meters */
#define MV_CABLELEN_80_110	0x0100	/* 80 - 100 meters */
#define MV_CABLELEN_110_140	0x0180	/* 110 - 140 meters */
#define MV_CABLELEN_140		0x0200	/* more than 140 meters */

/* Interrupt mask register, set a bit to enable the interrupt.  */

#define MV_IMR		18

#define MV_IMR_ANEGERR		0x8000	/* autoneg error */
#define MV_IMR_SPEED		0x4000	/* speed changed */
#define MV_IMR_DUPLEX		0x2000	/* duplex changed */
#define MV_IMR_PGRX		0x1000	/* page received */
#define MV_IMR_ANEGDONE		0x0800	/* autoneg completed */
#define MV_IMR_LINK		0x0400	/* link status changed */
#define MV_IMR_SYMERR		0x0200	/* symbol error */
#define MV_IMR_FALSCAR		0x0100	/* false carrier */
#define MV_IMR_FIFOERR		0x0080	/* FIFO overflow/underflow */
#define MV_IMR_MDIX		0x0040	/* MDI crossover changed */
#define MV_IMR_DOWNSHIFT	0x0020	/* downshift */
#define MV_IMR_ENERGY		0x0010	/* energy detect */
#define MV_IMR_POLARITY		0x0002	/* polarity changed */
#define MV_IMR_JABBER		0x0001	/* jabber detect */

/* Interrupt register */

#define MV_ISR		19

#define MV_ISR_ANEGERR		0x8000	/* autoneg error */
#define MV_ISR_SPEED		0x4000	/* speed changed */
#define MV_ISR_DUPLEX		0x2000	/* duplex changed */
#define MV_ISR_PGRX		0x1000	/* page received */
#define MV_ISR_ANEGDONE		0x0800	/* autoneg completed */
#define MV_ISR_LINK		0x0400	/* link status changed */
#define MV_ISR_SYMERR		0x0200	/* symbol error */
#define MV_ISR_FALSCAR		0x0100	/* false carrier */
#define MV_ISR_FIFOERR		0x0080	/* FIFO overflow/underflow */
#define MV_ISR_MDIX		0x0040	/* MDI crossover changed */
#define MV_ISR_DOWNSHIFT	0x0020	/* downshift */
#define MV_ISR_ENERGY		0x0010	/* energy detect */
#define MV_ISR_POLARITY		0x0002	/* polarity changed */
#define MV_ISR_JABBER		0x0001	/* jabber detect */

/* Extended PHY specific control register */

#define MV_ECR		20

#define MV_ECR_TBI_RCLK		0x1000	/* TBI RCLK */
#define MV_ECR_DOWNSHIFT_MASTER	0x0C00	/* master downshift */
#define MV_ECR_DOWNSHIFT_SLAVE	0x0300	/* slave downshift */
#define MV_ECR_MAC_IF_SPD	0x0070	/* MAC interface speed */
#define MV_ECR_SERIAL_IF_ANEG	0x0008	/* serial iface autoneg enable */

/* The ECR register in the 88E114x parts is defined differently */

#define MV_ECR_CARREXT		0x8000	/* Enable carrier extension */
#define MV_ECR_LOOPBK		0x4000	/* Line loopback */
#define MV_ECR_POWERDOWN	0x2000	/* Power down */
#define MV_ECR_LINKPULSE	0x1000	/* 0 = enable link pulse, 1 = disable*/
#define MV_ECR_DOWNSHIFT_CNT	0x0E00	/* Downshift retry counter */
#define MV_ECR_DOWNSHIFT_ENB	0x0100	/* Downshift enable */
#define MV_ECR_RGMII_RX_DELAY	0x0080	/* 88E114x only */
/*#define MV_ECR_MAC_IF_SPD	0x0070*/	/* MAC interface speed */
#define MV_ECR_DTE_DETECT_ENB	0x0004	/* DTE detect enable */
#define MV_ECR_RGMII_TX_DELAY	0x0002	/* 88E114x only */

/* RX error counter register */

#define MV_RXERR	21


/* 88E1145 Extended PHY Specific Control Register */
#define MV_88E1145_PHY_EXT_CR 20

#define RGMII_RX_delay  0x0080
#define RGMII_TX_delay  0x0002

#define MV_88E1145_PHY_PAGE   29
#define MV_88E1145_PHY_CAL_OV 30

/* Extended PHY specific status register */

#define MV_ESR		27

#define MV_ESR_FC_AUTO_DIS	0x8000	/* Fiber/copper autoselect disable */
#define MV_ESR_FC_AUTO_REG	0x4000	/* Fiber/copper auto register select */
#define MV_ESR_FC_RESULT	0x2000	/* Fiber/copper resolution */
#define MV_ESR_SIF_ANEG_BYP_ENB	0x1000	/* Serial IF autoneg bypass enable */
#define MV_ESR_SIF_ANEG_BYP_STS	0x0800	/* Serial IF autoneg bypass status */
#define MV_ESR_INTR_POL		0x0400	/* interrupt polarity */
#define MV_ESR_AUTO_MEDIUM_DIS	0x0200	/* Disable auto medium register sel */
#define MV_ESR_DTE_STS_HYSTER	0x01E0	/* DTE status hysteresis */
#define MV_ESR_DTE_PWR_STS	0x0010	/* 1 = DTE power needed, 0 = not */
#define MV_ESR_HWCFG_MODE	0x000F	/* hardware config mode */

/* Not all of these are supported on all chips */

#define MV_HWCFG_SGMII_CLK		0  /* SGMII, copper, with clock */
#define MV_HWCFG_MII_TO_100FX		2  /* MII to 100baseFX */
#define MV_HWCFG_RGMII_TO_FIBER		3  /* GMII to fiber */
#define MV_HWCFG_SGMII_NOCLK		4  /* SGMII, copper, no clock */
#define MV_HWCFG_TBI_TO_FIBER		5  /* TBI to fiber */
#define MV_HWCFG_RGMII_TO_SGMII		6  /* RGMII to SGMII */
#define MV_HWCFG_GMII_TO_FIBER		7  /* GMII to fiber */
#define MV_HWCFG_1000BX_ANEG		8  /* 1000baseX, autoneg to copper */
#define MV_HWCFG_RTBI_TO_COPPER		9  /* RTBI to copper */
#define MV_HWCFG_RGMII_TO_100FX		10 /* RTMII to 100baseFX */
#define MV_HWCFG_RGMII_TO_COPPER	11 /* RGMII/modified MII to copper */
#define MV_HWCFG_1000BX_NOANEG		12 /* 1000baseX, no autoneg to copper */
#define MV_HWCFG_TBI_TO_COPPER		13 /* TBI to copper */
#define MV_HWCFG_GMII_TO_SGMII		14 /* GMII to SGMII */
#define MV_HWCFG_GMII_TO_COPPER		15 /* GMII to copper */

/* Page select register */

#define MV_PAGESEL	29

/* The following two special registers are specific to the 88E114x parts */

/* Calibration override, register 30, page 3 */

#define MV_CALOV	30

#define MV_CALOV_RESTART	0x8000	/* Calibration restart */
#define MV_CALOV_COMPLETE	0x4000	/* Calibration complete */
#define MV_CALOV_PMOSVAL	0x1F00
#define MV_CALOV_LATCH		0x0040	/* Latch in new value */
#define MV_CALOV_PMOS_NMOS	0x0020 	/* 1 == latch PMOS, 0 == latch NMOS */
#define MV_CALOV_NMOSVAL	0x001F

#define MV_PMOS(x)		(((x) & 0x1F) << 8)
#define MV_NMOS(x)		((x) & 0x1F)

/* Miscellaneous control, register 30, page 18 */

#define MV_MISCCTL	30

#define MV_MISCCTL_RSVD0	0xF000	/* Preserve when updating bit 0 or 1 */
#define MV_MISCCTL_PMOS_IMP	0x0E00	/* Calibration PMOS target impedance */
#define MV_MISCCTL_NMOS_IMP	0x01C0	/* Calibration NMOS target impedance */
#define MV_MISCCTL_PKTGEN	0x003C	/* packet generator */
#define MV_MISCCTL_RSVD1	0x0002	/* Preserve when updating bit 0 or 1 */
#define MV_MISCCTL_NEXT_DISABLE	0x0001	/* Disable NEXT noise cancelling */

/* Default values are PMOS == 45 ohms and NMOS == 56 ohms */

#define MV_PMOSIMP_52OHM	0x0000
#define MV_PMOSIMP_45OHM	0x0200
#define MV_PMOSIMP_36OHM	0x0400
#define MV_PMOSIMP_32OHM	0x0600
#define MV_PMOSIMP_30OHM	0x0800
#define MV_PMOSIMP_25OHM	0x0A00
#define MV_PMOSIMP_24OHM	0x0C00
#define MV_PMOSIMP_22OHM	0x0E00

#define MV_NMOSIMP_55OHM	0x0000
#define MV_NMOSIMP_56OHM	0x0040
#define MV_NMOSIMP_39OHM	0x0080
#define MV_NMOSIMP_34OHM	0x00C0
#define MV_NMOSIMP_30OHM	0x0100
#define MV_NMOSIMP_27OHM	0x0140
#define MV_NMOSIMP_24OHM	0x0180
#define MV_NMOSIMP_22OHM	0x01C0

#define MV_PMOSIMP_1111_80OHM	0x0000
#define MV_PMOSIMP_1111_69OHM	0x0200
#define MV_PMOSIMP_1111_49OHM	0x0800
#define MV_PMOSIMP_1111_44OHM	0x0A00
#define MV_PMOSIMP_1111_41OHM	0x0C00
#define MV_PMOSIMP_1111_38OHM	0x0E00

#define MV_NMOSIMP_1111_80OHM	0x0000
#define MV_NMOSIMP_1111_69OHM	0x0040
#define MV_NMOSIMP_1111_49OHM	0x0100
#define MV_NMOSIMP_1111_44OHM	0x0140
#define MV_NMOSIMP_1111_41OHM	0x0180
#define MV_NMOSIMP_1111_38OHM	0x01C0

#endif /* __INCmv88E1xxxPhyh */
