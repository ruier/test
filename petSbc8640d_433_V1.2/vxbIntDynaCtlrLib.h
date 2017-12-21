/* vxbIntDynaLib.h -- VxBus Dynamic Interrupt library header file */

/*
*
* Copyright (C) 2007  Wind River Systems, Inc. All rights are reserved.
*
* The right to copy, distribute, modify, or otherwise make use
* of this software may be licensed only pursuant to the terms
* of an applicable Wind River license agreement.
*/

/*
modification history
--------------------
01c,18jul07,tor  share struct
01b,15jul07,tor  fix pInputPin
01a,20jun07,dtr  created
*/

#ifndef _INC_vxbIntDynaLibH
#define _INC_vxbIntDynaLibH

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/hwMemLib.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <logLib.h>
#include <intLib.h>
#include <stdio.h>
#include <private/funcBindP.h>


struct dynamicIntrTable 
    {
    UINT32  inputPin;
    UINT32  numOfInterrupts;
    };


STATUS vxbIntDynaConnect
    (
    VXB_DEVICE_ID instance,
    int           vecCount,
    struct vxbIntDynaVecInfo * pDynaVecIntr
    );



STATUS vxbIntDynaVecAlloc
    (
    VXB_DEVICE_ID        vecOwner,
    VXB_DEVICE_ID        serviceInstance,
    int                  numVecs,
    struct vxbIntDynaVecInfo * pDynaVec
    );

VXB_DEVICE_ID vxbIntDynaVecOwnerFind(VXB_DEVICE_ID instID);

STATUS vxbIntDynaVecDevProgram
    (
    VXB_DEVICE_ID pVectorOwner,
    VXB_DEVICE_ID serviceInstance,
    struct vxbIntDynaVecInfo * pDynaVec   /* single vector: first element used */
    );

STATUS vxbIntDynaVecDevMultiProgram
    (
    VXB_DEVICE_ID pVectorOwner,
    VXB_DEVICE_ID serviceInstance,
    int           numVectors,
    struct vxbIntDynaVecInfo * pDynaVec
    );

STATUS vxbIntDynaVecMultiConnect
    (
    VXB_DEVICE_ID         intCtlrID,
    VXB_DEVICE_ID         instID,
    int                   numVecs,
    struct vxbIntDynaVecInfo  * pDynaVec
    );

void vxbIntDynaCtlrInit();

METHOD_DECL(vxbIntDynaVecProgram);
METHOD_DECL(vxbIntDynaVecEnable);
METHOD_DECL(vxbIntDynaVecDisable);
METHOD_DECL(vxbIntDynaVecAlloc);
METHOD_DECL(vxbIntDynaVecConnect);
METHOD_DECL(vxbIntDynaVecOwnerFind);
METHOD_DECL(vxbIntDynaVecDevMultiProgram);
METHOD_DECL(vxbIntDynaVecMultiConnect);



#define VXB_INT_MAX_DYNAMIC_SOURCES 0x10


#endif /* _INC_vxbIntDynaLibH */
 
