/* 20comp_mnd.cdf - configuration for MIPC Network driver */

/*
 * Copyright (c) 2008-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01g,28mar11,brk  improved init order and dependencies to allow
                 use of bootline, network apps, and IFCONFIG component
                 added bootline backplne IP assignment
01f,19nov10,h_w  Modify INIT_AFTER to eliminate compiler warning (WIND00238721)
01e,23jun10,h_w  upgrade to MND 2.1
01d,25sep09,spw  Introduce MND_TX_BUFS, MND_RX_BUFS
01c,23sep09,spw  Expand on MND_CFG_STR description
01b,21sep09,spw  Change default MTU to match MIPC_SM_MTU
01a,03mar09,spw  written.
*/

Folder FOLDER_MND {
	NAME		MIPC Network Device
	SYNOPSIS	END device between multiple operating systems.
	_CHILDREN	FOLDER_MIPC
}
/*	was
	REQUIRES	INCLUDE_MIPC_SM  
	_INIT_ORDER	usrRoot
	INIT_AFTER	usrNetworkInit */

Component INCLUDE_MND {
	NAME		Multi-OS IPC Network Device
	SYNOPSIS	Provides support for MIPC END devices.
	REQUIRES	INCLUDE_MIPC_SM  INCLUDE_NETWORK INCLUDE_END
	MODULES		mipcnetEnd.o mipcnetIpc.o
	CONFIGLETTES	usrMnd.c
	INIT_RTN	usrMndInit (MND_MTU, MND_TX_BUFS, MND_RX_BUFS, MND_CTRL_PORT, MND_CFG_STR);
	_INIT_ORDER	usrNetworkInit 
	INIT_AFTER	INCLUDE_IPCOM
	INIT_BEFORE	INCLUDE_NET_BOOT_CONFIG 
	_CHILDREN	FOLDER_MND
	CFG_PARAMS	MND_MTU \
				MND_TX_BUFS \
				MND_RX_BUFS \
				MND_CFG_STR \
				MND_CTRL_PORT
}

Parameter MND_MTU {
    NAME        Max bytes in data packet
    SYNOPSIS    Determines the maximum size, in bytes, of a MND frame payload. \
Should not be larger than MIPC_SM_MTU value. A value of -1 will let MND uses the \
MTU of the MIPC bus.
    TYPE        int
    DEFAULT     -1
}

Parameter MND_TX_BUFS {
	NAME		Number of transmission buffers
	SYNOPSIS	Sets the number of transmission buffers of the underlying MIPC \
control and data sockets. A value of -1 will set the value according to the MND \
configuration string MND_CFG_STR nodes allowed on a MIPC bus. The value of TX \
bufs in this case will be double of the power of 2 greater than the number of \
MIPC nodes allowed on a bus. All MND related sockets including control sockets \
for buses and data sockets for units have the same number of transmission \
buffers which is determined by this parameter.
	TYPE		int
	DEFAULT		-1
}

Parameter MND_RX_BUFS {
	NAME		Number of receive buffers
	SYNOPSIS	Sets the number of receive buffers of the underlying MIPC \
control and data sockets. A value of -1 will set the value according to the MND \
configuration string MND_CFG_STR nodes allowed on a MIPC bus. The value of RX \
bufs in this case will be double of the power of 2 greater than the number of \
MIPC nodes allowed on the bus. All MND related sockets including control sockets \
for buses and data sockets for units have the same number of receive buffers \
which is determined by this parameter.
	TYPE		int
	DEFAULT		-1
}

Parameter MND_CTRL_PORT {
	NAME		MND control socket port number 
	SYNOPSIS	Specifies the number of the MIPC port used by the MND control \
socket to manage MND devices on a MIPC bus.  Should not be larger than \
(MIPC_SM_PORTS -1) value. 
	TYPE		UINT
	DEFAULT		23
}

Parameter MND_CFG_STR {
	NAME		MND configuration string
	SYNOPSIS	The configuration string format is: \
#unit=unitNum segment=segNum [dport=portNum] bus=busName #unit= ... \
Where: \
'#' is a separator between MND devices. \
unit=unitNum is the MND device number, i.e. mnd0, mnd1 ... mndn. \
segment=segNum is the mipcnet segment on which to attach the MND unit. \
dport=portNum is the port used to send data packets. If this parameter is set \
            to -1 or omitted, portNum is chosen automatically. Otherwise, \
            portNum must be unique for each unit on a given bus and must be \
            smaller than (MIPC_SM_PORTS - 1). \
bus=busName is the MIPC bus on which the MND unit resides.               
	TYPE		char *
	DEFAULT		"#unit=0 segment=0 dport=24 bus=main"
}

Component INCLUDE_MND_BACKPLANE_IP
{
	NAME		MND address from Bootline's backplane IP
	SYNOPSIS	The bootline paramenter for the backplane IP address is assigned \
	            to the mnd0 device 
	REQUIRES	INCLUDE_MND INCLUDE_NET_BOOT
	EXCLUDES	INCLUDE_SM_NET
	CONFIGLETTES	net/coreip/usrNetBoot.c net/coreip/usrNetBootConfig.c \
                        net/coreip/usrNetIpAttachCommon.c
	INIT_RTN	usrMndBpIpSet();
	_INIT_ORDER	usrNetworkInit 
	INIT_AFTER	INCLUDE_NET_BOOT_CONFIG
	_CHILDREN	FOLDER_MND
}