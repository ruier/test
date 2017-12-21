/* mot82xxPci.c - Motorola ads 82xx PCI Bridge functions */

/*
 * Copyright (c) 2002-2003, 2005, 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
#include "copyright_wrs.h"

/*
modification history
--------------------
01f,10jun09,wqi  Correct MPC827x's interrupt numbers. (WIND00086544)
01e,08sep05,dtr  Remove write to SIUMCR as now done in sysHwInit.
01d,25mar05,mdo  Documentation fixes for apigen
01c,01oct03,dee  changed structure element names for all dma buffer
                 descriptors
01b,08jan03,dtr  Added support for PCI DMA and Error Handling.
                 Implemented workaround for PCI Bridge read errata.
01a,04jul02,dtr  File created.
*/

/*
DESCRIPTION
This file is the PCI Host Bridge setup File.

INCLUDE FILES:
*/

#include "vxWorks.h"
#include "config.h"
#include "sysLib.h"

#include "drv/pci/pciConfigLib.h" 	
#include "drv/pci/pciIntLib.h" 	
#include "drv/pci/pciAutoConfigLib.h"	
#include "sysBusPci.h"
#include "drv/mem/m8260Memc.h"
#include "drv/intrCtl/m8260IntrCtl.h"
#include "m827xInNu.h"
#include "drv/mem/m8260Siu.h"
#include "mot82xxPci.h"


#ifdef PCI_BRIDGE_READ_ERRATA_WORKAROUND

/* Anything above 0x3ffffff can be handled by IDMA */ 
#define PCI_ACCESS_MASK 0xfc000000 

/* Global function called in sysLib.c */ 
void pciBridgeIdmaInit();
IMPORT  USHORT  sysWord(int);
IMPORT  UCHAR  sysByte(int);
IMPORT  ULONG  sysLong(int);
#endif /* PCI_BRIDGE_READ_ERRATA_WORKAROUND */

#ifdef INCLUDE_PCI_DMA
STATUS pciDmaTransfer(char*,char*,UINT32,UINT32 );
LOCAL void pciDmaHandler();
#endif

/* These three arrays are for convenience and speed */

/* Each interrupt line is wire OR'd to another on another slot 
 * Hence for first 4 bits - 8 is slot 0 INT A
 * links     second       - 4 is slot 1 INT B
 * links     third        - 2 is slot 2 INT C
 * All are the same interrupt effectively linked but they all
 * need to be cleared/masked in the interrupt register. 
 * Refer to the ADS8266 PCI Board Users Manual.
 */
 
LOCAL UINT32  pciSlotIntDisableMask[MAX_NUM_VECTORS] = 
        {0x84200000,     /* Slot 0 */
	 0x18400000,     /* Slot 1 */
	 0x21800000};    /* Slot 2 */

LOCAL UINT32  pciSlotIntEnableMask[MAX_NUM_VECTORS] = 
        {0x7bd00000,
	 0xe7b00000,
	 0xde700000};

LOCAL UINT32  pciSlotIntClearMask[MAX_NUM_VECTORS] = 
        {0x7bdfffff,
	 0xe7bfffff,
	 0xde7fffff};

LOCAL INTR_HANDLER  pciDeviceIntTable[MAX_NUM_VECTORS] = { {NULL,0},
							   {NULL,0},
							   {NULL,0} };

BOOL       firstTime = TRUE;

LOCAL void pciDeviceIntHandler(void);
STATUS pciDeviceIntConnect(int vector,VOIDFUNCPTR *handler,int arg);
STATUS pciDeviceIntDisconnect(int vector,VOIDFUNCPTR *handler);
STATUS pciDeviceIntEnable(int);
STATUS pciDeviceIntDisable(int);
LOCAL void pciMultiInterruptHandler();
LOCAL void pciErrorHandler();
LOCAL UINT32  pciIntEnabled = 0;

VOIDFUNCPTR _func_pciDmaUserHandler[] = {NULL,NULL,NULL,NULL};


typedef struct {
    UINT32 nmi;        /* 60x error original initiated for PCI */
    UINT32 ira;        /* Illegal register access */ 
    UINT32 i2o_ipqo;   /* I2O Error queue overflow inbound */ 
    UINT32 i2o_ofqo;   /* I2O Error queue overflow outbound*/
    UINT32 perr_wr;    /* PCI Error Parity Error on write */
    UINT32 perr_rd;    /* PCI Error Parity Error on read */
    UINT32 serr;       /* PCI Error System error */
    UINT32 tar_abt;    /* PCI Error configuration access device isn't available */
    UINT32 no_rsp;     /* PCI Error no response */
    UINT32 datapar_rd; /* PCI Error data parity error on read */
    UINT32 datapar_wr; /* PCI Error data parity error on write */
    UINT32 addrpar; /* PCI Error address parity error */
} PCI_ERROR_COUNT;

LOCAL PCI_ERROR_COUNT  pciErrorCount; 

