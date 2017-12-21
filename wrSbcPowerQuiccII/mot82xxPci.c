/* mot82xxPci.c - Motorola  wrSbcPowerQuiccII 82xx PCI Bridge functions */

/* Copyright 2003 - 2008 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/*
modification history
--------------------
01i,21aug08,wap  Set prefetch enable bit for inbound PCI window
01h,23sep04,mdo  Documentation fixes for apigen
01m,07jan05,h_k  cleaned up compiler errors and warnings.
01j,23dec04,ckj  WLAN support for Atheros AR521X
01i,21dec04,ami  Compilation error removed
01h,21dec04,ami  Merged in changes for USB Porting
01g,08aug03,scm  remove references to GEI for release...
01f,05aug03,scm  update to support basic PCI-to-PCI bridging...
01e,02jun03,scm  up the delay for faster boards...
01d,29may03,scm  remove diab warning...
01c,13may03,scm  add delay to get over release of the PCI reset through
                 bridge...
01b,09may03,scm  remove duplicate var names...
01a,24apr03,scm  File created.
*/

/* PCI Host Bridge setup File */

#include "vxWorks.h"
#include "config.h"
#include "sysLib.h"

#include "drv/pci/pciConfigLib.h"
#include "drv/pci/pciIntLib.h"
#include "drv/pci/pciAutoConfigLib.h"
#include "sysBusPci.h"
#include "drv/mem/m8260Memc.h"
#include "drv/intrCtl/m8260IntrCtl.h"
#include "drv/mem/m8260Siu.h"
#include "mot82xxPci.h"

IMPORT void sysUsDelay (UINT32 delay);

LOCAL BOOL firstTime = TRUE;

LOCAL DL_LIST	pciDeviceIntList[NUMBER_OF_PIC_IRQS];	/* link list of int handlers */

LOCAL void pciDeviceIntHandler(void);

STATUS pciDeviceIntConnect(int vector, VOIDFUNCPTR *handler, int arg);
#if defined (INCLUDE_FEI_END) || defined (INCLUDE_AR521X_END)
STATUS pciDeviceIntDisconnect(int vector, VOIDFUNCPTR *handler);
#endif

STATUS pciDeviceIntDisconnect2(int vector, VOIDFUNCPTR *handler, int arg);

STATUS pciDeviceIntEnable(int);
STATUS pciDeviceIntDisable(int);

/*************************************************************************
*
* mot82xxBridgeInit - Bridge/Interrupts/PCI Bus initialization
*
* This function performs all the initialization
* required for the Bridge/Interrupts/PCI Bus to function. It does some
* low level processor initialization which might normally be done in
* romInit as it is optional to do use this and shows what the core
* changes required to bring up the bridge.
*
* RETURNS : NONE
*
* ERRNO
*/

