/* vxbQorIQRman.c - Freescale QorIQ RMAN VxBus driver */

/*
 * Copyright (c) 2012, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,20may13,j_z  change port mask bit to enable port 1,
                 fix Coverity Prevent issue.(WIND00418128)
01c,08mar13,j_z  add T4240 RMAN support, change fqid distribution method
01b,20nov12,j_z  added support for Type 11 message and scatter-gather
                 mode of Type 9 message, fix rman stop at large flow issue.
01a,03sep12,j_z  written
*/

/*
DESCRIPTION

This module implements driver support for the Freescale QorIQ processor
RapidIO message manager (RMan) module, The RMan is compliant with the
Type9 RapidIO Interconnect Specification Part 2 - Message Passing Logical
Specification Rev. 1.3, and Type11 RapidIO Interconnect Specification
Part 10 - Data Streaming Logical Specification Rev. 1.3a.

It supports RapidIO Type5 outbound NWrite operation, Type6 outbound
Streaming Write, Type8 port-writes inbound/outbound, Type9 data streaming
inbound/outbound, Type10 DoorBell inbound/outbound, Type11 mail box inbound/
outbound. This driver only Type9, type10, type 11 operations. Type9 support
scatter/gather transmit, the max fragmentation number is defined by RMAN_MAXFRAG.
It can transmit max 64K bytes in one transmit. Type 11 only implement mbox/letter
are 0 and use the same frame quene case, the transmit length must multiple of 8 bytes,
otherwise Rman will auto padding it. Qman will quene the send/receive job.

EXTERNAL INTERFACE
The driver provides a vxBus external interface. The only exported
routine is the rmanRegister() function, which registers the driver with
VxBus. Since the queue manager is a processor local bus device, each device
instance must be specified in the hwconf.c file in a BSP. The hwconf entry
entry must specify the following parameters:

\is

\i <regBase>
Specifies the base address where the controller's CSR registers are mapped
into the host's address space. All register offsets are computed relative
to this address.

\i <coreNum>
Specifies this module belongs to which core.

\i <portNum>
Specifies this which port this driver is service.

\i <bufNum>
Specifies the buffer number. Must confirm this number is enough for RMAN
to process the packet.

\i <sizePerBuf>
Specifies each buffer size.

\ie

An example hwconf entry is shown below:

\cs
struct hcfResource rman0Resources[] = {
    { "regBase",      HCF_RES_INT,    { (void *)(CCSBAR + 0x1e0000) } },
    { "portNum",      HCF_RES_INT,    { (void *)0 } },
    { "coreNum",      HCF_RES_INT,    { (void *)0 } },
    { "bufNum",       HCF_RES_INT,    { (void *)2048}},
    { "sizePerBuf",   HCF_RES_INT,    { (void *)1600}},
};
#   define rman0Num NELEMENTS(rman0Resources)
\ce

SEE ALSO: vxBus
\tb  RapidIO Interconnect Specification

*/

#include <vxWorks.h>
#include <intLib.h>
#include <logLib.h>
#include <semLib.h>
#include <sysLib.h>
#include <vxBusLib.h>
#include <wdLib.h>
#include <string.h>

#include <vxAtomicLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/vxbus/vxbPlbLib.h>
#include <hwif/util/vxbDmaBufLib.h>
#include <hwif/util/vxbParamSys.h>
#include <../src/hwif/h/vxbus/vxbAccess.h>
#include <../src/hwif/h/resource/vxbQorIQRman.h>
#include <../src/hwif/h/resource/vxbQorIQFman.h>

#ifdef RMAN_DBG
#   include <private/kwriteLibP.h>
#   undef  LOCAL
#   define LOCAL

#   define DBG_INTR           0x00000001
#   define DBG_INIT           0x00000002
#   define DBG_RW             0x00000004
#   define DBG_ERR            0x00000008
#   define DBG_TEST           0x00000010
#   define DBG_ALL            0xFFFFFFFF
#   define DBG_NONE           0

UINT32 rmanDbgMask = DBG_ERR;

#   define RMAN_LOG(mask, X0, X1, X2, X3, X4, X5, X6) \
       { \
       if (_func_kprintf != NULL) \
          { \
          if(rmanDbgMask & mask) \
              _func_kprintf(X0, (int)X1, (int)X2, (int)X3, (int)X4, \
                  (int)X5, (int)X6);  \
          } \
       }
#else /* RMAN_DBG */
#    define RMAN_LOG(mask, X0, X1, X2, X3, X4, X5, X6)
#endif /* RMAN_DBG */

METHOD_DECL(sysVirtToPhys);
METHOD_DECL(sysPhysToVirt);
LOCAL void   rmanInstInit (VXB_DEVICE_ID);
LOCAL void   rmanInstInit2 (VXB_DEVICE_ID);
LOCAL void   rmanInstConnect (VXB_DEVICE_ID);
LOCAL void   rmanTxHandle (void *, QPORTAL_FDESC *, UINT32, UINT32);
LOCAL void   rmanRxHandle (void *, QPORTAL_FDESC *, UINT32, UINT32);
LOCAL STATUS rmanRxFqInit (RMAN_DRV_CTRL *, IBCU_CONFIG *);
LOCAL STATUS rmanIntRxInstall (VXB_DEVICE_ID, ULONG, MSG_RXCB_FUNC, void *);
LOCAL STATUS rmanCfgIbcu (RMAN_DRV_CTRL *, IBCU_CONFIG *);
LOCAL STATUS rmanIntSend (VXB_DEVICE_ID, ULONG, struct msgbuf *,
          MSG_TXCB_FUNC, void *);
LOCAL STATUS rmanQmanInit (VXB_DEVICE_ID);
LOCAL STATUS rmanBmanInit (VXB_DEVICE_ID);
LOCAL MSG_TRANS_API * rmanMsgAPIGet (VXB_DEVICE_ID);
#ifdef RMAN_DBG
   STATUS rmanShow (VXB_DEVICE_ID, int);
#endif /* RMAN_DBG */



/*Type10 DoorBell inbound/outbound, Type11 mail box inbound/outbound*/

LOCAL MSG_TRANS_API qorIqRmanMsgApi =
    {
    (FUNCPTR)rmanIntRxInstall,
    (FUNCPTR)rmanIntSend,
    (FUNCPTR)NULL,
    (FUNCPTR)NULL,
    (FUNCPTR)NULL
    };

LOCAL struct drvBusFuncs rmanFuncs =
    {
    rmanInstInit,       /* devInstanceInit */
    rmanInstInit2,      /* devInstanceInit2 */
    rmanInstConnect     /* devConnect */
    };

LOCAL device_method_t rman_methods[] =
    {
    DEVMETHOD(vxbMsgApiGet, rmanMsgAPIGet),
#ifdef RMAN_DBG
    DEVMETHOD(busDevShow,   rmanShow),
#endif
    DEVMETHOD_END
    };

LOCAL struct vxbPlbRegister rmanDevPlbRegistration =
    {
    {
    NULL,                   /* pNext */
    VXB_DEVID_DEVICE,       /* devID */
    VXB_BUSID_PLB,          /* busID = PLB */
    VXB_VER_4_0_0,          /* vxbVersion */
    QORIQ_RMAN_NAME,        /* drvName */
    &rmanFuncs,             /* pDrvBusFuncs */
    &rman_methods[0],       /* pMethods */
    NULL,                   /* devProbe */
    NULL                    /* pParamDefaults */
    },
    };