IMPORT  UINT32  vxImmrGet  	(void);
IMPORT  USHORT  sysPciInWord(int);
LOCAL   UINT32  immrVal = INTERNAL_MEM_MAP_ADDR;

#ifdef PCI_BRIDGE_READ_ERRATA_WORKAROUND
LOCAL BD_DMA *idmaBd;
LOCAL IDMA_PARAM *pIdmaParam;
#endif /* PCI_BRIDGE_READ_ERRATA_WORKAROUND */

/*************************************************************************
* 
* mot82xxBridgeInit -  initialize the PCI Bridge
*
* This function performs all the initialisation required for the 
* Bridge/Interrupts/PCI Bus to function. It does some low level processor 
* initialisation which might normally be done in romInit as it is optional 
* to do use this and shows what the core changes required to bring up the 
* bridge.
* 
* RETURNS: N/A
*
* ERRNO
*/
void mot82xxBridgeInit(void)
    {
      STATUS result;
      immrVal = (UINT32)vxImmrGet();

      /* Make PCI lowest priority */
      /* Each 4 bits is a device bus request  and the MS 4bits 
         is highest priority */
      /* Bus               4bit value 
	 ---               ----------
         CPM high          0b0000
         CPM middle        0b0001
	     CPM low           0b0010
         PCI request       0b0011
         Reserved          0b0100
         Reserved          0b0101
         Internal Core     0b0110
         External Master 1 0b0111
         External Master 2 0b1000
         External Master 3 0b1001
         The rest are reserved */

      *M8260_PPC_ALRH (INTERNAL_MEM_MAP_ADDR) = 0x61207893;

      /* Park bus on core while modifying PCI Bus accesses */
      *M8260_PPC_ACR (INTERNAL_MEM_MAP_ADDR) = PPC_ACR_BUS_PARK_CORE;

      /* Standard GPCM */
      *(VUINT32*) M8260_OR3(INTERNAL_MEM_MAP_ADDR) = PCI_INT_MEM_OR8_SETTING;
      *(VUINT32*) M8260_BR3(INTERNAL_MEM_MAP_ADDR) = PCI_INT_MEM_BR8_SETTING;

      /* Set outbound translation window addresses */
   
      sysPciOutLong((UINT32*)(PCI_OUTBOUND_BASE_ADRS_REG0(immrVal)),
		    (CPU_PCI_MEM_ADRS>>12) & 0xfffff);

      sysPciOutLong((UINT32*)(PCI_OUTBOUND_TRANS_ADRS_REG0(immrVal)),
		    (PCI_MEM_ADRS>>12) & 0xfffff);

      sysPciOutLong((UINT32*)(PCI_OUTBOUND_BASE_ADRS_REG1(immrVal)),
		    (CPU_PCI_MEMIO_ADRS>>12) & 0xfffff);

      sysPciOutLong((UINT32*)(PCI_OUTBOUND_TRANS_ADRS_REG1(immrVal)),
		    (PCI_MEMIO_ADRS>>12) & 0xfffff);

      sysPciOutLong((UINT32*)(PCI_OUTBOUND_BASE_ADRS_REG2(immrVal)),
		    (CPU_PCI_IO_ADRS>>12) & 0xfffff);

      sysPciOutLong((UINT32*)(PCI_OUTBOUND_TRANS_ADRS_REG2(immrVal)),
		    (PCI_IO_ADRS>>12) & 0xfffff);

      *(VUINT32*) PCIMSK_REG0 (INTERNAL_MEM_MAP_ADDR) = PCIMSK0_SETTING;

      /* set mask for 128KBytes */

      *(VUINT32*) PCIMSK_REG1 (INTERNAL_MEM_MAP_ADDR) = PCIMSK1_SETTING;

      /* set start addresses and enable window*/

      *(VUINT32*) PCIBR_REG0 (INTERNAL_MEM_MAP_ADDR) = PCIBR0_SETTING;
      *(VUINT32*) PCIBR_REG1 (INTERNAL_MEM_MAP_ADDR) = PCIBR1_SETTING;

      /* Switch on the outbound translation windows */
      sysPciOutLong((UINT32*)(PCI_OUTBOUND_COMP_MASK_REG0(immrVal)),
		    PCI_WINDOW_ENABLE_BIT | PCI_MEM_SIZE_MASK);


      sysPciOutLong((UINT32*)(PCI_OUTBOUND_COMP_MASK_REG1(immrVal)),
		    PCI_WINDOW_ENABLE_BIT | PCI_MEMIO_SIZE_MASK);


      /* Set the PCI reset line high to switch on PCI bus */

      sysPciOutLong((UINT32*)(immrVal| PCI_GCR_REG) , 
		    0);

 
      sysPciOutLong((UINT32*)(immrVal| PCI_GCR_REG) , 
		    EXTERNAL_PCI_BUS_ENABLE); 

      sysMsDelay(1);
     
      /* the bridge registers need to be accessed as little endian */
      /* Set up inbound address translation registers */
      /* Translation address is the locally mapped address */
      sysPciOutLong((UINT32*)(immrVal| PCI_INBOUND_TRANS_ADRS_REG0),
		    (PCI_LOCAL_MEM_BUS>>12) & 0xfffff);

      /* Base Address is the PCI address */
      sysPciOutLong((UINT32*)(immrVal| PCI_INBOUND_BASE_ADRS_REG0),
		    (PCI_MSTR_MEM_BUS>>12) & 0xfffff);

      /* This initialises the size of the window and enables it */
      sysPciOutLong((UINT32*)(immrVal |PCI_INBOUND_COMP_MASK_REG0),
		    PCI_MSTR_MEM_SIZE_MASK | PCI_WINDOW_ENABLE_BIT | PCI_SNOOP_ENABLE | PCI_PREFETCHABLE);


      /* See above for description - puts PCI request as highest priority */
      *M8260_PPC_ALRH (INTERNAL_MEM_MAP_ADDR) = 0x30126745;

      /* Park the bus on the PCI */
      *M8260_PPC_ACR (INTERNAL_MEM_MAP_ADDR) = PPC_ACR_BUS_PARK_PCI;

      /* Setup The PIMMR PCI Base address */

      result = sysPciConfigWrite(0,0,0,
				 PIMMR_BASE_ADRS_REGISTER_OFFSET,
				 PIMMR_BASE_ADRS_REGISTER_WIDTH,
				 PCI_BRIDGE_PIMMR_BASE_ADRS);
			       
      result = sysPciConfigWrite(0,0,0,
				 CLASS_OFFSET,
				 CLASS_WIDTH,
				 BRIDGE_CLASS_TYPE);

      /* configure the bridge as bus master */

      result = sysPciConfigWrite(0,0,0,
				 COMMAND_REGISTER_OFFSET,
				 COMMAND_REGISTER_WIDTH,
				 (BUS_MASTER_ENABLE_BIT|MEMORY_SPACE_ACCESS_ENABLE_BIT));

    }