void mot82xxBridgeInit()
    {
     STATUS result;
     UINT32 compMsk;
     UINT32 mem_pciSz;
     UINT32 io_pciSz;
     UINT32 idx;

     /* Take the bridge out of reset */
     sysPciOutLong ((UINT32*)PQII_PCI_GEN_CTRL, 0x00000001);

     /* determine the size of the PCI Memory space */
     for (mem_pciSz=0, idx=0; idx < 32; idx++)
         {
         if ((PCI_MEM_SIZE >> idx) & 0x1)
             {
             while (idx < 32)
                {
                mem_pciSz |= (0x1 << idx);
                idx++;
                }
             }
         }
     compMsk = (0x80000000 | (mem_pciSz >> 12));  /* ENABLE, to PCI Memory, No Prefetch */

     /*
      * Map one aperature for Memory Mapping
      */
    sysPciOutLong ((UINT32*)PQII_PCI_OUT_COMP_MSK0, 0x00000000);
    sysPciOutLong ((UINT32*)PQII_PCI_OUT_TRANS_ADRS0, (CPU_PCI_MEM_ADRS >> 12));
    sysPciOutLong ((UINT32*)PQII_PCI_OUT_BASE_ADRS0, (PCI_MEM_BASE >> 12));
    sysPciOutLong ((UINT32*)PQII_PCI_OUT_COMP_MSK0,  compMsk);


    /* determine the size of the PCI I/O space */
    for (io_pciSz=0, idx=0; idx < 32; idx++)
        {
        if ((PCI_IO_SIZE >> idx) & 0x1)
            {
            while (idx < 32)
                {
                io_pciSz |= (0x1 << idx);
                idx++;
                }
            }
        }
    compMsk = (0xC0000000 | (io_pciSz >> 12));  /* ENABLE, to PCI IO, No Prefetch */

    /*
     * Map one aperture for I/O Space
     */
    sysPciOutLong ((UINT32*)PQII_PCI_OUT_COMP_MSK1, 0x00000000);
    sysPciOutLong ((UINT32*)PQII_PCI_OUT_TRANS_ADRS1, (CPU_PCI_IO32_ADRS >> 12));
    sysPciOutLong ((UINT32*)PQII_PCI_OUT_BASE_ADRS1, (PCI_IO_BASE >>12));
    sysPciOutLong ((UINT32*)PQII_PCI_OUT_COMP_MSK1, compMsk);

    /*
     * Third not used - Set to zero
     */
    sysPciOutLong ((UINT32*)PQII_PCI_OUT_COMP_MSK2, 0x00000000);
    sysPciOutLong ((UINT32*)PQII_PCI_OUT_TRANS_ADRS2, 0x00000000);
    sysPciOutLong ((UINT32*)PQII_PCI_OUT_BASE_ADRS2, 0x00000000);

    /*
     * Map the IN aperture
     */
    sysPciOutLong ((UINT32*)PQII_PCI_IN_TRANS_ADRS0, (PCI_LOCAL_MEM_BUS >> 12));
    sysPciOutLong ((UINT32*)PQII_PCI_IN_BASE_ADRS0, (PCI_MSTR_MEM_BUS >> 12));
    /* ENABLE, Snooping Enabled, Prefetch Enabled */
    sysPciOutLong ((UINT32*)PQII_PCI_IN_COMP_MSK0, (0xA0000000 | (mem_pciSz >> 12)));

    /*
     * Second IN not used
     */
    sysPciOutLong ((UINT32*)PQII_PCI_IN_TRANS_ADRS1, 0x00000000);
    sysPciOutLong ((UINT32*)PQII_PCI_IN_BASE_ADRS1, 0x00000000);
    sysPciOutLong ((UINT32*)PQII_PCI_IN_COMP_MSK1, 0x00000000);

    /*
     * Write the configuration registers to configure to be PCI Master and
     * PCI space as memory & IO space
     */
    result = sysPciConfigWrite(0,0,0,COMMAND_REGISTER_OFFSET,COMMAND_REGISTER_WIDTH,
                               (BUS_MASTER_ENABLE_BIT |
                                MEMORY_SPACE_ACCESS_ENABLE_BIT |
                                IO_SPACE_ACCESS_ENABLE_BIT));

    /* Disable the discard Timer */
    sysPciOutLong ((UINT32*)PQII_DISCARD_TIMER_CTRL, 0x00FFFFFE);

    /*
     * Set the DMA to use low bandwidth on 60x bus and route all interrupts
     * to core
     */
    sysPciOutLong ((UINT32*)PQII_GEN_PURPOSE_CTRL, 0x00000000);

    /* Clear all error status */
    sysPciOutLong ((UINT32*)PQII_ERR_STAT, 0x00001FFF);

    /* Set the PCI Outbound Translation Registers */
    *(UINT32 *)PQII_PCI_MSK0 = PCIMSK0_SETTING;
    *(UINT32 *)PQII_PCI_MSK1 = PCIMSK1_SETTING;

    *(UINT32 *)PQII_PCI_BR0 = PCIBR0_SETTING;
    *(UINT32 *)PQII_PCI_BR1 = PCIBR1_SETTING;

    /* delay to get over release of the PCI reset through bridge */
    sysUsDelay (3000);

    /* Initialize the array of pci irq lists */
    for (idx = 0; idx < NUMBER_OF_PIC_IRQS; idx++)
        dllInit (&pciDeviceIntList[idx]);

    }

/*************************************************************************
*
* pciDeviceIntConnect - connect a PCI interrupt to handler and vector
*
* This function connects an interrupt handler to the PCI interrupt and 
* PCI interrupt vector. It disables the interrupt vector via the PCI 
* interrupt mask register prior to initialization in order to get it to 
* a known state.
*
* RETURNS : OK or ERROR
*
* ERRNO
*
*/


