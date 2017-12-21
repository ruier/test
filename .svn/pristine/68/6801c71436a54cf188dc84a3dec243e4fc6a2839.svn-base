/* ppc5200.h - Motorola MPC5200 CPU control registers */

/*
 * Copyright (c) 2003, 2005-2006, 2009, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01o,05apr13,ylu  add arbiter configuration register macros BLDIS. 
01n,10sep09,x_z  move SVR to vxALib.s
01m,21aug09,e_d  change __INCm5200h to __INCppc5200h. (WIND00115449)
01l,14aug09,e_d  change PSC6_PORT to 6. (WIND00176550)
01k,05aug09,e_d  add ATA_BASE_ADRS0 and ATA_BASE_ADRS1 define. (WIND00171937)
01j,15dec06,jw   change PCI Device ID for Lite5200B board, add Sdelay register
01i,22feb06,pkr  add RTC registers
01h,03feb06,pkr  add SVR
01g,11aug05,j_b  added PCI reg macros
01f,04may05,pkr  add arbiter master priority register definitions
01e,05dec03,hkh  error correction: arithmetic expressions in macros has to be put between "(" and ")"
				 wrong shifts for EENA2, EENA3
01d,01dec03,e_c  add register definition for fast boot
01c,18nov03,pdr  merge from IceCube.
01b,15sep03,pdr  added register definition
01a,25Jun03,pkr	 written
*/

/*
This file contains I/O addresses and related constants for the MPC5200
*/

#ifndef __INCppc5200h
#define __INCppc5200h

