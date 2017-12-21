/* configNet.h - network configuration header */

/* Copyright 1984-2005 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,03aug05,dtr  Remove comma from device tbl.
01a,04feb05,dtr  written from ads8560/configNet.h.
*/
 
#ifndef INCconfigNeth
#define INCconfigNeth

#include "vxWorks.h"
#include "config.h"
#include "end.h"
#include "miiLib.h"

/* defines */
 
#define SYS_END_BUFF_LOAN_PRI   1
#define SYS_END_BUFF_LOAN_SEC   2
#define SYS_END_BUFF_LOAN_TER   3
#define SYS_END_BUFF_LOAN_QUA   4
#define SYS_END_BUFF_LOAN_QUI   5
#define SYS_END_BUFF_LOAN_SEN   6


#ifdef INCLUDE_FEI_END
#define FEI_LOAD_FUNC   sysFei82557EndLoad
IMPORT END_OBJ* FEI_LOAD_FUNC (char *, void*);
#define FEI_LOAD_STRING ""
#endif /* INCLUDE_FEI_END */


#ifdef INCLUDE_GEI8254X_END
#   define GEI82543_LOAD_FUNC     sysGei8254xEndLoad  /* external interface */
#   define GEI82543_BUFF_LOAN     1                /* enable buffer loan */
#   define GEI_MAX_UNITS          1                /* Maximum device units */
#   define GEI82543_LOAD_STRING_0 geiEndLoadStr

char geiEndLoadStr [END_DESC_MAX] = {0};

IMPORT END_OBJ* GEI82543_LOAD_FUNC (char*, void*);
#endif 



#ifdef INCLUDE_MOT_TSEC_END
#define END_LOAD_FUNC_PRI       sysMotTsecEndLoad
#define END_BUFF_LOAN_PRI       SYS_END_BUFF_LOAN_PRI   
#define END_LOAD_FUNC_SEC       sysMotTsecEndLoad
#define END_BUFF_LOAN_SEC       SYS_END_BUFF_LOAN_SEC  
#endif


#if defined(INCLUDE_PRIMARY_TSEC_END)
IMPORT END_OBJ * END_LOAD_FUNC_PRI (char *, void*);
#endif
#if defined(INCLUDE_SECONDARY_TSEC_END)
IMPORT END_OBJ * END_LOAD_FUNC_SEC (char *, void*);
#endif


#define END_LOAD_STRING ""      /* created in sys<device>End.c */


/* max number of SENS ipAttachments we can have */
 
#ifndef IP_MAX_UNITS
#   define IP_MAX_UNITS (NELEMENTS (endDevTbl) - 1)
#endif
 
END_TBL_ENTRY endDevTbl [] =
{

#if defined(INCLUDE_PRIMARY_TSEC_END)
    {0, END_LOAD_FUNC_PRI, END_LOAD_STRING, END_BUFF_LOAN_PRI, NULL},
#endif
#if defined(INCLUDE_SECONDARY_TSEC_END)
    {1, END_LOAD_FUNC_SEC, END_LOAD_STRING, END_BUFF_LOAN_SEC, NULL},
#endif

#ifdef INCLUDE_FEI_END
    { 0, FEI_LOAD_FUNC, FEI_LOAD_STRING, 3, NULL, FALSE},
#endif /*INCLUDE_FEI_END */

#ifdef INCLUDE_GEI8254X_END
    { 0, GEI82543_LOAD_FUNC, GEI82543_LOAD_STRING_0, GEI82543_BUFF_LOAN, NULL, FALSE},
#endif

    { 0, END_TBL_END, NULL, 0, NULL, FALSE}
};
#endif /* INCconfigNeth */
