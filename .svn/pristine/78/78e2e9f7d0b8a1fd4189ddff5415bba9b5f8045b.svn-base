/* includes */

#include <vxWorks.h>
#include <stdio.h>			/* for printf() */
#include <logLib.h>			/* for logMsg() */
#include <string.h>			/* for strcmp() */

#include <vxBusLib.h>			/* for VXB_DEVICE_ID */
#include <hwif/vxbus/vxBus.h>
#include <hwif/util/hwMemLib.h>		/* for hwMemAlloc() */
#include <hwif/vxbus/hwConf.h>		/* for devResourceGet() and hcfDeviceGet() */
#include <driverControl.h>		/* for struct vxbDriverControl */
#include "vxbm8641SynUart.h"



#define SYN_UART_DBG_ON
#undef SYN_UART_DBG_ON
#ifdef SYN_UART_DBG_ON
#define SYN_UART_DBG_MSG(fmt,a,b,c,d,e,f)  logMsg(fmt,a,b,c,d,e,f)
#define STATIC 
#else 
#define SYN_UART_DBG_MSG(fmt,a,b,c,d,e,f)
#undef STATIC
#define STATIC static
#endif 



/* locals */

STATIC void synUartInstInit(VXB_DEVICE_ID pInst);
STATIC void synUartInstInit2(VXB_DEVICE_ID pInst);
STATIC void synUartInstConnect(VXB_DEVICE_ID pInst);


STATIC STATUS synUartDriverCtrl(VXB_DEVICE_ID  pInst,  struct vxbDriverControl *pControl);


STATIC void synUartIntHandle(VXB_DEVICE_ID pInst);





STATIC struct drvBusFuncs synUartFuncs =
    {
    	synUartInstInit,        /* devInstanceInit */
    	synUartInstInit2,       /* devInstanceInit2 */
    	synUartInstConnect      /* devConnect */
    };

STATIC struct vxbDevRegInfo synUartDevRegistration =
    {
    NULL,               /* pNext */
    VXB_DEVID_DEVICE,   /* devID */
    VXB_BUSID_PLB,      /* busID = PLB */
    VXB_VER_4_2_1,      /* vxbVersion */
    "synUart",         /* drvName */
    &synUartFuncs,     /* pDrvBusFuncs */
    NULL,               /* pMethods */
    NULL,               /* devProbe */
    NULL                /* pParamDefaults */
    };


STATIC device_method_t synUart_methods[] =
    {
    DEVMETHOD(driverControl, synUartDriverCtrl),
    { 0, 0}
    };


//static UINT32 dirFlag[6]={0,1,0,1,0};

/* ------------------------------------------------------------ */

/* local defines       */

/* driver functions */

/*******************************************************************************
*
* wrsampleRegister - register wrsample driver
*
* This routine registers the wrsample driver and device recognition
* data with the vxBus subsystem.
*
* RETURNS: N/A
*
* ERRNO
*/

extern int sysGetCpuID(void);

void m8641SynUartDrvRegister(void)
    {
		//if(sysGetCpuID() == 0) 
		{
			SYN_UART_DBG_MSG("run into m8641SynUartDrvRegister\n" ,1,2,3,4,5,6);	
			vxbDevRegister((struct vxbDevRegInfo *)&synUartDevRegistration);
		}
    }

/*******************************************************************************
*
* wrsampleInstInit - initialize wrsample device
*
* This is the wrsample initialization routine.
*
* RETURNS: N/A
*
* ERRNO
*/



STATIC void synUartInstInit
    (
    VXB_DEVICE_ID pInst
    )
{
    SYN_UART_DRV_CTRL *pDrvCtrl;
    /* to store the HCF device */
    HCF_DEVICE *        pHcf = NULL; 

    SYN_UART_DBG_MSG("run into synUartInstInit\n" ,1,2,3,4,5,6);
    if (pInst == NULL)    
    {
    	logMsg("pInst == NULL\n",1,2,3,4,5,6);
		return;
    }
    pDrvCtrl =(SYN_UART_DRV_CTRL * )hwMemAlloc(sizeof (SYN_UART_DRV_CTRL));
    if (pDrvCtrl == NULL)  return;  

    /* get the HCF device from vxBus device structure */
    pHcf = hcfDeviceGet (pInst);
    if (pHcf == NULL)     return;
    if (devResourceGet(pHcf,"regBase", HCF_RES_INT,(void *)&pDrvCtrl->regBase) != OK)      pDrvCtrl->regBase = 0;
    if (devResourceGet(pHcf,"dirFlag", HCF_RES_INT,(void *)&pDrvCtrl->dirFlag) != OK)      pDrvCtrl->dirFlag = 0;
	pInst->pDrvCtrl = pDrvCtrl;
    /* publish methods */
    pInst->pMethods = &synUart_methods[0];

 }