#ifdef __cplusplus
extern "C" {
#endif

/* additional special purpose register encoding */

#define SPRG4       276 /* software program register 4 */
#define SPRG5       277 /* software program register 5 */
#define SPRG6       278 /* software program register 6 */
#define SPRG7       279 /* software program register 7 */

#define PVR         287 /* processor version register */

#define HID2        1011 /* Hardware Implmentation-Dependent Register 2 */
#define MBAR        311 /* Memory Base Address Register */

#define MBAR_SIZE	0x40000	/* 256 KB */

#define MBAR_RESET_VALUE	0x80000000

#define MPC5200_PCI_DEVICE_ID	0x5809  /* MPC5200B prcoessor */
#define MPC5200_PCI_VENDOR_ID	0x1057
#define MPC5200_PCI_ID			((MPC5200_PCI_DEVICE_ID << 16) | MPC5200_PCI_VENDOR_ID)

/*-----------------------------------------------------------*/
/* IPBI Peripheral registers 						  0x0000 */
/*-----------------------------------------------------------*/

/* MBAR to IPBI */
#define MBAR_IPBI				0x0000

#define IPBI_CS0_START_OFF		0x0004
#define IPBI_CS0_STOP_OFF		0x0008
#define IPBI_CS1_START_OFF		0x000C
#define IPBI_CS1_STOP_OFF		0x0010
#define IPBI_CS2_START_OFF		0x0014
#define IPBI_CS2_STOP_OFF		0x0018
#define IPBI_CTRL_OFF			0x0054

#define IPBI_SDRAM_CONF0_OFF	0x0034
#define IPBI_SDRAM_CONF1_OFF	0x0038

#define IPBI_CTRL_CSBOOT		0x02000000
#define IPBI_CTRL_CS0			0x00010000
#define IPBI_CTRL_CS1			0x00020000
#define IPBI_CTRL_CS2			0x00040000
#define IPBI_CTRL_WSE			0x00000001

#define IPBI_BASE_ADRS			(MBAR_VALUE + MBAR_IPBI)

#define IPBI_CS2_START			((volatile UINT32 *)(IPBI_BASE_ADRS + IPBI_CS2_START_OFF))
#define IPBI_CS2_STOP			((volatile UINT32 *)(IPBI_BASE_ADRS + IPBI_CS2_STOP_OFF))
#define IPBI_CTRL				((volatile UINT32 *)(IPBI_BASE_ADRS + IPBI_CTRL_OFF))

/*-----------------------------------------------------------*/
/* SDRAM											  0x0100 */
/*-----------------------------------------------------------*/

/* MBAR to LPC */
#define MBAR_SDRAM		0x0100
#define SDRAM_BASE_ADRS			(MBAR_VALUE + MBAR_SDRAM)

#define SDRAM_MODE_OFF		0x0000
#define SDRAM_CTRL_OFF		0x0004
#define SDRAM_CONF1_OFF		0x0008
#define SDRAM_CONF2_OFF		0x000C
#define SDRAM_SDELAY_OFF        0x0090
#define SDRAM_SDELAY			((volatile UINT32 *)(SDRAM_BASE_ADRS + SDRAM_SDELAY_OFF))

/*-----------------------------------------------------------*/
/* Clock Distribution Module (CDM)                    0x0200 */
/*-----------------------------------------------------------*/

/* MBAR to CDM */
#define MBAR_CDM			0x0200

#define CDM_PORSTCFG_OFF	0x0004	/* CDM Power On Reset Configuration Register */
#define CDM_BREAD_CRUM_OFF	0x0008	/* CDM Bread Crumb Register */
#define CDM_CFG_OFF			0x000C	/* CDM Configuration Register */
#define CDM_DIVIDER_OFF		0x0010	/* CDM 48MHz Fractional Divider Configuration Register */
#define CDM_CLK_ENABLE_OFF	0x0014	/* CDM Clock Enable Register */
#define CDM_OSC_CONFIG_OFF	0x0018	/* CDM System Oscillator Configuration Register */
#define CDM_PSC1_MCLOCK_OFF	0x0028	/* CDM PSC1 Mclock Config */
#define CDM_PSC2_MCLOCK_OFF	0x002C	/* CDM PSC2 Mclock Config */
#define CDM_PSC3_MCLOCK_OFF	0x0030	/* CDM PSC3 Mclock Config */
#define CDM_PSC6_MCLOCK_OFF	0x0034	/* CDM PSC6 Mclock Config */
	
#define CDM_BASE_ADRS		(MBAR_VALUE + MBAR_CDM)

#define CDM_PORSTCFG   		((volatile UINT32 *)(CDM_BASE_ADRS + CDM_PORSTCFG_OFF   ))
#define CDM_BREAD_CRUM 		((volatile UINT32 *)(CDM_BASE_ADRS + CDM_BREAD_CRUM_OFF ))
#define CDM_CFG        		((volatile UINT32 *)(CDM_BASE_ADRS + CDM_CFG_OFF        ))
#define CDM_DIVIDER    		((volatile UINT32 *)(CDM_BASE_ADRS + CDM_DIVIDER_OFF    ))
#define CDM_CLK_ENABLE 		((volatile UINT32 *)(CDM_BASE_ADRS + CDM_CLK_ENABLE_OFF ))
#define CDM_OSC_CONFIG 		((volatile UINT32 *)(CDM_BASE_ADRS + CDM_OSC_CONFIG_OFF ))
#define CDM_PSC1_MCLOCK		((volatile UINT32 *)(CDM_BASE_ADRS + CDM_PSC1_MCLOCK_OFF))
#define CDM_PSC2_MCLOCK		((volatile UINT32 *)(CDM_BASE_ADRS + CDM_PSC2_MCLOCK_OFF))
#define CDM_PSC3_MCLOCK		((volatile UINT32 *)(CDM_BASE_ADRS + CDM_PSC3_MCLOCK_OFF))
#define CDM_PSC6_MCLOCK		((volatile UINT32 *)(CDM_BASE_ADRS + CDM_PSC6_MCLOCK_OFF))

/*-----------------------------------------------------------*/
/* LPC 												  0x0300 */
/*-----------------------------------------------------------*/

/* MBAR to LPC */
#define MBAR_LPC				0x0300

#define LPC_CS0_BOOT_CFG_OFF	0x0000
#define LPC_CS1_CFG_OFF			0x0004
#define LPC_CS2_CFG_OFF			0x0008
#define LPC_CS_CTRL_OFF			0x0018
#define LPC_CS_BURST_CTRL_OFF	0x0028
#define LPC_CS_DEAD_CTRL_OFF	0x002C

#define LPC_CS_CTRL_EBEE		0x8000
#define LPC_CS_CTRL_ME			0x0100

#define LPC_BASE_ADRS			(MBAR_VALUE + MBAR_LPC)

#define LPC_CS2_CFG				((volatile UINT32 *)(LPC_BASE_ADRS + LPC_CS2_CFG_OFF))

/*----------------------------------------------------------- */
/* Interrupt Controller (ICTL)                         0x0500 */
/*----------------------------------------------------------- */

#define MBAR_ICTL			0x0500

 
#define ICTL_PIMR_OFF		0x0		/* ICTL Peripheral Interrupt Mask Register */
#define ICTL_PPR_OFF		0x4		/* ICTL Peripheral Priority and HI/LO Select [3] Register */
#define ICTL_EEETR_OFF	    0x10	/* ICTL External Enable and Externals Types Register */
#define ICTL_CPMIMR_OFF	    0x14	/* ICTL Critical Priority and Main Interrupt Mask Register */
#define ICTL_MIPR_OFF		0x18	/* ICTL Main Interrupt Priority and INT/SMI Select [2] Register */
#define ICTL_ISR_OFF		0x24	/* ICTL PerStat, MainStat, CritStat Encoded Register */
#define CTL_CISAR_OFF	    0x28	/* ICTL Critical Interrupt Status All Register */
#define ICTL_MISAR_OFF	    0x2C    /* ICTL Main Interrupt Status All Register */
#define ICTL_PISAR_OFF	    0x30    /* ICTL Peripheral Interrupt Status All Register */
#define ICTL_BISAR_OFF	    0x38    /* ICTL Bus Error Status Register */

#define ICTL_BASE_ADRS		(MBAR_VALUE + MBAR_ICTL)

#define ICTL_PIMR			((volatile UINT32 *)(ICTL_BASE_ADRS + ICTL_PIMR_OFF))
#define ICTL_PPR			((volatile UINT32 *)(ICTL_BASE_ADRS + ICTL_PPR_OFF))		/* [3] */
#define ICTL_EEETR			((volatile UINT32 *)(ICTL_BASE_ADRS + ICTL_EEETR_OFF))
#define ICTL_CPMIMR			((volatile UINT32 *)(ICTL_BASE_ADRS + ICTL_CPMIMR_OFF))
#define ICTL_MIPR			((volatile UINT32 *)(ICTL_BASE_ADRS + ICTL_MIPR_OFF))	/* [2] */
#define ICTL_ISR			((volatile UINT32 *)(ICTL_BASE_ADRS + ICTL_ISR_OFF))
#define ICTL_CISAR			((volatile UINT32 *)(ICTL_BASE_ADRS + ICTL_CISAR_OFF))
#define ICTL_MISAR			((volatile UINT32 *)(ICTL_BASE_ADRS + ICTL_MISAR_OFF))
#define ICTL_PISAR			((volatile UINT32 *)(ICTL_BASE_ADRS + ICTL_PISAR_OFF))
#define ICTL_BISAR			((volatile UINT32 *)(ICTL_BASE_ADRS + ICTL_BISAR_OFF))



/* External Enable and External Types Register */

#define ICTL_EEETR_ECLR0_MASK			(1<<27)
#define ICTL_EEETR_ECLR1_MASK			(1<<26)
#define ICTL_EEETR_ECLR2_MASK			(1<<25)
#define ICTL_EEETR_ECLR3_MASK			(1<<24)

#define ICTL_EEETR_ETYPE0_MASK			(3<<22)
#define ICTL_EEETR_ETYPE0_LEVEL_HI		(0<<22)
#define ICTL_EEETR_ETYPE0_RISING_EDGE	(1<<22)
#define ICTL_EEETR_ETYPE0_FALLING_EDGE	(2<<22)
#define ICTL_EEETR_ETYPE0_LEVEL_LO		(3<<22)

#define ICTL_EEETR_ETYPE1_MASK			(3<<20)
#define ICTL_EEETR_ETYPE1_LEVEL_HI		(0<<20)
#define ICTL_EEETR_ETYPE1_RISING_EDGE	(1<<20)
#define ICTL_EEETR_ETYPE1_FALLING_EDGE	(2<<20)
#define ICTL_EEETR_ETYPE1_LEVEL_LO 	 	(3<<20)

#define ICTL_EEETR_ETYPE2_MASK			(3<<18)
#define ICTL_EEETR_ETYPE2_LEVEL_HI		(0<<18)
#define ICTL_EEETR_ETYPE2_RISING_EDGE	(1<<18)
#define ICTL_EEETR_ETYPE2_FALLING_EDGE	(2<<18)
#define ICTL_EEETR_ETYPE2_LEVEL_LO		(3<<18)

#define ICTL_EEETR_ETYPE3_MASK			(3<<16)
#define ICTL_EEETR_ETYPE3_LEVEL_HI		(0<<16)
#define ICTL_EEETR_ETYPE3_RISING_EDGE	(1<<16)
#define ICTL_EEETR_ETYPE3_FALLING_EDGE	(2<<16)
#define ICTL_EEETR_ETYPE3_LEVEL_LO		(3<<16)


#define ICTL_EEETR_MEE					(1<<12)

#define ICTL_EEETR_EENA0_MASK			(1<<11)
#define ICTL_EEETR_EENA1_MASK			(1<<10)
#define ICTL_EEETR_EENA2_MASK			(1<<9)
#define ICTL_EEETR_EENA3_MASK			(1<<8)

#define ICTL_EEETR_CEb					(1<<0)

/* Critical Priority and Main Interrupt Mask Register */

#define ICTL_CPMIMR_MAIN_MASK0			(1<<16)
#define ICTL_CPMIMR_MAIN_MASK1			(1<<15)
#define ICTL_CPMIMR_MAIN_MASK2			(1<<14)
#define ICTL_CPMIMR_MAIN_MASK3			(1<<13)
#define ICTL_CPMIMR_MAIN_MASK4			(1<<12)
#define ICTL_CPMIMR_MAIN_MASK5			(1<<11)
#define ICTL_CPMIMR_MAIN_MASK6			(1<<10)
#define ICTL_CPMIMR_MAIN_MASK7			(1<<9)
#define ICTL_CPMIMR_MAIN_MASK8			(1<<8)
#define ICTL_CPMIMR_MAIN_MASK9			(1<<7)
#define ICTL_CPMIMR_MAIN_MASK10			(1<<6)
#define ICTL_CPMIMR_MAIN_MASK11			(1<<5)
#define ICTL_CPMIMR_MAIN_MASK12			(1<<4)
#define ICTL_CPMIMR_MAIN_MASK13			(1<<3)
#define ICTL_CPMIMR_MAIN_MASK14			(1<<2)
#define ICTL_CPMIMR_MAIN_MASK15			(1<<1)
#define ICTL_CPMIMR_MAIN_MASK16			(1<<0)
#define ICTL_CPMIMR_MASK_ALL			0x0001FFFF

/* Interrupt Status Register */

#define ICTL_CEbSh						(1<<0)
#define ICTL_ISR_CSe_SHIFT				8
#define ICTL_ISR_CSe_MASK				(7<<ICTL_ISR_CSe_SHIFT)
#define ICTL_ISR_CSe_MASK_NR			(3<<ICTL_ISR_CSe_SHIFT)
#define ICTL_ISR_MSe_SHIFT				16
#define ICTL_ISR_MSe_MASK				(0x3f<<ICTL_ISR_MSe_SHIFT)
#define ICTL_ISR_MSe_MASK_NR			(0x1f<<ICTL_ISR_MSe_SHIFT)
#define ICTL_ISR_PSe_SHIFT				24
#define ICTL_ISR_PSe_MASK				(0x3f<<ICTL_ISR_PSe_SHIFT)
#define ICTL_ISR_PSe_MASK_NR			(0x1f<<ICTL_ISR_PSe_SHIFT)

/*-----------------------------------------------------------*/
/* General Purpose Timer (GPT)                        0x0600 */
/*-----------------------------------------------------------*/

/* MBAR to GPT */
#define MBAR_GPT			0x0600	/* General Purpose Timer base register (GPT) */

#define GPT_NUM_TIMER_MAX	8	/* number of timers */

#define GPT_BASE(num)		((UINT32) (MBAR_VALUE + MBAR_GPT + (num*0x10)))

#define GPT_EMSR_OFF		0x0	/* GPT Enable and mode Select Register */
#define GPT_CIR_OFF			0x4	/* GPT Counter input register */
#define GPT_PWMCR_OFF		0x8	/* GPT PWM Configuration Register */
#define GPT_SR_OFF			0xc	/* GPT STatus Register */

#define GPT_EMSR(num)		((volatile UINT32 *) (GPT_BASE(num) + GPT_EMSR_OFF))
#define GPT_CIR(num)		((volatile UINT32 *) (GPT_BASE(num) + GPT_CIR_OFF))
#define GPT_PWMCR(num)		((volatile UINT32 *) (GPT_BASE(num) + GPT_PWMCR_OFF))
#define GPT_SR(num)			((volatile UINT32 *) (GPT_BASE(num) + GPT_SR_OFF))


/*-----------------------------------------------------------*/
/* Slice Timer (SLT)                                  0x0700 */
/*-----------------------------------------------------------*/

/* MBAR to SLT */
#define MBAR_SLT			0x0700	/* Slice Timer base register (SLT) */

#define SLT_NUM_TIMER_MAX	2	 /* number of timers */
#define SLT_ADRS_OFF		0x10 /* Timer 1 register offset from slt0 registers */


/* registers access definition */
#define SLT0_TCNT_OFF		0x00	/* SLT0 Terminal Count Register */
#define SLT0_CTRL_OFF		0x04	/* SLT0 Control Register */
#define SLT0_VALUE_OFF		0x08	/* SLT0 Count Value register */
#define SLT0_STATUS_OFF		0x0C	/* SLT0 Timer Status register */
	
#define SLT1_TCNT_OFF		0x10	/* SLT0 Terminal Count Register */
#define SLT1_CTRL_OFF		0x14	/* SLT0 Control Register */
#define SLT1_VALUE_OFF		0x18	/* SLT0 Count Value register */
#define SLT1_STATUS_OFF		0x1C	/* SLT0 Timer Status register */


	
#define SLT0_BASE_ADRS		(MBAR_VALUE + MBAR_SLT)
#define SLT_BASE(num)		((UINT32) (SLT0_BASE_ADRS + (num*0x10)))

#define SLT0_TCNT			((volatile UINT32 *) (SLT0_BASE_ADRS + SLT0_TCNT_OFF))
#define SLT0_CTRL			((volatile UINT32 *) (SLT0_BASE_ADRS + SLT0_CTRL_OFF))
#define SLT0_VALUE			((volatile UINT32 *) (SLT0_BASE_ADRS + SLT0_VALUE_OFF))
#define SLT0_STATUS			((volatile UINT32 *) (SLT0_BASE_ADRS + SLT0_STATUS_OFF))

#define SLT1_TCNT			((volatile UINT32 *) (SLT0_BASE_ADRS + SLT1_TCNT_OFF))
#define SLT1_CTRL			((volatile UINT32 *) (SLT0_BASE_ADRS + SLT1_CTRL_OFF))
#define SLT1_VALUE			((volatile UINT32 *) (SLT0_BASE_ADRS + SLT1_VALUE_OFF))
#define SLT1_STATUS			((volatile UINT32 *) (SLT0_BASE_ADRS + SLT1_STATUS_OFF))

#define SLT_TCNT(num)		((volatile UINT32 *) (SLT_BASE(num) + SLT0_TCNT_OFF))
#define SLT_CTRL(num)		((volatile UINT32 *) (SLT_BASE(num) + SLT0_CTRL_OFF))
#define SLT_VALUE(num)		((volatile UINT32 *) (SLT_BASE(num) + SLT0_VALUE_OFF))
#define SLT_STATUS(num)	    ((volatile UINT32 *) (SLT_BASE(num) + SLT0_STATUS_OFF))


/* Control Register */
#define SLT_CTRL_ENABLE		0x01000000	/* Timer enable bit */
#define SLT_CTRL_INT_ENABLE	0x02000000	/* Interrupt enable bit */
#define SLT_CTRL_RUN_WAIT	0x04000000	/* Run/Wait bit (continusly mode or not) */

/* Status Register */
#define SLT_STATUS_RESET	0x01000000	/* ST bit - Timer reached terminal count */


/*-----------------------------------------------------------*/
/* Real Time Clock (RTC)							  0x0800 */
/*-----------------------------------------------------------*/

/* MBAR to GPT */
#define MBAR_RTC			0x0800	/* Real Time Clock base register */

#define RTC_BASE_ADRS		(MBAR_VALUE + MBAR_RTC)


#define RTC_TS_OFF			0x0	/* RTC Time Set Register */

#define RTC_TS_SET_TIME			0x02000000
#define RTC_TS_PAUSE_TIME		0x01000000
#define RTC_TS_SLCT_HOUR_12		0x00200000
#define RTC_TS_SLCT_HOUR_24		0x00000000
#define RTC_TS_C24HOUR_SHIFT	16
#define RTC_TS_MINUTE_SHIFT		8
#define RTC_TS_SECOND_SHIFT		0

#define RTC_DS_OFF			0x4	/* RTC Date Set register */

#define RTC_DS_SET_TIME			0x02000000
#define RTC_DS_PAUSE_TIME		0x01000000
#define RTC_DS_MONTH_SHIFT		16
#define RTC_DS_WEEKDAY_SHIFT	8
#define RTC_DS_DAY_SHIFT		0

#define RTC_NYS_OFF			0x8	/* RTC New Year Software Register */

#define RTC_NYS_WRITE_SW		0x01000000
#define RTC_NYS_SW_SHIFT		16
#define RTC_NYS_YEAR_MASK		0x00000FFF
#define RTC_NYS_YEAR_SHIFT		0

#define RTC_AIE_OFF			0xc	/* RTC Alarm/Interrupt Enable Register */

#define RTC_CT_OFF			0x10	/* RTC Current Time Register */

#define RTC_CT_HOUR_MASK		0x001F0000
#define RTC_CT_HOUR_SHIFT		16
#define RTC_CT_MINUTE_MASK		0x00003F00
#define RTC_CT_MINUTE_SHIFT		8
#define RTC_CT_SECOND_MASK		0x0000003F
#define RTC_CT_SECOND_SHIFT		0

#define RTC_CD_OFF			0x14	/* RTC Current Date Register */

#define RTC_CD_MONTH_MASK		0x0F000000
#define RTC_CD_MONTH_SHIFT		24
#define RTC_CD_WEEKDAY_MASK		0x00E00000
#define RTC_CD_WEEKDAY_SHIFT	21
#define RTC_CD_DAY_MASK			0x001F0000
#define RTC_CD_DAY_SHIFT		16
#define RTC_CD_YEAR_MASK		0x00000FFF
#define RTC_CD_YEAR_SHIFT		0

#define RTC_TS		((volatile UINT32 *) (RTC_BASE_ADRS + RTC_TS_OFF))
#define RTC_DS		((volatile UINT32 *) (RTC_BASE_ADRS + RTC_DS_OFF))
#define RTC_NYS		((volatile UINT32 *) (RTC_BASE_ADRS + RTC_NYS_OFF))
#define RTC_AIE		((volatile UINT32 *) (RTC_BASE_ADRS + RTC_AIE_OFF))
#define RTC_CT 		((volatile UINT32 *)(RTC_BASE_ADRS + RTC_CT_OFF))
#define RTC_CD 		((volatile UINT32 *)(RTC_BASE_ADRS + RTC_CD_OFF))


/*-----------------------------------------------------------*/
/* I 2 C controlers (I2C)                             0x3d00 */
/*-----------------------------------------------------------*/

/* MBAR to I2C */
#define MBAR_I2C			0x3d00	/* I2C base register (I2C) */

#define I2C_NUM_MAX			2		/* number of I2C controllers */
#define I2C_ADRS_OFF		0x40	/* I2C2 register offset from I2C1 */


/* registers access definition */
#define I2C_ADR_OFF			0x00	/* I2C Address Register */
#define I2C_FDR_OFF			0x04	/* I2C Frequency Divider Register */
#define I2C_CR_OFF			0x08	/* I2C Control register */
#define I2C_SR_OFF			0x0C	/* I2C Status register */
#define I2C_DIO_OFF			0x10	/* I2C data I/O register */
#define I2C_ICR_OFF			0x20	/* I2C Interrupt Control register */
	
#define I2C_BASE_ADRS		(MBAR_VALUE + MBAR_I2C)
#define I2C_BASE(num)		((UINT32) (I2C_BASE_ADRS + (num*I2C_ADRS_OFF)))

#define I2C_ADR(num)		((volatile UINT32 *) (I2C_BASE(num) + I2C_ADR_OFF))
#define I2C_FDR(num)		((volatile UINT32 *) (I2C_BASE(num) + I2C_FDR_OFF))
#define I2C_CR(num)			((volatile UINT32 *) (I2C_BASE(num) + I2C_CR_OFF))
#define I2C_SR(num)			((volatile UINT32 *) (I2C_BASE(num) + I2C_SR_OFF))
#define I2C_DIO(num)		((volatile UINT32 *) (I2C_BASE(num) + I2C_DIO_OFF))

#define I2C_ICR				((volatile UINT32 *) (I2C_BASE_ADRS + I2C_ICR_OFF))



/*----------------------------------------------------------- */
/* GPIO_STD 										   0x0B00 */
/*----------------------------------------------------------- */

/* MBAR to GPIO_STD */
#define MBAR_GPIO_STD      0x0B00

#define GPIO_STD_BASE_ADRS	(MBAR_VALUE + MBAR_GPIO_STD)

/* Offset GPIO_STD to register */

#define GPIO_STD_PORT_CONFIG	0x00  /* long */
	
#define GPIO_PCR_OFF			0x00  /* GPIO Port Configuration Register */
#define GPIO_SEN_OFF			0x04  /* GPIO Enable Register */
#define GPIO_SOD_OFF			0x08  /* GPIO Open Drain Type Register */
#define GPIO_SDD_OFF			0x0c  /* GPIO Data Direction Register */
#define GPIO_SDO_OFF			0x10  /* GPIO Data Output Register */
#define GPIO_SDI_OFF			0x14  /* GPIO Data Input Register */
#define GPIO_OOE_OFF			0x18  /* GPIO Output Only Enable Register */
#define GPIO_ODO_OFF			0x1c  /* GPIO Output Data Register Out Register */
#define GPIO_SIE_OFF			0x20  /* GPIO Simple Interrupt Enable Register */
#define GPIO_SIOD_OFF			0x24  /* GPIO Simple Interrupt Open Drain Emulation Register */
#define GPIO_SIDD_OFF			0x28  /* GPIO Simple Interrupt Data Direction Register */
#define GPIO_SIDO_OFF			0x2c  /* GPIO Simple Interrupt Data Out Register */
#define GPIO_SIIE_OFF			0x30  /* GPIO Simple Interrupt Enable Register */
#define GPIO_SIIT_OFF			0x34  /* GPIO Simple Interrupt Type Register */
#define GPIO_SIME_OFF			0x38  /* GPIO Simple Interrupt Master Enable Register */
#define GPIO_SIST_OFF			0x3c  /* GPIO Simple Interrupt Status Register */
			
	

#define GPIO_PCR				((volatile UINT32 *) (GPIO_STD_BASE_ADRS + GPIO_PCR_OFF))
#define GPIO_SEN				((volatile UINT32 *) (GPIO_STD_BASE_ADRS + GPIO_SEN_OFF))
#define GPIO_SOD				((volatile UINT32 *) (GPIO_STD_BASE_ADRS + GPIO_SOD_OFF))
#define GPIO_SDD				((volatile UINT32 *) (GPIO_STD_BASE_ADRS + GPIO_SDD_OFF))
#define GPIO_SDO				((volatile UINT32 *) (GPIO_STD_BASE_ADRS + GPIO_SDO_OFF))
#define GPIO_SDI				((volatile UINT32 *) (GPIO_STD_BASE_ADRS + GPIO_SDI_OFF))
#define GPIO_OOE				((volatile UINT32 *) (GPIO_STD_BASE_ADRS + GPIO_OOE_OFF))
#define GPIO_ODO				((volatile UINT32 *) (GPIO_STD_BASE_ADRS + GPIO_ODO_OFF))
#define GPIO_SIE				((volatile UINT32 *) (GPIO_STD_BASE_ADRS + GPIO_SIE_OFF))
#define GPIO_SIOD				((volatile UINT32 *) (GPIO_STD_BASE_ADRS + GPIO_SIOD_OFF))
#define GPIO_SIDD				((volatile UINT32 *) (GPIO_STD_BASE_ADRS + GPIO_SIDD_OFF))
#define GPIO_SIDO				((volatile UINT32 *) (GPIO_STD_BASE_ADRS + GPIO_SIDO_OFF))
#define GPIO_SIIE				((volatile UINT32 *) (GPIO_STD_BASE_ADRS + GPIO_SIIE_OFF))
#define GPIO_SIIT				((volatile UINT32 *) (GPIO_STD_BASE_ADRS + GPIO_SIIT_OFF))
#define GPIO_SIME				((volatile UINT32 *) (GPIO_STD_BASE_ADRS + GPIO_SIME_OFF))
#define GPIO_SIST				((volatile UINT32 *) (GPIO_STD_BASE_ADRS + GPIO_SIST_OFF))
	
			

#define GPIO_STD_PC_ETH_MASK     0x000F0000
#define GPIO_STD_PC_ETH_18WIRE   0x00050000

#define GPIO_STD_PC_PCI_MASK     0x00008000
#define GPIO_STD_PC_PCI_ENABLED  0x00000000
#define GPIO_STD_PC_PCI_DISABLED 0x00008000

#define GPIO_STD_PORTCFG		((volatile UINT32 *) (GPIO_STD_BASE_ADRS + GPIO_STD_PORT_CONFIG))

#define GPIO_STD_PC_USB_MASK     0x00007000
#define GPIO_STD_PC_USB_1_ONLY   0x00001000
#define GPIO_STD_PC_USB_2_ETH    0x00011000
#define GPIO_STD_PC_USB_2_PSC3   0x00010100

#define GPIO_STD_PC_PSC3_SPI	 0x00000800

#define GPIO_STD_PC_USB_SE_MODE  0x00004000
#define GPIO_STD_PC_USB_DM_MODE  0x00000000

/* i=0: PSC1; i=1: PSC2, i=3: PSC2, i=6: PSC6 */

#define PSC1_PORT	0
#define PSC2_PORT	1
#define PSC3_PORT	2
#define PSC4_PORT	3
#define PSC5_PORT	4

/* 
 * The PSC address offset is PSC1 = MBAR + 0x2000 PSC2 = MBAR + 0x2200 
 * PSC3 = MBAR + 0x2400 PSC4 = MBAR + 0x2600 PSC5 = MBAR + 0x2800. But   
 * PSC6 = MBAR + 0x2C00. So we define the PSC6_PORT is 6 not 5.
 */

#define PSC6_PORT	6

#define GPIO_STD_PC_PSC_SHIFT(i,v) ((v)<<(4*i))
#define GPIO_STD_PC_PSC_MASK(i)    GPIO_STD_PC_PSC_SHIFT(i,0xF)
#define GPIO_STD_PC_PSC_UART(i)    GPIO_STD_PC_PSC_SHIFT(i,0x4)
#define GPIO_STD_PC_PSC_UART_CD(i) GPIO_STD_PC_PSC_SHIFT(i,0x5)

#define GPIO_STD_PC_PSC6_MASK    0x00700000	/* bits 9-11 */
#define GPIO_STD_PC_PSC6_UART    0x00500000	/* 101b = IrDa/UART */

/*----------------------------------------------------------- */
/* GPIO_WKUP										   0x0C00 */
/*----------------------------------------------------------- */

/* MBAR to GPIO_WKUP */
#define MBAR_GPIO_WKUP     0x0C00

#define GPIO_WKUP_BASE_ADRS	(MBAR_VALUE + MBAR_GPIO_WKUP)

/* Offset GPIO_WKUP to register */

#define GPIO_WKUP_WE  0x00  /* long, write enable */
#define GPIO_WKUP_OD  0x04  /* long, open drain */
#define GPIO_WKUP_DD  0x08  /* long, data direction */
#define GPIO_WKUP_DO  0x0C  /* long, data value out */
#define GPIO_WKUP_IE  0x10  /* long, interrupt enable */
#define GPIO_WKUP_SIE 0x14  /* long, simple interrupt enable */
#define GPIO_WKUP_IT  0x18  /* long, interrupt type */
#define GPIO_WKUP_ME  0x1C  /* long, data value outmaster enable */
#define GPIO_WKUP_DI  0x20  /* long, data input */
#define GPIO_WKUP_SR  0x24  /* long, sataus register */


#define GPW_WE			((volatile UINT32 *) (GPIO_WKUP_BASE_ADRS + GPIO_WKUP_WE))
#define GPW_OD			((volatile UINT32 *) (GPIO_WKUP_BASE_ADRS + GPIO_WKUP_OD))
#define GPW_DD			((volatile UINT32 *) (GPIO_WKUP_BASE_ADRS + GPIO_WKUP_DD))
#define GPW_DO			((volatile UINT32 *) (GPIO_WKUP_BASE_ADRS + GPIO_WKUP_DO))
#define GPW_IE			((volatile UINT32 *) (GPIO_WKUP_BASE_ADRS + GPIO_WKUP_IE))
#define GPW_SIE			((volatile UINT32 *) (GPIO_WKUP_BASE_ADRS + GPIO_WKUP_SIE))
#define GPW_IT			((volatile UINT32 *) (GPIO_WKUP_BASE_ADRS + GPIO_WKUP_IT))
#define GPW_ME			((volatile UINT32 *) (GPIO_WKUP_BASE_ADRS + GPIO_WKUP_ME))
#define GPW_DI			((volatile UINT32 *) (GPIO_WKUP_BASE_ADRS + GPIO_WKUP_DI))
#define GPW_SR			((volatile UINT32 *) (GPIO_WKUP_BASE_ADRS + GPIO_WKUP_SR))


/*----------------------------------------------------------- */
/* PCI												   0x0D00 */
/*----------------------------------------------------------- */

#define MBAR_PCI		0xD00

#define PCI_BASE_ADRS	(MBAR_VALUE + MBAR_PCI)

/* Type 0 Configuration Header */
#define PCI_SCR_OFF		0x04 /* Status/Command register */
#define PCI_CR1_OFF		0x0c /* Configuration  1 register */
#define PCI_BAR0_OFF	0x10 /* Base address register 0 */
#define PCI_BAR1_OFF	0x14 /* Base address register 1 */
#define PCI_ICR2_OFF    0x3C /* Configuration 2 register */

/* General Control/Status */
#define PCI_GSCR_OFF	0x60 /* Global Status/Control Register */
#define PCI_TBATR0_OFF	0x64 /* Target Base Address Translation Register 0 */
#define PCI_TBATR1_OFF	0x68 /* Target Base Address Translation Register 1 */
#define PCI_TCBR_OFF	0x6C /* Target Control Register */
#define PCI_IW0BTAR_OFF	0x70 /* Initiator Window 0 Base/Translation Addr Reg */
#define PCI_IW1BTAR_OFF	0x74 /* Initiator Window 1 Base/Translation Addr Reg */
#define PCI_IW2BTAR_OFF	0x78 /* Initiator Window 2 Base/Translation Addr Reg */
#define PCI_IWCR_OFF	0x80 /* Initiator Window Configuration Register */
#define PCI_ICR_OFF		0x84 /* Initiator Control Register */
#define PCI_ISR_OFF		0x88 /* Initiator Status Register */
#define PCI_ARB_OFF		0x8C /* PCI Arbiter Register */
#define PCI_CAR_OFF		0xF8 /* Configuration Address Register */

#define PCISCR		((volatile UINT32 *)(PCI_BASE_ADRS + PCI_SCR_OFF))
#define PCICR1		((volatile UINT32 *)(PCI_BASE_ADRS + PCI_CR1_OFF))
#define PCIBAR0		((volatile UINT32 *)(PCI_BASE_ADRS + PCI_BAR0_OFF))
#define PCIBAR1		((volatile UINT32 *)(PCI_BASE_ADRS + PCI_BAR1_OFF))
#define PCICR2      ((volatile UINT32 *)(PCI_BASE_ADRS + PCI_ICR2_OFF))
#define PCIGSCR		((volatile UINT32 *)(PCI_BASE_ADRS + PCI_GSCR_OFF))
#define PCITBATR0	((volatile UINT32 *)(PCI_BASE_ADRS + PCI_TBATR0_OFF))
#define PCITBATR1	((volatile UINT32 *)(PCI_BASE_ADRS + PCI_TBATR1_OFF))
#define PCITCBR		((volatile UINT32 *)(PCI_BASE_ADRS + PCI_TCBR_OFF))
#define PCIIW0BTAR	((volatile UINT32 *)(PCI_BASE_ADRS + PCI_IW0BTAR_OFF))
#define PCIIW1BTAR	((volatile UINT32 *)(PCI_BASE_ADRS + PCI_IW1BTAR_OFF))
#define PCIIW2BTAR	((volatile UINT32 *)(PCI_BASE_ADRS + PCI_IW2BTAR_OFF))
#define PCIIWCR		((volatile UINT32 *)(PCI_BASE_ADRS + PCI_IWCR_OFF))
#define PCIICR		((volatile UINT32 *)(PCI_BASE_ADRS + PCI_ICR_OFF))
#define PCIISR		((volatile UINT32 *)(PCI_BASE_ADRS + PCI_ISR_OFF))
#define PCIARB		((volatile UINT32 *)(PCI_BASE_ADRS + PCI_ARB_OFF))
#define PCICAR		((volatile UINT32 *)(PCI_BASE_ADRS + PCI_CAR_OFF))

/* Command/Status register */
#define PCISCR_B				0x00000004	/* bus master enable */
#define PCISCR_M				0x00000002	/* memory access control */
#define PCISCR_MW				0x00000010	/* memory write and invalidate enable */

/* Cconfiguration 1 register */
#define PCICR1_LAT_MASK		    0x0000ff00
#define PCICR1_LAT_SHIFT		8

/* Configuration 2 register */
#define PCICR2_MAX_LAT          0xFF
#define PCICR2_MIN_GNT          0xFF

/* Global Status/Control Register */
#define PCIGSCR_BM				0x40000000
#define PCIGSCR_PE				0x20000000
#define PCIGSCR_SE				0x10000000
#define PCIGSCR_BME				0x00004000
#define PCIGSCR_PEE				0x00002000
#define PCIGSCR_SEE				0x00001000

/* IW Conf reg */

#define PCIIWCR_ENABLE			0x1
#define PCIIWCR_MEM				0x0
#define PCIIWCR_IO				0x8

#define PCIIWCR_READ_LINE		0x2
#define PCIIWCR_READ_MULTI		0x4
#define PCIIWCR_READ_SINGLE		0x0

/* Initiator Control Register */
#define PCIICR_REE				0x04000000
#define PCIICR_IAE				0x02000000
#define PCIICR_TAE				0x01000000

/* Initiator Status Register */
#define PCIISR_RE				0x04000000
#define PCIISR_IA				0x02000000
#define PCIISR_TA				0x01000000

/* PCI Arbiter Register */
#define PCIARB_RESET			0x01000000

/*-----------------------------------------------------------*/
/* USB 											      0x1000 */
/*-----------------------------------------------------------*/

#define MBAR_USB			0x1000

#define USB_BASE_ADRS		(MBAR_VALUE + MBAR_USB)

/*-----------------------------------------------------------*/
/* SMDA 											  0x1200 */
/*-----------------------------------------------------------*/

#define MBAR_SDMA			0x1200

#define SDMA_BASE_ADRS		(MBAR_VALUE + MBAR_SDMA)

/*-----------------------------------------------------------*/
/* XLB												  0x1F00 */
/*-----------------------------------------------------------*/

#define MBAR_XLB		0x1F00

#define XLB_CR_OFF		0x40 /* arbiter configuration register */
#define XLB_VR_OFF		0x44 /* arbiter version register */
#define XLB_SR_OFF		0x48 /* arbiter status register */
#define XLB_IER_OFF		0x4C /* arbiter interrupt enable register */
#define XLB_ACR_OFF		0x50 /* arbiter address capture register */
#define XLB_BSCR_OFF	0x54 /* arbiter bus signal capture register */
#define XLB_ATTOR_OFF	0x58 /* arbiter addr tenure timeout register */
#define XLB_DTTOR_OFF	0x5C /* arbiter data tenure timeout register */
#define XLB_BATOR_OFF	0x60 /* arbiter bus activity timeout register */
#define XLB_MPER_OFF	0x64 /* arbiter master prio enable register */
#define XLB_MPR_OFF		0x68 /* arbiter master prio register */
#define XLB_BAR_OFF		0x6C /* arbiter base address register */
#define XLB_SWR_OFF		0x70 /* arbiter snoop window register */

#define XLB_BASE_ADRS	(MBAR_VALUE + MBAR_XLB)

#define XLB_CR			((volatile UINT32 *)(XLB_BASE_ADRS + XLB_CR_OFF))
#define XLB_SR			((volatile UINT32 *)(XLB_BASE_ADRS + XLB_SR_OFF))
#define XLB_IER	   		((volatile UINT32 *)(XLB_BASE_ADRS + XLB_IER_OFF))
#define XLB_ACR			((volatile UINT32 *)(XLB_BASE_ADRS + XLB_ACR_OFF))
#define XLB_BSCR		((volatile UINT32 *)(XLB_BASE_ADRS + XLB_BSCR_OFF))
#define XLB_ATTOR		((volatile UINT32 *)(XLB_BASE_ADRS + XLB_ATTOR_OFF))
#define XLB_DTTOR		((volatile UINT32 *)(XLB_BASE_ADRS + XLB_DTTOR_OFF))
#define XLB_BATOR		((volatile UINT32 *)(XLB_BASE_ADRS + XLB_BATOR_OFF))
#define XLB_MPER		((volatile UINT32 *)(XLB_BASE_ADRS + XLB_MPER_OFF))
#define XLB_MPR			((volatile UINT32 *)(XLB_BASE_ADRS + XLB_MPR_OFF))
#define XLB_BAR			((volatile UINT32 *)(XLB_BASE_ADRS + XLB_BAR_OFF))
#define XLB_SWR			((volatile UINT32 *)(XLB_BASE_ADRS + XLB_SWR_OFF))

 
/* arbiter configuration register */

#define XLB_CR_PLDIS_MASK		0x80000000	/* pipeline disabled */
#define XLB_CR_PLDIS_DISABLED	0x80000000
#define XLB_CR_PLDIS_ENABLED	0x00000000
#define XLB_CR_BLDIS_MASK       0x00010000	/* pipeline disabled */
#define XLB_CR_BLDIS_DISABLED   0x00010000
#define XLB_CR_BLDIS_ENABLED    0x00000000
#define XLB_CR_SE_MASK			0x00008000	/* snoop enabled */
#define XLB_CR_SE_DISABLED		0x00000000
#define XLB_CR_SE_ENABLED		0x00008000
#define XLB_CR_USE_WWF_MASK		0x00004000	/* force write-write-flush */
#define XLB_CR_USE_WWF_DISABLED	0x00000000
#define XLB_CR_USE_WWF_ENABLED	0x00004000
#define XLB_CR_TBEN_MASK		0x00002000	/* timebase enable */
#define XLB_CR_TBEN_DISABLED	0x00000000
#define XLB_CR_TBEN_ENABLED		0x00002000
#define XLB_CR_WS_MASK			0x00000800	/* minimum wait state */
#define XLB_CR_WS_0				0x00000000
#define XLB_CR_WS_1				0x00000800
#define XLB_CR_PM_MASK			0x00000060	/* parking mode */
#define XLB_CR_PM_NON			0x00000000
#define XLB_CR_PM_MOST_RECENT	0x00000040
#define XLB_CR_PM_SELECTED		0x00000060
#define XLB_CR_SP_MASK			0x00000700	/* selected parked master */
#define XLB_CR_SP_SHIFT			8
#define XLB_CR_BA_MASK			0x00000008	/* bus activity TO enable */
#define XLB_CR_BA_DISABLED		0x00000000
#define XLB_CR_BA_ENABLED		0x00000008
#define XLB_CR_DT_MASK          0x00000004  /* data tenure TO enable */
#define XLB_CR_DT_DISABLED      0x00000000
#define XLB_CR_DT_ENABLED       0x00000004
#define XLB_CR_AT_MASK          0x00000002  /* address tenure TO enable */
#define XLB_CR_AT_DISABLED      0x00000000
#define XLB_CR_AT_ENABLED       0x00000002

/* arbiter status register */
#define XLB_SR_SEA_MASK			0x00000100	/* slave error ack */
#define XLB_SR_MM_MASK			0x00000080	/* multiple masters */
#define XLB_SR_TTA_MASK			0x00000040	/* TT address only */
#define XLB_SR_TTR_MASK			0x00000020	/* TT reserved */
#define XLB_SR_ECW_MASK			0x00000010	/* external control word R/W */
#define XLB_SR_TTM_MASK			0x00000008	/* TBST/TSIZ mismatch */
#define XLB_SR_BA_MASK			0x00000004	/* bus activity timeout */
#define XLB_SR_DT_MASK			0x00000002	/* data tenure timeout */
#define XLB_SR_AT_MASK			0x00000001	/* address tenure timeout */
#define XLB_SR_CLEAR_ALL		(XLB_SR_ECW_MASK|XLB_SR_DT_MASK|XLB_SR_AT_MASK)

/* arbiter interrupt enable register */
#define XLB_IER_SEAE_MASK		0x00000100	/* slave error ack */
#define XLB_IER_MME_MASK		0x00000080	/* multiple masters */
#define XLB_IER_TTAE_MASK		0x00000040	/* TT address only */
#define XLB_IER_TTRE_MASK		0x00000020	/* TT reserved */
#define XLB_IER_ECWE_MASK		0x00000010	/* external control word R/W */
#define XLB_IER_TTME_MASK		0x00000008	/* TBST/TSIZ mismatch */
#define XLB_IER_BAE_MASK		0x00000004	/* bus activity timeout */
#define XLB_IER_DTE_MASK		0x00000002	/* data tenure timeout */
#define XLB_IER_ATE_MASK		0x00000001	/* address tenure timeout */

/* arbiter bus signal capture register */
#define XLB_BSCR_TSIZ_MASK		0x00000380 	/* captured TSIZ */
#define XLB_BSCR_TSIZ_SHIFT		7
#define XLB_BSCR_GBL_MASK		0x00000040 	/* captured GBL */
#define XLB_BSCR_TBST_MASK		0x00000020 	/* captured TBST */
#define XLB_BSCR_TT_MASK		0x0000001F 	/* captured TT */
#define XLB_BSCR_TT_SHIFT		0

/* arbiter master priority enable register */
#define XLB_MPER_HARDWARE		0
#define XLB_MPER_SOFTWARE		0x0000000F

#define XLB_MPER_PCI			0x00000008
#define XLB_MPER_BESTCOMM		0x00000004
#define XLB_MPER_USB			0x00000002
#define XLB_MPER_CORE			0x00000001

/* arbiter master priority register */
#define XLB_MPER_M7_SHIFT		28
#define XLB_MPER_M6_SHIFT		24
#define XLB_MPER_M5_SHIFT		20
#define XLB_MPER_M4_SHIFT		16
#define XLB_MPER_M3_SHIFT		12
#define XLB_MPER_M2_SHIFT		8
#define XLB_MPER_M1_SHIFT		4
#define XLB_MPER_M0_SHIFT		0

#define XLB_MPR_PCI_SHIFT		XLB_MPER_M3_SHIFT
#define XLB_MPR_BESTCOMM_SHIFT	XLB_MPER_M2_SHIFT
#define XLB_MPR_USB_SHIFT		XLB_MPER_M1_SHIFT
#define XLB_MPR_CORE_SHIFT		XLB_MPER_M0_SHIFT

/*-----------------------------------------------------------*/
/* PSC 												  0x2000 */
/* PSC 												  0x2400 */
/* PSC 												  0x2800 */
/*-----------------------------------------------------------*/

/* Offset MBAR to PSCx */
#define MBAR_PSC1			0x2000
#define MBAR_PSC2			0x2200
#define MBAR_PSC3			0x2400
#define MBAR_PSC4			0x2600
#define MBAR_PSC5			0x2800
#define MBAR_PSC6			0x2C00

#define PSC_NUM_MAX			6		/* number of PSC used controllers */
#define PSC_ADRS_OFF		0x200	/* offset between PSC controllers */



#define PSC1_BASE_ADRS		(MBAR_VALUE + MBAR_PSC1)
#define PSC2_BASE_ADRS		(MBAR_VALUE + MBAR_PSC2)
#define PSC3_BASE_ADRS		(MBAR_VALUE + MBAR_PSC3)
#define PSC6_BASE_ADRS		(MBAR_VALUE + MBAR_PSC6)

/* Offset PSCx to register */
#define PSC_MR			0x00  /* byte */
#define PSC_SR_CSR		0x04  /* short */
#define PSC_CR			0x08  /* byte */
#define PSC_RB_TB		0x0c  /* long */
#define PSC_IPCR_ACR	0x10  /* byte */
#define PSC_ISR_IMR		0x14  /* short */
#define PSC_DP			0x18  /* byte */
#define PSC_DL			0x1c  /* byte */
/* IVR */
/* IP */
#define PSC_OP1			0x38  
/* OP0 */
#define PSC_SICR		0x40  
#define PSC_IRCR1		0x44  
#define PSC_IRCR2		0x48  
#define PSC_IRSDR		0x4c  
#define PSC_IRMDR		0x50  
#define PSC_IRFDR		0x54 
#define PSC_RFNUM		0x58  
#define PSC_TFNUM		0x5c  
#define PSC_RFCNTL		0x68  
#define PSC_RFALARM		0x6e  
#define PSC_RFRPTR		0x72  
#define PSC_RFWPTR		0x76  
#define PSC_TFCNTL		0x88  
#define PSC_TFALARM		0x8e  
#define PSC_TFRPTR		0x92  
#define PSC_TFWPTR		0x96  


#define PSC_BASE(num)			((UINT32) (PSC1_BASE_ADRS + (num*PSC_ADRS_OFF)))

#define PSC_MR_ADR(num)			((volatile UINT32 *) (PSC_BASE(num) + PSC_MR))
#define PSC_SR_CSR_ADR(num)		((volatile UINT32 *) (PSC_BASE(num) + PSC_SR_CSR))
#define PSC_CR_ADR(num)			((volatile UINT32 *) (PSC_BASE(num) + PSC_CR))
#define PSC_RB_TB_ADR(num)		((volatile UINT32 *) (PSC_BASE(num) + PSC_RB_TB))
#define PSC_IPCR_ACR_ADR(num)	((volatile UINT32 *) (PSC_BASE(num) + PSC_IPCR_ACR))
#define PSC_ISR_IMR_ADR(num)	((volatile UINT32 *) (PSC_BASE(num) + PSC_ISR_IMR))
#define PSC_DP_ADR(num)			((volatile UINT32 *) (PSC_BASE(num) + PSC_DP))
#define PSC_DL_ADR(num)			((volatile UINT32 *) (PSC_BASE(num) + PSC_DL))
#define PSC_OP1_ADR(num)		((volatile UINT32 *) (PSC_BASE(num) + PSC_OP1))
#define PSC_SICR_ADR(num)		((volatile UINT32 *) (PSC_BASE(num) + PSC_SICR))
#define PSC_IRCR1_ADR(num)		((volatile UINT32 *) (PSC_BASE(num) + PSC_IRCR1))
#define PSC_IRCR2_ADR(num)		((volatile UINT32 *) (PSC_BASE(num) + PSC_IRCR2))
#define PSC_IRSDR_ADR(num)		((volatile UINT32 *) (PSC_BASE(num) + PSC_IRSDR))
#define PSC_IRMDR_ADR(num)		((volatile UINT32 *) (PSC_BASE(num) + PSC_IRMDR))
#define PSC_IRFDR_ADR(num)		((volatile UINT32 *) (PSC_BASE(num) + PSC_IRFDR))
#define PSC_RFNUM_ADR(num)		((volatile UINT32 *) (PSC_BASE(num) + PSC_RFNUM))
#define PSC_TFNUM_ADR(num)		((volatile UINT32 *) (PSC_BASE(num) + PSC_TFNUM))
#define PSC_RFCNTL_ADR(num)		((volatile UINT32 *) (PSC_BASE(num) + PSC_RFCNTL))
#define PSC_RFALARM_ADR(num)	((volatile UINT32 *) (PSC_BASE(num) + PSC_RFALARM))
#define PSC_RFRPTR_ADR(num)		((volatile UINT32 *) (PSC_BASE(num) + PSC_RFRPTR))
#define PSC_RFWPTR_ADR(num)		((volatile UINT32 *) (PSC_BASE(num) + PSC_RFWPTR))
#define PSC_TFCNTL_ADR(num)		((volatile UINT32 *) (PSC_BASE(num) + PSC_TFCNTL))
#define PSC_TFALARM_ADR(num)	((volatile UINT32 *) (PSC_BASE(num) + PSC_TFALARM))
#define PSC_TFRPTR_ADR(num)		((volatile UINT32 *) (PSC_BASE(num) + PSC_TFRPTR))
#define PSC_TFWPTR_ADR(num)		((volatile UINT32 *) (PSC_BASE(num) + PSC_TFWPTR))


/* Interrupt Status and Mask Register */
#define PSC_ISR_IMR_RxRDY  (1<<9)   /* RxRDY or FFULL */
#define PSC_ISR_IMR_TxRDY  (1<<8)   
#define PSC_ISR_IMR_DEOF   (1<<7)   
#define PSC_ISR_IMR_DB     (1<<10)   
#define PSC_ISR_IMR_IPC    (1<<15)

/* register bit definitions */

/* MR1 - mode register 1 */
#define PSC_UART_MR1_BC0	(1 << 0)	/* bits per char 0 */
#define PSC_UART_MR1_BC1	(1 << 1)	/* bits per char 1 */
#define PSC_UART_MR1_PT		(1 << 2)	/* parity type */
#define PSC_UART_MR1_PM0	(1 << 3)	/* parity mode 0 */
#define PSC_UART_MR1_PM1	(1 << 4)	/* parity mode 1 */
#define PSC_UART_MR1_ERR	(1 << 5)	/* error mode */
#define PSC_UART_MR1_RX_IRQ	(1 << 6)	/* receiver interrupt select */
#define PSC_UART_MR1_RX_RTS	(1 << 7)	/* Rx RTS control */

/* Some common modes */
#define PSC_UART_MR1_BITS_CHAR_5	0
#define PSC_UART_MR1_BITS_CHAR_6	PSC_UART_MR1_BC0
#define PSC_UART_MR1_BITS_CHAR_7	PSC_UART_MR1_BC1
#define PSC_UART_MR1_BITS_CHAR_8	(PSC_UART_MR1_BC0 \
					 | PSC_UART_MR1_BC1)