/*************************************************************************
* 
* pciDeviceIntConnect - connect PCI interrupt to a handler
*
* This function connects a interrupt handler to the pci interrupt and 
* PCI interrupt vector. It disables the interrupt vector via the PCI 
* interrupt mask register prior to initialisation in order to get it to 
* a known state assuming the interrupt should be at this point.  
*
* RETURNS: OK or ERROR
*
* ERRNO
*/

STATUS pciDeviceIntConnect
    (
    int vector,
    VOIDFUNCPTR *handler,
    int arg
    )
    {
    if (vector>=MAX_NUM_VECTORS)
        return (ERROR);

      pciDeviceIntDisable(vector);

    if (firstTime==TRUE)
        {
        (void)_func_intConnectRtn((VOIDFUNCPTR *)INUM_IRQ2,
                                  (VOIDFUNCPTR)pciDeviceIntHandler,0);
        /* Initialise Interrupt Mask */
        firstTime=FALSE;
        }

      pciDeviceIntTable[vector].vec = (VOIDFUNCPTR)handler;
      pciDeviceIntTable[vector].arg = arg;
      (void)m8260IntEnable(INUM_IRQ2);

      return OK;
 
    }


/*************************************************************************
* 
* pciDeviceIntDisconnect - disconnect interrupt handler to PCI interrupt 
*
* This function disconnects the device interrupt handler from the PCI 
* interrupt and disables the relevant PCI interrupt.
*
* RETURNS: OK or ERROR
*
* ERRNO
*/

STATUS pciDeviceIntDisconnect
    (
    int vector,
    VOIDFUNCPTR *handler
    )
    {
      if (vector>=MAX_NUM_VECTORS)
          return (ERROR);

      (void)pciDeviceIntDisable(vector);

      pciDeviceIntTable[vector].vec = NULL;
      pciDeviceIntTable[vector].arg = 0;

#ifdef PCI_DEBUG
      logMsg("PCI Interrupt disconnected %d\n",vector,0,0,0,0,0);
#endif

      firstTime=TRUE;

      return OK;
 
    }


/*************************************************************************
* 
* pciDeviceIntEnable - enable PCI interrupt for a given vector
*
* This function enables the specific PCI interrupt related to the vector. 
*
* RETURNS: OK or ERROR
*
* ERRNO
*/
 
STATUS pciDeviceIntEnable
    (
    int vector
    )
    {

    if (vector>=MAX_NUM_VECTORS)
	    return (ERROR);

#ifdef PCI_DEBUG
    logMsg("PCI Interrupt Enable %x\n",(int)vector,0,0,0,0,0);
#endif /* PCI_DEBUG */

    (void)m8260IntDisable(INUM_IRQ2);
    *(UINT32*)PCI_INTERRUPT_MASK_REG &=pciSlotIntEnableMask[vector];
    (void)m8260IntEnable(INUM_IRQ2);

    return (OK);
    }

