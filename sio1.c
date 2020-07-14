#include "cpu.h"
#include "types.h"
#include "const.h"
#include "var.h"
#include "sub.h"



INT16U  sio1_led_tmr;


void sio1_led_on( void)
{
  P9OUT &=~BIT6;
  P9OUT &=~BIT2;
  sio1_led_tmr =20;
}

void sio1_led_off( void)
{
  P9OUT |=BIT2;
  P9OUT |=BIT6;
  sio1_led_tmr =0;
}
void sio1_led_tmr_proc ( void)
{
  if ( sio1_led_tmr )
  {   if ( sio1_led_tmr ==1 )
      {
        P9OUT |=BIT2;
        P9OUT |=BIT6;
        sio1_led_tmr =0;
       }
      else sio1_led_tmr --;
  }

}

void com1_rcv (void)
{
  P9OUT &=(~BIT3);
}

void com1_tr (void)
{
  P9OUT |= BIT3;
}


void sio1_init (void)
{
  com1_rcv();

  switch (hld_reg[SIO1_SET])
  {
   case 0x1003:     //1200 bps 8 n 1
     UCA0CTL0=0;
     UCA0CTL1 |= UCSSEL_2;
     UCA0BR0 =B1200 %256;
     UCA0BR1 =B1200 /256;
     UCA0MCTL = 0x00;
     sio1_rx_tmr =B1200_RXTMR;
     sio1_tx_tmr =B1200_TXTMR;
   break;

   case 0x1004:    //2400 BPS 8 n 1
     UCA0CTL0=0;
     UCA0CTL1 |= UCSSEL_2;
     UCA0BR0 =B2400 %256;
     UCA0BR1 =B2400 /256;
     UCA0MCTL = 0x00;
     sio1_rx_tmr =B2400_RXTMR;
     sio1_tx_tmr =B2400_TXTMR;
   break;

    case 0x1005:   //4800BPS 8 n 1
     UCA0CTL0=0;
     UCA0CTL1 |= UCSSEL_2;
     UCA0BR0 =B4800 %256;
     UCA0BR1 =B4800 /256;
     UCA0MCTL = 0x00;
     sio1_rx_tmr =B4800_RXTMR;
     sio1_tx_tmr =B4800_TXTMR;
    break;

    case 0x1006: //9600BPS 8 n 1
     UCA0CTL0=0;
     UCA0CTL1 |= UCSSEL_2;
     UCA0BR0 =B9600 %256;
     UCA0BR1 =B9600 /256;
     UCA0MCTL = 0x00;
     sio1_rx_tmr =B9600_RXTMR;
     sio1_tx_tmr =B9600_TXTMR;
    break;

   case 0x1007: //19200BPS 8 n 1
     UCA0CTL0=0;
     UCA0CTL1 |= UCSSEL_2;
     UCA0BR0 =B19200 %256;
     UCA0BR1 =B19200 /256;
     UCA0MCTL = 0x00;
     sio1_rx_tmr =B19200_RXTMR;
     sio1_tx_tmr =B19200_TXTMR;
   break;

   case 0x1008: //38400BPS 8 n 1
     UCA0CTL0=0;
     UCA0CTL1 |= UCSSEL_2;
     UCA0BR0 =B38400 %256;
     UCA0BR1 =B38400 /256;
     UCA0MCTL = 0x00;
     sio1_rx_tmr =B38400_RXTMR;
     sio1_tx_tmr =B38400_TXTMR;
    break;

   case 0x1009:   //57600BPS 8 n 1
     UCA0CTL0=0;
     UCA0CTL1 |= UCSSEL_2;
     UCA0BR0 =B38400 %256;
     UCA0BR1 =B38400 /256;
     UCA0MCTL = 0x00;
     sio1_rx_tmr =B38400_RXTMR;
     sio1_tx_tmr =B38400_TXTMR;
   break;

   case 0x100a:  //76800BPS 8 n 1
     UCA0CTL0=0;
     UCA0CTL1 |= UCSSEL_2;
     UCA0BR0 =B38400 %256;
     UCA0BR1 =B38400 /256;
     UCA0MCTL = 0x00;
     sio1_rx_tmr =B38400_RXTMR;
     sio1_tx_tmr =B38400_TXTMR;
   break;

   case 0x100b: //115200BPS 8 n 1
     UCA0CTL0=0;
     UCA0CTL1 |= UCSSEL_2;
     UCA0BR0 =B38400 %256;
     UCA0BR1 =B38400 /256;
     UCA0MCTL = 0x00;
     sio1_rx_tmr =B38400_RXTMR;
     sio1_tx_tmr =B38400_TXTMR;
   break;

   case 0x100c:  //187.5KBPS 8 N 1
     UCA0CTL0=0;
     UCA0CTL1 |= UCSSEL_2;
     UCA0BR0 =B38400 %256;
     UCA0BR1 =B38400 /256;
     UCA0MCTL = 0x00;
     sio1_rx_tmr =B38400_RXTMR;
     sio1_tx_tmr =B38400_TXTMR;
   break;


   case 0xD003:     //1200 bps 8 E 1
     UCA0CTL0=UCPEN+UCPAR;
     UCA0CTL1 |= UCSSEL_2;
     UCA0BR0 =B1200 %256;
     UCA0BR1 =B1200 /256;
     UCA0MCTL = 0x00;
     sio1_rx_tmr =B1200_RXTMR;
     sio1_tx_tmr =B1200_TXTMR;
   break;

   case 0xD004:    //2400 BPS 8 E 1
     UCA0CTL0=UCPEN+UCPAR;
     UCA0CTL1 |= UCSSEL_2;
     UCA0BR0 =B2400 %256;
     UCA0BR1 =B2400 /256;
     UCA0MCTL = 0x00;
     sio1_rx_tmr =B2400_RXTMR;
     sio1_tx_tmr =B2400_TXTMR;
   break;

    case 0xD005:   //4800BPS 8 E 1
     UCA0CTL0=UCPEN+UCPAR;
     UCA0CTL1 |= UCSSEL_2;
     UCA0BR0 =B4800 %256;
     UCA0BR1 =B4800 /256;
     UCA0MCTL = 0x00;
     sio1_rx_tmr =B4800_RXTMR;
     sio1_tx_tmr =B4800_TXTMR;
    break;

    case 0xD006: //9600BPS 8 E 1
     UCA0CTL0=UCPEN+UCPAR;
     UCA0CTL1 |= UCSSEL_2;
     UCA0BR0 =B9600 %256;
     UCA0BR1 =B9600 /256;
     UCA0MCTL = 0x00;
     sio1_rx_tmr =B9600_RXTMR;
     sio1_tx_tmr =B9600_TXTMR;
    break;

   case 0xD007: //19200BPS 8 E 1
     UCA0CTL0=UCPEN+UCPAR;
     UCA0CTL1 |= UCSSEL_2;
     UCA0BR0 =B19200 %256;
     UCA0BR1 =B19200 /256;
     UCA0MCTL = 0x00;
     sio1_rx_tmr =B19200_RXTMR;
     sio1_tx_tmr =B19200_TXTMR;
   break;

   case 0xD008: //38400BPS 8 E 1
     UCA0CTL0=UCPEN+UCPAR;
     UCA0CTL1 |= UCSSEL_2;
     UCA0BR0 =B38400 %256;
     UCA0BR1 =B38400 /256;
     UCA0MCTL = 0x00;
     sio1_rx_tmr =B38400_RXTMR;
     sio1_tx_tmr =B38400_TXTMR;
    break;

   case 0xD009:   //57600BPS 8 E 1
     UCA0CTL0=UCPEN+UCPAR;
     UCA0CTL1 |= UCSSEL_2;
     UCA0BR0 =B38400 %256;
     UCA0BR1 =B38400 /256;
     UCA0MCTL = 0x00;
     sio1_rx_tmr =B38400_RXTMR;
     sio1_tx_tmr =B38400_TXTMR;
   break;

   case 0xD00A:  //76800BPS 8 E 1
     UCA0CTL0=UCPEN+UCPAR;
     UCA0CTL1 |= UCSSEL_2;
     UCA0BR0 =B38400 %256;
     UCA0BR1 =B38400 /256;
     UCA0MCTL = 0x00;
     sio1_rx_tmr =B38400_RXTMR;
     sio1_tx_tmr =B38400_TXTMR;
   break;

   case 0xD00b: //115200BPS 8 E 1
     UCA0CTL0=UCPEN+UCPAR;
     UCA0CTL1 |= UCSSEL_2;
     UCA0BR0 =B38400 %256;
     UCA0BR1 =B38400 /256;
     UCA0MCTL = 0x00;
     sio1_rx_tmr =B38400_RXTMR;
     sio1_tx_tmr =B38400_TXTMR;
   break;

   case 0xD00c:  //187.5KBPS 8 E 1
     UCA0CTL0=UCPEN+UCPAR;
     UCA0CTL1 |= UCSSEL_2;
     UCA0BR0 =B38400 %256;
     UCA0BR1 =B38400 /256;
     UCA0MCTL = 0x00;
     sio1_rx_tmr =B38400_RXTMR;
     sio1_tx_tmr =B38400_TXTMR;
   break;


    case 0xE003:     //1200 bps 8 O 1
     UCA0CTL0=UCPEN;
     UCA0CTL1 |= UCSSEL_2;
     UCA0BR0 =B1200 %256;
     UCA0BR1 =B1200 /256;
     UCA0MCTL = 0x00;
     sio1_rx_tmr =B1200_RXTMR;
     sio1_tx_tmr =B1200_TXTMR;
   break;

   case 0xE004:    //2400 BPS 8 O 1
     UCA0CTL0=UCPEN;
     UCA0CTL1 |= UCSSEL_2;
     UCA0BR0 =B2400 %256;
     UCA0BR1 =B2400 /256;
     UCA0MCTL = 0x00;
     sio1_rx_tmr =B2400_RXTMR;
     sio1_tx_tmr =B2400_TXTMR;
   break;

    case 0xE005:   //4800BPS 8 O 1
     UCA0CTL0=UCPEN;
     UCA0CTL1 |= UCSSEL_2;
     UCA0BR0 =B4800 %256;
     UCA0BR1 =B4800 /256;
     UCA0MCTL = 0x00;
     sio1_rx_tmr =B4800_RXTMR;
     sio1_tx_tmr =B4800_TXTMR;
    break;

    case 0xE006: //9600BPS 8 O 1
     UCA0CTL0=UCPEN;
     UCA0CTL1 |= UCSSEL_2;
     UCA0BR0 =B9600 %256;
     UCA0BR1 =B9600 /256;
     UCA0MCTL = 0x00;
     sio1_rx_tmr =B9600_RXTMR;
     sio1_tx_tmr =B9600_TXTMR;
    break;

   case 0xE007: //19200BPS 8 O 1
     UCA0CTL0=UCPEN;
     UCA0CTL1 |= UCSSEL_2;
     UCA0BR0 =B19200 %256;
     UCA0BR1 =B19200 /256;
     UCA0MCTL = 0x00;
     sio1_rx_tmr =B19200_RXTMR;
     sio1_tx_tmr =B19200_TXTMR;
   break;

   case 0xE008: //38400BPS 8 O 1
     UCA0CTL0=UCPEN;
     UCA0CTL1 |= UCSSEL_2;
     UCA0BR0 =B38400 %256;
     UCA0BR1 =B38400 /256;
     UCA0MCTL = 0x00;
     sio1_rx_tmr =B38400_RXTMR;
     sio1_tx_tmr =B38400_TXTMR;
    break;

   case 0xE009:   //57600BPS 8 O 1
     UCA0CTL0=UCPEN;
     UCA0CTL1 |= UCSSEL_2;
     UCA0BR0 =B38400 %256;
     UCA0BR1 =B38400 /256;
     UCA0MCTL = 0x00;
     sio1_rx_tmr =B38400_RXTMR;
     sio1_tx_tmr =B38400_TXTMR;
   break;

   case 0xE00A:  //76800BPS 8 O 1
     UCA0CTL0=UCPEN;
     UCA0CTL1 |= UCSSEL_2;
     UCA0BR0 =B38400 %256;
     UCA0BR1 =B38400 /256;
     UCA0MCTL = 0x00;
     sio1_rx_tmr =B38400_RXTMR;
     sio1_tx_tmr =B38400_TXTMR;
   break;

   case 0xE00b: //115200BPS 8 O 1
     UCA0CTL0=UCPEN;
     UCA0CTL1 |= UCSSEL_2;
     UCA0BR0 =B38400 %256;
     UCA0BR1 =B38400 /256;
     UCA0MCTL = 0x00;
     sio1_rx_tmr =B38400_RXTMR;
     sio1_tx_tmr =B38400_TXTMR;
   break;

   case 0xE00c:  //187.5KBPS 8 O 1
     UCA0CTL0=UCPEN;
     UCA0CTL1 |= UCSSEL_2;
     UCA0BR0 =B38400 %256;
     UCA0BR1 =B38400 /256;
     UCA0MCTL = 0x00;
     sio1_rx_tmr =B38400_RXTMR;
     sio1_tx_tmr =B38400_TXTMR;
   break;



    default:
     UCA0CTL0=0;
     UCA0CTL1 |= UCSSEL_2;
     UCA0BR0 =B9600 %256;
     UCA0BR1 =B9600 /256;
     UCA0MCTL = 0x00;
     sio1_rx_tmr =B9600_RXTMR;
     sio1_tx_tmr =B9600_TXTMR;
    break;
  }
   UCA0CTL1 &= ~UCSWRST;
   IFG2 =0;
   _NOP();
   IFG2 =0;
   _NOP();
   IFG2 =0;
   IE2 |= UCA0RXIE+UCA0TXIE;
   com1_rcv();

}



