/* vxbGpioIntCtlr.c - Driver for Embedded Programmable Interrupt Controller */


/* includes */

#include <vxWorks.h>

#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <../src/hwif/h/vxbus/vxbAccess.h>
#include <hwif/util/hwMemLib.h>
#include <../src/hwif/intCtlr/vxbIntCtlrLib.h>
#include "vxbGpioIntCtrl.h"
#include "logLib.h"

int ns16550Pollprintf(char* pChar);
char sprintBuf_ns1655011[100];
#define DPRINTF_GPIO(X0, X1, X2, X3, X4, X5, X6)	{sprintf(sprintBuf_ns1655011,X0, X1, X2, X3, X4, X5, X6);ns16550Pollprintf(sprintBuf_ns1655011);}



LOCAL int       vxbSysGpioIntEnable (char *regBase,int vector);
LOCAL int       vxbSysGpioIntDisable (char *regBase,int vector);

LOCAL void vxbSysGpioInit(
	    VXB_DEVICE_ID pInst
	    );

LOCAL STATUS vxbGpioIntCtlrDevIsrConnect
    (
    VXB_DEVICE_ID	pIntCtlr,
    VXB_DEVICE_ID	pDev,
    int			indx,
    void		(*pIsr)(void * pArg),
    void *		pArg,
    int *		pInputPin
    );


LOCAL STATUS vxbGpioIntCtlrDevIsrDisconnect
    (
    VXB_DEVICE_ID	pIntCtlr,
    VXB_DEVICE_ID	pDev,
    int			indx,
    void		(*pIsr)(void * pArg),
    void *		pArg
    );

LOCAL STATUS vxbGpioIntCtlrDevIsrEnable
    (
    VXB_DEVICE_ID	pIntCtlr,
    VXB_DEVICE_ID	pDev,
    int			indx,
    void		(*pIsr)(void * pArg),
    void *		pArg
    );

LOCAL STATUS vxbGpioIntCtlrDevIsrDisable
    (
    VXB_DEVICE_ID	pIntCtlr,
    VXB_DEVICE_ID	pDev,
    int			indx,
    void		(*pIsr)(void * pArg),
    void *		pArg
    );

LOCAL void gpioIntCtlrInstInit(VXB_DEVICE_ID pInst);
LOCAL void gpioIntCtlrInstInit2(VXB_DEVICE_ID pInst);
LOCAL void gpioIntCtlrInstConnect(VXB_DEVICE_ID pInst);
LOCAL void vxbGpioIntrDeMux 
    (
    VXB_DEVICE_ID pIntCtlr
    );
void GpioDefaultISR(int vector);
/* locals */


LOCAL STATUS vxbGpioCleanInt(
		VXB_DEVICE_ID pIntCtlr,
		int gpioIndex 	 /*  gpioIndex	: 0~15*/
		);

LOCAL struct drvBusFuncs gpioIntCtlrFuncs =
    {
    gpioIntCtlrInstInit,        /* devInstanceInit */
    gpioIntCtlrInstInit2,       /* devInstanceInit2 */
    gpioIntCtlrInstConnect      /* devConnect */
    };


/*
 * Interrupt controllers use driver methods to communicate with
 * the VxBus interrupt management routines, vxbIntConnect(),
 * vxbIntEnable(), and so on.  The policy is that if a driver
 * publishes one of these methods, and the driver recognizes
 * the device trying to manage its interrupts, then these
 * routines are called.  However, if no driver publishes
 * these methods or no driver (with these methods published)
 * recognizes the device, then the system tries to connect
 * based on legacy support in the bus controller driver.
 */

METHOD_DECL(vxbIntCtlrConnect)
METHOD_DECL(vxbIntCtlrDisconnect)
METHOD_DECL(vxbIntCtlrEnable)
METHOD_DECL(vxbIntCtlrDisable)

DEVMETHOD_DEF(gpioIntClear, "clear gpio int");
METHOD_DECL(gpioIntClear)





