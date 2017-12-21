/* mv8E11xxPhy.c - driver for Marvell 88E11xx 10/100/1000 ethernet PHY chips */

/*
 * Copyright (c) 2005-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01n,29jul08,b_m  turn on 125MHz clock.
01m,12jun08,pmr  resource documentation
01l,05may08,tor  update version
01k,17mar08,wap  Add support for 88E1111 with mode selection (RGMII or GMII)
01j,20sep07,tor  VXB_VERSION_3
01i,01aug07,wap  Add support for 88E1011S (WIND00100585)
01h,14jul07,wap  Add fiber support for ads8560 boards
01g,02nov06,wap  Add 88E1145 support
01f,17oct06,pdg  replaced VXB_METHOD_DRIVER_UNLINK with an address
01e,02jun06,wap  Add support for 88E1000 part
01d,01jun06,wap  Add removal support
01c,31jan06,pdg  updated vxbus version
01b,05jan06,wap  Swap InstInit2 and InstConnect methods
01a,15sep05,wap  written
*/

#include <vxWorks.h>
#include <vxBusLib.h>

#include <hwif/vxbus/vxBus.h>
#include <hwif/util/hwMemLib.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbParamSys.h>
#include <../src/hwif/h/mii/miiBus.h>
#include <../src/hwif/h/mii/genericTbiPhy.h>
#include "mv88E1x11Phy.h"

LOCAL void mvPhyInit (VXB_DEVICE_ID);
LOCAL STATUS mvPhyFiberModeSet (VXB_DEVICE_ID, UINT32);
LOCAL STATUS mvPhyModeSet (VXB_DEVICE_ID, UINT32);
STATUS mvPhyModeGet (VXB_DEVICE_ID, UINT32 *, UINT32 *);
#ifdef notdef
LOCAL STATUS mvPhyIntCtl (VXB_DEVICE_ID, int);
LOCAL STATUS mvPhyIntAck (VXB_DEVICE_ID);
#endif

LOCAL void mvPhyDevInstInit(VXB_DEVICE_ID pDev);
LOCAL void mvPhyDevInstInit2(VXB_DEVICE_ID pDev);
LOCAL void mvPhyDevInstConnect(VXB_DEVICE_ID pDev);
LOCAL BOOL mvPhyProbe(VXB_DEVICE_ID pDev);
LOCAL STATUS mvPhyInstUnlink (VXB_DEVICE_ID, void *);

LOCAL UINT32 mvPhy88E114xFixup (VXB_DEVICE_ID pDev);

#define PHY_SETBIT(x, y, z)                                     \
    miiBusRead (x, pDrvCtrl->miiPhyAddr, y, &miiVal);           \
    miiBusWrite (x, pDrvCtrl->miiPhyAddr, y, miiVal | (z));     \

#define PHY_CLRBIT(x, y, z)                                     \
    miiBusRead (x, pDrvCtrl->miiPhyAddr, y, &miiVal);           \
    miiBusWrite (x, pDrvCtrl->miiPhyAddr, y, miiVal & ~(z));    \

LOCAL device_method_t mvPhyMethods[] =
    {
    DEVMETHOD(miiModeGet,       mvPhyModeGet),
    DEVMETHOD(miiModeSet,       mvPhyModeSet),
    DEVMETHOD(vxbDrvUnlink,     mvPhyInstUnlink),
    { 0, 0 }
    };

LOCAL struct drvBusFuncs mvPhyFuncs =
    {
    mvPhyDevInstInit,      /* devInstanceInit */
    mvPhyDevInstInit2,     /* devInstanceInit2 */
    mvPhyDevInstConnect    /* devInstanceConnect */
    };

LOCAL VXB_PARAMETERS mvParamDefaults[] =
    {
       {"mode", VXB_PARAM_INT32, {(void *)MV88E_MODE_COPPER}},
        {NULL, VXB_PARAM_END_OF_LIST, {NULL}}
    };

struct vxbDevRegInfo mvPhyDevRegistration =
    {
    NULL,               /* pNext */
    VXB_DEVID_DEVICE,   /* devID */
    VXB_BUSID_MII,      /* busID = MII Bus */
    VXB_VER_4_0_0,      /* busVer */
    "mv88E1x11Phy",     /* drvName */
    &mvPhyFuncs,        /* pDrvBusFuncs */
    mvPhyMethods,       /* pMethods */
    mvPhyProbe,         /* devProbe */
    mvParamDefaults	/* pParamDefaults */
    };

