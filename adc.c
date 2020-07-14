
#include "cpu.h"
#include "var.h"

void ad_var_init( void)
{

  for ( ad_cnt=0;ad_cnt<8;ad_cnt ++ )
  {
    ad_dc1_buf[ad_cnt]=0;
    ad_dc2_buf[ad_cnt]=0;
    ad_dc3_buf[ad_cnt]=0;
    ad_dc4_buf[ad_cnt]=0;
    ad_dc5_buf[ad_cnt]=0;
    ad_dc6_buf[ad_cnt]=0;
    ad_dc7_buf[ad_cnt]=0;
    ad_dc8_buf[ad_cnt]=0;
  }
  ad_cnt=0;


}

void adc_init ( void )
{
  INT16U i;

  ADC12CTL0 =SHT1_15+ADC12ON+MSC+SHT0_15+REFON+REF2_5V;    // Setup ADC12, ref., sampling time
  for (i=0;i<10000;i++) _NOP();
  ADC12CTL1 = SHP+CONSEQ_1+SHS_1 + ADC12SSEL_2+ADC12DIV_7;  // Use sampling timer

  ADC12MCTL0 = INCH_10+SREF_1;
  ADC12MCTL1 = INCH_1+SREF_1;
  ADC12MCTL2 = INCH_2+SREF_1;
  ADC12MCTL3 = INCH_3+SREF_1;
  ADC12MCTL4 = INCH_4+SREF_1;
  ADC12MCTL5 = INCH_5+SREF_1;
  ADC12MCTL6 = INCH_6+SREF_1;
  ADC12MCTL7 = INCH_7+SREF_1;
  ADC12MCTL8 = INCH_0+SREF_1+EOS;
  ADC12IE |=BIT8;
  ADC12CTL0 |=ENC;
  ad_var_init();
}



#pragma vector=ADC12_VECTOR
__interrupt void adc_isr(void)
{

   ad_dc1_buf[ad_cnt]=  ADC12MEM8;
   ad_dc2_buf[ad_cnt]=  ADC12MEM1;
   ad_dc3_buf[ad_cnt]=  ADC12MEM2;
   ad_dc4_buf[ad_cnt]=  ADC12MEM3;
   ad_dc5_buf[ad_cnt]=  ADC12MEM4;
   ad_dc6_buf[ad_cnt]=  ADC12MEM5;
   ad_dc7_buf[ad_cnt]=  ADC12MEM6;
   ad_dc8_buf[ad_cnt]=  ADC12MEM7;
   tempreture=ADC12MEM0;
   ad_cnt ++;
   ad_cnt &= (POINT-1);
   ADC12CTL0 |=ENC;

}




