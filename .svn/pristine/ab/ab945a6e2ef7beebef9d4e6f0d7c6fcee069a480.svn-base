/* wancomDefs.h - MV64260 Ethernet Driver To BSP Defines Header File */

/* Copyright 2004      Wind River Systems, Inc. */
/* Copyright 2002,2003 Motorola, Inc., All Rights Reserved */

/*
modification history
--------------------
01g,14jun04,h_k  modified for updated wancomEnd.
01d,23jul03,scb add push to GT64260_REG_WR - prevent interrupt with no cause. 
01c,19nov02,scb Undid (01b) change - fix for gei buffs in cacheable mem is in.
01b,18nov02,scb Changed #defines for "ETHER_..." to "WANCON_...".
01a,28oct02,scb Copied from hxeb100 base version 01a.
*/

/*

DESCRIPTION

This file connects the Wind River WANCOM driver #defines to the Motorola
MV64260 defines to allow the Wind River driver to compile with as little
modification as possible in the Motorola MV64260 BSP environment.

*/

#ifndef INCwancomDefsh
#define INCwandcomDefsh

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

/* includes */

#include "mv64260.h"

/* defines */

#define GT64260_BASE_ADRS          MV64260_REG_BASE

#define ETH_PHY_ADRS_PORT0         PHY_ADDR_ENET_PORT0
#define ETH_PHY_ADRS_PORT1         PHY_ADDR_ENET_PORT1

#define ETH_PORT_MEM_SIZE          ETHER_PORT_MEM_SIZE
#define ETH_MEM_START_ADRS(unit)   \
        (char *)(ETHER_MEM_START_ADDR + (ETHER_PORT_MEM_SIZE * unit))

#define GT64260_REG_RD(x,val)   \
        { \
                UINT32 tmp ; \
                tmp = *((UINT32*)(GT64260_BASE_ADRS+(x))); \
                EIEIO ; \
                (*val) = LONGSWAP(tmp); \
                EIEIO ; \
        }
#define GT64260_REG_WR(x,val)   \
        {       \
                UINT32 tmp ; \
        	tmp = LONGSWAP(val); \
                EIEIO ; \
                *((UINT32*)(GT64260_BASE_ADRS+(x))) = tmp ; \
                EIEIO ; \
                tmp = *((UINT32*)(GT64260_BASE_ADRS+(x))); \
        }

/* Main interrupt cause high */

#define INT_PRI_ETH0            ICI_MICH_INT_NUM_0
#define INT_PRI_ETH1            ICI_MICH_INT_NUM_1
#define INT_PRI_ETH2            ICI_MICH_INT_NUM_2

#define INT_LVL_ETH0            INT_PRI_ETH0
#define INT_LVL_ETH1            INT_PRI_ETH1
#define INT_LVL_ETH2            INT_PRI_ETH2

#define INT_VEC_ETH0            INT_LVL_ETH0
#define INT_VEC_ETH1            INT_LVL_ETH1
#define INT_VEC_ETH2            INT_LVL_ETH2

/* Ethernet Registers */

#define ETH_PHY_ADRS            FE_ENET_PHY_ADDR               /* (0x2000) */

#define ETH_SMIR                FE_ENET_SMI                    /* (0x2010) */

#ifdef __cplusplus

    }
#endif /* __cplusplus */

#endif /* INCwancomDefsh */
