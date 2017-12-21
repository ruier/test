#include "fsApi.h"

#ifndef COMPILE_BOOTROM

VOID pciDrvInit(VOID)
{
	m85xxPciInstInit3(); 
	nvmeVxbRegister();
	extIntInit();
	extPabackIntInit();
}


/*
 *for example "/vol/"
*/
UINT32 fsGetVolCapTest(UCHAR *vpcName)
{
    INT32   iFd = 0;
    UINT32  uiMByte = 0;
    
    iFd = open (vpcName,O_RDONLY, 0644);
    if (OK != ioctl(iFd,FIOLABELGET,&uiMByte))
    {
        printf("get vol capacity failed\n");
        close(iFd);
        return uiMByte;
    }
    
    /*printf("get vol capacity %d MByte\n",uiMByte);*/
    close(iFd);
    return uiMByte;
}

/*
 *for example "/vol/"
*/
UINT32 fsGetVolFreeCapTest(UCHAR *vpcName)
{
    INT32   iFd = 0;
    UINT32  uiMByte = 0;
    
    iFd = open (vpcName,O_RDONLY, 0644);
    
    if (OK != ioctl(iFd,FIONFREE,&uiMByte))
    {
        printf("get vol capacity failed\n");
        close(iFd);
        return uiMByte;
    }
    
   /* printf("get vol capacity %d MByte\n",uiMByte);*/
    close(iFd);
    return uiMByte;
}

#endif