void mvPhyRegister(void)
    {
    vxbDevRegister (&mvPhyDevRegistration);
    return;
    }

LOCAL void mvPhyDevInstInit
    (
    VXB_DEVICE_ID pDev
    )
    {
    vxbNextUnitGet (pDev);
    return;
    }

LOCAL void mvPhyDevInstConnect
    (
    VXB_DEVICE_ID pDev
    )
    {
    return;
    }


/*********************************************************************
*
* mvPhy88E114xFixup - fixup for 88E1145 PHYs in RGMII mode
*
* The CDS8548 rev2 board uses a Marvell 88E1145 quad 10/100/1000 PHY,
* which supports both GMII and RGMII modes. ETSEC ports 0 and 1
* are configured for GMII, but ports 2 and 3 are configured for
* RGMII. Those ports don't operate correctly unless we enable the
* RGMII/RTBI delay timing for the RX and TX clocks.
*
* We also adjust the impedance calibration slightly. (I am somewhat
* unsure of the justification for this tweak: testing shows that
* setting the timing delay seems to be all that's necessary to get
* the interfaces to work. Also, the Marvell documentation indicates
* this setting can only be done via port 0, and it affects all 4 ports
* at once.)
*
* It's possible the clock timing needs to be ajusted for all cases
* when the PHY is operating in RGMII mode (in which case it should
* be possible to set it automatically based on the HWCGF_MODE bits
* in the extended PHY status register), but this has not been
* confirmed, so for the time being this fixup is only enabled
* on a per-BSP basis.
*
* Note that this routine can also be used on the 88E1111 chip, which
* has the same impedance calibration circuitry.
*
* RETURNS: OK
*
* ERRNO: N/A
*/

LOCAL UINT32 mvPhy88E114xFixup
    (
    VXB_DEVICE_ID pDev
    )
    {
    MII_DRV_CTRL * pDrvCtrl;
    UINT16 miiVal;

    pDrvCtrl = pDev->pDrvCtrl;

    miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MV_ESR, &miiVal);

    /*
     * If this isn't an 88E114x part or we're not operating
     * in RGMII mode, just return.
     */

    if ((MII_MODEL(pDrvCtrl->miiId2) != MII_MODEL_MARVELL_E1145 &&
        MII_MODEL(pDrvCtrl->miiId2) != MII_MODEL_MARVELL_E1111) ||
        (miiVal & MV_ESR_HWCFG_MODE) != MV_HWCFG_RGMII_TO_COPPER)
        return (OK);

    /* Enable RGMII clock delays for RX and TX */

    PHY_SETBIT(pDev, MV_ECR, MV_ECR_RGMII_RX_DELAY|MV_ECR_RGMII_TX_DELAY);

    /* Select page 18 to access the misc control register */

    miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MV_PAGESEL, 18);

    miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MV_MISCCTL, &miiVal);

    /* Reset the PMOS and NMOS impedances */

    PHY_CLRBIT(pDev, MV_MISCCTL, MV_MISCCTL_PMOS_IMP|MV_MISCCTL_NMOS_IMP);

    if (MII_MODEL(pDrvCtrl->miiId2) == MII_MODEL_MARVELL_E1111)
        {
        /* Reprogram them for 41 ohms */

        PHY_SETBIT(pDev, MV_MISCCTL, MV_PMOSIMP_1111_41OHM);
        PHY_SETBIT(pDev, MV_MISCCTL, MV_NMOSIMP_1111_41OHM);
        }
       
    if (MII_MODEL(pDrvCtrl->miiId2) == MII_MODEL_MARVELL_E1145)
        {
        /* Reprogram them for 36 ohms and 39 ohms, respectively */

        PHY_SETBIT(pDev, MV_MISCCTL, MV_PMOSIMP_36OHM);
        PHY_SETBIT(pDev, MV_MISCCTL, MV_NMOSIMP_39OHM);
        }

    miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MV_MISCCTL, &miiVal);

    /* Select page 3 to access the calibration override register */
    
    miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MV_PAGESEL, 3);

    /* Restart calibration */

    miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MV_CALOV, MV_CALOV_RESTART);

    /* Return to page 0 */

    miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MV_PAGESEL, 0);

    return (OK);
    }

/*********************************************************************
*
* mvPhyInstUnlink - VxBus unlink handler
*
* This function implements the VxBus unlink method for this driver.
* We delete each media type that was originally added to the MII bus
* instance by this device and take ourselves off the miiMonitor task
* list.
*
* RETURNS: OK if shutdown succeeds, else ERROR
*
* ERRNO: N/A
*/
 