LOCAL device_method_t gpioIntCtlr_methods[] =
    {
    DEVMETHOD(vxbIntCtlrConnect, vxbGpioIntCtlrDevIsrConnect),
    DEVMETHOD(vxbIntCtlrEnable, vxbGpioIntCtlrDevIsrEnable),
    DEVMETHOD(vxbIntCtlrDisconnect, vxbGpioIntCtlrDevIsrDisconnect),
    DEVMETHOD(vxbIntCtlrDisable, vxbGpioIntCtlrDevIsrDisable),
    DEVMETHOD(gpioIntClear, vxbGpioCleanInt),
    //DEVMETHOD(fpgaTimerIntConnect, vxbFpgaTimerIntSetup), 
    { 0, 0}
    };

LOCAL struct vxbDevRegInfo gpioIntCtlrDevRegistration =
    {
    NULL,                 /* pNext */
    VXB_DEVID_DEVICE,     /* devID */
    VXB_BUSID_PLB,        /* busID = PLB */
    VXB_VER_4_0_0,      /* busVer */
    "gpioIntCtrl",               /* drvName */
    NULL,                 /* pDrvBusFuncs */
    gpioIntCtlr_methods,   /* pMethods */
    NULL,                 /* devProbe */
    NULL                  /* pParamDefaults */
    };

/* externs */

IMPORT int vxPpcIntMask;		/* MSR interrupts mask bits */

IMPORT int      intCtlrPinFind (VXB_DEVICE_ID, int, VXB_DEVICE_ID, 
                                struct intCtlrHwConf *);

extern int sysGetCpuID(void);
/* globals */



LOCAL BOOL vxbGpioIntFirstTime = TRUE;



/******************************************************************************
*
* vxbGpioIntCtlrRegister - register gpioIntCtlr driver
*
* This routine registers the GPIO INT driver and device recognition
* data with the vxBus subsystem.
*
*/

void vxbGpioIntCtlrRegister(void)
    {
    gpioIntCtlrDevRegistration.pDrvBusFuncs = &gpioIntCtlrFuncs;
    vxbDevRegister((struct vxbDevRegInfo *)&gpioIntCtlrDevRegistration);
    }

/******************************************************************************
*
* gpioIntCtlrInstInit - initialize gpioIntCtlr device
*
*/
LOCAL void gpioIntCtlrInstInit
(
    VXB_DEVICE_ID pInst
)
{
    struct vxbGpioIntCtlrDrvCtrl * pDrvCtrl;
    HCF_DEVICE * pHcf = (HCF_DEVICE *)pInst->pBusSpecificDevInfo;
    
    pDrvCtrl = (struct vxbGpioIntCtlrDrvCtrl *)hwMemAlloc(sizeof(*pDrvCtrl));
    if ( pDrvCtrl != NULL )
	{
	/* save instance ID */
    	 vxbRegMap (pInst, 0, &pDrvCtrl->handle);
		pDrvCtrl->pInst = pInst;
		/* get connectivity info from hwconf */
		intCtlrHwConfGet(pInst, pHcf, &(pDrvCtrl->isrHandle));
	
		if(devResourceGet (pHcf, "regBase", HCF_RES_INT,(void *)&(pInst->pRegBase[0]))==ERROR)
			pInst->pRegBase[0] =(void*)LOCAL_BUS_ADDR_CS3;
		
		//if(devResourceGet (pHcf, "maxIvec", HCF_RES_INT,(void *)&(pDrvCtrl->maxIvec))==ERROR)
		//	pDrvCtrl->maxIvec = 32;
		
		if(devResourceGet (pHcf, "maxGpioInt", HCF_RES_INT,(void *)&(pDrvCtrl->maxGpioInt))==ERROR)
			pDrvCtrl->maxGpioInt = 16;
		/* advertise intConnect() and other methods */
	
		pInst->pMethods = &gpioIntCtlr_methods[0];
		/* save pDrvCtrl in VXB_DEVICE structure */
		pInst->pDrvCtrl = pDrvCtrl;
		vxbRegMap (pInst, 0, &pDrvCtrl->regAccessHandle);
		vxbSysGpioInit(pInst);
	}
}

