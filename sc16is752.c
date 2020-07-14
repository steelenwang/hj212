#include    "cpu.h"
#include    "types.h"
#include    "const.h"
#include    "var.h"

#define SC16752_CS1_LOW  P9OUT&=~BIT1
#define SC16752_CS1_HI   P9OUT|=BIT1
#define SC16752_RST1_LOW  P9OUT&=~BIT0
#define SC16752_RST1_HI   P9OUT|=BIT0

#define SC16752_CS2_LOW  P7OUT&=~BIT7
#define SC16752_CS2_HI   P7OUT|=BIT7
#define SC16752_RST2_LOW  P7OUT&=~BIT6
#define SC16752_RST2_HI   P7OUT|=BIT6

#define RHR     0
#define THR     0
#define IER     1
#define FCR     2
#define IIR     2
#define LCR     3
#define MCR     4
#define LSR     5
#define MSR     6
#define SPR     7
#define TCR     6
#define TLR     7
#define TXLVL   8
#define RXLVL   9
#define IODIR   10
#define IOSTATE 11
#define IOCTL   14
#define EFCR    15

#define DLL     0
#define DLM     1
#define EFR     2

#define SCB300    384
#define SCB600    192
#define SCB1200   96
#define SCB2400   48
#define SCB4800   24
#define SCB9600   12
#define SCB19200  6
#define SCB38400  3
#define SCB57600  2
#define SCB76800  1
#define SCB115200 1

INT8U flush_buf;

const INT16U BR_TAB[16]=
{
  SCB9600,SCB300,SCB600,SCB1200 ,SCB2400,SCB4800,SCB19200,SCB38400,
  SCB57600,SCB76800,SCB115200,SCB9600,SCB9600,SCB9600,SCB9600,SCB9600
};

void delay_ ( void )
{
     _NOP();_NOP();_NOP();_NOP();_NOP();
     _NOP();_NOP();_NOP();_NOP();_NOP();
     _NOP();_NOP();_NOP();_NOP();_NOP();
     _NOP();_NOP();_NOP();_NOP();_NOP();

}
void sc16752_rst ( void )
{
  INT8U tmp;

  SC16752_RST1_LOW;
  SC16752_RST2_LOW;
  for ( tmp=0;tmp<125;tmp ++) _NOP();
  SC16752_RST1_HI;
  SC16752_RST2_HI;
}

void spi_wr_752 (INT8U reg, INT8U DataByte, INT8U channel) // mcu writes to SC16IS752
{
    reg <<= 3; // register address byte

    switch ( channel )
    {
     case 0:
     SC16752_CS1_LOW;
      _NOP();  _NOP();  _NOP();  _NOP();
      U1TXBUF=reg;
      delay_();
      U1TXBUF=DataByte;
      delay_();
      SC16752_CS1_HI;
      _NOP();  _NOP();  _NOP();  _NOP();
     break;

     case 1:
       reg |=0x2;
       SC16752_CS1_LOW;
      _NOP();  _NOP();  _NOP();  _NOP();
      U1TXBUF=reg;
      delay_();
      U1TXBUF=DataByte;
      delay_();
      SC16752_CS1_HI;
      _NOP();  _NOP();  _NOP();  _NOP();  _NOP();
    break;

    case 2:
       SC16752_CS2_LOW;
      _NOP();  _NOP();  _NOP();  _NOP();
      U1TXBUF=reg;
      delay_();
      U1TXBUF=DataByte;
      delay_();
      SC16752_CS2_HI;
      _NOP();  _NOP();  _NOP();  _NOP();
    break;

    case 3:
      reg |=0x2;
      SC16752_CS2_LOW;
      _NOP();  _NOP();  _NOP();  _NOP();
      U1TXBUF=reg;
      delay_();
      U1TXBUF=DataByte;
      delay_();
      SC16752_CS2_HI;
      _NOP();  _NOP();  _NOP();  _NOP();
    break;



    }

}

