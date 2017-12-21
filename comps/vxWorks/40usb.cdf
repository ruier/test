/* 40usb.cdf - Universal Serial Bus component description file */

/*
 * Copyright (c) 2000-2002, 2004-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
05h,09aug13,wyy  Drop USB_GEN2_SERIAL_COMMON_TASK_PRIORITY from 100 to 150 to
                 prevent that task from occupying too many time slot (WIND00429944)
05g,28apr13,ghs  Add support of USB message (WIND00411952)
05f,16apr13,ghs  Add USB PL2303 serial device support (WIND00412983)
05e,16jan13,x_s  Added the hidden component INCLUDE_USB_PCI_STUB_CUSTOM.
05d,06jan13,s_z  Redefine debug level using bitmap (WIND00371346)
05c,25sep12,j_x  Add XHCD debug support
05b,16may12,s_z  Add USB PLX TCD support
05a,19mar11,w_x  Add USB 3.0 xHCI HCD related components
04z,24may12,ljg  Require INCLUDE_SHELL when add KEYBOARD_SHELL_ATTACH
                 (WIND00347236)
04y,16aug11,j_x  Unify name of MHCD to MHDRCHCD (WIND00288821)
04x,05aug11,ljg  Modify Capabilities String (WIND00291584)
04w,06jul11,ghs  Adapting GEN2 MSC Driver parameters (WIND00183500)
04v,12jul11,s_z  Add IAD support by default
04u,22apr11,ljg  Add USBTGT printer and USBTGT keyboard emulators support
04t,10apr11,s_z  Correct the definition of USBTGT_NETWORK_DATA_POOL_COUNT
04s,07apr11,ghs  Correct Target Serial Function driver parameter name
                 (WIND00266172)
04r,28mar11,s_z  Remove unused components
04q,24mar11,s_z  Modify the usb target RNDIS function driver configuration
04p,24mar11,m_y  Modify usb target MSC function driver configuration items
04o,23mar11,w_x  Remove unnecessary OTG parameters
04n,22mar11,ghs  Remove unused parameter for serial emulator(WIND00261904)
04m,17mar11,m_y  Modify the usb target MSC function driver configuration
04l,08mar11,s_z  Fix RNDIS dependence issue (WIND00258048)
04k,08mar11,w_x  Add more OTG configuration parameters
04j,07mar11,ghs  fix code review issues
04i,23feb11,s_z  Add parameters to charge the target configuration
04h,22feb11,w_x  Fix some target component dependencies
04g,18feb11,w_x  Update target MSC and MHDRC TCD configuration
04f,12feb11,s_z  Add USB target network function driver support
04e,30jan11,ghs  Add usb serial emulator support
04d,14jan11,w_x  Initial USB OTG support changes (WIND00211541)
04c,14jan11,ghs  Correct dependence of audio demo (WIND00250645)
04b,07dec10,ghs  Change initialization order of USB host and target stack
                 (WIND00245236)
04a,25nov10,ghs  Correct target stack initialization order (WIND00242986)
03z,10nov10,ghs  Change default name size for Bulk/CBI driver (WIND00240908)
03y,01nov10,ghs  Disable usbPciStub.c for 64-bit compilation (WIND00237286)
03x,22oct10,ghs  Add usrUsbAudioDemo to usbHostDevInit (WIND00237406)
03w,18oct10,ghs  Combine class driver initialization into one group
                 (WIND00235953)
03v,16sep10,j_l  Include DMA32 only when needed (Fix for WIND00230745)
03u,08sep10,ghs  Add USB MHCI and SYNOPSYSHCI support
03t,07sep10,ghs  Use "usb0" as usb pegasus default name
03s,01sep10,ghs  Add parameter PEGASUS_END_NAME (WIND00231161)
03r,23aug10,m_y  Update prototype for EHCI/OHCI/UHCI init routine
                 (WIND00229662)
03q,07jul10,m_y  Mask synopsys USB host controller support
03p,10jun10,ghs  Add USB serial components
03o,24may10,m_y  Add parameter USB_WAIT_TIME_OUT_VALUE define.(WIND00183499)
03n,23mar10,j_x  Changed for USB debug (WIND00184542)
03m,04mar10,ghs  Required dependency add INCLUDE_PC_CONSOLE for usb boot shell
                 attach(WIND00202105)
03l,04mar10,m_y  add USB_CHECK_RESUBMIT_URB_FLAG define.(WIND00200272)
03k,17nov09,m_y  Add synopsys USB host controller support(WIND00123426)
03j,20oct09,w_x  Fix GEN2 MSC dependency and a small typo (WIND00187216)
03i,09sep09,adh  Add USB_MEM_PART_SIZE define (WIND00142070)
03h,09sep09,w_x  Remove FSL TCD silicon config options, they now should be set
                 by BSP config.h (WIND00180575)
03g,08sep09,y_l  Add usb Keyboard defination for boot shell and kernel shell
03f,21aug09,ghs  Add USB_HUB_POLLING_INTERVAL define(WIND00160843)
03e,17aug09,w_x  Reorganize GEN2 class driver component description
03d,02jul09,s_z  Add the GEN2 Dm960x serial USB-Ethernet support and adjust
                 the GEN2 USB-Ethernet dependency.
03c,18jun09,s_z  Adjust the GEN2 USB-Ethernet dependency.
03b,29apr09,l_z  GEN2 device driver dependency.
03a,12mar09,w_x  Added more GEN2 mass storage driver configuration options
02z,05mar09,s_z  Added USB GEN2 Usb-Ethernet class driver support
02y,09mar09,w_x  Added FSL TCD driver and USB target CDC serial emulator
02x,05jan09,w_x  Added USB GEN2 MSC class driver support
02w,13nov08,j_x  USB GEN2 class driver support
02v,09jul08,s_z  Reorder initialization of USB Keyboard (Defect WIND00119420)
02u,19sep07,ami  CQ:WIND00102614 Fix (Changes in USB Initialization Sequence)
02t,31aug07,jrp  WIND0010335 - remove trailing blanks in continuations
02s,02aug07,adh  WIND00034090 fix, usb Keyboard in target shell
02r,10aug07,p_g  defect fix WIND00038962
02q,26jul07,jrp  WIND00099137 - changed initialization to match command line
02p,28mar07,sup  Change the return type of API for speaker initialization
02o,05feb07,jrp  USB Headset Integration and Defect 69432
02n,25jan07,ami  Changes made as per vxBus modifications
02m,18jul06,???  fix for defect WIND00045229
02l,01mar06,ami  Extra Tab removed from END_PEGASUS Component (SPR#114377)
02k,10jan06,ami  Parameter to configure the Keyboard Buffer Size added (SPR
                 #116401 Fix)
02j,26dec05,ami  Required Dependency added for the USB Tool (SPR #113168)
02i,12oct05,hch  Fix malta4kc peripheral project initialization bug(SPR#110507)
                 Removed the extra "\" in INCLUDE_USB_MS_CBI_INIT component
02h,07oct05,hch  Add USB_BULK_NON_REMOVABLE_DISK configuration parameter
02g,28sep05,hch  Remove INCLUDE_DOSFS from INCLUDE_USB_MS_CBI_INIT(SPR#112903)
02f,21sep05,ami  Dependencies of usbTool Components added (SPR#111867)
02e,24aug05,ami  Returned type of function prototype changed (SPR #111445)
02d,18apr05,hch  Add PEGASUS_MAX_DEVS configuration parameter
02c,03mar05,hch  Remove PEGASUS_DESTINATION_ADDRESS parameter
                 Change the default PEGASUS_MAX_DEVS to 1
02b,02mar05,hch  Change the PEGASUS_TARGET_NAME to be usbTarg
02a,25feb05,hch  Add macros for multiple Pegasus device support (SPR #105739)
01z,20jan05,ami  Modifications to configure UFI_MAX_DEVS & UFI_MAX_DRV_NAME_SZ
                 from Project Facility
01y,06jan05,ami  Bulk Maximum Device Name Size added
01x,05jan05,ami  Changes for setting the maximum number of drives supported by
                 Bulk Class Driver through Project Facility
01w,20sep04,ami  NET2280 tested for High Speed
01v,17sep04,ami  Changes for NET2280
01u,16aug04,pdg  Fix for long booting time of OHCI and EHCI host controllers
                 in pentium
01t,21jul04,hch  Merged from development view
                 Corrected the file versions in modification history
01s,16jul04,???  Removed dependency of usbTool on the USB host
01r,09jul04,???  Support for isp1582 initialization using project facility
01q,30jun04,???  mass storage emulator inclusion
01p,11may04,hch  merge after D12 driver testing.
01o,22apr04,???  Support for USB 2.0 peripheral stack
01n,08may02,wef  SPR #77048 - removed INCLUDE_OHCI_PCI_INIT component.
01m,27mar02,wef  SPR 74822: fixed typeo in INCLUDE_AUDIO_DEMO component.
01l,26mar02,pmr  SPR 73970: removed INCLUDE_PCI requirement from
                 INCLUDE_OHCI_PCI_INIT.
01k,22mar02,rhe  Remove unwanted tab in USB Audio Demo SPR 73326
01j,08dec01,dat  Adding BSP_STUBS, INIT_BEFORE,
01i,08dec01,wef  removed references to ACM, KLSI, NC1080 and UGL components /
                 parameters, added new PEGASUS parameters.
01h,25apr01,wef  moved end initialization to usrIosExtraInit
01g,25apr01,wef  added communication, mass storage class, ugl drivers, moved
                 usb init order to usrIosCoreInit
01f,01may00,wef  removed usbTargTool
01e,25apr00,wef  added usbTargTool for testing the USB Target stack
01d,29mar00,wef  broke up usrUsbPciLib.c into three separtate
                 stub<bsp>PciLib.c files included in bsp conponent
01c,10feb00,wef  cleaned-up component description and removed init groups.
01b,09feb00,wef  added component INCLUDE_OHCI_INIT.  initializes OHCI on PCI
                 bus, fixed bug - two instances of MODULE instead of MODULES
01a,25jan00,wef  written
*/

/*
DESCRIPTION
This file contains descriptions for the USB components.
*/

/* Generic USB configuration parameters */

Folder    FOLDER_USB {
    NAME            USB
    _CHILDREN       FOLDER_BUSES
    SYNOPSIS        Universal Serial Bus (USB)
    CHILDREN        FOLDER_USB_HOST         \
                    FOLDER_USB_TARG         \
                    FOLDER_USB_OTG          \
                    FOLDER_USB_COMMON
}

Folder    FOLDER_USB_COMMON {
    NAME            USB Common
    SYNOPSIS        USB Common Components
    CHILDREN        INCLUDE_USB             \
                    INCLUDE_USB_INIT
}

Folder    FOLDER_USB_HOST {
    NAME            USB Host
    SYNOPSIS        USB Host Components
    CHILDREN        INCLUDE_OHCI            \
                    INCLUDE_UHCI            \
                    INCLUDE_EHCI            \
                    INCLUDE_USB_MHDRC_HCD   \
                    INCLUDE_SYNOPSYSHCI     \
                    INCLUDE_USB_XHCI_HCD
}

/* USB Target */

Folder    FOLDER_USB_TARG {
    NAME            USB Target
    SYNOPSIS        USB Target Stack Components
    CHILDREN        FOLDER_USB_GEN1_TARGET    \
                    FOLDER_USB_GEN2_TARGET
}

Folder    FOLDER_USB_GEN1_TARGET {
    NAME            USB GEN1 Target
    SYNOPSIS        USB GEN1 Target Stack Components
    _CHILDREN       FOLDER_USB_TARG
    CHILDREN        FOLDER_USB_PERIPHERAL_CONTROLLERS           \
                    FOLDER_USB_PERIPHERAL_STACK                 \
                    FOLDER_USB_PERIPHERAL_INIT
}

Folder    FOLDER_USB_OTG {
    NAME            USB OTG
    SYNOPSIS        USB OTG Components
    CHILDREN        INCLUDE_USB_OTG
}

Folder    FOLDER_USB_OTG_CONTROLLERS {
    NAME            USB OTG Controllers
    SYNOPSIS        USB OTG Controller Components
    _CHILDREN       FOLDER_USB_OTG
    CHILDREN        INCLUDE_USB_MHDRC_OCD
}

Folder    FOLDER_USB_OTG_CONTROLLER_INIT {
    NAME            USB OTG Controller Init
    SYNOPSIS        USB OTG Controller Initialization Components
    _CHILDREN       FOLDER_USB_OTG
    CHILDREN        FOLDER_USB_MHDRC_OCD_INIT
}

/* usb debug infomation - start */

Folder    FOLDER_USB_DEBUG {
    NAME            USB Debug
    SYNOPSIS        USB Stack Debug
    _CHILDREN       FOLDER_TOOLS
    CHILDREN        FOLDER_USB_SHOW            \
                    FOLDER_USB_TOOL            \
                    FOLDER_USB_TRACK
}

Folder    FOLDER_USB_SHOW {
    NAME            USB Show
    SYNOPSIS        USB Stack Show
    _CHILDREN       FOLDER_USB_DEBUG
    CHILDREN        INCLUDE_USB_SHOW
}

Folder    FOLDER_USB_TOOL {
    NAME            USB Tool
    SYNOPSIS        USB Stack TOOL
    _CHILDREN       FOLDER_USB_DEBUG
    CHILDREN        INCLUDE_USBTOOL
}

Folder    FOLDER_USB_TRACK {
    NAME            USB Track
    SYNOPSIS        USB Stack Track
    _CHILDREN       FOLDER_USB_DEBUG
    CHILDREN        FOLDER_USB_TRACK_HCD           \
                    FOLDER_USB_TRACK_USBD          \
                    FOLDER_USB_TRACK_TU            \
                    FOLDER_USB_TRACK_CLASS_DRIVERS
}

Folder    FOLDER_USB_TRACK_HCD {
    NAME            HCD
    SYNOPSIS        USB Stack Track HCD
    _CHILDREN       FOLDER_USB_TRACK
    CHILDREN        INCLUDE_USB_TRACK_EHCD         \
                    INCLUDE_USB_TRACK_UHCD         \
                    INCLUDE_USB_TRACK_OHCD         \
                    INCLUDE_USB_TRACK_XHCD         \
                    INCLUDE_USB_TRACK_MHDRCHCD     \
                    INCLUDE_USB_TRACK_PLXTCD       \
                    INCLUDE_USB_TRACK_SYNOPSYSHCD
}

Folder    FOLDER_USB_TRACK_TU {
    NAME            TU
    SYNOPSIS        USB Translation Unit
    _CHILDREN       FOLDER_USB_TRACK
    CHILDREN        INCLUDE_USB_TRACK_TU
}

Folder    FOLDER_USB_TRACK_USBD {
    NAME            USBD
    SYNOPSIS        USB Driver
    _CHILDREN       FOLDER_USB_TRACK
    CHILDREN        INCLUDE_USB_TRACK_USBD
}

Folder    FOLDER_USB_TRACK_OTG {
    NAME            OTG
    SYNOPSIS        USB OTG Framework Driver
    _CHILDREN       FOLDER_USB_TRACK
    CHILDREN        INCLUDE_USB_TRACK_OTG
}

Folder    FOLDER_USB_TRACK_TGT {
    NAME            TGT
    SYNOPSIS        USB Target Framework Driver
    _CHILDREN       FOLDER_USB_TRACK
    CHILDREN        INCLUDE_USB_TRACK_TGT \
                    INCLUDE_USB_TRACK_TGT_MSC \
                    INCLUDE_USB_TRACK_TGT_RNDIS \
                    INCLUDE_USB_TRACK_TGT_END_AGENT \
                    INCLUDE_USB_TRACK_TGT_SER \
                    INCLUDE_USB_TRACK_TGT_PRN \
                    INCLUDE_USB_TRACK_TGT_KBD
}

Folder    FOLDER_USB_TRACK_CLASS_DRIVERS {
    NAME            Class Drivers
    SYNOPSIS        USB Stack Track Class Drivers
    _CHILDREN       FOLDER_USB_TRACK
    CHILDREN        FOLDER_USB_TRACK_HUB          \
                    FOLDER_USB_TRACK_GEN1         \
                    FOLDER_USB_TRACK_GEN2
}

Folder    FOLDER_USB_TRACK_HUB {
    NAME            HUB
    SYNOPSIS        USB Stack Track HUB
    _CHILDREN       FOLDER_USB_TRACK_CLASS_DRIVERS
    CHILDREN        INCLUDE_USB_TRACK_HUB
}

Folder    FOLDER_USB_TRACK_GEN1 {
    NAME            GEN1
    SYNOPSIS        USB Stack Track GEN1 Class Drivers
    _CHILDREN       FOLDER_USB_TRACK_CLASS_DRIVERS
    CHILDREN        INCLUDE_USB_TRACK_GEN1_SPK    \
                    INCLUDE_USB_TRACK_GEN1_BLK    \
                    INCLUDE_USB_TRACK_GEN1_CBI    \
                    INCLUDE_USB_TRACK_GEN1_MSE    \
                    INCLUDE_USB_TRACK_GEN1_KBD    \
                    INCLUDE_USB_TRACK_GEN1_PRN    \
                    INCLUDE_USB_TRACK_GEN1_END
}

Folder    FOLDER_USB_TRACK_GEN2 {
    NAME            GEN2
    SYNOPSIS        USB Stack Track GEN2 Class Drivers
    _CHILDREN       FOLDER_USB_TRACK_CLASS_DRIVERS
    CHILDREN        INCLUDE_USB_TRACK_GEN2_HLP    \
                    INCLUDE_USB_TRACK_GEN2_MSC    \
                    INCLUDE_USB_TRACK_GEN2_MSE    \
                    INCLUDE_USB_TRACK_GEN2_KBD    \
                    INCLUDE_USB_TRACK_GEN2_PRN    \
                    INCLUDE_USB_TRACK_GEN2_END    \
                    INCLUDE_USB_TRACK_GEN2_SER
}

Component INCLUDE_USB_SHOW {
    NAME            usbShow
    SYNOPSIS        USB Show - Show USB device information
    REQUIRES        INCLUDE_USB
    _CHILDREN       FOLDER_USB_SHOW
    CONFIGLETTES    usrUsbShow.c
    CFG_PARAMS      USB_SHOW_LEVEL
}

Parameter USB_SHOW_LEVEL {
    NAME            USB Show Level
    SYNOPSIS        USB Show Level (0:SIMPLE, 1:NORMAL, 2:VERBOSE)
    DEFAULT         2
}

Component INCLUDE_USB_TRACK_EHCD {
    NAME            EHCD
    SYNOPSIS        USB EHCD Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_EHCI
    CFG_PARAMS      USB_TRACK_EHCD
}

Parameter USB_TRACK_EHCD {
    NAME            USB EHCD Debug Level BitMap
    SYNOPSIS        USB EHCD Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}

Component INCLUDE_USB_TRACK_XHCD {
    NAME            xHCI HCD
    SYNOPSIS        USB xHCI HCD Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB_XHCI_HCD
    CFG_PARAMS      USB_TRACK_XHCD
}

Parameter USB_TRACK_XHCD {
    NAME            USB xHCI HCD Debug Level
    SYNOPSIS        USB xHCI HCD Debug Level (0:OFF, 1:ERROR, 2:WARNING, 3:NORMAL, 4:VERBOSE)
    DEFAULT         1
}

