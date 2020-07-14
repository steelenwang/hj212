#include "cpu.h"
#include  "const.h"

#define TMRA5D0MS    40000
void  tmr_a_init( void)
{
  TACTL=TASSEL1+TACLR;
  //TACCTL0=CCIE;
  TACCR0=TMRA5D0MS ;               //1.25ms no interrupt
  TACCTL1=OUTMOD_3;
  TACCR1=4737;
  TACTL|=MC0;
}

#pragma vector=TIMERA0_VECTOR
__interrupt void tmr_a0_isr(void)
{

}

#pragma vector=TIMERA1_VECTOR
__interrupt void tmr_a1_isr(void)
{

}

