
#include "cpu.h"
#include "var.h"

void basic_tmr_init (void)
{
  BTCNT2=0;
  BTCTL =BTSSEL;
}

void rtc_tmr_init (void)
{
    basic_tmr_init();
    RTCTIM0=0;
    RTCTIM1=0;
    RTCCTL=RTCMODE_1+RTCTEV_1+RTCIE;

}

#pragma vector=BASICTIMER_VECTOR
__interrupt  void rtc_int (void)
{
   RTCCTL &=~RTCFG;

}

