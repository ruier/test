CPU         = PPC32
TOOL		    = e500v2diab
CPU_VARIANT	= _ppc85XX_e500v2

TGT_DIR = $(WIND_BASE)/target

include $(TGT_DIR)/h/make/defs.bsp

BOARD           = VPX6-SSD-T

TARGET_DIR      = VPX6-SSD-T
VENDOR          = prophet
BSP_DIR = $(TGT_DIR)/config/$(TARGET_DIR)
DRV_DIR = $(TGT_DIR)/config/$(TARGET_DIR)/FS/driver
BLK_DIR = $(TGT_DIR)/config/$(TARGET_DIR)/FS/blk
LIB_DIR = $(TGT_DIR)/config/$(TARGET_DIR)/FS/lib
TEST_DIR = $(TGT_DIR)/config/$(TARGET_DIR)/FS/test

VPX6-SSDOBJDIR   = $(BSP_DIR)$(DIRCHAR)lib$(DIRCHAR)obj$(BOARD)$(CPU)$(TOOL)vx
VPX6-SSDLIB = $(BSP_DIR)$(DIRCHAR)lib$(DIRCHAR)lib$(BOARD)$(CPU)$(TOOL)vx.a

ADDED_CFLAGS    += -g2

#include $(TGT_DIR)/h/make/rules.bsp
EXTRA_INCLUDE  += -I$(BSP_DIR)
 
FS_LIB:
#FILESYSTEM		
	$(MAKE) $(BSP_DIR)/FS/fsapi.o 
	$(AR) rv $(VPX6-SSDLIB) fsapi.o
	$(MV) fsapi.o $(VPX6-SSDOBJDIR)
		
#	$(MAKE) $(BSP_DIR)/FS/fsclu.o 
#	$(AR) rv $(VPX6-SSDLIB) fsclu.o
#	$(MV) fsclu.o $(VPX6-SSDOBJDIR)
#	
#	$(MAKE) $(BSP_DIR)/FS/fscache.o 
#	$(AR) rv $(VPX6-SSDLIB) fscache.o
#	$(MV) fscache.o $(VPX6-SSDOBJDIR)
#	
#	$(MAKE) $(BSP_DIR)/FS/fsdev.o 
#	$(AR) rv $(VPX6-SSDLIB) fsdev.o
#	$(MV) fsdev.o $(VPX6-SSDOBJDIR)
#	
#	$(MAKE) $(BSP_DIR)/FS/fsent.o 
#	$(AR) rv $(VPX6-SSDLIB) fsent.o
#	$(MV) fsent.o $(VPX6-SSDOBJDIR)
#	
#	$(MAKE) $(BSP_DIR)/FS/fsfdt.o 
#	$(AR) rv $(VPX6-SSDLIB) fsfdt.o
#	$(MV) fsfdt.o $(VPX6-SSDOBJDIR)
#	
#	$(MAKE) $(BSP_DIR)/FS/fsusr.o 
#	$(AR) rv $(VPX6-SSDLIB) fsusr.o
#	$(MV) fsusr.o $(VPX6-SSDOBJDIR)
	
#DRIVER	
#	$(MAKE) $(DRV_DIR)/spcdrv.o 
#	$(AR) rv $(VPX6-SSDLIB) $(DRV_DIR)/spcdrv.o
#	$(MV) $(DRV_DIR)/spcdrv.o $(VPX6-SSDOBJDIR)
#	
#	$(MAKE) $(DRV_DIR)/spchwi.o 
#	$(AR) rv $(VPX6-SSDLIB) $(DRV_DIR)/spchwi.o
#	$(MV) $(DRV_DIR)/spchwi.o $(VPX6-SSDOBJDIR)
#	
#	$(MAKE) $(DRV_DIR)/spcreq.o 
#	$(AR) rv $(VPX6-SSDLIB) $(DRV_DIR)/spcreq.o
#	$(MV) $(DRV_DIR)/spcreq.o $(VPX6-SSDOBJDIR)
#	
#	$(MAKE) $(DRV_DIR)/spcvxb.o 
#	$(AR) rv $(VPX6-SSDLIB) $(DRV_DIR)/spcvxb.o
#	$(MV) $(DRV_DIR)/spcvxb.o $(VPX6-SSDOBJDIR)

#BLK LAYER	
#	$(MAKE) $(BLK_DIR)/blkdev.o 
#	$(AR) rv $(VPX6-SSDLIB) $(BLK_DIR)/blkdev.o
#	$(MV) $(BLK_DIR)/blkdev.o $(VPX6-SSDOBJDIR)
#	
#	$(MAKE) $(BLK_DIR)/blkreq.o 
#	$(AR) rv $(VPX6-SSDLIB) $(BLK_DIR)/blkreq.o
#	$(MV) $(BLK_DIR)/blkreq.o $(VPX6-SSDOBJDIR)
#	
#	$(MAKE) $(BLK_DIR)/blktest.o 
#	$(AR) rv $(VPX6-SSDLIB) $(BLK_DIR)/blktest.o
#	$(MV) $(BLK_DIR)/blktest.o $(VPX6-SSDOBJDIR)
	
#FS LIBRARY	
#	$(MAKE) $(LIB_DIR)/fslib.o 
#	$(AR) rv $(VPX6-SSDLIB) $(LIB_DIR)/fslib.o
#	$(MV) $(LIB_DIR)/fslib.o $(VPX6-SSDOBJDIR)
#	
#	$(MAKE) $(LIB_DIR)/listlib.o 
#	$(AR) rv $(VPX6-SSDLIB) $(LIB_DIR)/listlib.o
#	$(MV) $(LIB_DIR)/listlib.o $(VPX6-SSDOBJDIR)
	
#FS TEST	
#	$(MAKE) $(TEST_DIR)/fstest.o 
#	$(AR) rv $(VPX6-SSDLIB) $(TEST_DIR)/fstest.o
#	$(MV) $(TEST_DIR)/fstest.o $(VPX6-SSDOBJDIR)	