#define PSC_UART_MR1_NO_PARITY	(PSC_UART_MR1_PM1)
#define PSC_UART_MR1_PAR_MODE_MULTI	(PSC_UART_MR1_PM0 \
					 | PSC_UART_MR1_PM1)
#define PSC_UART_MR1_EVEN_PARITY	0
#define PSC_UART_MR1_ODD_PARITY	PSC_UART_MR1_PT

/* MR2 - mode register 2 */
#define PSC_UART_MR2_SB0	(1 << 0)	/* stop bit control 0 */
#define PSC_UART_MR2_SB1	(1 << 1)	/* stop bit control 1 */
#define PSC_UART_MR2_SB2	(1 << 2)	/* stop bit control 2 */
#define PSC_UART_MR2_SB3	(1 << 3)	/* stop bit control 3 */
#define PSC_UART_MR2_TX_CTS	(1 << 4)	/* Tx CTS control */
#define PSC_UART_MR2_TX_RTS	(1 << 5)	/* Tx RTS control */
#define PSC_UART_MR2_CM0	(1 << 6)	/* channel mode 0 */
#define PSC_UART_MR2_CM1	(1 << 7)	/* channel mode 1 */

/* Some common bit lengths */
#define PSC_UART_MR2_STOP_BITS_1	(PSC_UART_MR2_SB0 \
					 | PSC_UART_MR2_SB1 \
					 | PSC_UART_MR2_SB2)
