#include "cpu.h"
#include  "const.h"
#include  "types.h"
#include  "var.h"
void  freq_measure(void);
void  com1_rcv ( void );
INT8U  get_modbus_adr(void);
void sio1_led_tmr_proc ( void);

void  tmr_b_init( void)
{
  TBCTL = TBSSEL_2 + TBCLR;
  TBCCTL0 = CCIE;
  TBCCR0  = TMRB10MS+TMRB1MS/2;  //目的让两个中断错开时间 500微秒
  TBCCTL1 = CCIE;
  TBCCR1  = TMRB1MS;
  TBCTL |=(MC1+ID0+ID1);
}

#pragma vector=TIMERB0_VECTOR
__interrupt void tmr_b0_isr(void)
{
    TBCCR0 +=TMRB10MS;
    freq_measure();
    sio1_led_tmr_proc ( );
    ms10_tmr ++;
    protocol_tmr ++;

}

#pragma vector=TIMERB1_VECTOR
__interrupt void tmr_b1_isr(void)
{
    INT8U tmp;
    switch( TBIV )
    {
     case 2:  // 1ms 中断服务
        TBCCR1 +=TMRB1MS;

   if ( 65535 == hld_reg[TIME_1MS_L]  )
    { hld_reg[TIME_1MS_L]=0;
      hld_reg[TIME_1MS_H] +=1;
    }
  else   hld_reg[0]++;
     break;

     case 4:  //串口1 收发转换用 总线超时判别
       if ( sio1_flag &BIT7 )
       {
         TBCCTL2 &= ~CCIE;
         sio1_flag &=~BIT7;
         switch ( sio1_set)
         {
           case 0:
           tmp = get_modbus_adr()&0xff;
           if (sio1_rx_lenth >3 && sio1_buf[0]==tmp) fr1_ready=1;
           else {sio1_rx_lenth=0;}
           break;

           case 1:
             fr1_ready=1;
           break;

           case 2:
             fr1_ready=1;
           break;

           default:
           tmp = get_modbus_adr()&0xff;
           if (sio1_rx_lenth >3 && sio1_buf[0]==tmp) fr1_ready=1;
           else {sio1_rx_lenth=0;}
           break;


         }
       }
       else if ( sio1_flag &BIT6)
       {
         TBCCTL2 &= ~CCIE;
         sio1_flag &=~BIT6;
         com1_rcv();
       }
     break;

     case 6:  //串口2 收发转换用  总线超时判别

     break;



    }
}

