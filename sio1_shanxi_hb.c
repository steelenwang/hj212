#include "cpu.h"
#include "types.h"
#include "const.h"
#include "var.h"
#include "sub.h"
#include <ctype.h>

#define     TEST    0
#define     WCLK    1
#define     RCLK 2
#define     WUPT 3
#define     RUPT 4
#define     SRDA 5
#define     ERDA 6
#define     STDC 7
#define     RDDC 8
#define     UPOD 9
#define     UPRD 10
#define     SDCH 11
#define     DTDC 12
#define     ECRC 13


#define     MIN10  60000L
#define     MIN30  18000L
#define     MIN60  36000L
#define     T5S    500

const INT8U STAR[]={"STAR"};

const INT8U END []={"END"};

const INT8U TEST_RES[]={"STARTEST002OKXXEND"};
const INT8U WCLK_RES[]={"STARWCLK00200XXEND"};
const INT8U RCLK_RES[]={"STARRCLK012yymmddhhmmssXXEND"};
const INT8U WUPT_RES[]={"STARWUPT00200XXEND"};
const INT8U RUPT_RES[]={"STARRUPT001dXXEND"};
const INT8U SRDA_RES[]={"STARSRDA00200XXEND"};
const INT8U ERDA_RES[]={"STARERDA00200XXEND"};
const INT8U STDC_RES[]={"STARSTDC00200XXEND"};
const INT8U RDDC_RES[]={"STARRDDC003cccXXEND"};
const INT8U SDCH_RES[]={"STARSDCH001xXXEND"};

  const INT8U *CMD_Str[]=
  {

   "TEST",   //0
   "WCLK",   //1
   "RCLK",   //2
   "WUPT", //3
   "RUPT", //4
   "SRDA", //5
   "ERDA", //6
   "STDC", //7
   "RDDC", //8
   "UPOD", //9
   "UPRD", //10
   "SDCH", //11
   "DTDC", //12
   "ECRC", //13

   "...."
  };
typedef struct
  {  INT16U  star;
     INT16U  type;
     INT16U  lenth;
     INT8U   *data;
     INT16U  crc;
     INT16U  end;
  } CMD_RESULT;

CMD_RESULT cmd_result;
INT8U Aset,Dset,Kset;
INT8U data_type[3];
INT32U shanxi_send_tmr;
INT16U shanxi_store_tmr;
INT16U shanxi_real_tmr;
INT16U shanxi_pwr_tmr;

INT16U shanxi_sio3_tmr,shanxi_sio4_tmr,shanxi_sio5_tmr,shanxi_sio6_tmr;


void sio1_led_on(void);
void sio3_init ( void);
void sio4_init ( void);
void sio5_init ( void);
void sio6_init ( void);
void shanxi_store_data( void);
void dc_get_avr ( void);

INT8U at45db_wr_page ( INT16U page_adr,INT8U *buf );
INT8U at45db_rd (INT16U page_adr,\
                 INT16U in_page_adr,\
                 INT8U *buf,\
                 INT16U lenth );



INT16U shanxihb_crc(INT8U *data_to_cal_crc, INT16U len)


{
	INT16U crc;
	INT8U i;
	
	crc = 0xFFFF;			//CRC初始值
	
	while (len-- != 0)
	{
		crc = crc ^ ((INT16U)*data_to_cal_crc++ << 8);
		
		for (i = 0; i < 8; ++i)
			if (crc & 0x8000)
				crc = (crc << 1) ^ 0x1021;
			else
				crc = crc << 1;
	}

	return (crc & 0xFFFF);
}

void  shanxi_hb_init( void)
{
  Aset=hld_reg[SHANXI_ASET];
  Dset=hld_reg[SHANXI_DSET];
  Kset=hld_reg[SHANXI_KSET];
  shanxi_send_tmr=MIN10;
  shanxi_real_tmr=0;
  shanxi_store_tmr=0;
}

void shixi_hb_init()
{

}

INT16U  cmp_str(INT8U *buf,INT8U *dest, INT16U lenth )
{
 INT8U tmp;

 for ( tmp=0;tmp<lenth;tmp ++ )
  {
    if ( *buf != *dest ) return  0x100;
    buf++;
    dest++;
  }
  return 0;

}

INT16U  read_int ( INT8U *buf,INT8U lenth )
{
   INT16U  tmp_result;
   INT8U tmp;
   tmp_result =0;
   for ( tmp=0;tmp<lenth;tmp++)
    {
      if ( isdigit ( *(buf+tmp) ) )
           tmp_result = tmp_result *10 +*(buf+tmp)-'0';
      else break;
    }
   return tmp_result;

}



INT16U  read_hex ( INT8U *buf,INT8U lenth )
{

   INT16U  tmp_result;
   INT8U tmp;
   tmp_result =0;
   for ( tmp=0;tmp<lenth;tmp++)
    {
      if ( isdigit ( *(buf+tmp) ) )
           tmp_result = tmp_result *16 +*(buf+tmp)-'0';
      else if ( isalpha( *(buf+tmp) ) )
        {
          if ( *(buf+tmp) >='A' &&  *(buf+tmp) <='F')
            tmp_result = tmp_result *16 +*(buf+tmp)-'A'+10;
          else if ( *(buf+tmp) >='a' &&  *(buf+tmp) <='f')
            tmp_result = tmp_result *16 +*(buf+tmp)-'a'+10;
          else return tmp_result;
        }
      else  return tmp_result;
    }
   return tmp_result;



}

