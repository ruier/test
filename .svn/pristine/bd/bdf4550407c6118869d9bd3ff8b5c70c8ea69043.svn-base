REM *******************************************
REM CF CONFIGURATION
REM *******************************************
CF TAR                             8275
CF SB                              SB
CF VECTOR                          LOW
CF RST                             YES
CF LENDIAN                         NO
CF MODE                            64
CF HRESET                          ENABLE
CF BRDHRESET                       DISABLE
CF WSPACE                          00000000 fff
CF STACK                           00000000 ffffffff
CF RSTCONF                         AUTO
CF TGTCONS                         BDM
CF TRESET                          ACTIVE
CF TRGIN                           OFF
CF TRGINFILTER                     OFF
CF TRGOUTMODE                      OFF
CF TRGOUT                          PULSEHI
CF CR                              00000000 40000
CF DLD                             NORMAL
CF CHECKSTOP                       YES
CF SPOWER                          YES
CF PONR                            0
CF RESET                           HRESET
CF TRPEXP                          YES
CF INCOLD                          YES
CF LATRACE                         NONE
CF CLKTYPE                         EXTERNAL
CF MMU                             OFF
CF BRKREP                          BRKREP
CF TMD                             DISABLE
CF CLK                             16
CF AIMMRER                         OFF
CF AIMMRVAL                        0e000000
CF RTP                             NO
CF INVCI                           YES
CF RPL                             1
REM *******************************************
REM SC CONFIGURATION
REM *******************************************
SC GRP ERASE
SCGA SIU              IMMR             000101A8  0F000c00 SIU              /va_dr /ua:0 /sa:0 /ue:0
SCGA SIU              SIUMCR           00010000  0E600000 SIU              /ua:0
SCGA SIU              SYPCR            00010004  FFFFFFC3 SIU              /ua:0
SCGA SIU              SWSR             0001000E  0000     SIU              /ua:0
SCGA SIU              BCR              00010024  100c0000 SIU              /ua:0
SCGA SIU              PPC_ACR          00010028  03       SIU              /ua:0
SCGA SIU              PPC_ALRH         0001002C  01234567 SIU              /ua:0
SCGA SIU              PPC_ALRL         00010030  89ABCDEF SIU              /ua:0
SCGA SIU              LCL_ACR          00010034  89       SIU              /ua:0
SCGA SIU              LCL_ALRH         00010038  01234567 SIU              /ua:0
SCGA SIU              LCL_ALRL         0001003C  89ABCDEF SIU              /ua:0
SCGA SIU              TESCR1           00010040  00004000 SIU              /ua:0
SCGA SIU              TESCR2           00010044  00000000 SIU              /ua:0
SCGA SIU              LTESCR1          00010048  00000000 SIU              /ua:0
SCGA SIU              LTESCR2          0001004C  00000000 SIU              /ua:0
SCGA SIU              PDTEA            00010050  00000000 SIU              /ua:0
SCGA SIU              PDTEM            00010054  00       SIU              /tclr:0 /ua:0
SCGA SIU              LDTEA            00010058  00000000 SIU              /tclr:1 /ua:0
SCGA SIU              LDTEM            0001005C  00       SIU              /tclr:2 /ua:0
SCGA MEMC             OR0              00010104  FF800876 MEMC             /ua:0
SCGA MEMC             BR0              00010100  FF801801 MEMC             /va_dr /ua:0 /sa:1
SCGA MEMC             OR1              0001010C  FFFF8010 MEMC             /ua:0
SCGA MEMC             BR1              00010108  04501801 MEMC             /va_dr /ua:0 /sa:1
SCGA MEMC             OR2              00010114  FF002CC0 MEMC             /ua:0
SCGA MEMC             BR2              00010110  00000041 MEMC             /va_dr /ua:0 /sa:1
SCGA MEMC             OR3              0001011C  FC002CC0 MEMC             /ua:0
SCGA MEMC             BR3              00010118  00000040 MEMC             /va_dr /ua:0 /sa:1
SCGA MEMC             OR4              00010124  FFC01480 MEMC             /ua:0
SCGA MEMC             BR4              00010120  04001860 MEMC             /va_dr /ua:0 /sa:1
SCGA MEMC             OR5              0001012C  FFFF03F6 MEMC             /ua:0
SCGA MEMC             BR5              00010128  22000800 MEMC             /va_dr /ua:0 /sa:1
SCGA MEMC             OR6              00010134  FF000856 MEMC             /ua:0
SCGA MEMC             BR6              00010130  E0001802 MEMC             /va_dr /ua:0 /sa:1
SCGA MEMC             OR7              0001013C  FFFF03F6 MEMC             /ua:0
SCGA MEMC             BR7              00010138  21000800 MEMC             /va_dr /ua:0 /sa:1
SCGA MEMC             OR8              00010144  00000000 MEMC             /ua:0
SCGA MEMC             BR8              00010140  00000000 MEMC             /va_dr /ua:0 /sa:1
SCGA MEMC             OR9              0001014C  FFFF0000 MEMC             /ua:0
SCGA MEMC             BR9              00010148  60000080 MEMC             /va_dr /ua:0 /sa:1
SCGA MEMC             OR10             00010154  FFFF0000 MEMC             /ua:0
SCGA MEMC             BR10             00010150  700000A0 MEMC             /va_dr /ua:0 /sa:1
SCGA MEMC             OR11             0001015C  FFFF0000 MEMC             /ua:0
SCGA MEMC             BR11             00010158  800000C0 MEMC             /va_dr /ua:0 /sa:1
SCGA MEMC             MAR              00010168  00000200 MEMC             /ua:0
SCGA MEMC             MAMR             00010170  00000000 MEMC             /ua:0
SCGA MEMC             MBMR             00010174  00000000 MEMC             /ua:0
SCGA MEMC             MCMR             00010178  00000000 MEMC             /ua:0
SCGA MEMC             MPTPR            00010184  2800     MEMC             /ua:0
SCGA MEMC             MDR              00010188  00000000 MEMC             /ua:0
SCGA MEMC             PSDMR            00010190  C34F36A3 MEMC             /ua:0
SCGA MEMC             LSDMR            00010194  0086a512 MEMC             /ua:0
SCGA MEMC             PURT             00010198  08       MEMC             /ua:0
SCGA MEMC             PSRT             0001019C  08       MEMC             /ua:0
SCGA MEMC             LURT             000101A0  00       MEMC             /ua:0
SCGA MEMC             LSRT             000101A4  13       MEMC             /ua:0
SCGA MEMC             PCIBR0           000101AC  00000000 MEMC             /ua:0
SCGA MEMC             PCIBR1           000101B0  00000000 MEMC             /tclr:3 /ua:0
SCGA MEMC             PCIMSK0          000101C4  00000000 MEMC             /tclr:4 /ua:0
SCGA MEMC             PCIMSK1          000101C8  00000000 MEMC             /tclr:5 /ua:0
SCGA UPMA             UPMA_T0          00010188  FFFFFFFF UPMA             /tclr:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T1          00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T2          00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T3          00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T4          00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T5          00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T6          00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T7          00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T8          00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T9          00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T10         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T11         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T12         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T13         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T14         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T15         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T16         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T17         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T18         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T19         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T20         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T21         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T22         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T23         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T24         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T25         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T26         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T27         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T28         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T29         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T30         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T31         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T32         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T33         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T34         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T35         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T36         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T37         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T38         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T39         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T40         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T41         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T42         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T43         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T44         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T45         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T46         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T47         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T48         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T49         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T50         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T51         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T52         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T53         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T54         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T55         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T56         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T57         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T58         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T59         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T60         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T61         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T62         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMA             UPMA_T63         00010188  FFFFFFFF UPMA             /tp1:3 /ua:0 /ue:3
SCGA UPMB             UPMB_T0          00010188  FFFFFFFF UPMB             /tclr:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T1          00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T2          00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T3          00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T4          00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T5          00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T6          00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T7          00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T8          00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T9          00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T10         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T11         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T12         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T13         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T14         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T15         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T16         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T17         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T18         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T19         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T20         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T21         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T22         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T23         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T24         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T25         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T26         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T27         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T28         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T29         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T30         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T31         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T32         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T33         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T34         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T35         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T36         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T37         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T38         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T39         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T40         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T41         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T42         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T43         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T44         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T45         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T46         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T47         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T48         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T49         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T50         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T51         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T52         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T53         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T54         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T55         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T56         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T57         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T58         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T59         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T60         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T61         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T62         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMB             UPMB_T63         00010188  FFFFFFFF UPMB             /tp1:4 /ua:0 /ue:4
SCGA UPMC             UPMC_T0          00010188  FFFFFFFF UPMC             /tclr:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T1          00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T2          00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T3          00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T4          00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T5          00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T6          00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T7          00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T8          00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T9          00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T10         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T11         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T12         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T13         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T14         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T15         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T16         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T17         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T18         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T19         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T20         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T21         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T22         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T23         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T24         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T25         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T26         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T27         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T28         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T29         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T30         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T31         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T32         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T33         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T34         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T35         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T36         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T37         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T38         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T39         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T40         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T41         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T42         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T43         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T44         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T45         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T46         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T47         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T48         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T49         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T50         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T51         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T52         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T53         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T54         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T55         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T56         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T57         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T58         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T59         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T60         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T61         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T62         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA UPMC             UPMC_T63         00010188  FFFFFFFF UPMC             /tp1:5 /ua:0 /ue:5
SCGA TIMERS           TMCNTSC          00010220  0000     TIMERS           /ua:0
SCGA TIMERS           TMCNT            00010224  00000000 TIMERS           /ua:0
SCGA TIMERS           TMCNTAL          0001022C  00000000 TIMERS           /ua:0
SCGA TIMERS           PISCR            00010240  0000     TIMERS           /ua:0
SCGA TIMERS           PITC             00010244  00000000 TIMERS           /ua:0
SCGA TIMERS           PITR             00010248  00000000 TIMERS           /ua:0
SCGA CLOCKS           SCCR             00010C80  00000119 CLOCKS           /ua:0
SCGA CLOCKS           SCMR             00010C88  0831C001 CLOCKS           /ua:0
SCGA CLOCKS           RSR              00010C90  00000003 CLOCKS           /ua:0
SCGA CLOCKS           RMR              00010C94  00000000 CLOCKS           /ua:0
SCGA CLOCKS           DLLMR            00010C98  00000000 CLOCKS           /ua:0
SCGA CLOCKS           DLLST            00010C9C  00000000 CLOCKS           /ua:0
SCGA SIC              SICR             00010C00  0000     SIC              /ua:0
SCGA SIC              SIVEC            00010C04  00000000 SIC              /ua:0
SCGA SIC              SIPNR_H          00010C08  00000100 SIC              /ua:0
SCGA SIC              SIPNR_L          00010C0C  00000000 SIC              /ua:0
SCGA SIC              SIPRR            00010C10  05309770 SIC              /ua:0
SCGA SIC              SCPRR_H          00010C14  05309770 SIC              /ua:0
SCGA SIC              SCPRR_L          00010C18  05309770 SIC              /ua:0
SCGA SIC              SIMR_H           00010C1C  00000000 SIC              /ua:0
SCGA SIC              SIMR_L           00010C20  00000000 SIC              /ua:0
SCGA SIC              SIEXR            00010C24  00000000 SIC              /ua:0
SCGA IOPORTS          PDIRA            00010D00  00000000 IOPORTS          /ua:0
SCGA IOPORTS          PPARA            00010D04  00000000 IOPORTS          /ua:0
SCGA IOPORTS          PSORA            00010D08  00000000 IOPORTS          /ua:0
SCGA IOPORTS          PODRA            00010D0C  00000000 IOPORTS          /ua:0
SCGA IOPORTS          PDATA            00010D10  00000000 IOPORTS          /ua:0
SCGA IOPORTS          PDIRB            00010D20  00000000 IOPORTS          /ua:0
SCGA IOPORTS          PPARB            00010D24  00000000 IOPORTS          /ua:0
SCGA IOPORTS          PSORB            00010D28  00000000 IOPORTS          /ua:0
SCGA IOPORTS          PODRB            00010D2C  00000000 IOPORTS          /ua:0
SCGA IOPORTS          PDATB            00010D30  00000000 IOPORTS          /ua:0
SCGA IOPORTS          PDIRC            00010D40  00000000 IOPORTS          /ua:0
SCGA IOPORTS          PPARC            00010D44  00000000 IOPORTS          /ua:0
SCGA IOPORTS          PSORC            00010D48  00000000 IOPORTS          /ua:0
SCGA IOPORTS          PODRC            00010D4C  00000000 IOPORTS          /ua:0
SCGA IOPORTS          PDATC            00010D50  00000000 IOPORTS          /ua:0
SCGA IOPORTS          PDIRD            00010D60  00000000 IOPORTS          /ua:0
SCGA IOPORTS          PPARD            00010D64  00000000 IOPORTS          /ua:0
SCGA IOPORTS          PSORD            00010D68  00000000 IOPORTS          /ua:0
SCGA IOPORTS          PODRD            00010D6C  00000000 IOPORTS          /ua:0
SCGA IOPORTS          PDATD            00010D70  00030000 IOPORTS          /ua:0
SCGA CPMTIMERS        TGCR1            00010D80  00       CPMTIMERS        /ua:0
SCGA CPMTIMERS        TGCR2            00010D84  00       CPMTIMERS        /ua:0
SCGA CPMTIMERS        TMR1             00010D90  0000     CPMTIMERS        /ua:0
SCGA CPMTIMERS        TMR2             00010D92  0000     CPMTIMERS        /ua:0
SCGA CPMTIMERS        TRR1             00010D94  FFFF     CPMTIMERS        /ua:0
SCGA CPMTIMERS        TRR2             00010D96  FFFF     CPMTIMERS        /ua:0
SCGA CPMTIMERS        TCR1             00010D98  0000     CPMTIMERS        /ua:0
SCGA CPMTIMERS        TCR2             00010D9A  0000     CPMTIMERS        /ua:0
SCGA CPMTIMERS        TCN1             00010D9C  0000     CPMTIMERS        /ua:0
SCGA CPMTIMERS        TCN2             00010D9E  0000     CPMTIMERS        /ua:0
SCGA CPMTIMERS        TMR3             00010DA0  0000     CPMTIMERS        /ua:0
SCGA CPMTIMERS        TMR4             00010DA2  0000     CPMTIMERS        /ua:0
SCGA CPMTIMERS        TRR3             00010DA4  FFFF     CPMTIMERS        /ua:0
SCGA CPMTIMERS        TRR4             00010DA6  FFFF     CPMTIMERS        /ua:0
SCGA CPMTIMERS        TCR3             00010DA8  0000     CPMTIMERS        /ua:0
SCGA CPMTIMERS        TCR4             00010DAA  0000     CPMTIMERS        /ua:0
SCGA CPMTIMERS        TCN3             00010DAC  0000     CPMTIMERS        /ua:0
SCGA CPMTIMERS        TCN4             00010DAE  0000     CPMTIMERS        /ua:0
SCGA CPMTIMERS        TER1             00010DB0  0000     CPMTIMERS        /ua:0
SCGA CPMTIMERS        TER2             00010DB2  0000     CPMTIMERS        /ua:0
SCGA CPMTIMERS        TER3             00010DB4  0000     CPMTIMERS        /ua:0
SCGA CPMTIMERS        TER4             00010DB6  0000     CPMTIMERS        /ua:0
SCGA PCI              MAP              01234567  0F010900 PCI              /no_addr /no_in /hide /tld:0 /ue:6
SCGA PCI              XOFFSET          76543210  00000004 PCI              /no_addr /no_in /hide /tld:1 /ue:6
SCGA PCI              ANCHOR0          89ABCDEF  00000000 PCI              /no_addr /no_in /hide /sa:4 /ue:6
SCGA PCI              ANCHOR1          FEDCBA98  00000000 PCI              /no_addr /no_in /hide /sa:5 /ue:6
SCGA PCI              VENDORID         00000000  1057     PCI              /r(nwf) /w(nwf) /lendian /r /ua:5
SCGA PCI              ADDR_00          00000000  80000000 PCI              /wo /hide /lendian /w /ua:4
SCGA PCI              DEVICEID         00000002  18C0     PCI              /r(nwf) /w(nwf) /lendian /r /ua:5
SCGA PCI              ADDR_02          00000000  80000002 PCI              /wo /hide /lendian /w /ua:4
SCGA PCI              PCICMD           00000000  0006     PCI              /r(nwf) /w(nwf) /lendian /ua:5
SCGA PCI              ADDR_04          00000000  80000004 PCI              /wo /hide /lendian /w /ua:4
SCGA PCI              PCISTAT          00000002  00B0     PCI              /r(nwf) /w(nwf) /lendian /r /ua:5
SCGA PCI              ADDR_06          00000000  80000006 PCI              /wo /hide /lendian /w /ua:4
SCGA PCI              REVID            00000000  40       PCI              /r(nwf) /w(nwf) /lendian /r /ua:5
SCGA PCI              ADDR_08          00000000  80000008 PCI              /wo /hide /lendian /w /ua:4
SCGA PCI              PIR              00000001  00       PCI              /r(nwf) /w(nwf) /lendian /r /ua:5
SCGA PCI              ADDR_09          00000000  80000009 PCI              /wo /hide /lendian /w /ua:4
SCGA PCI              SUBCCODE         00000002  00       PCI              /r(nwf) /w(nwf) /lendian /r /ua:5
SCGA PCI              ADDR_0A          00000000  8000000A PCI              /wo /hide /lendian /w /ua:4
SCGA PCI              PBCCR            00000003  06       PCI              /r(nwf) /w(nwf) /lendian /r /ua:5
SCGA PCI              ADDR_0B          00000000  8000000B PCI              /wo /hide /lendian /w /ua:4
SCGA PCI              PCLSR            00000000  00       PCI              /r(nwf) /w(nwf) /lendian /ua:5
SCGA PCI              ADDR_0C          00000000  8000000C PCI              /wo /hide /lendian /w /ua:4
SCGA PCI              PLTR             00000001  00       PCI              /r(nwf) /w(nwf) /lendian /ua:5
SCGA PCI              ADDR_0D          00000000  8000000D PCI              /wo /hide /lendian /w /ua:4
SCGA PCI              HTR              00000002  00       PCI              /r(nwf) /w(nwf) /lendian /r /ua:5
SCGA PCI              ADDR_0E          00000000  8000000E PCI              /wo /hide /lendian /w /ua:4
SCGA PCI              BISTCTRL         00000003  00       PCI              /r(nwf) /w(nwf) /lendian /r /ua:5
SCGA PCI              ADDR_0F          00000000  8000000F PCI              /wo /hide /lendian /w /ua:4
SCGA PCI              PIMMRBAR         00000000  00000000 PCI              /r(nwf) /w(nwf) /lendian /ua:5
SCGA PCI              ADDR_10          00000000  80000010 PCI              /wo /hide /lendian /w /ua:4
SCGA PCI              GPLABAR0         00000000  00000000 PCI              /r(nwf) /w(nwf) /lendian /ua:5
SCGA PCI              ADDR_14          00000000  80000014 PCI              /wo /hide /lendian /w /ua:4
SCGA PCI              GPLABAR1         00000000  00000000 PCI              /r(nwf) /w(nwf) /lendian /ua:5
SCGA PCI              ADDR_18          00000000  80000018 PCI              /wo /hide /lendian /w /ua:4
SCGA PCI              SUBVENDOR        00000000  0000     PCI              /r(nwf) /w(nwf) /lendian /ua:5
SCGA PCI              ADDR_2c          00000000  8000002C PCI              /wo /hide /lendian /w /ua:4
SCGA PCI              SUBID            00000002  0000     PCI              /r(nwf) /w(nwf) /lendian /ua:5
SCGA PCI              ADDR_2e          00000000  8000002E PCI              /wo /hide /lendian /w /ua:4
SCGA PCI              CP               00000000  48       PCI              /r(nwf) /w(nwf) /lendian /r /ua:5
SCGA PCI              ADDR_34          00000000  80000034 PCI              /wo /hide /lendian /w /ua:4
SCGA PCI              ILR              00000000  00       PCI              /r(nwf) /w(nwf) /lendian /ua:5
SCGA PCI              ADDR_3C          00000000  8000003C PCI              /wo /hide /lendian /w /ua:4
SCGA PCI              INTPIN           00000001  01       PCI              /r(nwf) /w(nwf) /lendian /r /ua:5
SCGA PCI              ADDR_3D          00000000  8000003D PCI              /wo /hide /lendian /w /ua:4
SCGA PCI              MIN_GNT          00000002  00       PCI              /r(nwf) /w(nwf) /lendian /r /ua:5
SCGA PCI              ADDR_3E          00000000  8000003E PCI              /wo /hide /lendian /w /ua:4
SCGA PCI              MAX_LAT          00000003  00       PCI              /r(nwf) /w(nwf) /lendian /r /ua:5
SCGA PCI              ADDR_3F          00000000  8000003F PCI              /wo /hide /lendian /w /ua:4
SCGA PCI              PCIBFR           00000000  0000     PCI              /r(nwf) /w(nwf) /lendian /ua:5
SCGA PCI              ADDR_44          00000000  80000044 PCI              /wo /hide /lendian /w /ua:4
SCGA PCI              PBACR            00000002  8000     PCI              /r(nwf) /w(nwf) /lendian /ua:5
SCGA PCI              ADDR_46          00000000  80000046 PCI              /wo /hide /lendian /w /ua:4
SCGA PCI              PCIHSRB          00000000  00000006 PCI              /r(nwf) /w(nwf) /lendian /ua:5
SCGA PCI              ADDR_48          00000000  80000048 PCI              /wo /hide /lendian /w /ua:4
SCGA PCI              PMCR             00010870  00000000 PCI              /lendian /ua:0
SCGA PCI              DTCR             00010878  00000000 PCI              /lendian /ua:0
SCGA PCI              GPCR             0001087C  00000000 PCI              /lendian /ua:0
SCGA PCI              PCI_GCR          00010880  00000000 PCI              /lendian /ua:0
SCGA PCI              ESR              00010884  00000000 PCI              /lendian /ua:0
SCGA PCI              EMR              00010888  000000FF PCI              /lendian /ua:0
SCGA PCI              ECR              0001088C  000000FF PCI              /lendian /ua:0
SCGA PCI              PCIEACR          00010890  00000000 PCI              /lendian /ua:0
SCGA PCI              PCIEDCR          00010898  00000000 PCI              /lendian /ua:0
SCGA PCI              PCIECCR          000108A0  00000000 PCI              /lendian /ua:0
SCGA PCI              PCICFGADDR       00010900  00000000 PCI              /lendian /w /ua:0
SCGA PCI              PCICFGDATA       00010904  00000000 PCI              /lendian /ua:0
SCGA PCI              INTACK           00010908  00000000 PCI              /lendian /r /ua:0
SCGA ATU              OTAR0            00010800  00000000 ATU              /lendian /ua:0
SCGA ATU              OBAR0            00010808  00000000 ATU              /lendian /ua:0
SCGA ATU              OCMR0            00010810  00000000 ATU              /lendian /ua:0
SCGA ATU              OTAR1            00010818  00000000 ATU              /lendian /ua:0
SCGA ATU              OBAR1            00010820  00000000 ATU              /lendian /ua:0
SCGA ATU              OCMR1            00010828  00000000 ATU              /lendian /ua:0
SCGA ATU              OTAR2            00010830  00000000 ATU              /lendian /ua:0
SCGA ATU              OBAR2            00010838  00000000 ATU              /lendian /ua:0
SCGA ATU              OCMR2            00010840  00000000 ATU              /lendian /ua:0
SCGA ATU              ITAR0            000108E8  00000000 ATU              /lendian /ua:0
SCGA ATU              IBAR0            000108F0  00000000 ATU              /lendian /ua:0
SCGA ATU              ICMR0            000108F8  00000000 ATU              /lendian /ua:0
SCGA ATU              ITAR1            000108D0  00000000 ATU              /lendian /ua:0
SCGA ATU              IBAR1            000108D8  00000000 ATU              /lendian /ua:0
SCGA ATU              ICMR1            000108E0  00000000 ATU              /lendian /ua:0
SCGA I2O              OISR             00010430  00000000 I2O              /lendian /ua:0
SCGA I2O              OIMR             00010434  00000000 I2O              /lendian /ua:0
SCGA I2O              IFQPR            00010440  00000000 I2O              /lendian /ua:0
SCGA I2O              OFQPR            00010444  00000000 I2O              /lendian /ua:0
SCGA I2O              IMR0             00010450  00000000 I2O              /lendian /ua:0
SCGA I2O              IMR1             00010454  00000000 I2O              /lendian /ua:0
SCGA I2O              OMR0             00010458  00000000 I2O              /lendian /ua:0
SCGA I2O              OMR1             0001045C  00000000 I2O              /lendian /ua:0
SCGA I2O              ODBR             00010460  00000000 I2O              /lendian /ua:0
SCGA I2O              IDBR             00010468  00000000 I2O              /lendian /ua:0
SCGA I2O              IMISR            00010480  00000000 I2O              /lendian /ua:0
SCGA I2O              IMIMR            00010484  00000000 I2O              /lendian /ua:0
SCGA I2O              IFHPR            000104A0  00000000 I2O              /lendian /ua:0
SCGA I2O              IFTPR            000104A8  00000000 I2O              /lendian /ua:0
SCGA I2O              IPHPR            000104B0  00000000 I2O              /lendian /ua:0
SCGA I2O              IPTPR            000104B8  00000000 I2O              /lendian /ua:0
SCGA I2O              OFHPR            000104C0  00000000 I2O              /lendian /ua:0
SCGA I2O              OFTPR            000104C8  00000000 I2O              /lendian /ua:0
SCGA I2O              OPHPR            000104D0  00000000 I2O              /lendian /ua:0
SCGA I2O              OPTPR            000104D8  00000000 I2O              /lendian /ua:0
SCGA I2O              MUCR             000104E4  00000000 I2O              /lendian /ua:0
SCGA I2O              QBAR             000104F0  00000000 I2O              /lendian /ua:0
SCGA DMA              DMR0             00010500  00000000 DMA              /lendian /ua:0
SCGA DMA              SR0              00010504  00000000 DMA              /lendian /ua:0
SCGA DMA              CDAR0            00010508  00000000 DMA              /lendian /ua:0
SCGA DMA              SAR0             00010510  00000000 DMA              /lendian /ua:0
SCGA DMA              DAR0             00010518  00000000 DMA              /lendian /ua:0
SCGA DMA              BCR0             00010520  00000000 DMA              /lendian /ua:0
SCGA DMA              NDAR0            00010524  00000000 DMA              /lendian /ua:0
SCGA DMA              DMR1             00010580  00000000 DMA              /lendian /ua:0
SCGA DMA              SR1              00010584  00000000 DMA              /lendian /ua:0
SCGA DMA              CDAR1            00010588  00000000 DMA              /lendian /ua:0
SCGA DMA              SAR1             00010590  00000000 DMA              /lendian /ua:0
SCGA DMA              DAR1             00010598  00000000 DMA              /lendian /ua:0
SCGA DMA              BCR1             000105A0  00000000 DMA              /lendian /ua:0
SCGA DMA              NDAR1            000105A4  00000000 DMA              /lendian /ua:0
SCGA DMA              DMR2             00010600  00000000 DMA              /lendian /ua:0
SCGA DMA              SR2              00010604  00000000 DMA              /lendian /ua:0
SCGA DMA              CDAR2            00010608  00000000 DMA              /lendian /ua:0
SCGA DMA              SAR2             00010610  00000000 DMA              /lendian /ua:0
SCGA DMA              DAR2             00010618  00000000 DMA              /lendian /ua:0
SCGA DMA              BCR2             00010620  00000000 DMA              /lendian /ua:0
SCGA DMA              NDAR2            00010624  00000000 DMA              /lendian /ua:0
SCGA DMA              DMR3             00010680  00000000 DMA              /lendian /ua:0
SCGA DMA              SR3              00010684  00000000 DMA              /lendian /ua:0
SCGA DMA              CDAR3            00010688  00000000 DMA              /lendian /ua:0
SCGA DMA              SAR3             00010690  00000000 DMA              /lendian /ua:0
SCGA DMA              DAR3             00010698  00000000 DMA              /lendian /ua:0
SCGA DMA              BCR3             000106A0  00000000 DMA              /lendian /ua:0
SCGA DMA              NDAR3            000106A4  00000000 DMA              /lendian /ua:0
SCGA FCC              GFMR1            00011300  00000000 FCC              /ua:0
SCGA FCC              FPSMR1           00011304  00000000 FCC              /ua:0
SCGA FCC              FTODR1           00011308  0000     FCC              /ua:0
SCGA FCC              FDSR1            0001130C  7E7E     FCC              /ua:0
SCGA FCC              FCCE1            00011310  00000000 FCC              /ua:0
SCGA FCC              FCCM1            00011314  00000000 FCC              /ua:0
SCGA FCC              FCCS1            00011318  00       FCC              /ua:0
SCGA FCC              FTIRR1           0001131C  00000000 FCC              /ua:0
SCGA FCC              GFMR2            00011320  00000000 FCC              /ua:0
SCGA FCC              FPSMR2           00011324  00000000 FCC              /ua:0
SCGA FCC              FTODR2           00011328  0000     FCC              /ua:0
SCGA FCC              FDSR2            0001132C  7E7E     FCC              /ua:0
SCGA FCC              FCCE2            00011330  00000000 FCC              /ua:0
SCGA FCC              FCCM2            00011334  00000000 FCC              /ua:0
SCGA FCC              FCCS2            00011338  00       FCC              /ua:0
SCGA FCC              FTIRR2           0001133C  00000000 FCC              /ua:0
SCGA FCC              GFMR3            00011340  00000000 FCC              /ua:0
SCGA FCC              FPSMR3           00011344  00000000 FCC              /ua:0
SCGA FCC              FTODR3           00011348  0000     FCC              /ua:0
SCGA FCC              FDSR3            0001134C  7E7E     FCC              /ua:0
SCGA FCC              FCCE3            00011350  00000000 FCC              /ua:0
SCGA FCC              FCCM3            00011354  00000000 FCC              /ua:0
SCGA FCC              FCCS3            00011358  00       FCC              /ua:0
SCGA TCL              TCMODE1          00011400  0000     TCL              /ua:0
SCGA TCL              CDSMR1           00011402  0000     TCL              /ua:0
SCGA TCL              TCER1            00011404  0000     TCL              /ua:0
SCGA TCL              TC_RCC1          00011406  0000     TCL              /ua:0
SCGA TCL              TCMR1            00011408  0000     TCL              /ua:0
SCGA TCL              TC_FCC1          0001140A  0000     TCL              /ua:0
SCGA TCL              TC_CCC1          0001140C  0000     TCL              /ua:0
SCGA TCL              TC_ICC1          0001140E  0000     TCL              /ua:0
SCGA TCL              TC_TCC1          00011410  0000     TCL              /ua:0
SCGA TCL              TC_ECC1          00011412  0000     TCL              /ua:0
SCGA TCL              TCMODE2          00011420  0000     TCL              /ua:0
SCGA TCL              CDSMR2           00011422  0000     TCL              /ua:0
SCGA TCL              TCER2            00011424  0000     TCL              /ua:0
SCGA TCL              TC_RCC2          00011426  0000     TCL              /ua:0
SCGA TCL              TCMR2            00011428  0000     TCL              /ua:0
SCGA TCL              TC_FCC2          0001142A  0000     TCL              /ua:0
SCGA TCL              TC_CCC2          0001142C  0000     TCL              /ua:0
SCGA TCL              TC_ICC2          0001142E  0000     TCL              /ua:0
SCGA TCL              TC_TCC2          00011430  0000     TCL              /ua:0
SCGA TCL              TC_ECC2          00011432  0000     TCL              /ua:0
SCGA TCL              TCMODE3          00011440  0000     TCL              /ua:0
SCGA TCL              CDSMR3           00011442  0000     TCL              /ua:0
SCGA TCL              TCER3            00011444  0000     TCL              /ua:0
SCGA TCL              TC_RCC3          00011446  0000     TCL              /ua:0
SCGA TCL              TCMR3            00011448  0000     TCL              /ua:0
SCGA TCL              TC_FCC3          0001144A  0000     TCL              /ua:0
SCGA TCL              TC_CCC3          0001144C  0000     TCL              /ua:0
SCGA TCL              TC_ICC3          0001144E  0000     TCL              /ua:0
SCGA TCL              TC_TCC3          00011450  0000     TCL              /ua:0
SCGA TCL              TC_ECC3          00011452  0000     TCL              /ua:0
SCGA TCL              TCMODE4          00011460  0000     TCL              /ua:0
SCGA TCL              CDSMR4           00011462  0000     TCL              /ua:0
SCGA TCL              TCER4            00011464  0000     TCL              /ua:0
SCGA TCL              TC_RCC4          00011466  0000     TCL              /ua:0
SCGA TCL              TCMR4            00011468  0000     TCL              /ua:0
SCGA TCL              TC_FCC4          0001146A  0000     TCL              /ua:0
SCGA TCL              TC_CCC4          0001146C  0000     TCL              /ua:0
SCGA TCL              TC_ICC4          0001146E  0000     TCL              /ua:0
SCGA TCL              TC_TCC4          00011470  0000     TCL              /ua:0
SCGA TCL              TC_ECC4          00011472  0000     TCL              /ua:0
SCGA TCL              TCMODE5          00011480  0000     TCL              /ua:0
SCGA TCL              CDSMR5           00011482  0000     TCL              /ua:0
SCGA TCL              TCER5            00011484  0000     TCL              /ua:0
SCGA TCL              TC_RCC5          00011486  0000     TCL              /ua:0
SCGA TCL              TCMR5            00011488  0000     TCL              /ua:0
SCGA TCL              TC_FCC5          0001148A  0000     TCL              /ua:0
SCGA TCL              TC_CCC5          0001148C  0000     TCL              /ua:0
SCGA TCL              TC_ICC5          0001148E  0000     TCL              /ua:0
SCGA TCL              TC_TCC5          00011490  0000     TCL              /ua:0
SCGA TCL              TC_ECC5          00011492  0000     TCL              /ua:0
SCGA TCL              TCMODE6          000114A0  0000     TCL              /ua:0
SCGA TCL              CDSMR6           000114A2  0000     TCL              /ua:0
SCGA TCL              TCER6            000114A4  0000     TCL              /ua:0
SCGA TCL              TC_RCC6          000114A6  0000     TCL              /ua:0
SCGA TCL              TCMR6            000114A8  0000     TCL              /ua:0
SCGA TCL              TC_FCC6          000114AA  0000     TCL              /ua:0
SCGA TCL              TC_CCC6          000114AC  0000     TCL              /ua:0
SCGA TCL              TC_ICC6          000114AE  0000     TCL              /ua:0
SCGA TCL              TC_TCC6          000114B0  0000     TCL              /ua:0
SCGA TCL              TC_ECC6          000114B2  0000     TCL              /ua:0
SCGA TCL              TCMODE7          000114C0  0000     TCL              /ua:0
SCGA TCL              CDSMR7           000114C2  0000     TCL              /ua:0
SCGA TCL              TCER7            000114C4  0000     TCL              /ua:0
SCGA TCL              TC_RCC7          000114C6  0000     TCL              /ua:0
SCGA TCL              TCMR7            000114C8  0000     TCL              /ua:0
SCGA TCL              TC_FCC7          000114CA  0000     TCL              /ua:0
SCGA TCL              TC_CCC7          000114CC  0000     TCL              /ua:0
SCGA TCL              TC_ICC7          000114CE  0000     TCL              /ua:0
SCGA TCL              TC_TCC7          000114D0  0000     TCL              /ua:0
SCGA TCL              TC_ECC7          000114D2  0000     TCL              /ua:0
SCGA TCL              TCMODE8          000114E0  0000     TCL              /ua:0
SCGA TCL              CDSMR8           000114E2  0000     TCL              /ua:0
SCGA TCL              TCER8            000114E4  0000     TCL              /ua:0
SCGA TCL              TC_RCC8          000114E6  0000     TCL              /ua:0
SCGA TCL              TCMR8            000114E8  0000     TCL              /ua:0
SCGA TCL              TC_FCC8          000114EA  0000     TCL              /ua:0
SCGA TCL              TC_CCC8          000114EC  0000     TCL              /ua:0
SCGA TCL              TC_ICC8          000114EE  0000     TCL              /ua:0
SCGA TCL              TC_TCC8          000114F0  0000     TCL              /ua:0
SCGA TCL              TC_ECC8          000114F2  0000     TCL              /ua:0
SCGA TCL              TCGSR            00011500  0000     TCL              /r /ua:0
SCGA TCL              TCGER            00011502  0000     TCL              /ua:0
SCGA I2C              I2MOD            00011860  00       I2C              /ua:0
SCGA I2C              I2ADD            00011864  00       I2C              /ua:0
SCGA I2C              I2BRG            00011868  FF       I2C              /ua:0
SCGA I2C              I2COM            0001186C  00       I2C              /ua:0
SCGA I2C              I2CER            00011870  00       I2C              /ua:0
SCGA I2C              I2CMR            00011874  00       I2C              /ua:0
SCGA COMM             CPCR             000119C0  00000000 COMM             /ua:0
SCGA COMM             RCCR             000119C4  0000     COMM             /ua:0
SCGA COMM             RMDR             000119C8  00000000 COMM             /ua:0
SCGA COMM             RMDS             000119DE  0000     COMM             /ua:0
SCGA COMM             RCTR1            000119CC  0000     COMM             /ua:0
SCGA COMM             RCTR2            000119CE  0000     COMM             /ua:0
SCGA COMM             RCTR3            000119D0  0000     COMM             /ua:0
SCGA COMM             RCTR4            000119D2  0000     COMM             /ua:0
SCGA COMM             RTER             000119D6  0000     COMM             /ua:0
SCGA COMM             RTMR             000119DA  0000     COMM             /ua:0
SCGA COMM             RTSCR            000119DC  0000     COMM             /ua:0
SCGA COMM             BRGC1            000119F0  00000000 COMM             /ua:0
SCGA COMM             BRGC2            000119F4  00000000 COMM             /ua:0
SCGA COMM             BRGC3            000119F8  00000000 COMM             /ua:0
SCGA COMM             BRGC4            000119FC  00000000 COMM             /ua:0
SCGA COMM             BRGC5            000115F0  00000000 COMM             /ua:0
SCGA COMM             BRGC6            000115F4  00000000 COMM             /ua:0
SCGA COMM             BRGC7            000115F8  00000000 COMM             /ua:0
SCGA COMM             BRGC8            000115FC  00000000 COMM             /ua:0
SCGA COMM             GSMR_L1          00011A00  00000000 COMM             /ua:0
SCGA COMM             GSMR_H1          00011A04  00000000 COMM             /ua:0
SCGA COMM             PSMR1            00011A08  0000     COMM             /ua:0
SCGA COMM             TODR1            00011A0C  0000     COMM             /ua:0
SCGA COMM             DSR1             00011A0E  7E7E     COMM             /ua:0
SCGA COMM             SCCE1            00011A10  0000     COMM             /ua:0
SCGA COMM             SCCM1            00011A14  0000     COMM             /ua:0
SCGA COMM             SCCS1            00011A17  00       COMM             /ua:0
SCGA COMM             GSMR_L2          00011A20  00000000 COMM             /ua:0
SCGA COMM             GSMR_H2          00011A24  00000000 COMM             /ua:0
SCGA COMM             PSMR2            00011A28  0000     COMM             /ua:0
SCGA COMM             TODR2            00011A2C  0000     COMM             /ua:0
SCGA COMM             DSR2             00011A2E  7E7E     COMM             /ua:0
SCGA COMM             SCCE2            00011A30  0000     COMM             /ua:0
SCGA COMM             SCCM2            00011A34  0000     COMM             /ua:0
SCGA COMM             SCCS2            00011A37  00       COMM             /ua:0
SCGA COMM             GSMR_L3          00011A40  00000000 COMM             /ua:0
SCGA COMM             GSMR_H3          00011A44  00000000 COMM             /ua:0
SCGA COMM             PSMR3            00011A48  0000     COMM             /ua:0
SCGA COMM             TODR3            00011A4C  0000     COMM             /ua:0
SCGA COMM             DSR3             00011A4E  7E7E     COMM             /ua:0
SCGA COMM             SCCE3            00011A50  0000     COMM             /ua:0
SCGA COMM             SCCM3            00011A54  0000     COMM             /ua:0
SCGA COMM             SCCS3            00011A57  00       COMM             /ua:0
SCGA COMM             GSMR_L4          00011A60  00000000 COMM             /ua:0
SCGA COMM             GSMR_H4          00011A64  00000000 COMM             /ua:0
SCGA COMM             PSMR4            00011A68  0000     COMM             /ua:0
SCGA COMM             TODR4            00011A6C  0000     COMM             /ua:0
SCGA COMM             DSR4             00011A6E  7E7E     COMM             /ua:0
SCGA COMM             SCCE4            00011A70  0000     COMM             /ua:0
SCGA COMM             SCCM4            00011A74  0000     COMM             /ua:0
SCGA COMM             SCCS4            00011A77  00       COMM             /ua:0
SCGA COMM             SMCMR1           00011A82  0000     COMM             /ua:0
SCGA COMM             SMCE1            00011A86  00       COMM             /ua:0
SCGA COMM             SMCM1            00011A8A  00       COMM             /ua:0
SCGA COMM             SMCMR2           00011A92  0000     COMM             /ua:0
SCGA COMM             SMCM2            00011A9A  00       COMM             /ua:0
SCGA COMM             SMCE2            00011A96  00       COMM             /ua:0
SCGA SPI              SPMODE           00011AA0  0200     SPI              /ua:0
SCGA SPI              SPIE             00011AA6  00       SPI              /ua:0
SCGA SPI              SPIM             00011AAA  00       SPI              /ua:0
SCGA SPI              SPCOM            00011AAD  00       SPI              /ua:0
SCGA SI               SI1AMR           00011B20  0000     SI               /ua:0
SCGA SI               SI1BMR           00011B22  0000     SI               /ua:0
SCGA SI               SI1CMR           00011B24  0000     SI               /ua:0
SCGA SI               SI1DMR           00011B26  0000     SI               /ua:0
SCGA SI               SI1GMR           00011B28  00       SI               /ua:0
SCGA SI               SI1CMDR          00011B2A  00       SI               /ua:0
SCGA SI               SI1STR           00011B2C  00       SI               /ua:0
SCGA SI               SI1RSR           00011B2E  0000     SI               /ua:0
SCGA SI               MCCE1            00011B30  0000     SI               /ua:0
SCGA SI               MCCM1            00011B34  0000     SI               /ua:0
SCGA SI               MCCF1            00011B38  00       SI               /ua:0
SCGA SI               SI2AMR           00011B40  0000     SI               /ua:0
SCGA SI               SI2BMR           00011B42  0000     SI               /ua:0
SCGA SI               SI2CMR           00011B44  0000     SI               /ua:0
SCGA SI               SI2DMR           00011B46  0000     SI               /ua:0
SCGA SI               SI2GMR           00011B48  00       SI               /ua:0
SCGA SI               SI2CMDR          00011B4A  00       SI               /ua:0
SCGA SI               SI2STR           00011B4C  00       SI               /ua:0
SCGA SI               SI2RSR           00011B4E  0000     SI               /ua:0
SCGA SI               MCCE2            00011B50  0000     SI               /ua:0
SCGA SI               MCCM2            00011B54  0000     SI               /ua:0
SCGA SI               MCCF2            00011B58  00       SI               /ua:0
SCGA CPMUX            CMXSI1CR         00011B00  00       CPMUX            /ua:0
SCGA CPMUX            CMXSI2CR         00011B02  00       CPMUX            /ua:0
SCGA CPMUX            CMXFCR           00011B04  00000000 CPMUX            /ua:0
SCGA CPMUX            CMXSCR           00011B08  00000000 CPMUX            /ua:0
SCGA CPMUX            CMXSMR           00011B0C  00       CPMUX            /ua:0
SCGA CPMUX            CMXUAR           00011B0E  0000     CPMUX            /ua:0
SCGA ESTSDRAM         SDRMR            00010C94  0000     ESTSDRAM         /wo /w /ua:0
SCGA ESTSDRAM         SDPSRT           0001019C  13       ESTSDRAM         /wo /w /ua:0
SCGA ESTSDRAM         PSDMR1           00010190  834f36a3 ESTSDRAM         /wo /w /ua:0
SCGA ESTSDRAM         PSDMR2           00010190  834f36a3 ESTSDRAM         /wo /w /ua:0
SCGA ESTSDRAM         PSDMR3           00010190  ab4f36a3 ESTSDRAM         /wo /w /ua:0
SCGA ESTSDRAM         MEM1             00000000  FF       ESTSDRAM         /wo /w
SCGA ESTSDRAM         PSDMR4           00010190  ab4f36A3 ESTSDRAM         /wo /w /ua:0
SCGA ESTSDRAM         PSDMR5           00010190  8b4f36A3 ESTSDRAM         /wo /w /ua:0
SCGA ESTSDRAM         MEM2             00000000  FF       ESTSDRAM         /wo /w
SCGA ESTSDRAM         MEM3             00000001  FF       ESTSDRAM         /wo /w
SCGA ESTSDRAM         MEM4             00000002  FF       ESTSDRAM         /wo /w
SCGA ESTSDRAM         MEM5             00000003  FF       ESTSDRAM         /wo /w
SCGA ESTSDRAM         MEM6             00000004  FF       ESTSDRAM         /wo /w
SCGA ESTSDRAM         MEM7             00000005  FF       ESTSDRAM         /wo /w
SCGA ESTSDRAM         MEM8             00000006  FF       ESTSDRAM         /wo /w
SCGA ESTSDRAM         MEM9             00000007  FF       ESTSDRAM         /wo /w
SCGA ESTSDRAM         PSDMR6           00010190  8b4f36a3 ESTSDRAM         /wo /w /ua:0
SCGA ESTSDRAM         PSDMR7           00010190  9b4f36a3 ESTSDRAM         /wo /w /ua:0
SCGA ESTSDRAM         MEM10            00000000  FF       ESTSDRAM         /wo /w
SCGA ESTSDRAM         PSDMR8           00010190  9b4f36a3 ESTSDRAM         /wo /w /ua:0
SCGA ESTSDRAM         PSDMR9           00010190  c34f36a3 ESTSDRAM         /wo /w /ua:0
SCGA ESTLSDRAM        SDLSRT           000101A4  0E       ESTLSDRAM        /wo /w /ua:0
SCGA ESTLSDRAM        LSDMR1           00010194  2b4F7ea3 ESTLSDRAM        /wo /w /ua:0
SCGA ESTLSDRAM        MEM11            04000000  FF       ESTLSDRAM        /wo /w
SCGA ESTLSDRAM        LSDMR2           00010194  0b4f7ea3 ESTLSDRAM        /wo /w /ua:0
SCGA ESTLSDRAM        MEM12            04000000  FF       ESTLSDRAM        /wo /w
SCGA ESTLSDRAM        MEM13            04000001  FF       ESTLSDRAM        /wo /w
SCGA ESTLSDRAM        MEM14            04000002  FF       ESTLSDRAM        /wo /w
SCGA ESTLSDRAM        MEM15            04000003  FF       ESTLSDRAM        /wo /w
SCGA ESTLSDRAM        MEM16            04000004  FF       ESTLSDRAM        /wo /w
SCGA ESTLSDRAM        MEM17            04000005  FF       ESTLSDRAM        /wo /w
SCGA ESTLSDRAM        MEM18            04000006  FF       ESTLSDRAM        /wo /w
SCGA ESTLSDRAM        MEM19            04000007  FF       ESTLSDRAM        /wo /w
SCGA ESTLSDRAM        LSDMR3           00010194  2b4f36a3 ESTLSDRAM        /wo /w /ua:0
SCGA ESTLSDRAM        MEM20            04000000  FF       ESTLSDRAM        /wo /w
SCGA ESTLSDRAM        LSDMR4           00010194  c34f36a3 ESTLSDRAM        /wo /w /ua:0
REM *******************************************
REM CF GROUP CONFIGURATION
REM *******************************************
CF GRP                  SIU ENABLED
CF GRP                  MEMC ENABLED
CF GRP                  UPMA DISABLED
CF GRP                  UPMB DISABLED
CF GRP                  UPMC DISABLED
CF GRP                  TIMERS DISABLED
CF GRP                  CLOCKS DISABLED
CF GRP                  SIC DISABLED
CF GRP                  IOPORTS DISABLED
CF GRP                  CPMTIMERS DISABLED
CF GRP                  PCI DISABLED
CF GRP                  ATU DISABLED
CF GRP                  I2O DISABLED
CF GRP                  DMA DISABLED
CF GRP                  FCC DISABLED
CF GRP                  TCL DISABLED
CF GRP                  I2C DISABLED
CF GRP                  COMM DISABLED
CF GRP                  SPI DISABLED
CF GRP                  SI ENABLED
CF GRP                  CPMUX DISABLED
CF GRP                  ESTSDRAM ENABLED
CF GRP                  ESTLSDRAM DISABLED