#define PSC_UART_MR2_STOP_BITS_2	(PSC_UART_MR2_SB0 \
					 | PSC_UART_MR2_SB1 \
					 | PSC_UART_MR2_SB2 \
					 | PSC_UART_MR2_SB3)

/* SR - status register */
#define PSC_UART_SR_CDE		(1 << 7)	/* DCD error */
#define PSC_UART_SR_RXRDY	(1 << 8)	/* Rx ready */
#define PSC_UART_SR_FFULL	(1 << 9)	/* FIFO full */
#define PSC_UART_SR_TXRDY	(1 << 10)	/* Tx ready */
#define PSC_UART_SR_TXEMP	(1 << 11)	/* Tx empty */
#define PSC_UART_SR_OE		(1 << 12)	/* overrun error */
#define PSC_UART_SR_PE		(1 << 13)	/* parity error */
#define PSC_UART_SR_FE		(1 << 14)	/* framing error */
#define PSC_UART_SR_RB		(1 << 15)	/* received break */

/* CSR - clock select register */
#define PSC_UART_CSR_TCS0	(1 << 8)	/* Tx clock select 0 */
#define PSC_UART_CSR_TCS1	(1 << 9)	/* Tx clock select 1 */
#define PSC_UART_CSR_TCS2	(1 << 10)	/* Tx clock select 2 */
#define PSC_UART_CSR_TCS3	(1 << 11)	/* Tx clock select 3 */
#define PSC_UART_CSR_RCS0	(1 << 12)	/* Rx clock select 0 */
#define PSC_UART_CSR_RCS1	(1 << 13)	/* Rx clock select 1 */
#define PSC_UART_CSR_RCS2	(1 << 14)	/* Rx clock select 2 */
#define PSC_UART_CSR_RCS3	(1 << 15)	/* Rx clock select 3 */