INT8U spi_rd_752 (INT8U reg, INT8U  channel) // mcu read from SC16IS752
{
    reg = (reg<<3) | 0x80;

    switch ( channel )
    {
     case 0:
     SC16752_CS1_LOW;
      _NOP();  _NOP();  _NOP();  _NOP();
      U1TXBUF=reg;
      delay_();
      U1TXBUF=0x55;
      delay_();
      SC16752_CS1_HI;
      _NOP();  _NOP();  _NOP();  _NOP();
     break;

     case 1:
       reg |=0x2;
       SC16752_CS1_LOW;
      _NOP();  _NOP();  _NOP();  _NOP();
      U1TXBUF=reg;
       delay_();
      U1TXBUF=0x55;
      delay_();
      SC16752_CS1_HI;
      _NOP();  _NOP();  _NOP();
    break;

    case 2:
       SC16752_CS2_LOW;
      _NOP();  _NOP();  _NOP();  _NOP();
      U1TXBUF=reg;
      delay_();
      U1TXBUF=0x55;
      delay_();
      SC16752_CS2_HI;
      _NOP();  _NOP();  _NOP();  _NOP();
    break;

    case 3:
      reg |=0x2;
      SC16752_CS2_LOW;
      _NOP();  _NOP();  _NOP();  _NOP();
      U1TXBUF=reg;
     delay_();
      U1TXBUF=0x55;
      delay_();
      SC16752_CS2_HI;
      _NOP();  _NOP();  _NOP();
    break;

    }

  return U1RXBUF;
}

void sio3_init ( void)
{
INT8U tmp;
  INT16U tmp_rate;

spi_wr_752 (LCR, 0x80, 0); // 0x80 to program baud rate
tmp =hld_reg[SIO3_SET] &0xf;   // init baud rate
tmp_rate = BR_TAB[tmp];
tmp =LOW(tmp_rate);
spi_wr_752 (DLL, tmp,0);
tmp =HI(tmp_rate);
spi_wr_752 (DLM, tmp, 0);
spi_wr_752 (LCR, 0xBF, 0); // access EFR register
spi_wr_752 (EFR, 0X10, 0); // enable enhanced registers
tmp_rate=hld_reg[SIO3_SET]& 0xf000;

if ( tmp_rate ==0xd000 ) spi_wr_752 (LCR, 0x1b, 0);
else if ( tmp_rate ==0xe000) spi_wr_752 (LCR, 0x0b, 0);
else spi_wr_752 (LCR, 0x03, 0); // 8 data bit, 1 stop bit, no parity

spi_wr_752 (FCR, 0x37, 0); // enable FIFO mode

spi_wr_752 (SPR, 'A', 0); // scratch pad = character A (0x41)
spi_wr_752 (IODIR, 0xFF, 0); // set GPIO [7:0] to output (default: 0x00=input)
spi_wr_752 (IOSTATE, 0x00, 0); // set GPIO [7:0] to 0x00 (LEDs on)
spi_wr_752 (IER, 0x01, 0); // enable Rx data ready interrupt
spi_wr_752 (EFCR, 0x10, 0);  // auto rs485

}

void sio4_init( void)
{
INT8U tmp;
  INT16U tmp_rate;
spi_wr_752 (LCR, 0x80, 1); // 0x80 to program baud rate
tmp =hld_reg[SIO4_SET] &0xf;   // init baud rate
tmp_rate = BR_TAB[tmp];
tmp =LOW(tmp_rate);
spi_wr_752 (DLL, tmp,1);
tmp =HI(tmp_rate);
spi_wr_752 (DLM, tmp, 1);
spi_wr_752 (LCR, 0xBF, 1); // access EFR register
spi_wr_752 (EFR, 0X10, 1); // enable enhanced registers
tmp_rate=hld_reg[SIO4_SET]& 0xf000;

if ( tmp_rate ==0xd000 ) spi_wr_752 (LCR, 0x1b, 1);
else if ( tmp_rate ==0xe000) spi_wr_752 (LCR, 0x0b, 1);
else spi_wr_752 (LCR, 0x03, 1); // 8 data bit, 1 stop bit, no parity

spi_wr_752 (FCR, 0x37, 1); // enable FIFO mode

spi_wr_752 (SPR, 'B', 1); // scratch pad = character A (0x41)
spi_wr_752 (IODIR, 0xFF, 1); // set GPIO [7:0] to output (default: 0x00=input)
spi_wr_752 (IOSTATE, 0x00, 1); // set GPIO [7:0] to 0x00 (LEDs on)
spi_wr_752 (IER, 0x01, 1); // enable Rx data ready interrupt
spi_wr_752 (EFCR, 0x10, 1);  // auto rs485


}

