#include    <string.h>
#include    "cpu.h"
#include    "types.h"
#include     "var.h"



const  INT8U HLD_FUC []=
        {   1, 1,0,0,      0,0,0,0,     0,0,0,0,      0,0,0,0,
            0, 0,0,0,      0,0,0,0,     0,0,0,0,      0,0,0,0,
            0, 0,0,0,      0,0,0,0,     0,0,0,0,      0,0,0,0,
            0, 0,0,0,      0,0,0,0,     0,0,0,0,      0,0,0,0,
            0, 0,0,0,      0,0,0,0,     0,0,0,0,      0,0,0,0,
            0, 0,0,0,      0,0,0,0,     0,0,0,0,      0,0,0,0,
            0, 0,0,0,      0,0,0,0,     0,0,0,0,      0,0,0,0,
            0, 0,0,0,      0,0,0,0,     0,0,0,2,      2,2,0,0,
        };

const INT8U WR_KEY[]={ 0xaa,0x55,0xa5,0x5a,0x68,0x72,0x67,0x10,0x68,0x48,0x60,0x03,0x68,0x70,0x33,0x53};

void gprs_pwr_on(void)
{
  P9OUT |=BIT4;
}
void gprs_pwr_off(void)
{
  P9OUT &=~BIT4;
}

INT16S judge_wr_key (INT8U *src)

{
   INT8U   *tmp_p,tmpcnt;

  tmp_p=(INT8U *) WR_KEY;
  for ( tmpcnt=0;tmpcnt<16;tmpcnt++ )
     {
       if ( *tmp_p++ !=*src++ ) return -1;
     }
   return 0;
}


void  setmem( INT8S* buffer, INT16S lenth, INT8U val)
{

          while ( lenth-- >0)
          {
           *buffer++ = val;
          }

}
INT32S lmax ( INT32S value1,INT32S value2)
{
if ( value1>=value2 ) return value1;
else return value2;

}
INT32S lmin ( INT32S value1,INT32S value2)
{
if ( value1>=value2 ) return value2;
else return value1;

}

INT16S imax2 (  INT16S  value1, INT16S  value2)
{
if ( value1>=value2 ) return value1;
else return value2;

}
INT16S  imin2 (INT16S  value1, INT16S  value2)
{
if ( value1>=value2 ) return value2;
else return value1;

}

INT16S  imax3 ( INT16S  value1, INT16S  value2,INT16S value3)
{
 if ( value1>=value2 )
   {
     if ( value1>=value3) return value1;
     else return value3;

   }
else
   {
     if ( value2>=value3) return value2;
     else return value3;
   }

}
INT16S imin3 ( INT16S value1, INT16S value2, INT16S value3)
{
 if ( value1>=value2 )
    {
    if ( value2 >=value3) return value3;
    else   return value2;
    }
 else
    {
    if ( value1 >=value3) return value3;
    else   return value1;
    }

}


