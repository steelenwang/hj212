#include "cpu.h"
#include "types.h"
void io_init ( void)
{
  //p1 port ---------------
    P1DIR = 0;
    P1OUT = 0;
    P1IFG = 0;
    P1IES = 0;
    P1SEL = 0;
    P1IE  = 0xff;
  //p1 port ---------------

  //p2 port ---------------
    P2DIR = 0;
    P2OUT = 0;
    P2IFG = 0;
    P2IES = 0xc0;
    P2SEL = 0;
    P2IE  = 0xff;
  //p2 port ---------------

  //p3 port ---------------
    P3DIR = 0xff;
    P3SEL = 0;
    P3OUT = 0x0;
  //p3 port ---------------

  //P4 PORT-------------
    P4SEL =BIT3|BIT4|BIT5|BIT6|BIT7;
    P4DIR =0;
    P4OUT =0;
  //P4PORT ------------

  //p5 port -------------
    P5SEL = 0;
    P5DIR = ~BIT1;
    P5OUT =0xff;
  //p5 port ------------

  //p6 port -------------
    P6SEL =0xff ;
    P6DIR =0x0;
    P6OUT = 0x0;
  //p6 port -------------

  //p7 port -------------
     P7SEL =0x0;
     P7DIR =0xf0;
     P7OUT =0xf0;
  //p7 port -------------

  //p8 port -------------
     P8SEL =0x0;
     P8DIR =0x0;
     P8OUT =0x0;
  //p8 port -------------

  //p9 port -------------
    P9SEL =0x0;
    P9DIR = 0x7f;
    P9OUT =0xff;
  //p9 port -------------

  //p10 port -------------
    P10SEL = 0x0;
    P10DIR = 0x0;
    P10OUT = 0x0;
  //p10 port -------------

}

void clk_init ( void)
{
  INT16U i;


  _BIS_SR(OSCOFF + SCG0 + GIE);
  FLL_CTL1 &= ~XT2OFF;
  do
  {
  IFG1 &= ~OFIFG;                           // Clear OSCFault flag
  for (i = 15; i > 0; i--);                  // Time for flag to set
  }
  while ((IFG1 & OFIFG));                   // OSCFault flag still set?

  FLL_CTL1 |= (SELM1+SELS );                       // MCLK = XT2


}

void enable_int( void )
{
   IFG2 = 0;
   _NOP();
   IFG2 = 0;
   _NOP();
   IFG2 = 0;
   _NOP();
   _EINT();
}