LOCAL STATUS mvPhyInstUnlink
    (
    VXB_DEVICE_ID pDev,
    void * unused
    )
    {
    VXB_DEVICE_ID pBus;
    MII_DRV_CTRL * pDrvCtrl;
 
    pDrvCtrl = (MII_DRV_CTRL *)pDev->pDrvCtrl;
 
    if (pDrvCtrl->miiInitialized == FALSE)
        return (ERROR);
 
    /* Only our parent bus can delete us. */
    if (pDrvCtrl->miiLeaving == FALSE)
        return (ERROR);
 
    /* Remove ourselves from the miiMonitor task list. */
 
    miiBusListDel (pDev);
 
    /* Remove media list entries. */
 
    pBus = vxbDevParent (pDev);

    if (MII_OUI(pDrvCtrl->miiId1, pDrvCtrl->miiId2) == MII_OUI_MARVELL &&
        MII_MODEL(pDrvCtrl->miiId2) == MII_MODEL_MARVELL_E1011S)
        {
        miiBusMediaDel (pBus, IFM_ETHER|IFM_1000_SX);
        miiBusMediaDel (pBus, IFM_ETHER|IFM_1000_SX|IFM_FDX);
        }
    miiBusMediaDel (pBus, IFM_ETHER|IFM_1000_T);
    miiBusMediaDel (pBus, IFM_ETHER|IFM_1000_T|IFM_FDX);
    miiBusMediaDel (pBus, IFM_ETHER|IFM_100_TX);
    miiBusMediaDel (pBus, IFM_ETHER|IFM_100_TX|IFM_FDX);
    miiBusMediaDel (pBus, IFM_ETHER|IFM_10_T);
    miiBusMediaDel (pBus, IFM_ETHER|IFM_10_T|IFM_FDX);
    miiBusMediaDel (pBus, IFM_ETHER|IFM_AUTO);

    pDrvCtrl->miiInitialized = FALSE;

    return (OK);
    }

/*********************************************************************
*
* mvPhyDevInstInit2 - vxBus instInit2 handler
*
* This routine function does the final driver setup. The PHY registers
* its media types with its parent bus and adds itself to the MII
* monitoring list.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void mvPhyDevInstInit2
    (
    VXB_DEVICE_ID pDev
    )
    {
    VXB_DEVICE_ID pBus;
    MII_DRV_CTRL * pDrvCtrl;

    pDrvCtrl = (MII_DRV_CTRL *)pDev->pDrvCtrl;

    if (pDrvCtrl->miiInitialized == TRUE)
        return;

    pDrvCtrl->miiInitialized = TRUE;

    /*
     * Tell miiBus about the media we support.
     */

    pBus = vxbDevParent (pDev);

    if (MII_OUI(pDrvCtrl->miiId1, pDrvCtrl->miiId2) == MII_OUI_MARVELL &&
        MII_MODEL(pDrvCtrl->miiId2) == MII_MODEL_MARVELL_E1011S)
        {
        miiBusMediaAdd (pBus, IFM_ETHER|IFM_1000_SX);
        miiBusMediaAdd (pBus, IFM_ETHER|IFM_1000_SX|IFM_FDX);
        }
    miiBusMediaAdd (pBus, IFM_ETHER|IFM_1000_T);
    miiBusMediaAdd (pBus, IFM_ETHER|IFM_1000_T|IFM_FDX);
    miiBusMediaAdd (pBus, IFM_ETHER|IFM_100_TX);
    miiBusMediaAdd (pBus, IFM_ETHER|IFM_100_TX|IFM_FDX);
    miiBusMediaAdd (pBus, IFM_ETHER|IFM_10_T);
    miiBusMediaAdd (pBus, IFM_ETHER|IFM_10_T|IFM_FDX);
    miiBusMediaAdd (pBus, IFM_ETHER|IFM_AUTO);
    miiBusMediaDefaultSet (pBus, IFM_ETHER|IFM_AUTO);

    /*
     * Initialize the PHY.
     */

    mvPhyInit (pDev);

    /* Add to the monitor list. */

    miiBusListAdd (pDev);

    return;
    }

/*********************************************************************
*
* mvPhyProbe - vxBus probe handler
*
* This routine checks the ID register values of the PHY and compares
* the OUI and model numbers against a list of supported Broadcom
* PHYs. If the device isn't supported, we return FALSE.
*
* RETURNS: TRUE if device is a Broadcom 54xx PHY, otherwise FALSE
*
* ERRNO: N/A
*/