INT8U get_switch ( void)
{
  const INT8U conv_tab[] =
  {
   0x00,0x80,0x40,0xc0,  0x20,0xa0,0x60,0xe0, 0x10,0x90,0x50,0xd0, 0x30,0xb0,0x70,0xf0,
   0x08,0x88,0x48,0xc8,  0x28,0xa8,0x68,0xe8, 0x18,0x98,0x58,0xd8, 0x38,0xb8,0x78,0xf8,
   0x04,0x84,0x44,0xc4,  0x24,0xa4,0x64,0xe4, 0x14,0x94,0x54,0xd4, 0x34,0xb4,0x74,0xf4,
   0x0c,0x8c,0x4c,0xcc,  0x2c,0xac,0x6c,0xec, 0x1c,0x9c,0x5c,0xdc, 0x3c,0xbc,0x7c,0xfc,

   0x02,0x82,0x42,0xc2,  0x22,0xa2,0x62,0xe2, 0x12,0x92,0x52,0xd2, 0x32,0xb2,0x72,0xf2,
   0x0a,0x8a,0x4a,0xca,  0x2a,0xaa,0x6a,0xea, 0x1a,0x9a,0x5a,0xda, 0x3a,0xba,0x7a,0xfa,
   0x06,0x86,0x46,0xc6,  0x26,0xa6,0x66,0xe6, 0x16,0x96,0x56,0xd6, 0x36,0xb6,0x76,0xf6,
   0x0e,0x8e,0x4e,0xce,  0x2e,0xae,0x6e,0xee, 0x1e,0x9e,0x5e,0xde, 0x3e,0xbe,0x7e,0xfe,

   0x01,0x81,0x41,0xc1,  0x21,0xa1,0x61,0xe1, 0x11,0x91,0x51,0xd1, 0x31,0xb1,0x71,0xf1,
   0x09,0x89,0x49,0xc9,  0x29,0xa9,0x69,0xe9, 0x19,0x99,0x59,0xd9, 0x39,0xb9,0x79,0xf9,
   0x05,0x85,0x45,0xc5,  0x25,0xa5,0x65,0xe5, 0x15,0x95,0x55,0xd5, 0x35,0xb5,0x75,0xf5,
   0x0d,0x8d,0x4d,0xcd,  0x2d,0xad,0x6d,0xed, 0x1d,0x9d,0x5d,0xdd, 0x3d,0xbd,0x7d,0xfd,

   0x03,0x83,0x43,0xc3,  0x23,0xa3,0x63,0xe3, 0x13,0x93,0x53,0xd3, 0x33,0xb3,0x73,0xf3,
   0x0b,0x8b,0x4b,0xcb,  0x2b,0xab,0x6b,0xeb, 0x1b,0x9b,0x5b,0xdb, 0x3b,0xbb,0x7b,0xfb,
   0x07,0x87,0x47,0xc7,  0x27,0xa7,0x67,0xe7, 0x17,0x97,0x57,0xd7, 0x37,0xb7,0x77,0xf7,
   0x0f,0x8f,0x4f,0xcf,  0x2f,0xaf,0x6f,0xef, 0x1f,0x9f,0x5f,0xdf, 0x3f,0xbf,0x7f,0xff,

  };

  return conv_tab[P10IN];
}

INT8U get_modbus_adr( void)
{
  INT8U tmp_adr;
  tmp_adr= hld_reg[MODBUS_ADR];
  if (tmp_adr==0 || tmp_adr==255 ) return 1;
  else return tmp_adr;
}

INT8U get_set( void )
{
  return (P9IN &BIT7);
}

extern void set_ds1302all_time ( INT8U hour_tmp,INT8U min_tmp,INT8U sec_tmp);
extern void ds1302_rtc_init(void);
extern  void read_ds1302time_all(INT16U * tmp_tmr);
extern  void read_rtctime_all(INT16U * tmp_tmr);
extern  void set_ds1302_date (  INT8U year_tmp,INT8U month_tmp,INT8U date_tmp);


void rtc_init ( void)
{
 INT16U  i,k,l;
 INT32U  time;

  ds1302_rtc_init();

  read_ds1302time_all( &hld_reg[H_YEAR] );

  i= hld_reg[H_MIN];
  k= i &0xf0;
  k=k/16;
  k= k*10;
  k= k+ (i & 0xf);
  l= k;

  k= i &0xf000;
  k= k / 0x1000 ;
  k= k*10;
  k=k +  (i&0xf00)/0x100;
  k= k*60;
  l +=k;

  i= hld_reg[H_DAY];
  k= i & 0xf0;
  k= k/16;
  k= k*10;
  k=k + (i & 0xf);
  time= k;

  time = time *3600l+l;
  time = time *1000l;

  hld_reg[0]=  time % 65536l;
  hld_reg[1] = time /65536l;
}
void read_time_all( void)
{
  read_rtctime_all( &hld_reg[H_YEAR] );
}
void set_time_all (void)
{
   INT32U  time;
   INT16U  sec,sec10,min,min10,hour,hour10;
   time = hld_reg[1];
   time = time * 65536l+  hld_reg[0];
   time =time /1000;
   hour10= time/36000l;
   time=time%36000l;
   hour=time/3600;
   time= time %3600;
   min10=time/600;
   time =time %600;
   min=time/60;
   time = time %60;
   sec10= time /10;
   sec=   time %10;

   sec=sec10*16+sec;
   min=min10*16+min;
   hour= hour10 *16+hour;
   set_ds1302all_time ( hour,min,sec);
}