void sio5_init ( void)
{
  INT8U tmp;
  INT16U tmp_rate;
  spi_wr_752 (LCR, 0x80, 2); // 0x80 to program baud rate
tmp =hld_reg[SIO5_SET] &0xf;   // init baud rate
tmp_rate = BR_TAB[tmp];
tmp =LOW(tmp_rate);
spi_wr_752 (DLL, tmp,2);
tmp =HI(tmp_rate);
spi_wr_752 (DLM, tmp, 2);
spi_wr_752 (LCR, 0xBF, 2); // access EFR register
spi_wr_752 (EFR, 0X10, 2); // enable enhanced registers
tmp_rate=hld_reg[SIO5_SET]& 0xf000;

if ( tmp_rate ==0xd000 ) spi_wr_752 (LCR, 0x1b, 2);
else if ( tmp_rate ==0xe000) spi_wr_752 (LCR, 0x0b, 2);
else spi_wr_752 (LCR, 0x03, 2); // 8 data bit, 1 stop bit, no parity

spi_wr_752 (FCR, 0x37, 2); // enable FIFO mode

spi_wr_752 (SPR, 'C', 2); // scratch pad = character A (0x41)
spi_wr_752 (IODIR, 0xFF, 2); // set GPIO [7:0] to output (default: 0x00=input)
spi_wr_752 (IOSTATE, 0x00, 2); // set GPIO [7:0] to 0x00 (LEDs on)
spi_wr_752 (IER, 0x01, 2); // enable Rx data ready interrupt
spi_wr_752 (EFCR, 0x10, 2);  // auto rs485

}


void sio6_init ( void )
{
  INT8U tmp;
  INT16U tmp_rate;
spi_wr_752 (LCR, 0x80, 3); // 0x80 to program baud rate
tmp =hld_reg[SIO6_SET] &0xf;   // init baud rate
tmp_rate = BR_TAB[tmp];
tmp =LOW(tmp_rate);
spi_wr_752 (DLL, tmp,3);
tmp =HI(tmp_rate);
spi_wr_752 (DLM, tmp, 3);
spi_wr_752 (LCR, 0xBF, 3); // access EFR register
spi_wr_752 (EFR, 0X10, 3); // enable enhanced registers
tmp_rate=hld_reg[SIO6_SET]& 0xf000;

if ( tmp_rate ==0xd000 ) spi_wr_752 (LCR, 0x1b, 3);
else if ( tmp_rate ==0xe000) spi_wr_752 (LCR, 0x0b, 3);
else spi_wr_752 (LCR, 0x03, 3); // 8 data bit, 1 stop bit, no parity

spi_wr_752 (FCR, 0x37, 3); // enable FIFO mode

spi_wr_752 (SPR, 'D', 3); // scratch pad = character A (0x41)
spi_wr_752 (IODIR, 0xFF, 3); // set GPIO [7:0] to output (default: 0x00=input)
spi_wr_752 (IOSTATE, 0x00, 3); // set GPIO [7:0] to 0x00 (LEDs on)
spi_wr_752 (IER, 0x01, 3); // enable Rx data ready interrupt
spi_wr_752 (EFCR, 0x10, 3);  // auto rs485
}