/*******************************************************************************
*
* wrsampleInstInit2 - initialize wrsample device
*
* This is seconde phase initialize routine for VxBus driver. 
*
* RETURNS: N/A
*
* ERRNO
*/

STATIC void synUartInstInit2
    (
    VXB_DEVICE_ID pInst
    )
    {
    SYN_UART_DRV_CTRL *pDrvCtrl;
    
    SYN_UART_DBG_MSG("run into synUartInstInit2\n" ,1,2,3,4,5,6);
    pDrvCtrl=pInst->pDrvCtrl;
    pDrvCtrl->userISR[RCV_INT_F] = NULL;
    pDrvCtrl->userISR[EMPTY_INT_F] = NULL;
    pDrvCtrl->unitNumber=pInst->unitNumber;

    pDrvCtrl->synUartAccessSem = semBCreate (SEM_Q_FIFO, SEM_FULL);
	vxbIntConnect (pInst, 0, (VOIDFUNCPTR)synUartIntHandle,pInst);
	vxbIntEnable (pInst, 0,(VOIDFUNCPTR) synUartIntHandle,pInst);
}



/*******************************************************************************
*
* wrsampleInstConnect - VxBus connect phase routine for wrsample driver
*
* This is connect phase routine.
*
* RETURNS: N/A
*
* ERRNO: not set
*/

STATIC void synUartInstConnect
    (
    VXB_DEVICE_ID pInst
    )
    {
	/*synUart_DBG_MSG("synUartInstConnect() called\n", 1,2,3,4,5,6);*/
    }


/*******************************************************************************
*
* i2cDriverCtrl - handle {driverControl}() method calls
*
* This routine handles driverControl method call from application layer.
*
* RETURNS: ERROR or OK
*
* ERRNO: not set
*/

STATIC STATUS synUartDriverCtrl
    (
    VXB_DEVICE_ID               pInst,
    struct vxbDriverControl *   pControl
    )
    {
	
	SYN_UART_DRV_CTRL *pDrvCtrl;
	SYN_UART_DRV_CTRL_INFO *pDrvCtrlInfo;
   	UINT32 value=0;
   	UINT32 regBase=0;
   	UINT8 shiftBit;
   	UINT32 dataValue;
   	INT8 timeout=4;
   	
    //SYN_UART_DBG_MSG("run into synUartDriverCtrl\n" ,1,2,3,4,5,6);
    
    pDrvCtrlInfo=pControl->drvCtrlInfo;
    
    if ( pInst == NULL || pControl == NULL )   return(ERROR);
    if ( strcmp(pControl->driverName, "synUart" ) != 0 ) return(ERROR);
    pDrvCtrl =(SYN_UART_DRV_CTRL *)pInst->pDrvCtrl; //指针为什么要指向他？
    
    if(semTake(pDrvCtrl->synUartAccessSem,0x100)==ERROR)/*WAIT_FOREVER*/ 
    {
    	printf("a process is using the channel now!!\n");
    	 return(ERROR);	
    }    
    
    switch (pControl->drvCtrlCmd)
    {
		case SYN_UART_BAUD_SET:
			regBase = (pDrvCtrl->regBase)+SYN_UART_BAUD_REG(pDrvCtrl->unitNumber);
			*(UINT32*)regBase = *pDrvCtrlInfo->value;
			//logMsg("SYN_UART_BAUD_SET:0x%x\n",*pDrvCtrlInfo->value,2,3,4,5,6);
			timeout =4;
			while(timeout--)
			{
				dataValue=*(UINT32*)regBase;
				if(dataValue == (*pDrvCtrlInfo->value)) break;
			}
			if(timeout<=0) return ERROR;
			break;	
			
		case SYN_UART_BAUD_GET:
			regBase = (pDrvCtrl->regBase)+SYN_UART_BAUD_REG(pDrvCtrl->unitNumber);
			*pDrvCtrlInfo->value = *(UINT32*)regBase ;
			//logMsg("regBase=0x%x,value=0x%x,pDrvCtrl->unitNumber=0x%x\n",regBase,*pDrvCtrlInfo->value,pDrvCtrl->unitNumber,4,5,6);
			break;	
		case SYN_UART_SWITCH_CPU:
			regBase = (pDrvCtrl->regBase)+SYN_UART_SWITCH_CPU_REG(pDrvCtrl->unitNumber);
			*(UINT32*)regBase = *pDrvCtrlInfo->value;
			break;
		case SYN_UART_CHAIN_ENABLE:
			regBase = (pDrvCtrl->regBase)+SYN_UART_ENABLE_REG(pDrvCtrl->unitNumber);
			while(*(UINT32*)regBase !=3) *(UINT32*)regBase =3;
			break;
		case SYN_UART_CHAIN_DISABLE:
			regBase = (pDrvCtrl->regBase)+SYN_UART_ENABLE_REG(pDrvCtrl->unitNumber);
			*(UINT32*)regBase =0;
			break;
	    case SYN_UART_RCV_INT_SETUP:
			pDrvCtrl->userISR[RCV_INT_F] = pDrvCtrlInfo->intCallBack;			
	    	//printf("0x%x---0x%x\n",pDrvCtrl->userISR[RCV_INT_F],pDrvCtrlInfo->intCallBack);
	    	break;
	    case SYN_UART_EMPTY_INT_SETUP:
		    	pDrvCtrl->userISR[EMPTY_INT_F] =pDrvCtrlInfo->intCallBack;
		    	break;
	    case SYN_UART_FUNC_GET:
	    	*pDrvCtrlInfo->value=pDrvCtrl->dirFlag;
	    	break;
	    case SYN_UART_WRITE:
	    	regBase = (pDrvCtrl->regBase)+SYN_UART_DATA_REG(pDrvCtrl->unitNumber);
	    	*(UINT32*)regBase = *pDrvCtrlInfo->value;
	    	break;
	    case SYN_UART_READ:
	    	regBase = (pDrvCtrl->regBase)+SYN_UART_DATA_REG(pDrvCtrl->unitNumber);
	    	*pDrvCtrlInfo->value=*(UINT32*)regBase;
	    	//printf("0x%x\n",*pDrvCtrlInfo->value);
	    	break;
	    case SYN_UART_STATUS_GET:
	    	regBase = (pDrvCtrl->regBase)+SYN_UART_STATUS_REG(pDrvCtrl->unitNumber);
	    	*pDrvCtrlInfo->value= *(UINT32*)regBase;
	    	break;
	    case SYN_UART_RCV_INT_MASK:
	    	shiftBit = 0;
	     	goto intMask;
	    case SYN_UART_EMPTY_INT_MASK:
	    	shiftBit = 1;
intMask:	
            regBase = (pDrvCtrl->regBase)+SYN_UART_INT_ENABLE_REG(pDrvCtrl->unitNumber);
            value= *pDrvCtrlInfo->value;
	    	if(value&0x01)  *(UINT32*)regBase &= (~(1<<shiftBit));
	    	else 	*(UINT32*)regBase |= (1<<shiftBit);
	    	break;
		default:
			break;
    }
    semGive(pDrvCtrl->synUartAccessSem);
    return OK;
}