void set_time_( void)
{

     INT16U  i,k,l;
     INT32U  time ;
     INT8U hour,min,sec;
     INT8U yy,mm,dd;

     yy= hld_reg[H_YEAR]/256;
     mm= hld_reg[H_YEAR] &0xff;
     dd= hld_reg[H_DAY]/256;
     set_ds1302_date (yy,mm,dd );

     hour=hld_reg[H_DAY]&0xff;
     min= hld_reg[H_MIN] /256;
     sec= hld_reg[H_MIN] &0xff;
     set_ds1302all_time ( hour,min,sec);



  i= hld_reg[H_MIN];
  k= i &0xf0;
  k=k/16;
  k= k*10;
  k= k+ (i & 0xf);
  l= k;

  k= i &0xf000;
  k= k / 0x1000 ;
  k= k*10;
  k=k +  (i&0xf00)/0x100;
  k= k*60;
  l +=k;

  i= hld_reg[H_DAY];
  k= i & 0xf0;
  k= k/16;
  k= k*10;
  k=k + (i & 0xf);
  time= k;

  time = time *3600l+l;
  time = time *1000l;

  hld_reg[0]=  time % 65536l;
  hld_reg[1] = time /65536l;


}

void write_hldreg( INT16U reg_adr,INT16U value)
{
        switch ( HLD_FUC [reg_adr])
        {
           case 1:
           hld_reg[reg_adr]=value;
           set_time_all();
           break;

           case 2:
           hld_reg[reg_adr]=value;
           set_time_();
           break;

           case 3:

           break;

           default:
           hld_reg[reg_adr]=value;

        }


}

void write_hldreg_broadcast( INT16U reg_adr,INT16U value)
{
       switch ( HLD_FUC [reg_adr])
        {
           case 1:
           hld_reg[reg_adr]=value;
           set_time_all();
           break;

           case 2:
           hld_reg[reg_adr]=value;
           set_time_();
           break;

           default:
           hld_reg[reg_adr]=value;
        }


}

//bit opR

/* BITOPS.C - Various bit-manipulation procedures.
 *
 *	Audit Trail
 *
 *	09/11/87 - v001 - GSK - Original
 */

#include "ctype.h"

#define FULLBYT 0xff
#define TBUFSIZ 16
#define SUCCESS	0
#define FAILURE	-1
//#define NULL (char *)0

//typedef unsigned BOOL;

/* *************************** GENERAL NOTES *****************************
 *
 *	Most of these routines deal with "bit fields" of arbitrary
 *	length. In general, you pass them a (char) pointer to a block of memory,
 * 	which will be interpreted and manipulated as a stream of bits, regardless
 *	of byte or word boundaries after the original pointer. Several of these
 *	routines take a bit offset argument, which is an ABSOLUTE number of bits
 *	(zero-relative) from the base pointer; in other words, a bit offset of
 *	75 points to the 76th bit in the block, starting from the base pointer.
 *
 ****************************************************************************/


/* GETBIT - Get the value of the bit (0, 1) at passed ptr, offset by bitoffs
 *			bits. The offset may be any positive number; if >8, the byte
 *			pointer will be incremented accordingly.
 */

INT16S getbit(INT8S* ptr, INT16S bitoffs)

//char *ptr;		/* Base pointer */
//short bitoffs;	/* Offset from ptr in number of bits */

{

	if (ptr == NULL || bitoffs < 0)
	    return (FAILURE);

	ptr += (bitoffs / 8);	/* Knock up ptr to byte requested */
	bitoffs %= 8;		/* Get bit relative to its own byte */

	//return (short)((*ptr >> (7 - bitoffs)) & 1);
	return (short)((*ptr >> bitoffs) & 1);
}


/* SETBIT - Set a specified bit number in a char string to 1 or 0. The bit
 *			number may be any positive value; if > 8, the byte pointer will
 *			be incremented accordingly.
 */

INT16S setbit(INT8S *ptr, INT16S bitnum, INT16S val)

//char *ptr;		/* Base of string */
//short bitnum;	/* Bit number, 0 relative */
//short val;		/* Value to set, 1 or 0 */

{
	INT16S byteoffs;		/* Byte offset in string */
	INT8S  mask;		/* Masking value */


	if (ptr == NULL || bitnum < 0 || (val != 0 && val != 1))
	    return (FAILURE);

	ptr += (byteoffs = bitnum / 8);	/* Calc offset, reset ptr */

	if (byteoffs)
	    bitnum %= (byteoffs * 8);	/* Find bit within byte */

/* Mask = bit string of zeroes with one in bitnum position */

 	//mask = 1 << (7 - bitnum);
        mask = 1 <<  bitnum;
	if (val)			/* If turn-on bit, OR with mask */
	    *ptr |= mask;
	else				/* Else, AND with inverse (ones-complement) of mask */
	    *ptr &= ~mask;

	return (SUCCESS);
}


