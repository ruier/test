/* rioException.h - rioException.c's header file.*/

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

#ifndef __RIO_EXCEPTION_H_
#define __RIO_EXCEPTION_H_

#ifdef __cplusplus
extern "C" {
#endif
	
#define CONFIG_WRITE_TIMEOUT 10
#define CONFIG_READ_TIMEOUT 10    //config read timeout
#define CONFIG_READ_TIMEOUT_TIME 0x800000
	
extern int rioReadState;
extern STATUS excHookInit (void);
	
extern STATUS rioErrorIntSetup(void);	
	
	
	
#ifdef __cplusplus
}
#endif

#endif //__RIO_EXCEPTION_H_