Component INCLUDE_USB_TRACK_OHCD {
    NAME            OHCD
    SYNOPSIS        USB OHCD Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_OHCI
    CFG_PARAMS      USB_TRACK_OHCD
}

Parameter USB_TRACK_OHCD {
    NAME            USB OHCD Debug Level BitMap
    SYNOPSIS        USB OHCD Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}

Component INCLUDE_USB_TRACK_UHCD {
    NAME            UHCD
    SYNOPSIS        USB UHCD Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_UHCI
    CFG_PARAMS      USB_TRACK_UHCD
}

Parameter USB_TRACK_UHCD {
    NAME            USB UHCD Debug Level BitMap
    SYNOPSIS        USB UHCD Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}

Component INCLUDE_USB_TRACK_MHDRCHCD {
    NAME            MHDRCHCD
    SYNOPSIS        USB MHDRCHCD Tracking
    CONFIGLETTES    usrUsbDebug.c
    CFG_PARAMS      USB_TRACK_MHDRCHCD
}

Parameter USB_TRACK_MHDRCHCD {
    NAME            USB MHDRCHCD Debug Level BitMap
    SYNOPSIS        USB MHDRCHCD Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}

Component INCLUDE_USB_TRACK_PLXTCD {
    NAME            PLXTCD
    SYNOPSIS        USB PLXTCD Tracking
    CONFIGLETTES    usrUsbDebug.c
    CFG_PARAMS      USB_TRACK_PLXTCD
}

Parameter USB_TRACK_PLXTCD {
    NAME            USB PLXTCD Debug Level
    SYNOPSIS        USB PLXTCD Debug Level (0:OFF, 1:ERROR, 2:WARNING, 3:NORMAL, 4:VERBOSE)
    DEFAULT         1
}

Component INCLUDE_USB_TRACK_SYNOPSYSHCD {
    NAME            SYNOPSYSHCD
    SYNOPSIS        USB SYNOPSYSHCD Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_SYNOPSYSHCI
    CFG_PARAMS      USB_TRACK_SYNOPSYSHCD
}

Parameter USB_TRACK_SYNOPSYSHCD {
    NAME            USB SYNOPSYSHCD Debug Level BitMap
    SYNOPSIS        USB SYNOPSYSHCD Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}

Component INCLUDE_USB_TRACK_TU {
    NAME            TU
    SYNOPSIS        USB Translation Unit Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB
    CFG_PARAMS      USB_TRACK_TU
}

Parameter USB_TRACK_TU {
    NAME            USB TU Debug Level BitMap
    SYNOPSIS        USB TU Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}

Component INCLUDE_USB_TRACK_USBD {
    NAME            USBD
    SYNOPSIS        USB Driver Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB
    CFG_PARAMS      USB_TRACK_USBD
}

Parameter USB_TRACK_USBD {
    NAME            USB Driver Debug Level BitMap
    SYNOPSIS        USB Driver Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}

Component INCLUDE_USB_TRACK_HUB {
    NAME            HUB
    SYNOPSIS        USB HUB Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB
    CFG_PARAMS      USB_TRACK_HUB
}

Parameter USB_TRACK_HUB {
    NAME            USB HUB Debug Level BitMap
    SYNOPSIS        USB HUB Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}

Component INCLUDE_USB_TRACK_OTG {
    NAME            OTG
    SYNOPSIS        USB Driver Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB
    CFG_PARAMS      USB_TRACK_OTG
}

Parameter USB_TRACK_OTG {
    NAME            USB OTG Driver Debug Level BitMap
    SYNOPSIS        USB OTG Driver Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}

Component INCLUDE_USB_TRACK_TGT {
    NAME            TGT
    SYNOPSIS        USB Target Driver Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB
    CFG_PARAMS      USB_TRACK_TGT
}

Parameter USB_TRACK_TGT {
    NAME            USB Target Driver Debug Level BitMap
    SYNOPSIS        USB Target Driver Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}

Component INCLUDE_USB_TRACK_TGT_MSC {
    NAME            TGT MSC
    SYNOPSIS        USB Target MSC Driver Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB
    CFG_PARAMS      USB_TRACK_TGT_MSC
}

Parameter USB_TRACK_TGT_MSC {
    NAME            USB Target MSC Driver Debug Level BitMap
    SYNOPSIS        USB Target MSC Driver Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}

Component INCLUDE_USB_TRACK_TGT_RNDIS {
    NAME            TGT RNDIS Function Driver Tracker
    SYNOPSIS        TGT RNDIS Function Driver Tracker
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB
    CFG_PARAMS      USB_TRACK_TGT_RNDIS
}

Parameter USB_TRACK_TGT_RNDIS {
    NAME            USB Target RNDIS Driver Debug Level BitMap
    SYNOPSIS        USB Target RNDIS Driver Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}

Component INCLUDE_USB_TRACK_TGT_END_AGENT {
    NAME            TGT END Agent Tracker
    SYNOPSIS        TGT END Agent Tracker
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB
    CFG_PARAMS      USB_TRACK_TGT_END
}

Parameter USB_TRACK_TGT_END {
    NAME            USB Target END Driver Debug Level BitMap
    SYNOPSIS        USB Target END Driver Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}

Component INCLUDE_USB_TRACK_TGT_SER {
    NAME            TGT SER
    SYNOPSIS        USB Target Serial Driver Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB
    CFG_PARAMS      USB_TRACK_TGT_SER
}

Parameter USB_TRACK_TGT_SER {
    NAME            USB Target Serial Driver Debug Level BitMap
    SYNOPSIS        USB Target Serial Driver Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}

Component INCLUDE_USB_TRACK_TGT_PRN {
    NAME            TGT PRN
    SYNOPSIS        USB Target Printer Driver Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB
    CFG_PARAMS      USB_TRACK_TGT_PRN
}

Parameter USB_TRACK_TGT_PRN {
    NAME            USB Target Printer Driver Debug Level BitMap
    SYNOPSIS        USB Target Printer Driver Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}

Component INCLUDE_USB_TRACK_TGT_KBD {
    NAME            TGT KBD
    SYNOPSIS        USB Target Keyboard Driver Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB
    CFG_PARAMS      USB_TRACK_TGT_KBD
}

Parameter USB_TRACK_TGT_KBD {
    NAME            USB Target Keyboard Driver Debug Level BitMap
    SYNOPSIS        USB Target Keyboard Driver Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}

/* GEN1 */

Component INCLUDE_USB_TRACK_GEN1_SPK {
    NAME            SPEAKER
    SYNOPSIS        USB GEN1 Speaker Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB_SPEAKER
    CFG_PARAMS      USB_TRACK_GEN1_SPK
}

Parameter USB_TRACK_GEN1_SPK {
    NAME            USB GEN1 Speaker Debug Level BitMap
    SYNOPSIS        USB GEN1 Speaker Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}

Component INCLUDE_USB_TRACK_GEN1_BLK {
    NAME            BULK
    SYNOPSIS        USB GEN1 BULK Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB_MS_BULKONLY
    CFG_PARAMS      USB_TRACK_GEN1_BLK
}

Parameter USB_TRACK_GEN1_BLK {
    NAME            USB GEN1 Bulk Debug Level BitMap
    SYNOPSIS        USB GEN1 Bulk Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}

Component INCLUDE_USB_TRACK_GEN1_CBI {
    NAME            CBI
    SYNOPSIS        USB GEN1 BULK Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB_MS_CBI
    CFG_PARAMS      USB_TRACK_GEN1_CBI
}

Parameter USB_TRACK_GEN1_CBI {
    NAME            USB GEN1 CBI Debug Level BitMap
    SYNOPSIS        USB GEN1 CBI Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}

Component INCLUDE_USB_TRACK_GEN1_MSE {
    NAME            MOUSE
    SYNOPSIS        USB GEN1 MOUSE Driver Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB_MOUSE
    CFG_PARAMS      USB_TRACK_GEN1_MSE
}

Parameter USB_TRACK_GEN1_MSE {
    NAME            USB GEN1 Mouse Debug Level BitMap
    SYNOPSIS        USB GEN1 Mouse Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}

Component INCLUDE_USB_TRACK_GEN1_KBD {
    NAME            KEYBOARD
    SYNOPSIS        USB GEN1 Keyboard Driver Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB_KEYBOARD
    CFG_PARAMS      USB_TRACK_GEN1_KBD
}

Parameter USB_TRACK_GEN1_KBD {
    NAME            USB GEN1 Keyboard Debug Level BitMap
    SYNOPSIS        USB GEN1 Keyboard Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}

Component INCLUDE_USB_TRACK_GEN1_PRN {
    NAME            PRINTER
    SYNOPSIS        USB GEN1 Printer Driver Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB_PRINTER
    CFG_PARAMS      USB_TRACK_GEN1_PRN
}

Parameter USB_TRACK_GEN1_PRN {
    NAME            USB GEN1 Printer Debug Level BitMap
    SYNOPSIS        USB GEN1 Printer Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}

Component INCLUDE_USB_TRACK_GEN1_END {
    NAME            END
    SYNOPSIS        USB GEN1 END Driver Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB_PEGASUS_END
    CFG_PARAMS      USB_TRACK_GEN1_END
}

Parameter USB_TRACK_GEN1_END {
    NAME            USB GEN1 END Debug Level BitMap
    SYNOPSIS        USB GEN1 END Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}

/* GEN2 */

Component INCLUDE_USB_TRACK_GEN2_HLP {
    NAME            HELPER
    SYNOPSIS        USB GEN2 Helper Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB_GEN2_HELPER
    CFG_PARAMS      USB_TRACK_GEN2_HLP
}

Parameter USB_TRACK_GEN2_HLP {
    NAME            USB GEN2 Helper Debug Level BitMap
    SYNOPSIS        USB GEN2 Helper Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}

Component INCLUDE_USB_TRACK_GEN2_MSC {
    NAME            MASS STORAGE
    SYNOPSIS        USB GEN2 Mass Storage Driver Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB_GEN2_STORAGE
    CFG_PARAMS      USB_TRACK_GEN2_MSC
}

Parameter USB_TRACK_GEN2_MSC {
    NAME            USB GEN2 Mass Storage Debug Level BitMap
    SYNOPSIS        USB GEN2 Mass Storage Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}

Component INCLUDE_USB_TRACK_GEN2_MSE {
    NAME            MOUSE
    SYNOPSIS        USB GEN2 MOUSE Driver Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB_GEN2_MOUSE
    CFG_PARAMS      USB_TRACK_GEN2_MSE
}

Parameter USB_TRACK_GEN2_MSE {
    NAME            USB GEN2 Mouse Debug Level BitMap
    SYNOPSIS        USB GEN2 Mouse Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}

Component INCLUDE_USB_TRACK_GEN2_KBD {
    NAME            KEYBOARD
    SYNOPSIS        USB GEN2 Keyboard Driver Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB_GEN2_KEYBOARD
    CFG_PARAMS      USB_TRACK_GEN2_KBD
}

Parameter USB_TRACK_GEN2_KBD {
    NAME            USB GEN2 Keyboard Debug Level BitMap
    SYNOPSIS        USB GEN2 Keyboard Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}

Component INCLUDE_USB_TRACK_GEN2_PRN {
    NAME            PRINTER
    SYNOPSIS        USB GEN2 Printer Driver Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB_GEN2_PRINTER
    CFG_PARAMS      USB_TRACK_GEN2_PRN
}

Parameter USB_TRACK_GEN2_PRN {
    NAME            USB GEN2 Printer Debug Level BitMap
    SYNOPSIS        USB GEN2 Printer Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}

Component INCLUDE_USB_TRACK_GEN2_END {
    NAME            END
    SYNOPSIS        USB GEN2 END Driver Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB_GEN2_PEGASUS
    CFG_PARAMS      USB_TRACK_GEN2_END
}

Parameter USB_TRACK_GEN2_END {
    NAME            USB GEN2 END Debug Level BitMap
    SYNOPSIS        USB GEN2 END Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}

Component INCLUDE_USB_TRACK_GEN2_SER {
    NAME            SERIAL
    SYNOPSIS        USB GEN2 Serial Driver Tracking
    CONFIGLETTES    usrUsbDebug.c
    REQUIRES        INCLUDE_USB_GEN2_SERIAL_INIT
    CFG_PARAMS      USB_TRACK_GEN2_SER
}

Parameter USB_TRACK_GEN2_SER {
    NAME            USB GEN2 Serial Debug Level BitMap
    SYNOPSIS        USB GEN2 Serial Debug Level BitMap Enable By Set (Bit 0:INFO, Bit 1:ERROR, Bit 2:WARNING, Bit 3:NORMAL, Bit 4:VERBOSE)
    DEFAULT         1
}

/* usb debug infomation - end */

Folder FOLDER_USB_PERIPHERAL_CONTROLLERS {
    NAME            USB Peripheral Controllers
    SYNOPSIS        USB Target Controllers
    _CHILDREN       FOLDER_USB_GEN1_TARGET
    CHILDREN        INCLUDE_PDIUSBD12                       \
                    INCLUDE_PHILIPS1582                     \
                    INCLUDE_NET2280                         \
                    INCLUDE_FSL_TCD
}

Folder FOLDER_USB_PERIPHERAL_STACK {
    NAME            USB Peripheral Stack
    SYNOPSIS        USB Peripheral stack hardware \
                    independent components
    _CHILDREN       FOLDER_USB_GEN1_TARGET
    CHILDREN        INCLUDE_USB_TARG                \
                    INCLUDE_KBD_EMULATOR            \
                    INCLUDE_PRN_EMULATOR            \
                    INCLUDE_D12_EMULATOR            \
                    INCLUDE_MS_EMULATOR             \
                    INCLUDE_SER_EMULATOR
}

Folder FOLDER_USB_PERIPHERAL_INIT {
    NAME            USB Peripheral Init
    SYNOPSIS        USB Peripheral stack Initialization
    _CHILDREN       FOLDER_USB_GEN1_TARGET
    CHILDREN        INCLUDE_KBD_EMULATOR_INIT       \
                    INCLUDE_PRN_EMULATOR_INIT       \
                    INCLUDE_MS_EMULATOR_INIT        \
                    INCLUDE_SER_EMULATOR_INIT

}

Folder  FOLDER_USB_HOST_DEVICES {
    NAME            USB Host Devices
    SYNOPSIS        USB Devices
    _CHILDREN       FOLDER_USB_HOST
    CHILDREN        FOLDER_USB_GEN1_DEVICES    \
                    FOLDER_USB_GEN2_DEVICES    \
                    INCLUDE_USB_HOST_CLASS_INIT
}

Component INCLUDE_USB_HOST_CLASS_INIT {
    NAME            USB Host Class Driver Init
    SYNOPSIS        USB Host Class Driver Initialization
    INIT_RTN        usbHstClassInit();
    _INIT_ORDER     usrRoot
    CONFIGLETTES    usrUsbInit.c
    PROTOTYPE       void usbHstClassInit (void);
}

Folder  FOLDER_USB_GEN2_DEVICES {
    NAME            USB GEN2 Class drivers
    SYNOPSIS        USB Generation 2 Class Device Drivers
    CHILDREN        FOLDER_USB_GEN2_DEVICES_INIT    \
                    INCLUDE_USB_GEN2_PRINTER        \
                    FOLDER_USB_GEN2_END_CONTROLLERS \
                    INCLUDE_USB_GEN2_MOUSE          \
                    INCLUDE_USB_GEN2_KEYBOARD       \
                    INCLUDE_USB_GEN2_STORAGE        \
                    FOLDER_USB_GEN2_SER_ADAPTERS
}

Component INCLUDE_USB_GEN2_HELPER {
    NAME            USB GEN2 Helper Init
    SYNOPSIS        USB GEN2 Helper Task Init (needed to delete device structs)
    REQUIRES        INCLUDE_USB_HOST_CLASS_INIT
    CFG_PARAMS      USB_GEN2_HELPER_LATENCY     \
                    USB_GEN2_HELPER_PRIORITY    \
                    USB_GEN2_HELPER_DELETABLE_QUEUE_SIZE
}

Parameter USB_GEN2_HELPER_LATENCY {
    NAME            USB GEN2 Helper Task Latency (Milliseconds)
    SYNOPSIS        USB GEN2 Helper Task Latency between 2 attempts to delete devices (Milliseconds)
    DEFAULT         1000
}

Parameter USB_GEN2_HELPER_PRIORITY {
    NAME            USB GEN2 Helper Task priority
    SYNOPSIS        USB GEN2 Helper Task priority
    DEFAULT         150
}
Parameter USB_GEN2_HELPER_DELETABLE_QUEUE_SIZE {
    NAME            USB GEN2 Helper queue size
    SYNOPSIS        USB GEN2 Helper queue size (max elems in the queue)
    DEFAULT         100
}

Folder FOLDER_USB_GEN1_DEVICES {
    NAME            USB GEN1 Class drivers
    SYNOPSIS        USB Generation 1 Class Device Drivers
    CHILDREN        INCLUDE_USB_MOUSE          \
                    INCLUDE_USB_KEYBOARD       \
                    INCLUDE_USB_PRINTER        \
                    INCLUDE_USB_SPEAKER        \
                    INCLUDE_USB_PEGASUS_END    \
                    INCLUDE_USB_MS_BULKONLY    \
                    INCLUDE_USB_MS_CBI
}

Folder  FOLDER_USB_HOST_INIT {
    NAME            USB Host Controller Init
    SYNOPSIS        USB Host Component Initialization
    _CHILDREN       FOLDER_USB_HOST
    CHILDREN        INCLUDE_HCD_BUS           \

}

Folder  FOLDER_USB_GEN1_DEVICES_INIT {
    NAME            USB Device Init
    SYNOPSIS        USB Device Component Initialization
    _CHILDREN       FOLDER_USB_GEN1_DEVICES
    CHILDREN        INCLUDE_USB_MOUSE_INIT              \
                    INCLUDE_USB_PRINTER_INIT            \
                    INCLUDE_USB_SPEAKER_INIT            \
                    INCLUDE_USB_MS_BULKONLY_INIT        \
                    INCLUDE_USB_MS_CBI_INIT             \
                    INCLUDE_USB_PEGASUS_END_INIT        \
                    INCLUDE_USB_AUDIO_DEMO              \
                    INCLUDE_USB_HEADSET_DEMO
}

Folder  FOLDER_USB_GEN2_DEVICES_INIT {
    NAME            USB GEN2 Device Init
    SYNOPSIS        USB Generation 2 Device Component Initialization
    CHILDREN        INCLUDE_USB_GEN2_PRINTER_INIT       \
                    INCLUDE_USB_GEN2_END_INIT           \
                    INCLUDE_USB_GEN2_MOUSE_INIT         \
                    INCLUDE_USB_GEN2_STORAGE_INIT       \
                    FOLDER_USB_GEN2_SERIAL_INIT         \
                    INCLUDE_USB_GEN2_HELPER
}

Component INCLUDE_USB {
    NAME            USB Common Stack
    SYNOPSIS        USB Common Stack
    MODULES         usbd.o
#ifndef _WRS_CONFIG_LP64
    BSP_STUBS       usbPciStub.c
#endif
    REQUIRES        INCLUDE_USB_ASYN_MSG
    CONFIGLETTES    usrUsbInit.c    \
                    usrUsbDebug.c
    CFG_PARAMS      USB_HUB_POLLING_INTERVAL           \
                    USB_MEM_PART_SIZE                  \
                    USB_CHECK_RESUBMIT_URB_FLAG        \
                    USB_WAIT_TIME_OUT_VALUE
}