/* COUNTBIT - Count the bits of specified value (0 or 1) in a bit field. The
 *				field may be any number of bytes in length. If val==0, then
 *				"off" bits will be counted; otherwise, "on" bits.
 */

INT16S countbit(INT8S *basep, INT16S nbytes, INT16S val)

//char *basep;	/* Starting byte to begin count */
//short nbytes;	/* Number of bytes to count thru */
//short val;		/* Bit value to count, 0 or 1 */

{
	INT16S count;	/* Number of bits counted */
	INT16S bit;	/* Bit loop counter */
	INT8S *ptr;	/* Loop pointer */


	if (basep == NULL || nbytes <= 0 || (val != 0 && val != 1))
	    return (FAILURE);

	count = 0;

	for (ptr = basep; ptr < basep + nbytes; ptr++)
		{
	    for (bit = 0; bit < 8; bit++)
		count += (getbit(ptr, bit) == val);
		}

	return (count);
}


/* BITPATRN -  Generate an array of chars or ints corresponding to the bit
 *				pattern of the passed char. If patyp == 'C', an array of
 *				character '0's and '1's will be generated; if 'I', an array
 *				numeric 0/1's.
 *
 *			NOTE that you pass a single CHAR, not a char *. Thus if you
 *				need a pattern array more than 8 elements long, call
 *				bitpatrn() in a loop and concatenate the results.
 */

INT16S bitpatrn(INT8S  chr, INT8S *patp, INT8S patyp)

//char chr;	/* Char to generate pattern from */
//char *patp;	/* Ptr to array of chars or ints, depending on patyp */
//char patyp;	/* Pattern type: Char or Int (NOT case sensitive) */

{
	INT16S ptrinc;	/* Pointer increment per bit */
	INT16S bit;		/* Bit counter */
	INT16S bitval;	/* Bit value, 1 or 0 */


	patyp = toupper(patyp);

	if (patp == NULL || (patyp != 'C' && patyp != 'I'))
	    return (FAILURE);

	ptrinc = 1 + (patyp == 'I');

	for (bit = 0; bit < 8; bit++, patp += ptrinc)
		{
	    bitval = getbit(&chr, bit);

	    if (patyp == 'I')	/* Int array: Set low bytes of each pair */
			{
			*patp = bitval;
			*(patp + 1) = 0;
	    	}
	    else
			*patp = '0' + bitval;
		}
	return (SUCCESS);
}


/* BYTE2BIT -  Translate the byte passed to a bit pattern, placed at char
 *				*ptr, offset by bitoffs bits from left. Do not write onto next
 *				byte from ptr if lastbyt set. This allows a bit pattern to be
 *				copied to another place in memory regardless of byte
 *				boundaries. For example, if bitoffs == 3, then the left 5 bits
 *				of the passed byte will be copied to the right 5 bits of the
 *				destination ptr, and the right 3 bits of the byte onto the
 *				left 3 bits of the following location (unless lastbyt == YES),
 *				in which case only the FIRST dest char will be written on).
 *				Issuing a call with a ZERO bit offset, like:
 *
 *					byte2bit('A', dest, 0, anything);
 *
 *				is equivalent to: *dest = 'A';
 */

INT16S byte2bit(INT8S byte, INT8S *ptr, INT16S bitoffs, BOOL lastbyt)

//char byte;		/* Character to translate */
//char *ptr;		/* DESTINATION Ptr: Points to starting byte to write to */
//short bitoffs;	/* Offset of starting bit from DESTINATION ptr, 0-7 */
//BOOL lastbyt;	/* Flag: Do not write bits to next char if set */

{
	INT16S bit;	/* Bit counter */


	if (ptr == NULL || bitoffs < 0 || bitoffs > 7)
	    return (FAILURE);

/* Write left side of passed byte to right side of 1st destination char,
   leaving left side of dest char unchanged */

	for (bit = bitoffs; bit <= 7; bit++)
	    setbit(ptr, bit, getbit(&byte, bit - bitoffs));

/* Write right side of byte to left side of next destination char,
   leaving right side unchanged */

	if (bitoffs && !lastbyt)
	    for (bit = 0; bit < bitoffs; bit++)
			setbit(ptr + 1, bit, getbit(&byte, bit + 8 - bitoffs));

	return (SUCCESS);
}


