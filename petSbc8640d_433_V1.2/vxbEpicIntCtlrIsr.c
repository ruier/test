
/* vxbEpicIntCtlrIsr.c - Embedded Programmable Interrupt Controller ISR and IPI Emit portions */

/*
 * Copyright (c) 2008  Wind River Systems, Inc. All rights are reserved.
 *
 * The right to copy, distribute or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement.
 */

/*
modification history
--------------------
01b,10sep08,tcr  add SV instrumentation
01a,02jun08,h_k  split from vxbEpicIntCtlr.c.
*/

/*
DESCRIPTION
This module implements the ISR and IPI Emit portions of Embedded Programmable
Interrupt Controller (EPIC) driver for the MPC8548.

INCLUDE FILES:
*/

#include <vxWorks.h>
#include <vsbConfig.h>
#include <hwif/intCtlr/vxbEpicIntCtlr.h>
#include <hwif/vxbus/vxBus.h>
#include <../src/hwif/intCtlr/vxbIntCtlrLib.h>
#include <vxIpiLib.h>

#ifdef _WRS_CONFIG_SV_INSTRUMENTATION
#include <private/eventP.h>
#endif /* _WRS_CONFIG_SV_INSTRUMENTATION */

/* externs */

IMPORT UINT32 * vxbEpicIntAckReg;		/* int ack reg adrs */
IMPORT struct intCtlrHwConf * pVxbEpicIsrHandle;/* &EPIC_DRV_CTRL(isrHandle) */
IMPORT UINT32 * vxbEpicIpiDpatchReg0;		/* IPI dispatch reg0 adrs */
IMPORT USHORT vxbEpicSpuriousVector;		/* Superious Vector Number */
IMPORT int vxPpcIntMask;			/* MSR interrupts mask bits */
/* forward declarations */
#ifdef _WRS_CONFIG_SV_INSTRUMENTATION
LOCAL void vxbSysEpicIntHandlerSV (void);
#endif /* _WRS_CONFIG_SV_INSTRUMENTATION */

/*******************************************************************************
*
* vxbSysEpicIntHandler - handles the EPIC interrupts to the CPU
*
* This routine handles interrupts originating from the embedded interrupt
* controller on the MPC8540 PowerPC processor.
* This handler is entered from the 0x500 exception.
*
* This routine is entered with CPU external interrupts enables.
*
* Since the EPIC is the primary interrupt controller this driver
* first initiates an Epic acknowledge call and reads the vector
* put out by the EPIC. Subsequent vectors have to be obtained if
* an external interrupt controller is connected to one of the
* vxbEpic handlers.
*
* This routine then processes the interrupt by calling all the interrupt
* service routines chained to the vector.
*
* Finally, this routine re-arms the interrupt at the PIC by performing an
* PIC EOI for both the EPIC and the 8259s.
*
* RETURNS:  N/A
*
* ERRNO: N/A
*
* \INTERNAL
* Changes to this routine should be mirrored in vxbSysEpicIntHandlerSV(), which
* should be almost identical apart from the System Viewer instrumentation.
*
*/

void vxbSysEpicIntHandler (void)
    {

    UINT32	vxbEpicIntVec;
    int		intMask = vxPpcIntMask;
#ifdef _WRS_CONFIG_SV_INSTRUMENTATION
    if (ACTION_IS_SET)
        {
        vxbSysEpicIntHandlerSV ();
        return;
        }
#endif /* _WRS_CONFIG_SV_INSTRUMENTATION */
    
    /* loop till all interrupts are cleared */
  
    while (TRUE)
	{
	/* clear int, return the vec for highest IRQ */

	vxbEpicIntVec =
	    EPIC_REG_NO_OFFSET_READ(vxbEpicIntAckReg) & 0xffff;

	if (vxbEpicIntVec == vxbEpicSpuriousVector)
	    break;
#if 0
	if( (vxbEpicIntVec>0x47)&&(vxbEpicIntVec < 253))
	{	
		logMsg("vxbEpicIntVec = 0x%x,MsirNO = %d",vxbEpicIntVec,MsirNO,3,4,5,6);
/*		printstr("IRQ0 coming ,vxbEpicIntVec=");
		printnum(vxbEpicIntVec);
		*(UINT32*)0x50200038=0xffffffff;
*/	}
#endif
#if 0
	//if( (vxbEpicIntVec==0)||(vxbEpicIntVec == 2))
	if (vxbEpicIntVec<10)
	{	
		logMsg("vxbEpicIntVec = 0x%x,MsirNO = %d",vxbEpicIntVec,MsirNO,3,4,5,6);
/*		printstr("IRQ0 coming ,vxbEpicIntVec=");
		printnum(vxbEpicIntVec);
		*(UINT32*)0x50200038=0xffffffff;
*/	}
#endif
	
	VXB_EPICINTCTLR_ENABLE_INTS (intMask);

        VXB_INTCTLR_ISR_CALL(pVxbEpicIsrHandle, vxbEpicIntVec);

	VXB_EPICINTCTLR_DISABLE_INTS (intMask);

	/*
	 * Disable External Interrupts
	 * External Interrupts will be re-enabled in the kernel's wrapper
	 * of this Interrupt.
	 */

	/*
	 * Note: Sharing one static valuable between INT_ACK and EOI registers
	 *       access shortens the code size in Diab/PPC. It reduces the
	 *       times of the memory access.
	 */

	EPIC_REG_NO_OFFSET_WRITE((UINT32)vxbEpicIntAckReg +
				 (EPIC_EOI_REG - EPIC_INT_ACK_REG), 0x0);
	} /* while */
    }

