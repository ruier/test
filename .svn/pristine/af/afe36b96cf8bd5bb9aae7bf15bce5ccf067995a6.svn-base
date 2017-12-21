/* hwconf.c - HW configuration support module for NT simulator */

/*
 * Copyright (c) 2006-2009,2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01i,20feb13,elp  added sysInstParamsTable (WIND00401231)
01h,26jun09,h_w  remove MCB, MOB and VxBus references for MIPC
01g,02apr09,elp  removed PLB support, support IPI for non SMP.
01f,31oct08,ebh  Remove MOB_PLB_WDB references
01e,03sep08,elp  added MIPC support
01d,29aug08,kab  Change _WRS_VX_SMP to _WRS_CONFIG_SMP
01c,10jul07,tor  convert CPCs to vxIpiLib
01b,21dec06,bwa  cleanup.
01a,05jul06,bwa  written.
*/

#include <vxWorks.h>
#include <vsbConfig.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/vxbPlbLib.h>
#include <hwif/util/vxbParamSys.h>
#include <hwif/vxbus/hwConf.h>
#include <config.h>

#ifdef INCLUDE_DSHM_BUS_PLB
void * dshmRegBaseDummy [10];	/* the simulators don't have real register base
				 * addresses, but an entry is needed by the
				 * vxbus subsystem
				 */

const struct hcfResource dshmBusCtlrSimRes[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "bus_type",	RES_ADDR,	{(void *)"plb"} },
#if (CPU==SIMSPARCSOLARIS || CPU==SIMLINUX)
	{ "intr",	RES_INT,	{(void *)IV_USR2} },
#else
	{ "intr",	RES_INT,	{(void *)BUS_INT} },
#endif
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "SM_START",	RES_ADDR,	{(void *) -1} },
	{ "POOLSIZE",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
	{ "NODE",	RES_INT,	{(void *) DSHM_BUS_PLB_NODE} },
	{ "RMW",	RES_ADDR,	{(void *) DSHM_BUS_PLB_RMW} },
	{ "NRETRIES",	RES_INT,	{(void *) DSHM_BUS_PLB_NRETRIES} },
	{ "MAXNODES",	RES_INT,	{(void *) DSHM_BUS_PLB_MAXNODES} },
	{ "NENTRIES",	RES_INT,	{(void *) DSHM_BUS_PLB_NENTRIES} },
	{ "ENTRY_SIZE",	RES_INT,	{(void *) DSHM_BUS_PLB_ENTRY_SIZE} },
};
#endif /* INCLUDE_DSHM_BUS_PLB */

/* Peer devices that represent each possible peer in the system. The intrLevel
 * is set to -1, but this is just a placeholder: the reason that they can't be
 * set up here is that the value is allocated at runtime for the simulator, by
 * the host OS.
 *
 * Since the location of the SM pool can be set at runtime when using the
 * -memsize flag, the anchor address cannot be known here and must be
 * determined by the peer driver based on the node number and pool size, which
 * must be equal for all nodes.
 */
#ifdef INCLUDE_DSHM_BUS_PLB
const struct hcfResource dshmPeerVxSim0[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)0} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool", 	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};

#if (DSHM_BUS_PLB_MAXNODES > 1)
const struct hcfResource dshmPeerVxSim1[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,  {(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)1} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};
#endif

#if (DSHM_BUS_PLB_MAXNODES > 2)
const struct hcfResource dshmPeerVxSim2[] = {
	{ "regBase",	RES_INT,  {(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)2} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};

const struct hcfResource dshmPeerVxSim3[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)3} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};
#endif

#if (DSHM_BUS_PLB_MAXNODES > 4)
const struct hcfResource dshmPeerVxSim4[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)4} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};

const struct hcfResource dshmPeerVxSim5[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)5} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};

const struct hcfResource dshmPeerVxSim6[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)6} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};

const struct hcfResource dshmPeerVxSim7[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)7} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};
#endif

#if (DSHM_BUS_PLB_MAXNODES > 8)
const struct hcfResource dshmPeerVxSim8[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)8} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};

const struct hcfResource dshmPeerVxSim9[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)9} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};

const struct hcfResource dshmPeerVxSim10[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)10} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};

const struct hcfResource dshmPeerVxSim11[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)11} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};

const struct hcfResource dshmPeerVxSim12[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)12} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};

const struct hcfResource dshmPeerVxSim13[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)13} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};

const struct hcfResource dshmPeerVxSim14[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)14} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};

const struct hcfResource dshmPeerVxSim15[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)15} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};
#endif

#if (DSHM_BUS_PLB_MAXNODES > 16)
const struct hcfResource dshmPeerVxSim16[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)16} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};

const struct hcfResource dshmPeerVxSim17[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)17} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};

const struct hcfResource dshmPeerVxSim18[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)18} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};

const struct hcfResource dshmPeerVxSim19[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)19} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};

const struct hcfResource dshmPeerVxSim20[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)20} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};

const struct hcfResource dshmPeerVxSim21[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)21} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};

const struct hcfResource dshmPeerVxSim22[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)22} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};

const struct hcfResource dshmPeerVxSim23[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)23} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};

const struct hcfResource dshmPeerVxSim24[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)24} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};

const struct hcfResource dshmPeerVxSim25[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)25} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};

const struct hcfResource dshmPeerVxSim26[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)26} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};

const struct hcfResource dshmPeerVxSim27[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)27} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};

const struct hcfResource dshmPeerVxSim28[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)28} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};

const struct hcfResource dshmPeerVxSim29[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)29} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};