LOCAL
BOOL mvPhyProbe
    (
    VXB_DEVICE_ID pDev
    )
    {
    MII_DRV_CTRL * pDrvCtrl;
    UINT16 miiId1;
    UINT16 miiId2;

    pDrvCtrl = pDev->pDrvCtrl;
    miiId1 = pDrvCtrl->miiId1;
    miiId2 = pDrvCtrl->miiId2;

    if (MII_OUI(miiId1, miiId2) == MII_OUI_MARVELL &&
        MII_MODEL(miiId2) == MII_MODEL_MARVELL_E1111)
	return(TRUE);

    if (MII_OUI(miiId1, miiId2) == MII_OUI_MARVELL &&
        MII_MODEL(miiId2) == MII_MODEL_MARVELL_E1145)
	return(TRUE);

    if (MII_OUI(miiId1, miiId2) == MII_OUI_MARVELL &&
        MII_MODEL(miiId2) == MII_MODEL_MARVELL_E1011)
        return(TRUE);

    if (MII_OUI(miiId1, miiId2) == MII_OUI_MARVELL &&
        MII_MODEL(miiId2) == MII_MODEL_MARVELL_E1011S)
        return(TRUE);

    if (MII_OUI(miiId1, miiId2) == MII_OUI_MARVELL &&
        MII_MODEL(miiId2) == MII_MODEL_xxMARVELL_E1000)
        return(TRUE);

    return (FALSE);
    }

