#include "types.h"
#include "const.h"

#ifndef  SUB_H00
#define  SUB_H00
extern void gprs_pwr_on(void);
extern void gprs_pwr_off(void);

extern INT8U get_modbus_adr( void);
extern INT8U get_switch( void);
extern void run_led_flash(void);
extern void run_led_on(void);
extern void run_led_off(void);

extern INT32S lmax ( INT32S value1, INT32S value2);
extern INT32S lmin ( INT32S value1, INT32S value2);
extern INT16S imin2 ( INT16S value1,INT16S value2);
extern INT16S imax2 ( INT16S value1,INT16S value2);
extern INT16S imin3 ( INT16S value1,INT16S value2,INT16S value3);
extern INT16S imax3 ( INT16S value1,INT16S value2,INT16S value3);

extern void  setdo (INT16U pos,INT16U val);

extern void set_time_all (void);
extern void read_time_all( void);

extern void com1_rcv(void);
extern void com1_tr(void);
extern void com2_rcv(void);
extern void com2_tr(void);


extern INT16U crc_check( INT8U *buf,INT16U lenth);
extern INT16U crc(INT8U *puchMsg , INT16U usDataLen);
extern void gen_err_msg( INT8U * str,INT8U err_code );

extern void write_hldreg( INT16U reg_adr,INT16U value);
extern void write_hldreg_broadcast( INT16U reg_adr,INT16U value);

extern INT16U   i2c_write_byte ( INT8U  ch,INT16U  address);
extern INT16U   i2c_read_byte ( INT16U  address);

extern INT16S   block_empty( INT8U *adr);
extern INT16S judge_wr_key (INT8U *src);
//BIT OPR
extern INT16S getbit(INT8S* ptr, INT16S bitoffs);
extern INT16S setbit(INT8S *ptr, INT16S bitnum, INT16S val);
extern INT16S countbit(INT8S *basep, INT16S nbytes, INT16S val);
extern INT16S bitpatrn(INT8S  chr, INT8S *patp, INT8S patyp);
extern INT16S byte2bit(INT8S byte, INT8S *ptr, INT16S bitoffs, BOOL lastbyt);
extern INT8S bit2byte(INT8S *ptr, INT16S bitoffs, BOOL lastbyt);
extern INT16S  insbits(INT8S *basep, INT16S bitoffs, INT16S nbits, INT16S val, INT16S fldsize);
extern INT16S delbits(INT8S *basep, INT16S bitoffs, INT16S nbits, INT16S val, INT16S fldsize);
extern INT16U bitzvalu(INT8S *ptr, INT16S bitoffs, INT16S nbits);


//SOE
//extern void soe_write ( INT16U di_tmp,INT16U attrib_tmp);
//extern SOE  *soe_read ( INT16U rel_adr );
//extern void soe_init( void);
//SOE end

//flash

extern INT16S flash_block_erase( INT16U block_num);
extern INT16S flash_char_write (INT8U *buf ,INT8U content);
extern INT16S flash_block_write( INT8U *dest ,INT8U *source ,INT8U buf_size);
extern INT16S block_empty( INT8U *adr);
#endif