STATIC void synUartIntHandle(VXB_DEVICE_ID pInst)
{
	UINT32 intFlag,regBase/*,intEnable*/,flag/*,mask,i*/,clearIntFlag/*,dataValue*/;
	SYN_UART_DRV_CTRL *pDrvCtrl;
	P_INT_USR_ISR userISR=NULL;
	UINT32 dataValue;
	pDrvCtrl = pInst->pDrvCtrl;	
	regBase = pDrvCtrl->regBase+SYN_UART_INT_FLAG_REG(pDrvCtrl->unitNumber);
	intFlag=*(UINT32*)(regBase);
	
//	for(i=0;i<2;i++)
	{
		flag = (intFlag)&0x01;
		//logMsg("intFlag 0x%x\n",intFlag,2,3,4,5,6);
		if(flag==1)
		{
			clearIntFlag=1;
		
			*(UINT32*)(regBase) = clearIntFlag;
			for(;;)
			{
				dataValue=*(UINT32*)(regBase);
		//		logMsg("0x%x\n",dataValue,2,3,4,5,6);
				//if(((dataValue)&0x1) != 0) *(UINT32*)(regBase) = clearIntFlag;
				//else break;
				 *(UINT32*)(regBase) = clearIntFlag;
				 break;
			}
	
			userISR=pDrvCtrl->userISR[0];
			if(userISR!= NULL)
			{
				(*userISR)(pDrvCtrl->unitNumber);
			}

#if 0	
			//if((pDrvCtrl->unitNumber)== 1) logMsg("1-%d:*(UINT32*)(regBase)=0x%x-0x%x\n",i,*(UINT32*)(regBase),clearIntFlag,3,4,5,6);
			while(1)
			{
				dataValue=*(UINT32*)(regBase);
				//if((pDrvCtrl->unitNumber)== 1) logMsg("2-%d:*(UINT32*)(regBase)=0x%x-0x%x\n",i,dataValue,clearIntFlag,3,4,5,6);
				if(((dataValue>>i)&0x01) != 1) 
				{
					*(UINT32*)(regBase) = clearIntFlag;
					//if((pDrvCtrl->unitNumber)== 1) logMsg("3-%d:*(UINT32*)(regBase)=0x%x-0x%x\n",i,*(UINT32*)(regBase),clearIntFlag,3,4,5,6);
				}
				else break;
			}
#endif
		}
	}
}


