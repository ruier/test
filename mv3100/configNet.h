/* configNet.h - network configuration header */

/* Copyright 1984-1999 Wind River Systems, Inc. */
/* Copyright 2004-2005 Motorola Inc. All Rights Reserved */

/*
modification history
--------------------
01j,19aug05,efb  Added missing IP_MAX_UNITS define based on table size.
01i,09may05,efb  Changed names of END load functions to Mpc8540 from Mot.
01h,09mar05,efb  Adjusted table for new TSEC driver.
01g,08dec04,cak  Removed Fei references as well as conditional INCLUDE_END
		 and INCLUDE_NETWORK statement. 
01f,23aug04,scb  Starting point for "Big Easy".
01e,20aug03,dtr  Adding in TSEC config.
01d,31jul03,dtr  Adding fei secondary enet.
01c,07jul03,mil  Added FCC END.
01b,12jun02,kab  SPR 74987: cplusplus protection
01a,10oct99,mtl   written from yk 750 by teamF1

*/
 
#ifndef INCnetConfigh
#define INCnetConfigh

#ifdef __cplusplus
    extern "C" {
#endif

/* includes */

#include "vxWorks.h"
#include "end.h"

/* defines */

#if defined (INCLUDE_PRIMARY_TSEC_END) || defined (INCLUDE_SECONDARY_TSEC_END)
#define END_LOAD_FUNC_PRI       sysMpc8540TsecEndLoad
#define END_LOAD_FUNC_SEC       sysMpc8540TsecEndLoad
#endif  /* INCLUDE_PRIMARY_TSEC_END || INCLUDE_SECONDARY_TSEC_END */

#ifdef INCLUDE_MOT_FEC_END
#define END_LOAD_FUNC_TER       sysMpc8540FecEndLoad
#endif /* INCLUDE_MOT_FEC_END */

#ifdef INCLUDE_SMEND
# define SMEND_LOAD_STRING       ""
# define SMEND_LOAD_FUNC        sysSmEndLoad
#endif /* INCLUDE_SMEND */

/* The END_LOAD_STRING is defined empty and created dynamicaly */

#define END_LOAD_STRING ""      /* created in sys<device>End.c */

#if defined (INCLUDE_PRIMARY_TSEC_END)
IMPORT END_OBJ * END_LOAD_FUNC_PRI (char *, void*);
#endif
#if defined (INCLUDE_SECONDARY_TSEC_END)
IMPORT END_OBJ * END_LOAD_FUNC_SEC (char *, void*);
#endif
#if defined (INCLUDE_MOT_FEC_END)
IMPORT END_OBJ * END_LOAD_FUNC_TER (char *, void*);
#endif

#ifdef INCLUDE_SMEND
IMPORT END_OBJ * SMEND_LOAD_FUNC (char *, void *);
#endif /* INCLUDE_SMEND */

/* each entry will be processed by muxDevLoad() */

END_TBL_ENTRY endDevTbl [] =
    {
#if defined (INCLUDE_PRIMARY_TSEC_END)
    {0, END_LOAD_FUNC_PRI, END_LOAD_STRING, 0, NULL, FALSE},
#endif
#if defined (INCLUDE_SECONDARY_TSEC_END)
    {1, END_LOAD_FUNC_SEC, END_LOAD_STRING, 0, NULL, FALSE},
#endif
#if defined (INCLUDE_MOT_FEC_END)
    {0, END_LOAD_FUNC_TER, END_LOAD_STRING, 0, NULL, FALSE},
#endif
#ifdef INCLUDE_SMEND
    {0, SMEND_LOAD_FUNC, SMEND_LOAD_STRING, 0, NULL, FALSE},
#endif
    {0, END_TBL_END, NULL, 0, NULL} 	/* must be last */
    };

/* define IP_MAX_UNITS to the actual number in the table. */

#ifndef IP_MAX_UNITS
#   define IP_MAX_UNITS            (NELEMENTS(endDevTbl) - 1)
#endif  /* ifndef IP_MAX_UNITS */

#ifdef __cplusplus
    }
#endif

#endif /* INCnetConfigh */