/******************************************************************************
*
* epicIntCtlrInstInit2 - initialize epicIntCtlr device
*
*/

LOCAL void gpioIntCtlrInstInit2
    (
    VXB_DEVICE_ID pInst
    )
    {
	   VOIDFUNCPTR defaultVec;         /*  default vector */

	    /* Get the default vector connected to the External Interrupt (0x500) */

	    defaultVec = GpioDefaultISR;

	   // *(UINT32*)(LOCAL_BUS_ADDR_CS3 + GPIO_CTRL_REG) = 0xFFFF;

	    if(vxbGpioIntFirstTime)
		{
	    	if(vxbIntConnect(pInst,0,vxbGpioIntrDeMux,(void*)pInst) == ERROR)
	    		DPRINTF_GPIO("vxbIntConnect error!\n",1,2,3,4,5,6);
	    	if(vxbIntEnable(pInst,0,vxbGpioIntrDeMux,(void*)pInst)==ERROR)
	    		DPRINTF_GPIO("vxbIntEnable ERROR!\n",1,2,3,4,5,6);	
	    	vxbGpioIntFirstTime= FALSE;
		}   
	    //DPRINTF_GPIO("exit gpioIntCtlrInstInit2!\n",1,2,3,4,5,6);
    }

/******************************************************************************
*
* gpioIntCtlrInstConnect - connect epicIntCtlr device to I/O system
*
*/

LOCAL void gpioIntCtlrInstConnect
    (
    VXB_DEVICE_ID pInst
    )
    {
    }

void GpioDefaultISR(int vector)
{
	DPRINTF_GPIO("gpioDefaultISR:Spurious interrupt 0x%x\n",vector,0,0,0,0,0);
}

/*******************************************************************************
*
* vxbSysGpioInit - initialize the vxbEpic controller
*
* This routine resets the global Configuration Register, thus it:
*  -  disables all interrupts
*  -  sets vxbEpic registers to reset values
*
* It then sets the EPIC operation mode to Mixed Mode (vs. Pass Through
* mode). At this point only mixed mode is supported which means the EPIC
* is not configured for the pass through mode.
*
* Only direct interrupt sources is supported by the PIC.  Serial
* stype interrupt is not available.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void vxbSysGpioInit 
    (
    VXB_DEVICE_ID pInst
    )
    {
    char  *regBase = pInst->pRegBase[0];

    /* disable all interrupts */
    switch( sysGetCpuID())
    {
    case 0:
    	 *(UINT32*)(regBase + GPIO_INT_ENABLE_A) = 0x0;
 	    *(UINT32*)(regBase + GPIO_INT_STATUS_A) = 0xFFFF;
    	break;
    case 1:
    	 *(UINT32*)(regBase + GPIO_INT_ENABLE_B) = 0x0;
  	    *(UINT32*)(regBase + GPIO_INT_STATUS_B) = 0xFFFF;;
    	break; 

    default:
    	break;
    }  
    }
/*******************************************************************************
*
* vxbSysGpioIntEnable - enable a EPIC interrupt level
*
* This routine enables a specified EPIC interrupt level.
*
* \NOMANUAL
*
* RETURNS: OK, ERROR
*
* ERRNO: N/A
*/

LOCAL int vxbSysGpioIntEnable
    (
    char *regBase,
    int vector
    )
    {
   /* enable interrupt on EPIC */
   switch( sysGetCpuID())
   {
   case 0:
	 *(UINT32*)(regBase + GPIO_INT_STATUS_A) |= (1<<vector);
   	 *(UINT32*)(regBase + GPIO_INT_ENABLE_A) |= (1<<vector);
   	break;
   case 1:
	 *(UINT32*)(regBase + GPIO_INT_STATUS_B) |= (1<<vector);
   	 *(UINT32*)(regBase + GPIO_INT_ENABLE_B) |= (1<<vector);
   	break; 
   default:
   	break;
   }   
   return OK;
    }

