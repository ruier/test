/* ctors and dtors arrays -- to be used by runtime system */
/*   to call static constructors and destructors          */
/*                                                        */
/* NOTE: Use a C compiler to compile this file. If you    */
/*       are using GNU C++, be sure to use compile this   */
/*       file using a GNU compiler with the               */
/*       -fdollars-in-identifiers flag.                   */


#if defined(__GNUC__)

extern void __register_frame_info (const void *, void *);
extern void *__deregister_frame_info (const void *);
extern const unsigned __EH_FRAME_BEGIN__[];
static void _STI__15_ctors ()
{
  /* This structure must approximately match that in unwind-dw2-fde.h.
     In particular it must be no smaller, and no less aligned.  */
  static struct object {
    void *pc_begin; void *tbase; void *dbase; void *u;
    unsigned long b; void *fde_end; struct object *next;
  } object;
  __register_frame_info (__EH_FRAME_BEGIN__, &object);
}

static void _STD__15_dtors ()
{
  __deregister_frame_info (__EH_FRAME_BEGIN__);
}
#else
static void _STI__15_ctors ()
{
}

static void _STD__15_dtors ()
{
}
#endif

char __dso_handle = 0;

void _GLOBAL__I_65535_0__ZSt7nothrow();

void _GLOBAL__I_65535_0___cxa_allocate_exception();

void _GLOBAL__I_65535_0___cxa_get_globals_fast();

extern void (*_ctors[])();
void (*_ctors[])() =
    {
    _STI__15_ctors,
    _GLOBAL__I_65535_0__ZSt7nothrow,
    _GLOBAL__I_65535_0___cxa_allocate_exception,
    _GLOBAL__I_65535_0___cxa_get_globals_fast,
    0
    };

void _STD__15_dtors();

void _GLOBAL__D_65535_0___gthread_key_create();

extern void (*_dtors[])();
void (*_dtors[])() =
    {
    _STD__15_dtors,
    _GLOBAL__D_65535_0___gthread_key_create,
    0
    };

#include <vxWorks.h>
#include <config.h>

/* build variables */
#if defined(INCLUDE_LOADER) || defined(INCLUDE_WDB)
#ifdef __GNUC__
__asm("	.section \".wrs_build_vars\",\"a\"");
#endif
#ifdef __DCC__
__asm("	.section \".wrs_build_vars\",\"r\"");
#endif
__asm("	.global _wrs_build_vars");
__asm("	.global _wrs_build_vars_end");
__asm("_wrs_build_vars:");
#else
#ifdef __GNUC__
__asm("	.section \".wrs_build_vars\"");
#endif
#ifdef __DCC__
__asm("	.section \".wrs_build_vars\",\"n\"");
#endif
#endif
__asm("	.ascii \"set WIND_HOME /view/ot.26_share.2013091112/wind\"");
__asm("	.byte 0");
__asm("	.ascii \"set WIND_BASE $(WIND_HOME)/vxworks-6.9\"");
__asm("	.byte 0");
__asm("	.ascii \"set VSB_DIR $(WIND_BASE)/target/lib\"");
__asm("	.byte 0");
__asm("	.ascii \"set ANALYSIS_DKM_DIR $(VSB_DIR)/simpc/SIMNT/common/dkm/analysis\"");
__asm("	.byte 0");
#if defined(INCLUDE_LOADER) || defined(INCLUDE_WDB)
__asm("_wrs_build_vars_end:");
#endif
__asm("	.ascii \"end\"");
__asm("	.byte 0");
__asm("	.previous");

/* OSCB static tables */
#include <oscb.h>

static const uint64_t oscbOSFlags = OSCB_OS_FLAGS_VALUE;
static const uint64_t oscbCapProtocols = OSCB_VX_CAP_PROTOCOLS;
static const uint64_t oscbCapKernel = OSCB_VX_CAP_KERNEL;
#ifdef RUNTIME_CREATION_DATE
static const char oscbOSSig[] = RUNTIME_CREATION_DATE;
#elif defined(RUNTIME_CREATION_TIME)
static const char oscbOSSig[] = RUNTIME_CREATION_TIME;
#else
static const char oscbOSSig[] = __DATE__" "__TIME__;
#endif /* RUNTIME_CREATION_DATE */
static const char oscbOSBuildPath[] = "";

static const struct OSCB_key_value oscb_keyval_table[] =
    {
    {OSCB_OS_SIGNATURE, (void*) oscbOSSig},
    {OSCB_OS_BUILD_PATH, (void*) oscbOSBuildPath},
    {OSCB_TARGET_SIGNATURE, (void*) &sysProcNum},
    {OSCB_OS_BUILD_HOST, (void*) sysBootHost},
    {OSCB_OS_HEARTBEAT, (void*) &vxAbsTicks},
    {OSCB_OS_FLAGS, (void*) &oscbOSFlags},
#if defined(INCLUDE_LOADER) || defined(INCLUDE_WDB)
    {OSCB_VX_BUILD_VARS, (void*) _wrs_build_vars},
    {OSCB_VX_BUILD_VARS_END, (void*) _wrs_build_vars_end},
#endif
    {OSCB_VX_BOOTLINE, (void*) &sysBootLine},
    {OSCB_VX_EXC_RECORD, (void*) &sysExcMsg},
    {OSCB_VX_CAPABILITY_SET_PROTOCOLS, (void*) &oscbCapProtocols},
    {OSCB_VX_CAPABILITY_SET_KERNEL, (void*) &oscbCapKernel},
#ifdef INCLUDE_WINDVIEW
    {OSCB_VX_SV_LOGS, (void*) &wvCurrentLogList},
#endif
    {OSCB_TERM, (void*) 0}
    };

const struct OSCB_head __oscb_hdr =
    {
    {OSCB_SIGN_0, OSCB_SIGN_1, OSCB_SIGN_2, OSCB_SIGN_3},
    OSCB_ENDIAN,
    OSCB_PRO_MODEL,
    OSCB_VERSION,
    OSCB_VENDOR_ID,
    RUNTIME_NAME,
    RUNTIME_VERSION,
    (void*) &oscb_keyval_table
    };
