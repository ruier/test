/* configNet.h - network configuration header */

/*
 * Copyright (c) 1998, 2004, 2006-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01f,06oct08,b_m  remove legacy fei/gei driver support.
01e,11jan07,jw   Change for standard adapters.
01d,03feb06,pkr  add support for Lite5200b
01c,30dec04,k_p  SPR #99747,removed INCLUDE_NETWORK dependencies
01b,03feb04,bjn  Adding IS_ICECUBE_OLD.
01a,02jun98,cn   written.
*/

#ifndef __INCconfigNeth
#define __INCconfigNeth

#if defined (INCLUDE_END)

#include <vxWorks.h>
#include <end.h>

/* defines */

#define PHY_10BASE_T            0x00    /* 10 Base-T */
#define PHY_10BASE_T_FDX        0x01    /* 10 Base Tx, full duplex */
#define PHY_100BASE_TX          0x02    /* 100 Base Tx */
#define PHY_100BASE_TX_FDX      0x03    /* 100 Base TX, full duplex */
#define PHY_100BASE_T4          0x04    /* 100 Base T4 */

#ifdef INCLUDE_FEC_END

/* Freescale Fast Ethernet Controller */

#define FEC_LOAD_FUNC           m5200FecEndLoad

/*
 * "<bufBase>:<bufSize>:<fifoTxBase>:<fifoRxBase>
 * :<tbdNum>:<rbdNum>:<phyAddr>:<isoPhyAddr>:<phyDefMode>:<userFlags>
 * :<clockSpeed>"
 */

/*
 * clockSpeed is optional for processors running at 50 MHz or less but required
 * for proper MII management operation, otherwise.  The "stringize" macros are
 * ANSI C.  Use of K&R C requires clockSpeed to be hard-coded or set in a
 * separate function
 */

#define FEC_CLOCK_STRINGIZE(x)  #x
#define FEC_CLOCK_SPEED(x)      FEC_CLOCK_STRINGIZE(x)

/*
 * userFlags = FEC_END_USR_PHY_NO_FD | FEC_END_USR_PHY_NO_100
 *             0x4                     0x8
 */

#define FEC_LOAD_STR    "-1:0x0:-1:-1:0x40:0x30:0x0:0xff:2:0x4:" \
                        FEC_CLOCK_SPEED(IPB_CLOCK_LITERAL)

#define FEC_BUFF_LOAN   1

IMPORT END_OBJ * FEC_LOAD_FUNC (char*, void*);

/*
 * this table may be customized by the user to force a
 * particular order how different technology abilities may be
 * negotiated by the PHY.
 */

INT16 m5200FecPhyAnOrderTbl [] =
    {
    PHY_100BASE_T4,             /* 100Base-T4 */
    PHY_100BASE_TX_FDX,         /* 100Base-T full duplex*/
    PHY_100BASE_TX,             /* 100Base-T */
    PHY_10BASE_T_FDX,           /* 10Base-T full duplex */
    PHY_10BASE_T,               /* 10Base-T */
    -1                          /* Signal end of table */
    };

#endif /* INCLUDE_FEC_END */

/* max number of SENS ipAttachments we can have */

#ifndef IP_MAX_UNITS
#   define IP_MAX_UNITS (NELEMENTS (endDevTbl) - 1)
#endif

/* END device table */

END_TBL_ENTRY endDevTbl [] = {
#ifdef INCLUDE_FEC_END
    { 0, FEC_LOAD_FUNC, FEC_LOAD_STR, FEC_BUFF_LOAN, NULL, FALSE },
#endif /* INCLUDE_FEC_END */

    { 0, END_TBL_END, NULL, 0, NULL, FALSE },
};

#endif /* INCLUDE_END */

#endif /* __INCconfigNeth */