/*******************************************************************************
*
* vxbSysGpioIntDisable - disable a GPIO interrupt level
*
* This routine disables a specified EPIC interrupt level.
*
* \NOMANUAL
*
* RETURNS: OK, ERROR
*
* ERRNO: N/A
*/

LOCAL int  vxbSysGpioIntDisable
    (
    char *regBase,
    int vector
    )
    {
    /* disable interrupt on GPIO */
    switch( sysGetCpuID())
    {
    case 0:
    	 *(UINT32*)(regBase + GPIO_INT_ENABLE_A) &= (~(1<<vector));
    	break;
    case 1:
    	 *(UINT32*)(regBase + GPIO_INT_ENABLE_B)  &= (~(1<<vector));
    	break; 
    default:
    	break;
    }    
    return OK;

    }
/*******************************************************************************
*
* vxbGpioIntrDeMux - SIU interrupt demultiplexer 
*
* This routine must be bound to external interrupt exception (vector 0x500). 
* It is used to call the appropriate handler with its argument when an
* interrupt occurs. 
*
* The interrupts are prioritized in the following order:
*
* NOTE: when this function is called the interrupts are still locked. It's
* this function responsability to unlock the interrupt.
*
* RETURNS: N/A
*/

LOCAL void vxbGpioIntrDeMux 
    (
    VXB_DEVICE_ID pIntCtlr
    )
    {
    struct vxbGpioIntCtlrDrvCtrl * pDrvCtrl = pIntCtlr->pDrvCtrl;
    char  *regBase = pIntCtlr->pRegBase[0];
    int intLevel,i,cpuID=0;
    UINT32 intValue,data=0;
    UINT32 maskOldValue = 0;
    cpuID = sysGetCpuID();
    /* read the interrupt vector register */
    switch( cpuID )
    {
    case 0:    
    	maskOldValue = *(UINT32*)(regBase + GPIO_INT_ENABLE_A);
    	intValue = (*(UINT32*)(regBase + GPIO_INT_STATUS_A)) & ((*(UINT32*)(regBase + GPIO_INT_ENABLE_A)));
   	    *(UINT32*)(regBase + GPIO_INT_ENABLE_A) = 0x0;
    	break;
    case 1:
    	maskOldValue = *(UINT32*)(regBase + GPIO_INT_ENABLE_B);
    	intValue = (*(UINT32*)(regBase + GPIO_INT_STATUS_B)) & ((*(UINT32*)(regBase + GPIO_INT_ENABLE_B)));
     	*(UINT32*)(regBase + GPIO_INT_ENABLE_B) = 0x0;
    	break; 
    default:
    	break;
    } 
    
    	
    for(i=0;i<(pDrvCtrl->maxGpioInt);i++)
    {
    	if((intValue>>i)&0x01)
    	{
    		intCpuUnlock (vxPpcIntMask); 
    		/* call the Interrupt Handler */
    		//hdz add for clear int 20121218
    		data = (1<<i);
    	    switch( cpuID )
    	    {
				case 0: 
					vxbWrite32((void*)pDrvCtrl->handle,(UINT32*)((char*)regBase + GPIO_INT_STATUS_A),data);
					break;
				case 1: 
					vxbWrite32((void*)pDrvCtrl->handle,(UINT32*)((char*)regBase + GPIO_INT_STATUS_B),data);
					break;
				default:
					break;
					
    	    }
    		VXB_INTCTLR_ISR_CALL(&pDrvCtrl->isrHandle, i);
    		/* restore the interrupt mask */
    		intLevel = intCpuLock ();

        }
    }
    
    switch( cpuID )
    {
		case 0:  
			*(UINT32*)(regBase + GPIO_INT_ENABLE_A) = maskOldValue;
			break;
		case 1:  
			*(UINT32*)(regBase + GPIO_INT_ENABLE_B) = maskOldValue;
			break;
		default:
			break;
    }
}
/*******************************************************************************
*
* vxbGpioIntCtlrDevIsrConnect - connect device ISR
*
*/