Component INCLUDE_USB_ASYN_MSG {
    NAME            USB Message Asyn Lib Init
    SYNOPSIS        USB Message Asynchronous Library Initialization
    REQUIRES        INCLUDE_ERF
    INIT_RTN        usbMsgAsynLibInit();
    _INIT_ORDER     usrRoot
    INIT_AFTER      usrScInit
    INIT_BEFORE     INCLUDE_USB_HOST_CLASS_INIT
}

Parameter USB_HUB_POLLING_INTERVAL {
    NAME            USB Hub Polling Interval (in ms)
    SYNOPSIS        USB Hub Polling Interval (in ms)
    REQUIRES        INCLUDE_USB
    TYPE            int
    DEFAULT         2
}

Parameter USB_MEM_PART_SIZE {
    NAME            USB Initial Memory Partition Size (in bytes)
    SYNOPSIS        USB Initial Memory Partition Size (in bytes)
    REQUIRES        INCLUDE_USB
    TYPE            int
    DEFAULT         0x200000
}

Parameter USB_CHECK_RESUBMIT_URB_FLAG {
    NAME            Check URB resubmited multiple times enable flag (TRUE or FALSE)
    SYNOPSIS        Check if a URB is resubmitted multiple times with the same transfer buffer (TRUE or FALSE)
    REQUIRES        INCLUDE_USB
    TYPE            bool
    DEFAULT         FALSE
}

Parameter USB_WAIT_TIME_OUT_VALUE {
    NAME            USB Waiting Time Out Value (in ms)
    SYNOPSIS        USB Waiting Time Out Value (in ms)
    REQUIRES        INCLUDE_USB
    TYPE            int
    DEFAULT         5000
}

Component INCLUDE_USB_TARG {
    NAME            USB Peripheral Stack
    SYNOPSIS        USB Peripheral Stack
    MODULES         usbTargInitExit.o
    BSP_STUBS       usbPciStub.c
    CONFIGLETTES    usrUsbTargPciInit.c
    PROTOTYPE       void usrUsbTargPciInit();
    INIT_RTN        usrUsbTargPciInit ();
    _INIT_ORDER     usrRoot
    INIT_AFTER      INCLUDE_SYSCLK_INIT
    INIT_BEFORE     usrRtpInit
}

Component INCLUDE_UHCI {
    NAME            UHCI
    SYNOPSIS        Universal Host Controller Interface
    MODULES         usbUhcdInitialization.o
    REQUIRES        INCLUDE_USB \
                    INCLUDE_DMA_SYS
    LINK_SYMS       usbUhcdInit
}

Component INCLUDE_USB_PCI_STUB_CUSTOM {
    NAME            USB PCI Stub Custom
    SYNOPSIS        USB PCI Stub Custom
    _CHILDREN       FOLDER_NOT_VISIBLE
}

#ifdef    _WRS_CONFIG_LP64
Component INCLUDE_UHCI {
    REQUIRES        += INCLUDE_CACHE_DMA32_LIB
}
#endif    /* _WRS_CONFIG_LP64 */

Component INCLUDE_UHCI_INIT {
    NAME            UHCI Init
    SYNOPSIS        Initializes UHCI Driver
    _CHILDREN       FOLDER_USB_HOST_INIT
    REQUIRES        INCLUDE_USB_INIT    \
                    INCLUDE_HCD_BUS     \
                    INCLUDE_UHCI
    EXCLUDES        INCLUDE_USBTOOL
}

Component INCLUDE_OHCI {
    NAME            OHCI
    SYNOPSIS        Open Host Controller Interface
    MODULES         usbOhci.o
    REQUIRES        INCLUDE_USB \
                    INCLUDE_DMA_SYS
    LINK_SYMS       usbOhcdInit
}

#ifdef    _WRS_CONFIG_LP64
Component INCLUDE_OHCI {
    REQUIRES        += INCLUDE_CACHE_DMA32_LIB
}
#endif    /* _WRS_CONFIG_LP64 */

Component INCLUDE_OHCI_INIT {
    NAME            OHCI Init
    SYNOPSIS        Initializes OHCI Driver
    _CHILDREN       FOLDER_USB_HOST_INIT
    REQUIRES        INCLUDE_USB_INIT    \
                    INCLUDE_HCD_BUS     \
                    INCLUDE_OHCI
    EXCLUDES        INCLUDE_USBTOOL
}

Component INCLUDE_EHCI {
    NAME            EHCI
    SYNOPSIS        Enhanced Host Controller Interface Instance
    MODULES         usbEhcdInitExit.o
    REQUIRES        INCLUDE_USB \
                    INCLUDE_DMA_SYS
    LINK_SYMS       usbEhcdInit
}

#ifdef    _WRS_CONFIG_LP64
Component INCLUDE_EHCI {
    REQUIRES        += INCLUDE_CACHE_DMA32_LIB
}
#endif    /* _WRS_CONFIG_LP64 */

Component INCLUDE_EHCI_INIT {
    NAME            EHCI Init
    SYNOPSIS        Initializes EHCI Driver
    _CHILDREN       FOLDER_USB_HOST_INIT
    REQUIRES        INCLUDE_USB_INIT    \
                    INCLUDE_HCD_BUS     \
                    INCLUDE_EHCI
    EXCLUDES        INCLUDE_USBTOOL
}

Component INCLUDE_USB_XHCI_HCD {
    NAME            USB 3.0 xHCI HCD
    SYNOPSIS        USB 3.0 xHCI HCD for eXtensible Host Controller Interface
    MODULES         usbXhcd.o
    REQUIRES        INCLUDE_USB \
                    INCLUDE_DMA_SYS
    LINK_SYMS       usbXhcdInit
}

#ifdef    _WRS_CONFIG_LP64
Component INCLUDE_USB_XHCI_HCD {
    REQUIRES        += INCLUDE_CACHE_DMA32_LIB
}
#endif    /* _WRS_CONFIG_LP64 */

Component INCLUDE_USB_XHCI_HCD_INIT {
    NAME            xHCI HCD Init
    SYNOPSIS        Initializes USB 3.0 xHCI HCD Driver
    _CHILDREN       FOLDER_USB_HOST_INIT
    REQUIRES        INCLUDE_USB_INIT    \
                    INCLUDE_HCD_BUS     \
                    INCLUDE_USB_XHCI_HCD
    EXCLUDES        INCLUDE_USBTOOL
}

Component INCLUDE_SYNOPSYSHCI {
    NAME            SYNOPSYS HCI
    SYNOPSIS        Synopsys Host Controller Interface Instance
    MODULES         usbSynopsysHcdInitExit.o
    REQUIRES        INCLUDE_USB
    LINK_SYMS       usbSynopsysHcdInit
}

Component INCLUDE_SYNOPSYSHCI_INIT {
    NAME            Synopsys HCI Init
    SYNOPSIS        Initializes Synopsys USB HOST Controller Driver
    _CHILDREN       FOLDER_USB_HOST_INIT
    REQUIRES        INCLUDE_USB_INIT    \
                    INCLUDE_HCD_BUS     \
                    INCLUDE_SYNOPSYSHCI
    EXCLUDES        INCLUDE_USBTOOL
}


Component INCLUDE_USB_MHDRC_HCD {
    NAME            USB MHDRC HCD
    SYNOPSIS        Mentor Graphics USB Host Controller Driver
    MODULES         usbMhdrcHcd.o
    REQUIRES        INCLUDE_USB
    LINK_SYMS       usbMhdrcHcdInit
}

Component INCLUDE_USB_MHDRC_HCD_INIT {
    NAME            USB MHDRC HCD Init
    SYNOPSIS        Initializes USB MHDRC HCD Driver
    _CHILDREN       FOLDER_USB_HOST_INIT
    REQUIRES        INCLUDE_USB_INIT \
                    INCLUDE_HCD_BUS \
                    INCLUDE_USB_MHDRC_HCD
    EXCLUDES        INCLUDE_USBTOOL
}

Component INCLUDE_HCD_BUS {
    NAME            USB Host Controller Start
    SYNOPSIS        Register USB Host Controller to vxBus
    REQUIRES        INCLUDE_USB_INIT    \
                    INCLUDE_USB
    EXCLUDES        INCLUDE_USBTOOL
    PROTOTYPE       void vxbUsbHciRegister (void);
    INIT_RTN        vxbUsbHciRegister();
    _INIT_ORDER     hardWareInterFaceBusInit
}

Component INCLUDE_USB_MHDRC_OCD {
    NAME            USB MHDRC OTG Controller Driver
    SYNOPSIS        Registers USB MHDRC OCD with vxBus
    MODULES         usbMhdrcOcd.o
    REQUIRES        INCLUDE_USB_OTG
    EXCLUDES        INCLUDE_USBTOOL
    LINK_SYMS       vxbUsbMhdrcOcdRegister
}

Folder FOLDER_USB_MHDRC_OCD_INIT {
    NAME            USB MHDRC OTG Controller Driver Init
    SYNOPSIS        USB MHDRC OTG Controller Driver Initialization
    _CHILDREN       FOLDER_USB_OTG_CONTROLLER_INIT
    CHILDREN        INCLUDE_USB_MHDRC_OCD_HCD_INIT \
                    INCLUDE_USB_MHDRC_OCD_TCD_INIT
}

Component INCLUDE_USB_MHDRC_OCD_INIT {
    NAME            USB MHDRC OTG OCD Init
    SYNOPSIS        USB MHDRC OTG OCD Initialization
    REQUIRES        INCLUDE_USB_MHDRC_OCD \
                    INCLUDE_USB_INIT
    EXCLUDES        INCLUDE_USBTOOL
}

Component INCLUDE_USB_MHDRC_OCD_HCD_INIT {
    NAME            USB MHDRC OTG OCD and HCD Init
    SYNOPSIS        USB MHDRC OTG OCD and HCD Initialization
    REQUIRES        INCLUDE_USB_MHDRC_OCD \
                    INCLUDE_USB_MHDRC_HCD \
                    INCLUDE_HCD_BUS       \
                    INCLUDE_USB_MHDRC_OCD_INIT
    EXCLUDES        INCLUDE_USBTOOL \
                    INCLUDE_USB_MHDRC_HCD_INIT \
                    INCLUDE_USB_MHDRC_TCD_INIT
    CONFIGLETTES    usrUsbInit.c
}

Component INCLUDE_USB_MHDRC_OCD_TCD_INIT {
    NAME            USB MHDRC OTG OCD and TCD Init
    SYNOPSIS        USB MHDRC OTG OCD and TCD Initialization
    REQUIRES        INCLUDE_USB_MHDRC_OCD \
                    INCLUDE_USB_MHDRC_TCD \
                    INCLUDE_TCD_BUS       \
                    INCLUDE_USB_MHDRC_OCD_INIT
    EXCLUDES        INCLUDE_USBTOOL \
                    INCLUDE_USB_MHDRC_TCD_INIT \
                    INCLUDE_USB_MHDRC_HCD_INIT
    CONFIGLETTES    usrUsbInit.c
}

Component INCLUDE_USB_OTG {
    NAME            USB OTG Driver Framework
    SYNOPSIS        USB OTG Driver Framework
    MODULES         usbOtg.o
    REQUIRES        INCLUDE_USB
    EXCLUDES        INCLUDE_USBTOOL
    _INIT_ORDER     usrRoot
    CONFIGLETTES    usrUsbInit.c
    PROTOTYPE       void usrUsbOtgInit(void);
    INIT_RTN        usrUsbOtgInit ();
    INIT_AFTER      usrIosCoreInit
    INIT_BEFORE     usrToolsInit
    CFG_PARAMS      USB_OTG_MGR_BASE_NAME \
                    USB_OTG_MGR_TASK_PRIORITY \
                    USB_OTG_MGR_EVENT_WAIT_TIME \
                    USB_OTG_HNP_POLLING_INTERVAL
}

Parameter USB_OTG_MGR_BASE_NAME {
    NAME            USB OTG manager iosLib base name
    SYNOPSIS        Device iosLib base name associated to USB OTG Controller (max 20 bytes)
    DEFAULT         "/usbOtg"
}

Parameter USB_OTG_MGR_TASK_PRIORITY {
    NAME            USB OTG manager task priority
    SYNOPSIS        USB OTG manager task priority (min 5, max 50)
    TYPE            int
    DEFAULT         5
}

Parameter USB_OTG_MGR_EVENT_WAIT_TIME {
    NAME            USB OTG manager event notification wait timeout
    SYNOPSIS        USB OTG manager event notification wait timeout (in ms, min 500, max 5000)
    TYPE            int
    DEFAULT         1000
}

Parameter USB_OTG_HNP_POLLING_INTERVAL {
    NAME            USB OTG HNP polling interval
    SYNOPSIS        USB OTG HNP polling interval (in ms, min 1000, max 2000)
    TYPE            int
    DEFAULT         1500
}

Component INCLUDE_USB_INIT {
    NAME            USB Common Stack Init
    SYNOPSIS        USB Common Stack Initialization
    REQUIRES        INCLUDE_USB
    EXCLUDES        INCLUDE_USBTOOL
    CONFIGLETTES    usrUsbInit.c
    PROTOTYPE       STATUS usbInit (void);
    INIT_RTN        usbInit ();
}

Component INCLUDE_USBTOOL {
    NAME            usbTool
    SYNOPSIS        USB Tool - Test Application for Host Stack
    REQUIRES        INCLUDE_USB
    _CHILDREN       FOLDER_USB_TOOL
    CONFIGLETTES    usrUsbTool.c
    EXCLUDES        INCLUDE_USB_INIT                \
                    INCLUDE_UHCI_INIT               \
                    INCLUDE_OHCI_INIT               \
                    INCLUDE_EHCI_INIT               \
                    INCLUDE_SYNOPSYSHCI_INIT        \
                    INCLUDE_USB_MHDRC_HCD_INIT      \
                    INCLUDE_USB_MOUSE_INIT          \
                    INCLUDE_USB_KEYBOARD_INIT       \
                    INCLUDE_USB_PRINTER_INIT        \
                    INCLUDE_USB_SPEAKER_INIT        \
                    INCLUDE_USB_MS_BULKONLY_INIT    \
                    INCLUDE_USB_MS_CBI_INIT         \
                    INCLUDE_USB_PEGASUS_END_INIT    \
                    INCLUDE_USB_AUDIO_DEMO          \
                    INCLUDE_KBD_EMULATOR_INIT       \
                    INCLUDE_PRN_EMULATOR_INIT       \
                    INCLUDE_MS_EMULATOR_INIT        \
                    INCLUDE_SER_EMULATOR_INIT       \
                    INCLUDE_USBTGT_KBD_INIT         \
                    INCLUDE_USBTGT_PRN_INIT         \
                    INCLUDE_USBTGT_MSC_INIT         \
                    INCLUDE_USBTGT_SER_INIT         \
                    INCLUDE_USBTGT_NETWORK_INIT     \
                    INCLUDE_USBTGT_RNDIS_INIT       \
                    INCLUDE_USBTGT_FUNCDEMO_INIT

}

Component INCLUDE_USB_GEN2_PRINTER {
    NAME            USB GEN2 Printer
    SYNOPSIS        USB Generation 2 Printer Class Driver
    MODULES         usb2Prn.o
    REQUIRES        INCLUDE_USB    \
                    INCLUDE_USB_GEN2_HELPER
    EXCLUDES        INCLUDE_USB_PRINTER \
                    INCLUDE_USBTOOL

}

Component INCLUDE_USB_GEN2_PRINTER_INIT {
    NAME            USB GEN2 Printer Init
    SYNOPSIS        USB GEN2 Printer Driver Initialization
    REQUIRES        INCLUDE_USB_GEN2_PRINTER        \
                    INCLUDE_USB_INIT                \
                    INCLUDE_USB_HOST_CLASS_INIT
    EXCLUDES        INCLUDE_USBTOOL                 \
                    INCLUDE_USB_PRINTER_INIT
    CFG_PARAMS      USB_GEN2_PRINTER_NAME
}

Parameter USB_GEN2_PRINTER_NAME {
    NAME            USB GEN2 Printer driver base name
    SYNOPSIS        Device base name associated to the GEN2 Printer device
    DEFAULT         "/usb2Prn/"
}

Component INCLUDE_USB_GEN2_STORAGE {
    NAME            USB GEN2 Mass Storage
    SYNOPSIS        USB Generation 2 Mass Storage Class Driver
    MODULES         usb2Msc.o usb2MscBBB.o usb2MscCBI.o usb2MscBLK.o usb2MscXBD.o \
                    usb2MscCommandSet.o usb2MscDirectAccess.o usb2MscCompliance.o
    REQUIRES        INCLUDE_USB             \
                    INCLUDE_USB_GEN2_HELPER \
                    INCLUDE_ERF             \
                    INCLUDE_XBD             \
                    INCLUDE_FS_MONITOR
    EXCLUDES        INCLUDE_USB_MS_BULKONLY \
                    INCLUDE_USB_MS_CBI      \
                    INCLUDE_USBTOOL
    CFG_PARAMS      USB2_MSC_FS_INTERFACE_MODE \
                    USB2_MSC_DIRECT_ACCESS_ENABLE \
                    USB2_MSC_DYNAMIC_CONFIG \
                    USB2_MSC_COMPLIANCE_ENABLE \
                    USB2_MSC_CBI_TRANSPORT_ENABLE \
                    USB2_MSC_QUIRKY_DEV_SUPPORT_LEVEL
}

Parameter USB2_MSC_DYNAMIC_CONFIG {
    NAME            USB GEN2 Mass Storage driver dynamic configuration
    SYNOPSIS        USB GEN2 Mass Storage driver dynamic configuration support
    TYPE            bool
    DEFAULT         FALSE
}

Parameter USB2_MSC_FS_INTERFACE_MODE {
    NAME            USB GEN2 Mass Storage driver filesystem interface mode
    SYNOPSIS        Filesystem interface mode (USB2_MSC_FS_INTERFACE_XBD or USB2_MSC_FS_INTERFACE_BLK_DEV) to use for the GEN2 Mass Storage device
    TYPE            int
    DEFAULT         USB2_MSC_FS_INTERFACE_XBD
}

Parameter USB2_MSC_DIRECT_ACCESS_ENABLE {
    NAME            USB GEN2 Mass Storage driver direct access enable flag
    SYNOPSIS        Flag to enable or disable direct access module of the GEN2 Mass Storage driver
    TYPE            bool
    DEFAULT         FALSE
}

Parameter USB2_MSC_COMPLIANCE_ENABLE {
    NAME            USB GEN2 Mass Storage driver compliance test module enable flag
    SYNOPSIS        Flag to enable or disable compliance test module of the GEN2 Mass Storage driver
    TYPE            bool
    DEFAULT         FALSE
}

Parameter USB2_MSC_CBI_TRANSPORT_ENABLE {
    NAME            USB GEN2 Mass Storage driver CB[I] transport module enable flag
    SYNOPSIS        Flag to enable or disable CB[I] transport module of the GEN2 Mass Storage driver
    TYPE            bool
    DEFAULT         TRUE
}

Parameter USB2_MSC_QUIRKY_DEV_SUPPORT_LEVEL {
    NAME            USB GEN2 Mass Storage quirky device global support level
    SYNOPSIS        USB GEN2 Mass Storage quirky device global support level(0:No support, 1:Relaxed support, 2:Support all)
    TYPE            int
    DEFAULT         1
}

