#include    "cpu.h"
#include    "types.h"

#define SCK_HIGH  P5OUT |=BIT5
#define SCK_LOW   P5OUT &=~BIT5
#define SCK_OUT   P5DIR |=BIT5

#define SD_HIGH   P5OUT |=BIT6
#define SD_LOW    P5OUT &=~BIT6
#define SD_IN     P5DIR &=~BIT6
#define SD_OUT    P5DIR |=BIT6
#define SD        P5IN  & BIT6

#define RST_OUT   P5DIR |=BIT7
#define RST_LOW   P5OUT &=~BIT7
#define RST_HIGH  P5OUT |=BIT7

#define CMD       0x80
#define RDCLK     0x81
#define RDRAM     0xC1
#define WRCLK     0x80
#define WRRAM     0xC0

#define SEC 0
#define MIN 2
#define HR  4
#define DATE 6
#define MON  8
#define DAY  10
#define YEAR 12
#define CONTROL 14
#define TICK   16

void kill_wd(void);
void wr ( INT8U   cmd )
{
  INT8U  i;
  SCK_OUT;SD_OUT;
  SCK_LOW;_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
  SD_LOW;_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
  for (i=0;i<8;i++)
  {  _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
  _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
  _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
     if ( cmd & 0x1 )    SD_HIGH;
     else SD_LOW;
    _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
    _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
    _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
    SCK_HIGH;_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
    _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
    SCK_LOW;_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
    _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
    cmd>>=1;
  }

}
INT8U  rd ( void )
{
  INT8U  i,tmp,tmp1;
  SCK_OUT;
  SCK_LOW;_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
  SD_IN;_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
  tmp=0;tmp1=0x1;
  for (i=0;i<8;i++)
  {
     _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
    if (SD )  tmp |=tmp1;
    else tmp &=(~tmp1);
     _NOP();_NOP();_NOP();_NOP();_NOP();
    SCK_HIGH;_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
    _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
    SCK_LOW;_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
    _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();

    tmp1<<=1;
  }
 return tmp;
}
void set_time(INT8U  adr,INT8U  time)
{

    SCK_OUT;SD_OUT;
    SCK_LOW;_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
    SD_LOW;_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
    RST_LOW;_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
    _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
    RST_HIGH;_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
   _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
    wr( WRCLK | adr);
    wr( time);
   _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
    RST_LOW;_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
  _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
}

INT8U  read_time(INT8U  adr)
{

    INT8U  tmp;
    SCK_OUT;SD_OUT;
    SCK_LOW;_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
    SD_LOW;_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
    RST_LOW;_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
    _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
    RST_HIGH;_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
    _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
     wr( RDCLK | adr);
     tmp=rd( );
    _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
     RST_LOW;_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
    _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
     return tmp;
}
void ds1302_rtc_init(void)
{
 INT16U  i;

  RST_OUT;
  RST_HIGH;
  i=10; while (i--) _NOP();
  set_time(CONTROL,0);
  i= read_time(SEC);
  if ( i&0x80) set_time(SEC,i &0x7f);
  set_time(TICK,0xa5);



}
void read_rtctime_all(INT16U * tmp_tmr)
{
    INT16U  tmp;

   tmp=read_time(YEAR);
   tmp = tmp * 256+read_time( MON);
   *tmp_tmr++= tmp;

   tmp=read_time(DATE);
   tmp = tmp * 256+read_time(HR);
   *tmp_tmr++= tmp;

   tmp=read_time(MIN);
   tmp = tmp * 256+read_time( SEC);
   *tmp_tmr= tmp;


}

void read_ds1302time_all(INT16U * tmp_tmr)
{
   INT16U  tmp;
   INT8U   tmp_sec;

   tmp_sec=read_time( SEC);
   tmp=0;
   while ( (tmp_sec ^ read_time( SEC)) == 0)
     {
     _NOP();
     tmp++;
     kill_wd();
     if (tmp>6000 ) break;
      };
   tmp=read_time(YEAR);
   tmp = tmp * 256+read_time( MON);
   *tmp_tmr++= tmp;

   tmp=read_time(DATE);
   tmp = tmp * 256+read_time(HR);
   *tmp_tmr++= tmp;

   tmp=read_time(MIN);
   tmp = tmp * 256+read_time( SEC);
   *tmp_tmr= tmp;

}

void set_ds1302all_time ( INT8U hour_tmp,INT8U min_tmp,INT8U sec_tmp)
{
    set_time( HR,hour_tmp);
    set_time( MIN,min_tmp);
    set_time( SEC,sec_tmp);

}


void set_ds1302_date (  INT8U year_tmp,INT8U month_tmp,INT8U date_tmp)
{

    set_time( YEAR,year_tmp);
    set_time( MON,month_tmp);
    set_time( DATE,date_tmp);

}

