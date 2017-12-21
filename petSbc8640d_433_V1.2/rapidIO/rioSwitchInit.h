
/*
 * Copyright (c) 2013- Prophet Electronic,Inc.
 * 
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Prophet license agreement. 
 */


/*
modification history
-----------------------
03/06/2013,sjy Created the file.
 */

#ifndef __RIO_SWITCH_INIT_H_
#define __RIO_SWITCH_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif
	
extern void initRioSwitch();
#define SWITCH_INIT_DEBUG
#ifdef SWITCH_INIT_DEBUG	
#define SWITCH_INIT_DBG_LOG(fmt,a1,a2,a3,a4,a5,a6)  DEBUG_LOG(fmt,a1,a2,a3,a4,a5,a6)
#else
#define SWITCH_INIT_DBG_LOG(fmt,a1,a2,a3,a4,a5,a6) 
#endif
	
#ifdef __cplusplus
}
#endif

#endif //__RIO_SWITCH_INIT_H_