LOCAL void mvPhyInit
    (
    VXB_DEVICE_ID pDev
    )
    {
    MII_DRV_CTRL * pDrvCtrl;
    UINT16 miiVal;
    UINT32 nonGenericBspMod=0;
    UINT32 phyAddr=0;
    struct hcfDevice *pHcf;
    int ix;
    int i;
    VXB_INST_PARAM_VALUE val;
    STATUS r;

    pDrvCtrl = (MII_DRV_CTRL *)pDev->pDrvCtrl;

    miiVal = MII_CR_POWER_DOWN;
    miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MII_CTRL_REG, miiVal);

    miiVal = 0;
    miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MII_CTRL_REG, miiVal);

    for (ix = 0; ix < hcfDeviceNum; ix++)
        {
        if (strcmp("mv88E14xPhy", hcfDeviceList[ix].devName) != 0)
            continue;

        pHcf = (struct hcfDevice *)&hcfDeviceList[ix];

        if (pHcf == NULL)
            continue;

        /* 
         * resourceDesc {
         * The phyAddr resource specifies the MII address
         * of the device to which the non-generic BSP mod
         * should be applied. }
         */
        if (devResourceGet(pHcf, "phyAddr",
                HCF_RES_INT, (void *)&phyAddr) != OK ||
        /* 
         * resourceDesc {
         * The nonGenericBspMod resource specifies the
         * type of extra initialization which should 
         * should be performed on the device specified by
         * the phyAddr resource. }
         */
            devResourceGet(pHcf, "nonGenericBspMod",
                HCF_RES_INT, (void *)&nonGenericBspMod) != OK )
            continue;

        /* Check that the record matches this PHY device. */

	if (phyAddr == pDrvCtrl->miiPhyAddr)
	    {
	    switch(nonGenericBspMod)
		{
		case MV88E_CDS8548_BSP_MOD:
		    mvPhy88E114xFixup (pDev);
		    break;
		default:
		    break;
		}
	    }
        }


    /* Enable advertisement of gigE modes. */

    miiVal = MII_MASSLA_CTRL_1000T_FD|MII_MASSLA_CTRL_1000T_HD;
    miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MII_MASSLA_CTRL_REG, miiVal);

    /*
     * If this is an 88E1011S PHY, we can optionally support fiber
     * mode.
     *
     * Note that fiber support has only been tested on the Freescale
     * ADS8560 board, which supports both fiber and copper connections
     * to the on-board TSEC interfaces. With the ADS8560, the TSECs
     * can be configured for either TBI or GMII mode (using a dip switch
     * and jumper combination). The 88E1011 supports both fiber over TBI
     * and fiber over GMII modes, so in theory we should be able to get
     * fiber mode to work with either the TBI or GMII media interface.
     * However, I've only been able to get it to work using GMII. This
     * is preferable anyway, since that means we can select either copper
     * or fiber operation via software rather than requiring the user to
     * manually set the switches and jumpers on the board.
     *
     * Experimentation has shown that the only way to get fiber over GMII
     * mode to work requires first forcing the PHY to fiber over TBI, and
     * then to autoselection, with fiber mode autoneg registers enabled.
     * If we configure the PHY this way, it will favor fiber optic mode
     * if both copper and fiber media are connected.
     */

    /*
     * paramDesc {
     * The mode parameter specifies how the device
     * should operate.  The selection can be 
     * MV88E_MODE_COPPER, MV88E_MODE_FIBER, 
     * MV88E_MODE_RGMII, MV88E_MODE_RGMII_FIXUP, 
     * or MV88E_MODE_GMII. }
     */
    r = vxbInstParamByNameGet (pDev, "mode", VXB_PARAM_INT32, &val);
    if (MII_OUI(pDrvCtrl->miiId1, pDrvCtrl->miiId2) == MII_OUI_MARVELL &&
        MII_MODEL(pDrvCtrl->miiId2) == MII_MODEL_MARVELL_E1011S)
        {
        if (r == OK && val.int32Val == MV88E_MODE_FIBER)
            {
            miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MV_ESR,
                MV_ESR_FC_AUTO_DIS|MV_HWCFG_TBI_TO_FIBER);
            miiBusWrite (pDev, pDrvCtrl->miiPhyAddr,
                MII_CTRL_REG, MII_CR_RESET);
            miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MV_ESR,
                MV_ESR_FC_AUTO_REG|MV_HWCFG_GMII_TO_COPPER);
            }
        else
            miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MV_ESR,
                MV_ESR_FC_AUTO_DIS|MV_HWCFG_GMII_TO_COPPER);
        }
    if (MII_OUI(pDrvCtrl->miiId1, pDrvCtrl->miiId2) == MII_OUI_MARVELL &&
        MII_MODEL(pDrvCtrl->miiId2) == MII_MODEL_MARVELL_E1111)
        {
        /* Select RGMII mode */
        if (r == OK && val.int32Val == MV88E_MODE_RGMII)
            {
            miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MV_ECR, &miiVal);
            miiVal |= MV_ECR_RGMII_TX_DELAY|MV_ECR_RGMII_RX_DELAY;
            miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MV_ECR, miiVal);
            miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MV_ESR, &miiVal);
            miiVal &= ~MV_ESR_HWCFG_MODE;
            miiVal |= MV_HWCFG_RGMII_TO_COPPER;
            miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MV_ESR, miiVal);
            }
        /* Select RGMII mode with impedance fixup */
        if (r == OK && val.int32Val == MV88E_MODE_RGMII_FIXUP)
            {
            miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MV_ESR, &miiVal);
            miiVal &= ~MV_ESR_HWCFG_MODE;
            miiVal |= MV_HWCFG_RGMII_TO_COPPER;
            miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MV_ESR, miiVal);
            mvPhy88E114xFixup (pDev);
            }
        /* Select GMII mode */
        if (r == OK && val.int32Val == MV88E_MODE_GMII)
            {
            miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MV_ECR, &miiVal);
            miiVal &= ~(MV_ECR_RGMII_TX_DELAY|MV_ECR_RGMII_RX_DELAY);
            miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MV_ECR, miiVal);
            miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MV_ESR, &miiVal);
            miiVal &= ~MV_ESR_HWCFG_MODE;
            miiVal |= MV_HWCFG_GMII_TO_COPPER;
            miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MV_ESR, miiVal);
            }
        /*
         *add by lgh for RGMII to FIBER support
        */
        if((OK == r) && (MV88E_MODE_RGMII_TO_FIBER == val.int32Val))
            {                
            miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MV_ECR, &miiVal);
            miiVal |= MV_ECR_RGMII_TX_DELAY|MV_ECR_RGMII_RX_DELAY;
            miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MV_ECR, miiVal);

            miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MV_ESR, &miiVal);
            miiVal &= ~MV_ESR_HWCFG_MODE;
            miiVal |=  MV_HWCFG_RGMII_TO_SGMII;/*MV_HWCFG_RGMII_TO_FIBER*/
            miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MV_ESR, miiVal); 

           /* logMsg("====phy addr 0x%x val 0x%x \n",pDrvCtrl->miiPhyAddr,miiVal,0,0,0,0);*/
            }
        if (r == OK && val.int32Val == MV88E_MODE_SGMII_TO_COPPER)
            {
            miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MV_ESR, &miiVal);
            miiVal &= ~MV_ESR_HWCFG_MODE;
            miiVal |= MV_HWCFG_SGMII_NOCLK;
            miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MV_ESR, miiVal);
            }
        }

    /* turn on 125MHz clock in case hardware configuration sets it off */

    PHY_CLRBIT (pDev, MV_CR, MV_CR_125CLK_DIS);

    /* Set reset bit and then wait for it to clear. */
    miiVal = MII_CR_RESET;
    miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MII_CTRL_REG, miiVal);

    for (i = 0; i < 1000; i++)
        {
        miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MII_CTRL_REG, &miiVal);
        if (!(miiVal & MII_CR_RESET))
            break;
        }      
    return;
    }


