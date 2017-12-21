/* configNet.h - network configuration header */

/* Copyright (c) 2006 Wind River Systems, Inc. */
/* Copyright (C) 2005 Tundra Semiconductor */

/*
modification history
--------------------
01c,20mar06,dtr  Change unit number for secondary tsiG.
01b,01feb06,dtr   tidyup
01a,05oct05,klai  written (from sp7447/configNet.h)
*/
 
#ifndef INCnetConfigh
#define INCnetConfigh

#ifdef __cplusplus
extern "C" {
#endif

#ifdef INCLUDE_NETWORK /* ignore everything if NETWORK not included */

#include <vxWorks.h>
#include "config.h"
#include <end.h>

#define TSI108_LOAD_FUNC tsi108EndLoad
  
#define TSI108_BUFF_LOAN   1

#define SYS_END_BUFF_LOAN_PRI   1
#define SYS_END_BUFF_LOAN_SEC   2

#ifdef INCLUDE_GEI8254X_END
#define END_LOAD_FUNC_PRI       sysGei8254xEndLoad
#define END_BUFF_LOAN_PRI       SYS_END_BUFF_LOAN_PRI   
#define END_LOAD_FUNC_SEC       sysGei8254xEndLoad
#define END_BUFF_LOAN_SEC       SYS_END_BUFF_LOAN_SEC   
IMPORT END_OBJ * sysGei8254xEndLoad (char *, void *);
#endif

/* The END_LOAD_STRING is defined empty and created dynamicaly */

#define END_LOAD_STRING ""      /* created in sys<device>End.c */


#if defined(INCLUDE_PRIMARY_GEI_END)
IMPORT END_OBJ * END_LOAD_FUNC_PRI (char *, void*);
#endif
#if defined(INCLUDE_SECONDARY_GEI_END)
IMPORT END_OBJ * END_LOAD_FUNC_SEC (char *, void*);
#endif

/* <devNum>:<portRegAdrs>:<phyRegAdrs>:<phyMiiAdrs>:<intNum> */

/* load string for port0 */
#define	TSI108_LOAD_STRING	"0:0xc0006000:0xc0006000:8:16"
/* load string for port1 */
#define	TSI108_LOAD_STRING1	"1:0xc0006400:0xc0006000:9:17"

IMPORT END_OBJ * TSI108_LOAD_FUNC ();

END_TBL_ENTRY endDevTbl [] =
    {

      { 0, TSI108_LOAD_FUNC, TSI108_LOAD_STRING, TSI108_BUFF_LOAN, NULL, FALSE},
#ifdef INCLUDE_SECONDARY_GIGE
      { 1, TSI108_LOAD_FUNC, TSI108_LOAD_STRING1, TSI108_BUFF_LOAN, NULL, FALSE},
#endif /* INCLUDE_SECONDARY_GIGE */
#if defined(INCLUDE_PRIMARY_GEI_END)
    {0, END_LOAD_FUNC_PRI, END_LOAD_STRING, END_BUFF_LOAN_PRI, NULL},
#endif
#if defined(INCLUDE_SECONDARY_GEI_END)
    {1, END_LOAD_FUNC_SEC, END_LOAD_STRING, END_BUFF_LOAN_SEC, NULL},
#endif
      { 0, END_TBL_END, NULL, 0, NULL, FALSE},
    };

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_NETWORK */
#endif /* INCnetConfigh */