/* BIT2BYTE.C - Take next 8 bits from starting char *, offset by specified
 *				number of bits (bitoffs), translate to a byte and return
 *				value as a char. Starting point need not conform to a byte
 *				boundary. For example, if bitoffs = 3, then translation will
 *				begin with the 4th bit (offsets are 0-relative) of the passed
 *				ptr; if lastbyt == YES, then only the remaining 5 bits will
 *				be translated (offsets 3 thru 7); otherwise, a full 8 bits
 *				will be translated, the remaining 3 coming from the left 3
 *				bits of the byte following ptr. Issuing a call with a ZERO
 *				bit offset like:
 *
 *					mychar = bit2byte(there, 0, anything);
 *
 *				is equivalent to: mychar = *there;
 */

INT8S bit2byte(INT8S *ptr, INT16S bitoffs, BOOL lastbyt)

//char *ptr;		/* Ptr to starting byte to take from */
//short bitoffs;	/* Offset of starting bit from ptr, 0-7 */
//BOOL lastbyt;	/* Flag: Do not take bits from next char if set */

{
	INT8S byte;


	if (ptr == NULL || bitoffs < 0 || bitoffs > 7)
	    return (0);

	byte = *ptr << bitoffs;		/* Set left side of byte */

	if (bitoffs && !lastbyt)	/* Set right side from next byte */

	    byte |= (*(++ptr) >> (8 - bitoffs));

	return (byte);
}


/* INSBITS - Insert specified number of bits to bit field beginning at basep,
 *				starting insertion bitoffs bits from the base. Inserted bits
 *				will be all of the same	value (0 or 1). Bits above the
 *				insertion point will be pushed up in memory, up the limit of
 *				total field size fldsize (arbitrary size limit defined by
 *				TBUFSIZ).
 *
 *			NOTE: If you need a routine to insert a specific bit pattern
 *				(not just all 0's or 1's), use this as a template. You only
 *				need to change the "fill middle bytes" section.
 */

INT16S  insbits(INT8S *basep, INT16S bitoffs, INT16S nbits, INT16S val, INT16S fldsize)

//char *basep;	/* Ptr to base of bit field */
//short bitoffs;	/* Number of bits offset from left to begin insert */
//short nbits;	/* Number of bits to insert */
//short val;		/* Value to insert, 1 or 0 */
//short fldsize;	/* Total size of bit field in bytes */

{
	INT8S buffer[TBUFSIZ];		/* Temporary copy buffer */
	INT8S *firstbyt, *lastbyt;	/* Bytes where insert begins/ends */
	INT16S firstoffs, lastoffs;	/* Bit offsets in first, last bytes */
	INT16S n;					/* Field index counter */
	INT16S lastbit;				/* Offset of last bit in field */
	INT8S *ptr;					/* Temporary ptr */


	if (basep == NULL || nbits <= 0 || fldsize <= 0 || fldsize > TBUFSIZ)
	    return (FAILURE);

	firstbyt = basep + bitoffs / 8;
	lastbit = bitoffs + nbits - 1;
	lastbyt = basep + lastbit / 8;
	firstoffs = bitoffs % 8;
	lastoffs = lastbit % 8;

	if (firstbyt >= basep + fldsize)
	    return (FAILURE);

	 setmem(buffer, sizeof(buffer), 0);	/* Init copy buffer */

/* Copy from firstbyt thru end of field to buffer */

	for (ptr = firstbyt, n = 0; ptr < basep + fldsize; ptr++, n++)
	    buffer[n] = bit2byte(ptr, firstoffs, ptr == basep + fldsize - 1);

/* Set the starting byte, leaving left part as is */

	if (val)
	    *firstbyt |= FULLBYT >> firstoffs;
	else
	    *firstbyt &= FULLBYT << (8 - firstoffs);

/* Fill middle bytes; do not overwrite end of field */

	for (ptr = firstbyt + 1; ptr < lastbyt; ptr++)
		{
	    if (ptr >= basep + fldsize)
			break;

		*ptr = (val) ? FULLBYT : 0;
		}

/* Set ending byte, leaving right part as is */

	if (lastbyt < basep + fldsize && lastbyt != firstbyt)
		{
	    if (val)
	    	*lastbyt |= FULLBYT << (7 - lastoffs);
	    else
	    	*lastbyt &= FULLBYT >> (lastoffs + 1);
		}

/* Write buffer from last byte to end of field */

	for (ptr = lastbyt, n = 0; ptr < basep + fldsize; ptr++, n++)
	    byte2bit(buffer[n], ptr, lastoffs + 1, ptr == basep + fldsize - 1);

	return (SUCCESS);
}