VOID mvPhyReset(VXB_DEVICE_ID pDev)
{
    MII_DRV_CTRL * pDrvCtrl;
    UINT16 miiVal = 0;

    pDrvCtrl = pDev->pDrvCtrl;
    
    miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MII_CTRL_REG, &miiVal);
    miiVal |= MII_CR_RESET;
    miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MII_CTRL_REG, miiVal);
}

VOID mvPhyCfg(VXB_DEVICE_ID pDev)
{
    MII_DRV_CTRL * pDrvCtrl;
    UINT16 miiVal = 0;

    pDrvCtrl = pDev->pDrvCtrl;

    /*
     *enable fiber copper selection
    */
    miiBusRead (pDev, pDrvCtrl->miiPhyAddr, 27, &miiVal);
    miiVal &= (~0x8000);
    miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, 27, miiVal);

    miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MII_CTRL_REG, &miiVal);
    miiVal |= MII_CR_RESET;
    miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MII_CTRL_REG, miiVal);

    miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MV_ECR, &miiVal);
    miiVal |= MV_ECR_RGMII_TX_DELAY|MV_ECR_RGMII_RX_DELAY;
    miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MV_ECR, miiVal);

    miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MV_ESR, &miiVal);
    miiVal &= ~MV_ESR_HWCFG_MODE;
    miiVal |=  MV_HWCFG_RGMII_TO_FIBER;/**/
    miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MV_ESR, miiVal); 

    miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MII_CTRL_REG, &miiVal);
    miiVal |= MII_CR_RESET;
    miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MII_CTRL_REG, miiVal);
}

VOID mvPhyLoop(VXB_DEVICE_ID pDev)
{
    MII_DRV_CTRL * pDrvCtrl;
    UINT16 miiVal = 0;

    pDrvCtrl = pDev->pDrvCtrl;
    
    miiBusWrite (pDev, pDrvCtrl->miiPhyAddr,18, 0);
    miiBusRead (pDev, pDrvCtrl->miiPhyAddr, 9, &miiVal);
    miiVal &= (~1800);
    miiVal |= 1800;
    miiBusWrite (pDev, pDrvCtrl->miiPhyAddr,9, miiVal);

    miiBusRead (pDev, pDrvCtrl->miiPhyAddr, 0, &miiVal);
    miiVal &= (~0x1000);
    miiVal |= 0x1000;
    miiBusWrite (pDev, pDrvCtrl->miiPhyAddr,0, miiVal);

    taskDelay(sysClkRateGet()/10);
    miiBusWrite (pDev, pDrvCtrl->miiPhyAddr,29, 7);
    miiBusRead (pDev, pDrvCtrl->miiPhyAddr, 30, &miiVal);
    miiVal &= (~8);
    miiVal |= 8;
    miiBusWrite (pDev, pDrvCtrl->miiPhyAddr,30, miiVal);

    miiBusWrite (pDev, pDrvCtrl->miiPhyAddr,29, 0x10);
    miiBusRead (pDev, pDrvCtrl->miiPhyAddr, 30, &miiVal);
    miiVal &= (~2);
    miiVal |= 2;
    miiBusWrite (pDev, pDrvCtrl->miiPhyAddr,30, miiVal);

    miiBusWrite (pDev, pDrvCtrl->miiPhyAddr,29, 0x12);
    miiBusRead (pDev, pDrvCtrl->miiPhyAddr, 30, &miiVal);
    miiVal &= (~1);
    miiVal |= 1;
    miiBusWrite (pDev, pDrvCtrl->miiPhyAddr,30, miiVal);
}

