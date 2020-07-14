#include "types.h"
#include "const.h"
#include "sub.h"
//--------adc----------


INT16U ad_dc1_buf[POINT];
INT16U ad_dc2_buf[POINT];
INT16U ad_dc3_buf[POINT];
INT16U ad_dc4_buf[POINT];
INT16U ad_dc5_buf[POINT];
INT16U ad_dc6_buf[POINT];
INT16U ad_dc7_buf[POINT];
INT16U ad_dc8_buf[POINT];
INT16U tempreture;

INT8U  ad_cnt,ad_ctl;
//--------adc----------
//----------di-----------
INT8U   di[MAXDI/8],di_old,di_old2,di_old3;
INT16U  count[REAL_MAXDI],tmp_count[REAL_MAXDI];
INT8U   soetmp_di_cnt1[REAL_MAXDI];
INT8U   soetmp_di_cnt2[REAL_MAXDI];
INT16U  soetmp_timeH[REAL_MAXDI];
INT16U  soetmp_timeL[REAL_MAXDI];
// dir var end

//do var
INT8U   do_out[MAXDO/8];
INT8U   do_tmr[REAL_MAXDO];



// Sio1 Var
INT8U   sio1_buf[MAX_SIO_BUF],sio1_txbuf[MAX_SIO_BUF];

INT8U   fr1_ready,sio1_flag;
INT16U  sio1_rx_lenth,sio1_tx_lenth,sio1_tx_lenth2;
INT16U  sio1_rx_tmr,sio1_tx_tmr;
INT8U  sio_int_flag;
INT8U   sio1_set;

//sio1 Var end

//sio3 Var


//sio3 Var
INT8U  sio3_buf[MAX_SIO_BUF];
INT8U  fr3_ready,sio3_flag;
INT16U sio3_rx_lenth,sio3_tx_lenth,sio3_tx_lenth2;
INT16U sio3_rx_tmr,sio3_tx_tmr;
//sio3 Var end
//sio4 Var
INT8U  sio4_buf[MAX_SIO_BUF];
INT8U  fr4_ready,sio4_flag;
INT16U sio4_rx_lenth,sio4_tx_lenth,sio4_tx_lenth2;
INT16U sio4_rx_tmr,sio4_tx_tmr;
//sio4 Var end

//sio5 Var
INT8U  sio5_buf[MAX_SIO_BUF];
INT8U  fr5_ready,sio5_flag;
INT16U sio5_rx_lenth,sio5_tx_lenth,sio5_tx_lenth2;
INT16U sio5_rx_tmr,sio5_tx_tmr;
//sio5 Var end
//sio6 Var
INT8U  sio6_buf[MAX_SIO_BUF];
INT8U  fr6_ready,sio6_flag;
INT16U sio6_rx_lenth,sio6_tx_lenth,sio6_tx_lenth2;
INT16U sio6_rx_tmr,sio6_tx_tmr;

//sio6 Var end


INT16S   data    [MAXDATA];

FP32     float_data [MAX_FLOAT];
FP32     float_avr_data[MAX_FLOAT];
FP32     avr_data[MAX_FLOAT];
INT32U   avr_cnt;

INT16U   hld_reg [MAXHLD];


// soe
SOE soe_buf[MAXFIFO];
SOE *soe_write_p, *soe_read_p;

INT8U  ms10_tmr,protocol_tmr;

INT8U  at45db_buf[529];
INT16U at_buf_point;
INT16U store_tmr;


INT16U crc(INT8U *puchMsg , INT16U usDataLen);
INT8U at45db_wr_page ( INT16U page_adr,INT8U *buf );
void dc_get_avr( void );

void var_init (void )
{
 INT16U tmp,tmp_cnt;
 INT8U  *tmp_point;

 tmp_point= (INT8U*) hld_reg;

 if ( hld_reg[HLD_REG_INIT1] !=0x55aa )
 {

   for ( tmp_cnt=0; tmp_cnt<sizeof (hld_reg);tmp_cnt++)
            {
               tmp=i2c_read_byte(tmp_cnt);
               if ( tmp & 0xff00)  while (1);
               else  *tmp_point++ = tmp &0xff;
            }

   if ( hld_reg[HLD_REG_INIT1] !=0x55aa )
   {
     for ( tmp =0; tmp <256; tmp ++) hld_reg[tmp]= 0;

    hld_reg[DC1_XH]=4096;
    hld_reg[DC2_XH]=4096;
    hld_reg[DC3_XH]=4096;
    hld_reg[DC4_XH]=4096;
    hld_reg[DC5_XH]=4096;
    hld_reg[DC6_XH]=4096;
    hld_reg[DC7_XH]=4096;
    hld_reg[DC8_XH]=4096;

    hld_reg[DC1_MIN]=0;
    hld_reg[DC2_MIN]=0;
    hld_reg[DC3_MIN]=0;
    hld_reg[DC4_MIN]=0;
    hld_reg[DC5_MIN]=0;
    hld_reg[DC6_MIN]=0;
    hld_reg[DC7_MIN]=0;
    hld_reg[DC8_MIN]=0;

    hld_reg[DC1_MAX]=4096;
    hld_reg[DC2_MAX]=4096;
    hld_reg[DC3_MAX]=4096;
    hld_reg[DC4_MAX]=4096;
    hld_reg[DC5_MAX]=4096;
    hld_reg[DC6_MAX]=4096;
    hld_reg[DC7_MAX]=4096;
    hld_reg[DC8_MAX]=4096;

    hld_reg[DC1_SCALE]=4096;
    hld_reg[DC2_SCALE]=4096;
    hld_reg[DC3_SCALE]=4096;
    hld_reg[DC4_SCALE]=4096;
    hld_reg[DC5_SCALE]=4096;
    hld_reg[DC6_SCALE]=4096;
    hld_reg[DC7_SCALE]=4096;
    hld_reg[DC8_SCALE]=4096;

    hld_reg[FREQ_GATE]=100;
    hld_reg[SIO1_SET]= 0x1000;
    hld_reg[SIO3_SET]= 0x1000;
    hld_reg[SIO4_SET]= 0x1000;
    hld_reg[SIO5_SET]= 0x1000;
    hld_reg[SIO6_SET]= 0x1000;

    hld_reg[SIO3_TMOUT]=5000;
    hld_reg[SIO4_TMOUT]=5000;
    hld_reg[SIO5_TMOUT]=5000;
    hld_reg[SIO6_TMOUT]=5000;

    hld_reg[SHANXI_STORE_TMR]= 600;
    hld_reg[SHANXI_ASET]=0xff;
    hld_reg[SHANXI_DSET]=0xff;
    hld_reg[SHANXI_KSET]=0xff;
    hld_reg[SHANXI_SEND_TMR]= 1;
    hld_reg[SHANXI_REAL_TMR]= 0;
    hld_reg[HLD_REG_INIT1]=0x55aa;
   }
 }
    at_buf_point=0;
}



