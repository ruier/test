/* sysFslSpi.h - FreeScale PowerPC eSPI driver header file */

/*
 * Copyright (c) 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,10jun09,syt  written
*/

#ifndef __INCfslSpih
#define __INCfslSpih

#ifdef __cplusplus
extern "C" {
#endif

#include <semLib.h>

/* SPI registers, fields and masks */

/* SPI register offsets */

#define SPMODE              0x00   /* eSPI mode register */
#define SPIE                0x04   /* eSPI event register */
#define SPIM                0x08   /* eSPI mask register */
#define SPCOM               0x0C   /* eSPI command register */
#define SPITF               0x10   /* eSPI transmit FIFO access register*/
#define SPIRF               0x14   /* eSPI receive FIFO access register */
#define SPMODE0             0x20   /* eSPI CS0 mode register */
#define SPMODE1             0x24   /* eSPI CS1 mode register */
#define SPMODE2             0x28   /* eSPI CS2 mode register */
#define SPMODE3             0x2C   /* eSPI CS3 mode register */

/* bit fields */

/* eSPI Mode Register bits */

#define SPI_EN_BIT          0
#define SPI_EN              (0x01 << (31 - SPI_EN_BIT))
#define SPI_LOOP_BIT        1
#define SPI_LOOP            (0x01 << (31 - SPI_LOOP))
#define SPI_TXTHR_BIT       23
#define SPI_TXTHR_LEN       (0x20 << (31 - SPI_TXTHR_BIT))
#define SPI_RXTHR_BIT       31
#define SPI_RXTHR_LEN       (0x1f << (31 - SPI_RXTHR_BIT))

/* eSPI Event Register bits */

#define RXCNT               7
#define TXCNT               15
#define TXE_BIT             16
#define TXE                 (0x01 << (31 - TXE_BIT))
#define DON_BIT             17
#define DON                 (0x01 << (31 - DON_BIT))
#define RXT_BIT             18
#define RXT                 (0x01 << (31 - RXT_BIT))
#define RXF_BIT             19
#define RXF                 (0x01 << (31 - RXF_BIT))
#define TXT_BIT             20
#define TXT                 (0x01 << (31 - TXT_BIT))
#define RNE_BIT             22
#define RNE                 (0x01 << (31 - RNE_BIT))
#define TNF_BIT             23
#define TNF                 (0x01 << (31 - TNF_BIT))

#define SPI_EVENT_CLR       0xFFFFFFFF

/* eSPI CSn Mode Register bits */

#define CI_BIT              0
#define CI                  (0x01 << (31 - CI_BIT))
#define CP_BIT              1
#define REV_BIT             2
#define DIV16_BIT           3
#define PM_BIT              7
#define ODD_BIT             8
#define POL_BIT             11
#define LEN_BIT             15
#define CSBEF_BIT           19
#define CSAFT_BIT           23
#define CSCG_BIT            28

/* eSPI Command Register bits */

#define CS_BIT              1
#define SPI_CS(n)           (n << (31 - CS_BIT))
#define RX_DELAY_BIT        2
#define DO_BIT              3
#define TO_BIT              4
#define SPCOM_TO            (0x01 << (31 -TO_BIT))
#define HLD_BIT             5
#define SKIP_BIT             15
#define SPI_SKIP(n)         (n << (31 - SKIP_BIT))
#define TRANLEN             16

/* eSPI CSn Mode Register bits */

#define CI_BIT              0
#define CI                  (0x01 << (31 - CI_BIT))
#define CP_BIT              1
#define CP                  (0x01 << (31 - CP_BIT))
#define REV_BIT             2
#define REV                 (0x01 << (31 - REV_BIT))
#define DIV16_BIT           3
#define DIV16               (0x01 << (31 - DIV16_BIT))
#define PM_BIT              7
#define ODD_BIT             8
#define ODD                 (0x01 << (31 - ODD_BIT))
#define POL_BIT             11
#define POL                 (0x01 << (31 - POL_BIT))
#define LEN_BIT             15
#define CSBEF_BIT           19
#define CSAFT_BIT           23
#define CSCG_BIT            28

/* Special defines */

#define SPI_CS0             0
#define SPI_CS1             1
#define SPI_CS2             2
#define SPI_CS3             3

#define TRANS_ONLY          1
#define NORMAL              0

#define SPI_CS0_CFG_INIT    0x24171118
#define SPI_CS1_CFG_INIT    0x24171118
#define SPI_CS2_CFG_INIT    0x24171118
#define SPI_CS3_CFG_INIT    0x24171118

STATUS  sysSpiWriteCmd  (UINT32, UINT32, UINT8 *, UINT32, UINT32);
void    sysSpiRead      (UINT8 *, UINT32);
void    sysSpiWriteData (UINT8 *, UINT32);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __INCfslSpih */