void write_int (INT8U *buf,INT16U tmp_data )
{
   *buf++ = tmp_data/1000+'0';
   tmp_data %=1000;
   *buf++ = tmp_data/100+'0';
   tmp_data %=100;
   *buf++ = tmp_data/10+'0';
   *buf = tmp_data%10 +'0';

}

void write_lenth (INT8U *buf,INT16U tmp_data )
{

   tmp_data %=1000;
   *buf++ = tmp_data/100+'0';
   tmp_data %=100;
   *buf++ = tmp_data/10+'0';
   *buf = tmp_data%10 +'0';

}

void write_hex (INT8U *buf,INT16U tmp_data )
{
   *buf++ = tmp_data/4096+'0';
   tmp_data %=4096;
   *buf++ = tmp_data/256+'0';
   tmp_data %=256;
   *buf++ = tmp_data/16+'0';
   *buf = tmp_data% 16 +'0';

}


void shanxi_send_realdata(void)
{
  const INT8U SEND_RDATA[]= {"STARUPRDxxx"};
  INT16U  tmp,tmp_val;
  INT8U   tmp8,shift,tmp_pos;
  for ( tmp =0;tmp<sizeof(SEND_RDATA)-1;tmp++)
   {
      sio1_buf[tmp]=SEND_RDATA[tmp];
   }
   write_hex ( &sio1_buf[tmp],hld_reg[H_YEAR]);
   tmp+=4;
   write_hex ( &sio1_buf[tmp],hld_reg[H_DAY]);
   tmp+=4;
   write_hex ( &sio1_buf[tmp],hld_reg[H_MIN]);
   tmp+=4;
   shift=0x01;
   for ( tmp8 =0;tmp8<8;tmp8++)
   {
       if ( Aset &shift )
       {
         sio1_buf[tmp]='A';
         tmp++;
         sio1_buf[tmp]='0'+tmp8;
         tmp++;
         tmp_val = (INT16U ) float_data[tmp8];
         write_int(&sio1_buf[tmp],tmp_val);
         tmp+=4;
       }
      shift=shift<<1;
   }
   sio1_buf[tmp]='K';
   tmp++;
   sio1_buf[tmp]= di[0];
   tmp++;
   tmp_pos =tmp;
   tmp_pos= tmp_pos-11;
   write_lenth( &sio1_buf[8],tmp_pos);

   tmp_val =shanxihb_crc( sio1_buf,tmp );
   sio1_buf[tmp]= tmp_val /256;
   tmp++;
   sio1_buf[tmp]= tmp_val %256;
   tmp++;
   sio1_buf[tmp]='E';tmp++;
   sio1_buf[tmp]='N';tmp++;
   sio1_buf[tmp]='D';tmp++;

   com1_tr();


   sio1_tx_lenth =0;
   sio1_tx_lenth2=tmp;
   UCA0TXBUF= sio1_buf[0];

}
void shanxi_send_avr_val(void)
{

  const INT8U SEND_RDATA[]= {"STARUPTDxxx"};
  INT16U  tmp,tmp_val;
  INT8U   tmp8,shift,tmp_pos;

  dc_get_avr();

  for ( tmp =0;tmp<sizeof(SEND_RDATA)-1;tmp++)
   {
      sio1_buf[tmp]=SEND_RDATA[tmp];
   }
   write_hex ( &sio1_buf[tmp],hld_reg[H_YEAR]);
   tmp+=4;
   write_hex ( &sio1_buf[tmp],hld_reg[H_DAY]);
   tmp+=4;
   write_hex ( &sio1_buf[tmp],hld_reg[H_MIN]);
   tmp+=4;
   shift=0x01;
   for ( tmp8 =0;tmp8<8;tmp8++)
   {
       if ( Aset &shift )
       {
         sio1_buf[tmp]='A';
         tmp++;
         sio1_buf[tmp]='0'+tmp8;
         tmp++;
         tmp_val = (INT16U ) avr_data[tmp8];
         write_int(&sio1_buf[tmp],tmp_val);
         tmp+=4;
       }
      shift=shift<<1;
   }
   sio1_buf[tmp]='K';
   tmp++;
   sio1_buf[tmp]= di[0];
   tmp++;
   tmp_pos =tmp;

   tmp_pos= tmp_pos-11;
   write_lenth( &sio1_buf[8],tmp_pos);

   tmp_val =shanxihb_crc( sio1_buf,tmp );
   sio1_buf[tmp]= tmp_val /256;
   tmp++;
   sio1_buf[tmp]= tmp_val %256;
   tmp++;
   sio1_buf[tmp]='E';tmp++;
   sio1_buf[tmp]='N';tmp++;
   sio1_buf[tmp]='D';tmp++;

   com1_tr();


   sio1_tx_lenth =0;
   sio1_tx_lenth2=tmp;
   UCA0TXBUF= sio1_buf[0];

}
void shanxi_send_hisdata( void )
{

}
void send_sio3_recv (void)
{
  INT16U  tmp_lenth;
  INT16U   tmp;

  INT8U UPDC_RES[]={"STARUPDCNOW"};


  if ( sio3_rx_lenth ==0 ) return;

  for ( tmp=0;tmp<sizeof(UPDC_RES)-1;tmp++)
  {
    sio1_buf[tmp]=UPDC_RES[tmp];
  }
  write_hex( &sio1_buf[tmp],hld_reg[H_YEAR]);
  tmp+=4;
  write_hex( &sio1_buf[tmp],hld_reg[H_DAY]);
  tmp+=4;
  write_hex( &sio1_buf[tmp],hld_reg[H_MIN]);
  tmp+=2;
  write_lenth( &sio1_buf[tmp],sio3_rx_lenth+1);
  tmp+=3;
  sio1_buf[tmp]='0';
  tmp ++;

  for ( tmp_lenth=0;tmp_lenth<sio3_rx_lenth;tmp_lenth++)
  {
     sio1_buf[tmp++]= sio3_buf[tmp_lenth];
  }

   tmp_lenth =shanxihb_crc( sio1_buf,tmp );
   sio1_buf[tmp]= tmp_lenth /256;
   tmp++;
   sio1_buf[tmp]= tmp_lenth %256;
   tmp++;
   sio1_buf[tmp]='E';tmp++;
   sio1_buf[tmp]='N';tmp++;
   sio1_buf[tmp]='D';tmp++;
   sio1_buf[8]=data_type[0];
   sio1_buf[9]=data_type[1];
   sio1_buf[10]=data_type[2];
   com1_tr();
    sio3_rx_lenth=0;
    fr3_ready =0;

   sio1_tx_lenth =0;
   sio1_tx_lenth2=tmp;
   UCA0TXBUF= sio1_buf[0];




}
void send_sio4_recv (void)
{
  INT16U  tmp_lenth;
  INT16U   tmp;

  INT8U UPDC_RES[]={"STARUPDCNOW"};


  if ( sio4_rx_lenth ==0 ) return;

  for ( tmp=0;tmp<sizeof(UPDC_RES)-1;tmp++)
  {
    sio1_buf[tmp]=UPDC_RES[tmp];
  }
  write_hex( &sio1_buf[tmp],hld_reg[H_YEAR]);
  tmp+=4;
  write_hex( &sio1_buf[tmp],hld_reg[H_DAY]);
  tmp+=4;
  write_hex( &sio1_buf[tmp],hld_reg[H_MIN]);
  tmp+=2;
  write_lenth( &sio1_buf[tmp],sio4_rx_lenth+1);
  tmp+=3;
  sio1_buf[tmp]='1';
  tmp ++;

  for ( tmp_lenth=0;tmp_lenth<sio4_rx_lenth;tmp_lenth++)
  {
     sio1_buf[tmp++]= sio4_buf[tmp_lenth];
  }

   tmp_lenth =shanxihb_crc( sio1_buf,tmp );
   sio1_buf[tmp]= tmp_lenth /256;
   tmp++;
   sio1_buf[tmp]= tmp_lenth %256;
   tmp++;
   sio1_buf[tmp]='E';tmp++;
   sio1_buf[tmp]='N';tmp++;
   sio1_buf[tmp]='D';tmp++;
   sio1_buf[8]=data_type[0];
   sio1_buf[9]=data_type[1];
   sio1_buf[10]=data_type[2];
   com1_tr();
    sio4_rx_lenth=0;
    fr4_ready =0;

   sio1_tx_lenth =0;
   sio1_tx_lenth2=tmp;
   UCA0TXBUF= sio1_buf[0];


}
void send_sio5_recv (void)
{
  INT16U  tmp_lenth;
  INT16U   tmp;

  INT8U UPDC_RES[]={"STARUPDCNOW"};


  if ( sio5_rx_lenth ==0 ) return;

  for ( tmp=0;tmp<sizeof(UPDC_RES)-1;tmp++)
  {
    sio1_buf[tmp]=UPDC_RES[tmp];
  }
  write_hex( &sio1_buf[tmp],hld_reg[H_YEAR]);
  tmp+=4;
  write_hex( &sio1_buf[tmp],hld_reg[H_DAY]);
  tmp+=4;
  write_hex( &sio1_buf[tmp],hld_reg[H_MIN]);
  tmp+=2;
  write_lenth( &sio1_buf[tmp],sio5_rx_lenth+1);
  tmp+=3;
  sio1_buf[tmp]='2';
  tmp ++;

  for ( tmp_lenth=0;tmp_lenth<sio5_rx_lenth;tmp_lenth++)
  {
     sio1_buf[tmp++]= sio5_buf[tmp_lenth];
  }

   tmp_lenth =shanxihb_crc( sio1_buf,tmp );
   sio1_buf[tmp]= tmp_lenth /256;
   tmp++;
   sio1_buf[tmp]= tmp_lenth %256;
   tmp++;
   sio1_buf[tmp]='E';tmp++;
   sio1_buf[tmp]='N';tmp++;
   sio1_buf[tmp]='D';tmp++;
   sio1_buf[8]=data_type[0];
   sio1_buf[9]=data_type[1];
   sio1_buf[10]=data_type[2];
   com1_tr();
    sio5_rx_lenth=0;
    fr5_ready =0;

   sio1_tx_lenth =0;
   sio1_tx_lenth2=tmp;
   UCA0TXBUF= sio1_buf[0];


}
void send_sio6_recv (void)
{
  INT16U  tmp_lenth;
  INT16U   tmp;

  INT8U UPDC_RES[]={"STARUPDCNOW"};


  if ( sio6_rx_lenth ==0 ) return;

  for ( tmp=0;tmp<sizeof(UPDC_RES)-1;tmp++)
  {
    sio1_buf[tmp]=UPDC_RES[tmp];
  }
  write_hex( &sio1_buf[tmp],hld_reg[H_YEAR]);
  tmp+=4;
  write_hex( &sio1_buf[tmp],hld_reg[H_DAY]);
  tmp+=4;
  write_hex( &sio1_buf[tmp],hld_reg[H_MIN]);
  tmp+=2;
  write_lenth( &sio1_buf[tmp],sio6_rx_lenth+1);
  tmp+=3;
  sio1_buf[tmp]='3';
  tmp ++;

  for ( tmp_lenth=0;tmp_lenth<sio6_rx_lenth;tmp_lenth++)
  {
     sio1_buf[tmp++]= sio6_buf[tmp_lenth];
  }

   tmp_lenth =shanxihb_crc( sio1_buf,tmp );
   sio1_buf[tmp]= tmp_lenth /256;
   tmp++;
   sio1_buf[tmp]= tmp_lenth %256;
   tmp++;
   sio1_buf[tmp]='E';tmp++;
   sio1_buf[tmp]='N';tmp++;
   sio1_buf[tmp]='D';tmp++;
   sio1_buf[8]=data_type[0];
   sio1_buf[9]=data_type[1];
   sio1_buf[10]=data_type[2];
   com1_tr();
    sio6_rx_lenth=0;
    fr6_ready =0;

   sio1_tx_lenth =0;
   sio1_tx_lenth2=tmp;
   UCA0TXBUF= sio1_buf[0];



}