LOCAL STATUS mvPhyFiberModeSet
    (
    VXB_DEVICE_ID pDev,
    UINT32 mode
    )
    {
    MII_DRV_CTRL * pDrvCtrl;
    UINT16 miiVal;
    UINT16 miiAnar = 0;
    UINT16 miiCtl = 0;
    UINT16 miiSts;

    pDrvCtrl = (MII_DRV_CTRL *)pDev->pDrvCtrl;

    /* Get status register so we can look for extended capabilities. */

    miiBusRead (pDev, pDrvCtrl->miiPhyAddr, TBI_SR, &miiSts);

    switch(IFM_SUBTYPE(mode)) {
    case IFM_AUTO:
        /* Set autoneg advertisement to advertise all modes. */
        miiAnar = TBI_ANAR_1000HD|TBI_ANAR_1000FD;
        miiCtl = TBI_CR_AUTO_EN|TBI_CR_RESTART;
        break;
    case IFM_1000_SX:
        if ((mode & IFM_GMASK) == IFM_FDX)
            miiAnar = TBI_ANAR_1000FD;
        else
            miiAnar = TBI_ANAR_1000HD;
        miiCtl = TBI_CR_AUTO_EN|TBI_CR_RESTART;
        break;
    default:
        return (ERROR);
        break;
    }

    mvPhyInit (pDev);

    miiBusRead (pDev, pDrvCtrl->miiPhyAddr, TBI_ANAR , &miiVal);
    miiVal &= ~(TBI_ANAR_1000HD|TBI_ANAR_1000FD);
    miiVal |= miiAnar;
    miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, TBI_ANAR, miiVal);

    miiBusRead (pDev, pDrvCtrl->miiPhyAddr, TBI_CR, &miiVal);
    miiVal &= ~(TBI_CR_FDX|TBI_CR_1000|TBI_CR_AUTO_EN|TBI_CR_RESTART);
    miiVal |= miiCtl;
    miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, TBI_CR, miiVal);

    return(OK);
    }

LOCAL STATUS mvPhyModeSet
    (
    VXB_DEVICE_ID pDev,
    UINT32 mode
    )
    {
    MII_DRV_CTRL * pDrvCtrl;
    UINT16 miiVal;
    UINT16 miiAnar = 0;
    UINT16 gmiiAnar = 0;
    UINT16 miiCtl = 0;
    VXB_INST_PARAM_VALUE val;
    STATUS r;

    pDrvCtrl = (MII_DRV_CTRL *)pDev->pDrvCtrl;

    /*
     * paramDesc {
     * See the previous description for the mode parameter. }
     */
    r = vxbInstParamByNameGet (pDev, "mode", VXB_PARAM_INT32, &val);

    if (MII_OUI(pDrvCtrl->miiId1, pDrvCtrl->miiId2) == MII_OUI_MARVELL &&
        MII_MODEL(pDrvCtrl->miiId2) == MII_MODEL_MARVELL_E1011S &&
        r == OK && val.int32Val == MV88E_MODE_FIBER)
        return (mvPhyFiberModeSet (pDev, mode));
 
    switch(IFM_SUBTYPE(mode)) {
    case IFM_AUTO:
        /* Set autoneg advertisement to advertise all modes. */
        miiAnar = MII_ANAR_10TX_HD|MII_ANAR_10TX_FD|
                  MII_ANAR_100TX_HD|MII_ANAR_100TX_FD;
        gmiiAnar = MII_MASSLA_CTRL_1000T_FD|MII_MASSLA_CTRL_1000T_HD;
        miiCtl = MII_CR_AUTO_EN|MII_CR_RESTART;
        break;
    case IFM_1000_T:
        if ((mode & IFM_GMASK) == IFM_FDX)
		gmiiAnar = MII_MASSLA_CTRL_1000T_FD;
	else
		gmiiAnar = MII_MASSLA_CTRL_1000T_HD;
	miiAnar = 0;
        miiCtl = MII_CR_AUTO_EN|MII_CR_RESTART;
	break;
    case IFM_100_TX:
        miiCtl = MII_CR_AUTO_EN|MII_CR_RESTART;
        if ((mode & IFM_GMASK) == IFM_FDX)
            miiAnar = MII_ANAR_100TX_FD;
        else
            miiAnar = MII_ANAR_100TX_HD;
        gmiiAnar = 0;
        break;
    case IFM_10_T:
        miiCtl = MII_CR_AUTO_EN|MII_CR_RESTART;
        if ((mode & IFM_GMASK) == IFM_FDX)
            miiAnar = MII_ANAR_10TX_FD;
        else
            miiAnar = MII_ANAR_10TX_HD;
        gmiiAnar = 0;
        break;
    default:
        return (ERROR);
        break;
    }

    mvPhyInit (pDev);

    miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MII_AN_ADS_REG, &miiVal);
    miiVal &= ~(MII_ANAR_10TX_HD|MII_ANAR_10TX_FD|
                MII_ANAR_100TX_HD|MII_ANAR_100TX_FD);
    miiVal |= miiAnar;
    miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MII_AN_ADS_REG, miiVal);

    miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MII_MASSLA_CTRL_REG, &miiVal);
    miiVal &= ~(MII_MASSLA_CTRL_1000T_HD|MII_MASSLA_CTRL_1000T_FD);
    miiVal |= gmiiAnar;
    miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MII_MASSLA_CTRL_REG, miiVal);

    miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MII_CTRL_REG, &miiVal);
    miiVal &= ~(MII_CR_FDX|MII_CR_100|MII_CR_AUTO_EN|
                MII_CR_RESTART|MII_CR_1000);
    miiVal |= miiCtl;
    miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MII_CTRL_REG, miiVal);

    return(OK);
    }