LOCAL STATUS vxbGpioIntCtlrDevIsrConnect
    (
    VXB_DEVICE_ID	pIntCtlr,
    VXB_DEVICE_ID	pDev,
    int			indx,
    void		(*pIsr)(void * pArg),
    void *		pArg,
    int *		pInputPin
    )
    {
    struct vxbGpioIntCtlrDrvCtrl * pDrvCtrl = pIntCtlr->pDrvCtrl;
    int  inputPin = ERROR;

	
    inputPin = intCtlrPinFind(pDev, indx, pIntCtlr, &pDrvCtrl->isrHandle);
    if ( inputPin == ERROR ) 
    	return(ERROR);

    *pInputPin = inputPin;
    
    if(vxbGpioIntFirstTime)
	{
    	vxbIntConnect(pIntCtlr,0,vxbGpioIntrDeMux,(void*)pIntCtlr);
    	vxbIntEnable(pIntCtlr,0,vxbGpioIntrDeMux,(void*)pIntCtlr);	
    	vxbGpioIntFirstTime= FALSE;
	}

    if(intCtlrISRAdd(&pDrvCtrl->isrHandle, inputPin, pIsr, pArg)==OK)
    {
//    	DPRINTF_GPIO("vxbgpioIntCtlrDevIsrConnect:inputPin=0x%x,pIsr=0x%x,pArg=0x%x\n",inputPin,pIsr,pArg,4,5,6);
    	return OK;
    	
    }
    else 
    {
    	DPRINTF_GPIO("intCtlrISRAdd error\n",1,2,3,4,5,6);
    	return ERROR;
    } 
    }

/*******************************************************************************
*
* vxbGpioIntCtlrDevIsrDisconnect - disconnect device ISR
*
*  FIXME: This needs to be implemented
*
*/

LOCAL STATUS vxbGpioIntCtlrDevIsrDisconnect
    (
    VXB_DEVICE_ID	pIntCtlr,
    VXB_DEVICE_ID	pDev,
    int			indx,
    void		(*pIsr)(void * pArg),
    void *		pArg
    )
    {
    return(ERROR);
    }

/*******************************************************************************
*
* vxbGpioIntCtlrDevIsrEnable - enable device ISR
*
*/

LOCAL STATUS vxbGpioIntCtlrDevIsrEnable
    (
    VXB_DEVICE_ID	pIntCtlr,
    VXB_DEVICE_ID	pDev,
    int			indx,
    void		(*pIsr)(void * pArg),
    void *		pArg
    )
    {	    
    struct vxbGpioIntCtlrDrvCtrl * pDrvCtrl = pIntCtlr->pDrvCtrl;
    int                 inputPin;
    char *regBase = pIntCtlr->pRegBase[0];
    
    inputPin = intCtlrPinFind(pDev, indx, pIntCtlr, &pDrvCtrl->isrHandle);
    
    if ( inputPin == ERROR )
        return(ERROR);

	intCtlrISREnable(&(pDrvCtrl->isrHandle), inputPin, pIsr, pArg);
	
	vxbSysGpioIntEnable(regBase,inputPin);

    return(OK);
    }

/*******************************************************************************
*
* vxbGpioIntCtlrDevIsrDisable - Disable device ISR
*
*/

LOCAL STATUS vxbGpioIntCtlrDevIsrDisable
    (
    VXB_DEVICE_ID	pIntCtlr,
    VXB_DEVICE_ID	pDev,
    int			indx,
    void		(*pIsr)(void * pArg),
    void *		pArg
    )
    {
    struct vxbGpioIntCtlrDrvCtrl * pDrvCtrl = pIntCtlr->pDrvCtrl;
    int                 inputPin;
    BOOL		allDisabled;
    char *regBase = pIntCtlr->pRegBase[0];
    
    inputPin = intCtlrPinFind(pDev, indx, pIntCtlr,&pDrvCtrl->isrHandle);
    if ( inputPin == ERROR )     return(ERROR);
    
	allDisabled = intCtlrISRDisable(&(pDrvCtrl->isrHandle), inputPin,
					pIsr, pArg);

	if (allDisabled)  vxbSysGpioIntDisable(regBase,inputPin);
    return(OK);
    }