/*******************************************************************************
*
* vxbEpicIpiGen - Generate Inter Processor Interrupt
*
* This functions generates a IPI interrupt at the target CPU sets specified bye
* the second argument. The first arguments can be of the four IPI channels
* available at the EPIC.
*
* RETURNS: OK or ERROR for an invalid IPI channel
*
*
* ERRNO: N/A
*
* \NOMANUAL
*
*/

STATUS vxbEpicIpiGen
    (
    VXB_DEVICE_ID pCtlr,
    INT32 ipiId,
    cpuset_t cpus
    )
    {
    /* 
     * Note: Getting the IPI dispatch register address with Reg0 +
     *       (0x10 * ipiId) is faster than searching it from a table, such as
     *       epicIpiDpatchReg[ipiId], in Diab/PPC.
     */

    EPIC_REG_NO_OFFSET_WRITE((UINT32)vxbEpicIpiDpatchReg0 +
			     (0x10 * ipiId), cpus);

    return (OK);
    } 


#ifdef _WRS_CONFIG_SV_INSTRUMENTATION

/*******************************************************************************
*
* vxbSysEpicIntHandlerSV - handles the EPIC interrupts to the CPU, instrumented
*
* This routine is identical to vxbSysEpicIntHandler(), but contains System
* Viewer instrumentation. If System Viewer is not enabled, the other, faster,
* routine is used.
*
* RETURNS:  N/A
*
* ERRNO: N/A
*
* SEE ALSO:
* vxbSysEpicIntHandler()

* \NOMANUAL
*/

LOCAL void vxbSysEpicIntHandlerSV (void)
    {
    UINT32	vxbEpicIntVec;
    int		intMask = vxPpcIntMask;
    int         loopCnt = -1;
    
    /* loop till all interrupts are cleared */

    while (TRUE)
	{
	/* clear int, return the vec for highest IRQ */

	vxbEpicIntVec =
	    EPIC_REG_NO_OFFSET_READ(vxbEpicIntAckReg) & 0xffff;

	if (vxbEpicIntVec == vxbEpicSpuriousVector)
	    break;

	VXB_EPICINTCTLR_ENABLE_INTS (intMask);

        WV_EVT_INT_ENT(vxbEpicIntVec);
        loopCnt ++;
        
        VXB_INTCTLR_ISR_CALL(pVxbEpicIsrHandle, vxbEpicIntVec);

	VXB_EPICINTCTLR_DISABLE_INTS (intMask);

	/*
	 * Disable External Interrupts
	 * External Interrupts will be re-enabled in the kernel's wrapper
	 * of this Interrupt.
	 */

	/*
	 * Note: Sharing one static valuable between INT_ACK and EOI registers
	 *       access shortens the code size in Diab/PPC. It reduces the
	 *       times of the memory access.
	 */

	EPIC_REG_NO_OFFSET_WRITE((UINT32)vxbEpicIntAckReg +
				 (EPIC_EOI_REG - EPIC_INT_ACK_REG), 0x0);
	} /* while */
    
    while (loopCnt-- > 0)        
        EVT_CTX_0(EVENT_INT_EXIT);
    }

#endif /* _WRS_CONFIG_SV_INSTRUMENTATION */