#pragma vector=USCIAB0RX_VECTOR
__interrupt void sio1_rx_int (void)
{
   if ( UCA0STAT & UCRXERR)
         {
           sio1_buf[MAX_SIO_BUF-1]=UCA0RXBUF;
           sio1_rx_lenth=0;
           fr1_ready=0;
           TBCCTL1 &= ~CCIE;
           sio1_buf[MAX_SIO_BUF-1]=UCA0RXBUF;

           }

   else if (sio1_rx_lenth == 999)
        {
        sio1_rx_lenth=0;
        fr1_ready=0;
        sio1_buf[MAX_SIO_BUF-1]=UCA0RXBUF;
        TBCCTL1 &= ~CCIE;
        }

   else {
         sio1_buf[sio1_rx_lenth]=UCA0RXBUF;
         sio1_rx_lenth++;
         TBCCR2  = TBR+sio1_rx_tmr;
         TBCCTL2 = CCIE;
         sio1_flag |=BIT7;
         }
}

#pragma vector=USCIAB0TX_VECTOR
__interrupt void sio1_tx_int (void)
{
      sio1_tx_lenth++;
      if (sio1_tx_lenth2 == sio1_tx_lenth)
        {
          IFG2 &= ~UCA0TXIFG;
          TBCCR2  = TBR+sio1_tx_tmr;
          TBCCTL2 = CCIE;
          sio1_flag |=BIT6;
          sio1_tx_lenth=0;
          IFG2 &= ~UCA0TXIFG;

        }
      else UCA0TXBUF=sio1_buf[sio1_tx_lenth];

}