void shanxi_tmr_proc(void)
{
  INT16U  tmp_val;
  static  INT8U t1s_tmr =0;

  if (  protocol_tmr !=0)
  { _DINT();
    tmp_val=protocol_tmr;
    protocol_tmr=0;
   _EINT();



    if ( shanxi_real_tmr)
    {
      if (shanxi_real_tmr > tmp_val)
      {
        shanxi_real_tmr -=tmp_val;
       }
     else
      {
        shanxi_real_tmr =hld_reg[ SHANXI_REAL_TMR];
        sio1_led_on();
        shanxi_send_realdata();
      }
    }

     if ( shanxi_send_tmr)
    {
      if (shanxi_send_tmr > tmp_val)
       {
           shanxi_send_tmr -=tmp_val;

       }
     else
      {
           switch (hld_reg[ SHANXI_SEND_TMR])
           {
             case 1:
             shanxi_send_tmr=MIN10;
             if ( hld_reg[ SHANXI_REAL_TMR]==0 )
             shanxi_send_avr_val();
             sio1_led_on();
             break;

             case 2:
             shanxi_send_tmr=MIN30;
             if ( hld_reg[ SHANXI_REAL_TMR]==0 )
              shanxi_send_avr_val();
             sio1_led_on();
             break;

             case 3:
             shanxi_send_tmr=MIN60;
             if ( hld_reg[ SHANXI_REAL_TMR]==0 )
              shanxi_send_avr_val();
             sio1_led_on();
             break;

             default:
             shanxi_send_tmr=0;
             break;
           }


      }
    }

    t1s_tmr += tmp_val;

    if ( t1s_tmr >=100)
    {
      t1s_tmr -=100;
      shanxi_pwr_tmr ++;
      if ( shanxi_pwr_tmr >1230 ) shanxi_pwr_tmr=0;
      else if ( shanxi_pwr_tmr >1200 ) do_out[0] &=~BIT4;
      else  do_out[0] |=BIT4;
      shanxi_store_data();

    }


    tmp_val = tmp_val *10;

    if(shanxi_sio3_tmr)
    {
      if ( shanxi_sio3_tmr >tmp_val)
      {
        shanxi_sio3_tmr -=tmp_val;
      }
      else
      {
       // shanxi_sio3_tmr =hld_reg[SIO3_TMOUT];
        shanxi_sio3_tmr=0;
        send_sio3_recv ();
        sio1_led_on();
      }
     }
  if(shanxi_sio4_tmr)
    {
      if ( shanxi_sio4_tmr >tmp_val)
      {
        shanxi_sio4_tmr -=tmp_val;
      }
      else
      {
        shanxi_sio4_tmr=0;
        send_sio4_recv ();
        sio1_led_on();
      }
    }
  if(shanxi_sio5_tmr)
    {
     if ( shanxi_sio5_tmr >tmp_val)
      {
        shanxi_sio5_tmr -=tmp_val;
      }
      else
      {
        shanxi_sio5_tmr=0;
        send_sio5_recv ();
        sio1_led_on();
      }
    }
  if(shanxi_sio6_tmr)
    {
      if ( shanxi_sio6_tmr >tmp_val)
      {
        shanxi_sio6_tmr -=tmp_val;
      }
      else
      {
        shanxi_sio6_tmr=0;
        send_sio6_recv ();
        sio1_led_on();
      }
    }






  }




}


