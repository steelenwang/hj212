#include    "cpu.h"
#include    "types.h"


#define   SDA_HIGH     P5OUT|=BIT4
#define   SDA_LOW      P5OUT&=~BIT4
#define   SDA_IN       P5DIR&=~BIT4
#define   SDA_OUT      P5DIR|=BIT4

#define   SCL_HIGH     P5OUT|=BIT3
#define   SCL_LOW      P5OUT&=~BIT3
#define   SCL_IN       P5DIR&=~BIT3
#define   SCL_OUT      P5DIR|=BIT3

#define   SDA_IN_DATA  P5IN&BIT4
#define   SCL_IN_DATA  P5IN&BIT3

#define   WP_HIGH      P5OUT|=BIT2
#define   WP_LOW       P5OUT&=~BIT2

#define   WP_IN        P5DIR &=~BIT2
#define   WP_OUT       P5DIR |=BIT2

void i2c_init (void)
{
        SDA_OUT;
        SCL_OUT;
i2c_rst:
        _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
        _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	SDA_HIGH;
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	SCL_HIGH;
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	SDA_IN;
	SCL_IN;
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
         if  (!(SDA_IN_DATA))
           {
           _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	  _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
           SCL_OUT;
          _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	  _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
          SCL_LOW;
          _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	  _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	  goto i2c_rst;
           }

        SDA_OUT;
	SCL_OUT;
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	SDA_LOW;
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	SCL_LOW;
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();

}

int  i2c_start( void)
{

        SDA_OUT;
        SCL_OUT;
        _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
        _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	SDA_HIGH;
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	SCL_HIGH;
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	SDA_IN;
	SCL_IN;
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	if (!(SDA_IN_DATA))   return -1;
	if (!(SCL_IN_DATA))   return -1;
	SDA_OUT;
	SCL_OUT;
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	SDA_LOW;
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	SCL_LOW;
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	return 0;
}

void i2c_stop( void)
{
        SDA_OUT;

        SDA_LOW;
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	SCL_HIGH;
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	SDA_HIGH;
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	
}

void i2c_nak( void)
{
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	SDA_HIGH;
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	SCL_HIGH;
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	SCL_LOW;
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	
}
void i2c_ack( void)
{       _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
        _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
        SDA_LOW;
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	SCL_HIGH;
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	SCL_LOW;
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
}

INT8U  i2c_shin ( void )
{
       INT8U  i2c_tmp;
       INT8U  ch;
       _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       ch=0;
       SDA_HIGH;
      _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
      _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       SDA_IN;
       _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       for (i2c_tmp=0;i2c_tmp<8;i2c_tmp++)
       {
       	
       	ch<<=1;
       	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       	SCL_HIGH;
       	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       	if (SDA_IN_DATA) ch |=1;
       	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       	SCL_LOW;
       	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();    	
       	
       }

       return ch;	
	
}


INT16U i2c_sout ( INT8U  ch)
{
	INT8U  i2c_tmp;

       _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       for (i2c_tmp=0;i2c_tmp<8;i2c_tmp++)
       {
        _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
        _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
        _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       	if ( ch & 0x80) SDA_HIGH;
       	else SDA_LOW;
       	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       	SCL_HIGH;
       	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
        SCL_LOW;
       	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();	
       	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
        ch <<=1;      	
       }
       SDA_LOW;
       _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       	_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       SDA_IN;
       _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();	
       _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       SCL_HIGH;
       _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();	
       _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();	
       _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();	
       _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       ch=SDA_IN_DATA;
       _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       SCL_LOW;
       _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
       SDA_OUT;
       return ( ch);

}


INT16U   i2c_read_current(INT16U  address)
{
	INT16U  ch;
	INT8U   i2ctmp;
 	if (i2c_start()!=0)
	    {
	     i2c_stop();
	     return 0x0100;	
	    }	
	i2ctmp= address/256 ;
	i2ctmp= (i2ctmp &0x7) *2 ;	
	i2ctmp |=0xa1;
	if (  i2c_sout (i2ctmp)!=0)
	    {
	     i2c_stop();
	     return 0x0200;	
	    }	
	ch= i2c_shin();
	i2c_nak();
	i2c_stop();
	return  ch;
}

INT16U   i2c_read_byte ( INT16U  address)
{
	INT8U  i2ctmp;
	
	
	if (i2c_start()!=0)
	    {
	     i2c_stop();
	     return 0x0100;	
	    }	
	i2ctmp= address/256 ;
	i2ctmp= (i2ctmp &0x7) *2 ;	
	i2ctmp |=0xa0;
	
	if ( i2c_sout ( i2ctmp ) !=0)
	    {
	     i2c_stop();
	     return 0x0200;	
	    }	
		
	if ( i2c_sout (address &0xff) !=0)
	    {
	     i2c_stop();
	     return 0x0800;	
	    }	
	i2c_stop();
	address=i2c_read_current(address);
	i2c_stop();
	return address;
}


INT16U  i2c_write_byte ( INT8U  ch,INT16U  address)

{
INT8U  i2ctmp;

        WP_LOW;
        WP_OUT;

        _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP();
        _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP();
        _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP();
        _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP();
         _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP();
        _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP();
	
	if (i2c_start()!=0)
	    {
	     i2c_stop();
	     WP_HIGH;
	     return 0x0100;	
	    }	
	i2ctmp= address/256 ;
	i2ctmp= i2ctmp &0x7;
	i2ctmp *=2;
	i2ctmp |=0xa0;
	if ( i2c_sout (i2ctmp) !=0)
	    {
	     i2c_stop();
	     WP_HIGH;
	     return 0x0200;	
	    }	
	i2ctmp= (INT8U) (address & 0x00ff);
	if ( i2c_sout (i2ctmp) !=0)
	    {
	     i2c_stop();
	     WP_HIGH;
	     return 0x0400;	
	    }	
	
	    	
	if ( i2c_sout (ch) !=0)
	    {
	     i2c_stop();
	     WP_HIGH;
	     return 0x0800;	
	    }	
	
	i2c_stop();
	WP_HIGH;
	_NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP();
        _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP();
        _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP();
        _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP();
        _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP();
        _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP();
        _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP();
        _NOP(); _NOP(); _NOP(); _NOP(); _NOP(); _NOP();
	return 0;    	
}