LOCAL STATUS vxbGpioCleanInt(
		VXB_DEVICE_ID pIntCtlr,
		int gpioIndex 	 /*  gpioIndex	: 0~15*/
		)
{
	struct vxbGpioIntCtlrDrvCtrl * pDrvCtrl = pIntCtlr->pDrvCtrl;
	char  *regBase = pIntCtlr->pRegBase[0];
	UINT32 data=0;
	int cpuId;
	cpuId = sysGetCpuID();
	data |= (1<<gpioIndex);
	switch(cpuId)
	{
	case 0:
		vxbWrite32((void*)pDrvCtrl->handle,(UINT32*)((char*)regBase + GPIO_INT_STATUS_A),data);
		break;
	case 1:
		vxbWrite32((void*)pDrvCtrl->handle,(UINT32*)((char*)regBase + GPIO_INT_STATUS_B),data);
		break;	
	default:
		break;
	}
	 return OK;
}


#if 0

void vxbGpioIntCtlrpDrvCtrlShow()
{
    struct vxbGpioIntCtlrDrvCtrl * pDrvCtrl = gpioIntCtlrID->pDrvCtrl;
    struct vxbIntCtlrInpTop * pTop;
    struct intCtlrHwConf *    pInputs;
    struct vxbIntCtlrInput *  pTbl;
    struct vxbIntCtlrPin *    pPin;
    struct intCtlrMoreDrvName *       pMore;
    UINT8 i,j;
    
    pInputs= &(pDrvCtrl->isrHandle);
    pTop = pInputs->pTop;
    
    // printf("fpgaIntCtlrID=0x%x\n",fpgaIntCtlrID);
    // printf("&(pDrvCtrl->isrHandle)=0x%x\n",&(pDrvCtrl->isrHandle));
    DPRINTF_GPIO("gpioIntCtlrID=0x%x\n",gpioIntCtlrID,2,3,4,5,6);
    DPRINTF_GPIO("&(pDrvCtrl->isrHandle)=0x%x\n",&(pDrvCtrl->isrHandle),2,3,4,5,6);
    
    for ( i = 0 ; i < 2 ; i++ )
    {
    	pTbl=pTop->tbl[i];
		if(pTbl !=NULL)
		{
			DPRINTF_GPIO("size of pin is 0x%x\n",VXB_INTCTLRLIB_NUM_PINS(pInputs),2,3,4,5,6);
			for ( j = 0 ; j < 8 ; j++ )
			{
				pPin = &pTbl->pins[j];	
				DPRINTF_GPIO("pPin=0x%x\n",pPin,2,3,4,5,6);
				if ( ( pPin->drvName != 0 ) )
				{
					// printf("%s_%d index %d\n",pPin->drvName, pPin->drvUnit, pPin->intIndex);
					DPRINTF_GPIO("%s_%d index %d\n",pPin->drvName, pPin->drvUnit, pPin->intIndex,4,5,6);
					 pMore = pPin->pMoreNames;
					 while ( pMore != NULL )
					 {
						//printf("%s_%d index %d\n",  pMore->drvName, pMore->drvUnit, pMore->pinIndex);
						 DPRINTF_GPIO("%s_%d index %d\n",  pMore->drvName, pMore->drvUnit, pMore->pinIndex,4,5,6);
						pMore = pMore->pNext;
					 }
					//printf("isr = userISR (*0x%08x)(0x%08x)\n",(int)pPin->isr, (int)pPin->pArg);
					 DPRINTF_GPIO("isr = userISR (*0x%08x)(0x%08x)\n",(int)pPin->isr, (int)pPin->pArg,3,4,5,6); 
				}
			}
		}
    }
    	
}

#endif