INT16U  analyis_recieve ( void )
{
   //  > 255 err
   // 1~254 cmd
    // 255 CRC err
  INT16U tmp_result,tmp_crc;
  INT8U  tmp_p;

  cmd_result.star =cmp_str ( sio1_buf,(INT8U*)STAR,4);

  tmp_p=0;
  while (1)
  {
    if  (*CMD_Str[tmp_p]=='.') break;
    tmp_result = cmp_str ( (sio1_buf+4),(INT8U*)CMD_Str[tmp_p],4);
    if (tmp_result ==0 ) break;
    else tmp_p ++;
  }
  cmd_result.type = tmp_p;
  if (  tmp_p==DTDC )
    {
      tmp_result=read_int(&sio1_buf[21],3);
      cmd_result.lenth= tmp_result;
      cmd_result.data = sio1_buf+24;

      tmp_crc = sio1_buf[tmp_result+24];
      tmp_crc =tmp_crc *256;
      tmp_crc =tmp_crc  + sio1_buf[tmp_result+25];
      cmd_result.crc = tmp_crc;
      cmd_result.end =cmp_str ( (sio1_buf+tmp_result+26),(INT8U*)END,3);
      return  shanxihb_crc (sio1_buf,tmp_result+26);

    }
  else
  {
    tmp_result=read_int(&sio1_buf[8],3);
    cmd_result.lenth= tmp_result;
    cmd_result.data = sio1_buf+11;

    tmp_crc = sio1_buf[tmp_result+11];
    tmp_crc =tmp_crc *256;
    tmp_crc =tmp_crc  + sio1_buf[tmp_result+12];
    cmd_result.crc = tmp_crc;
    cmd_result.end =cmp_str ( (sio1_buf+tmp_result+13),(INT8U*)END,3);
    return  shanxihb_crc (sio1_buf,tmp_result+13);
  }
}

