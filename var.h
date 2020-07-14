
#include "types.h"
#include "const.h"

#ifndef VAR_H00
#define VAR_H00  1



extern INT16U ad_dc1_buf[POINT];
extern INT16U ad_dc2_buf[POINT];
extern INT16U ad_dc3_buf[POINT];
extern INT16U ad_dc4_buf[POINT];
extern INT16U ad_dc5_buf[POINT];
extern INT16U ad_dc6_buf[POINT];
extern INT16U ad_dc7_buf[POINT];
extern INT16U ad_dc8_buf[POINT];

extern INT8U  ad_cnt,ad_ctl;
extern INT16U tempreture;
//----------di-----------
extern INT8U   di[MAXDI/8],di_old,di_old2,di_old3;
extern INT16U  count[REAL_MAXDI],tmp_count[REAL_MAXDI];
extern INT8U   soetmp_di_cnt1[REAL_MAXDI];
extern INT8U   soetmp_di_cnt2[REAL_MAXDI];
extern INT16U  soetmp_timeH[REAL_MAXDI];
extern INT16U  soetmp_timeL[REAL_MAXDI];
// dir var end

//do var
extern INT8U   do_out[MAXDO/8];
extern INT8U   do_tmr[REAL_MAXDO];

// Sio1 Var
extern INT8U   sio1_buf[MAX_SIO_BUF],sio1_txbuf[MAX_SIO_BUF];
extern INT8U fr1_ready,sio1_flag;
extern INT16U  sio1_rx_lenth,sio1_tx_lenth,sio1_tx_lenth2;
extern INT16U sio1_rx_tmr,sio1_tx_tmr;


//sio1 Var end


extern INT8U  sio_int_flag;
extern INT8U   sio1_set;
//----------
extern INT8U  sio3_buf[MAX_SIO_BUF];
extern INT8U  fr3_ready,sio3_flag;
extern INT16U sio3_rx_lenth,sio3_tx_lenth,sio3_tx_lenth2;
extern INT16U sio3_rx_tmr,sio3_tx_tmr;
//sio3 Var end
//sio4 Var
extern INT8U  sio4_buf[MAX_SIO_BUF];
extern INT8U  fr4_ready,sio4_flag;
extern INT16U sio4_rx_lenth,sio4_tx_lenth,sio4_tx_lenth2;
extern INT16U sio4_rx_tmr,sio4_tx_tmr;
//sio4 Var end

//sio5 Var
extern INT8U  sio5_buf[MAX_SIO_BUF];
extern INT8U  fr5_ready,sio5_flag;
extern INT16U sio5_rx_lenth,sio5_tx_lenth,sio5_tx_lenth2;
extern INT16U sio5_rx_tmr,sio5_tx_tmr;
//sio5 Var end
//sio6 Var
extern INT8U  sio6_buf[MAX_SIO_BUF];
extern INT8U  fr6_ready,sio6_flag;
extern INT16U sio6_rx_lenth,sio6_tx_lenth,sio6_tx_lenth2;
extern INT16U sio6_rx_tmr,sio6_tx_tmr;

//sio6 Var end


//----------
extern INT16S   data    [ MAXDATA];
extern INT16U   hld_reg [MAXHLD];
extern FP32     float_data [MAX_FLOAT];
extern FP32     float_avr_data[MAX_FLOAT];
extern FP32     avr_data[MAX_FLOAT];
extern INT32U   avr_cnt;


//soe
extern SOE soe_buf[MAXFIFO];
extern SOE *soe_write_p, *soe_read_p;

extern INT8U  ms10_tmr,protocol_tmr;

extern INT8U  at45db_buf[529];
extern INT16U at_buf_point;
extern INT16U store_tmr;
void var_init (void );

#else
#endif