/*****************************************************************************
*
* rmanRegister - register Rman driver into VxBus subsystem
*
* This routine registers the rman driver with VxBus as a
* child of the PLB bus type.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void rmanRegister (void)
    {
    vxbDevRegister ((struct vxbDevRegInfo *)&rmanDevPlbRegistration);
    return;
    }

/*****************************************************************************
*
* rmanInstInit - VxBus instInit handler
*
* This function implements the VxBus instInit handler for an Rman
* device instance.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void rmanInstInit
    (
    VXB_DEVICE_ID pDev
    )
    {
    return;
    }

/*****************************************************************************
*
* rmanInstInit2 - VxBus instInit2 handler
*
* This function implements the VxBus instInit2 handler for an RMAN
* device instance. Once we reach this stage of initialization, it's
* safe for us to allocate memory, so we can create our pDrvCtrl
* structure and do some initial hardware setup.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void rmanInstInit2
    (
    VXB_DEVICE_ID pDev
    )
    {
    RMAN_DRV_CTRL * pDrvCtrl;
    FMAN_DRV_CTRL * pFmanDrvCtrl;
    VXB_DEVICE_ID   pFmanDev;
    const struct hcfDevice *pHcf;
    UINT32 ipCfg;
    IBCU_CONFIG typeXIbcuCfg;

    pDrvCtrl = malloc (sizeof(RMAN_DRV_CTRL));
    if (pDrvCtrl == NULL)
        return;

    bzero ((char *)pDrvCtrl, sizeof(RMAN_DRV_CTRL));
    pDev->pDrvCtrl = pDrvCtrl;

    pDrvCtrl->pDev = pDev;
    vxbRegMap (pDev, 0, &pDrvCtrl->rmanHandle);

    pHcf = hcfDeviceGet (pDev);

    if (pHcf == NULL)
        return;

    /*
     * resourceDesc {
     * The coreNum resource specifies the CPU to which to
     * bind this RMAN instance. It controls which portal
     * the driver will use for its enqueue and dequeue
     * operations. If not specified, it will default to 0. }
     */

    devResourceGet (pHcf, "coreNum", HCF_RES_INT, (void *)&pDrvCtrl->rmanCpu);

    /*
     * resourceDesc {
     * The portNum resource specifies the rman Port number. }
     */

    devResourceGet (pHcf, "portNum", HCF_RES_INT, (void *)&pDrvCtrl->rmanPort);

    if (pDrvCtrl->rmanPort >= RMAN_MAX_PORT)
        {
        free (pDrvCtrl);
        return;
        }
    /*
     * resourceDesc {
     * The resource bpoolNum specifies the Bman buffer pool number. }
     */

    devResourceGet (pHcf, "bufNum", HCF_RES_INT, (void *)&pDrvCtrl->rmanBufNum);

    if (pDrvCtrl->rmanBufNum == 0)
        pDrvCtrl->rmanBufNum =  RMAN_BMANBUF_NUM;

    /*
     * resourceDesc {
     * The bpoolPerSize resource specifies the each Bman buffer pool size. }
     */

    devResourceGet (pHcf, "sizePerBuf", HCF_RES_INT, (void *)&pDrvCtrl->rmanSizePerBuf);

    /*if (pDrvCtrl->rmanSizePerBuf == 0)*/
        pDrvCtrl->rmanSizePerBuf = RMAN_BMANBUF_PERSIZE;

    pDrvCtrl->rmanVirtToPhys =
        (rmanVirtToPhysFunc) vxbDevMethodGet (vxbDevParent (pDev),
        DEVMETHOD_CALL(sysVirtToPhys));
    pDrvCtrl->rmanPhysToVirt =
        (rmanPhysToVirtFunc) vxbDevMethodGet (vxbDevParent (pDev),
        DEVMETHOD_CALL(sysPhysToVirt));

    /* Allocate descriptors (actually scatter/gather lits) */

    pDrvCtrl->pRmanSCdescMem = memalign (_CACHE_ALIGN_SIZE,
        sizeof(RMAN_TX_DESC) * RMAN_TX_DESC_CNT);

    bzero ((char *)pDrvCtrl->pRmanSCdescMem,
        sizeof(RMAN_TX_DESC) * RMAN_TX_DESC_CNT);

    pDrvCtrl->rmanSem = semMCreate (SEM_Q_PRIORITY|
        SEM_DELETE_SAFE|SEM_INVERSION_SAFE);

    pDrvCtrl->rmanTxFree = RMAN_TX_DESC_CNT;
    pDrvCtrl->rmanTxProd = 0;

    /*
     * Note: this buffer pool id should be unique within the system.
     * The value RMAN_BPID is used as a bias just so that the RMAN buffer
     * pool numbers won't collide with the TGEC/DTSEC buffer pool numbers.
     */

    pDrvCtrl->rmanBpid = RMAN_BPID + pDrvCtrl->rmanPort;
    pDrvCtrl->rmanSgBpid  = pDrvCtrl->rmanBpid + RMAN_MAX_PORT;

    /*
     * We need to chose frame queue IDs that are unique on a
     * system-wide basis, since the queue manager portals on all
     * cores interact with each other, and since we don't want
     * frame queue IDs for different ports to overlap. We use
     * the convention that the RMAN_RX_FQID, RMAN_TX_FQID values
     * are 'magic' and that no other driver in the system will
     * use them. Don't conflict with other module
     * which use DPAA module, DTSEC/TGSEC
     */


    if (pDrvCtrl->rmanPort == 0)
        {
        pDrvCtrl->rmanTxFqId = RMAN_FQID_PORT0_POOL ;
        pDrvCtrl->rmanTxDoneFqId = RMAN_FQID_PORT0_POOL + 1;
        pDrvCtrl->rmanRxFqId = RMAN_FQID_PORT0_POOL + 2;
        }

    if (pDrvCtrl->rmanPort == 1)
        {
        pDrvCtrl->rmanTxFqId = RMAN_FQID_PORT1_POOL ;
        pDrvCtrl->rmanTxDoneFqId = RMAN_FQID_PORT1_POOL + 1;
        pDrvCtrl->rmanRxFqId = RMAN_FQID_PORT1_POOL + 2;
        }

    ipCfg = CSR_READ_4(pDev, RMAN_IPBRR1);

    pDrvCtrl->rmanIbNum = 2 << (((ipCfg >> 6) & 0x3) + 1);

    pFmanDev = vxbInstByNameFind ("QorIQFman", 0);
    if (pFmanDev != NULL)
        {
        pFmanDrvCtrl = (FMAN_DRV_CTRL *)(pFmanDev->pDrvCtrl);

        if (FMAN_MJ(pFmanDrvCtrl->fmanRev) == FMAN_REVISION6)
            pDrvCtrl->rmanWqChan = RMAN_QMAN_CHANNEL_T4240 + pDrvCtrl->rmanPort;
        else
            pDrvCtrl->rmanWqChan = RMAN_QMAN_CHANNEL + pDrvCtrl->rmanPort;
        }
    else
        return;

    qportalAcquire (pDrvCtrl->rmanCpu, &pDrvCtrl->rmanQportal);
    bportalAcquire (pDrvCtrl->rmanCpu, &pDrvCtrl->rmanBportal);

    /* Setup callouts */

    qportalCalloutAdd (pDrvCtrl->rmanQportal, rmanRxHandle,
        pDrvCtrl, pDrvCtrl->rmanRxFqId + RMAN_TYPE9_FQID_OFFSET,
        &pDrvCtrl->rmanRxCtx[RMAN_TYPE9_FQID_OFFSET]);

    qportalCalloutAdd (pDrvCtrl->rmanQportal, rmanRxHandle,
        pDrvCtrl, pDrvCtrl->rmanRxFqId+ RMAN_TYPE10_FQID_OFFSET,
        &pDrvCtrl->rmanRxCtx[RMAN_TYPE10_FQID_OFFSET]);

    qportalCalloutAdd (pDrvCtrl->rmanQportal, rmanRxHandle,
        pDrvCtrl, pDrvCtrl->rmanRxFqId + RMAN_TYPE11_FQID_OFFSET,
         &pDrvCtrl->rmanRxCtx[RMAN_TYPE11_FQID_OFFSET]);

    qportalCalloutAdd (pDrvCtrl->rmanQportal, rmanTxHandle,
        pDrvCtrl, pDrvCtrl->rmanTxDoneFqId, &pDrvCtrl->rmanTxDoneCtx);

    /* Create QMAN frame queues */

    rmanQmanInit (pDev);

    /* Populate RX pool */

    rmanBmanInit (pDev);

    if (pDev->unitNumber == 0)
        {
        CSR_WRITE_4(pDev, RMAN_MMMR, RMAN_RESET);

        CSR_WRITE_4(pDev, RMAN_MMEDR, MMEDR_CLEAR);
        CSR_WRITE_4(pDev, RMAN_MMIER, MMIER_ENABLE_ALL);
        }

    typeXIbcuCfg.port = pDrvCtrl->rmanPort;
    typeXIbcuCfg.portMask = 0x0;
    typeXIbcuCfg.sid = 0;
    typeXIbcuCfg.sidMask = 0xFFFF;
    typeXIbcuCfg.did = 0;
    typeXIbcuCfg.didMask = 0xFFFF;
    typeXIbcuCfg.fqid = pDrvCtrl->rmanRxFqId + RMAN_TYPE10_FQID_OFFSET;
    typeXIbcuCfg.fqMode = DIRECT;
    typeXIbcuCfg.bpid = pDrvCtrl->rmanBpid;
    typeXIbcuCfg.sgbpid = 0;
    typeXIbcuCfg.msgsize = pDrvCtrl->rmanSizePerBuf & 0xFFFF;
    
    typeXIbcuCfg.sgsize = 0;
    typeXIbcuCfg.dataoffset = RM_DATA_OFFSET;
    typeXIbcuCfg.tran.type  = RIO_TYPE_DBELL;
    typeXIbcuCfg.tran.flowlvl = 5;
    typeXIbcuCfg.tran.flowlvlMask = 1;
    rmanRxFqInit (pDrvCtrl, &typeXIbcuCfg);

    bzero ((char *)&typeXIbcuCfg, sizeof(IBCU_CONFIG));

    typeXIbcuCfg.port = pDrvCtrl->rmanPort;
    typeXIbcuCfg.portMask = 0x0;
    typeXIbcuCfg.sid = 0xFF;
    typeXIbcuCfg.sidMask = 0xFF;
    typeXIbcuCfg.did = 0xFF;
    typeXIbcuCfg.didMask = 0xFF;
    typeXIbcuCfg.fqid = pDrvCtrl->rmanRxFqId + RMAN_TYPE9_FQID_OFFSET;
    typeXIbcuCfg.fqMode = DIRECT;
    typeXIbcuCfg.bpid = pDrvCtrl->rmanBpid;
    typeXIbcuCfg.sgbpid = pDrvCtrl->rmanSgBpid;
    typeXIbcuCfg.msgsize = pDrvCtrl->rmanSizePerBuf & 0xFFFF;
    typeXIbcuCfg.sgsize = RMAN_SGTABLE_PERSIZE;
    typeXIbcuCfg.dataoffset = RM_DATA_OFFSET;
    typeXIbcuCfg.tran.type = RIO_TYPE_DSTR;
    typeXIbcuCfg.tran.flowlvl = 0;
    typeXIbcuCfg.tran.flowlvlMask = 2;
    typeXIbcuCfg.tran.dstr.streamid = 0;
    typeXIbcuCfg.tran.dstr.streamidMask = 0x1F;;
    typeXIbcuCfg.tran.dstr.cos = 0;
    typeXIbcuCfg.tran.dstr.cosMask = 0;

    rmanRxFqInit (pDrvCtrl, &typeXIbcuCfg);

    /* only implement mbox = letter = 0 */

    bzero ((char *)&typeXIbcuCfg, sizeof(IBCU_CONFIG));
    typeXIbcuCfg.port = pDrvCtrl->rmanPort;
	logMsg("rman port 0x%x \n",typeXIbcuCfg.port,0,0,0,0,0);
    typeXIbcuCfg.portMask = 0x0;
    typeXIbcuCfg.sid = 0;
    typeXIbcuCfg.sidMask = 0xFF;
    typeXIbcuCfg.did = 0;
    typeXIbcuCfg.didMask = 0xFF;
    typeXIbcuCfg.fqid = pDrvCtrl->rmanRxFqId + RMAN_TYPE11_FQID_OFFSET;
    typeXIbcuCfg.fqMode = DIRECT;
    typeXIbcuCfg.bpid = pDrvCtrl->rmanBpid;
    typeXIbcuCfg.sgbpid = 0;
    typeXIbcuCfg.msgsize = pDrvCtrl->rmanSizePerBuf & 0xFFFF;
    typeXIbcuCfg.sgsize =  0;
    typeXIbcuCfg.dataoffset = RM_DATA_OFFSET;
    typeXIbcuCfg.tran.type = RIO_TYPE_MBOX;
    typeXIbcuCfg.tran.flowlvl = 5;
    typeXIbcuCfg.tran.flowlvlMask = 1;
    typeXIbcuCfg.tran.mbox.mbox = 0;
    typeXIbcuCfg.tran.mbox.mboxMask = 0x3;
    typeXIbcuCfg.tran.mbox.ltr = 2;
    typeXIbcuCfg.tran.mbox.ltrMask = 0x3;
    typeXIbcuCfg.tran.mbox.msglen = RMAN_MBOX_SEG_NUM;
    typeXIbcuCfg.tran.mbox.msglenMask = 1;

    rmanRxFqInit (pDrvCtrl, &typeXIbcuCfg);

    return;
    }

