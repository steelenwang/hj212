#include    "cpu.h"
#include    "types.h"
#include    "var.h"
#include    "const.h"

#define     DI_USED  0x80
#define     DI_TIME  0x20

void di_init ( void)
{
di_old =0xff;
di_old2 =0xff;
di_old3 =0xff;

di[0]=0x00;
di[1]=0x00;

}


void di_proc ( void )
{
   int cnt00;
   unsigned char tmp,shif,di1;
   di1=P1IN;
   di[0]=~di1;
   di1=P2IN;
   tmp =P4IN;
   di1=di1 &0x3f;
   tmp =tmp &0x3;
   tmp =tmp<<6;
   di1 =di1 | tmp;
   di[1]= ~di1;
   if (P9IN &BIT7) di[2] |=BIT0;
   else   di[2] &=~BIT0;

/*
   di1=P1IN;

   shif=0x01;

   tmp=di1 ^di_old;

   for (cnt00=0;cnt00<8;cnt00++)
   {
      if (soetmp_di_cnt1[cnt00] & DI_USED)
      {

        if (tmp & shif )
           {
           soetmp_di_cnt2[cnt00]=DI_TIME;

            if (--soetmp_di_cnt1[cnt00] == DI_USED)
               {
                    soe_write_p->di_num=cnt00;

                    if (tmp & shif & di1)   soe_write_p->soe_attrib=0x0;      // 1 rise  0 down  /
                    else  soe_write_p->soe_attrib=0x01;

                     soe_write_p->soe_timeL=soetmp_timeL[cnt00];
                     soe_write_p->soe_timeH=soetmp_timeH[cnt00];
                     di_old=(di_old & ( ~shif) ) | (di1 & shif) ;
                     soetmp_di_cnt1[cnt00]=0x0;
                     soetmp_di_cnt2[cnt00]=0x0;

                     if (soe_write_p == &soe_buf[MAXFIFO-1]) soe_write_p=soe_buf;
                     else soe_write_p++;

               }
             }

       else {

            soetmp_di_cnt1[cnt00]=DI_USED+DI_TIME;

            if (--soetmp_di_cnt2[cnt00] == 0x0) soetmp_di_cnt1[cnt00]=0x00;

            }
      }
   shif <<=1;
   }

di[0]=~di_old ;


   di1=P2IN;

   shif=0x01;

   tmp=di1 ^di_old2;

   for (  ;cnt00<14;cnt00++)
   {
      if (soetmp_di_cnt1[cnt00] & DI_USED)
      {

        if (tmp & shif )
           {
           soetmp_di_cnt2[cnt00]=DI_TIME;

            if (--soetmp_di_cnt1[cnt00] == DI_USED)
               {
                    soe_write_p->di_num=cnt00;

                    if (tmp & shif & di1)   soe_write_p->soe_attrib=0x0;      // 1 rise  0 down
                    else  soe_write_p->soe_attrib=0x01;

                     soe_write_p->soe_timeL=soetmp_timeL[cnt00];
                     soe_write_p->soe_timeH=soetmp_timeH[cnt00];
                     di_old2=(di_old2 & ( ~shif) ) | (di1 & shif) ;
                     soetmp_di_cnt1[cnt00]=0x0;
                     soetmp_di_cnt2[cnt00]=0x0;

                     if (soe_write_p == &soe_buf[MAXFIFO-1]) soe_write_p=soe_buf;
                     else soe_write_p++;

               }
             }

       else {

            soetmp_di_cnt1[cnt00]=DI_USED+DI_TIME;

            if (--soetmp_di_cnt2[cnt00] == 0x0) soetmp_di_cnt1[cnt00]=0x00;

            }
      }
   shif <<=1;
   }

   di1=P4IN;

   shif=0x02;

   tmp=di1 ^di_old3;

   for (  ;cnt00<16;cnt00++)
   {
      if (soetmp_di_cnt1[cnt00] & DI_USED)
      {

        if (tmp & shif )
           {
           soetmp_di_cnt2[cnt00]=DI_TIME;

            if (--soetmp_di_cnt1[cnt00] == DI_USED)
               {
                    soe_write_p->di_num=cnt00;

                    if (tmp & shif & di1)   soe_write_p->soe_attrib=0x0;      // 1 rise  0 down
                    else  soe_write_p->soe_attrib=0x01;

                     soe_write_p->soe_timeL=soetmp_timeL[cnt00];
                     soe_write_p->soe_timeH=soetmp_timeH[cnt00];
                     di_old3=(di_old3 & ( ~shif) ) | (di1 & shif) ;
                     soetmp_di_cnt1[cnt00]=0x0;
                     soetmp_di_cnt2[cnt00]=0x0;

                     if (soe_write_p == &soe_buf[MAXFIFO-1]) soe_write_p=soe_buf;
                     else soe_write_p++;

               }
             }

       else {

            soetmp_di_cnt1[cnt00]=DI_USED+DI_TIME;

            if (--soetmp_di_cnt2[cnt00] == 0x0) soetmp_di_cnt1[cnt00]=0x00;

            }
      }
   shif >>=1;
   }

di[1]=~((di_old2& 0x3f)|( (di_old3<<6) &0xc0)); */


}

void di_1ms_proc ( void )
{


}
#pragma vector=PORT1_VECTOR

__interrupt void p1_int( void)
{
  INT8U   di_tmp,shift_tmp;
  INT8U   di_cntt, di_tmp_cnt;

  di_tmp =P1IFG;
  P1IFG=0x0;
  di_tmp_cnt =0;
  shift_tmp =0x1;
  for (di_cntt=2;di_cntt<18;di_cntt+=2)
  {
   if (di_tmp & shift_tmp)
   {
      if (hld_reg[di_cntt] !=65535)  hld_reg[di_cntt]++;
      else
       { hld_reg[di_cntt+1]++;
         hld_reg[di_cntt]=0;
       }
      count[di_tmp_cnt]++;
      tmp_count[di_tmp_cnt]++;

    }
   di_tmp_cnt ++;
   shift_tmp<<=1;
  }

}
#pragma vector=PORT2_VECTOR

__interrupt void p2_int( void)
{
  INT8U   di_tmp,shift_tmp;
  INT8U   di_cntt, di_tmp_cnt;

  di_tmp =P2IFG;
  sio_int_flag |= ( di_tmp &(BIT7|BIT6) );
  P2IFG=0x0;
  di_tmp_cnt =8;
  shift_tmp =0x1;
  for (di_cntt=18;di_cntt<34;di_cntt+=2)
  {
   if (di_tmp & shift_tmp)
   {
      if (hld_reg[di_cntt] !=65535)  hld_reg[di_cntt]++;
      else
       { hld_reg[di_cntt+1]++;
         hld_reg[di_cntt]=0;
       }
      count[di_tmp_cnt]++;
      tmp_count[di_tmp_cnt]++;

    }
   di_tmp_cnt ++;
   shift_tmp<<=1;
  }


}

void  freq_measure (void)
{
   static  INT16U gate_tmr=0;
   INT8U   tmp2;

   gate_tmr++;

   if ( gate_tmr == hld_reg[ FREQ_GATE ] )
   {
       gate_tmr =0;
       for ( tmp2=0;tmp2<16;tmp2++ )
       {
         data[DC_FREQ + tmp2 ] = count[tmp2];

       }
       for ( tmp2=0;tmp2<14;tmp2++ )
       {
           count[tmp2]=0;
       }
   }


}
