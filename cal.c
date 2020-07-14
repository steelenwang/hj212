
#include    "cpu.h"
#include    "types.h"
#include    "const.h"
#include    "var.h"
#include    "sub.h"

#define  DC_COFF  54500L

void cal_init (void)
{
  INT8U tmp;
  avr_cnt =0;
  for ( tmp =0;tmp<MAX_FLOAT;tmp++)
  {
         float_data [tmp]=0.0;
         float_avr_data[tmp]=0.0;
  }
}

void dc_cal (void)
{

	INT8U  i;
	INT32S  tmp;
	FP32    ftmp;
	tmp=0l;
       // _DINT();
	for ( i=0;i<POINT;i++)  tmp=tmp+ad_dc1_buf[i];	
        // _DINT();
	tmp= tmp * hld_reg[DC1_XH] /DC_COFF ;
	data[DC1]= (int) tmp;
	
	tmp=0l;
	for ( i=0;i<POINT;i++)  tmp=tmp+ad_dc2_buf[i];	
	tmp= tmp * hld_reg[DC2_XH] /DC_COFF ;
	data[DC2]= (int) tmp;
	
	tmp=0l;
	for ( i=0;i<POINT;i++)  tmp=tmp+ad_dc3_buf[i];	
	tmp= tmp * hld_reg[DC3_XH] /DC_COFF ;
	data[DC3]= (int) tmp;
	
	tmp=0l;
	for ( i=0;i<POINT;i++)  tmp=tmp+ad_dc4_buf[i];	
	tmp= tmp * hld_reg[DC4_XH] /DC_COFF ;
	data[DC4]= (int) tmp;
	

        tmp=0l;
	for ( i=0;i<POINT;i++)  tmp=tmp+ad_dc5_buf[i];	
	tmp= tmp * hld_reg[DC5_XH] /DC_COFF ;
	data[DC5]= (int) tmp;
	
	tmp=0l;
	for ( i=0;i<POINT;i++)  tmp=tmp+ad_dc6_buf[i];	
	tmp= tmp * hld_reg[DC6_XH] /DC_COFF ;
	data[DC6]= (int) tmp;
	
	tmp=0l;
	for ( i=0;i<POINT;i++)  tmp=tmp+ad_dc7_buf[i];	
	tmp= tmp * hld_reg[DC7_XH] /DC_COFF ;
	data[DC7]= (int) tmp;
	
	tmp=0l;
	for ( i=0;i<POINT;i++)  tmp=tmp+ad_dc8_buf[i];	
	tmp= tmp * hld_reg[DC8_XH] /DC_COFF ;
	data[DC8]= (int) tmp;
	
        tmp=tempreture;
	tmp= (tmp-1615)*704/410;
	data[TEMPR]=(int) tmp;

        for ( i=0;i<REAL_AI;i++)
        {
          ftmp = data[DC1+i];
          ftmp = ftmp- hld_reg[DC1_MIN+i];
          ftmp = ftmp * hld_reg[DC1_SCALE+i];
          tmp=   hld_reg[DC1_MAX+i]- hld_reg[DC1_MIN+i];
          if ( tmp !=0 )   ftmp = ftmp /tmp;
          float_data[i]= ftmp;
        }
}


void dc_cal_avr ( void )
{
     INT8U tmp;

     avr_cnt ++;
     for ( tmp=0;tmp<MAX_FLOAT;tmp ++)
     {
       float_avr_data[tmp]=float_avr_data[tmp]+float_data[tmp];

     }

}

void dc_get_avr( void )
{
  INT8U  tmp;

  if (avr_cnt !=0 )
   {
     for ( tmp=0;tmp<MAX_FLOAT;tmp ++)
      {
       avr_data[tmp]=float_avr_data[tmp]/avr_cnt;
       float_avr_data[tmp]=0.0;
      }
   }
  else
  {
    for ( tmp=0;tmp<MAX_FLOAT;tmp ++)
      avr_data[tmp]=float_data[tmp];
  }

  avr_cnt =0;

}