/*****************************************************************************
*
* rmanInstConnect - VxBus instConnect handler
*
* This function implements the VxBus instConnect handler for an Rman
* device instance.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void rmanInstConnect
    (
    VXB_DEVICE_ID pDev
    )
    {
    return;
    }

/*******************************************************************************
*
* rmanMsgAPIGet - method to get message API
*
* This function return common message transmit API, it only be called by vxbDevMethodGet
*
* RETURNS: &qorIqRmanMsgApi
*
*/

LOCAL MSG_TRANS_API * rmanMsgAPIGet
    (
    VXB_DEVICE_ID pDev
    )
    {
    return &qorIqRmanMsgApi;
    }

/*****************************************************************************
*
* rmanBmanInit -  Bman buffer initialize
*
* This routine initializes the buffer pool for Rman.
*
* RETURNS: ERROR/OK
*
* ERRNO: N/A
*/

LOCAL STATUS rmanBmanInit
    (
    VXB_DEVICE_ID pDev
    )
    {
    RMAN_DRV_CTRL * pDrvCtrl;
    UINT64 *pBuf = NULL;
    UINT64 addr;
    int i;

    pDrvCtrl = pDev->pDrvCtrl;

    pBuf = (UINT64 *)malloc(pDrvCtrl->rmanBufNum * sizeof(UINT64));
    if (pBuf == NULL)
        return ERROR;

    /* Drain any buffers still present in our chosen pool. */

    while (bportalCountGet (pDrvCtrl->rmanBportal, pDrvCtrl->rmanBpid))
        {
        if (bportalLocalAlloc (pDrvCtrl->rmanBpid, &addr) == ERROR)
            break;
        }

    while (bportalCountGet (pDrvCtrl->rmanBportal, pDrvCtrl->rmanSgBpid))
        {
        if (bportalLocalAlloc (pDrvCtrl->rmanSgBpid, &addr) == ERROR)
            break;
        }

    /* Load the buffer manager with some buffers */

    for (i = 0; i < pDrvCtrl->rmanBufNum; i++)
        {
        pBuf[i] = memalign (_CACHE_ALIGN_SIZE, pDrvCtrl->rmanSizePerBuf);
        if (pBuf[i] == NULL)
            {
            free (pBuf);
            return (ERROR);
            }

        addr = RMAN_VTOP(pDrvCtrl, pBuf[i]);
        if (bportalLocalFree (pDrvCtrl->rmanBpid, addr) == ERROR)
            {
            free (pBuf);
            return (ERROR);
            }
        }

    for (i = 0; i < pDrvCtrl->rmanBufNum; i++)
        {
        pBuf[i] = memalign (_CACHE_ALIGN_SIZE, RMAN_SGTABLE_PERSIZE);
        if (pBuf[i] == NULL)
            {
            free (pBuf);
            return (ERROR);
            }

        addr = RMAN_VTOP(pDrvCtrl, pBuf[i]);
        if (bportalLocalFree (pDrvCtrl->rmanSgBpid, addr) == ERROR)
            {
            free (pBuf);
            return (ERROR);
            }
        }

    free (pBuf);

    return (OK);
    }