Component INCLUDE_USB_GEN2_STORAGE_INIT {
    NAME            USB GEN2 Mass Storage Init
    SYNOPSIS        USB GEN2 Mass Storage Driver Initialization
    CONFIGLETTES    usrUsb2MscInit.c
    REQUIRES        INCLUDE_USB_GEN2_STORAGE \
                    INCLUDE_USB_INIT
    EXCLUDES        INCLUDE_USBTOOL \
                    INCLUDE_USB_MS_BULKONLY_INIT \
                    INCLUDE_USB_MS_CBI_INIT
    PROTOTYPE       STATUS usrUsb2MscInit (void);
    INIT_RTN        usrUsb2MscInit ();
    _INIT_ORDER     usrIosExtraInit
    INIT_AFTER      INCLUDE_VRFS
    INIT_BEFORE     INCLUDE_BOOT_LINE_INIT
    CFG_PARAMS      USB2_MSC_DRIVE_NAME \
                    USB2_MSC_MAX_NAME_SZ \
                    USB2_MSC_SERVICE_TASK_NAME \
                    USB2_MSC_MAX_DEVICES \
                    USB2_MSC_MAX_DRIVES \
                    USB2_MSC_DIRECT_ACCESS_BASE_NAME \
                    USB2_MSC_COMPLIANCE_LOG_FILE_BASE_NAME \
                    USB2_MSC_MEDIUM_HOTPLUG_CHECK_ENABLE \
                    USB2_MSC_WRITE_PROTECTION_CHECK_ENABLE \
                    USB2_MSC_MEDIUM_HOTPLUG_CHECK_INTERVAL \
                    USB2_MSC_STATUS_CHECK_ENABLE \
                    USB2_MSC_RESET_TIMEOUT \
                    USB2_MSC_COMMAND_TIMEOUT \
                    USB2_MSC_STATUS_TIMEOUT \
                    USB2_MSC_MAX_TRANSFER_BLOCKS \
                    USB2_MSC_SERVICE_TASK_PRIORITY \
                    USB2_MSC_SERVICE_TASK_OPTIONS \
                    USB2_MSC_SERVICE_TASK_STACK_SIZE
}

Parameter USB2_MSC_DRIVE_NAME {
    NAME            USB GEN2 Mass Storage driver drive base name
    SYNOPSIS        Drive base name associated to the GEN2 Mass Storage device
    TYPE            char *
    DEFAULT         "/bd"
}

Parameter USB2_MSC_MAX_NAME_SZ {
    NAME            USB GEN2 Mass Storage driver drive base name max length
    SYNOPSIS        Max length of drive base name associated to the GEN2 Mass Storage device
    TYPE            int
    DEFAULT         50
}

Parameter USB2_MSC_SERVICE_TASK_NAME {
    NAME            USB GEN2 Mass Storage driver XBD service task base name
    SYNOPSIS        XBD service task base name associated to the GEN2 Mass Storage device
    TYPE            char *
    DEFAULT         "tUsb2Msc"
}

Parameter USB2_MSC_MAX_DEVICES {
    NAME            USB GEN2 Mass Storage driver max devices
    SYNOPSIS        Max number of devices supported by the GEN2 Mass Storage driver
    TYPE            int
    DEFAULT         10
}

Parameter USB2_MSC_MAX_DRIVES {
    NAME            USB GEN2 Mass Storage driver max LUNs per device
    SYNOPSIS        Max count of LUNs to be supported by the GEN2 Mass Storage driver
    TYPE            int
    DEFAULT         16
}

Parameter USB2_MSC_DIRECT_ACCESS_BASE_NAME {
    NAME            USB GEN2 Mass Storage driver direct access base name
    SYNOPSIS        Direct access base name associated to the GEN2 Mass Storage device
    TYPE            char *
    DEFAULT         "/usb2Msc"
}

Parameter USB2_MSC_COMPLIANCE_LOG_FILE_BASE_NAME {
    NAME            USB GEN2 Mass Storage driver compliance test output file name
    SYNOPSIS        Compliance test log file name associated to the GEN2 Mass Storage device
    TYPE            char *
    DEFAULT         "stdout"
}

Parameter USB2_MSC_MEDIUM_HOTPLUG_CHECK_ENABLE {
    NAME            USB GEN2 Mass Storage driver medium hotplug check enable flag
    SYNOPSIS        Flag to enable or disable medium hotplug check of the GEN2 Mass Storage driver
    TYPE            bool
    DEFAULT         TRUE
}

Parameter USB2_MSC_MEDIUM_HOTPLUG_CHECK_INTERVAL {
    NAME            USB GEN2 Mass Storage driver medium hotplug check interval (in ms)
    SYNOPSIS        Medium hotplug check interval(in ms) for the GEN2 Mass Storage driver
    TYPE            int
    DEFAULT         2000
}

Parameter USB2_MSC_RESET_TIMEOUT {
    NAME            USB GEN2 Mass Storage driver reset timeout (in ms)
    SYNOPSIS        Reset timeout (in ms) for the GEN2 Mass Storage driver
    TYPE            int
    DEFAULT         2000
}

Parameter USB2_MSC_COMMAND_TIMEOUT {
    NAME            USB GEN2 Mass Storage driver command timeout (in ms)
    SYNOPSIS        Command timeout (in ms) for the GEN2 Mass Storage driver
    TYPE            int
    DEFAULT         2000
}

Parameter USB2_MSC_STATUS_TIMEOUT {
    NAME            USB GEN2 Mass Storage driver status timeout (in ms)
    SYNOPSIS        Status timeout (in ms) for the GEN2 Mass Storage driver
    TYPE            int
    DEFAULT         2000
}

Parameter USB2_MSC_STATUS_CHECK_ENABLE {
    NAME            USB GEN2 Mass Storage driver status check enable flag
    SYNOPSIS        Flag to enable or disable status check of the GEN2 Mass Storage driver
    TYPE            bool
    DEFAULT         TRUE
}

Parameter USB2_MSC_WRITE_PROTECTION_CHECK_ENABLE {
    NAME            USB GEN2 Mass Storage driver write protection check enable flag
    SYNOPSIS        Flag to enable or disable write protection check of the GEN2 Mass Storage driver
    TYPE            bool
    DEFAULT         FALSE
}

Parameter USB2_MSC_MAX_TRANSFER_BLOCKS {
    NAME            USB GEN2 Mass Storage driver maximum blocks per transfer
    SYNOPSIS        Maximum blocks per transfer for the GEN2 Mass Storage driver
    TYPE            int
    DEFAULT         64
}

Parameter USB2_MSC_SERVICE_TASK_PRIORITY {
    NAME            USB GEN2 Mass Storage driver XBD service task priority
    SYNOPSIS        XBD service task priority associated to the GEN2 Mass Storage device
    TYPE            int
    DEFAULT         150
}

Parameter USB2_MSC_SERVICE_TASK_OPTIONS {
    NAME            USB GEN2 Mass Storage driver XBD service task options
    SYNOPSIS        XBD service task options associated to the GEN2 Mass Storage device
    TYPE            int
    DEFAULT         0
}

Parameter USB2_MSC_SERVICE_TASK_STACK_SIZE {
    NAME            USB GEN2 Mass Storage driver XBD service task stack size
    SYNOPSIS        XBD service task stack size associated to the GEN2 Mass Storage device
    TYPE            int
    DEFAULT         4096
}

Component INCLUDE_USB_GEN2_MOUSE {
    NAME            USB GEN2 Mouse
    SYNOPSIS        USB Generation 2 Mouse Class Driver
    MODULES         usb2Mse.o
    REQUIRES        INCLUDE_USB    \
                    INCLUDE_USB_GEN2_HELPER
    EXCLUDES        INCLUDE_USB_MOUSE \
                    INCLUDE_USBTOOL

}

Component INCLUDE_USB_GEN2_MOUSE_INIT {
    NAME            USB GEN2 Mouse Init
    SYNOPSIS        USB GEN2 Mouse Driver Initialization
    REQUIRES        INCLUDE_USB_GEN2_MOUSE \
                    INCLUDE_USB_HOST_CLASS_INIT \
                    INCLUDE_USB_INIT
    EXCLUDES        INCLUDE_USBTOOL \
                    INCLUDE_USB_MOUSE_INIT
    CFG_PARAMS      USB_GEN2_MOUSE_NAME
}

Parameter USB_GEN2_MOUSE_NAME {
    NAME            USB GEN2 Mouse driver base name
    SYNOPSIS        Device base name associated to the GEN2 mouse device
    DEFAULT         "/usb2Mse/"
}


Component INCLUDE_USB_GEN2_KEYBOARD {
    NAME            USB GEN2 Keyboard
    SYNOPSIS        USB Generation 2 Keyboard Class Driver
    MODULES         usb2Kbd.o
    REQUIRES        INCLUDE_USB    \
                    INCLUDE_USB_GEN2_HELPER
    EXCLUDES        INCLUDE_USB_KEYBOARD \
                    INCLUDE_USBTOOL
}

Folder  FOLDER_USB_GEN2_KEYBOARD_INIT {
    NAME            USB GEN2 Keyboard Initialization
    SYNOPSIS        USB GEN2 Keyboard Driver Initialization
    _CHILDREN       FOLDER_USB_GEN2_DEVICES_INIT
    CHILDREN        INCLUDE_USB_GEN2_KEYBOARD_INIT          \
                    INCLUDE_USB_GEN2_KEYBOARD_SHELL_ATTACH  \
                    INCLUDE_USB_GEN2_KEYBOARD_BOOTSHELL_ATTACH
}

Component INCLUDE_USB_GEN2_KEYBOARD_INIT {
    NAME            USB GEN2 Keyboard Init
    SYNOPSIS        USB GEN2 Keyboard Driver Initialization
    REQUIRES        INCLUDE_USB_GEN2_KEYBOARD \
                    INCLUDE_USB_HOST_CLASS_INIT \
                    INCLUDE_USB_INIT
    EXCLUDES        INCLUDE_USBTOOL \
                    INCLUDE_USB_KEYBOARD_INIT
    CFG_PARAMS      USB_GEN2_KEYBOARD_NAME USB_GEN2_KEYBOARD_TYPEMATIC_MS
}

Parameter USB_GEN2_KEYBOARD_NAME {
    NAME            USB GEN2 keyboard driver base name
    SYNOPSIS        Device base name associated to the GEN2 keyboard device
    DEFAULT         "/usb2Kbd/"
}

Parameter USB_GEN2_KEYBOARD_TYPEMATIC_MS {
    NAME            USB GEN2 keyboard Typematic Rate in ms
    SYNOPSIS        USB GEN2 keyboard Typematic Rate in ms\
                    Defines how often the current pressed key will be sent    \
                    to the application (ASCII MODE). -1 is FOREVER, any other \
                    integer is the time in ms
    DEFAULT         128
}

Component INCLUDE_USB_GEN2_KEYBOARD_SHELL_ATTACH {
    NAME            USB GEN2 keyboard attaching to vxWorks Shell
    SYNOPSIS        Attaches the USB keyboard to the vxWorks target shell
    REQUIRES        INCLUDE_USB_GEN2_KEYBOARD_INIT   \
                    INCLUDE_SHELL
    EXCLUDES        INCLUDE_USB_GEN2_KEYBOARD_BOOTSHELL_ATTACH
}

Component INCLUDE_USB_GEN2_KEYBOARD_BOOTSHELL_ATTACH {
    NAME            USB GEN2 keyboard attaching to bootAppShell (only for bootrom)
    SYNOPSIS        Attaches the USB keyboard to the bootAppShell
    REQUIRES        INCLUDE_USB_GEN2_KEYBOARD_INIT \
                    INCLUDE_PC_CONSOLE
    EXCLUDES        INCLUDE_USB_GEN2_KEYBOARD_SHELL_ATTACH
}


Folder  FOLDER_USB_GEN2_END_CONTROLLERS {
    NAME            USB GEN2 END Controllers
    SYNOPSIS        USB Generation 2 END Class Controllers
    CHILDREN        INCLUDE_USB_GEN2_PEGASUS      \
                    INCLUDE_USB_GEN2_DM960X       \
                    INCLUDE_USB_GEN2_ASIX
}

Component INCLUDE_USB_GEN2_PEGASUS {
    NAME            Pegasus Serial Controllers
    SYNOPSIS        USB Generation 2 Pegasus serial controllers
    MODULES         usb2Pgs.o usb2End.o
    REQUIRES        INCLUDE_USB                \
                    INCLUDE_USB_GEN2_HELPER
    EXCLUDES        INCLUDE_USB_PEGASUS        \
                    INCLUDE_USBTOOL
}

Component INCLUDE_USB_GEN2_DM960X {
    NAME            Dm960x Serial Controllers
    SYNOPSIS        USB Generation 2 Dm960x serial controllers
    MODULES         usb2Dm960x.o usb2End.o
    REQUIRES        INCLUDE_USB \
                    INCLUDE_USB_GEN2_HELPER
    EXCLUDES        INCLUDE_USBTOOL
}


Component INCLUDE_USB_GEN2_ASIX {
    NAME            Asix Serial Controllers
    SYNOPSIS        USB Generation 2 Asix  serial controllers
    MODULES         usb2Asix.o usb2End.o
    REQUIRES        INCLUDE_USB \
                    INCLUDE_USB_GEN2_HELPER
    EXCLUDES        INCLUDE_USBTOOL
}

Component INCLUDE_USB_GEN2_END_INIT {
    NAME            USB GEN2 END Device Init
    SYNOPSIS        USB GEN2 END Device Component Initialization
    REQUIRES        INCLUDE_USB_INIT \
                    INCLUDE_USB_HOST_CLASS_INIT
    EXCLUDES        INCLUDE_USBTOOL \
                    INCLUDE_USB_PEGASUS_END_INIT
    CFG_PARAMS      USB_GEN2_END_NAME       \
                    USB_GEN2_END_IP_ADDRESS \
                    USB_GEN2_END_NET_MASK   \
                    USB_GEN2_END_MAX_DEVS   \
                    USB_GEN2_END_COMMON_TASK_PRIORITY
}

Parameter  USB_GEN2_END_IP_ADDRESS {
    NAME            USB GEN2 END Device IP Address
    SYNOPSIS        USB GEN2 END Device IP Address
    DEFAULT         {"90.0.0.50"}
}

Parameter  USB_GEN2_END_NET_MASK {
    NAME            USB GEN2 END Device Net Mask
    SYNOPSIS        USB GEN2 END Device Net Mask
    DEFAULT         {0xffff0000}
}

Parameter USB_GEN2_END_NAME {
    NAME            USB GEN2 End driver base name
    SYNOPSIS        Drive Base Name associated to the GEN2 End device
    DEFAULT         "usb2End"
}


Parameter  USB_GEN2_END_MAX_DEVS {
    NAME            USB GEN2 End Device maximum devices
    SYNOPSIS        USB GEN2 End Device maximum devices
    DEFAULT         1
}

Parameter USB_GEN2_END_COMMON_TASK_PRIORITY {
    NAME            USB GEN2 End Common Task priority
    SYNOPSIS        USB GEN2 End Common Task priority
    DEFAULT         100
}

Folder  FOLDER_USB_GEN2_SERIAL_INIT {
    NAME            USB GEN2 Serial Device Initialization
    SYNOPSIS        USB GEN2 Serial Device Driver Initialization
    _CHILDREN       FOLDER_USB_GEN2_DEVICES_INIT
    CHILDREN        INCLUDE_USB_GEN2_SERIAL_INIT            \
                    INCLUDE_USB_GEN2_SERIAL_PCCONSOLE_INIT
}

Component INCLUDE_USB_GEN2_SERIAL_INIT {
    NAME            USB GEN2 Serial Device Init
    SYNOPSIS        USB GEN2 USB-to-Serial Device Component Initialization
    REQUIRES        INCLUDE_USB_INIT \
                    INCLUDE_USB_HOST_CLASS_INIT
    EXCLUDES        INCLUDE_USBTOOL
    CFG_PARAMS      USB_GEN2_SERIAL_NAME                  \
                    USB_GEN2_SERIAL_DEFAULT_BAUDRATE      \
                    USB_GEN2_SERIAL_IOS_RING_BUFFER_SIZE  \
                    USB_GEN2_SERIAL_TIME_OUT              \
                    USB_GEN2_SERIAL_COMMON_TASK_PRIORITY
}

Parameter  USB_GEN2_SERIAL_NAME {
    NAME            USB GEN2 Serial Device Name
    SYNOPSIS        USB GEN2 Serial Device Name
    DEFAULT         "/usb2ttyS/"
}

Parameter  USB_GEN2_SERIAL_DEFAULT_BAUDRATE {
    NAME            USB GEN2 Serial Default Baud Rate
    SYNOPSIS        USB GEN2 Serial Default Baud Rate
    DEFAULT         9600
}

Parameter  USB_GEN2_SERIAL_IOS_RING_BUFFER_SIZE {
    NAME            USB GEN2 Serial IOS Ring Buffer Size
    SYNOPSIS        USB GEN2 Serial IOS Ring Buffer Size
    DEFAULT         8192
}

Parameter  USB_GEN2_SERIAL_TIME_OUT {
    NAME            USB GEN2 Serial Wait Timeout(in ms)
    SYNOPSIS        USB GEN2 Serial Wait Timeout(in ms)
    DEFAULT         1000
}

Parameter  USB_GEN2_SERIAL_COMMON_TASK_PRIORITY {
    NAME            USB GEN2 Serial Common Task Priority
    SYNOPSIS        USB GEN2 Serial Common Task Priority
    DEFAULT         150
}

Component INCLUDE_USB_GEN2_SERIAL_PCCONSOLE_INIT {
    NAME            USB GEN2 Serial Device Attach to PC Console Init
    SYNOPSIS        USB GEN2 USB-to-Serial Device Attach to PC Console
    REQUIRES        INCLUDE_USB_INIT
    EXCLUDES        INCLUDE_USBTOOL
    INIT_RTN        usbPcConVxbRegister();
    CONFIGLETTES    usrUsb2SerialPcConInit.c
    MODULES         usb2SerialPcConsole.o
    _INIT_ORDER     usrRoot
    INIT_BEFORE     usrIosCoreInit
    INIT_AFTER      usrRtpInit
    CFG_PARAMS      USB_GEN2_SERIAL_CONSOLE_DEVICE_NAME       \
                    USB_GEN2_SERIAL_PCCON_ATTACH_DEVICE_NAME  \
                    USB_GEN2_SERIAL_PCCON_DEFAULT_BAUDRATE    \
                    USB_GEN2_SERIAL_PCCON_DUMMY_BUFFER_SIZE   \
                    USB_GEN2_SERIAL_PCCON_OUT_BUFFER_SIZE     \
                    USB_GEN2_SERIAL_PCCON_TASK_PRIORITY       \
                    USB_GEN2_SER_MAX_CONSOLE
}

Parameter  USB_GEN2_SERIAL_CONSOLE_DEVICE_NAME {
    NAME            USB GEN2 Serial PC Console Attached Device Name
    SYNOPSIS        USB GEN2 Serial PC Console Attached Device Name
    DEFAULT         "/usbPcCon/"
}

Parameter  USB_GEN2_SERIAL_PCCON_ATTACH_DEVICE_NAME {
    NAME            USB GEN2 Serial PC Console Attached Device Name
    SYNOPSIS        USB GEN2 Serial PC Console Attached Device Name
    DEFAULT         "/usb2ttyS/"
}