/*************************************************************************
* 
* pciDeviceIntDisable - disable PCI interrupt for a given vector 
*
* This function disables the specific PCI interrupt related to the vector. 
*
* RETURNS: OK or ERROR
*
* ERRNO
*/

STATUS pciDeviceIntDisable
    (
    int vector
    )
    {

    if (vector>=MAX_NUM_VECTORS)
	    return (ERROR);

#ifdef PCI_DEBUG
    logMsg("PCI Interrupt disable %x\n",(int)vector,0,0,0,0,0);
#endif /* PCI_DEBUG */

    (void)m8260IntDisable(INUM_IRQ2);

    *(UINT32*)PCI_INTERRUPT_MASK_REG |= pciSlotIntDisableMask[vector];

    (void)m8260IntEnable(INUM_IRQ2);

    return (OK);

    }

/*************************************************************************
* 
* pciDeviceIntHandler - PCI interrupt dispatch routine
*
* This is called from the 8260 interrupt controller and is linked to IRQ6. 
* It finds which vectors have the corresponding interrupt set and 
* calls the relevant interrupt handler. The priority based on 
* slot and the slot<->vector are the effectively the same. The vector
* is based upon device number which for ADS board is :
*                  0x16 -> slot 0 -> vector 0
*                  0x17 -> slot 1 -> vector 1
*                  0x18 -> slot 2 -> vector 2
* For a custom board this may need to be modified.
*
* RETURNS: N/A
*
* ERRNO 
*/

LOCAL void pciDeviceIntHandler(void) 
    {
    volatile int vector;
    volatile UINT32 status;

    status = *(VUINT32*)PCI_INTERRUPT_STATUS_REG;

    for (vector=0;vector<MAX_NUM_VECTORS;vector++)
        {
        /* Check Interrupt routine available and interrupt vector is set */
        if ( (pciDeviceIntTable[vector].vec!=NULL)
             &&(pciDeviceIntTable[vector].arg!=0) &&
             ((status & pciSlotIntDisableMask[vector])!=0))
            {
            /* Call Interrupt Handler for specific vector */
            pciDeviceIntTable[vector].vec(pciDeviceIntTable[vector].arg);
            /* Clear the specific interrupt */
            *(VUINT32*)PCI_INTERRUPT_STATUS_REG &=pciSlotIntClearMask[vector];
            }
        }

#ifdef PCI_DEBUG
    logMsg("PCI interrupt received %x\n",(int)status,0,0,0,0,0);
#endif /* PCI_DEBUG */

    }

/****************************************************************
*   
* pciErrorHandlingInit - initialize PCI error interrupts
*
* This function sets up the error control and mask registers. 
* It also connects the INUM_PCI to the interrupt handler and 
* activates the interrupt.
*
* RETURNS: N/A
*
* ERRNO
*/

void pciErrorHandlingInit(void)
    {
    volatile int loop;

    /* Initialise ECR to include particular PCI Errors */
    /* Not required as most useful errors already enabled on reset*/

    /* Connect handler to INUM_PCI*/
    (void)_func_intConnectRtn( (VOIDFUNCPTR *)INUM_PCI,
                               (VOIDFUNCPTR)pciMultiInterruptHandler,
                                0);
  
    for (loop=0;loop<sizeof(pciErrorCount)/sizeof(UINT32);loop++)
        {
        *(UINT32*)((UINT32*)&pciErrorCount + loop) = 0;
        }

    /* Clear the Error Status register */
    sysPciOutLong((UINT32*)PCI_ESR(INTERNAL_MEM_MAP_ADDR),0x0);

    /* Activate/Initialise Mask/Control Registers */
    sysPciOutLong((UINT32*)PCI_EMR(INTERNAL_MEM_MAP_ADDR),0xeff);
    sysPciOutLong((UINT32*)PCI_ECR(INTERNAL_MEM_MAP_ADDR),0x0);

    /* Allow the interrupt handler to process the interrupt */
    pciIntEnabled|=PCI_ERROR_INT_ENABLED;

    /* Enable Interrupts */
    m8260IntEnable(INUM_PCI);

    #ifdef PCI_DEBUG
    logMsg("PCI Error Handling Initialised.\n",0,0,0,0,0,0);
    #endif /* PCI_DEBUG */

    }
#ifdef INCLUDE_PCI_DMA
/****************************************************************
*   
* pciDmaInit - initialize DMA interrupts
*
* This function sets up the error control and mask registers. 
* It also connects the INUM_PCI to the interrupt handler and 
* activates the interrupt.
*
* RETURNS: N/A
*
* ERRNO
*/

void pciDmaInit(void)
    {

    (void)_func_intConnectRtn( (VOIDFUNCPTR *)INUM_PCI,
                               (VOIDFUNCPTR)pciMultiInterruptHandler,
                                0);

    pciIntEnabled|=DMA_INT_ENABLED;

    /* Enable Interrupts */
    m8260IntEnable(INUM_PCI);

    }

