/* hwconf.c - Hardware configuration support module */

/*
 * Copyright (c) 2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* 
Modfication Histroy 
--------------------
01b,05jul08,y_w  Code cleanup
01a,23apr08,y_w  written
*/

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/vxbPlbLib.h>
#include <hwif/vxbus/vxbPciLib.h>
#include <hwif/util/vxbParamSys.h>
#include <hwif/vxbus/hwConf.h>
#include "config.h"

IMPORT UINT32 sysCalcBusSpd (void);

struct hcfResource ppcDecTimerResources[] = {
    { "regBase",      HCF_RES_INT,  { ((void *)0) } },
    { "minClkRate",   HCF_RES_INT,  { ((void *)SYS_CLK_RATE_MIN) } },
    { "maxClkRate",   HCF_RES_INT,  { ((void *)SYS_CLK_RATE_MAX) } },
    { "decClockFreq", HCF_RES_ADDR, { ((void *)sysCalcBusSpd) } }
};

#define ppcDecTimerNum  NELEMENTS(ppcDecTimerResources)

const struct hcfDevice hcfDeviceList[] = {
    { "ppcDecTimerDev", 0, VXB_BUSID_PLB, 0, ppcDecTimerNum, ppcDecTimerResources }
};

const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] = {
    { NULL, 0, NULL, VXB_PARAM_END_OF_LIST, { (void *)0 } }
};