Parameter  USB_GEN2_SERIAL_PCCON_DEFAULT_BAUDRATE {
    NAME            USB GEN2 Serial PC Console Baud Rate
    SYNOPSIS        USB GEN2 Serial PC Console Baud Rate
    DEFAULT         9600
}

Parameter  USB_GEN2_SERIAL_PCCON_DUMMY_BUFFER_SIZE {
    NAME            USB GEN2 Serial PC Console Dummy Buffer Size
    SYNOPSIS        USB GEN2 Serial PC Console Dummy Buffer Size
    DEFAULT         4096
}

Parameter  USB_GEN2_SERIAL_PCCON_OUT_BUFFER_SIZE {
    NAME            USB GEN2 Serial PC Console Out Buffer Size
    SYNOPSIS        USB GEN2 Serial PC Console Out Buffer Size, Must Large Than TTY LIB Buffer(Default is 512)
    DEFAULT         1024
}

Parameter  USB_GEN2_SERIAL_PCCON_TASK_PRIORITY {
    NAME            USB GEN2 Serial PC Console TTY Task Priority
    SYNOPSIS        USB GEN2 Serial PC Console TTY Task Priority
    DEFAULT         100
}

Parameter  USB_GEN2_SER_MAX_CONSOLE {
    NAME            USB GEN2 Serial Max PC Console number
    SYNOPSIS        USB GEN2 Serial Max PC Console number(1 to 10)
    DEFAULT         1
}

Folder FOLDER_USB_GEN2_SER_ADAPTERS {
    NAME            USB GEN2 Serial Adapters Driver
    SYNOPSIS        USB Generation 2 USB to Serial Class Adapters
    CHILDREN        INCLUDE_USB_GEN2_SER_FTDI232    \
                    INCLUDE_USB_GEN2_SER_WRS_CDC    \
                    INCLUDE_USB_GEN2_SER_PL2303
}

Component INCLUDE_USB_GEN2_SER_FTDI232 {
    NAME            FTDI 232 Serial Adapter Driver
    SYNOPSIS        USB Generation 2 FTDI 232 Serial Adapter
    MODULES         usb2Serial.o usb2SerialFtdi232.o
    REQUIRES        INCLUDE_USB                 \
                    INCLUDE_USB_GEN2_HELPER     \
                    INCLUDE_USB_GEN2_SERIAL_INIT
    EXCLUDES        INCLUDE_USBTOOL
    CFG_PARAMS      USB_GEN2_SER_FTDI_POLL_INTERVAL

}

Parameter  USB_GEN2_SER_FTDI_POLL_INTERVAL {
    NAME            USB GEN2 Serial FTDI Polling Task Interval
    SYNOPSIS        USB GEN2 Serial FTDI Polling Task Interval(0 for no delay)
    DEFAULT         0
}

Component INCLUDE_USB_GEN2_SER_WRS_CDC {
    NAME            WRS USB CDC Serial Adapter Driver
    SYNOPSIS        USB Generation 2 Wind River Systems USB CDC SerialAdapter Driver
    MODULES         usb2Serial.o usb2SerialWrs.o
    REQUIRES        INCLUDE_USB                 \
                    INCLUDE_USB_GEN2_HELPER     \
                    INCLUDE_USB_GEN2_SERIAL_INIT
    EXCLUDES        INCLUDE_USBTOOL
    CFG_PARAMS      USB_GEN2_SER_WRSCDC_POLL_INTERVAL
}

Parameter  USB_GEN2_SER_WRSCDC_POLL_INTERVAL {
    NAME            USB GEN2 Serial WRS CDC Polling Task Interval
    SYNOPSIS        USB GEN2 Serial Wind River Systems USB CDC Polling Task Interval(0 for no delay)
    DEFAULT         0
}

Component INCLUDE_USB_GEN2_SER_PL2303 {
    NAME            Prolific 2303 Serial Adapter Driver
    SYNOPSIS        USB Generation 2 Prolific 2303 Serial Adapter Driver
    MODULES         usb2Serial.o usb2SerialPL2303.o
    REQUIRES        INCLUDE_USB                 \
                    INCLUDE_USB_GEN2_HELPER     \
                    INCLUDE_USB_GEN2_SERIAL_INIT
    EXCLUDES        INCLUDE_USBTOOL
    CFG_PARAMS      USB_GEN2_SER_PL2303_POLL_INTERVAL
}

Parameter  USB_GEN2_SER_PL2303_POLL_INTERVAL {
    NAME            USB GEN2 Serial Prolific 2303 Polling Task Interval
    SYNOPSIS        USB GEN2 Serial Prolific 2303 Polling Task Interval(0 for no delay)
    DEFAULT         0
}

Component INCLUDE_USB_AUDIO_DEMO {
    NAME            USB Audio Demo
    SYNOPSIS        USB Audio Demo for Host Stack
    REQUIRES        INCLUDE_USB_INIT            \
                    INCLUDE_USB_HOST_CLASS_INIT \
                    INCLUDE_USB_SPEAKER
    EXCLUDES        INCLUDE_USBTOOL             \
                    INCLUDE_USB_SPEAKER_INIT    \
                    INCLUDE_USB_HEADSET_DEMO
}

Component INCLUDE_USB_MOUSE {
    NAME            Mouse
    SYNOPSIS        USB Mouse Driver
    MODULES         usbMouseLib.o
    REQUIRES        INCLUDE_USB
}

Component INCLUDE_USB_MOUSE_INIT {
    NAME            Mouse Init
    SYNOPSIS        USB Mouse Driver Initialization
    REQUIRES        INCLUDE_USB_MOUSE \
                    INCLUDE_USB_HOST_CLASS_INIT \
                    INCLUDE_USB_INIT
    EXCLUDES        INCLUDE_USBTOOL
}

Component INCLUDE_USB_KEYBOARD {
    NAME            Keyboard
    SYNOPSIS        USB Keyboard Driver
    MODULES         usbKeyboardLib.o
    REQUIRES        INCLUDE_USB
}

Folder  FOLDER_USB_KEYBOARD_INIT {
    NAME            Keyboard Init
    SYNOPSIS        Keyboard Driver Init
    _CHILDREN       FOLDER_USB_GEN1_DEVICES_INIT
    CHILDREN        INCLUDE_USB_KEYBOARD_INIT \
                    INCLUDE_USB_KEYBOARD_SHELL_ATTACH \
                    INCLUDE_USB_KEYBOARD_BOOTSHELL_ATTACH
}

Component INCLUDE_USB_KEYBOARD_INIT {
    NAME            Keyboard Initialization
    SYNOPSIS        USB Keyboard Driver Initialization
    REQUIRES        INCLUDE_USB_KEYBOARD \
                    INCLUDE_USB_HOST_CLASS_INIT \
                    INCLUDE_USB_INIT
    EXCLUDES        INCLUDE_USBTOOL
    CFG_PARAMS      USB_KBD_QUEUE_SIZE   \
                    USB_MAX_KEYBOARDS
}

Parameter USB_KBD_QUEUE_SIZE {
    NAME            USB Keyboard Circular Queue Size
    SYNOPSIS        Maximum number of data bytes the queue can hold
    DEFAULT         8
}

Parameter USB_MAX_KEYBOARDS {
    NAME            USB Maximum number of keyboards
    SYNOPSIS        The largest number of simultaneous keyboards
    DEFAULT         5
}

Component INCLUDE_USB_KEYBOARD_SHELL_ATTACH {
    NAME            USB keyboard attaching to vxWorks Shell
    SYNOPSIS        Attaches the USB keyboard to the vxWorks target shell
    REQUIRES        INCLUDE_USB_KEYBOARD_INIT  \
                    INCLUDE_SHELL
    EXCLUDES        INCLUDE_USB_KEYBOARD_BOOTSHELL_ATTACH
}

Component INCLUDE_USB_KEYBOARD_BOOTSHELL_ATTACH {
    NAME            USB keyboard attaching to bootAppShell (only for bootrom)
    SYNOPSIS        Attaches the USB keyboard to the bootAppShell
    REQUIRES        INCLUDE_USB_KEYBOARD_INIT \
                    INCLUDE_PC_CONSOLE
    EXCLUDES        INCLUDE_USB_KEYBOARD_SHELL_ATTACH
}

Component INCLUDE_USB_PRINTER {
    NAME            Printer
    SYNOPSIS        USB printer Driver
    MODULES         usbPrinterLib.o
    REQUIRES        INCLUDE_USB
    EXCLUDES        INCLUDE_USB_GEN2_PRINTER
}

Component INCLUDE_USB_PRINTER_INIT {
    NAME            Printer Init
    SYNOPSIS        USB Printer Driver Initialization
    REQUIRES        INCLUDE_USB_PRINTER \
                    INCLUDE_USB_HOST_CLASS_INIT \
                    INCLUDE_USB_INIT
    EXCLUDES        INCLUDE_USBTOOL \
                    INCLUDE_USB_GEN2_PRINTER_INIT
}

Component INCLUDE_USB_SPEAKER {
    NAME            Speaker
    SYNOPSIS        USB Printer Driver
    MODULES         usbSpeakerLib.o
    REQUIRES        INCLUDE_USB
}

Component INCLUDE_USB_SPEAKER_INIT {
    NAME            Speaker Init
    SYNOPSIS        USB Speaker Driver Initialization
    REQUIRES        INCLUDE_USB_SPEAKER \
                    INCLUDE_USB_HOST_CLASS_INIT \
                    INCLUDE_USB_INIT
    EXCLUDES        INCLUDE_USBTOOL
}

Component INCLUDE_USB_HEADSET_DEMO {
    NAME            Headset Demo
    SYNOPSIS        USB Headset Demonstration
    REQUIRES        INCLUDE_USB_SPEAKER_INIT \
                    INCLUDE_USB_SPEAKER \
                    INCLUDE_USB_HOST_CLASS_INIT \
                    INCLUDE_USB_INIT
    EXCLUDES        INCLUDE_USBTOOL \
                    INCLUDE_USB_AUDIO_DEMO
    CFG_PARAMS      USB_BRCM_AUDIO_CALLBACK_PERIOD \
                    USB_BRCM_POLL_TASK_PRIORITY \
                    USB_BRCM_SPKR_NAME
}

Parameter USB_BRCM_AUDIO_CALLBACK_PERIOD {
    NAME            Streaming audio polling rate
    SYNOPSIS        The number of milliseconds in the streaming audio update
    DEFAULT         5
}

Parameter USB_BRCM_POLL_TASK_PRIORITY {
    NAME            Headset audio task priority
    SYNOPSIS        Priority of the Headset streaming audio update task
    DEFAULT         75
}

Parameter USB_BRCM_SPKR_NAME {
    NAME            Name of the Headset
    SYNOPSIS        Name of the Headset
    DEFAULT         "/usbSp/0"
}

Component INCLUDE_USB_MS_BULKONLY {
    NAME            Mass Storage - Bulk
    SYNOPSIS        Bulk Only Mass Storage USB Driver
    MODULES         usbBulkDevLib.o
    REQUIRES        INCLUDE_USB \
                    INCLUDE_ERF \
                    INCLUDE_XBD \
                    INCLUDE_FS_MONITOR
}

Component INCLUDE_USB_MS_BULKONLY_INIT {
    NAME            Bulk Mass Storage Init
    SYNOPSIS        Bulk Only Mass Storage USB Driver Initialization
    REQUIRES        INCLUDE_USB_MS_BULKONLY \
                    INCLUDE_USB_INIT
    EXCLUDES        INCLUDE_USBTOOL
    CONFIGLETTES    usrUsbBulkDevInit.c
    PROTOTYPE       STATUS usrUsbBulkDevInit (void);
    INIT_RTN        usrUsbBulkDevInit();
    _INIT_ORDER     usrIosExtraInit
    INIT_AFTER      INCLUDE_VRFS
    INIT_BEFORE     INCLUDE_BOOT_LINE_INIT
    CFG_PARAMS      BULK_DRIVE_NAME \
                    BULK_MAX_DEVS   \
                    BULK_MAX_DRV_NAME_SZ  \
                    USB_BULK_NON_REMOVABLE_DISK
}

Parameter BULK_MAX_DEVS {
    NAME            USB Bulk Maximum Drives
    SYNOPSIS        Maximum number of drives
    DEFAULT         2
}

Parameter BULK_DRIVE_NAME {
    NAME            USB Bulk Drive Name
    SYNOPSIS        Drive Name assigned to the USB Bulk only device
    DEFAULT         "/bd"
}

Parameter BULK_MAX_DRV_NAME_SZ {
    NAME            USB Bulk Device Name Size
    SYNOPSIS        Maximum size of USB Bulk Device Name
    DEFAULT         10
}

Parameter USB_BULK_NON_REMOVABLE_DISK {
    NAME            USB Bulk Device Non removable flag
    SYNOPSIS        Flag for disabling statusChk for non removable devices
    DEFAULT         FALSE
}

Component INCLUDE_USB_MS_CBI {
    NAME            Mass Storage - CBI
    SYNOPSIS        Control/Bulk/Interrupt - Mass Storage USB Driver
    MODULES         usbCbiUfiDevLib.o
    REQUIRES        INCLUDE_USB \
                    INCLUDE_ERF \
                    INCLUDE_XBD \
                    INCLUDE_FS_MONITOR
}

Component INCLUDE_USB_MS_CBI_INIT {
    NAME            CBI Mass Storage Init
    SYNOPSIS        Control/Bulk/Interrupt Mass Storage USB Driver \
                    Initialization
    REQUIRES        INCLUDE_USB_MS_CBI \
                    INCLUDE_USB_INIT
    EXCLUDES        INCLUDE_USBTOOL
    CONFIGLETTES    usrUsbCbiUfiDevInit.c
    PROTOTYPE       STATUS usrUsbCbiUfiDevInit (void);
    INIT_RTN        usrUsbCbiUfiDevInit ();
    _INIT_ORDER     usrIosExtraInit
    INIT_AFTER      INCLUDE_VRFS
    INIT_BEFORE     INCLUDE_BOOT_LINE_INIT
    CFG_PARAMS      CBI_DRIVE_NAME \
                    UFI_MAX_DEVS   \
                    UFI_MAX_DRV_NAME_SZ
}

Parameter CBI_DRIVE_NAME {
    NAME            USB CBI Drive Name
    SYNOPSIS        Drive Name assigned to the USB CBI device
    DEFAULT         "/cbid"
}
Parameter UFI_MAX_DEVS {
    NAME            Maximum UFI Devices
    SYNOPSIS        Maximum number of UFI devices
    DEFAULT         2
}

Parameter UFI_MAX_DRV_NAME_SZ {
    NAME            USB UFI Device Name Size
    SYNOPSIS        Maximum size of device name
    DEFAULT         10
}

Component INCLUDE_USB_PEGASUS_END {
    NAME            End - Pegasus
    SYNOPSIS        End - Pegasus USB Driver
    MODULES         usbPegasusEnd.o
    REQUIRES        INCLUDE_USB
}

Component INCLUDE_USB_PEGASUS_END_INIT {
    NAME            End - Pegasus IPv4 Initialization
    SYNOPSIS        End - Pegasus USB IPv4 Driver Initialization
    REQUIRES        INCLUDE_USB_PEGASUS_END \
                    INCLUDE_USB_HOST_CLASS_INIT \
                    INCLUDE_USB_INIT
    EXCLUDES        INCLUDE_USBTOOL
    CFG_PARAMS      PEGASUS_IP_ADDRESS  \
                    PEGASUS_NET_MASK    \
                    PEGASUS_TARGET_NAME \
                    PEGASUS_MAX_DEVS    \
                    PEGASUS_END_NAME
}

Parameter PEGASUS_MAX_DEVS {
    NAME            USB Pegasus Device maximum number
    SYNOPSIS        Maximum number of supported Pegasus devices
    DEFAULT         1
}

Parameter PEGASUS_IP_ADDRESS {
    NAME            Pegasus IP Address
    SYNOPSIS        USB Pegasus Device IP Address
    DEFAULT         {"90.0.0.50"}
}

Parameter PEGASUS_NET_MASK {
    NAME            Pegasus Net Mask
    SYNOPSIS        USB Pegasus Device Net Mask
    DEFAULT         {0xffff0000}
}

Parameter PEGASUS_TARGET_NAME {
    NAME            Pegasus Target Name
    SYNOPSIS        USB Pegasus Device Target Name
    DEFAULT         {"usbTarg0"}
}

Parameter PEGASUS_END_NAME {
    NAME            Pegasus End Name
    SYNOPSIS        USB Pegasus End Name
    DEFAULT         {"usb"}
}

Component INCLUDE_KBD_EMULATOR {
    NAME            Keyboard Emulator
    SYNOPSIS        USB Keyboard Emulator Firmware
    MODULES         usbTargKbdLib.o
    REQUIRES        INCLUDE_USB_TARG
}

Component INCLUDE_PRN_EMULATOR {
    NAME            Printer Emulator
    SYNOPSIS        USB Printer Emulator Firmware
    MODULES         usbTargPrnLib.o
    REQUIRES        INCLUDE_USB_TARG
}

Component INCLUDE_D12_EMULATOR {
    NAME            D12 Emulator
    SYNOPSIS        USB D12 Emulator Firmware
    MODULES         usbTargPhilipsD12EvalLib.o
    REQUIRES        INCLUDE_USB_TARG
}


Component INCLUDE_MS_EMULATOR {
    NAME            Mass Storage Emulator
    SYNOPSIS        USB Mass Storage Emulator Firmware
    MODULES         usbTargMsLib.o usbTargRbcLib.o usbTargRbcCmd.o
    REQUIRES        INCLUDE_USB_TARG
}

Component INCLUDE_SER_EMULATOR {
    NAME            CDC ACM Serial Emulator
    SYNOPSIS        USB CDC ACM Serial Emulator Firmware
    MODULES         usbTargSerLib.o
    REQUIRES        INCLUDE_USB_TARG
}

Component INCLUDE_KBD_EMULATOR_INIT {
    NAME            Targ Keyboard Emulator Init
    SYNOPSIS        Targ Keyboard Emulator Driver Initialization
    REQUIRES        INCLUDE_KBD_EMULATOR
    EXCLUDES        INCLUDE_USBTOOL
    CONFIGLETTES    usrUsbTargKbdInit.c
    PROTOTYPE       void usrUsbTargKbdInit (void);
    INIT_RTN        usrUsbTargKbdInit();
    _INIT_ORDER     usrRoot
    INIT_AFTER      usbHstClassInit
    INIT_BEFORE     usrAppInit
}

Component INCLUDE_PRN_EMULATOR_INIT {
    NAME            Targ Printer Emulator Init
    SYNOPSIS        Targ Printer Emulator Driver Initialization
    REQUIRES        INCLUDE_PRN_EMULATOR
    EXCLUDES        INCLUDE_USBTOOL
    CONFIGLETTES    usrUsbTargPrnInit.c
    PROTOTYPE       void usrUsbTargPrnInit (void);
    INIT_RTN        usrUsbTargPrnInit();
    _INIT_ORDER     usrRoot
    INIT_AFTER      usbHstClassInit
    INIT_BEFORE     usrAppInit
}
Component INCLUDE_MS_EMULATOR_INIT {
    NAME            Targ Mass Storage Emulator Init
    SYNOPSIS        Targ Mass Storage Emulator Driver Initialization
    REQUIRES        INCLUDE_MS_EMULATOR
    EXCLUDES        INCLUDE_USBTOOL
    CONFIGLETTES    usrUsbTargMsInit.c
    PROTOTYPE       void usbTargMsInit (void);
    INIT_RTN        usbTargMsInit();
    _INIT_ORDER     usrRoot
    INIT_AFTER      usbHstClassInit
    INIT_BEFORE     usrAppInit
}

