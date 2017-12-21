/* wdbVisionDriver.c - Wind River Vision Driver WDB_COMM initialization library */

/*
 * Copyright 1999, 2001-2004, 2006-2007 Wind River Systems Inc.
 *
 * The right to copy, distribute or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement.
 */

/*
modification history
--------------------
02m,12mar07,pes  Update for MIPS library restructure.
02l,21feb07,pes  Add support for new MIPS lib structure.
02k,13feb07,rec  WIND00086917 - add coldfire support
02j,22mar06,n_b  Added ARMARCH6 and ARMARCH6_T support
02i,28oct04,j_b  integrate TMD patch 2.0
02h,16mar04,r_s  Added PPC85XX Support
02g,14jun03,r_s  Added ARMARCH5 and ARMARCH5_T support
02f,20jun03,nwd  Deliver of Tornado 2.2 Cumulative Patch 1
02e,31jul02,e_t  Added support for MIPS64.
02d,13jul02,tcr  Added support for the 440.
02c,30nov01,g_h  Cleaning for T2.2
02b,12nov01,g_h  Add support for ColdFire
02a,04jun01,g_h  Changed to wdbVisionDriver based version of for the TMD mode
01c,09apr01,rnr  Changed to vDriver based version of for the TMD mode
01b,07feb01,g_h  renaming module name and cleaning
01a,24dec99,est  adapted from wdbNETROM.c
*/

/*
DESCRIPTION
Initializes the Wind River Vision Tools connection for the WDB agent.

NOMANUAL
*/

/* Include main packet driver source module */

#include "wdb/vision/wdbVisionDrv.c"
#include "drv/wdb/vision/wdbVisionPktDrv.h"
#include "wdb/vision/wdbVisionPktDrv.c"

#if defined (INCLUDE_WDB_COMM_VTMD)
#include "wdb/vision/visionTmdDrv.c"
#if   ((CPU == PPC603) || (CPU == PPC860) || (CPU == PPC604) || (CPU == PPC405) || (CPU == PPC440)) || (CPU == PPC85XX)
#include "wdb/vision/visionTmdAsmPpc.c"
#elif _WRS_MIPS_32BIT_CPU
#include "wdb/vision/visionTmdAsmMips.c"
#elif _WRS_MIPS_64BIT_CPU
#include "wdb/vision/visionTmdAsmMips.c"
#elif ((CPU == ARMARCH4)||(CPU==ARMARCH4_T) || (CPU == ARMARCH5) || (CPU == ARMARCH5_T) || (CPU == ARMARCH6) || (CPU == ARMARCH6_T))
#include "wdb/vision/visionTmdAsmArm.c"
#elif (CPU == XSCALE)
#include "wdb/vision/visionTmdAsmXscale.c"
#elif ((CPU == SH7700) || (CPU == SH7750) || (CPU == SH7600))
#include "wdb/vision/visionTmdAsmSh.c"
#elif ((CPU == MCF5200) || (CPU == MCF5400))
#include "wdb/vision/visionTmdAsmCfire.c"
#else
#error "This CPU is not supported !!!"
#endif
#endif /* (INCLUDE_WDB_COMM_VTMD) */

/* Check MTU */

#if     WDB_MTU > WDB_VISION_PKT_MTU
#undef  WDB_MTU
#define WDB_VISION_PKT_MTU
#endif  /* WDB_MTU > WDB_VISION_PKT_MTU */

/***************************************************************************
*
* wdbCommDevInit - initialize the vision connection
*
* This routine initialize the vision connection
*
* RETURN: OK or ERROR
*/

STATUS wdbCommDevInit
    (
    WDB_COMM_IF  * pCommIf,
    char        ** ppWdbInBuf,
    char        ** ppWdbOutBuf
    )
    {
    LOCAL WDB_VISION_PKT_DEV wdbVisionPktDev;
    LOCAL uint_t             wdbInBuf [WDB_MTU/4];
    LOCAL uint_t             wdbOutBuf [WDB_MTU/4];

    /* update input & output buffer pointers */

    *ppWdbInBuf  = (char *) wdbInBuf;
    *ppWdbOutBuf = (char *) wdbOutBuf;

    /* update communication interface mtu */

    wdbCommMtu = WDB_MTU;

    wdbVisionPktDevInit (&wdbVisionPktDev, udpRcv);

    if (udpCommIfInit (pCommIf, &wdbVisionPktDev.wdbDrvIf) == ERROR)
        return (ERROR);

    return (OK);
    }
