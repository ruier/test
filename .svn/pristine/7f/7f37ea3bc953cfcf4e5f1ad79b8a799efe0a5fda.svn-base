#ifndef __RIO_FSL_H__
#define __RIO_FSL_H__

#define SRIO_BASE_ADDR            0xe0000000
#define SRIO_DOORBELL_IDMR        0xd3460
#define SRIO_DOORBELL_IDSR        0xd3464
#define SRIO_DOORBELL_EIDQDPAR    0xd3468
#define SRIO_DOORBELL_IDQDPAR     0xd346c
#define SRIO_DOORBELL_EIDQEPAR    0xd3470
#define SRIO_DOORBELL_IDQEPAR     0xd3474
#define SRIO_DOORBELL_MIRI        0xd3478
#define SRIO_GCCSR                0xc013c
#define SRIO_BDIDCSR              0xc0060
#define SRIO_LMREQCSR             0xc0140
#define SRIO_LMRESPCSR            0xc0144

#define SRIO_REQ_INPUT_STATUS     0x4
#define SRIO_REQ_RST_DEV          0x3
#define SRIO_REQ_RESP_VALID       0x80000000


#define SRIO_MAX_RETRY_REQ        8

#define SRIO_MAINTENANCE_ADDR     0xc0000000

#define M8641RIO_ROWTAR1          0xd0c20 /*ROWTAR1*/
#define M8641RIO_ROWTEAR1         0xd0c24 /*ROWTEAR1*/
#define M8641RIO_ROWBAR1          0xd0c28 /*ROWBAR1*/
#define M8641RIO_ROWAR1           0xd0c30 /*ROWAR1*/
#define M8641RIO_IMR              0xd3060
#define M8641RIO_ISR              0xd3064
#define M8641RIO_IFQDPAR          0xd306C
#define M8641RIO_IFQEPAR          0xd3074
#define M8641RIO_IMMIRIR          0xd3078
#define M8641RIO_OMR              0xd3000
#define M8641RIO_OSR              0xd3004
#define M8641RIO_ODQDPAR          0xd300C
#define M8641RIO_EOSAR            0xd3010
#define M8641RIO_OSAR             0xd3014
#define M8641RIO_ODPR             0xd3018
#define M8641RIO_ODATR            0xd301C
#define M8641RIO_ODCR             0xd3020
#define M8641RIO_ODQEPAR          0xd3028
#define M8641RIO_OMRETCR          0xd302c
#define M8641RIO_OMMGR            0xd3030
#define M8641RIO_OMMLR            0xd3034
#define M8641RIO_ODMR             0xd3400
#define M8641RIO_ODSR             0xd3404
#define M8641RIO_ODDPR            0xd3418
#define M8641RIO_ODDATR           0xd341C
#define M8641RIO_ODRETCR          0xd342C
#define M8641RIO_SEGMENT_SET(base,winNum,segNum) \
            ((volatile UINT32 *)(((char*)base) + 0xd0c10 +winNum*0x20+segNum*0x4))

#define SRIO_TRAN_ADDR            0x40000000

#define FSL_SR_OUT_DB1_INT_VEC   45
#define FSL_SR_IN_DB1_INT_VEC    46
#define FSL_SR_OUT_MU1_INT_VEC   49
#define FSL_SR_IN_MU1_INT_VEC    50
#define FSL_SR_OUT_MU2_INT_VEC   51
#define FSL_SR_IN_MU2_INT_VEC    52

#define FSL_WRITE_REG(addr,data)    (*(volatile UINT32 *)(addr) = data)  
#define FSL_READ_REG(addr)          (*(volatile UINT32 *)(addr))


extern STATUS intConnect(VOIDFUNCPTR *vector,VOIDFUNCPTR routine,int parameter);
extern UINT32 rioMapInb(UINT32 localAddr, UINT32 srioAddr, UINT32 size, UINT32 flags);

#endif