Component INCLUDE_SER_EMULATOR_INIT {
    NAME            Targ CDC ACM Serial Emulator Init
    SYNOPSIS        Targ CDC ACM Serial Emulator Driver Initialization
    REQUIRES        INCLUDE_SER_EMULATOR
    EXCLUDES        INCLUDE_USBTOOL
    CONFIGLETTES    usrUsbTargSerInit.c
    PROTOTYPE       void usbTargSerInit (void);
    INIT_RTN        usbTargSerInit();
    _INIT_ORDER     usrRoot
    INIT_AFTER      usrRtpInit
    INIT_BEFORE     usrIosCoreInit
}

Component INCLUDE_PDIUSBD12 {
    NAME            PDIUSBD12
    SYNOPSIS        Target Controller Driver for PDIUSBD12
    MODULES         usbTcdPdiusbd12InitExit.o
    REQUIRES        INCLUDE_USB_TARG
}

Component INCLUDE_PHILIPS1582 {
    NAME            PhilipsIsp1582
    SYNOPSIS        Target Controller Driver for Philips Isp 1582
    MODULES         usbTcdIsp1582InitExit.o
    REQUIRES        INCLUDE_USB_TARG
    PROTOTYPE       void sysIsp1582PciDisconnect (void);
    INIT_RTN        sysIsp1582PciDisconnect ();
    _INIT_ORDER     usrRoot
    INIT_AFTER      usrRtpInit
    INIT_BEFORE     INCLUDE_KBD_EMULATOR_INIT       \
                    INCLUDE_PRN_EMULATOR_INIT       \
                    INCLUDE_MS_EMULATOR_INIT
}

Component INCLUDE_NET2280 {
    NAME            NetChip NET2280
    SYNOPSIS        Target Controller Driver for NetChip NET2280
    MODULES         usbTcdNET2280InitExit.o
    REQUIRES        INCLUDE_USB_TARG
}

Component INCLUDE_FSL_TCD {
    NAME            Freescale Dual Role USB controller
    SYNOPSIS        Target Controller Driver for Freescale Dual Role USB controller
    MODULES         usbTcdFslInitExit.o
    REQUIRES        INCLUDE_USB_TARG
}

Folder    FOLDER_USB_GEN2_TARGET {
    NAME            USB GEN2 Target
    SYNOPSIS        USB GEN2 Target Stack Components
    _CHILDREN       FOLDER_USB_TARG
    CHILDREN        FOLDER_USB_GEN2_TARGET_CONTROLLERS           \
                    FOLDER_USB_GEN2_TARGET_CONTROLLERS_INIT      \
                    FOLDER_USB_GEN2_TARGET_STACK                 \
                    FOLDER_USB_GEN2_TARGET_FUNCTION              \
                    FOLDER_USB_GEN2_TARGET_FUNCTION_INIT
}

Folder FOLDER_USB_GEN2_TARGET_CONTROLLERS {
    NAME            USB GEN2 Target Controllers
    SYNOPSIS        USB GEN2 Target Controllers
    _CHILDREN       FOLDER_USB_GEN2_TARGET
    CHILDREN        INCLUDE_USB_MHDRC_TCD \
                    INCLUDE_USB_PLX_TCD
}

Folder FOLDER_USB_GEN2_TARGET_CONTROLLERS_INIT {
    NAME            USB GEN2 Target Controllers Init
    SYNOPSIS        USB GEN2 Target Controllers Initialization
    _CHILDREN       FOLDER_USB_GEN2_TARGET
    CHILDREN        INCLUDE_USB_MHDRC_TCD_INIT \
                    INCLUDE_USB_PLX_TCD_INIT
}

Folder FOLDER_USB_GEN2_TARGET_STACK {
    NAME            USB GEN2 Target Stack
    SYNOPSIS        USB GEN2 Target stack hardware \
                    independent components
    _CHILDREN       FOLDER_USB_GEN2_TARGET
    CHILDREN        INCLUDE_USB_TGT
}

Folder FOLDER_USB_GEN2_TARGET_FUNCTION {
    NAME            USB GEN2 Target Function Driver
    SYNOPSIS        USB GEN2 Target Function Driver
    _CHILDREN       FOLDER_USB_GEN2_TARGET
    CHILDREN        FOLDER_USBTGT_KBD             \
                    FOLDER_USBTGT_PRN             \
                    FOLDER_USBTGT_MSC             \
                    INCLUDE_USBTGT_FUNC_BUS       \
                    INCLUDE_USBTGT_SER            \
                    FOLDER_USBTGT_NETWORK         \
                    INCLUDE_USBTGT_FUNCDEMO
}

Folder FOLDER_USB_GEN2_TARGET_FUNCTION_INIT {
    NAME            USB GEN2 Target Function Driver Init
    SYNOPSIS        USB GEN2 Target Function Driver Initialization
    _CHILDREN       FOLDER_USB_GEN2_TARGET
    CHILDREN        INCLUDE_USBTGT_KBD_INIT       \
                    INCLUDE_USBTGT_PRN_INIT       \
                    INCLUDE_USBTGT_MSC_INIT       \
                    INCLUDE_USBTGT_SER_INIT       \
                    FOLDER_USBTGT_NETWORK_INIT     \
                    INCLUDE_USBTGT_FUNCDEMO_INIT
}

Folder FOLDER_USBTGT_NETWORK {
    NAME            USB GEN2 Target Network Function Driver
    SYNOPSIS        USB GEN2 Target Network Function Driver
    _CHILDREN       FOLDER_USB_GEN2_TARGET_FUNCTION
    CHILDREN        INCLUDE_USBTGT_NETWORK            \
                    INCLUDE_USBTGT_VRTL_END \
                    SELECT_USBTGT_RNDIS
    DEFAULTS        INCLUDE_USBTGT_NETWORK  \
                    INCLUDE_USBTGT_VRTL_END \
                    SELECT_USBTGT_RNDIS
}

Folder FOLDER_USBTGT_NETWORK_INIT {
    NAME            USB GEN2 Target Network Function Driver Init
    SYNOPSIS        USB GEN2 Target Network Function Driver Initialization
    _CHILDREN       FOLDER_USB_GEN2_TARGET_FUNCTION_INIT
    CHILDREN        INCLUDE_USBTGT_NETWORK_INIT       \
                    INCLUDE_USBTGT_RNDIS_INIT
    DEFAULT         FOLDER_USBTGT_NETWORK
}


Component INCLUDE_USB_MHDRC_TCD {
    NAME            USB MHDRC TCD
    SYNOPSIS        Mentor Graphics USB Target Controller Driver
    MODULES         usbMhdrcTcd.o
    REQUIRES        INCLUDE_USB_TGT
    LINK_SYMS       usbVxbMhdrcTcdRegister
    CFG_PARAMS      USB_MHDRC_TCD_NAME           \
                    USB_MHDRC_TCD_VENDOR_ID      \
                    USB_MHDRC_TCD_PRODUCT_ID     \
                    USB_MHDRC_TCD_MFG_STRING     \
                    USB_MHDRC_TCD_PROD_STRING    \
                    USB_MHDRC_TCD_SERIAL_STRING
}

Parameter USB_MHDRC_TCD_NAME {
    NAME            USB Target Device Name
    SYNOPSIS        USB Target Device Name
    DEFAULT         {"/usbTgt"}
}

Parameter USB_MHDRC_TCD_VENDOR_ID {
    NAME            USB Target Device Vendor ID
    SYNOPSIS        USB Target Device Vendor ID
    DEFAULT         {0x1439}
}

Parameter USB_MHDRC_TCD_PRODUCT_ID {
    NAME            USB Target Device Product ID
    SYNOPSIS        USB Target Device Product ID
    DEFAULT         {0xFF00}
}

Parameter USB_MHDRC_TCD_MFG_STRING {
    NAME            USB Target Manufacture Name
    SYNOPSIS        USB Target Manufacture Name
    DEFAULT         {"Wind River Systems"}
}

Parameter USB_MHDRC_TCD_PROD_STRING {
    NAME            USB Target Product Name
    SYNOPSIS        USB Target Product Name
    DEFAULT         {"USB MHDRC Target Device"}
}

Parameter USB_MHDRC_TCD_SERIAL_STRING  {
    NAME            USB Target Product Serial Number
    SYNOPSIS        USB Target Product Serial Name
    DEFAULT         {"0123456789012"}
}

Component INCLUDE_USB_MHDRC_TCD_INIT {
    NAME            USB MHDRC TCD Init
    SYNOPSIS        Initializes USB MHDRC TCD Driver
    REQUIRES        INCLUDE_USB_INIT \
                    INCLUDE_TCD_BUS  \
                    INCLUDE_USB_MHDRC_TCD
    EXCLUDES        INCLUDE_USBTOOL
}

/* PLX TCD */

Component INCLUDE_USB_PLX_TCD {
    NAME            USB PLX TCD
    SYNOPSIS        PLX USB Target Controller Driver
    MODULES         usbPlxTcdInitExit.o
    REQUIRES        INCLUDE_USB_TGT
    LINK_SYMS       usbPlxTcdVxbRegister
    CFG_PARAMS      USB_PLX_TCD_NAME           \
                    USB_PLX_TCD_VENDOR_ID      \
                    USB_PLX_TCD_PRODUCT_ID     \
                    USB_PLX_TCD_MFG_STRING     \
                    USB_PLX_TCD_PROD_STRING    \
                    USB_PLX_TCD_SERIAL_STRING
}

Parameter USB_PLX_TCD_NAME {
    NAME            USB Target Device Name
    SYNOPSIS        USB Target Device Name
    DEFAULT         {"/usbTgt"}
}

Parameter USB_PLX_TCD_VENDOR_ID {
    NAME            USB Target Device Vendor ID
    SYNOPSIS        USB Target Device Vendor ID
    DEFAULT         {0x1439}
}

Parameter USB_PLX_TCD_PRODUCT_ID {
    NAME            USB Target Device Product ID
    SYNOPSIS        USB Target Device Product ID
    DEFAULT         {0xFF00}
}

Parameter USB_PLX_TCD_MFG_STRING {
    NAME            USB Target Manufacture Name
    SYNOPSIS        USB Target Manufacture Name
    DEFAULT         {"Wind River Systems"}
}

Parameter USB_PLX_TCD_PROD_STRING {
    NAME            USB Target Product Name
    SYNOPSIS        USB Target Product Name
    DEFAULT         {"USB PLX Target Device"}
}

Parameter USB_PLX_TCD_SERIAL_STRING  {
    NAME            USB Target Product Serial Number
    SYNOPSIS        USB Target Product Serial Name
    DEFAULT         {"0123456789012"}
}

Component INCLUDE_USB_PLX_TCD_INIT {
    NAME            USB PLX TCD Init
    SYNOPSIS        Initializes USB PLX TCD Driver
    REQUIRES        INCLUDE_USB_INIT \
                    INCLUDE_TCD_BUS  \
                    INCLUDE_USB_PLX_TCD
    EXCLUDES        INCLUDE_USBTOOL
}

Component INCLUDE_USBTGT_FUNC_BUS {
    NAME            Register All the USB TGT Vxbus Compatible Function Driver to VxBus
    SYNOPSIS        Register All the USB TGT Vxbus Compatible Function Driver to VxBus
    INIT_RTN        usrUsbTgtFuncVxbRegister();
    _INIT_ORDER     usrRoot
    INIT_BEFORE     usrIosCoreInit
    INIT_AFTER      usrRtpInit
}

Component INCLUDE_USBTGT_KBD_INIT {
    NAME            USB GEN2 Target Keyboard Function Driver Init
    SYNOPSIS        USB GEN2 Target Keyboard Function Driver Initialization
    REQUIRES        INCLUDE_USBTGT_KBD     \
                    INCLUDE_USBTGT_KBD_1   \
                    INCLUDE_USB_TGT_FUNC_INIT
    EXCLUDES        INCLUDE_USBTOOL
}

Folder FOLDER_USBTGT_KBD {
    NAME            USB GEN2 Target Keyboard Function Driver
    SYNOPSIS        USB GEN2 Target Keyboard Function Driver
    CHILDREN        INCLUDE_USBTGT_KBD     \
                    USBTGT_KBD_SELECT
}

Component INCLUDE_USBTGT_KBD {
    NAME            USB GEN2 Target Keyboard Function Driver
    SYNOPSIS        USB GEN2 Target Keyboard Function Driver
    MODULES         usbTgtKbdLib.o
    REQUIRES        INCLUDE_USB_TGT
    EXCLUDES        INCLUDE_KBD_EMULATOR           \
                    INCLUDE_USBTOOL
}

Selection USBTGT_KBD_SELECT {
    NAME            USB GEN2 Target Keyboard Function Driver selections
    SYNOPSIS        USB GEN2 Target Keyboard Function Driver selections
    COUNT           0-
    CHILDREN        INCLUDE_USBTGT_KBD_1 \
                    INCLUDE_USBTGT_KBD_2
    DEFAULTS        INCLUDE_USBTGT_KBD_1
}

Component INCLUDE_USBTGT_KBD_1 {
    NAME            USB GEN2 Target Keyboard Function Driver configuration #1
    SYNOPSIS        USB GEN2 Target Keyboard Function Driver configuration #1
    CFG_PARAMS      USBTGT_KBD_ATTACH_TCD_NAME1     \
                    USBTGT_KBD_ATTACH_TCD_UNIT1     \
                    USBTGT_KBD_NAME1                \
                    USBTGT_KBD_UNIT1                \
                    USBTGT_KBD_CONFIG_NUM1
}

Component INCLUDE_USBTGT_KBD_2 {
    NAME            USB GEN2 Target Keyboard Function Driver configuration #2
    SYNOPSIS        USB GEN2 Target Keyboard Function Driver configuration #2
    REQUIRES        INCLUDE_USBTGT_KBD_1
    CFG_PARAMS      USBTGT_KBD_ATTACH_TCD_NAME2     \
                    USBTGT_KBD_ATTACH_TCD_UNIT2     \
                    USBTGT_KBD_NAME2                \
                    USBTGT_KBD_UNIT2                \
                    USBTGT_KBD_CONFIG_NUM2
}

Parameter USBTGT_KBD_ATTACH_TCD_NAME1 {
    NAME            USB GEN2 Target Keyboard Function Driver Attach TCD Name #1
    SYNOPSIS        USB GEN2 Target Keyboard Function Driver Attach TCD Name #1
    DEFAULT         "/usbTgt"
}

Parameter USBTGT_KBD_ATTACH_TCD_UNIT1 {
    NAME            USB GEN2 Target Keyboard Function Driver Attach TCD Unit Number #1
    SYNOPSIS        USB GEN2 Target Keyboard Function Driver Attach TCD Unit Number #1
    DEFAULT         0
}

Parameter USBTGT_KBD_NAME1 {
    NAME            USB GEN2 Target Keyboard Function Driver Name #1
    SYNOPSIS        USB GEN2 Target Keyboard Function Driver Name #1
    DEFAULT         "usbTgtKbd"
}

Parameter USBTGT_KBD_UNIT1 {
    NAME            USB GEN2 Target Keyboard Function Driver Unit #1
    SYNOPSIS        USB GEN2 Target Keyboard Function Driver Unit #1
    DEFAULT         0
}

Parameter USBTGT_KBD_CONFIG_NUM1 {
    NAME            USB GEN2 Target Keyboard Function Driver Configuration Number #1
    SYNOPSIS        USB GEN2 Target Keyboard Function Driver Configuration Number #1
    DEFAULT         1
}

Parameter USBTGT_KBD_ATTACH_TCD_NAME2 {
    NAME            USB GEN2 Target Keyboard Function Driver Attach TCD Name #2
    SYNOPSIS        USB GEN2 Target Keyboard Function Driver Attach TCD Name #2
    DEFAULT         "/usbTgt"
}

Parameter USBTGT_KBD_ATTACH_TCD_UNIT2 {
    NAME            USB GEN2 Target Keyboard Function Driver Attach TCD Unit Number #2
    SYNOPSIS        USB GEN2 Target Keyboard Function Driver Attach TCD Unit Number #2
    DEFAULT         0
}

Parameter USBTGT_KBD_NAME2 {
    NAME            USB GEN2 Target Keyboard Function Driver Name #2
    SYNOPSIS        USB GEN2 Target Keyboard Function Driver Name #2
    DEFAULT         "usbTgtKbd"
}

Parameter USBTGT_KBD_UNIT2 {
    NAME            USB GEN2 Target Keyboard Function Driver Unit #2
    SYNOPSIS        USB GEN2 Target Keyboard Function Driver Unit #2
    DEFAULT         1
}

Parameter USBTGT_KBD_CONFIG_NUM2 {
    NAME            USB GEN2 Target Keyboard Function Driver Configuration Number #2
    SYNOPSIS        USB GEN2 Target Keyboard Function Driver Configuration Number #2
    DEFAULT         1
}

Component INCLUDE_USBTGT_PRN_INIT {
    NAME            USB GEN2 Target Printer Function Driver Init
    SYNOPSIS        USB GEN2 Target Printer Function Driver Initialization
    REQUIRES        INCLUDE_USBTGT_PRN \
                    INCLUDE_USBTGT_PRN_1 \
                    INCLUDE_USB_TGT_FUNC_INIT
    EXCLUDES        INCLUDE_USBTOOL
}

Folder FOLDER_USBTGT_PRN {
    NAME            USB GEN2 Target Printer Function Driver
    SYNOPSIS        USB GEN2 Target Printer Function Driver
    CHILDREN        INCLUDE_USBTGT_PRN   \
                    USBTGT_PRN_SELECT
}

Component INCLUDE_USBTGT_PRN {
    NAME            USB GEN2 Target Printer Function Driver
    SYNOPSIS        USB GEN2 Target Printer Function Driver
    MODULES         usbTgtPrnLib.o
    REQUIRES        INCLUDE_USB_TGT
    EXCLUDES        INCLUDE_PRN_EMULATOR           \
                    INCLUDE_USBTOOL
}

Selection USBTGT_PRN_SELECT {
    NAME            USB GEN2 Target Printer Function Driver selections
    SYNOPSIS        USB GEN2 Target Printer Function Driver selections
    COUNT           0-
    CHILDREN        INCLUDE_USBTGT_PRN_1 \
                    INCLUDE_USBTGT_PRN_2
    DEFAULTS        INCLUDE_USBTGT_PRN_1
}

Component INCLUDE_USBTGT_PRN_1 {
    NAME            USB GEN2 Target Printer Function Driver configuration #1
    SYNOPSIS        USB GEN2 Target Printer Function Driver configuration #1
    CFG_PARAMS      USBTGT_PRN_ATTACH_TCD_NAME1     \
                    USBTGT_PRN_ATTACH_TCD_UNIT1     \
                    USBTGT_PRN_NAME1                \
                    USBTGT_PRN_UNIT1                \
                    USBTGT_PRN_CONFIG_NUM1          \
                    USBTGT_PRN_CAPS1
}

