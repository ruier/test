/* usrSnmp.c - SNMP agent initialization */

/*
 *  Copyright 2003-2010 Wind River Systems, Inc.
 *  All rights reserved.  Provided under license only.
 *  Distribution or other use of this software is only
 *  permitted pursuant to the terms of a license agreement
 *  from Wind River Systems (and is otherwise prohibited).
 *  Refer to that license agreement for terms of use.
 */

/*
modification history
--------------------
01n,08jul10,y_t  fix for WIND00208622
01m,25jun08,zhr  clean up  
01l,22jun08,zhr  Fixed wrsnmp_configure prototype issue  
01k,08jun08,zhr  updated for m2lib dynmaically loading 
01j,08mar08,zhr  updated for m2lib migration
01i,20sep07,jbl  fix an unprototyped function call
01h,05sep06,jbl  add config command for NOR method routines
01g,28jun05,job  need some locking controls around socket creation
01f,19apr05,job  update copyright notices
01e,01apr05,job  add function prototypes
01d,02dec03,job  update copyright information
01c,15aug03,ps  add agentX init routines
01b,11aug03,ten  Clean up: function was returning OK, should be void
01a,05aug03,ggg  Created
*/

/*
DESCRIPTION

SNMP agent initialization code
*/

#include <semLib.h>

extern void initSnmpV3Component(void);
extern void initAxMasterComponent(void);
extern void initAxSubagentComponent(void);
extern void initStartSnmpd(void);
extern int wrsnmp_configure(int);
extern void initSnmpd(void);

SEM_ID snmpdSocketControlSem;

/*****************************************************************************
* usrSnmpInit - initializes the SNMP agent
*
* This function initializes the SNMP agent
*
* RETURNS: ERROR if initialization problems occur
* ERRNO: N/A
*/
void usrSnmpCfgInit() 
    {

    snmpdSocketControlSem = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY);        
                                          
    initSnmpd();

#ifdef INCLUDE_WINDMANAGE_SNMP_V3CORE
    initSnmpV3Component();
#endif

#ifdef INCLUDE_WINDMANAGE_SNMP_AGENTX_MASTERCORE
    initAxMasterComponent();
#endif

#ifdef INCLUDE_WINDMANAGE_SNMP_AGENTX_SUBAGENTCORE
    initAxSubagentComponent();
#endif
   if (SNMP_USE_M2LIB) 
       {
       wrsnmp_configure(1);
       }
    else 
       wrsnmp_configure(0);
    
   initStartSnmpd();

   }

