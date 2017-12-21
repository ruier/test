# Makefile - make rules for wrSbcPowerQuiccII Wind River SBCPowerQUICC II Board
#
# Copyright 1988-2006 Wind River Systems, Inc.
#
# modification history
# --------------------
# 01l,18mar09,pcs  Undo the previous checkin which happend by mistake.
# 01k,10mar09,pcs  Debug.
# 01j,11jun07,vik  removed cmdLine.c build
# 01i,01dec06,d_c  Add dependency on sysLib.c to create cmdLine.c
# 01j,02mar07,rec  WIND00089420 - remove ipnet makefile workaround
# 01i,13dec06,rec  vxWorks 6.5 changes
# 01h,21sep05,pch  adjust CPU and add CPU_VARIANT to enable kernel source
#                  project builds for scalability
# 01g,08sep03,j_b  increase system memory for bootrom
# 01f,29jul03,scm  for fei/gei/pcmcia only builds
# 01e,29jul03,j_b  remove local END driver references
# 01d,12jun03,j_b  remove debug flag by default
# 01c,28may03,j_b  change END object files to .obj
# 01b,20may03,jb3  make motscc local.
# 01a,01apr03,j_b  adapted from wrSbc8260Atm BSP makefile (ver 01f).
#
# DESCRIPTION
# -----------
# This file contains rules for building VxWorks for the
# Wind River SBCPowerQUICC II board.
#
#  TARGETS
#  -------
#  This section briefly lists the available build targets.
#
# INCLUDES
#     makeTarget
#*/

CPU             = PPC32
CPU_VARIANT     = _ppc603
TOOL            = diab

TGT_DIR = $(WIND_BASE)/target
include $(TGT_DIR)/h/make/defs.bsp

##
## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.
##

TARGET_DIR	= wrSbcPowerQuiccII
VENDOR		= WindRiver
BOARD		= SBCPowerQUICC II

RELEASE         += bootrom_uncmp.hex

USR_ENTRY	= usrInit

## The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
## in config.h, MakeSkel, Makefile, and Makefile.*
## All definitions for these constants must be identical.

ROM_TEXT_ADRS   = FFF00100  # ROM entry address
ROM_SIZE        = 00100000  # number of bytes of ROM space

RAM_LOW_ADRS    = 00100000  # RAM text/data address
RAM_HIGH_ADRS   = 01C00000  # RAM text/data address, for 64Meg 01D00000 and for 16Meg 00D00000

## Uncomment the following line to enable debugging
## EXTRA_DEFINE    = -g

MACH_EXTRA	= 
EXTRA_MODULES   =  

## Only redefine make definitions above this point, or the expansion of
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp

