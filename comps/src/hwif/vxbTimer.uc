#ifdef INCLUDE_TIMER_SYS
#include <../config/comps/src/usrVxbTimerSys.c>
IMPORT STATUS vxbSysClkLibInit();
#  ifdef INCLUDE_AUX_CLK
IMPORT STATUS vxbAuxClkLibInit();
#  endif /* INCLUDE_AUX_CLK */
#  ifdef INCLUDE_TIMESTAMP
IMPORT STATUS vxbTimestampLibInit();
#  endif /* INCLUDE_TIMESTAMP */
#  ifdef INCLUDE_TIMER_RTC
IMPORT STATUS vxbRtcLibInit();
#  endif /* INCLUDE_TIMER_RTC */
IMPORT STATUS vxbDelayLibInit();
#else  /* INCLUDE_TIMER_SYS */
#  ifdef INCLUDE_TIMER_STUB
IMPORT void vxbTimerStubInit();
#  endif /* INCLUDE_TIMER_STUB */
#endif /* INCLUDE_TIMER_SYS */
