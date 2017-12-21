/* hwconf.c - Hardware configuration support module */

/*
 * Copyright (c) 2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,22apr08,z_l  Created
*/

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbParamSys.h>
#include "config.h"
#include "hpc2.h"

const struct hcfResource ppcDecTimerDevResources[] = {
    { VXB_REG_BASE,   HCF_RES_INT,  { (void *)0 } },
    { "decClockFreq", HCF_RES_ADDR, { (void *)sysClkFreqGet } },
    { "minClkRate",   HCF_RES_INT,  { (void *)SYS_CLK_RATE_MIN } },
    { "maxClkRate",   HCF_RES_INT,  { (void *)SYS_CLK_RATE_MAX } }
};

#define ppcDecTimerDevNum NELEMENTS(ppcDecTimerDevResources)

const struct hcfDevice hcfDeviceList[] = {
    { "ppcDecTimerDev", 0, VXB_BUSID_PLB, 0, ppcDecTimerDevNum, ppcDecTimerDevResources },
};

const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] = {
    { NULL, 0, NULL, VXB_PARAM_END_OF_LIST, { (void *)0 } }
};
