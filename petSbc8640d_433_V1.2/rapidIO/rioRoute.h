/* CPS1848RioRoute.h - This file contains the route path configuration ,header file.*/

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


#ifndef __RIO_ROUTE_H_
#define __RIO_ROUTE_H_

/* includes */
#include "rioConfig.h"

/* importes */
extern UINT32 hostCpuID;
extern struct switch_desc * switchArray[];
extern int detectedSwitchNum;

struct route_info {
	int remoteSwitchID;
	int hopCount;
	int numOfPort;
	UINT32* portList;
};

#endif /* __RIO_ROUTE_H_ */