/****************************************************************
*   
* pciDmaTransfer - configure a DMA transfer
*
* This function sets up a direct DMA transfer using the PCI 
* Bridge. 
*
* RETURNS: OK or ERROR
*
* ERRNO
*/
STATUS pciDmaTransfer 
    ( 
    char* source,
    char* destination,
    UINT32 bytes,
    UINT32 dmaRegNum 
    )
    {
    volatile UINT32 statusReg;


    if (((UINT32)source & PCI_DMA_SOURCE_MASK)!=(UINT32)source)
        {
        printf("Source address not 32 byte aligned\n");
        return(ERROR);
        }

    if (((UINT32)destination & PCI_DMA_DEST_MASK)!=(UINT32)destination)
        {
        printf("Destination address not 32 byte aligned\n");
        return(ERROR);
        }

    if(((UINT32)bytes & PCI_DMA_MAX_BYTE_COUNT_MASK)!=bytes)
        {
        printf("Byte Count too large\n"); 
        return(ERROR);
        }

    taskLock();

    statusReg = PCI_IN_LONG ((int)PCI_DMASR(immrVal,dmaRegNum));
    while ((statusReg & DMASR_CB)==DMASR_CB)
        statusReg = PCI_IN_LONG ((int)PCI_DMASR(immrVal,dmaRegNum));

    sysPciOutLong ( PCI_DMASAR (immrVal,dmaRegNum),( (UINT32)source & PCI_DMA_SOURCE_MASK ) );
    sysPciOutLong ( PCI_DMADAR (immrVal,dmaRegNum),( (UINT32)destination & PCI_DMA_DEST_MASK ) );
    sysPciOutLong ( PCI_DMABCR (immrVal,dmaRegNum),( bytes & PCI_DMA_MAX_BYTE_COUNT_MASK ) );

    statusReg = PCI_IN_LONG ((int)PCI_DMAMR(immrVal,dmaRegNum));
    sysPciOutLong ( PCI_DMAMR (immrVal,dmaRegNum),((statusReg & DMAMR_CS_MASK)|DMAMR_CTM_DIRECT));

    sysPciOutLong ( PCI_DMAMR (immrVal,dmaRegNum),statusReg | DMAMR_EOTIE | DMAMR_CS | DMAMR_CTM_DIRECT);

    taskUnlock();

    return(OK);
    
    }

/****************************************************************************
*   
* pciDmaHandler - DMA interrupt handler
*
* This function is called from the pciMultiInterrupt handler and handles 
* the interrupts for the each DMA channel 
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void pciDmaHandler(void)
    {
    volatile UINT32 loop;
    
    for (loop=0;loop<NO_OF_DMA_CHANNELS;loop++)
        {
        /* Check DMA channel status register */
        if ( ( PCI_IN_LONG( (int)PCI_DMASR(immrVal,loop) ) & 0x83) !=0 )
            /* Call DMA Handler */
            {
#           ifdef PCI_DMA_DEBUG
            logMsg("PCI DMA %x\n",loop,0,0,0,0,0);
#           endif /* PCI_DMA_DEBUG */
            if (_func_pciDmaUserHandler[loop]!=NULL)
                _func_pciDmaUserHandler[loop]();
            sysPciOutLong(PCI_DMASR(immrVal,loop),0x00000083);
            }
       }

    }

#endif /* INCLUDE_PCI_DMA */
/****************************************************************************
*   
* pciMultiInterruptHandler -  m8260IntrCtl interrupt dispatch
*
* This function is called from the m8260IntrCtl and it calls the specific 
* interrupt handler for interrupt. 
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void pciMultiInterruptHandler(void)
    {
    /* Check PCI Error/DMA/Message FIFO interrupt */  
    /* If to be handled go to handler, otherwise clear interrupt */
    if ( (PCI_ESR_INT_SET != 0) && ( (pciIntEnabled | PCI_ERROR_INT_ENABLED ) != 0) )
        /* Call PCI Error Handler */
        pciErrorHandler();

#ifdef INCLUDE_PCI_DMA
    if ( ( pciIntEnabled | DMA_INT_ENABLED ) != 0 )
        pciDmaHandler();
#endif /* INCLUDE_PCI_DMA */

    /* TBD PCI MESSAGE HANDLING */    
    /*  if ((MESSAGE_SR_INT_SET!=0) && (pciIntEnabled|MESSAGE_INT_ENABLED!=0)) */
    /* Call Message FIFO Handler */ 
    /*      pciMessageHandler();*/

    }