STATUS pciDeviceIntConnect
    (
    int vector,
    VOIDFUNCPTR *handler,
    int arg
    )
    {
    PCI_INT_RTN *pRtn;
    int oldLevel;

    /*
     * Check whether the vector address is valid. In this case,
     * both IRQ and vector address are the same.
     */
    if (vector>=NUMBER_OF_PIC_IRQS)
        return (ERROR);

    /* Allocate memory for the structure */ 
    pRtn = (PCI_INT_RTN *)malloc (sizeof (PCI_INT_RTN));

    /* Return error if memory allocation fails */

    if (pRtn == NULL)
	return (ERROR);

    /* Disable the interrupts before hooking ISR */

    pciDeviceIntDisable(vector);


    /* 
     * If pciDeviceIntList[irq] is null, then bind the handler 
     * using _func_intConnectRtn
     */

    if (firstTime == TRUE)
        {
        if ((_func_intConnectRtn == NULL) ||
            ((STATUS)_func_intConnectRtn ((VOIDFUNCPTR *) INUM_IRQ6,
                                          (VOIDFUNCPTR) pciDeviceIntHandler, 0)
                                           == ERROR))
	    {
            printf("Error in connecting the pciDeviceIntHandler routine\n");
            free(pRtn);
            return ERROR;
	    }
        firstTime = FALSE;
        }

    /* Update the structure members */

    pRtn->routine   = (VOIDFUNCPTR) handler;
    pRtn->parameter = arg;

    /* 
     * Add the element to the list. This is required 
     * as many devices may share
     * the same IRQ
     */
    oldLevel = intLock ();			/* LOCK INTERRUPT */
    dllAdd (&pciDeviceIntList[vector], &pRtn->node);
    intUnlock (oldLevel);			/* UNLOCK INTERRUPT */


    /* Enable back the interrupts */
    pciDeviceIntEnable(vector);


#ifdef PCI_DEBUG
    logMsg("PCI Interrupt connected %d\n",vector,0, 0, 0, 0, 0);
#endif

    return OK;
    }

#if defined  (INCLUDE_FEI_END) || defined (INCLUDE_AR521X_END)
/*************************************************************************
*
* pciDeviceIntDisconnect - disconnect a PCI interrupt from handler and vector
*
* This function disconnects the device interrupt handler from the PCI 
* interrupt and disables the relevant PCI interrupt.
*
* RETURNS : OK or ERROR
*
* ERRNO
*
*/

STATUS pciDeviceIntDisconnect
    (
     int vector,
     VOIDFUNCPTR *handler
    )
    {
    PCI_INT_RTN *pRtn;
    int oldLevel;

    /* 
     * If this function is called without calling pciDeviceIntConnect or
     * the irq number is incorrect, return an ERROR.
     */ 
    if (firstTime ||
        (vector>=NUMBER_OF_PIC_IRQS))
        return ERROR;

    /* 
     * This loop searches for the structure which is specific to this
     * function pointer. If the function pointer matches, then the 
     * node element of the list is removed.
     */ 
    for (pRtn = (PCI_INT_RTN *)DLL_FIRST (&pciDeviceIntList[vector]); 
         pRtn != NULL;
	 pRtn = (PCI_INT_RTN *)DLL_NEXT (&pRtn->node))
	{
	if (pRtn->routine == (VOIDFUNCPTR) handler)
	    {
	    oldLevel = intLock ();			/* LOCK INTERRUPT */
	    dllRemove (&pciDeviceIntList[vector], &pRtn->node);
	    intUnlock (oldLevel);			/* UNLOCK INTERRUPT */

	    free ((char *)pRtn);

            if (DLL_EMPTY(&pciDeviceIntList[vector]))
                (void)pciDeviceIntDisable(vector);

#ifdef PCI_DEBUG
            logMsg("PCI Interrupt disconnected %d\n",vector,0,0,0,0,0);
#endif

	    return (OK);
	    }
	}
    return (ERROR);
    }
#endif


