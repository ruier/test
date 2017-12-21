/* configNet.h - network configuration header */

/* Copyright 1984-2007 Wind River Systems, Inc. */

/*
modification history
--------------------
01g,16aug07,wap  Switch to new VxBus TSEC driver, remove support for FCC
                 (WIND00101678)
01f,25jan07,ami  Ported to use the vxBus compliant mottsec driver 
01e,20aug03,dtr  Adding in TSEC config.
01d,31jul03,dtr  Adding fei secondary enet.
01c,07jul03,mil  Added FCC END.
01b,12jun02,kab  SPR 74987: cplusplus protection
01a,10oct99,mtl   written from yk 750 by teamF1

*/
 
#if (defined(INCLUDE_END) && defined(INCLUDE_NETWORK))

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
   primary/secondary/teriary/quaternary/quinary/senary
   TSEC assumed highest buffer priority followed by FCC then GEI */

#define SYS_END_BUFF_LOAN_PRI   1
#define SYS_END_BUFF_LOAN_SEC   2
#define SYS_END_BUFF_LOAN_TER   3
#define SYS_END_BUFF_LOAN_QUA   4
#define SYS_END_BUFF_LOAN_QUI   5
#define SYS_END_BUFF_LOAN_SEN   6


#ifdef INCLUDE_GEI8254X_END
#define END_LOAD_FUNC_QUI       sysGei8254xEndLoad
#define END_BUFF_LOAN_QUI       SYS_END_BUFF_LOAN_QUI
#define END_LOAD_FUNC_SEN       sysGei8254xEndLoad
#define END_BUFF_LOAN_SEN       SYS_END_BUFF_LOAN_SEN
IMPORT END_OBJ * sysGei8254xEndLoad (char *, void *);
#endif

/* Secondary network device */
/* secondary devices are currently unsupported in this release */

/* The END_LOAD_STRING is defined empty and created dynamicaly */

#define END_LOAD_STRING ""      /* created in sys<device>End.c */

/* define IP_MAX_UNITS to the actual number in the table. */

#ifndef IP_MAX_UNITS
#define IP_MAX_UNITS            (NELEMENTS(endDevTbl) - 1)
#endif  /* ifndef IP_MAX_UNITS */

/* each entry will be processed by muxDevLoad() */

END_TBL_ENTRY endDevTbl [] =
    {
#if defined(INCLUDE_PRIMARY_GEI_END)
    {0, END_LOAD_FUNC_QUI, END_LOAD_STRING, END_BUFF_LOAN_PRI, NULL},
#endif
#if defined(INCLUDE_SECONDARY_GEI_END)
    {1, END_LOAD_FUNC_SEN, END_LOAD_STRING, END_BUFF_LOAN_SEC, NULL},
#endif
    {0, END_TBL_END, NULL, 0, NULL} 	/* must be last */
    };
#endif /* INCnetConfigh */

#ifdef __cplusplus
    }
#endif

#endif /* (defined (INCLUDE_END) && defined (INCLUDE_NETWORK)) */