/*****************************************************************************
*
* rmanReqIbcu -  requests inbound traffic classification units
*
* This function requests an inbound traffic classification units.
*
* RETURNS: ERROR/IBCU number
*
* ERRNO: N/A
*/

LOCAL int rmanReqIbcu
    (
    RMAN_DRV_CTRL * pDrvCtrl,
    int             fqid
    )
    {
    int ibIdx, cuIdx;

    if (!fqid)
        return ERROR;

    /* let differ port use different ibcu */

    for (ibIdx = 0; ibIdx < pDrvCtrl->rmanIbNum; ibIdx++)
        {
        for (cuIdx = 0; cuIdx < RMAN_CU_NUM_PER_IB; cuIdx++)
            if (((CSR_READ_4(pDrvCtrl->pDev, RMAN_IBCU_MR(ibIdx, cuIdx)) &
                RMAN_IBCU_ENABLE) == 0) && (CSR_READ_4(pDrvCtrl->pDev,
                RMAN_IBCU_FQR(ibIdx, cuIdx)) == 0))
                {
                RMAN_LOG(DBG_INIT,"rmanGetIbcu(): ibIdx %d cuIdx %d 0x%x 0x%x\r\n", ibIdx,
                    cuIdx, CSR_READ_4(pDrvCtrl->pDev, RMAN_IBCU_MR(ibIdx, cuIdx)),
                    CSR_READ_4(pDrvCtrl->pDev, RMAN_IBCU_FQR(ibIdx, cuIdx)), 5, 6);
                return ibIdx * RMAN_CU_NUM_PER_IB + cuIdx;
                }
        }

    return ERROR;
    }

/*****************************************************************************
*
* rmanRxFqInit -  initialize inbound traffic classification units
*
* This function initialize inbound traffic classification units
*
* RETURNS: ERROR/OK
*
* ERRNO: N/A
*/

LOCAL STATUS rmanRxFqInit
    (
    RMAN_DRV_CTRL   * pDrvCtrl,
    IBCU_CONFIG     * rxcfg
    )
    {
    int ibcu;

    if (!rxcfg->fqid)
        return ERROR;

    ibcu = rmanReqIbcu (pDrvCtrl, rxcfg->fqid);
    if (ibcu < 0)
        {
        RMAN_LOG(DBG_ERR, "rmanRxFqInit():fqid(0x%x) failed to request ibcu resource \r\n",
              rxcfg->fqid, 2, 3, 4, 5, 6);
        return ERROR;
        }
    RMAN_LOG(DBG_INIT, "rmanRxFqInit():Bind FQID 0x%x to IBCU %d \r\n", rxcfg->fqid, ibcu, 3,4,5,6);

    rxcfg->ibcu = ibcu;

    return rmanCfgIbcu (pDrvCtrl, rxcfg);
    }

/*****************************************************************************
*
* rmanTxHandle -  Rman TX interrupt handler
*
* This function implements the Rman TX interrupt handler
*
* RETURNS: ERROR/OK
*
* ERRNO: N/A
*/

LOCAL void rmanTxHandle
    (
    void          * arg,
    QPORTAL_FDESC * pFd,
    UINT32          fqId,
    UINT32          ctxB
    )
    {
    UINT32 type;
    RMAN_DRV_CTRL * pDrvCtrl;

    type = pFd->qportalCmd >> 28;

    RMAN_LOG(DBG_INTR, "rmanTxHandle():rmanTxHandle 0x%X  0x%X  0x%X  0x%X  0x%X 0x%X \r\n",
        pFd->qportalBpid, fqId, pFd->qportalAddrLo,
        pFd->qportalOffLen, pFd->qportalCmd, type);

    if (type >= RIO_TYPE_NUM)
        return;

    pDrvCtrl = (RMAN_DRV_CTRL *)arg;

    if ((pFd->qportalCmd & 0xFFFF) != 0)
       {
       CSR_WRITE_4(pDrvCtrl->pDev, RMAN_MMEDR,
           CSR_READ_4(pDrvCtrl->pDev, RMAN_MMEDR));
       pDrvCtrl->rmanTxErrPkts ++;
       }

    /* type9 use Scatter/Gather Table, type10 use Bpid */

    if (type == RIO_TYPE9)
        {
        RMAN_LOG(DBG_INTR, "rmanTxHandle():rmanTxFree = %d \r\n",pDrvCtrl->rmanTxFree, 2,3,4,5,6);
        semTake (pDrvCtrl->rmanSem, WAIT_FOREVER);
        pDrvCtrl->rmanTxFree++;
        semGive (pDrvCtrl->rmanSem);
        }

    if (pDrvCtrl->typeXCallBackArg[type].pTxCallBack != NULL)
        (pDrvCtrl->typeXCallBackArg[type].pTxCallBack)
         ((STATUS)(pFd->qportalCmd & 0xFFFF), pDrvCtrl->typeXCallBackArg[type].pTxArg);
    }