/****************************************************************************
*   
* pciErrorHandler - handle PCI error interrupts
*
* This function is called from pciMultiInterrupt handler and handles the 
* interrupt for PCI Errors  
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void pciErrorHandler(void)
    {
    volatile int esrRegVal;

    /* Check which Errors are set */
    esrRegVal = PCI_IN_LONG( (int)PCI_ESR(INTERNAL_MEM_MAP_ADDR) );

    if ( (esrRegVal & PCIERR_MSK_NMI)!=0)
        {
#       ifdef PCI_MULTI_DEBUG
        logMsg("PCI Error NMI\n",0,0,0,0,0,0);
#       endif /* PCI_MULTI_DEBUG */
        pciErrorCount.nmi++;
        }
 
    if ( (esrRegVal & PCIERR_MSK_IRA) != 0)
        {
#       ifdef PCI_MULTI_DEBUG
        logMsg("PCI Error IRA\n",0,0,0,0,0,0);
#       endif /* PCI_MULTI_DEBUG */
        pciErrorCount.ira++;
        }

    if ( (esrRegVal & PCIERR_MSK_I2O_IPQO) != 0)
        {
#       ifdef PCI_MULTI_DEBUG
        logMsg("PCI Error I2O_IPQO\n",0,0,0,0,0,0);
#       endif /* PCI_MULTI_DEBUG */
        pciErrorCount.i2o_ipqo++;
        }

    if ( (esrRegVal & PCIERR_MSK_I2O_OFQO ) != 0)
       {
#      ifdef PCI_MULTI_DEBUG
       logMsg("PCI Error I2O_OFQO\n",0,0,0,0,0,0);
#      endif /* PCI_MULTI_DEBUG */
       pciErrorCount.i2o_ofqo++;
       }

    if ( ( esrRegVal & PCIERR_MSK_PERR_WR ) != 0)
        {
#       ifdef PCI_MULTI_DEBUG
        logMsg("PCI Error Parity Error on write\n",0,0,0,0,0,0);
#       endif /* PCI_MULTI_DEBUG */
        pciErrorCount.perr_wr++;
        }

    if ( ( esrRegVal & PCIERR_MSK_PERR_RD )!=0)
        {
#       ifdef PCI_MULTI_DEBUG
        logMsg("PCI Error Parity Error on read\n",0,0,0,0,0,0);
#       endif /* PCI_MULTI_DEBUG */
        pciErrorCount.perr_rd++;
        }

    if ( ( esrRegVal & PCIERR_MSK_SERR ) != 0)
        {
#       ifdef PCI_MULTI_DEBUG
        logMsg("PCI Error SERR\n",0,0,0,0,0,0);
#       endif /* PCI_MULTI_DEBUG */
        pciErrorCount.serr++;
        }

    if ( ( esrRegVal & PCIERR_MSK_TAR_ABT ) != 0 )
        {
#       ifdef PCI_MULTI_DEBUG
        logMsg("PCI Error\n",0,0,0,0,0,0);
#       endif /* PCI_MULTI_DEBUG */
        pciErrorCount.tar_abt++;
        }

    if (( esrRegVal&PCIERR_MSK_NO_RSP)!=0)
        { 
#       ifdef PCI_MULTI_DEBUG
        logMsg("PCI Error no response\n",0,0,0,0,0,0);
#       endif /* PCI_MULTI_DEBUG */
        pciErrorCount.no_rsp++;
        }

    if ( ( esrRegVal & PCIERR_MSK_DATA_PAR_RD ) != 0 )
        {
#       ifdef PCI_MULTI_DEBUG
        logMsg("PCI Error data parity error on read\n",0,0,0,0,0,0);
#       endif /* PCI_MULTI_DEBUG */
        pciErrorCount.datapar_rd++;
        }

    if ( ( esrRegVal & PCIERR_MSK_DATA_PAR_WR ) != 0 )
        {
#       ifdef PCI_MULTI_DEBUG
        logMsg("PCI Error data parity error on write\n",0,0,0,0,0,0);
#       endif /* PCI_MULTI_DEBUG */
        pciErrorCount.datapar_wr++;
        }

    if ( ( esrRegVal & PCIERR_MSK_ADDR_PAR ) != 0)
        {
#       ifdef PCI_MULTI_DEBUG
        logMsg("PCI Error address parity error\n",0,0,0,0,0,0);
#       endif /* PCI_MULTI_DEBUG */
        pciErrorCount.addrpar++;
        }
   
    sysPciOutLong ( (UINT32*)PCI_ESR(INTERNAL_MEM_MAP_ADDR), 0xfff );
    esrRegVal = PCI_IN_LONG ( (int)PCI_ESR( INTERNAL_MEM_MAP_ADDR ) );
  
}

/****************************************************************************
*   
* pciMessageHandler - message interrupt handler
*
* This function is called from the pciMultiInterrupt handler and handles 
* the interrupts for the messages.
*
* \NOMANUAL       It's not implemented at the momemt    
*
* RETURNS: N/A
*
* ERRNO
*/

void pciMessageHandler(void)
    {
    /* Clear reg for now TBD */ 
    }

#ifdef INCLUDE_SHOW_ROUTINES

/****************************************************************************
*   
* pciErrorShow - dump PCI Bridge errors
*
* This function can be called by the user and displays the number and type 
* of errors detected by the PCI bridge.
*
* RETURNS: N/A
*
* ERRNO
*/