void sc752_init(void )
{
//channel 0
  INT8U tmp;
  INT16U tmp_rate;

sc16752_rst ();


spi_wr_752 (LCR, 0x80, 0); // 0x80 to program baud rate
tmp =hld_reg[SIO3_SET] &0xf;   // init baud rate
tmp_rate = BR_TAB[tmp];
tmp =LOW(tmp_rate);
spi_wr_752 (DLL, tmp,0);
tmp =HI(tmp_rate);
spi_wr_752 (DLM, tmp, 0);
spi_wr_752 (LCR, 0xBF, 0); // access EFR register
spi_wr_752 (EFR, 0X10, 0); // enable enhanced registers
tmp_rate=hld_reg[SIO3_SET]& 0xf000;

if ( tmp_rate ==0xd000 ) spi_wr_752 (LCR, 0x1b, 0);
else if ( tmp_rate ==0xe000) spi_wr_752 (LCR, 0x0b, 0);
else spi_wr_752 (LCR, 0x03, 0); // 8 data bit, 1 stop bit, no parity

spi_wr_752 (FCR, 0x37, 0); // enable FIFO mode

spi_wr_752 (SPR, 'A', 0); // scratch pad = character A (0x41)
spi_wr_752 (IODIR, 0xFF, 0); // set GPIO [7:0] to output (default: 0x00=input)
spi_wr_752 (IOSTATE, 0x00, 0); // set GPIO [7:0] to 0x00 (LEDs on)
spi_wr_752 (IER, 0x01, 0); // enable Rx data ready interrupt
spi_wr_752 (EFCR, 0x10, 0);  // auto rs485

//channel 1

spi_wr_752 (LCR, 0x80, 1); // 0x80 to program baud rate
tmp =hld_reg[SIO4_SET] &0xf;   // init baud rate
tmp_rate = BR_TAB[tmp];
tmp =LOW(tmp_rate);
spi_wr_752 (DLL, tmp,1);
tmp =HI(tmp_rate);
spi_wr_752 (DLM, tmp, 1);
spi_wr_752 (LCR, 0xBF, 1); // access EFR register
spi_wr_752 (EFR, 0X10, 1); // enable enhanced registers
tmp_rate=hld_reg[SIO4_SET]& 0xf000;

if ( tmp_rate ==0xd000 ) spi_wr_752 (LCR, 0x1b, 1);
else if ( tmp_rate ==0xe000) spi_wr_752 (LCR, 0x0b, 1);
else spi_wr_752 (LCR, 0x03, 1); // 8 data bit, 1 stop bit, no parity

spi_wr_752 (FCR, 0x37, 1); // enable FIFO mode

spi_wr_752 (SPR, 'B', 1); // scratch pad = character A (0x41)
spi_wr_752 (IODIR, 0xFF, 1); // set GPIO [7:0] to output (default: 0x00=input)
spi_wr_752 (IOSTATE, 0x00, 1); // set GPIO [7:0] to 0x00 (LEDs on)
spi_wr_752 (IER, 0x01, 1); // enable Rx data ready interrupt
spi_wr_752 (EFCR, 0x10, 1);  // auto rs485


//channel 2

spi_wr_752 (LCR, 0x80, 2); // 0x80 to program baud rate
tmp =hld_reg[SIO5_SET] &0xf;   // init baud rate
tmp_rate = BR_TAB[tmp];
tmp =LOW(tmp_rate);
spi_wr_752 (DLL, tmp,2);
tmp =HI(tmp_rate);
spi_wr_752 (DLM, tmp, 2);
spi_wr_752 (LCR, 0xBF, 2); // access EFR register
spi_wr_752 (EFR, 0X10, 2); // enable enhanced registers
tmp_rate=hld_reg[SIO5_SET]& 0xf000;

if ( tmp_rate ==0xd000 ) spi_wr_752 (LCR, 0x1b, 2);
else if ( tmp_rate ==0xe000) spi_wr_752 (LCR, 0x0b, 2);
else spi_wr_752 (LCR, 0x03, 2); // 8 data bit, 1 stop bit, no parity

spi_wr_752 (FCR, 0x37, 2); // enable FIFO mode

spi_wr_752 (SPR, 'C', 2); // scratch pad = character A (0x41)
spi_wr_752 (IODIR, 0xFF, 2); // set GPIO [7:0] to output (default: 0x00=input)
spi_wr_752 (IOSTATE, 0x00, 2); // set GPIO [7:0] to 0x00 (LEDs on)
spi_wr_752 (IER, 0x01, 2); // enable Rx data ready interrupt
spi_wr_752 (EFCR, 0x10, 2);  // auto rs485

//channel 3

spi_wr_752 (LCR, 0x80, 3); // 0x80 to program baud rate
tmp =hld_reg[SIO6_SET] &0xf;   // init baud rate
tmp_rate = BR_TAB[tmp];
tmp =LOW(tmp_rate);
spi_wr_752 (DLL, tmp,3);
tmp =HI(tmp_rate);
spi_wr_752 (DLM, tmp, 3);
spi_wr_752 (LCR, 0xBF, 3); // access EFR register
spi_wr_752 (EFR, 0X10, 3); // enable enhanced registers
tmp_rate=hld_reg[SIO6_SET]& 0xf000;

if ( tmp_rate ==0xd000 ) spi_wr_752 (LCR, 0x1b, 3);
else if ( tmp_rate ==0xe000) spi_wr_752 (LCR, 0x0b, 3);
else spi_wr_752 (LCR, 0x03, 3); // 8 data bit, 1 stop bit, no parity

spi_wr_752 (FCR, 0x37, 3); // enable FIFO mode

spi_wr_752 (SPR, 'D', 3); // scratch pad = character A (0x41)
spi_wr_752 (IODIR, 0xFF, 3); // set GPIO [7:0] to output (default: 0x00=input)
spi_wr_752 (IOSTATE, 0x00, 3); // set GPIO [7:0] to 0x00 (LEDs on)
spi_wr_752 (IER, 0x01, 3); // enable Rx data ready interrupt
spi_wr_752 (EFCR, 0x10, 3);  // auto rs485

}

INT8U read_sio_status ( INT8U channel )
{
   return spi_rd_752 (IIR, channel);
}
INT8U read_sio (INT8U channel )
{
  return spi_rd_752 (0, channel);
}

void write_sio (INT8U data,INT8U channel)
{
  spi_wr_752 (0, data,  channel);
}
INT8U read_sio_rxfifo ( INT8U channel )
{
   return spi_rd_752 (RXLVL, channel);
}

INT8U read_sio_txfifo ( INT8U channel )
{
   return spi_rd_752 (TXLVL, channel);
}

INT8U read_sio_lsr ( INT8U channel )
{
   return spi_rd_752 (LSR, channel);
}

INT8U read_io ( INT8U channel )
{
   return spi_rd_752 (IOSTATE, channel);
}

void write_io ( INT8U data,INT8U channel)
{
   spi_wr_752 (IOSTATE, data,  channel);
}

void sio_proc(void )
{
  INT8U tmp;

  // chip1 rx proc
   if ( sio_int_flag &BIT7 )
    {
      sio_int_flag &=~BIT7;
      // sio3  rx proc
      tmp=read_sio_status(0);
      tmp &=0x3f;
      switch ( tmp )
      {
       case 0:    //MODEM中断

       break;

       case 2:     //发送中断

       break;

       case 4:   //接收中断
        tmp = read_io (0);  // rx _led control
        tmp &=~BIT4;
        write_io( tmp,0);

        tmp = read_sio_lsr(0);
        if ( tmp & BIT7 )    //接收错误
        {
          tmp = read_sio_rxfifo(0);
          for ( ;tmp!=0;tmp-- )
          {
            flush_buf= read_sio(0);  //清空接收缓冲
            flush_buf ++;
          }
        }
        else                //接收正确
        {
         tmp = read_sio_rxfifo(0);
         for ( ;tmp!=0;tmp-- )
          {
          sio3_buf[sio3_rx_lenth]=read_sio(0);
          sio3_rx_lenth ++;
          sio3_rx_lenth &=0x1ff;
          }
        }
      break;

      case 6:   //接收线路中断

      break;

      case 12:  //接收超时中断
        fr3_ready =1;
        tmp = read_io (0);  // rx _led control
        tmp &=~BIT4;
        write_io( tmp,0);

        tmp = read_sio_lsr(0);
        if ( tmp & BIT7 )    //接收错误
        {
          tmp = read_sio_rxfifo(0);
          for ( ;tmp!=0;tmp-- )
          {
            flush_buf= read_sio(0);  //清空接收缓冲
            flush_buf ++;
          }
        }
        else                //接收正确
        {
         tmp = read_sio_rxfifo(0);
         for ( ;tmp!=0;tmp-- )
          {
          sio3_buf[sio3_rx_lenth]=read_sio(0);
          sio3_rx_lenth ++;
          sio3_rx_lenth &=0x1ff;
          }
        }
      break;

      case 0x10:  //接收到XOFF字符

      break;

      case 0x20:   //CTS RTS 有效

      break;

      case 0x30:  //输入脚状态改变

      break;

      default:

      break;


      }

      //   sio4  proc
      tmp=read_sio_status(1);
      tmp &=0x3f;
      switch ( tmp )
      {
       case 0:    //MODEM中断

       break;

       case 2:     //发送中断

       break;

       case 4:   //接收中断
        tmp = read_io (0);  // rx _led control
        tmp &=~BIT6;
        write_io( tmp,0);

        tmp = read_sio_lsr(1);
        if ( tmp & BIT7 )    //接收错误
        {
          tmp = read_sio_rxfifo(1);
          for ( ;tmp!=0;tmp-- )
          {
            flush_buf= read_sio(1);  //清空接收缓冲
            flush_buf ++;
          }
        }
        else                //接收正确
        {
         tmp = read_sio_rxfifo(1);
         for ( ;tmp!=0;tmp-- )
          {
          sio4_buf[sio4_rx_lenth]=read_sio(1);
          sio4_rx_lenth ++;
          sio4_rx_lenth &=0x1ff;
          }
        }
      break;

      case 6:   //接收线路中断

      break;

      case 12:  //接收超时中断
        fr4_ready =1;
        tmp = read_io (0);  // rx _led control
        tmp &=~BIT6;
        write_io( tmp,0);

        tmp = read_sio_lsr(0);
        if ( tmp & BIT7 )    //接收错误
        {
          tmp = read_sio_rxfifo(1);
          for ( ;tmp!=0;tmp-- )
          {
            flush_buf= read_sio(1);  //清空接收缓冲
            flush_buf ++;
          }
        }
        else                //接收正确
        {
         tmp = read_sio_rxfifo(1);
         for ( ;tmp!=0;tmp-- )
          {
          sio4_buf[sio4_rx_lenth]=read_sio(1);
          sio4_rx_lenth ++;
          sio4_rx_lenth &=0x1ff;
          }
        }
      break;

      case 0x10:  //接收到XOFF字符

      break;

      case 0x20:   //CTS RTS 有效

      break;

      case 0x30:  //输入脚状态改变

      break;

      default:

      break;


      }


    }

   else
    {
        tmp = read_io (0);  // rx _led control
        tmp |=(BIT4|BIT6);
        write_io( tmp,0);
    }




   // chip 2 rx proc
   if ( sio_int_flag &BIT6 )
    {
      sio_int_flag &=~BIT6;
      // sio5  rx proc
      tmp=read_sio_status(2);
      tmp &=0x3f;
      switch ( tmp )
      {
       case 0:    //MODEM中断

       break;

       case 2:     //发送中断

       break;

       case 4:   //接收中断
        tmp = read_io (2);  // rx _led control
        tmp &=~BIT4;
        write_io( tmp,2);

        tmp = read_sio_lsr(2);
        if ( tmp & BIT7 )    //接收错误
        {
          tmp = read_sio_rxfifo(2);
          for ( ;tmp!=0;tmp-- )
          {
            flush_buf= read_sio(2);  //清空接收缓冲
            flush_buf ++;
          }
        }
        else                //接收正确
        {
         tmp = read_sio_rxfifo(2);
         for ( ;tmp!=0;tmp-- )
          {
          sio5_buf[sio5_rx_lenth]=read_sio(2);
          sio5_rx_lenth ++;
          sio5_rx_lenth &=0x1ff;
          }
        }
      break;

      case 6:   //接收线路中断

      break;

      case 12:  //接收超时中断
        fr5_ready =1;
        tmp = read_io (2);  // rx _led control
        tmp &=~BIT4;
        write_io( tmp,2);

        tmp = read_sio_lsr(2);
        if ( tmp & BIT7 )    //接收错误
        {
          tmp = read_sio_rxfifo(2);
          for ( ;tmp!=0;tmp-- )
          {
            flush_buf= read_sio(2);  //清空接收缓冲
            flush_buf ++;
          }
        }
        else                //接收正确
        {
         tmp = read_sio_rxfifo(2);
         for ( ;tmp!=0;tmp-- )
          {
          sio5_buf[sio5_rx_lenth]=read_sio(2);
          sio5_rx_lenth ++;
          sio5_rx_lenth &=0x1ff;
          }
        }
      break;

      case 0x10:  //接收到XOFF字符

      break;

      case 0x20:   //CTS RTS 有效

      break;

      case 0x30:  //输入脚状态改变

      break;

      default:

      break;


      }

      //   sio6  proc
      tmp=read_sio_status(3);
      tmp &=0x3f;
      switch ( tmp )
      {
       case 0:    //MODEM中断

       break;

       case 2:     //发送中断

       break;

       case 4:   //接收中断
        tmp = read_io (2);  // rx _led control
        tmp &=~BIT6;
        write_io( tmp,2);

        tmp = read_sio_lsr(3);
        if ( tmp & BIT7 )    //接收错误
        {
          tmp = read_sio_rxfifo(3);
          for ( ;tmp!=0;tmp-- )
          {
            flush_buf= read_sio(3);  //清空接收缓冲
            flush_buf ++;
          }
        }
        else                //接收正确
        {
         tmp = read_sio_rxfifo(3);
         for ( ;tmp!=0;tmp-- )
          {
          sio6_buf[sio6_rx_lenth]=read_sio(3);
          sio6_rx_lenth ++;
          sio6_rx_lenth &=0x1ff;
          }
        }
      break;

      case 6:   //接收线路中断

      break;

      case 12:  //接收超时中断
        fr6_ready =1;
        tmp = read_io (2);  // rx _led control
        tmp &=~BIT6;
        write_io( tmp,2);

        tmp = read_sio_lsr(3);
        if ( tmp & BIT7 )    //接收错误
        {
          tmp = read_sio_rxfifo(3);
          for ( ;tmp!=0;tmp-- )
          {
            flush_buf= read_sio(3);  //清空接收缓冲
            flush_buf ++;
          }
        }
        else                //接收正确
        {
         tmp = read_sio_rxfifo(3);
         for ( ;tmp!=0;tmp-- )
          {
          sio6_buf[sio6_rx_lenth]=read_sio(3);
          sio6_rx_lenth ++;
          sio6_rx_lenth &=0x1ff;
          }
        }
      break;

      case 0x10:  //接收到XOFF字符

      break;

      case 0x20:   //CTS RTS 有效

      break;

      case 0x30:  //输入脚状态改变

      break;

      default:

      break;


      }


    }

   else
    {
        tmp = read_io (2);  // rx _led control
        tmp |=(BIT4|BIT6);
        write_io( tmp,2);
    }




   // sio3 tx proc
   if ( sio3_tx_lenth < sio3_tx_lenth2 )
    {
        tmp = read_io (0);
        tmp &=~BIT5;
        write_io( tmp,0);
        for (   ;sio3_tx_lenth < sio3_tx_lenth2; )
        {
            tmp =read_sio_txfifo(0);
            if ( tmp <4 ) break;
            write_sio( sio3_buf[sio3_tx_lenth],0);
            sio3_tx_lenth ++;
        }
    }
   else
   {
        tmp = read_io (0);
        tmp |=BIT5;
        write_io( tmp,0);
   }
   // sio4 tx proc
   if ( sio4_tx_lenth < sio4_tx_lenth2 )
    {
        tmp = read_io (0);
        tmp &=~BIT7;
        write_io( tmp,0);

        for (   ;sio4_tx_lenth < sio4_tx_lenth2; )
        {
            tmp =read_sio_txfifo(1);
            if ( tmp <4 ) break;
            write_sio( sio4_buf[sio4_tx_lenth],1);
            sio4_tx_lenth ++;
        }
    }
   else
   {
        tmp = read_io (0);
        tmp |=BIT7;
        write_io( tmp,0);
   }

    // sio5 tx proc
   if ( sio5_tx_lenth < sio5_tx_lenth2 )
    {
        tmp = read_io (2);
        tmp &=~BIT5;
        write_io( tmp,2);
        for (   ;sio5_tx_lenth < sio5_tx_lenth2; )
        {
            tmp =read_sio_txfifo(2);
            if ( tmp <4 ) break;
            write_sio( sio5_buf[sio5_tx_lenth],2);
            sio5_tx_lenth ++;
        }
    }
   else
   {
        tmp = read_io (2);
        tmp |=BIT5;
        write_io( tmp,2);
   }

     // sio6 tx proc
   if ( sio6_tx_lenth < sio6_tx_lenth2 )
    {
        tmp = read_io (2);
        tmp &=~BIT7;
        write_io( tmp,2);
        for (   ;sio6_tx_lenth < sio6_tx_lenth2; )
        {
            tmp =read_sio_txfifo(3);
            if ( tmp <4 ) break;
            write_sio( sio6_buf[sio6_tx_lenth],3);
            sio6_tx_lenth ++;
        }
    }
   else
   {
        tmp = read_io (2);
        tmp |=BIT7;
        write_io( tmp,2);
   }

   if ( flush_buf ) _NOP();
   else flush_buf --;
}