/* DELBITS.C - Delete specified number of bits from bit field beginning at
 *				basep offset by bitoffs bits. The bit field may be any number
 *				of bytes in length, and the deleted section may be any number
 *				of bits in length and need not conform to byte boundaries. The
 *				bits above the deleted section are moved down to fill the
 *				gap, and the top of the field filled with the specified value.
 *
 *		NOTE: Bit field size fldsize is arbitrarily limited to TBUFSIZ as
 *				defined below.
 */

INT16S delbits(INT8S *basep, INT16S bitoffs, INT16S nbits, INT16S val, INT16S fldsize)

//char *basep;	/* Ptr to base of bit field */
//short bitoffs;	/* Number of bits offset from left to begin delete */
//short nbits;	/* Number of bits to delete */
//short val;		/* Value to fill vacated right side of field, 1 or 0 */
//short fldsize;	/* Total size of bit field in BYTES */

{
	INT8S buffer[TBUFSIZ];		/* Temporary copy buffer */
	INT8S *firstbyt, *lastbyt;	/* Bytes where delete begins/ends */
	INT16S firstoffs, lastoffs;	/* Bit offsets in first, last bytes */
	INT16S n;					/* Field index counter */
	INT16S lastbit;				/* Offset of last bit in field */
	INT8S *ptr;					/* Temporary ptr */


	if (basep == NULL || nbits <= 0 || fldsize <= 0 || fldsize > TBUFSIZ)
	    return (FAILURE);

	firstbyt = basep + bitoffs / 8;
	lastbit = bitoffs + nbits - 1;
	lastbyt = basep + lastbit / 8;
	firstoffs = bitoffs % 8;
	lastoffs = lastbit % 8;

	if (firstbyt >= basep + fldsize)
	    return (FAILURE);

	if (val)
	    setmem(buffer, sizeof(buffer), FULLBYT);	/* Init copy buffer */
	else
	    setmem(buffer, sizeof(buffer), 0);

/* Copy to buffer: From bit after end of deleted section to end of field */

	if (++lastoffs > 7)
		{
	    lastbyt++;
	    lastoffs = 0;
		}

	for (ptr = lastbyt, n = 0; ptr < basep + fldsize; ptr++, n++)
	    buffer[n] = bit2byte(ptr, lastoffs, ptr == basep + fldsize - 1);

/* Write buffer to deleted part thru end of field; end is filled with val */

	for (ptr = firstbyt, n = 0; ptr < basep + fldsize; ptr++, n++)
	    byte2bit(buffer[n], ptr, firstoffs, ptr == basep + fldsize - 1);

	return (SUCCESS);
}


/* BITZVALU - Evaluate the next nbits bits from the starting char * and bit
 *				offset and return the value. Uses a simple counting algorithm,
 *				starting with the rightmost bit in the designated field and
 *				moving left, adding a successive power of 2 for each "on" bit.
 *
 *			EXAMPLES: Given a pointer to the array of hex values: 1E A4 7C
 *
 *						bitzvalu(ptr, 8, 8)		returns 164 (00A4)
 *						bitzvalu(ptr, 3, 4)		returns  15 (000F)
 *						bitzvalu(ptr, 10, 10)	returns 583 (0247)
 *
 *		NOTE: Returns an unsigned value. The number of bits evaluated may not
 *			  be more than 16. If bad parameters are passed, returns 0.
 */

INT16U bitzvalu(INT8S *ptr, INT16S bitoffs, INT16S nbits)

//char *ptr;		/* Ptr to starting byte to take from */
//short bitoffs;	/* Offset of starting bit from ptr */
//short nbits;	/* Number of bits to evaluate */
{
	INT16U num = 0;	/* Initialize all bits to 0 */
	INT16S bit;
	INT16U mult = 1;

	if (ptr == NULL || nbits < 1 || nbits > 16)
		return (0);

	for (bit = nbits - 1; bit >= 0; bit--, mult *= 2)
		num += (getbit(ptr, bitoffs + bit) * mult);

	return (num);
}

