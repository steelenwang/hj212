#include    "cpu.h"
#include    "types.h"
#include    "var.h "
#include    "sub.h"

void do_init ( void)
{
  do_out[0]=0;do_out[1]=0;
  do_out[2]=0;do_out[3]=0;
  do_out[4]=0;do_out[5]=0;
  do_out[6]=0;do_out[7]=0;
}

void do_proc ( void)
{
const INT8U out_tab [16]=
   {
      0x00,0x01,0x04,0x05,
      0x10,0x11,0x14,0x15,
      0x40,0x41,0x44,0x45,
      0x50,0x51,0x54,0x55
   };


 INT8U  tmp_do;
 INT8U  tmp_doo;

 tmp_do =do_out[0]&0x0f;
 tmp_doo=out_tab[tmp_do];
 P3OUT= tmp_doo;

 if ( do_out[0] &BIT4) gprs_pwr_on();
 else   gprs_pwr_off();

}

void do_tmr_proc( void)
{
   INT8U dotmp,dotmp2;

   dotmp2=hld_reg[DO_SET]&0xff ;

   for ( dotmp=0;dotmp<REAL_MAXDO;dotmp++)
   {
        if ( do_tmr[dotmp])
           {
              if ( do_tmr[dotmp] == 1)
                 {
                   do_tmr[dotmp]=0;
                   if ( getbit ( &dotmp2, dotmp ))  setbit( do_out,dotmp,0);

                 }
              do_tmr[dotmp]--;
           }

   }
}

void  setdo (INT16U pos,INT16U val )
{
  INT8S  tmp;
  tmp= hld_reg[DO_SET]&0xff;

  if ( val == 0) setbit( do_out,pos,val);
  else if ( pos<REAL_MAXDO)
    {
      setbit( do_out,pos,val);
      if (getbit ( &tmp, pos ) )  do_tmr[pos]=hld_reg[RELAY_RLS_TMR];

    }
  else  setbit( do_out,pos,val);


}