STATUS pciDeviceIntDisconnect2
    (
     int vector,
     VOIDFUNCPTR *handler,
     int arg
    )
    {
    PCI_INT_RTN *pRtn;
    int oldLevel;

    /*
     * If this function is called without calling pciDeviceIntConnect or
     * the irq number is incorrect, return an ERROR.
     */
    if (firstTime ||
        (vector>=NUMBER_OF_PIC_IRQS))
        return ERROR;

    /*
     * This loop searches for the structure which is specific to this
     * function pointer. If the function pointer matches, then the
     * node element of the list is removed.
     */ 
    for (pRtn = (PCI_INT_RTN *)DLL_FIRST (&pciDeviceIntList[vector]); 
         pRtn != NULL;
	 pRtn = (PCI_INT_RTN *)DLL_NEXT (&pRtn->node))
	{
	if (pRtn->routine == (VOIDFUNCPTR) handler)
	    {
	    oldLevel = intLock ();			/* LOCK INTERRUPT */
	    dllRemove (&pciDeviceIntList[vector], &pRtn->node);
	    intUnlock (oldLevel);			/* UNLOCK INTERRUPT */

	    free ((char *)pRtn);

            if (DLL_EMPTY(&pciDeviceIntList[vector]))
                (void)pciDeviceIntDisable(vector);

#ifdef PCI_DEBUG
            logMsg("PCI Interrupt disconnected %d\n",vector,0,0,0,0,0);
#endif

	    return (OK);
	    }
	}

    return (ERROR);


    }

/*************************************************************************
*
* pciDeviceIntEnable - enable an interrupt for a given vector
*
* This function enables the specific PCI interrupt related to the vector.
*
* RETURNS : OK or ERROR
*
* ERRNO
*/

STATUS pciDeviceIntEnable
    (
     int vector
    )
    {
    if (vector>=NUMBER_OF_PIC_IRQS)
        return(ERROR);

    (void)m8260IntDisable(INUM_IRQ6);
    sysIntEnablePIC(vector);
    (void)m8260IntEnable(INUM_IRQ6);

#ifdef PCI_DEBUG
    logMsg("PCI Interrupt Enabled %x\n",(int)vector,0,0,0,0,0);
#endif /* PCI_DEBUG */

      return (OK);
    }

/*************************************************************************
*
* pciDeviceIntDisable - Disables an interrupt for a given vector
*
* This function disables the specific PCI interrupt related to the vector.
*
* RETURNS : OK or ERROR
*
* ERRNO
*
*/

STATUS pciDeviceIntDisable
    (
     int vector
    )
    {
    if (vector>=NUMBER_OF_PIC_IRQS)
        return(ERROR);

    (void)m8260IntDisable(INUM_IRQ6);
    sysIntDisablePIC(vector);
    (void)m8260IntEnable(INUM_IRQ6);

#ifdef PCI_DEBUG
    logMsg("PCI Interrupt disabled %x\n",(int)vector,0,0,0,0,0);
#endif /* PCI_DEBUG */

      return (OK);
    }

/*************************************************************************
*
* pciDeviceIntHandler -  IRQ6 interrupt dispatch handler
*
* This is called from the 826x interrupt controller and is linked to IRQ6.
*
* It finds which vectors have the corresponding interrupt set and
* calls the relevant interrupt handler.
*
* RETURNS : N/A
*
* ERRNO
*/

LOCAL void pciDeviceIntHandler
    (
     void
    )
    {
    volatile int iv;
    PCI_INT_RTN *pRtn;
#ifdef PCI_DEBUG
    volatile int status;

    status = *(VUINT32*)PQII_SIPNR_H;
    logMsg("PCI interrupt received %x\n",(int)status,0,0,0,0,0);
#endif /* PCI_DEBUG */

    /* Retrieve the level of the interrupt */

    iv = sysIntLevel();

#ifdef PCI_DEBUG
    logMsg("pciDeviceIntHandler OUT :PIC IRQ received %x\n",iv,0,0,0,0,0);
#endif /* PCI_DEBUG */

    /* Call all the ISR functions registered for this level of interrupt */

    for (pRtn = (PCI_INT_RTN *)DLL_FIRST (&pciDeviceIntList[iv]); pRtn != NULL;
       	 pRtn = (PCI_INT_RTN *)DLL_NEXT (&pRtn->node))
       (* pRtn->routine) (pRtn->parameter);
          
    /* PIC EOI */
    sysIntEoi();
        
    /* Clear the IRQ6 interrupt */
    *(VUINT32*)PQII_SIPNR_H |=IRQ6_SIPNR_H_MASK; 

    }