void pciErrorShow (void)
    {

    printf("PCI Error Statistics\n");
    printf("--------------------\n");
    printf("Number of NMI Errors %d\n",pciErrorCount.nmi);
    printf("Number of IRA Errors %d\n",pciErrorCount.ira);
    printf("Number of I2O_IPQO Errors %d\n",pciErrorCount.i2o_ipqo);
    printf("Number of I2O_OFQO Errors %d\n",pciErrorCount.i2o_ofqo);
    printf("Number of Parity Write Errors %d\n",pciErrorCount.perr_wr);
    printf("Number of Parity Read  Errors %d\n",pciErrorCount.perr_rd);
    printf("Number of SERR Errors %d\n",pciErrorCount.serr);
    printf("Number of Target Abort Errors %d\n",pciErrorCount.tar_abt);
    printf("Number of No Response Errors %d\n",pciErrorCount.no_rsp);
    printf("Number of Data Parity Read Errors %d\n",pciErrorCount.datapar_rd);
    printf("Number of Data Parity Write Errors %d\n",pciErrorCount.datapar_wr);
    printf("Number of Address Parity Errors %d\n",pciErrorCount.addrpar);

    }
#endif /* INCLUDE_SHOW_ROUTINES */


#ifdef PCI_BRIDGE_READ_ERRATA_WORKAROUND

/**************************************************************************
*
* pciBridgeIdmaInit -  initialize IDMA2 PCI errata fix
*
* This is called to initialize IDMA2 for the PCI errata fix.
* It Initialises the IDMA. It can be called one time only. For any IDMA 
* transfer used in the code after this initialization only the source ptr
* has to be changed. The dest ptr in this case is conveniently positioned 
* in the DPRAM and not in the external memory in order to reduce the
* access latency.
*
* RETURNS: N/A
*
* ERRNO
*/

void pciBridgeIdmaInit(void)
    {
    char *pramBuf  = (char *)m82xxDpramAlignedMalloc(0x100,0x100);
    char *dpramBuf = (char *)m82xxDpramAlignedMalloc(0x40,0x40);
    char *bdBuf    = (char *)m82xxDpramAlignedMalloc(0x40,0x40);

    /*
     * init bds
     */

    idmaBd = (BD_DMA*)bdBuf;
    idmaBd->dstptr = (UINT32)dpramBuf; /* Destination for the idma is a one empty word location is DPRAM */

    /*
     * init idma
     */

    *(VUINT16*)IDMA2_BASE =  (UINT16)((UINT32)pramBuf & 0xffff); 
    pIdmaParam = (IDMA_PARAM *)pramBuf;
    pIdmaParam->ibase =   (UINT16)((UINT32)bdBuf & 0xffff);
    pIdmaParam->ibdptr =  (UINT16)((UINT32)bdBuf & 0xffff);
    pIdmaParam->dcm   = DCM_DUAL_ADDR | DCM_TRANS_BUF_128 | 
                        DCM_MEM_MEM | DCM_DINC | DCM_SINC;

    pIdmaParam->dpr_buf = (UINT16)((UINT32)dpramBuf & 0xffff);   /* DPR_BUF=64x2(DMA_WRAP)-32  */
    pIdmaParam->ss_max =  0x0060;   /*  Steady state value from peripheral to memory 96 bytes */
    pIdmaParam->sts =     0x0060;   /*Transfers from peripheral is 96 bytes*/
    pIdmaParam->dts =     0x0060;   /*Transfer to memory is 96 bytes (60x) bursts*/

    }

/****************************************************************************
*
* pciBridgeRegisterReadLong - read a long from PCI Bridge using IDMA 
*
* This routine reads 32-bit values from the PCI Bridge using IDMA.
*  
* RETURNS: 32-bit value at location  
*
* ERRNO
*/

UINT32 pciBridgeRegisterReadLong
    (
    int src
    ) 
    {
    volatile int key;
    UINT32 value;

    if ((src >= CPU_PCI_MEM_ADRS) && (src < CPU_PCI_IO_ADRS))
        {
        key = intLock();  /* Need to make sure only one access at a time */

        /* Setup buffer descriptor */ 
        idmaBd->srcptr = (UINT32)src; 
        idmaBd->length = 4; /* Need to set to prevent alignment issues */
        /* This needs to be set each time for the valid bit */ 
        idmaBd->cstatus = (IDMABD_INT_ENABLE | IDMABD_WRAP | IDMABD_LAST |
	   		      IDMABD_DBO_BIG_ENDIAN | IDMABD_SBO_BIG_ENDIAN |
			      IDMABD_VALID | IDMABD_DST_DONE |
			      IDMABD_SRC_LOCAL);	


        while (*((VUINT32*)CPCR_REG) & CPCR_FLG); /* Wait till CPA available */
        /* IDMA2 PAGE/CODE IDMA command start_idma */
        *((VUINT32*)CPCR_REG) = IDMA2_PAGE_SUBBLOCK|CPCR_FLG|CPCR_START_IDMA;

        while (*((VUINT32*)CPCR_REG) & CPCR_FLG); /* Wait till CPCR available */

        while (!((*(VUINT8*)IDSR2) & 0x1)); /* Wait for completion of IDMA */ 

        value = sysLong(idmaBd->dstptr); /* Byte Swap  */

        /*Clear the IDSR2 status bit so each access waits for actual completion*/ 
        *((VUINT8*)IDSR2) = 0x1;  

        intUnlock(key);  /* Allow access to idma */
        }
    else
        { 
        value = sysLong((int)src); /* Byte Swap  */
        }

    return (value);
    }
                  

