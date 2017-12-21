/* i2cBoard.h - 835 I2C Driver Header Module */

/* create by:YZF 2012.11.30 */

#ifndef	INCmpc8640dI2ch
#define	INCmpc8640dI2ch

#include <vxWorks.h>
#include <config.h>

#ifdef __cplusplus
extern "C" {
#endif
	

IMPORT STATUS i2cInit();
IMPORT	float getChannelTemp(UINT8 );
IMPORT STATUS setAlertThres(UINT8 ,UINT8 );
IMPORT UINT8 getAlertThres(UINT8 );
IMPORT STATUS setOverThres(UINT8 ,UINT8 );
IMPORT UINT8 getOverThres(UINT8 );
IMPORT void sysMsDelay(UINT);
IMPORT double getLocalTemp(UINT8 );
IMPORT double getBoardVol(UINT8);
IMPORT double getBoardCurr(UINT8);
IMPORT double getTotalPower();
IMPORT STATUS initLct2991();
	
#ifdef __cplusplus
}
#endif


#endif /* INCi2cBoardh */