/*****************************************************************************
*
* rmanRxHandle - Rman RX interrupt handler
*
* This function implements the Rman RX interrupt handler
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void rmanRxHandle
    (
    void          * arg,
    QPORTAL_FDESC * pFd,
    UINT32          fqId,
    UINT32          ctxB
    )
    {
    RMAN_SC_TABLE * scTable;
    RMAN_SC_TABLE * scTableFirst;
    struct rmanIbMd * ibDesc = NULL;
    UINT64 dataAddr = 0, releaseAddr = 0;
    UINT32 type, dataLen = 0, i;
    RMAN_DRV_CTRL * pDrvCtrl = NULL;
    struct msgbuf bufdesc [RMAN_MAXFRAG];
	INT32 sid = 0;
	INT32 did = 0;

    pDrvCtrl = (RMAN_DRV_CTRL *)arg;

    /*
     * T4240 Rman discard type field in qportalCmd type = pFd->qportalCmd >> 28.
     * Then change fqid distribution method, use fqid to get type.
     */

    type = fqId - pDrvCtrl->rmanRxFqId + RIO_TYPE9;

    RMAN_LOG(DBG_INTR, "rmanRxHandle():in 0x%X 0x%X 0x%X 0x%X 0x%X %p \r\n",
        type,fqId,pFd->qportalAddrLo,pFd->qportalOffLen,
        pFd->qportalCmd, arg);

    if (type >= RIO_TYPE_NUM)
        return;

    if ((pFd->qportalCmd & 0xFFFF) != 0)
        {
        UINT32 status;
        status = CSR_READ_4(pDrvCtrl->pDev, RMAN_MMEDR);
        CSR_WRITE_4(pDrvCtrl->pDev, RMAN_MMEDR, status);
        pDrvCtrl->rmanRxErrPkts ++;
        RMAN_LOG(DBG_INTR,"rmanRxHandle():rxInt err 0x%X\r\n", pFd->qportalCmd & 0xFFFF,2,3,4,5,6);
        }

    bzero ((char *)bufdesc, sizeof (struct msgbuf) * RMAN_MAXFRAG);
    if ((type == RIO_TYPE10) || (type == RIO_TYPE11))
        {
        ibDesc = (struct rmanIbMd *)(ULONG)((UINT64)(pFd->qportalAddrLo) |
            ((UINT64)pFd->qportalAddrHi << 32));
        if (ibDesc == NULL)
            return;
        
		sid = ibDesc->sid; 
		did = ibDesc->did; 
		
        dataAddr = (UINT64)(ULONG)ibDesc + (UINT64)((pFd->qportalOffLen >> 20) & 0x1fff);
        dataLen = pFd->qportalOffLen & 0xFFFFFF;
        releaseAddr = (UINT64)(ULONG)ibDesc;
        bufdesc[0].buf = RMAN_PTOV(pDrvCtrl, (ULONG)dataAddr);
        bufdesc[0].len = dataLen;
        bufdesc[0].next = NULL;
        RMAN_LOG(DBG_ERR, "rmanRxHandle():bufdesc[0].buf = 0x%x bufdesc[0].len = 0x%x \r\n",
            bufdesc[0].buf, bufdesc[0].len, 3,4,5,6);
        }

    else if (type == RIO_TYPE9)
        {
        scTable = (RMAN_SC_TABLE *)(ULONG)((UINT64)(pFd->qportalAddrLo) |
            ((UINT64)pFd->qportalAddrHi << 32));

        if (scTable == NULL)
            return;

        scTableFirst = scTable;

        for (i = 0; i < RMAN_MAXFRAG; i++, scTable ++)
            {
            if (i == 0)
                {
                ibDesc = (struct rmanIbMd *)(ULONG)((UINT64)(scTable->addrLo) |
                    ((UINT64)scTable->addrHi << 32));
                if (ibDesc == NULL)
                    break;
                releaseAddr = (UINT64)(ULONG)ibDesc;
                bufdesc[i].buf = RMAN_PTOV(pDrvCtrl, (ULONG)ibDesc + (scTable->bpidOffset & 0x1fff));
                bufdesc[i].len = scTable->efLen & 0x3fffffff;
                RMAN_LOG(DBG_INTR, "rmanRxHandle():bufdesc[%d].buf = 0x%x "
                    "bufdesc[%d].len = 0x%x 0x%x\r\n", i,
                    bufdesc[i].buf, i, bufdesc[i].len, scTable->bpidOffset, 6);
                }
            else
                {
                bufdesc[i].buf = RMAN_PTOV(pDrvCtrl, (UINT64)(scTable->addrLo) |
                    ((UINT64)scTable->addrHi << 32));
                bufdesc[i].len = scTable->efLen & 0x3fffffff;
                RMAN_LOG(DBG_INTR, "rmanRxHandle():bufdesc[%d].buf = 0x%x "
                    "bufdesc[%d].len=0x%x 0x%x\r\n",i,
                    bufdesc[i].buf, i, bufdesc[i].len, scTable->bpidOffset, 6);
                }

            RMAN_LOG(DBG_INTR, "rmanRxHandle():0x%x 0x%x \r\n", (ULONG)scTable, scTable->efLen,
                    3, 4, 5, 6);

            if ((scTable->efLen & RMAN_SC_F_ENTRY) == 0)
                bufdesc[i].next = &bufdesc[i + 1];
            else
                {
                bufdesc[i].next = NULL;
                break;
                }
            }

        if (scTableFirst != NULL)
            {
            (void)bportalLocalFree (pDrvCtrl->rmanSgBpid,
                (UINT64)(ULONG)scTableFirst);
            }
        }

    RMAN_LOG(DBG_INTR, "rmanRxHandle(): %p  0x%llx  0x%x  0x%llx  %p %d\r\n",
        pDrvCtrl->typeXCallBackArg[type].pRxCallBack, dataAddr, dataLen,
        ibDesc, pDrvCtrl->typeXCallBackArg[type].pRxArg, ibDesc->sint);

  /*RIO_TYPE11  DBELL*/
  if(type == RIO_TYPE10)
  {
            if (pDrvCtrl->typeXCallBackArg[type].pRxCallBack != NULL)
        (pDrvCtrl->typeXCallBackArg[type].pRxCallBack)
        (sid, did, *(UINT16 *)bufdesc->buf);
  }
  else
  {
      if (pDrvCtrl->typeXCallBackArg[type].pRxCallBack != NULL)
        (pDrvCtrl->typeXCallBackArg[type].pRxCallBack)(&bufdesc[0],
            (STATUS)(pFd->qportalCmd & 0xFFFF),
                pDrvCtrl->typeXCallBackArg[type].pRxArg);
  }
    

    /* release first data buffer */

    if (releaseAddr != 0)
        (void)bportalLocalFree (pDrvCtrl->rmanBpid, releaseAddr);

    pDrvCtrl->rmanRxBytes += bufdesc[0].len;

    if (type == RIO_TYPE9)
        {
        if (bufdesc[0].next == NULL)
            return;

        for (i = 1; i < RMAN_MAXFRAG; i ++)
            {
            if (bufdesc[i].buf != NULL)
                (void)bportalLocalFree (pDrvCtrl->rmanBpid,
                   RMAN_VTOP(pDrvCtrl, bufdesc[i].buf));

            pDrvCtrl->rmanRxBytes += bufdesc[i].len;
            if (bufdesc[i].next == NULL)
                break;
            }
        }
    }

/*****************************************************************************
*
* rmanQmanInit -  initialize QMAN for RMAN
*
* This function initialize QMAN
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL STATUS rmanQmanInit
    (
    VXB_DEVICE_ID pDev
    )
    {
    RMAN_DRV_CTRL *pDrvCtrl = pDev->pDrvCtrl;

    pDrvCtrl = pDev->pDrvCtrl;

    if (qportalFqInit (pDrvCtrl->rmanQportal, pDrvCtrl->rmanRxFqId +
        RMAN_TYPE9_FQID_OFFSET,
        pDrvCtrl->rmanQportal->qportalIdx, 0, pDrvCtrl->rmanRxCtx[RMAN_TYPE9_FQID_OFFSET],
        0, 0, 0) != OK)
        return (ERROR);

    if (qportalFqInit (pDrvCtrl->rmanQportal, pDrvCtrl->rmanRxFqId +
        RMAN_TYPE10_FQID_OFFSET,
        pDrvCtrl->rmanQportal->qportalIdx, 0,
        pDrvCtrl->rmanRxCtx[RMAN_TYPE10_FQID_OFFSET],
        0, 0, 0) != OK)
        return (ERROR);

    if (qportalFqInit (pDrvCtrl->rmanQportal, pDrvCtrl->rmanRxFqId + 
        RMAN_TYPE11_FQID_OFFSET,
        pDrvCtrl->rmanQportal->qportalIdx, 0,
        pDrvCtrl->rmanRxCtx[RMAN_TYPE11_FQID_OFFSET],
        0, 0, 0) != OK)
        return (ERROR);

    if (qportalFqInit (pDrvCtrl->rmanQportal, pDrvCtrl->rmanTxFqId,
        pDrvCtrl->rmanWqChan, 0, 0xDEADBEEF, 0, 0, 0) != OK)
        return (ERROR);

    if (qportalFqInit (pDrvCtrl->rmanQportal, pDrvCtrl->rmanTxDoneFqId,
        pDrvCtrl->rmanQportal->qportalIdx, 0, pDrvCtrl->rmanTxDoneCtx,
        0, 0, 0) != OK)
        return ERROR;

    return (OK);
    }

/*****************************************************************************
*
* rmanCfgIbcu - configure an inbound traffic classification units
*
* This function initialize an inbound traffic classification units.
*
* RETURNS: OK/ERROR
*
* ERRNO: N/A
*/