#define PSC_UART_CSR_TIMER_TX	(PSC_UART_CSR_TCS0 \
				 | PSC_UART_CSR_TCS2 \
				 | PSC_UART_CSR_TCS3)
#define PSC_UART_CSR_TIMER_RX	(PSC_UART_CSR_RCS0 \
				 | PSC_UART_CSR_RCS2 \
				 | PSC_UART_CSR_RCS3)

/* CR - command register */
#define PSC_UART_CR_RC0		(1 << 0)	/* receiver control bit 0 */
#define PSC_UART_CR_RC1		(1 << 1)	/* receiver control bit 1 */
#define PSC_UART_CR_TC0		(1 << 2)	/* transmitter control bit 0 */
#define PSC_UART_CR_TC1		(1 << 3)	/* transmitter control bit 1 */
#define PSC_UART_CR_MISC0	(1 << 4)	/* misc control bit 0 */
#define PSC_UART_CR_MISC1	(1 << 5)	/* misc control bit 1 */
#define PSC_UART_CR_MISC2	(1 << 6)	/* misc control bit 2 */

/* Some common commands */
#define PSC_UART_CR_TX_ENABLE		(PSC_UART_CR_TC0)
#define PSC_UART_CR_TX_DISABLE		(PSC_UART_CR_TC1)
#define PSC_UART_CR_RX_ENABLE		(PSC_UART_CR_RC0)
#define PSC_UART_CR_RX_DISABLE		(PSC_UART_CR_RC1)
#define PSC_UART_CR_RESET_MODE_PTR	(PSC_UART_CR_MISC0)
#define PSC_UART_CR_RESET_RX		(PSC_UART_CR_MISC1)
#define PSC_UART_CR_RESET_TX		(PSC_UART_CR_MISC0 \
									| PSC_UART_CR_MISC1)