const INT8U CRC_ERR[]={"STARECRC000xxEND"};
const INT8U UPOD_ERR_RES[]={"STARUPOD002FFxxEND"};
const INT8U UPOD_RES[]={"STARUPOD06200"};
INT16U  search_day(INT16U year,INT16U day )
{
    INT16U tmp_pos,tmp_inpage_adr;
    INT16U tmp_cnt ,ytmp,dtmp,mtmp;
    INT8U  tmp_buf[32];

    tmp_cnt = 455;
    day =day/256;

    tmp_pos = hld_reg[AT45DB_PAGE];
    tmp_pos = tmp_pos /9;
    tmp_pos =tmp_pos *9;

    for ( ;tmp_cnt!=0; )
    {
      at45db_rd (tmp_pos,0,tmp_buf,sizeof (tmp_buf)-1 );
      ytmp = tmp_buf[0];
      ytmp = ytmp *256+tmp_buf[1];
      mtmp = tmp_buf[2];
      if ( (ytmp != year) || (mtmp != day) )
       {
       tmp_cnt --;
       if ( tmp_pos >9 )  tmp_pos -=9;
       else  tmp_pos =4085;
       continue;
       }
      else return tmp_pos;
    }
    return  0xffff;

}

void sio1_shanxi_hb(void )
{
    INT16U tmp_crc,tmp_h,tmp_val;
    INT8U tmp;
    INT8U *tmp_point;
    INT16U  ytmp,dtmp,mtmp;
    INT16U  tmp_pos,tmp_inpage_adr;

    INT8U tmp_cnt,tmp_buf[32];

    sio1_led_on();
    tmp_crc=analyis_recieve();

     if ( tmp_crc !=0 )
     {
        for ( tmp=0;tmp<sizeof (CRC_ERR);tmp++)
            sio1_buf[tmp]= CRC_ERR[tmp];

        com1_tr();
        tmp_crc = shanxihb_crc(sio1_buf,sizeof (CRC_ERR)-6);
        sio1_buf[sizeof (CRC_ERR)-6] = tmp_crc /256;
        sio1_buf[sizeof (CRC_ERR)-5] = tmp_crc %256;

        sio1_tx_lenth =0;
        sio1_tx_lenth2=sizeof (CRC_ERR)-1;
        UCA0TXBUF= sio1_buf[0];
        return ;
     }
    if ( cmd_result.star !=0 )return ;
    if ( cmd_result.end !=0 )return ;
    if  ( cmd_result.type > 13) return ;
    shanxi_pwr_tmr =0;
    switch ( cmd_result.type)
    {
     case     TEST:

        for ( tmp=0;tmp<sizeof (TEST_RES);tmp++)
            sio1_buf[tmp]= TEST_RES[tmp];

        com1_tr();
        tmp_crc = shanxihb_crc(sio1_buf,sizeof (TEST_RES)-6);
        sio1_buf[sizeof (TEST_RES)-6] = tmp_crc /256;
        sio1_buf[sizeof (TEST_RES)-5] = tmp_crc %256;

        sio1_tx_lenth =0;
        sio1_tx_lenth2=sizeof (TEST_RES)-1;
        UCA0TXBUF= sio1_buf[0];
     break;

     case     WCLK:
        tmp_crc = read_hex( &sio1_buf[11] ,4 );
        write_hldreg( H_YEAR,tmp_crc);
        tmp_crc = read_hex( &sio1_buf[15] ,4 );
        write_hldreg( H_DAY,tmp_crc);
        tmp_crc = read_hex( &sio1_buf[19] ,4 );
        write_hldreg( H_MIN,tmp_crc);

        for ( tmp=0;tmp<sizeof (WCLK_RES);tmp++)
            sio1_buf[tmp]= WCLK_RES[tmp];

        com1_tr();
        tmp_crc = shanxihb_crc(sio1_buf,sizeof (WCLK_RES)-6);
        sio1_buf[sizeof (WCLK_RES)-6] = tmp_crc /256;
        sio1_buf[sizeof (WCLK_RES)-5] = tmp_crc %256;
        sio1_tx_lenth =0;
        sio1_tx_lenth2=sizeof (WCLK_RES)-1;
        UCA0TXBUF= sio1_buf[0];

     break;

     case     RCLK:
        for ( tmp=0;tmp<sizeof (RCLK_RES);tmp++)
            sio1_buf[tmp]= RCLK_RES[tmp];

        write_hex( &sio1_buf[11],hld_reg[H_YEAR]);
        write_hex( &sio1_buf[15],hld_reg[H_DAY]);
        write_hex( &sio1_buf[19],hld_reg[H_MIN]);

        com1_tr();
        tmp_crc = shanxihb_crc(sio1_buf,sizeof (RCLK_RES)-6);
        sio1_buf[sizeof (RCLK_RES)-6] = tmp_crc /256;
        sio1_buf[sizeof (RCLK_RES)-5] = tmp_crc %256;
        sio1_tx_lenth =0;
        sio1_tx_lenth2=sizeof (RCLK_RES)-1;
        UCA0TXBUF= sio1_buf[0];
     break;

     case     WUPT:
         //process tmr DATA
        tmp_crc = read_hex (&sio1_buf[11],1);
        write_hldreg( SHANXI_SEND_TMR,tmp_crc);

        switch ( tmp_crc )
        {
          case 0:
            shanxi_send_tmr =0;
          break;

          case 1:
            shanxi_send_tmr =MIN10;
          break;

          case 2:
            shanxi_send_tmr =MIN30;
          break;

          case 3:
            shanxi_send_tmr =MIN60;
          break;

          default:
            shanxi_send_tmr =0;
          break;

        }

        for ( tmp=0;tmp<sizeof (WUPT_RES);tmp++)
            sio1_buf[tmp]= WUPT_RES[tmp];

        com1_tr();
        tmp_crc = shanxihb_crc(sio1_buf,sizeof (WUPT_RES)-6);
        sio1_buf[sizeof (WUPT_RES)-6] = tmp_crc /256;
        sio1_buf[sizeof (WUPT_RES)-5] = tmp_crc %256;
        sio1_tx_lenth =0;
        sio1_tx_lenth2=sizeof (WUPT_RES)-1;
        UCA0TXBUF= sio1_buf[0];

     break;

     case     RUPT:

       for ( tmp=0;tmp<sizeof (RUPT_RES);tmp++)

         sio1_buf[tmp]= RUPT_RES[tmp];

       //process tmr DATA
       if ( shanxi_send_tmr ==MIN10 )
              sio1_buf[11]= '1';
       else if (shanxi_send_tmr ==MIN30)
              sio1_buf[11]= '2';
       else if (shanxi_send_tmr ==MIN60)
              sio1_buf[11]= '3';
       else   sio1_buf[11]= '0';

        com1_tr();
        tmp_crc = shanxihb_crc(sio1_buf,sizeof (RUPT_RES)-6);
        sio1_buf[sizeof (RUPT_RES)-6] = tmp_crc /256;
        sio1_buf[sizeof (RUPT_RES)-5] = tmp_crc %256;
        sio1_tx_lenth =0;
        sio1_tx_lenth2=sizeof (RUPT_RES)-1;
        UCA0TXBUF= sio1_buf[0];
     break;

     case     SRDA:

        shanxi_real_tmr =T5S;
        hld_reg[SHANXI_REAL_TMR]=T5S;
        for ( tmp=0;tmp<sizeof (SRDA_RES);tmp++)
             sio1_buf[tmp]= SRDA_RES[tmp];

        com1_tr();
        tmp_crc = shanxihb_crc(sio1_buf,sizeof (SRDA_RES)-6);
        sio1_buf[sizeof (SRDA_RES)-6] = tmp_crc /256;
        sio1_buf[sizeof (SRDA_RES)-5] = tmp_crc %256;
        sio1_tx_lenth =0;
        sio1_tx_lenth2=sizeof (SRDA_RES)-1;
        UCA0TXBUF= sio1_buf[0];

     break;

     case     ERDA:
        shanxi_real_tmr =0;
        hld_reg[SHANXI_REAL_TMR]=0;
        for ( tmp=0;tmp<sizeof (ERDA_RES);tmp++)
             sio1_buf[tmp]= ERDA_RES[tmp];

        com1_tr();
        tmp_crc = shanxihb_crc(sio1_buf,sizeof (ERDA_RES)-6);
        sio1_buf[sizeof (ERDA_RES)-6] = tmp_crc /256;
        sio1_buf[sizeof (ERDA_RES)-5] = tmp_crc %256;
        sio1_tx_lenth =0;
        sio1_tx_lenth2=sizeof (ERDA_RES)-1;
        UCA0TXBUF= sio1_buf[0];
     break;

     case     STDC:

        Aset=sio1_buf[11];
        Dset=sio1_buf[12];
        Kset=sio1_buf[13];

        write_hldreg( SHANXI_ASET,Aset);
        write_hldreg( SHANXI_DSET,Dset);
        write_hldreg( SHANXI_KSET,Kset);

       for ( tmp=0;tmp<sizeof (STDC_RES);tmp++)
             sio1_buf[tmp]= STDC_RES[tmp];

        com1_tr();
        tmp_crc = shanxihb_crc(sio1_buf,sizeof (STDC_RES)-6);
        sio1_buf[sizeof (STDC_RES)-6] = tmp_crc /256;
        sio1_buf[sizeof (STDC_RES)-5] = tmp_crc %256;

        sio1_tx_lenth =0;
        sio1_tx_lenth2=sizeof (STDC_RES)-1;
        UCA0TXBUF= sio1_buf[0];
     break;

     case     RDDC:
        for ( tmp=0;tmp<sizeof (RDDC_RES);tmp++)
             sio1_buf[tmp]= RDDC_RES[tmp];
        sio1_buf[11]= Aset;
        sio1_buf[12]= Dset;
        sio1_buf[13]= Kset;
        com1_tr();
        tmp_crc = shanxihb_crc(sio1_buf,sizeof (RDDC_RES)-6);
        sio1_buf[sizeof (RDDC_RES)-6] = tmp_crc /256;
        sio1_buf[sizeof (RDDC_RES)-5] = tmp_crc %256;

        sio1_tx_lenth =0;
        sio1_tx_lenth2=sizeof (RDDC_RES)-1;
        UCA0TXBUF= sio1_buf[0];
      break;

     case     UPOD:   //主机召唤历史数据
        ytmp = read_hex ( &sio1_buf[11],4 );
        dtmp = read_hex ( &sio1_buf[15],4);
        mtmp = read_hex ( &sio1_buf[19],2);

        tmp_pos=search_day( ytmp,dtmp);
      if (   tmp_pos ==0xffff )
       {         // not find  the day data

          for ( tmp=0;tmp<sizeof (UPOD_ERR_RES);tmp++)
             sio1_buf[tmp]= UPOD_ERR_RES[tmp];
          com1_tr();
          tmp_crc = shanxihb_crc(sio1_buf,sizeof (UPOD_ERR_RES)-6);
          sio1_buf[sizeof (UPOD_ERR_RES)-6] = tmp_crc /256;
          sio1_buf[sizeof (UPOD_ERR_RES)-5] = tmp_crc %256;

          sio1_tx_lenth =0;
          sio1_tx_lenth2=sizeof (UPOD_ERR_RES)-1;
          UCA0TXBUF= sio1_buf[0];
       }
      else
       {          // find the data of the day
         tmp_cnt =144;
         tmp_inpage_adr =0;
         tmp_crc = dtmp &0xff;
         tmp_crc = tmp_crc *256 + mtmp ;


         for ( ;tmp_cnt!=0; )
          {
             at45db_rd (tmp_pos,tmp_inpage_adr,tmp_buf,sizeof (tmp_buf)-1 );
             tmp_h = tmp_buf[3];
             tmp_h = tmp_h *256+tmp_buf[4];

              if ( tmp_h < tmp_crc)  //读取的时间 > 给定的时间 查到数据
               {
                tmp_cnt --;
                tmp_inpage_adr +=32;
                tmp_inpage_adr &=0x1ff;
                if ( tmp_inpage_adr ==0) tmp_pos ++;
               }
               else  break;
          }
         // get the data
          for ( tmp=0;tmp<sizeof (UPOD_RES)-1;tmp++)
                sio1_buf[tmp]= UPOD_RES[tmp];

          write_hex(&sio1_buf[tmp],ytmp );
          tmp+=4;
          write_hex(&sio1_buf[tmp],dtmp );
          tmp+=4;

          mtmp *=256;
          write_hex(&sio1_buf[tmp],mtmp );
          tmp+=2;

          for ( tmp_crc=0;tmp_crc<8;tmp_crc ++)
          {
            sio1_buf[tmp]='A';
            tmp++;
            sio1_buf[tmp]='0'+tmp_crc;
            tmp++;
            tmp_val = tmp_buf[6 +tmp_crc*2];
            tmp_val = tmp_val *256 + tmp_buf[7 +tmp_crc*2];
            write_int(&sio1_buf[tmp],tmp_val);
            tmp+=4;
          }
          sio1_buf[tmp]='K';
          tmp++;
          sio1_buf[tmp]=di[0];
          tmp++;


          tmp_val =shanxihb_crc( sio1_buf,tmp );
          sio1_buf[tmp]= tmp_val /256;
          tmp++;
          sio1_buf[tmp]= tmp_val %256;
          tmp++;
          sio1_buf[tmp]='E';tmp++;
          sio1_buf[tmp]='N';tmp++;
          sio1_buf[tmp]='D';tmp++;

          com1_tr();
          sio1_tx_lenth =0;
          sio1_tx_lenth2=tmp;
          UCA0TXBUF= sio1_buf[0];
       }

     break;

     case     UPRD:    //主动上传实时数据 不处理

     break;

     case     SDCH:   //设置数字通道

       switch ( sio1_buf[11] )
       {
        case '0':
           if ( sio1_buf[13]==1 )
           {hld_reg[SIO3_SET]= 0x1003+sio1_buf[12]-'1';
             tmp_crc = read_int ( &sio1_buf[14],4);
             write_hldreg( hld_reg[SIO3_TMOUT], tmp_crc);
             sio3_init();
           }
           else _NOP();
         break;

        case '1':
         if ( sio1_buf[13]==1 )
           {hld_reg[SIO4_SET]= 0x1003+sio1_buf[12]-'1';
             tmp_crc = read_int ( &sio1_buf[14],4);
             write_hldreg( hld_reg[SIO4_TMOUT], tmp_crc);
             sio4_init();
           }
           else _NOP();
         break;

        case '2':
         if ( sio1_buf[13]==1 )
           {hld_reg[SIO5_SET]= 0x1003+sio1_buf[12]-'1';
             tmp_crc = read_int ( &sio1_buf[14],4);
             write_hldreg( hld_reg[SIO5_TMOUT], tmp_crc);
             sio5_init();
           }
           else _NOP();
         break;

        case '3':
         if ( sio1_buf[13]==1 )
           {hld_reg[SIO6_SET]= 0x1003+sio1_buf[12]-'1';
             tmp_crc = read_int ( &sio1_buf[14],4);
             write_hldreg( hld_reg[SIO6_TMOUT], tmp_crc);
             sio6_init();
           }
           else _NOP();
         break;


       }

       for ( tmp=0;tmp<sizeof (SDCH_RES);tmp++)
             sio1_buf[tmp]= SDCH_RES[tmp];

        com1_tr();
        tmp_crc = shanxihb_crc(sio1_buf,sizeof (SDCH_RES)-6);
        sio1_buf[sizeof (SDCH_RES)-6] = tmp_crc /256;
        sio1_buf[sizeof (SDCH_RES)-5] = tmp_crc %256;

        sio1_tx_lenth =0;
        sio1_tx_lenth2=sizeof (SDCH_RES)-1;
        UCA0TXBUF= sio1_buf[0];
     break;

     case     DTDC:   //往数字通道发送数据
       tmp_point = cmd_result.data;
       data_type[0]=sio1_buf[8];
       data_type[1]=sio1_buf[9];
       data_type[2]=sio1_buf[10];
       switch (*tmp_point )
       {
        case '0':
         tmp_point ++;
         tmp_crc =cmd_result.lenth-1;
         for ( tmp=0;tmp<tmp_crc;tmp++) sio3_buf[tmp]= *tmp_point++;
         sio3_tx_lenth =0;
         sio3_tx_lenth2 =tmp;
         if (hld_reg[SIO3_TMOUT]<=10 )shanxi_sio3_tmr = 5000;
         else  shanxi_sio3_tmr =hld_reg[SIO3_TMOUT];

         sio3_rx_lenth =0;
         fr3_ready=0;
        break;

        case '1':
         tmp_point ++;
         tmp_crc =cmd_result.lenth-1;
         for ( tmp=0;tmp<tmp_crc;tmp++) sio4_buf[tmp]= *tmp_point++;
         sio4_tx_lenth =0;
         sio4_tx_lenth2 =tmp;
         if (hld_reg[SIO4_TMOUT]<=10 )shanxi_sio4_tmr = 5000;
         else  shanxi_sio4_tmr =hld_reg[SIO4_TMOUT];
         sio4_rx_lenth =0;
         fr4_ready=0;
        break;

        case '2':
         tmp_point ++;
         tmp_crc =cmd_result.lenth-1;
         for ( tmp=0;tmp<tmp_crc;tmp++) sio5_buf[tmp]= *tmp_point++;
         sio5_tx_lenth =0;
         sio5_tx_lenth2 =tmp;
         if (hld_reg[SIO5_TMOUT]<=10 )shanxi_sio5_tmr = 5000;
         else  shanxi_sio5_tmr =hld_reg[SIO5_TMOUT];
         sio5_rx_lenth =0;
         fr5_ready=0;
        break;

        case '3':
         tmp_point ++;
         tmp_crc =cmd_result.lenth-1;
         for ( tmp=0;tmp<tmp_crc;tmp++) sio6_buf[tmp]= *tmp_point++;
         sio6_tx_lenth =0;
         sio6_tx_lenth2 =tmp;
         if (hld_reg[SIO6_TMOUT]<=10 )shanxi_sio6_tmr = 5000;
         else  shanxi_sio6_tmr =hld_reg[SIO6_TMOUT];
         sio6_rx_lenth =0;
         fr6_ready=0;
        break;

        default :
         break;

       }
     break;

     case     ECRC:   //CRC错误

     break;

    default :
     break;


    }

}


