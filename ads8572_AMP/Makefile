# Makefile - makefile for Freescale ads8572 (Whitefin) board AMP core 0
#
# Copyright (c) 2007-2008 Wind River Systems, Inc.
# 
# modification history
# --------------------
# 01g,02oct08,kab  Remove VXBUILD directive
# 01f,02sep08,dtr  Change to TGT_DIR.
# 01e,25aug08,dtr  Add VXBUILD=AMP default for release builds.
# 01d,18aug08,dtr  Fix bootrom build.
# 01c,14apr08,dtr  Make hard float default.
# 01b,23jan08,dtr  Revert to standard soft float build.
# 01a,17dec07,dtr created
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# FreeScale MPC8572 ADS reference board
#
# INCLUDES
#     makeTarget
#*/

CPU              = PPC32
#optional soft-float kernel 
#TOOL             = sfdiab
TOOL             = e500v2diab
#Support for 36 bit physical addresses in the MMU can be added for 
#vxWorks 6.2 and later with the following CPU_VARIANT.
CPU_VARIANT      = _ppc85XX_e500v2
#Use diab for vxWorks 5.5.1
#TOOL = diab

TGT_DIR = $(WIND_BASE)/target

include $(TGT_DIR)/h/make/defs.bsp
#include $(TGT_DIR)/h/make/make.$(CPU)$(TOOL)
#include $(TGT_DIR)/h/make/defs.$(WIND_HOST_TYPE)

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.


TARGET_DIR	= ads8572_AMP
VENDOR		= Freescale
BOARD		= ads8572

RELEASE_CMD		= bootrom_uncmp.hex bootrom_uncmp.bin bootrom_uncmp bootrom bootrom.hex

EXTRA_DEFINE    += -D_WRS_VX_AMP

LOCAL_MEM_LOCAL_ADRS = 0x00000000

ROM_TEXT_ADRS    = fff00100 # ROM entry address
ROM_SIZE         = 00100000 # number of bytes of ROM space
ROM_WARM_ADRS    = fff00104 # ROM warm entry address 
ROM_LINK_ADRS	 = fff00000
RAM_LOW_ADRS     = 00100000 # RAM text/data address
RAM_HIGH_ADRS    = 01000000 # RAM text/data address


MACH_EXTRA       = 

BOOT_EXTRA       = 
LIB_EXTRA        = 

VMA_START        = 0x0
USR_ENTRY        = usrInit
## Only redefine make definitions above this point, or the expansion of 
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp
#include $(TGT_DIR)/h/make/rules.$(WIND_HOST_TYPE)

