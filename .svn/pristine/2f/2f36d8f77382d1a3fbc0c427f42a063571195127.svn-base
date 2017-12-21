# Makefile - for config/cmps/vxWorks 
#
# Copyright 2005, 2008 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement
#
# modification history
# --------------------
# 01c,13oct08,h_k  removed path for the 3rdparty dirs.
# 01b,08aug08,d_l  add support for target/3rdparty directory
# 01a,13jul05,zhr  written
#
# DESCRIPTION
# This file contains the makefile rules for building the CxrCat.txt
#

RM            = rm -f
CONCATED_FILE = CxrCat.txt
CDF_FILES    := $(sort $(wildcard *.cdf))

all : $(CONCATED_FILE)

$(CONCATED_FILE) : $(CDF_FILES)
	@-$(RM) $(CONCATED_FILE);
	(for cdff in $(CDF_FILES); do \
            cat $$cdff >> $(CONCATED_FILE); \
         done) 

clean :
	@-$(RM) $(CONCATED_FILE)


