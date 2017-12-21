/* m8560CpmIntrCtl.h - Motorola ads 85xx board system-dependent header file */

/* Copyright 1984-2003 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/*
modification history
--------------------
01b,29aug06,wap  Add m85xxCpmIntDisconnect() prototype
01a,29jul04,mil  Removed BSP header file.
*/

typedef struct m8560VecTbl
    {
    UINT32 intVec;
    void   (*excHandler) ();
    UINT32 param;
    } M8560VEC_TBL;


#define INUM_CPM_SCC1 40
#define INUM_CPM_SCC2 41

extern void m85xxCpmIntConnect (int inum, VOIDFUNCPTR handler, int *param);
extern void m85xxCpmIntDisconnect (int inum, VOIDFUNCPTR handler, int *param);
extern void m85xxCpmIntEnable (int vector);
extern void m85xxCpmIntDisable (int vector);


