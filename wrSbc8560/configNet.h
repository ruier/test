/* configNet.h - network configuration header */

/* Copyright (c) 1984-2006 Wind River Systems, Inc. */

/*
modification history
--------------------
01l,13sep06,tfo  Remove WLAN support for Atheros AR521X, not supported
01k,30aug06,tfo  WLAN support for Atheros AR521X
01k,30aug06,wap  Switch over to VxBus FCC driver
01j,28Jul06,tor  change TSEC END to TSEC HEND
01i,15sep05,to   vxbus cleanup
01h,08aug05,mdo  Change WINDBUS to VXBUS
01g,08aug05,???  fix for itn8frz11 error
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


/* defines */
/* Six devices configurable :
   primary/secondary/tertiary/quaternary/quinary/senary
   TSEC assumed highest buffer priority followed by FCC */

#define SYS_END_BUFF_LOAN_PRI   1
#define SYS_END_BUFF_LOAN_SEC   2
#define SYS_END_BUFF_LOAN_TER   3
#define SYS_END_BUFF_LOAN_QUA   4

/* each entry will be processed by muxDevLoad() */

END_TBL_ENTRY endDevTbl [] =
    {
    {0, END_TBL_END, NULL, 0, NULL},
    {0, END_TBL_END, NULL, 0, NULL},
    {0, END_TBL_END, NULL, 0, NULL},
    {0, END_TBL_END, NULL, 0, NULL} 	/* must be last */
    };

/* define IP_MAX_UNITS to the actual number in the table. */

#ifndef IP_MAX_UNITS
#define IP_MAX_UNITS            (NELEMENTS(endDevTbl) - 1)
#endif  /* ifndef IP_MAX_UNITS */

#ifdef __cplusplus
    }
#endif

#endif /* INCnetConfigh */

#endif /* INCLUDE_END */