#define PSC_UART_CR_RESET_ERR		(PSC_UART_CR_MISC2)
#define PSC_UART_CR_RESET_BRK		(PSC_UART_CR_MISC0 \
									| PSC_UART_CR_MISC2)

/* ACR - auxiliary control register */
#define PSC_UART_ACR_IEC	(1 << 0)	/* input enable control */

/* IMR - interrupt mask register */
#define PSC_UART_IMR_TXRDY	(1 << 8)	/* transmitter ready */
#define PSC_UART_IMR_RXRDY	(1 << 9)	/* receiver ready */
#define PSC_UART_IMR_DB		(1 << 10)	/* delta break */
#define PSC_UART_IMR_COS	(1 << 15)	/* change of CTS state */

/* ISR - interrupt status register */
#define PSC_UART_ISR_TXRDY	(1 << 8)	/* transmitter ready */
#define PSC_UART_ISR_RXRDY	(1 << 9)	/* receiver ready */
#define PSC_UART_ISR_DB		(1 << 10)	/* delta break */
#define PSC_UART_ISR_COS	(1 << 15)	/* change of CTS state */

/* IP - input port register */
#define PSC_UART_IP_CTS		(1 << 0)	/* current CTS state */

/* OP1 - output set register */
#define PSC_UART_OP1_RTS	(1 << 0)	/* set RTS */

