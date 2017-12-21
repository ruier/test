#include <stdio.h>
#include <vxWorks.h>
#include <string.h>
#include <logLib.h>
#include <vxBusLib.h>			/* for VXB_DEVICE_ID */
#include <hwif/vxbus/vxBus.h>
#include <hwif/util/hwMemLib.h>		/* for hwMemAlloc() */
#include <hwif/vxbus/hwConf.h>		/* for devResourceGet() and hcfDeviceGet() */
#include <driverControl.h>		/* for struct vxbDriverControl */


extern VXB_DEVICE_ID vxbInstByNameFind(char *instName,int unit);

STATUS sysPhyRead(UINT8 phyAddr,UINT8 regAddr,UINT16 *dataVal)
{
	VXB_DEVICE_ID pDev;
	FUNCPTR  mdioRead;
//	FUNCPTR  mdioWrite;
	pDev=(VXB_DEVICE_ID)vxbInstByNameFind("prophetM8640dMdio",0);
	if(pDev == NULL)
	{
	 printf("bspMdioMethodGet:pDev = NULL\n");
	 return ERROR;
	}
	
	mdioRead =  vxbDevMethodGet(pDev, DEVMETHOD_CALL(miiRead));
	//mdioWrite =  vxbDevMethodGet(pDev,DEVMETHOD_CALL(miiWrite));
	if((mdioRead == NULL))
	{
	 printf("mdioRead || mdioRead == NULL\n");
	 return ERROR;
	}
	mdioRead(pDev,phyAddr,regAddr,dataVal);
	return OK;
}


STATUS sysPhyWrite(UINT8 phyAddr,UINT8 regAddr,UINT32 dataVal)
{
	STATUS rval=ERROR;
	VXB_DEVICE_ID pDev;
	//FUNCPTR  mdioRead;
	FUNCPTR  mdioWrite;
	pDev=(VXB_DEVICE_ID)vxbInstByNameFind("prophetM8640dMdio",0);
	if(pDev == NULL)
	{
	 printf("bspMdioMethodGet:pDev = NULL\n");
	 return ERROR;
	}
	
	//mdioRead =  vxbDevMethodGet(pDev, DEVMETHOD_CALL(miiRead));
	mdioWrite =  vxbDevMethodGet(pDev,DEVMETHOD_CALL(miiWrite));
	if((mdioWrite == NULL))
	{
	 printf("mdioRead || mdioRead == NULL\n");
	 return ERROR;
	}
	mdioWrite(pDev,phyAddr,regAddr,dataVal);
   return OK	;
}


STATUS sysPhyReadAll(UINT8 phyAddr)
{
	UINT32 i;
	UINT16 data;
	
	
	VXB_DEVICE_ID pDev;
	FUNCPTR  mdioRead;
	FUNCPTR  mdioWrite;
	pDev=(VXB_DEVICE_ID)vxbInstByNameFind("prophetM8640dMdio",0);
	if(pDev == NULL)
	{
	 printf("bspMdioMethodGet:pDev = NULL\n");
	 return ERROR;
	}
	
	mdioRead =  vxbDevMethodGet(pDev, DEVMETHOD_CALL(miiRead));
	mdioWrite =  vxbDevMethodGet(pDev,DEVMETHOD_CALL(miiWrite));
	if((mdioRead == NULL)||(mdioWrite == NULL))
	{
	 printf("mdioRead || mdioRead == NULL\n");
	 return ERROR;
	}
	for(i=0;i<32;i++)
	{
		mdioRead(pDev,phyAddr,i,(UINT16 *)&data);
		printf("reg:%d--dataVal:0x%x\n",i,data);
	}
	return OK;
}




//extern VXB_DEVICE_ID vxbInstByNameFind(char * instName,int unit);
STATUS sysPhyInit(UINT8 phyAddr)
{
	UINT16 data;
	VXB_DEVICE_ID pDev;
	FUNCPTR  mdioRead;
	FUNCPTR  mdioWrite;
	pDev=(VXB_DEVICE_ID)vxbInstByNameFind("prophetM8640dMdio",0);
	if(pDev == NULL)
	{
	 printf("bspMdioMethodGet:pDev = NULL\n");
	 return ERROR;
	}
	
	mdioRead =  vxbDevMethodGet(pDev, DEVMETHOD_CALL(miiRead));
	mdioWrite =  vxbDevMethodGet(pDev,DEVMETHOD_CALL(miiWrite));
	if((mdioRead == NULL)||(mdioWrite == NULL))
	{
	 printf("mdioRead || mdioRead == NULL\n");
	 return ERROR;
	}
	mdioRead(pDev,phyAddr,20,&data);
	data |=((1<<1)|(1<<7));
	mdioWrite(pDev,phyAddr,20,data);
	
	mdioRead(pDev,phyAddr,0,&data);
	//printf("data1=0x%x\n",data);
	data |= (1<<15);
	mdioWrite(pDev,phyAddr,0,data);
	return OK;
}


void readAA(UINT8 phyAddr,UINT8 regAddr)
{
UINT32 i;	
*(UINT32*)0xe8024524=0;
	*(UINT32*)0xe8024528=(phyAddr<<8)|regAddr;
	printf("data=0x%x\n",*(UINT32*)0xe8024530);
	*(UINT32*)0xe8024524=1;
	for(i=0;i<0x200000;i++);
	printf("data=0x%x\n",*(UINT32*)0xe8024530);
	*(UINT32*)0xe8024524=0;

}
