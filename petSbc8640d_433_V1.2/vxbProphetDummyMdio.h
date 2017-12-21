/* vxbProphetDummyMdio.h - header file for dummy MII MDIO driver */



/*
modification history
--------------------
01a,15mar2012,hdz  written
*/

#ifndef __INCvxbProphetDummyMdioh
#define __INCvxbProphetDummyMdioh

#ifdef __cplusplus
extern "C" {
#endif

IMPORT void vxbProphetDummyMdioRegister (void);

/* Private instance context */

typedef struct prophet_dummy_mdio_drv_ctrl
    {
    VXB_DEVICE_ID  dummyDev;
    UINT32         fullDuplex;
    UINT32         speed;
    } PROPHET_DUMMY_MDIO_DRV_CTRL;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCvxbDummyMdioh */

