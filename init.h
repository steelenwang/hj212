#ifndef INIT_00
#define INIT_00 1
void io_init ( void);
void clk_init ( void);
void adc_init ( void );
void lcd_init (void );
void i2c_init (void);

void ds1302_rtc_init(void );
void tmr_b_init( void);
void tmr_a_init( void);
void adc_init ( void );
void sio1_init( void);
void spi_init( void );
void enable_int( void );
void rtc_tmr_init (void);
void rtc_init ( void);
void sc752_init(void );
void at45db_init ( void );
void  shanxi_hb_init( void);
#else
#endif