/* OP2 - output reset register */
#define PSC_UART_OP2_RTS	(1 << 0)	/* clear RTS */

/*-----------------------------------------------------------*/
/* FEC (see m5200FecEnd.h) 							 0x03000 */
/*-----------------------------------------------------------*/

#define MBAR_ETH				0x3000

#define ETH_BASE_ADRS			((UINT32)MBAR_VALUE + MBAR_ETH)

#define FEC_EVENT_OFF       0x0004  /* interrupt event register */
#define FEC_MASK_OFF        0x0008  /* interrupt mask register */
#define FEC_CTRL_OFF        0x0024  /* FEC control register */
#define FEC_MII_SPEED_OFF   0x0044  /* MII speed register */
#define FEC_RX_CTRL_OFF     0x0084  /* rx control register */
#define FEC_TX_CTRL_OFF     0x00c4  /* tx control register */
#define FEC_ADDR_L_OFF      0x00e4  /* lower 32-bits of MAC address */
#define FEC_ADDR_H_OFF      0x00e8  /* upper 16-bits of MAC address */
#define FEC_HASH_H_OFF      0x0118  /* upper 32-bits of hash table */
#define FEC_HASH_L_OFF      0x011c  /* lower 32-bits of hash table */
#define FEC_DGR1_OFF		0x0120  
#define FEC_DGR2_OFF		0x0124  
#define FEC_XWMRK_OFF      	0x144
#define FEC_RFCNTRL_OFF		0x18C
#define FEC_RFLRFPTR_OFF	0x190
#define FEC_RFLWFPTR_OFF	0x194
#define FEC_RFALARM_OFF		0x198
#define FEC_RFRPTR_OFF		0x19C
#define FEC_RFWPTR_OFF		0x1A0
#define FEC_TFCNTRL_OFF		0x1AC
#define FEC_TFLRFPTR_OFF	0x1B0
#define FEC_TFLWFPTR_OFF	0x1B4
#define FEC_TFALARM_OFF		0x1B8
#define FEC_TFRPTR_OFF		0x1BC
#define FEC_TFWPTR_OFF		0x1C0
#define FEC_XMITFSM_OFF		0x1C8


