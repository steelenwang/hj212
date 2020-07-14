#ifndef CONST_H00
#define CONST_H00  1

#ifndef NULL
#define NULL   (void *) 0
#endif


#define HI(value)  value/256
//取高字节
#define LOW(value) value & 0xff

#define  MAX_SIO_BUF 512

#define  MAXDATA     256
#define  MAX_LENTH   0x7d
#define  MAX_DIO_LENTH  0x7d0
#define  MAXHLD      256
#define  MAXDO       64
#define  REAL_MAXDO  4
#define  MAXDI       64
#define  REAL_MAXDI  16

#define  MAXAI        64
#define  REAL_AI      8
#define  MAX_FLOAT    64

#define  MAXFIFO      64
#define  SOE_BASE_ADR 300
#define  SOE_END_ADR  526

#define SYS_CLK  8000000L

#define POINT   16

//TMRB 设置是进行了 8 M的8分频
#define TMRB1MS   1000
#define TMRB10MS  10000

#define TMRA1D25MS 10000
#define TMRACCR1   4737

#define B115200  69
#define B76800   104
#define B57600   139
#define B38400   208
#define B19200   417
#define B9600    833
#define B4800    1666
#define B2400    3332
#define B1200    6664

#define B1200_TXTMR  8600
#define B2400_TXTMR  4300
#define B4800_TXTMR  2150
#define B9600_TXTMR  1100
#define B19200_TXTMR 600
#define B38400_TXTMR 300
#define B57600_TXTMR 150
#define B76800_TXTMR 150
#define B115200_TXTMR 150

#define B1200_RXTMR   32000
#define B2400_RXTMR   16000
#define B4800_RXTMR   8000
#define B9600_RXTMR   4000
#define B19200_RXTMR  2000
#define B38400_RXTMR  1000
#define B57600_RXTMR  800
#define B76800_RXTMR  800
#define B115200_RXTMR 800

#define TIME_1MS_L  0
#define TIME_1MS_H  TIME_1MS_L+1

#define COUNT_START  2
#define COUNT_END    29

#define  FREQ_GATE   32
#define  AT45DB_PAGE 33

#define  SHANDONG_CTL1  45                //发送通道控制
#define  SHANDONG_CLT2  SHANDONG_CTL1+1   //发送通道控制
#define  SHANDONG_CLT3  SHANDONG_CTL1+2
#define  SHANDONG_CTL4  SHANDONG_CTL1+3

#define  TRANSMIT_NUM     SHANDONG_CTL4+1 //共64个发送序号控制
                                         // 从 49开始共64个
#define  TRANSMIT_NUM_END TRANSMIT_NUM +63

#define  DC1   0
#define  DC2   DC1+1
#define  DC3   DC1+2
#define  DC4   DC1+3
#define  DC5   DC1+4
#define  DC6   DC1+5
#define  DC7   DC1+6
#define  DC8   DC1+7

#define  TEMPR DC1+8
#define  VDCC  DC1+9

#define  DC_FREQ DC1+10



#define  H_YEAR         123
#define  H_DAY          124
#define  H_MIN          125

#define  HLD_REG_INIT1   126
#define  HLD_REG_INIT2   127

#define DC1_XH        128
#define DC2_XH        DC1_XH +1
#define DC3_XH        DC1_XH +2
#define DC4_XH        DC1_XH +3
#define DC5_XH        DC1_XH +4
#define DC6_XH        DC1_XH +5
#define DC7_XH        DC1_XH +6
#define DC8_XH        DC1_XH +7

#define DC1_MIN    DC8_XH +1
#define DC2_MIN    DC8_XH +2
#define DC3_MIN    DC8_XH +3
#define DC4_MIN    DC8_XH +4
#define DC5_MIN    DC8_XH +5
#define DC6_MIN    DC8_XH +6
#define DC7_MIN    DC8_XH +7
#define DC8_MIN    DC8_XH +8

#define DC1_MAX     DC8_MIN+1
#define DC2_MAX     DC8_MIN+2
#define DC3_MAX     DC8_MIN+3
#define DC4_MAX     DC8_MIN+4
#define DC5_MAX     DC8_MIN+5
#define DC6_MAX     DC8_MIN+6
#define DC7_MAX     DC8_MIN+7
#define DC8_MAX     DC8_MIN+8

#define DC1_SCALE   DC8_MAX+1
#define DC2_SCALE   DC8_MAX+2
#define DC3_SCALE   DC8_MAX+3
#define DC4_SCALE   DC8_MAX+4
#define DC5_SCALE   DC8_MAX+5
#define DC6_SCALE   DC8_MAX+6
#define DC7_SCALE   DC8_MAX+7
#define DC8_SCALE   DC8_MAX+8

#define   RELAY_ON_TMR    DC8_SCALE + 1
#define   RELAY_RLS_TMR   DC8_SCALE + 2
#define   DO_SET          DC8_SCALE + 3
#define   SIO1_TMR        DC8_SCALE + 4
#define   SIO1_SET        DC8_SCALE + 5


#define  SIO2_SET        DC8_SCALE + 7
#define  MODBUS_ADR      DC8_SCALE + 8

#define  SIO3_SET         MODBUS_ADR+8
#define  SIO3_PROTOCOL    SIO3_SET +1
#define  SIO3_TMOUT       SIO3_SET +2

#define  SIO4_SET         SIO3_SET +8
#define  SIO4_PROTOCOL    SIO4_SET +1
#define  SIO4_TMOUT       SIO4_SET +2

#define  SIO5_SET         SIO3_SET +8
#define  SIO5_PROTOCOL    SIO5_SET +1
#define  SIO5_TMOUT       SIO5_SET +2

#define  SIO6_SET         SIO3_SET +8
#define  SIO6_PROTOCOL    SIO6_SET +1
#define  SIO6_TMOUT       SIO5_SET +2

#define  SHANXI_ASET      SIO6_SET +8
#define  SHANXI_DSET      SIO6_SET +9
#define  SHANXI_KSET      SIO6_SET +10

#define  SHANXI_SEND_TMR  SIO6_SET +11
#define  SHANXI_REAL_TMR  SIO6_SET +12

#define  SHANXI_STORE_TMR SIO6_SET +13

#else
#endif

