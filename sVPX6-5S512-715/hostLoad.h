#ifndef __HOST_LOAD_H
#define __HOST_LOAD_H

#define MPC_FPGA_SPARTEN          0x92000000
#define HOST_LOAD_DATA_REG       (MPC_FPGA_SPARTEN + 0x40)
#define HOST_LOAD_CTRL_REG       (MPC_FPGA_SPARTEN + 0x44)
#define HOST_LOAD_STAT_REG       (MPC_FPGA_SPARTEN + 0x48)

#define HOST_LOAD_DONE_MAX       10000
#define HOST_LOAD_CLK_AFDONE     8

#define HOST_CLK_HIGH            1 
#define HOST_CLK_LOW             0
#define HOST_CTRL_PROGB          0
#define HOST_CTRL_RDWR           1
#define HOST_CTRL_CSI            2
#define HOST_STAT_INITB          1

#define HOST_DATA_BUF_LEN        64

#endif