#define FEC_EVENT			((volatile UINT32 *)(ETH_BASE_ADRS + FEC_EVENT_OFF))
#define FEC_CTRL			((volatile UINT32 *)(ETH_BASE_ADRS + FEC_CTRL_OFF))
#define FEC_MII_SPEED		((volatile UINT32 *)(ETH_BASE_ADRS + FEC_MII_SPEED_OFF))
#define FEC_RX_CTRL			((volatile UINT32 *)(ETH_BASE_ADRS + FEC_RX_CTRL_OFF))
#define FEC_TX_CTRL			((volatile UINT32 *)(ETH_BASE_ADRS + FEC_TX_CTRL_OFF))
#define FEC_ADDR_L			((volatile UINT32 *)(ETH_BASE_ADRS + FEC_ADDR_L_OFF))
#define FEC_ADDR_H			((volatile UINT32 *)(ETH_BASE_ADRS + FEC_ADDR_H_OFF))
#define FEC_HASH_L			((volatile UINT32 *)(ETH_BASE_ADRS + FEC_HASH_L_OFF))
#define FEC_HASH_H			((volatile UINT32 *)(ETH_BASE_ADRS + FEC_HASH_H_OFF))
#define FEC_DGR1			((volatile UINT32 *)(ETH_BASE_ADRS + FEC_DGR1_OFF))
#define FEC_DGR2			((volatile UINT32 *)(ETH_BASE_ADRS + FEC_DGR2_OFF))
#define FEC_XWMRK			((volatile UINT32 *)(ETH_BASE_ADRS + FEC_XWMRK_OFF))
#define FEC_RFCNTRL			((volatile UINT32 *)(ETH_BASE_ADRS + FEC_RFCNTRL_OFF))
#define FEC_RFLRFPTR		((volatile UINT32 *)(ETH_BASE_ADRS + FEC_RFLRFPTR_OFF))
#define FEC_RFLWFPTR		((volatile UINT32 *)(ETH_BASE_ADRS + FEC_RFLWFPTR_OFF))
#define FEC_RFALARM			((volatile UINT32 *)(ETH_BASE_ADRS + FEC_RFALARM_OFF))
#define FEC_RFRPTR			((volatile UINT32 *)(ETH_BASE_ADRS + FEC_RFRPTR_OFF))
#define FEC_RFWPTR			((volatile UINT32 *)(ETH_BASE_ADRS + FEC_RFWPTR_OFF))
#define FEC_TFCNTRL			((volatile UINT32 *)(ETH_BASE_ADRS + FEC_TFCNTRL_OFF))
#define FEC_TFLRFPTR		((volatile UINT32 *)(ETH_BASE_ADRS + FEC_TFLRFPTR_OFF))
#define FEC_TFLWFPTR		((volatile UINT32 *)(ETH_BASE_ADRS + FEC_TFLWFPTR_OFF))
#define FEC_TFALARM			((volatile UINT32 *)(ETH_BASE_ADRS + FEC_TFALARM_OFF))
#define FEC_TFRPTR			((volatile UINT32 *)(ETH_BASE_ADRS + FEC_TFRPTR_OFF))
#define FEC_TFWPTR			((volatile UINT32 *)(ETH_BASE_ADRS + FEC_TFWPTR_OFF))
#define FEC_XMITFSM			((volatile UINT32 *)(ETH_BASE_ADRS + FEC_XMITFSM_OFF))

/*-----------------------------------------------------------*/
/* ATA 											  0x8000 */
/*-----------------------------------------------------------*/

#define MBAR_ATA           0x3A00      
#define ATA_BASE_ADRS			((UINT32)MBAR_VALUE + MBAR_ATA)
#define ATA_BASE_ADRS0     (ATA_BASE_ADRS + 0x60)
#define ATA_BASE_ADRS1     (ATA_BASE_ADRS + 0x5C)

#define ATA_HOST_REG_OFF        0x08        /* ata host register offset */
#define ATA_PIO_TMG1_OFF        0x08        /* ata Pio Timing 1 register offset */
#define ATA_PIO_TMG2_OFF        0x0C        /* ata Pio Timing 2 register offset */
#define ATA_UNPRG_OFF           0x2C        /* ata Unimplemented register */

#define ATA_HOST_REG			((volatile UINT32 *)(ATA_BASE_ADRS + ATA_HOST_REG_OFF))
#define ATA_PIO_TMG1			((volatile UINT32 *)(ATA_BASE_ADRS + ATA_PIO_TMG1_OFF))
#define ATA_PIO_TMG2			((volatile UINT32 *)(ATA_BASE_ADRS + ATA_PIO_TMG2_OFF))
#define ATA_UNPRG				((volatile UINT32 *)(ATA_BASE_ADRS + ATA_UNPRG_OFF))

/*-----------------------------------------------------------*/
/* SRAM 											  0x8000 */
/*-----------------------------------------------------------*/

#define MBAR_SRAM				0x8000

#define SRAM_BASE_ADRS			(MBAR_VALUE + MBAR_SRAM)
#define SRAM_SIZE				(16*1024)
#define SRAM_ADRS(num)			((volatile UINT32*) (SRAM_BASE_ADRS + num))

/*-----------------------------------------------------------*/
/* SPI 						      0x0F00 */
/*-----------------------------------------------------------*/

#define MBAR_SPI      0x0F00

#define SPI_BASE_ADRS	(MBAR_VALUE + MBAR_SPI)

#define SPI_CR1_OFF	0x0
#define SPI_CR2_OFF	0x4
#define SPI_BRR_OFF	0x4
#define SPI_SR_OFF	0x5
#define SPI_DR_OFF	0x9
#define SPI_PDR_OFF	0xd
#define SPI_DDR_OFF	0x10

#define SPI_CR1		((volatile UINT8 *) (SPI_BASE_ADRS + SPI_CR1_OFF))
#define SPI_CR2		((volatile UINT8 *) (SPI_BASE_ADRS + SPI_CR2_OFF))
#define SPI_BRR		((volatile UINT8 *) (SPI_BASE_ADRS + SPI_BRR_OFF))
#define SPI_SR		((volatile UINT8 *) (SPI_BASE_ADRS + SPI_SR_OFF))
#define SPI_DR		((volatile UINT8 *) (SPI_BASE_ADRS + SPI_DR_OFF))
#define SPI_PDR		((volatile UINT8 *) (SPI_BASE_ADRS + SPI_PDR_OFF))
#define SPI_DDR		((volatile UINT8 *) (SPI_BASE_ADRS + SPI_DDR_OFF))

/* The following are miscellaneous constants used by SPI Control Register 1 */
#define SPI_CR1_SPIE            0x80
#define SPI_CR1_SPE             0x40
#define SPI_CR1_SPWOM           0x20
#define SPI_CR1_MASTER_MODE     0x10
#define SPI_CR1_CPOL			0x08
#define SPI_CR1_CPHA			0x04
#define SPI_CR1_SSOE			0x02
#define SPI_CR1_LSBFE			0x01

/* The following are miscellaneous constants used by SPI Status Register */
#define SPI_SR_SPIF             0x80
#define SPI_SR_WCOL             0x40
#define SPI_SR_MODF             0x10

#ifdef __cplusplus
}
#endif

#endif /* __INCppc5200h */
