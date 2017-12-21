/* hwconf.c - Hardware configuration support module */

/* Copyright (c) 2008 Wind River Systems, Inc. */

/*
modification history
--------------------
01a,15jan08,wap  written
*/

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/vxbPlbLib.h>
#include <hwif/vxbus/vxbPciLib.h>
#include <hwif/util/vxbParamSys.h>
#include <hwif/vxbus/hwConf.h>
#include <../src/hwif/h/mii/vxbIbmZmii.h>

#include "config.h"

#ifdef INCLUDE_VXB_IBM_ZMII

/*
 * The PHYs on the AMCC440EP board are strapped as follows:
 *
 * 	MAC			PHY
 *	---			---
 *	emac0			0
 *	emac1			3
 *
 * The PHYs are set for reduced pin mode (RMII).
 */

LOCAL const struct hcfResource zmii0Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)(ZMII0_BASE) } },
    { "emac0Mode",      HCF_RES_INT,    { (void *)ZMII_EMAC_RMII } },
    { "emac1Mode",      HCF_RES_INT,    { (void *)ZMII_EMAC_RMII } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } }
};
#define zmii0Num NELEMENTS(zmii0Resources)
#endif /* INCLUDE_VXB_IBM_ZMII */

#ifdef INCLUDE_EMAC_VXB_END
const struct hcfResource emac0Resources[] = {
    { "regBase", HCF_RES_INT, { (void *)(EMAC0_BASE) } },
    { "intr", HCF_RES_INT, { (void *)INT_VEC_ENET_0 } },
    { "intrLevel", HCF_RES_INT, { (void *)INT_LVL_ENET_0 } },
    { "phyAddr", HCF_RES_INT, { (void *)1 } },
    { "miiIfName", HCF_RES_STRING, { (void *)"emac" } },
    { "miiIfUnit", HCF_RES_INT, { (void *)0 } }
};

#define emac0Num NELEMENTS(emac0Resources)

#ifdef USE_BOTH_EMACS
const struct hcfResource emac1Resources[] = {
    { "regBase", HCF_RES_INT, { (void *)(EMAC1_BASE) } },
    { "intr", HCF_RES_INT, { (void *)INT_VEC_ENET_1 } },
    { "intrLevel", HCF_RES_INT, { (void *)INT_LVL_ENET_1 } },
    { "phyAddr", HCF_RES_INT, { (void *)3 } },
    { "miiIfName", HCF_RES_STRING, { (void *)"emac" } },
    { "miiIfUnit", HCF_RES_INT, { (void *)0 } }
};

#define emac1Num NELEMENTS(emac1Resources)
#endif /* USE_BOTH_EMACS */
#endif

#ifdef INCLUDE_VXB_IBM_MAL
const struct hcfResource mal0Resources[] = {
    { "regBase", HCF_RES_INT, { (void *)(MAL0_DCR_BASE) } },
    { "intr0", HCF_RES_INT, { (void *)(INT_VEC_MAL_TXEOB) } },
    { "intr0Level", HCF_RES_INT, { (void *)(INT_LVL_MAL_TXEOB) } },
    { "intr1", HCF_RES_INT, { (void *)(INT_VEC_MAL_RXEOB) } },
    { "intr1Level", HCF_RES_INT, { (void *)(INT_LVL_MAL_RXEOB) } },
    { "intr2", HCF_RES_INT, { (void *)(INT_VEC_MAL_SERR) } },
    { "intr2Level", HCF_RES_INT, { (void *)(INT_LVL_MAL_SERR) } },
    { "intr3", HCF_RES_INT, { (void *)(INT_VEC_MAL_TXDE) } },
    { "intr3Level", HCF_RES_INT, { (void *)(INT_LVL_MAL_TXDE) } },
    { "intr4", HCF_RES_INT, { (void *)(INT_VEC_MAL_RXDE) } },
    { "intr4Level", HCF_RES_INT, { (void *)(INT_LVL_MAL_RXDE) } }
};

#define mal0Num NELEMENTS(mal0Resources)
#endif /* INCLUDE_VXB_IBM_MAL */

const struct hcfDevice hcfDeviceList[] = {
#ifdef INCLUDE_VXB_IBM_ZMII
    { "zmii", 0, VXB_BUSID_PLB, 0, zmii0Num, zmii0Resources },
#endif /* INCLUDE_VXB_IBM_ZMII */
#ifdef INCLUDE_VXB_IBM_MAL
    { "mal", 0, VXB_BUSID_PLB, 0, mal0Num, mal0Resources },
#endif /* INCLUDE_VXB_IBM_MAL */
#ifdef INCLUDE_EMAC_VXB_END
    { "emac", 0, VXB_BUSID_PLB, 0, emac0Num, emac0Resources },
#ifdef USE_BOTH_EMACS
    { "emac", 1, VXB_BUSID_PLB, 0, emac1Num, emac1Resources }
#endif /* USE_BOTH_EMACS */
#endif
};

const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] =
    {
    { NULL, 0, NULL, VXB_PARAM_END_OF_LIST, {(void *)0} }
    };