Component INCLUDE_USBTGT_PRN_2 {
    NAME            USB GEN2 Target Printer Function Driver configuration #2
    SYNOPSIS        USB GEN2 Target Printer Function Driver configuration #2
    REQUIRES        INCLUDE_USBTGT_PRN_1
    CFG_PARAMS      USBTGT_PRN_ATTACH_TCD_NAME2     \
                    USBTGT_PRN_ATTACH_TCD_UNIT2     \
                    USBTGT_PRN_NAME2                \
                    USBTGT_PRN_UNIT2                \
                    USBTGT_PRN_CONFIG_NUM2          \
                    USBTGT_PRN_CAPS2
}

Parameter USBTGT_PRN_ATTACH_TCD_NAME1 {
    NAME            USB GEN2 Target Printer Function Driver Attach TCD Name #1
    SYNOPSIS        USB GEN2 Target Printer Function Driver Attach TCD Name #1
    DEFAULT         "/usbTgt"
}

Parameter USBTGT_PRN_ATTACH_TCD_UNIT1 {
    NAME            USB GEN2 Target Printer Function Driver Attach TCD Unit Number #1
    SYNOPSIS        USB GEN2 Target Printer Function Driver Attach TCD Unit Number #1
    DEFAULT         0
}

Parameter USBTGT_PRN_NAME1 {
    NAME            USB GEN2 Target Printer Function Driver Name #1
    SYNOPSIS        USB GEN2 Target Printer Function Driver Name #1
    DEFAULT         "usbTgtPrn"
}

Parameter USBTGT_PRN_UNIT1 {
    NAME            USB GEN2 Target Printer Function Driver Unit #1
    SYNOPSIS        USB GEN2 Target Printer Function Driver Unit #1
    DEFAULT         0
}

Parameter USBTGT_PRN_CONFIG_NUM1 {
    NAME            USB GEN2 Target Printer Function Driver Configuration Number #1
    SYNOPSIS        USB GEN2 Target Printer Function Driver Configuration Number #1
    DEFAULT         1
}

Parameter USBTGT_PRN_CAPS1 {
    NAME            USB GEN2 Target Printer Function Driver Capabilities String #1
    SYNOPSIS        USB GEN2 Target Printer Function Driver Capabilities String #1
    DEFAULT         "MFG:WRS;CMD:PS,BDC,D4;MDL:Emulator USB;CLS:PRINTER;DES:WRS Emulator USB;"
}

Parameter USBTGT_PRN_ATTACH_TCD_NAME2 {
    NAME            USB GEN2 Target Printer Function Driver Attach TCD Name #2
    SYNOPSIS        USB GEN2 Target Printer Function Driver Attach TCD Name #2
    DEFAULT         "/usbTgt"
}

Parameter USBTGT_PRN_ATTACH_TCD_UNIT2 {
    NAME            USB GEN2 Target Printer Function Driver Attach TCD Unit Number #2
    SYNOPSIS        USB GEN2 Target Printer Function Driver Attach TCD Unit Number #2
    DEFAULT         0
}

Parameter USBTGT_PRN_NAME2 {
    NAME            USB GEN2 Target Printer Function Driver Name #2
    SYNOPSIS        USB GEN2 Target Printer Function Driver Name #2
    DEFAULT         "usbTgtPrn"
}

Parameter USBTGT_PRN_UNIT2 {
    NAME            USB GEN2 Target Printer Function Driver Unit #2
    SYNOPSIS        USB GEN2 Target Printer Function Driver Unit #2
    DEFAULT         1
}

Parameter USBTGT_PRN_CONFIG_NUM2 {
    NAME            USB GEN2 Target Printer Function Driver Configuration Number #2
    SYNOPSIS        USB GEN2 Target Printer Function Driver Configuration Number #2
    DEFAULT         1
}

Parameter USBTGT_PRN_CAPS2 {
    NAME            USB GEN2 Target Printer Function Driver Capabilities String #2
    SYNOPSIS        USB GEN2 Target Printer Function Driver Capabilities String #2
    DEFAULT         "MFG:WRS;CMD:PS,BDC,D4;MDL:Emulator USB;CLS:PRINTER;DES:WRS Emulator USB;"
}

Component INCLUDE_USBTGT_MSC_INIT {
    NAME            USB GEN2 Target Mass Storage Function Driver Init
    SYNOPSIS        USB GEN2 Target Mass Storage Function Driver Initialization
    REQUIRES        INCLUDE_USBTGT_MSC   \
                    INCLUDE_USBTGT_MSC_1 \
                    INCLUDE_USB_TGT_FUNC_INIT
    EXCLUDES        INCLUDE_USBTOOL
}

Folder FOLDER_USBTGT_MSC {
    NAME            USB GEN2 Target Mass Storage Function Driver
    SYNOPSIS        USB GEN2 Target Mass Storage Function Driver
    CHILDREN        INCLUDE_USBTGT_MSC   \
                    USBTGT_MSC_SELECT
}

Component INCLUDE_USBTGT_MSC {
    NAME            USB GEN2 Target Mass Storage Function Driver
    SYNOPSIS        USB GEN2 Target Mass Storage Function Driver
    MODULES         usbTgtMscCmd.o usbTgtMscLib.o usbTgtMscUtil.o usbTgtMscCallback.o
    REQUIRES        INCLUDE_USB_TGT
    EXCLUDES        INCLUDE_MS_EMULATOR \
                    INCLUDE_USBTOOL
}

Selection USBTGT_MSC_SELECT {
    NAME            USB GEN2 Target Mass Storage Function Driver selections
    SYNOPSIS        USB GEN2 Target Mass Storage Function Driver selections
    COUNT           0-
    CHILDREN        INCLUDE_USBTGT_MSC_1 \
                    INCLUDE_USBTGT_MSC_2
    DEFAULTS        INCLUDE_USBTGT_MSC_1
}

Component INCLUDE_USBTGT_MSC_1 {
    NAME            USB GEN2 Target Mass Storage Function Driver configuration #1
    SYNOPSIS        USB GEN2 Target Mass Storage Function Driver configuration #1
    CFG_PARAMS      USBTGT_MSC_ATTACH_TCD_NAME1            \
                    USBTGT_MSC_ATTACH_TCD_UNIT1            \
                    USBTGT_MSC_NAME1                       \
                    USBTGT_MSC_UNIT1                       \
                    USBTGT_MSC_PROTOCOL1                   \
                    USBTGT_MSC_CONFIG_NUM1                 \
                    USBTGT_MSC_ATTACH_DEVICE_NAME1         \
                    USBTGT_MSC_ATTACH_DEVICE_REMOVABLE1    \
                    USBTGT_MSC_ATTACH_DEVICE_READONLY1
}

Component INCLUDE_USBTGT_MSC_2 {
    NAME            USB GEN2 Target Mass Storage Function Driver configuration #2
    SYNOPSIS        USB GEN2 Target Mass Storage Function Driver configuration #2
    REQUIRES        INCLUDE_USBTGT_MSC_1
    CFG_PARAMS      USBTGT_MSC_ATTACH_TCD_NAME2            \
                    USBTGT_MSC_ATTACH_TCD_UNIT2            \
                    USBTGT_MSC_NAME2                       \
                    USBTGT_MSC_UNIT2                       \
                    USBTGT_MSC_PROTOCOL2                   \
                    USBTGT_MSC_CONFIG_NUM2                 \
                    USBTGT_MSC_ATTACH_DEVICE_NAME2         \
                    USBTGT_MSC_ATTACH_DEVICE_REMOVABLE2    \
                    USBTGT_MSC_ATTACH_DEVICE_READONLY2
}

Parameter USBTGT_MSC_ATTACH_TCD_NAME1 {
    NAME            USB GEN2 Target Mass Storage Function Driver Attach TCD Name #1
    SYNOPSIS        USB GEN2 Target Mass Storage Function Driver Attach TCD Name #1
    DEFAULT         "/usbTgt"
}

Parameter USBTGT_MSC_ATTACH_TCD_UNIT1 {
    NAME            USB GEN2 Target Mass Storage Function Driver Attach TCD Unit #1
    SYNOPSIS        USB GEN2 Target Mass Storage Function Driver Attach TCD Unit #1
    DEFAULT         0
}

Parameter USBTGT_MSC_NAME1 {
    NAME            USB GEN2 Target Mass Storage Function Driver Name #1
    SYNOPSIS        USB GEN2 Target Mass Storage Function Driver Name #1
    DEFAULT         "usbTgtMsc"
}

Parameter USBTGT_MSC_UNIT1 {
    NAME            USB GEN2 Target Mass Storage Function Driver Unit #1
    SYNOPSIS        USB GEN2 Target Mass Storage Function Driver Unit #1
    DEFAULT         0
}

Parameter USBTGT_MSC_PROTOCOL1 {
    NAME            USB GEN2 Target Mass Storage Function Driver Protocol Type #1
    SYNOPSIS        USB GEN2 Target Mass Storage Function Driver Protocol Type #1
    DEFAULT         "RBC"
}

Parameter USBTGT_MSC_CONFIG_NUM1  {
    NAME            USB GEN2 Target Mass Storage Function Driver Configuration Number #1
    SYNOPSIS        USB GEN2 Target Mass Storage Function Driver Configuration Number #1
    DEFAULT         1
}

Parameter USBTGT_MSC_ATTACH_DEVICE_NAME1 {
    NAME            USB GEN2 Target Mass Storage Function Driver Attach Device Name #1
    SYNOPSIS        USB GEN2 Target Mass Storage Function Driver Attach Device Name #1
    DEFAULT         "/ram0"
}

Parameter USBTGT_MSC_ATTACH_DEVICE_READONLY1 {
    NAME            USB GEN2 Target Mass Storage Function Driver Attach Device Readonly Attribute #1
    SYNOPSIS        USB GEN2 Target Mass Storage Function Driver Attach Device Readonly Attribute #1
    DEFAULT         "n"
}

Parameter USBTGT_MSC_ATTACH_DEVICE_REMOVABLE1 {
    NAME            USB GEN2 Target Mass Storage Function Driver Attach Device Removable Attribute #1
    SYNOPSIS        USB GEN2 Target Mass Storage Function Driver Attach Device Removable Attribute #1
    DEFAULT         "y"
}

Parameter USBTGT_MSC_ATTACH_TCD_NAME2 {
    NAME            USB GEN2 Target Mass Storage Function Driver Attach TCD Name #2
    SYNOPSIS        USB GEN2 Target Mass Storage Function Driver Attach TCD Name #2
    DEFAULT         "/usbTgt"
}

Parameter USBTGT_MSC_ATTACH_TCD_UNIT2 {
    NAME            USB GEN2 Target Mass Storage Function Driver Attach TCD Unit #2
    SYNOPSIS        USB GEN2 Target Mass Storage Function Driver Attach TCD Unit #2
    DEFAULT         0
}

Parameter USBTGT_MSC_NAME2 {
    NAME            USB GEN2 Target Mass Storage Function Driver Name #2
    SYNOPSIS        USB GEN2 Target Mass Storage Function Driver Name #2
    DEFAULT         "usbTgtMsc"
}

Parameter USBTGT_MSC_UNIT2 {
    NAME            USB GEN2 Target Mass Storage Function Driver Unit #2
    SYNOPSIS        USB GEN2 Target Mass Storage Function Driver Unit #2
    DEFAULT         0
}

Parameter USBTGT_MSC_PROTOCOL2 {
    NAME            USB GEN2 Target Mass Storage Function Driver Protocol Type #2
    SYNOPSIS        USB GEN2 Target Mass Storage Function Driver Protocol Type #2
    DEFAULT         "RBC"
}

Parameter USBTGT_MSC_CONFIG_NUM2 {
    NAME            USB GEN2 Target Mass Storage Function Driver Configuration Number #2
    SYNOPSIS        USB GEN2 Target Mass Storage Function Driver Configuration Number #2
    DEFAULT         1
}

Parameter USBTGT_MSC_ATTACH_DEVICE_NAME2 {
    NAME            USB GEN2 Target Mass Storage Function Driver Attach Device Name #2
    SYNOPSIS        USB GEN2 Target Mass Storage Function Driver Attach Device Name #2
    DEFAULT         "/ram0"
}

Parameter USBTGT_MSC_ATTACH_DEVICE_READONLY2 {
    NAME            USB GEN2 Target Mass Storage Function Driver Attach Device Readonly Attribute #2
    SYNOPSIS        USB GEN2 Target Mass Storage Function Driver Attach Device Readonly Attribute #2
    DEFAULT         "n"
}

Parameter USBTGT_MSC_ATTACH_DEVICE_REMOVABLE2 {
    NAME            USB GEN2 Target Mass Storage Function Driver Attach Device Removable Attribute #2
    SYNOPSIS        USB GEN2 Target Mass Storage Function Driver Attach Device Removable Attribute #2
    DEFAULT         "y"
}

Component INCLUDE_USBTGT_SER_INIT {
    NAME            USB GEN2 Target CDC ACM Serial Function Driver Init
    SYNOPSIS        USB GEN2 Target CDC ACM Serial Function Driver Initialization
    INIT_RTN        usrUsbTgtSerVxbRegister();
    _INIT_ORDER     usrRoot
    INIT_BEFORE     usrIosCoreInit
    INIT_AFTER      usrRtpInit
    REQUIRES        INCLUDE_USBTGT_SER \
                    INCLUDE_USB_TGT_FUNC_INIT
    EXCLUDES        INCLUDE_USBTOOL
}

Component INCLUDE_USBTGT_SER {
    NAME            USB GEN2 Target CDC ACM Serial Function Driver
    SYNOPSIS        USB GEN2 Target CDC ACM Serial Function Driver
    MODULES         usbTgtSerLib.o usbTgtSerSio.o
    REQUIRES        INCLUDE_USB_TGT
    EXCLUDES        INCLUDE_USBTOOL            \
                    INCLUDE_SER_EMULATOR
    CFG_PARAMS      USBTGT_SER_BINDING_TO_TARGET_NAME \
                    USBTGT_SER_BINDING_TO_TARGET_UNIT \
                    USBTGT_SER_BINDING_TO_CONFIG_NUM  \
                    USBTGT_SER_FUNC_NAME \
                    USBTGT_SER_DEFAULT_BAUDRATE \
                    USBTGT_SER_DUMMY_BUFFER_SIZE \
                    USBTGT_SER_TX_TASK_PRIORITY \
                    USBTGT_SER_DUMMY_BUFFER_ENABLE \
                    USBTGT_SER_REDIRECT_MSG_ENABLE
}

Parameter USBTGT_SER_BINDING_TO_TARGET_NAME {
    NAME            Target Serial Function Driver Bind to the Target's Name
    SYNOPSIS        Target Serial Function Driver Bind to the Target's Name
    DEFAULT         "/usbTgt"
}

Parameter USBTGT_SER_BINDING_TO_TARGET_UNIT {
    NAME            Target Serial Function Driver Bind to the Target's Unit Number
    SYNOPSIS        Target Serial Function Driver Bind to the Target's Unit Number
    DEFAULT         0
}

Parameter USBTGT_SER_BINDING_TO_CONFIG_NUM {
    NAME            Target Serial Function Driver Bind to the Target's Configuration Number
    SYNOPSIS        Target Serial Function Driver Bind to the Target's Configuration Number
    DEFAULT         1
}

Parameter USBTGT_SER_FUNC_NAME {
    NAME            Target Serial Function Driver name
    SYNOPSIS        Target Serial Function Driver name
    DEFAULT         "usbTgtSer"
}

Parameter USBTGT_SER_DEFAULT_BAUDRATE {
    NAME            Target Serial Function Driver default baudrate
    SYNOPSIS        Target Serial Function Driver default baudrate
    DEFAULT         9600
}

Parameter USBTGT_SER_DUMMY_BUFFER_SIZE {
    NAME            Target Serial Function Driver default ios buffer size
    SYNOPSIS        Target Serial Function Driver default ios buffer size in bytes, 0 to disable dummy buffer
    DEFAULT         4096
}

Parameter USBTGT_SER_TX_TASK_PRIORITY {
    NAME            Target Serial Function Driver transfer task priority
    SYNOPSIS        Target Serial Function Driver transfer task priority (0 to 255)
    DEFAULT         100
}

Parameter USBTGT_SER_DUMMY_BUFFER_ENABLE {
    NAME            Target Serial Function Driver dummy buffer enable
    SYNOPSIS        Target Enable dummy buffer for Serial Function Driver(TRUE: enable, FALSE: disable)
    DEFAULT         TRUE
}

Parameter USBTGT_SER_REDIRECT_MSG_ENABLE {
    NAME            Target Serial Function Driver display redirect message
    SYNOPSIS        Target Serial Function Driver display redirect message on default TTY when boot up(TRUE: enable, FALSE: disable)
    DEFAULT         TRUE
}

Component INCLUDE_USBTGT_FUNCDEMO_INIT {
    NAME            USB Target Function Demo Driver Init
    SYNOPSIS        USB Target Function Demo Driver Initialization
    REQUIRES        INCLUDE_USBTGT_FUNCDEMO
    EXCLUDES        INCLUDE_USBTOOL
    CONFIGLETTES    usrUsbTgtFuncDemoInit.c
    PROTOTYPE       void usrUsbTgtFuncDemoInit (void);
    INIT_RTN        usrUsbTgtFuncDemoInit();
}

Component INCLUDE_USBTGT_FUNCDEMO {
    NAME            USB Target Function Demo Driver
    SYNOPSIS        USB Target Function Demo Driver
    CONFIGLETTES    usrUsbTgtFuncDemoInit.c
    PROTOTYPE       void usrUsbTgtFuncDemoConfig (void);
    INIT_RTN        usrUsbTgtFuncDemoConfig ();
    CFG_PARAMS      FUNCDEMO_BINDING_TO_TARGET_NAME \
                    FUNCDEMO_BINDING_TO_TARGET_UNIT \
                    FUNCDEMO_NAME                   \
                    FUNCDEMO_UNIT                   \
                    FUNCDEMO_BINDING_TO_CONFIG_NUM
}

Parameter FUNCDEMO_BINDING_TO_TARGET_NAME {
    NAME            The Target's Name Which Binded By The Function Demo Driver
    SYNOPSIS        The Target's Name Which Binded By The Function Demo Driver
    DEFAULT         "/usbTgt"
}

Parameter FUNCDEMO_BINDING_TO_TARGET_UNIT {
    NAME            The Target's Unit Number Which Binded By The Function Demo Driver
    SYNOPSIS        The Target's Unit Number Which Binded By The Function Demo Driver
    DEFAULT         0
}

Parameter FUNCDEMO_BINDING_TO_CONFIG_NUM {
    NAME            The Target's Configuration Number Which Binded By The Function Demo Driver
    SYNOPSIS        The Target's Configuration Number Which Binded By The Function Demo Driver
    DEFAULT         1
}

Parameter FUNCDEMO_NAME {
    NAME            The Function Demo Driver's Name
    SYNOPSIS        The Function Demo Driver's Name
    DEFAULT         "funcDemo"
}

Parameter FUNCDEMO_UNIT {
    NAME            The Function Demo Driver's Unit Number
    SYNOPSIS        The Function Demo Driver's Unit Number
    DEFAULT         0
}

