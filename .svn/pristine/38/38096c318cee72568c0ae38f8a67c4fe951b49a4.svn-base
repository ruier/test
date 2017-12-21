/* hwconf.c - Hardware configuration support module */

/* Copyright (c) 2006 Wind River Systems, Inc. */

/* 
Modfication Histroy 
--------------------
01b,22Sep06,sup	fix for defect WIND00066405 removed compiler warnings
01a,17Jul06,sup	created
*/

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/vxbPlbLib.h>
#include <hwif/vxbus/vxbPciLib.h>
#include <hwif/util/vxbParamSys.h>
#include <hwif/vxbus/hwConf.h>
#include "config.h"
IMPORT UINT32 sysCpuBusSpd (void);

struct hcfResource ppcDecTimerResources[] =  {
    {"regBase", HCF_RES_INT, {((void *)0)}},
    {"minClkRate",HCF_RES_INT, {((void *)SYS_CLK_RATE_MIN)}},
    {"maxClkRate",HCF_RES_INT, {((void *)SYS_CLK_RATE_MAX)}},
     {"decClockFreq",HCF_RES_ADDR, {((void *)sysCpuBusSpd)}}
};

#define ppcDecTimerNum         NELEMENTS(ppcDecTimerResources)

const struct hcfDevice hcfDeviceList[] = {
    {"ppcDecTimerDev", 0, VXB_BUSID_PLB, 0, ppcDecTimerNum, ppcDecTimerResources}
};

const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] =
    {
    { NULL, 0, NULL, VXB_PARAM_END_OF_LIST, {(void *)0} }
    };

