#include "cpu.h"
#include "types.h"

#include "init.h"
#include "proc.h"

#include "var.h"
#include "sub.h"



INT8U get_set(void );
void wr_24cl16(void)
{
 static  INT16U  i2c_tmp=0;
 static  INT8U  *hld_reg_tmp;
 INT16U   tmp;

 i2c_tmp &=0x1ff;
  if ( i2c_tmp ==0)
  {
    hld_reg_tmp=(INT8U *)hld_reg;
  }
  for ( tmp=0;tmp<256;tmp++ )
      i2c_write_byte( *hld_reg_tmp++, i2c_tmp++);


}
void run_led_on( void)
{
  P9OUT &=~BIT5;
}
void run_led_off ( void )
{
   P9OUT |= BIT5;
}

void run_led_flash( void)
{
  P9OUT ^= BIT5;
}

void kill_wd ( void )
{
   P5OUT ^=BIT0;
}

void self_test( void)
{
  if ( at45db_get_status ( )==0xff)  di[2] |=BIT1 ;
  else di[2] &=~BIT2 ;
}

void main ( void)
{
  INT16U tmp;
  INT8U  tmp_tmr1,tmp_tmr2,tmp_var;


  WDTCTL = WDTPW + WDTHOLD;
  for ( tmp=0;tmp<30000;tmp++) _NOP();
  WDTCTL = WDTPW + WDTHOLD;
  clk_init ( );
  io_init();
  kill_wd();
  i2c_init();
  var_init();
  tmr_b_init();
  tmr_a_init();
  run_led_flash();
  adc_init();
  sio1_init();
  spi_init();
  sc752_init();
  run_led_flash();
  //ds1302_rtc_init();
  rtc_tmr_init();
  rtc_init();
  at45db_init();
  shanxi_hb_init();
 // at45db_test();
  enable_int();

  ms10_tmr =0;
  protocol_tmr =0;

// car
  while (1)
  {
    dc_cal ();
    do_proc();
    di_proc();
    sio_proc ( );

    if ( ms10_tmr !=0 )
    {
         _DINT();
          tmp_var = ms10_tmr;
          ms10_tmr =0;
         _EINT();
          if ( tmp_tmr2 >tmp_var)
          {
            tmp_tmr2-=tmp_var;

          }
          else
          {
            run_led_flash();
            tmp_tmr2=50;

          }
          sio_proc ( );
          kill_wd();

          if ( tmp_tmr1 > tmp_var  )
          {
            tmp_tmr1-=tmp_var;
          }
          else
          {
                wr_24cl16 ();
                read_time_all();
                dc_cal_avr ( );
                self_test();
                tmp_tmr1=100;
          }

    }
    tmp =get_switch ();
    tmp &=0xe0;
    tmp >>=5;

    sio1_set = tmp;

     switch ( sio1_set )
     {
       case 0x00:     //MODBUS 规约
       if ( fr1_ready)
          {
           sio1_modbus_rtu();
           fr1_ready=0;
           sio1_rx_lenth=0;
          }
       break;

       case 0x01:  // 山西规约
       shanxi_tmr_proc();
       if ( fr1_ready )
       {
           sio1_shanxi_hb();
           fr1_ready=0;
           sio1_rx_lenth=0;
       }
       break;

       case 0x02:  //山东 吉林环保规约
        sio1_shandong_hb ();
       break;

       default:
        if ( fr1_ready)
          {
           sio1_modbus_rtu();
           fr1_ready=0;
           sio1_rx_lenth=0;
          }
        //剩余串口测试代码
       sio3_tx_lenth2 = sio3_rx_lenth;
       if ( sio3_tx_lenth > sio3_tx_lenth2 ) sio3_tx_lenth = sio3_tx_lenth2;

       sio4_tx_lenth2 = sio4_rx_lenth;
       if ( sio4_tx_lenth > sio4_tx_lenth2 ) sio4_tx_lenth = sio4_tx_lenth2;

       sio5_tx_lenth2 = sio5_rx_lenth;
       if ( sio5_tx_lenth > sio5_tx_lenth2 ) sio5_tx_lenth = sio5_tx_lenth2;

       sio6_tx_lenth2 = sio6_rx_lenth;
       if ( sio6_tx_lenth > sio6_tx_lenth2 ) sio6_tx_lenth = sio6_tx_lenth2;

       break;
     }







  }

}