Component INCLUDE_USBTGT_RNDIS_INIT {
    NAME            USB Target RNDIS Function Driver Init
    SYNOPSIS        USB Target RNDIS Function Driver Initialization
    REQUIRES        INCLUDE_USBTGT_RNDIS_1 \
                    SELECT_USBTGT_RNDIS_MEDIUM_TYPE_1 \
                    INCLUDE_USBTGT_NETWORK_INIT \
                    INCLUDE_USB_TGT_FUNC_INIT
    EXCLUDES        INCLUDE_USBTOOL
}

Selection SELECT_USBTGT_RNDIS {
    NAME            Selection of the USB Target RNDIS Function Driver Configuration
    SYNOPSIS        Selection of the USB Target RNDIS Function Driver Configuration
    _CHILDREN       FOLDER_USBTGT_NETWORK
    CHILDREN        FOLDER_USBTGT_RNDIS_1 \
                    FOLDER_USBTGT_RNDIS_2
    DEFAULTS        FOLDER_USBTGT_RNDIS_1
}

Folder FOLDER_USBTGT_RNDIS_1 {
    NAME        USBTGT RNDIS Function Configuration (Group 1)
    SYNOPSIS    USBTGT RNDIS Function Configuration (Group 1)
    CHILDREN        INCLUDE_USBTGT_RNDIS_1 \
                    SELECT_USBTGT_RNDIS_MEDIUM_TYPE_1
    DEFAULTS        SELECT_USBTGT_RNDIS_MEDIUM_TYPE_1 \
                    INCLUDE_USBTGT_RNDIS_1
}

Folder FOLDER_USBTGT_RNDIS_2 {
    NAME        USBTGT RNDIS Function Configuration (Group 2)
    SYNOPSIS    USBTGT RNDIS Function Configuration (Group 2)
    REQUIRES        INCLUDE_USBTGT_RNDIS_1 \
                    SELECT_USBTGT_RNDIS_MEDIUM_TYPE_1
    CHILDREN        SELECT_USBTGT_RNDIS_MEDIUM_TYPE_2 \
                    INCLUDE_USBTGT_RNDIS_2
    DEFAULTS        SELECT_USBTGT_RNDIS_MEDIUM_TYPE_2 \
                    INCLUDE_USBTGT_RNDIS_2
}

Component INCLUDE_USBTGT_RNDIS_1 {
    NAME            The 1st USB Target RNDIS Function (Default)
    SYNOPSIS        The 1st USB Target RNDIS Function (Default)
    REQUIRES        INCLUDE_USBTGT_NETWORK
    CFG_PARAMS      USBTGT_RNDIS_TCD_NAME1 \
                    USBTGT_RNDIS_TCD_UNIT1 \
                    USBTGT_RNDIS_NAME1                   \
                    USBTGT_RNDIS_UNIT1                   \
                    USBTGT_RNDIS_CONFIG_NUM1                   \
                    USBTGT_RNDIS_PM_ENABLE1                   \
                    USBTGT_RNDIS_TASK_PRIORITY1
}

Parameter USBTGT_RNDIS_TCD_NAME1 {
    NAME            The Tcd's Name Which Attached By The RNDIS Function #1
    SYNOPSIS        The Tcd's Name Which Attached By The RNDIS Function #1
    DEFAULT         "/usbTgt"
}

Parameter USBTGT_RNDIS_TCD_UNIT1 {
    NAME            The Tcd's Unit Number Which Attached By The RNDIS Function #1
    SYNOPSIS        The Tcd's Unit Number Which Attached By The RNDIS Function #1
    DEFAULT         0
}

Parameter USBTGT_RNDIS_NAME1 {
    NAME            The Name of the RNDIS Function #1
    SYNOPSIS        The Name of the RNDIS Function #1
    DEFAULT         "usbTgtRndis"
}

Parameter USBTGT_RNDIS_UNIT1 {
    NAME            The Unit Number of RNDIS Function #1
    SYNOPSIS        The Unit Number of RNDIS Function #1
    DEFAULT         0
}

Parameter USBTGT_RNDIS_CONFIG_NUM1 {
    NAME            The Configuration Number Which Bullt in By The RNDIS Function #1
    SYNOPSIS        The Configuration Number Which Bullt in By The RNDIS Function #1
    DEFAULT         1
}

Parameter USBTGT_RNDIS_PM_ENABLE1 {
    NAME            Enable the Power Management Support of the RNDIS Function #1
    SYNOPSIS        Enable the Power Management Support of the RNDIS Function #1
    DEFAULT         FALSE
}

Parameter USBTGT_RNDIS_TASK_PRIORITY1 {
    NAME            The Task Priority Used by The Usb RNDIS Function #1
    SYNOPSIS        The Task Priority Used by The Usb RNDIS Function #1
    DEFAULT         100
}

Selection SELECT_USBTGT_RNDIS_MEDIUM_TYPE_1 {
    NAME        Select the Medium Type Which Bound by RNDIS Function #1
    SYNOPSIS    Select the Medium Type Which Bound by RNDIS Function #1
    COUNT       1-1
    CHILDREN    INCLUDE_USBTGT_RNDIS_MEDIUM_TYPE_VIRTUAL_END1  \
                INCLUDE_USBTGT_RNDIS_MEDIUM_TYPE_END_BRIDGE1
    DEFAULTS    INCLUDE_USBTGT_RNDIS_MEDIUM_TYPE_VIRTUAL_END1
}

Component INCLUDE_USBTGT_RNDIS_MEDIUM_TYPE_VIRTUAL_END1  {
    NAME            The Virtual END Medium Type Bound to RNDIS Function #1
    SYNOPSIS        The Virtual END Medium Type Bound to RNDIS Function #1
    REQUIRES        INCLUDE_USBTGT_NETWORK    \
                    INCLUDE_USBTGT_RNDIS_1    \
                    INCLUDE_USBTGT_VRTL_END
    CFG_PARAMS      USBTGT_RNDIS_MEDIUM_VRTL_END_NAME1 \
                    USBTGT_RNDIS_MEDIUM_VRTL_END_UNIT1
}

Parameter USBTGT_RNDIS_MEDIUM_VRTL_END_NAME1 {
    NAME            The Virtual END Medium Name Which Bound to RNDIS Function #1
    SYNOPSIS        The Virtual END Medium Name Which Bound to RNDIS Function #1
    DEFAULT         "usbEnd"
}

Parameter USBTGT_RNDIS_MEDIUM_VRTL_END_UNIT1 {
    NAME            The 1st RNDIS Function's Virtual END Medium Unit Number
    SYNOPSIS        The 1st RNDIS Function's Virtual END Medium Unit Number
    DEFAULT         0
}

Component INCLUDE_USBTGT_RNDIS_MEDIUM_TYPE_END_BRIDGE1  {
    NAME            The END Bridge Medium Type Bound to The RNDIS Function #1
    SYNOPSIS        The END Bridge Medium Type Bound to The RNDIS Function #1
    REQUIRES        INCLUDE_USBTGT_NETWORK \
                    INCLUDE_USBTGT_RNDIS_1
    CFG_PARAMS      USBTGT_RNDIS_MEDIUM_END_BRIDGE_NAME1 \
                    USBTGT_RNDIS_MEDIUM_END_BRIDGE_UNIT1
}

Parameter USBTGT_RNDIS_MEDIUM_END_BRIDGE_NAME1 {
    NAME            The END Bridge Medium Name Which Bound to RNDIS Function #1
    SYNOPSIS        The END Bridge Medium Name Which Bound to RNDIS Function #1
    DEFAULT         "sme"
}

Parameter USBTGT_RNDIS_MEDIUM_END_BRIDGE_UNIT1 {
    NAME            The END Bridge Medium Unit Number Which Bound to RNDIS Function #1
    SYNOPSIS        The END Bridge Medium Unit Number Which Bound to RNDIS Function #1
    DEFAULT         0
}

Component INCLUDE_USBTGT_RNDIS_2 {
    NAME            The 2nd USB Target RNDIS Function
    SYNOPSIS        The 2nd USB Target RNDIS Function
    REQUIRES        INCLUDE_USBTGT_NETWORK
    CFG_PARAMS      USBTGT_RNDIS_TCD_NAME2 \
                    USBTGT_RNDIS_TCD_UNIT2 \
                    USBTGT_RNDIS_NAME2                   \
                    USBTGT_RNDIS_UNIT2                   \
                    USBTGT_RNDIS_CONFIG_NUM2                   \
                    USBTGT_RNDIS_PM_ENABLE2                   \
                    USBTGT_RNDIS_TASK_PRIORITY2
}

Parameter USBTGT_RNDIS_TCD_NAME2 {
    NAME            The Tcd's Name Which Attached By The RNDIS Function #2
    SYNOPSIS        The Tcd's Name Which Attached By The RNDIS Function #2
    DEFAULT         "/usbTgt"
}

Parameter USBTGT_RNDIS_TCD_UNIT2 {
    NAME            The Tcd's Unit Number Which Attached By The RNDIS Function #2
    SYNOPSIS        The Tcd's Unit Number Which Attached By The RNDIS Function #2
    DEFAULT         1
}

Parameter USBTGT_RNDIS_NAME2 {
    NAME            The Name of RNDIS Function #2
    SYNOPSIS        The Name of RNDIS Function #2
    DEFAULT         "usbTgtRndis"
}

Parameter USBTGT_RNDIS_UNIT2 {
    NAME            The Unit Number of RNDIS Function #2
    SYNOPSIS        The Unit Number of RNDIS Function #2
    DEFAULT         1
}

Parameter USBTGT_RNDIS_CONFIG_NUM2 {
    NAME            The Configuration Number Which Bullt in By The RNDIS Function #2
    SYNOPSIS        The Configuration Number Which Bullt in By The RNDIS Function #2
    DEFAULT         1
}

Parameter USBTGT_RNDIS_PM_ENABLE2 {
    NAME            Enable the Power Management Support of the RNDIS Function #2
    SYNOPSIS        Enable the Power Management Support of the RNDIS Function #2
    DEFAULT         FALSE
}

Parameter USBTGT_RNDIS_TASK_PRIORITY2 {
    NAME            The Task Priority Used by The Usb RNDIS Function #2
    SYNOPSIS        The Task Priority Used by The Usb RNDIS Function #2
    DEFAULT         100
}

Selection SELECT_USBTGT_RNDIS_MEDIUM_TYPE_2 {
    NAME        Select the Medium Type Which Bound by The RNDIS Function #2
    SYNOPSIS    Select the Medium Type Which Bound by The RNDIS Function #2
    COUNT       1-1
    CHILDREN    INCLUDE_USBTGT_RNDIS_MEDIUM_TYPE_VIRTUAL_END2  \
                INCLUDE_USBTGT_RNDIS_MEDIUM_TYPE_END_BRIDGE2
    DEFAULTS    INCLUDE_USBTGT_RNDIS_MEDIUM_TYPE_VIRTUAL_END2
}

Component INCLUDE_USBTGT_RNDIS_MEDIUM_TYPE_VIRTUAL_END2  {
    NAME            The Virtual END Medium Type Bound to The RNDIS Function #2
    SYNOPSIS        The Virtual END Medium Type Bound to The RNDIS Function #2
    REQUIRES        INCLUDE_USBTGT_NETWORK  \
                    INCLUDE_USBTGT_RNDIS_2    \
                    INCLUDE_USBTGT_VRTL_END
    CFG_PARAMS      USBTGT_RNDIS_MEDIUM_VRTL_END_NAME2 \
                    USBTGT_RNDIS_MEDIUM_VRTL_END_UNIT2
}

Parameter USBTGT_RNDIS_MEDIUM_VRTL_END_NAME2 {
    NAME            The Virtual END Medium Name Which Bound to RNDIS Function #2
    SYNOPSIS        The Virtual END Medium Name Which Bound to RNDIS Function #2
    DEFAULT         "usbEnd"
}

Parameter USBTGT_RNDIS_MEDIUM_VRTL_END_UNIT2 {
    NAME            The Virtual END Medium Unit Number Which Bound to RNDIS Function #2
    SYNOPSIS        The Virtual END Medium Unit Number Which Bound to RNDIS Function #2
    DEFAULT         1
}

Component INCLUDE_USBTGT_RNDIS_MEDIUM_TYPE_END_BRIDGE2  {
    NAME            The END Bridge Medium Type Bound to The RNDIS Function #2
    SYNOPSIS        The END Bridge Medium Type Bound to The RNDIS Function #2
    REQUIRES        INCLUDE_USBTGT_NETWORK \
                    INCLUDE_USBTGT_RNDIS_2
    CFG_PARAMS      USBTGT_RNDIS_MEDIUM_END_BRIDGE_NAME2 \
                    USBTGT_RNDIS_MEDIUM_END_BRIDGE_UNIT2
}

Parameter USBTGT_RNDIS_MEDIUM_END_BRIDGE_NAME2 {
    NAME            The END Bridge Medium Name Which Bound to RNDIS Function #2
    SYNOPSIS        The END Bridge Medium Name Which Bound to RNDIS Function #2
    DEFAULT         "sme"
}

Parameter USBTGT_RNDIS_MEDIUM_END_BRIDGE_UNIT2 {
    NAME            The END Bridge Medium Unit Number Which Bound to RNDIS Function #2
    SYNOPSIS        The END Bridge Medium Unit Number Which Bound to RNDIS Function #2
    DEFAULT         1
}

Component INCLUDE_USBTGT_NETWORK_INIT {
    NAME            USB Target Network Function Driver Init
    SYNOPSIS        USB Target Network Function Driver Initialization
    REQUIRES        INCLUDE_USBTGT_NETWORK  \
                    INCLUDE_USB_TGT_FUNC_INIT
    EXCLUDES        INCLUDE_USBTOOL
}

Component INCLUDE_USBTGT_NETWORK  {
    NAME            USB Target Network Function Driver
    SYNOPSIS        USB Target Network Function Driver
    REQUIRES        INCLUDE_USB \
                    INCLUDE_USB_TGT
    CFG_PARAMS      USBTGT_NETWORK_DATA_POOL_SIZE \
                    USBTGT_NETWORK_DATA_POOL_COUNT
}

Parameter USBTGT_NETWORK_DATA_POOL_SIZE {
    NAME            The Data Pool Size of Bind Buffer Used by The Usb Target Network Function Driver
    SYNOPSIS        The Data Pool Size of Bind Buffer Used by The Usb Target Network Function Driver, should be bigger than 1514
    DEFAULT         1560
}

Parameter USBTGT_NETWORK_DATA_POOL_COUNT {
    NAME            The Data Pool Counts of Bind Buffer Used by The Usb Target Network Function Driver
    SYNOPSIS        The Data Pool Counts of Bind Buffer Used by The Usb Target Network Function Driver
    DEFAULT         8
}

Component INCLUDE_USBTGT_VRTL_END  {
    NAME            The USB TGT Virtual END Driver
    SYNOPSIS        The USB TGT Virtual END Driver
    REQUIRES        INCLUDE_USBTGT_NETWORK \
                    INCLUDE_USBTGT_FUNC_BUS
    CFG_PARAMS      USBTGT_VRTL_END_MAX_COUNT  \
                    USBTGT_VRTL_END_NAME       \
                    USBTGT_VRTL_END_SPEED      \
                    USBTGT_VRTL_END_LOCAL_MAC  \
                    USBTGT_VRTL_END_REMOTE_MAC \
                    USBTGT_VRTL_END_TYPE
}

Parameter USBTGT_VRTL_END_MAX_COUNT {
    NAME            The Max Counts of The Virtual END Device
    SYNOPSIS        The Max Counts of The Virtual END Device
    DEFAULT         1
}

Parameter USBTGT_VRTL_END_NAME {
    NAME            The Name of The Virtual END Device
    SYNOPSIS        The Name of The Virtual END Device
    DEFAULT         "usbEnd"
}

Parameter USBTGT_VRTL_END_TYPE {
    NAME            The Type of The Virtual END Device Driver
    SYNOPSIS        The Type of The Virtual END Device Driver (0 : Lagecy END; 1 : vxBus END; 2 : vxBus END2)
    DEFAULT         1
}

Parameter USBTGT_VRTL_END_SPEED {
    NAME            The Speed of The Virtual END
    SYNOPSIS        The Speed of The Virtual END count by Mbsp (10 : 10Mbps; 100 : 100Mbps; 1000 : 1000Mbps)
    DEFAULT         100
}

Parameter USBTGT_VRTL_END_LOCAL_MAC  {
    NAME            The Local Virtual END MAC address
    SYNOPSIS        The Local Virtual END MAC address; usbEnd0's MAC address
    DEFAULT         {"00:11:22:33:44:00"}
}

Parameter USBTGT_VRTL_END_REMOTE_MAC {
    NAME            The Remote MAC Address of The Virtual END
    SYNOPSIS        The Remote MAC Address of The Virtual END, Seen on The Host Side (Such As Windows)
    DEFAULT         {"00:11:22:33:55:00"}
}

Component INCLUDE_USB_TGT {
    NAME            USB GEN2 Target Stack
    SYNOPSIS        USB GEN2 Target Stack
    REQUIRES        INCLUDE_USB
    MODULES         usbTgt.o usbTgtControlRequest.o
    CFG_PARAMS      USBTGT_IAD_DESC_ENABLE         \
                    USBTGT_DEV_DESC_FUNC_ENABLE    \
                    USBTGT_DEV_DESC_OF_FUNC_NAME   \
                    USBTGT_DEV_DESC_OF_FUNC_UNIT
}

Parameter USBTGT_IAD_DESC_ENABLE {
    NAME            Enable to Integrate IAD Descriptor
    SYNOPSIS        Enable to Integrate IAD Descriptor
    TYPE            BOOL
    DEFAULT         TRUE
}

Parameter USBTGT_DEV_DESC_FUNC_ENABLE {
    NAME            Enable to Use the Function Driver's Device Descriptors
    SYNOPSIS        Enable to Use the Function Driver's Device Descriptors
    TYPE            BOOL
    DEFAULT         FALSE
}

Parameter USBTGT_DEV_DESC_OF_FUNC_NAME {
    NAME            The Name of the Function Driver Provides Device Descriptor for Target Stack Using
    SYNOPSIS        The Name of the Function Driver Provides Device Descriptor for Target Stack Using
    DEFAULT         ""
}

Parameter USBTGT_DEV_DESC_OF_FUNC_UNIT {
    NAME            The Unit of the Function Driver Provides Device Descriptor for Target Stack Using
    SYNOPSIS        The Unit of the Function Driver Provides Device Descriptor for Target Stack Using
    TYPE            int
    DEFAULT         0
}

Component INCLUDE_USB_TGT_FUNC_INIT {
    NAME            USB GEN2 Target Function Driver Init
    SYNOPSIS        USB GEN2 Target Function Driver Initialization
    REQUIRES        INCLUDE_USB_INIT    \
                    INCLUDE_USB
    INIT_RTN        usrUsbTgtFuncInit();
    INIT_AFTER      usbHstClassInit
    _INIT_ORDER     usrRoot
    CONFIGLETTES    usrUsbInit.c
    PROTOTYPE       STATUS usrUsbTgtFuncInit (void);
}

Component INCLUDE_TCD_BUS {
    NAME            USB Target Controller Start
    SYNOPSIS        Register USB Target Controller to vxBus
    REQUIRES        INCLUDE_USB_INIT    \
                    INCLUDE_USB
    EXCLUDES        INCLUDE_USBTOOL
    PROTOTYPE       void vxbUsbTcdRegister (void);
    INIT_RTN        vxbUsbTcdRegister();
    _INIT_ORDER     hardWareInterFaceBusInit
}

