#include    "cpu.h"
#include    "types.h"
#include    "var.h"

#define AT45DB_CS_LOW   P7OUT&=~BIT5
#define AT45DB_CS_HI    P7OUT|=BIT5
#define AT45DB_RST_LOW   P7OUT&=~BIT4
#define AT45DB_RST_HI    P7OUT|=BIT4


#define AT45DB_ARRAY_RD  0xe8
#define AT45DB_PAGE_RD	 0xd2
#define AT45DB_BUF1_RD   0xd4
#define AT45DB_BUF2_RD   0xd6
#define AT45DB_STATUS_RD 0xd7

#define AT45DB_BUF1_WR   0x84
#define AT45DB_BUF2_WR   0x87
#define AT45DB_B12MER_WR 0x83
#define AT45DB_B22MER_WR 0x86
#define AT45DB_B12M_WR   0x88
#define AT45DB_B22M_WR   0x89
#define AT45DB_P_ER      0x81
#define AT45DB_B_ER      0x50
#define AT45DB_MPTB1     0x82
#define AT45DB_MPTB2     0x85

#define AT45DB_M2B1      0x53
#define AT45DB_M2B2      0x55
#define AT45DB_MCMPB1    0x60
#define AT45DB_MCMPB2    0x61

void delay ( void )
{
     _NOP();_NOP();_NOP();_NOP();_NOP();
     _NOP();_NOP();_NOP();_NOP();_NOP();
     _NOP();_NOP();_NOP();_NOP();_NOP();
     _NOP();_NOP();_NOP();_NOP();_NOP();

}
void at45db_init ( void )
{
  INT16U tmp;

  AT45DB_RST_LOW;
  for ( tmp=0;tmp<sizeof(at45db_buf) ;tmp ++)
   {
     at45db_buf[tmp]=0xff;
   }
  AT45DB_RST_HI;
  at_buf_point=0;

}

void at45db_cs_low (void)
{
  AT45DB_CS_LOW;
  _NOP();_NOP();_NOP();_NOP();_NOP();
}

void at45db_cs_hi (void)
{
  AT45DB_CS_HI ;
  _NOP();_NOP();_NOP();_NOP();_NOP();
}

INT8U at45db_get_status ( void)
{

     at45db_cs_low();
     U1TXBUF= AT45DB_STATUS_RD;
     delay();
     U1TXBUF= 0x55;
     delay();
     at45db_cs_hi();
     return U1RXBUF;   	
}

INT8U at45db_wr_page ( INT16U page_adr,INT8U *buf )
{
  INT8U tmp1,tmp2;
  INT16U tmp_lenth;

  if (at45db_get_status ()!=0xac ) return 255;
  page_adr &=0xfff;
  page_adr <<=2;
  tmp1 =page_adr /256;
  tmp2 =page_adr %256;
  at45db_cs_low();
  U1TXBUF= AT45DB_BUF1_WR;  //command
  delay();
  U1TXBUF=0x00;             //don't care
  delay();
  U1TXBUF=0x00;             //don't care
  delay();
  U1TXBUF=0x00;             // in page adr
  delay();

  for ( tmp_lenth=0;tmp_lenth<512;tmp_lenth++)
  {
    U1TXBUF=*buf;
    buf++;
    delay();delay();delay();
  }
  delay();delay();
  at45db_cs_hi();
  delay(); delay(); delay(); delay(); delay();
  delay(); delay(); delay();delay(); delay();
  delay(); delay(); delay(); delay(); delay();
  delay(); delay(); delay(); delay(); delay();
  delay(); delay(); delay();delay(); delay();
  delay(); delay(); delay(); delay(); delay();
  delay(); delay(); delay(); delay(); delay();
  delay(); delay(); delay();delay(); delay();
  delay(); delay(); delay(); delay(); delay();
  delay(); delay(); delay(); delay(); delay();
  delay(); delay(); delay();delay(); delay();
  delay(); delay(); delay(); delay(); delay();
  delay(); delay(); delay(); delay(); delay();
  delay(); delay(); delay();delay(); delay();
  delay(); delay(); delay(); delay(); delay();
  at45db_cs_low();
  U1TXBUF= AT45DB_B12MER_WR;
  delay();
  U1TXBUF= tmp1;  //page adr
  delay();
  U1TXBUF= tmp2;  //page adr
  delay();
  U1TXBUF= 0x55;
  delay();
  delay();
  delay();
  at45db_cs_hi();
  delay();
  delay();
  delay();
  delay();
  return 0;

}


INT8U at45db_rd (INT16U page_adr,INT16U in_page_adr,INT8U *buf,INT16U lenth )
{
  INT8U tmp1,tmp2;
  INT16U tmp_lenth;

  if (at45db_get_status ()!=0xac ) return 255;
  page_adr &=0xfff;
  page_adr <<=2;
  tmp1 = in_page_adr /256;
  page_adr |=tmp1;

  tmp1 =page_adr /256;
  tmp2 =page_adr %256;
  at45db_cs_low();
  U1TXBUF= AT45DB_PAGE_RD;  //command
  delay();
  U1TXBUF=tmp1;             //page adr
  delay();
  U1TXBUF=tmp2;             //page adr
  delay();
  tmp1= in_page_adr %256;
  U1TXBUF=tmp1;             // in page adr
  delay();

  U1TXBUF=0x55;             // don't care bit
  delay();
  U1TXBUF=0x55;             // don't care bit
  delay();
  U1TXBUF=0x55;             // don't care bit
  delay();
  U1TXBUF=0x55;             // don't care bit
  delay();

  for ( tmp_lenth=0;tmp_lenth< lenth;tmp_lenth++)
  {
    U1TXBUF=0x55;
    delay();
    *buf = U1RXBUF;
    buf++;
  }
  at45db_cs_hi();
   return 0;
}



INT8U at45db_rd_page (INT16U page_adr,INT8U *buf )
{
  INT8U tmp1,tmp2;
  INT16U tmp_lenth;

  if (at45db_get_status ()!=0xac ) return 255;
  page_adr &=0xfff;
  page_adr <<=2;
  tmp1 =page_adr /256;
  tmp2 =page_adr %256;
  at45db_cs_low();
  U1TXBUF= AT45DB_PAGE_RD;  //command
  delay();
  U1TXBUF=tmp1;             //page adr
  delay();
  U1TXBUF=tmp2;             //page adr
  delay();
  U1TXBUF=0x00;             // in page adr
  delay();

  U1TXBUF=0x55;             // don't care bit
  delay();
  U1TXBUF=0x55;             // don't care bit
  delay();
  U1TXBUF=0x55;             // don't care bit
  delay();
  U1TXBUF=0x55;             // don't care bit
  delay();

  for ( tmp_lenth=0;tmp_lenth<512;tmp_lenth++)
  {
    U1TXBUF=0x55;
    delay();
    *buf = U1RXBUF;
    buf++;
  }
  at45db_cs_hi();
   return 0;
}

void at45db_test ( void)
{
  INT16U  page;
  page =0;

  for ( page =0;page <512;page ++)
  {
    at45db_buf[page]= page;
  }
  page =0;
  at45db_wr_page (  page, at45db_buf);
  for ( page =0;page <30000;page ++) _NOP();

  page =0;

  while (1)
  {
    at45db_rd_page (page ,at45db_buf);
    page ++;
  }

}