LOCAL STATUS rmanCfgIbcu
    (
    RMAN_DRV_CTRL * pDrvCtrl,
    IBCU_CONFIG   * cfg
    )
    {
    VXB_DEVICE_ID pDev = pDrvCtrl->pDev;
    int ibIdx, cuIdx;

    ibIdx = cfg->ibcu / RMAN_CU_NUM_PER_IB;
    cuIdx = cfg->ibcu % RMAN_CU_NUM_PER_IB;

    RMAN_LOG(DBG_INIT, "rmanCfgIbcu():%d ibIdx %d cuIdx %d 0x%x\r\n", cfg->ibcu,
        ibIdx, cuIdx, cfg->fqid,5,6);

    CSR_WRITE_4(pDev, RMAN_IBCU_FQR(ibIdx, cuIdx), cfg->fqid);
    CSR_WRITE_4(pDev, RMAN_IBCU_RVR0(ibIdx, cuIdx), cfg->sid << 16 | cfg->did);
    CSR_WRITE_4(pDev, RMAN_IBCU_RMR0(ibIdx, cuIdx),
        cfg->sidMask << 16 | cfg->didMask);

    switch (cfg->tran.type)
        {
        case RIO_TYPE_DBELL:
            CSR_WRITE_4(pDev, RMAN_IBCU_RVR1(ibIdx, cuIdx),
                (cfg->tran.flowlvl << RMAN_IBCU_FLVMASK_SHIT) |
                (cfg->port << RMAN_IBCU_PORT_SHIT));
            CSR_WRITE_4(pDev, RMAN_IBCU_RMR1(ibIdx, cuIdx),
                cfg->tran.flowlvl << RMAN_IBCU_FLVMASK_SHIT |
                cfg->portMask << RMAN_IBCU_PORT_SHIT);
            CSR_WRITE_4(pDev, RMAN_IBCU_DBPR(ibIdx, cuIdx),
                (cfg->msgsize << 16) | cfg->bpid);
            CSR_WRITE_4(pDev, RMAN_IBCU_DOR(ibIdx, cuIdx),
                cfg->dataoffset);
            break;
        case RIO_TYPE_MBOX:
            CSR_WRITE_4(pDev, RMAN_IBCU_RVR1(ibIdx, cuIdx),
                cfg->tran.flowlvl << RMAN_IBCU_FLVMASK_SHIT |
                cfg->port << RMAN_IBCU_PORT_SHIT |
                cfg->tran.mbox.msglen << 8 |
                cfg->tran.mbox.ltr << 6 |
                cfg->tran.mbox.mbox);
            CSR_WRITE_4(pDev, RMAN_IBCU_RMR1(ibIdx, cuIdx),
                cfg->tran.flowlvlMask << RMAN_IBCU_FLVMASK_SHIT |
                cfg->portMask << RMAN_IBCU_PORT_SHIT |
                cfg->tran.mbox.msglenMask << 8 |
                cfg->tran.mbox.ltrMask << 6 |
                cfg->tran.mbox.mboxMask);
            CSR_WRITE_4(pDev, RMAN_IBCU_DBPR(ibIdx, cuIdx),
                (cfg->msgsize << 16) | cfg->bpid);
            CSR_WRITE_4(pDev, RMAN_IBCU_DOR(ibIdx, cuIdx),
                cfg->dataoffset);
            break;
        case RIO_TYPE_DSTR:
            CSR_WRITE_4(pDev, RMAN_IBCU_RVR1(ibIdx, cuIdx),
                cfg->tran.flowlvl << RMAN_IBCU_FLVMASK_SHIT |
                cfg->port << RMAN_IBCU_PORT_SHIT |
                cfg->tran.dstr.cos << 16 |
                cfg->tran.dstr.streamid);
            CSR_WRITE_4(pDev, RMAN_IBCU_RMR1(ibIdx, cuIdx),
                cfg->tran.flowlvlMask << RMAN_IBCU_FLVMASK_SHIT |
                cfg->portMask << RMAN_IBCU_PORT_SHIT |
                cfg->tran.dstr.cosMask << 16 |
                cfg->tran.dstr.streamidMask);
            CSR_WRITE_4(pDev, RMAN_IBCU_DBPR(ibIdx, cuIdx),
                (cfg->msgsize << 16) | cfg->bpid);
            CSR_WRITE_4(pDev, RMAN_IBCU_T9SGBPR(ibIdx, cuIdx),
                (cfg->sgsize << 16) | cfg->sgbpid);
            CSR_WRITE_4(pDev, RMAN_IBCU_DOR(ibIdx, cuIdx),
                cfg->dataoffset);
            break;
        default:
            return ERROR;
        }

    CSR_WRITE_4(pDev, RMAN_IBCU_MR(ibIdx, cuIdx),
        RMAN_IBCU_ENABLE | (cfg->fqMode << RMAN_IBCU_FQMODE_SHIT) |
        (cfg->tran.type << RMAN_IBCU_TRANTYPE_SHIT));

    RMAN_LOG(DBG_INIT,"rmanCfgIbcu(): %d %d 0x%x 0x%x \r\n", ibIdx, cuIdx,
                    CSR_READ_4(pDev, RMAN_IBCU_MR(ibIdx, cuIdx)),
                    CSR_READ_4(pDev, RMAN_IBCU_FQR(ibIdx, cuIdx)),5,6);

    return OK;
    }

/*****************************************************************************
*
* rmanIntRxInstall - interrupt receive callback register function
*
* This function register the interrupt receive callback routine.
*
* RETURNS: OK/ERROR
*
* ERRNO: N/A
*/

LOCAL STATUS rmanIntRxInstall
    (
    VXB_DEVICE_ID   pDev,
    ULONG           arg,
    MSG_RXCB_FUNC   rxCallback,
    void          * cbArg
    )
    {
    RMAN_DRV_CTRL * pDrvCtrl;
    RIO_TRANS_PARAMETER * transArg = (RIO_TRANS_PARAMETER *)arg;
    UINT16 type = transArg->transType;

    if(type >= RIO_TYPE_NUM)
        return ERROR;

    pDrvCtrl = pDev->pDrvCtrl;
    if (pDrvCtrl == NULL)
        return ERROR;

    pDrvCtrl->typeXCallBackArg[type].pRxCallBack = rxCallback;
    pDrvCtrl->typeXCallBackArg[type].pRxArg = cbArg;

    RMAN_LOG(DBG_INIT, "rmanIntRxInstall(): pDev = 0x%x  pDrvCtrl =%X %X %d %d \r\n",
        pDev, pDrvCtrl, pDrvCtrl->typeXCallBackArg[type].pRxCallBack,
        pDrvCtrl->rmanPort, type, 6);

    return OK;
    }

