/* configNet.h - network configuration header */

/* Copyright 1984-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01g,09aug07,wap  Convert to VxBus (WIND00100585)
01f,30aug04,j_b  remove INCLUDE_NETWORK dependencies (SPR #99747)
01e,20aug03,dtr  Adding in TSEC config.
01d,31jul03,dtr  Adding fei secondary enet.
01c,07jul03,mil  Added FCC END.
01b,12jun02,kab  SPR 74987: cplusplus protection
01a,10oct99,mtl   written from yk 750 by teamF1

*/

#ifdef INCLUDE_END

#ifndef INCnetConfigh
#define INCnetConfigh

#ifdef __cplusplus
    extern "C" {
#endif

/* includes */

#include "vxWorks.h"
#include "end.h"

/* each entry will be processed by muxDevLoad() */

END_TBL_ENTRY endDevTbl [] =
    {
    {0, END_TBL_END, NULL, 0, NULL, FALSE}
    };
#endif /* INCnetConfigh */

#ifdef __cplusplus
    }
#endif

#endif /* INCLUDE_END */