STATUS mvPhyModeGet
    (
    VXB_DEVICE_ID pDev,
    UINT32 * mode,
    UINT32 * status
    )
    {
    UINT16 miiSts;
    UINT16 miiMode;

    MII_DRV_CTRL * pDrvCtrl;

    pDrvCtrl = (MII_DRV_CTRL *)pDev->pDrvCtrl;

    *mode = IFM_ETHER;
    *status = IFM_AVALID;

    /* read the extended status register for all the link/speed/dpx/info */

    miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MV_STS, &miiSts);

    /* no link bit means no carrier. */
    if (!(miiSts & MV_STS_LINK))
        {
        *mode |= IFM_NONE;
        return (OK);
        }

    *status |= IFM_ACTIVE;

    switch (miiSts & MV_STS_SPEED) {
    case MV_SPEED_1000:
        *mode |= IFM_1000_T;
        /*
         * If this is an 88E1011S and we automatically detected fiber
         * media, report 1000baseSX instead of 1000baseT.
         */
       if(MII_OUI(pDrvCtrl->miiId1, pDrvCtrl->miiId2) == MII_OUI_MARVELL &&
           MII_MODEL(pDrvCtrl->miiId2) == MII_MODEL_MARVELL_E1111)
            {
            miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MV_ESR, &miiMode);
            if (miiMode & MV_ESR_FC_RESULT)
                {
                *mode &= ~IFM_1000_T;
                *mode |= IFM_1000_SX;
                }
            } 
        break;
    case MV_SPEED_100:
        *mode |= IFM_100_TX;
        break;
    case MV_SPEED_10:
        *mode |= IFM_10_T;
        break;
    default:
        *mode |= IFM_NONE;
        break;
    }

    if (miiSts & MV_STS_DUPLEX)
        *mode |= IFM_FDX;
    else
        *mode |= IFM_HDX;
    
    return (OK);
    }

#ifdef notdef

/*
 * Enable or disable PHY interrupts. If no interrupts supported,
 * return ERROR.
 */

LOCAL STATUS mvPhyIntCtl
    (
    VXB_DEVICE_ID pDev,
    int ctl
    )
    {
    UINT16 miiVal;
    MII_DRV_CTRL * pDrvCtrl;

    pDrvCtrl = (MII_DRV_CTRL *)pDev->pDrvCtrl;

    /* if ctl is set, unmask all interrupts, otherwise mask them all off. */
    if (ctl)
        miiVal = MV_IMR_SPEED|MV_IMR_DUPLEX|MV_IMR_LINK;
    else
        miiVal = 0;

    miiBusWrite (pDev, pDrvCtrl->miiPhyAddr, MV_IMR, miiVal);

    return (OK);
    }

/*
 * Ack PHY interrupts. Return OK if an interrupt was pending, error
 * if not.
 */

LOCAL STATUS mvPhyIntAck
    (
    VXB_DEVICE_ID pDev
    )
    {
    UINT16 miiVal;
    MII_DRV_CTRL * pDrvCtrl;

    pDrvCtrl = (MII_DRV_CTRL *)pDev->pDrvCtrl;

    /* Reading the register clears any pending interrupts. */

    miiBusRead (pDev, pDrvCtrl->miiPhyAddr, MV_ISR, &miiVal);

    if (miiVal & (MV_ISR_SPEED|MV_ISR_DUPLEX|MV_ISR_LINK))
        return (OK);
    return (ERROR);
    }

#endif