/*****************************************************************************
*
* rmanTxType9 - Type9 (Data stream send function)
*
* This routine implements Type9 transimission. It can send
* 64K byte each time. When use fragmentation, the total length of all segment
* can't exceed 64K bytes.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL int rmanTxType9
    (
    RMAN_DRV_CTRL   * pDrvCtrl,
    struct msgbuf   * desc,
    UINT16            destId
    )
    {
    QPORTAL_FDESC qPortDesc;
    volatile struct rmanObMd * md;
    STATUS ret;
    UINT64 addr;
    UINT32 offset = 0, used, length = 0;
    volatile RMAN_TX_DESC * pScDesc = NULL;
    struct msgbuf  * pCurBufDesc;
    volatile QPORTAL_SGLIST * pTx = NULL;

    if (desc->len == 0)
        return ERROR;

    semTake (pDrvCtrl->rmanSem, WAIT_FOREVER);

    if (pDrvCtrl->rmanTxFree ==0)
        {
        semGive (pDrvCtrl->rmanSem);
        return ERROR;
        }

    pScDesc = &pDrvCtrl->pRmanSCdescMem [pDrvCtrl->rmanTxProd];

    md = &pScDesc->md;

    md->ftype = RIO_TYPE_DSTR;
    md->br = 0 ;   /* buffer release */
    md->so = 0 ;
    md->cs = 1 ;   /* trans complete */
    md->es = 1 ;
    md->statusFqid = pDrvCtrl->rmanTxDoneFqId;
    md->count = desc->len;
    md->flowlvl = 5;
    md->tint = pDrvCtrl->rmanPort;
    md->did =  destId;
    md->otherAttr = 0  /* tran.dstr.cos*/;

    pTx = &pScDesc->rman_frag[0];

    /* must use first frag desc to save md */

    addr = RMAN_VTOP(pDrvCtrl, (void *)md);
    pTx->qportalAddrHi = RMAN_ADDR_HI(addr);
    pTx->qportalAddrLo = RMAN_ADDR_LO(addr);
    pTx->qportalLength = 0;
    pTx->qportalBpid   = 0;
    pTx->qportalOffset = 0;

    for (pCurBufDesc = desc, used = 1; pCurBufDesc != NULL; pCurBufDesc = pCurBufDesc->next)
        {
        RMAN_LOG(DBG_RW, "rmanTxType9():pCurBufDesc %p used %d \r\n", pCurBufDesc,
            used, 3, 4,5,6);

        if (pCurBufDesc->len != 0)
            {
            if (used == (RMAN_MAXFRAG + 1))
                break;

            pTx = &pScDesc->rman_frag[used];
            addr = RMAN_VTOP(pDrvCtrl, pCurBufDesc->buf);
            pTx->qportalAddrLo = RMAN_ADDR_LO(addr);
            pTx->qportalAddrHi = RMAN_ADDR_HI(addr);
            pTx->qportalLength = pCurBufDesc->len;
            pTx->qportalBpid = 0;
            pTx->qportalOffset = 0;
            used ++;
            length += pCurBufDesc->len;
            }
         }

    if (pCurBufDesc != NULL)
        {
        RMAN_LOG(DBG_ERR, "rmanTxType9():no sup %d \r\n", pDrvCtrl->rmanTxFree,2,3,4,5,6);
        semGive (pDrvCtrl->rmanSem);
        return (ENOSPC);
        }

    pTx->qportalLength |= QPORTAL_SGLIST_FINAL; /* last entry */

    md->count = length; /* update count */

    qPortDesc.qportalDdLioLo = 0;
    qPortDesc.qportalLioHi = 0;
    qPortDesc.qportalBpid = 0;
    qPortDesc.qportalCmd = (UINT32)RIO_TYPE_DSTR << 28;
    addr = RMAN_VTOP(pDrvCtrl, (void *)pScDesc);
    qPortDesc.qportalAddrLo = RMAN_ADDR_LO((addr));
    qPortDesc.qportalAddrHi = RMAN_ADDR_HI((addr));

    qPortDesc.qportalOffLen = RMAN_OB_GATHER | length | (offset << 20);

    pDrvCtrl->rmanTxBytes += length;

    ret = qportalLocalEnqueue (pDrvCtrl->rmanTxFqId, pDrvCtrl->rmanTxProd, &qPortDesc);

    if (ret != OK)
        {
        RMAN_LOG(DBG_ERR, "rmanTxType9():send %d %d ret 0x%x\r\n", pDrvCtrl->rmanTxProd,
            pDrvCtrl->rmanTxFree, ret, 4,5,6);

        semGive(pDrvCtrl->rmanSem);
        return ERROR;
        }

    RMAN_INC_DESC(pDrvCtrl->rmanTxProd, RMAN_TX_DESC_CNT);
    pDrvCtrl->rmanTxFree --;
    semGive(pDrvCtrl->rmanSem);

    RMAN_LOG(DBG_RW, "rmanTxType9():Enqueue status 0x%x \n", ret, 2,3,4,5,6);
    return ret;
    }

/*****************************************************************************
*
* rmanTxType10 - Type10 (Door Bell send function)
*
* This function implement Type10 transimission
*
* RETURNS: ERROR/OK
*
* ERRNO: N/A
*/

LOCAL int rmanTxType10
    (
    RMAN_DRV_CTRL *  pDrvCtrl,
    struct msgbuf *  desc,
    UINT16           destId
    )
    {
    QPORTAL_FDESC qPortDesc;
    struct rmanObMd md;
    STATUS ret;
    UINT64 addr;
    char * virt;
    UINT32 offset;

    bzero ((char *)&qPortDesc, sizeof(QPORTAL_FDESC));
    bzero ((char *)&md, sizeof(struct rmanObMd));

    semTake (pDrvCtrl->rmanSem, WAIT_FOREVER);

    ret = bportalAlloc (pDrvCtrl->rmanBportal, pDrvCtrl->rmanBpid, &addr);
    if (ret == ERROR)
    {
	    RMAN_LOG(DBG_ERR, "rmanTxType10():Allocation failed \n",1,2,3,4,5,6);
	    semGive (pDrvCtrl->rmanSem);
	    return ERROR;
    }

    RMAN_LOG(DBG_INIT, "rmanTxType10():Allocation 0x%LLx \n", addr, 2,3,4,5,6);

    virt = RMAN_PTOV(pDrvCtrl, addr);

    md.ftype = RIO_TYPE_DBELL;
    md.br = 1 ;
    md.so = 1 ;
    md.cs = 1 ;
    md.es = 1 ;
    md.reTryHopCntAdrs = 0xFF;
    md.statusFqid = pDrvCtrl->rmanTxDoneFqId;
    md.did = destId;
    md.flowlvl = 5;
    md.tint = pDrvCtrl->rmanPort;
    bcopy ((const char *)&md, (char *)virt, sizeof(struct rmanObMd));

    qPortDesc.qportalDdLioLo = 0;
    qPortDesc.qportalLioHi = 0;
    qPortDesc.qportalBpid = pDrvCtrl->rmanBpid;
    qPortDesc.qportalCmd = (UINT32)RIO_TYPE_DBELL << 28;
    qPortDesc.qportalAddrLo = RMAN_ADDR_LO((addr));
    qPortDesc.qportalAddrHi = RMAN_ADDR_HI((addr));

    offset = RM_FD_SIZE;

    bcopy (desc->buf, (char *)(virt + offset), RIO_DBELL_DATA_LEN); /* DBell 2 bytes */

    qPortDesc.qportalOffLen = RMAN_OB_SINGLE | RIO_DBELL_DATA_LEN | (offset << 20);

    pDrvCtrl->rmanTxBytes += 2;

    ret = qportalLocalEnqueue (pDrvCtrl->rmanTxFqId, pDrvCtrl->rmanTxProd, &qPortDesc);
    semGive (pDrvCtrl->rmanSem);
    RMAN_LOG(DBG_RW, "rmanTxType10(): Enqueue status 0x%x \n", ret, 2,3,4,5,6);

    return ret;
    }

/*****************************************************************************
*
* rmanTxType11 - Type11 (Mail Box send function)
*
* This function implement Type11 transimission, the length must multiple of 8 bytes,
* The max transmit length is 4K, only support mbox/letter are zero.
*
* RETURNS: ERROR/OK
*
* ERRNO: N/A
*/

