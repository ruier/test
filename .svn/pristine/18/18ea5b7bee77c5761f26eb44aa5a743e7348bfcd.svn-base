/* rioException.c - Handle the rapidIO interrupt,an read and write exceptions .*/

/*
 * Copyright (c) 2012- Prophet Electronic,Inc.
 * 
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Prophet license agreement. 
 */

/*
modification history
-----------------------
12/09/2012,sjy Created the file.
 */

/*
 * DESCRIPTION
 * Initialize the RapidIO Switch of CPS1848.
 * 
 */

/* includes */
#include <vxworks.h>
#include <arch/ppc/private/excArchLibP.h>
#include "rioException.h"
#include "rioPrivate.h"


/* veriables */
int rioReadState = 0;
LOCAL FUNCPTR oldExcBaseHook;		/* original _func_excBaseHook */



/******************************************************************************
 * rioErrorIntHandle - rapidIO error interrupt handle .
 *  
 */
LOCAL void rioErrorIntHandle()
{
	//ns16550Pollprintf("E!\n");
	m85xxRioRegWrite(LTLEDCSR,0x0);
	//*(UINT32 *)0xe80c0608 = 0x0;
	rioReadState = 1;
}

/******************************************************************************
 * rioErrorIntSetup - setup rapidIO error interrupt handle .
 *  
 */
LOCAL STATUS rioErrorIntSetup(void)
{
	VXB_DEVICE_ID pInst;
	
	pInst=(VXB_DEVICE_ID)vxbInstByNameFind ("m85xxCPU", 0);
	if (pInst == NULL)
	{
		ns16550Pollprintf ("rioDBIntSetup:Get pInst failed!!!\n");
		return ERROR;
	}
	vxbIntConnect (pInst, 6, rioErrorIntHandle, NULL);
	vxbIntEnable  (pInst, 6, rioErrorIntHandle, NULL);

	return OK;
}

/******************************************************************************
 * rapidIoScanHandle - RapidIO exception handle, this function is used to handle
 *  the RapidIO remote read and write exceptions.
 *  
 */
LOCAL BOOL rapidIoScanHandle
(
		vecTblOffset  vecNum,	/* exception vector number */
		ESFPPC *      pEsf,		/* pointer to exception stack frame */
		REG_SET *     pRegSet,	/* pointer to registers @ exception */
		EXC_INFO *    pExcInfo	/* not used here */
)
{

	if(rioReadState == 1)
	{
		return TRUE;
	}

	/*
	 * Exception was NOT due to vxMemProbe, let other hooks have a crack
	 * at processing the exception.
	 */

	if (oldExcBaseHook != NULL)
	{
		return oldExcBaseHook (vecNum, pEsf, pRegSet, pExcInfo);
	}

	/*
	 * Exception was NOT due to vxMemProbe, and there are no other hooks
	 * that have registered to process exceptions.  Inform excExcHandle()
	 * to continue default exception handling.
	 */

	return FALSE;
}

/******************************************************************************
 * excHookInit - init the rapidIO exception hook.Set the rapidIO exception handle.
 */
LOCAL STATUS excHookInit (void)
{
	if (oldExcBaseHook == NULL)
	{
		oldExcBaseHook = _func_excBaseHook;
		_func_excBaseHook = &rapidIoScanHandle;
		return OK;
	}
	return ERROR;
}

/******************************************************************************
 * rioExcInit - init the rapidIO exception hook.Set the rapidIO exception handle.
 */
STATUS rioExcInit()
{
	rioReadState = 0;
	
	excHookInit();
	rioErrorIntSetup(); 
	
	rioConfigLocalWrite(PRTOCCSR,CONFIG_READ_TIMEOUT_TIME);
	rioConfigLocalWrite(LTLEECSR,0x01000000);
	
	return OK;
}