const struct hcfResource dshmPeerVxSim30[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)30} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};

const struct hcfResource dshmPeerVxSim31[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)31} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};
#endif /* (DSHM_BUS_PLB_MAXNODES > 16) */

/* For more than 32 simulators participating in dSHM, add the devices'
 * description table following the template below. The only values that need
 * to be updated are the 'node' value and the dshmPeerVxSimX array name: both
 * both must reflect the node address (sequential starting at 32).
 */
#if 0
const struct hcfResource dshmPeerVxSim32[] = {
	{ "regBase",	RES_INT,	{(void *)dshmRegBaseDummy} },
	{ "intr",	RES_INT,	{(void *)-1} },
	{ "intrLevel",	RES_INT,	{(void *)-1} },
	{ "node",	RES_INT,	{(void *)32} },
	{ "anchor",	RES_ADDR,	(void *)-1} },
	{ "szPool",	RES_INT,	{(void *) DSHM_BUS_PLB_POOLSIZE} },
};
#endif /* 0 */

#endif /* INCLUDE_DSHM_BUS_PLB */

const struct hcfResource ipi0Resources[] = {
    { "regBase",	RES_INT, {(void *)-1} },
};

const struct hcfDevice hcfDeviceList[] = {
    { "ipi", 0, VXB_BUSID_PLB, 0, 
        NELEMENTS(ipi0Resources), ipi0Resources },
#ifdef INCLUDE_DSHM_BUS_PLB
    { "dshmBusCtlrSim", 0, VXB_BUSID_PLB, 0,
        NELEMENTS(dshmBusCtlrSimRes), dshmBusCtlrSimRes },
    { "dshmPeerVxSim", 0, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim0), dshmPeerVxSim0 },
#if (DSHM_BUS_PLB_MAXNODES > 1)
    { "dshmPeerVxSim", 1, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim1), dshmPeerVxSim1 },
#endif
#if (DSHM_BUS_PLB_MAXNODES > 2)
    { "dshmPeerVxSim", 2, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim2), dshmPeerVxSim2 },
    { "dshmPeerVxSim", 3, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim3), dshmPeerVxSim3 },
#endif
#if (DSHM_BUS_PLB_MAXNODES > 4)
    { "dshmPeerVxSim", 4, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim4), dshmPeerVxSim4 },
    { "dshmPeerVxSim", 5, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim5), dshmPeerVxSim5 },
    { "dshmPeerVxSim", 6, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim6), dshmPeerVxSim6 },
    { "dshmPeerVxSim", 7, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim7), dshmPeerVxSim7 },
#endif
#if (DSHM_BUS_PLB_MAXNODES > 8)
    { "dshmPeerVxSim", 8, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim8), dshmPeerVxSim8 },
    { "dshmPeerVxSim", 9, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim9), dshmPeerVxSim9 },
    { "dshmPeerVxSim", 10, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim10), dshmPeerVxSim10 },
    { "dshmPeerVxSim", 11, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim11), dshmPeerVxSim11 },
    { "dshmPeerVxSim", 12, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim12), dshmPeerVxSim12 },
    { "dshmPeerVxSim", 13, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim13), dshmPeerVxSim13 },
    { "dshmPeerVxSim", 14, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim14), dshmPeerVxSim14 },
    { "dshmPeerVxSim", 15, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim15), dshmPeerVxSim15 },
#endif
#if (DSHM_BUS_PLB_MAXNODES > 16)
    { "dshmPeerVxSim", 16, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim16), dshmPeerVxSim16 },
    { "dshmPeerVxSim", 17, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim17), dshmPeerVxSim17 },
    { "dshmPeerVxSim", 18, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim18), dshmPeerVxSim18 },
    { "dshmPeerVxSim", 19, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim19), dshmPeerVxSim19 },
    { "dshmPeerVxSim", 20, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim20), dshmPeerVxSim20 },
    { "dshmPeerVxSim", 21, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim21), dshmPeerVxSim21 },
    { "dshmPeerVxSim", 22, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim22), dshmPeerVxSim22 },
    { "dshmPeerVxSim", 23, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim23), dshmPeerVxSim23 },
    { "dshmPeerVxSim", 24, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim24), dshmPeerVxSim24 },
    { "dshmPeerVxSim", 25, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim25), dshmPeerVxSim25 },
    { "dshmPeerVxSim", 26, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim26), dshmPeerVxSim26 },
    { "dshmPeerVxSim", 27, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim27), dshmPeerVxSim27 },
    { "dshmPeerVxSim", 28, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim28), dshmPeerVxSim28 },
    { "dshmPeerVxSim", 29, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim29), dshmPeerVxSim29 },
    { "dshmPeerVxSim", 30, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim30), dshmPeerVxSim30 },
    { "dshmPeerVxSim", 31, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim31), dshmPeerVxSim31 },
#endif

/* For more than 32 simulators participating in dSHM, add the devices to the
 * enumeration table following the template below. Only the values set to '32'
 * in the template must be changed to reflect that device's address (sequential
 * starting at 32).
 */
#if 0
    { "dshmPeerVxSim", 32, VXB_BUSID_VIRTUAL, 0,
         NELEMENTS(dshmPeerVxSim32), dshmPeerVxSim32 },
#endif
#endif /* INCLUDE_DSHM_BUS_PLB */
    { 0, 0, 0, 0, 0, 0 }
};

const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] =
    {
    { NULL, 0, NULL, VXB_PARAM_END_OF_LIST, {(void *)0} }
    };