/****************************************************************************
*
* pciBridgeRegisterReadWord - read a word from PCI Bridge using IDMA 
*
* This routine reads 16-bit values from the PCI Bridge using IDMA.
*  
* RETURNS: 16-bit value at location  
*
* ERRNO
*/

UINT16 pciBridgeRegisterReadWord
    (
    int src
    ) 
    {
    volatile int key;
    UINT16 value;

    if (((UINT32)src >= CPU_PCI_MEM_ADRS) && ((UINT32)src < CPU_PCI_IO_ADRS))
        {
        key = intLock();  /* Need to make sure only one access at a time */

        /* Setup buffer descriptor */ 
        idmaBd->srcptr = (UINT32)src; 
        idmaBd->length = 2; /* Need to set to prevent alignment issues  */
        /* This needs to be set each time for the valid bit */ 
        idmaBd->cstatus = (IDMABD_INT_ENABLE | IDMABD_WRAP | IDMABD_LAST |
	   		      IDMABD_DBO_BIG_ENDIAN | IDMABD_SBO_BIG_ENDIAN |
			      IDMABD_VALID | IDMABD_DST_DONE |
			      IDMABD_SRC_LOCAL);	

        while (*((VUINT32*)CPCR_REG) & CPCR_FLG); /* Wait till CPCR available */

        /* IDMA2 PAGE/CODE IDMA command start_idma */
        *((VUINT32*)CPCR_REG) = IDMA2_PAGE_SUBBLOCK|CPCR_FLG|CPCR_START_IDMA;

        while (*((VUINT32*)CPCR_REG) & CPCR_FLG); /* Wait till CPCR available */

        while (!((*(VUINT8*)IDSR2) & 0x1));  /* Wait for completion of IDMA */ 

        value = sysWord(idmaBd->dstptr); /* Byte Swap  */

        /*Clear the IDSR2 status bit so each access waits for actual completion*/ 
        *((VUINT8*)IDSR2) = 0x1;  

        intUnlock(key);  /* Allow access to idma */
        }
    else
        { 
        value = sysWord((int)src); /* Byte Swap  */
        }

    return (value);
}

/****************************************************************************
*
* pciBridgeRegisterReadByte - read a byte from PCI Bridge using IDMA 
*
* This routine reads 8-bit values from the PCI Bridge using IDMA.
*  
* RETURNS: 8-bit value at location  
*
* ERRNO
*/

UINT8 pciBridgeRegisterReadByte
    (
    int src
    ) 
    {
    volatile int key;
    UINT8 value;

    if ((src >= CPU_PCI_MEM_ADRS) && (src < CPU_PCI_IO_ADRS))
        {
        key = intLock();  /* Need to make sure only one access at a time */

        /* Setup buffer descriptor */ 
        idmaBd->srcptr = (UINT32)src; 
        idmaBd->length = 1; /* Need to set to prevent alignment issues */
        /* This needs to be set each time for the valid bit */ 
        idmaBd->cstatus = (IDMABD_INT_ENABLE | IDMABD_WRAP | IDMABD_LAST |
	   		      IDMABD_DBO_BIG_ENDIAN | IDMABD_SBO_BIG_ENDIAN |
			      IDMABD_VALID | IDMABD_DST_DONE |
			      IDMABD_SRC_LOCAL);	

        while (*((VUINT32*)CPCR_REG) & CPCR_FLG); /* Wait till CPCR available */
        /* IDMA2 PAGE/CODE IDMA command start_idma */
        *((VUINT32*)CPCR_REG) = IDMA2_PAGE_SUBBLOCK|CPCR_FLG|CPCR_START_IDMA;

        while (*((VUINT32*)CPCR_REG) & CPCR_FLG); /* Wait till CPCR available */

        while (!((*(VUINT8*)IDSR2) & 0x1)); /* Wait for completion of IDMA */ 

        value = sysByte(idmaBd->dstptr);

        /*Clear the IDSR2 status bit so each access waits for actual completion*/ 
        *((VUINT8*)IDSR2) = 0x1;  

        intUnlock(key);  /* Allow access to idma */
        }
    else
        { 
        value = sysByte((int)src); /* Byte Swap  */
        }

    return (value);
    }

#endif /* PCI_BRIDGE_READ_ERRATA_WORKAROUND */