LOCAL int rmanTxType11
    (
    RMAN_DRV_CTRL *  pDrvCtrl,
    struct msgbuf *  desc,
    UINT8            mbox,
    UINT8            letter,
    UINT16           destId
    )
    {
    QPORTAL_FDESC qPortDesc;
    struct rmanObMd md;
    STATUS ret;
    UINT64 addr;
    char * virt;
    UINT32 offset;

    bzero ((char *)&qPortDesc, sizeof(QPORTAL_FDESC));
    bzero ((char *)&md, sizeof(struct rmanObMd));

    semTake (pDrvCtrl->rmanSem, WAIT_FOREVER);

    ret = bportalAlloc (pDrvCtrl->rmanBportal, pDrvCtrl->rmanBpid, &addr);
    if (ret == ERROR)
        {
        RMAN_LOG(DBG_ERR, "rmanTxType11():bportalAllocb failed \n",1,2,3,4,5,6);
        semGive (pDrvCtrl->rmanSem);
        return ERROR;
        }

    RMAN_LOG(DBG_RW, "rmanTxType11():bportalAllocb -- 0x%LLx \n", addr, 2,3,4,5,6);

    virt = RMAN_PTOV(pDrvCtrl, addr);

    md.ftype = RIO_TYPE_MBOX;
    md.br = 1 ;
    md.so = 0 ;
    md.cs = 1 ;
    md.es = 1 ;
    md.reTryHopCntAdrs = 0xFF;
    md.statusFqid = pDrvCtrl->rmanTxDoneFqId;
    md.did = destId;
    md.others = ((letter & 0x3) << 6) | (mbox & 0x3f);
    md.flowlvl = 5;
    md.reserved2 = 0; /* Disable multicase */
    md.tint = pDrvCtrl->rmanPort;
    md.count = desc->len;

    bcopy ((const char *)&md, (char *)virt, sizeof(struct rmanObMd));

    qPortDesc.qportalDdLioLo = 0;
    qPortDesc.qportalLioHi = 0;
    qPortDesc.qportalBpid = pDrvCtrl->rmanBpid;
    qPortDesc.qportalCmd = (UINT32)RIO_TYPE_MBOX << 28;
    qPortDesc.qportalAddrLo = RMAN_ADDR_LO((addr));
    qPortDesc.qportalAddrHi = RMAN_ADDR_HI((addr));

    offset = RM_FD_SIZE;

    bcopy (desc->buf, (char *)(virt + offset), desc->len);

    qPortDesc.qportalOffLen = RMAN_OB_SINGLE | desc->len | (offset << 20);

    pDrvCtrl->rmanTxBytes += desc->len;

    ret = qportalLocalEnqueue (pDrvCtrl->rmanTxFqId, pDrvCtrl->rmanTxProd, &qPortDesc);
    semGive (pDrvCtrl->rmanSem);
    RMAN_LOG(DBG_RW, "rmanTxType11():enqueue status 0x%x \n", ret, 2,3,4,5,6);

    return ret;
    }

/*****************************************************************************
*
* rmanIntSend - TypeX interrupt send function
*
* This function implement Type X interrupt function, it will call actual
* interrupt send function.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL STATUS rmanIntSend
    (
    VXB_DEVICE_ID   pDev,
    ULONG           arg,
    struct msgbuf * desc,
    MSG_TXCB_FUNC   txCallback,
    void          * cbArg
    )
    {
    STATUS ret = ERROR;
    RMAN_DRV_CTRL * pDrvCtrl;
    RIO_TRANS_PARAMETER * transArg = (RIO_TRANS_PARAMETER *)arg;
    UINT16 type = transArg->transType;
    UINT16 destId = transArg->destId;

    if (type >= RIO_TYPE_NUM)
        return ERROR;

    pDrvCtrl = pDev->pDrvCtrl;

    if (pDrvCtrl == NULL)
        return ERROR;

    pDrvCtrl->typeXCallBackArg[type].pTxCallBack = txCallback;
    pDrvCtrl->typeXCallBackArg[type].pTxArg = cbArg;

    switch (type)
        {
        case RIO_TYPE9:
            ret = rmanTxType9 (pDrvCtrl, desc, destId);
            break;
        case RIO_TYPE10:
            ret = rmanTxType10 (pDrvCtrl, desc, destId);
            break;
        case RIO_TYPE11:    /* unsupport multiCast */
            ret = rmanTxType11 (pDrvCtrl, desc, transArg->mbox, transArg->letter,
                destId);
            break;
        default:
            break;
        }

    return ret;
    }

#define RMAN_DBG
#ifdef RMAN_DBG

STATUS rmanShow
    (
    VXB_DEVICE_ID pDev,
    int verbose
    )
    {
    RMAN_DRV_CTRL * pDrvCtrl;
    if (pDev == NULL)
        return ERROR;

    pDrvCtrl = pDev->pDrvCtrl;

    printf("---pDrvCtrl->rmanSizePerBuf 0x%x---\n",pDrvCtrl->rmanSizePerBuf);
    printf("        %s unit %d on %s @ 0x%08x", pDev->pName,
        pDev->unitNumber, vxbBusTypeString(pDev->busID), (int)pDev);
    printf (" with busInfo %p\n", pDev->u.pSubordinateBus);
    if (verbose >= 1)
        {
        printf("            BAR0 @ 0x%08x (memory mapped)\n",
            (int)pDev->pRegBase[0]);
        printf("            pDrvCtrl @ 0x%08x\n", (int)pDev->pDrvCtrl);
        }

    if (verbose > 1)
        {
        printf ("            RMAN %d Inound Unit %d bpid 0x%x "
            "sgbpid 0x%x Qman Portal: %p work queue channel 0x%x\n",
            pDrvCtrl->rmanPort, pDrvCtrl->rmanIbNum, pDrvCtrl->rmanBpid,
            pDrvCtrl->rmanSgBpid, pDrvCtrl->rmanQportal, pDrvCtrl->rmanWqChan);
        printf ("            RX FqId: 0x%x TX FqId: 0x%x "
            "TX done FqId: 0x%x\n", pDrvCtrl->rmanRxFqId,
            pDrvCtrl->rmanTxFqId, pDrvCtrl->rmanTxDoneFqId);
        printf ("            T9 TxProd: 0x%x TxFree: 0x%x "
            "SgBpidBuf free: %d BpidBuf free: %d \n", pDrvCtrl->rmanTxProd,
            pDrvCtrl->rmanTxFree, bportalCountGet(pDrvCtrl->rmanBportal,
            pDrvCtrl->rmanSgBpid), bportalCountGet(pDrvCtrl->rmanBportal,
            pDrvCtrl->rmanBpid));
        printf ("            Tx ErrPkts: %d  Rx ErrPkts: %d "
            "Rx bytes: %lld  Tx bytes: %lld \n",
            pDrvCtrl->rmanTxErrPkts, pDrvCtrl->rmanRxErrPkts,
            pDrvCtrl->rmanRxBytes, pDrvCtrl->rmanTxBytes);
        printf ("            Rx type 9 call back function 0x%p, "
            "arg %p \n", pDrvCtrl->typeXCallBackArg[9].pRxCallBack,
            pDrvCtrl->typeXCallBackArg[9].pRxArg);
        printf ("            Rx type 10 call back function 0x%p, "
            "arg %p \n", pDrvCtrl->typeXCallBackArg[10].pRxCallBack,
            pDrvCtrl->typeXCallBackArg[10].pRxArg);
        printf ("            Rx type 11 call back function 0x%p, "
            "arg %p \n", pDrvCtrl->typeXCallBackArg[11].pRxCallBack,
            pDrvCtrl->typeXCallBackArg[11].pRxArg);
        }

    return (OK);
    }
#endif /* RMAN_DBG */