void shanxi_store_data ( void )
{
  static  INT16U  store_tmr =0;
  INT8U  tmp;
  INT16S  tmp_val;
  INT16U  tmp_crc;
  INT16U  tmp_pos;

  if (hld_reg[SHANXI_STORE_TMR]==0 ) return;


  if ( store_tmr >= hld_reg[SHANXI_STORE_TMR] )
  {
     store_tmr-=hld_reg[SHANXI_STORE_TMR];
     tmp_pos =at_buf_point;

     for (tmp=0;tmp<3;tmp++)
     {
       at45db_buf[at_buf_point++]= hld_reg[H_YEAR+tmp] /256;
       at45db_buf[at_buf_point++]= hld_reg[H_YEAR+tmp] %256;
     }   //6 byte
     dc_get_avr();
     for (tmp=0;tmp<8;tmp++)
     {
       tmp_val = avr_data[DC1+tmp];
       at45db_buf[at_buf_point++]= tmp_val /256;
       at45db_buf[at_buf_point++]= tmp_val %256;
     }   //16byte
     at45db_buf[at_buf_point++]=di[0];
     at45db_buf[at_buf_point++]=di[1];
     at45db_buf[at_buf_point++]=di[2];
     at45db_buf[at_buf_point++]=do_out[0];
     tmp = at_buf_point- tmp_pos+1;
     tmp_crc = crc( &at45db_buf[tmp_pos],tmp );
     at45db_buf[at_buf_point++]= tmp_crc/256;
     at45db_buf[at_buf_point++]= tmp_crc%256;

     if ( tmp_pos >=480 )
       {
         at_buf_point =0;
         at45db_wr_page ( hld_reg[AT45DB_PAGE],at45db_buf ) ;   // write flash
         hld_reg[AT45DB_PAGE]++;
         hld_reg[AT45DB_PAGE] &=0xfff;
       }
     else at_buf_point =tmp_pos +32;

  }
   store_tmr ++;

  if ( (hld_reg[H_DAY]& 0xff) ==0 && ((hld_reg[H_MIN]<0x0002)) )
  {
    at_buf_point=0;
    tmp_pos=hld_reg[AT45DB_PAGE];
    tmp_pos = tmp_pos /9;
    tmp_pos = tmp_pos +1;
    tmp_pos =tmp_pos *9;
    if ( tmp_pos >=4094 ) tmp_pos =0;
    hld_reg[AT45DB_PAGE] =tmp_pos;
    store_tmr =0;
  }

}


