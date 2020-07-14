#include "cpu.h"
#include "types.h"
#include "const.h"
#include "var.h"
#include "sub.h"


#define RD_COIL_STA       1
#define RD_INP_STA        2
#define RD_HLD_REG        3
#define RD_INP_REG        4
#define FORCE_SINGAL_COIL 5
#define PRESET_SINGAL_REG 6
#define FORCE_MUTI_COIL   15
#define FORCE_MUTI_REG    16
#define READ_FILE         20
#define WRITE_FILE        21
#define READ_FIFO         24

#define   NOT_NORMAL_ERR    0x10
#define   NOT_THIS_DEV      0x20
#define   CRC_ERR           0x11

#define   NORMAL            0

#define    MODBUS_EXCPT_1   1
#define    MODBUS_EXCPT_2   2
#define    MODBUS_EXCPT_3   3
#define    MODBUS_EXCPT_4   4
#define    MODBUS_EXCPT_5   5


SOE  *soe_read ( INT16U rel_adr );
void sio1_led_on( void);
void sio1_led_off( void);


const INT8U auchCRCHi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40
} ;

const INT8U  auchCRCLo[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
0x40
} ;
INT16U crc(INT8U *puchMsg , INT16U usDataLen)
{
   INT8U uchCRCHi = 0xFF ;     /* high byte of CRC initialized */
   INT8U uchCRCLo = 0xFF ;     /* low byte of CRC initialized */
   INT16U uIndex ;                   /* will index into CRC lookup table */
   while (usDataLen--)                  /* pass through message buffer */
   {
     uIndex = uchCRCHi ^ *puchMsg++ ;   /* calculate the CRC */
     uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
     uchCRCLo = auchCRCLo[uIndex] ;
   }
  return (uchCRCHi << 8 | uchCRCLo) ;
}
INT16U crc_check( INT8U *buf,INT16U lenth)

{
  INT16U tmp,tmp1;

  tmp=crc(buf,lenth-2);
  tmp1= *(buf+lenth-2)*256;
  tmp1=tmp1+ *(buf+lenth-1);
  if (tmp == tmp1 )return 0;
  else return 1;


}

void gen_err_msg( INT8U * str,INT8U err_code )

{
  *str= get_modbus_adr();
  str++;
  *str+=0x80;
  str++;
  *str=err_code;

}




INT8U  sio1_modbus_rtu ( void)
{
        INT8U ctmp1;
        INT8U *tmp_point;
        INT16U tmp,tmp1,tmp2,tmp3,lenth,*soe_buftmp;
        SOE *soe_tmp;

        ctmp1=sio1_rx_lenth;
         if (    ctmp1 < 3 )
            {   sio1_rx_lenth=0;
                sio1_rx_tmr=0;
                return  NOT_NORMAL_ERR;
            }
         if ( sio1_buf[0]!= get_modbus_adr() && sio1_buf[0] !=0 )
            {
               sio1_rx_lenth=0;
               sio1_rx_tmr=0;
               return NOT_THIS_DEV;
            }
         if ( crc_check ( sio1_buf,ctmp1  )  )
            {
               sio1_rx_lenth=0;
               sio1_rx_tmr=0;
               return CRC_ERR;
            }

        else if( sio1_buf[0] == get_modbus_adr() )
            {
                sio1_led_on();
                switch (sio1_buf[1] )

                  {
                     case  RD_COIL_STA:
                         tmp=sio1_buf[2];
                         tmp=tmp*256 + sio1_buf[3];
                         tmp1=sio1_buf[4];
                         tmp1= tmp1*256 + sio1_buf[5];

                         if (tmp1 <=MAX_DIO_LENTH)
                           {
                                if (tmp<=MAXDO && (tmp+tmp1)<=MAXDO)
                                  {
                                     if( tmp1 %8 )    sio1_buf[2]=tmp1/8+1;
                                     else  sio1_buf[2]=tmp1/8;
                                     for (lenth=0;lenth<tmp1;lenth++)
                                       {
                                         tmp2=setbit( &sio1_buf[3],lenth,getbit(do_out,tmp+lenth) );

                                       }
                                       lenth= sio1_buf[2] +3;

                                     }
                              else
                                   {
                                    gen_err_msg( sio1_buf,MODBUS_EXCPT_2 );
                                    lenth=3;
                                   }
                          }
                       else
                         {
                             gen_err_msg( sio1_buf,MODBUS_EXCPT_3 );
                             lenth=3;
                          }

                     break;


                     case RD_INP_STA:
                         tmp=sio1_buf[2];
                         tmp=tmp*256 + sio1_buf[3];
                         tmp1=sio1_buf[4];
                         tmp1= tmp1*256 + sio1_buf[5];

                         if (tmp1 <=MAX_DIO_LENTH)
                           {
                                if ( tmp<=MAXDI && (tmp+tmp1)<=MAXDI)
                                  {
                                     if( tmp1 %8 )    sio1_buf[2]=tmp1/8+1;
                                     else  sio1_buf[2]=tmp1/8;
                                     for (lenth=0;lenth<tmp1;lenth++)
                                       {
                                         tmp2=setbit( &sio1_buf[3],lenth,getbit(di,tmp+lenth) );

                                       }
                                       lenth= sio1_buf[2] +3;

                                     }
                              else
                                   {
                                    gen_err_msg( sio1_buf,MODBUS_EXCPT_2 );
                                    lenth=3;
                                   }
                          }
                       else
                         {
                             gen_err_msg( sio1_buf,MODBUS_EXCPT_3 );
                             lenth=3;
                          }

                     break;

                     case RD_HLD_REG:
                         tmp=sio1_buf[2];
                         tmp=tmp*256 + sio1_buf[3];
                         tmp1=sio1_buf[4];
                         tmp1= tmp1*256 + sio1_buf[5];

                         if (tmp1 <=MAX_LENTH )
                           {
                                if (tmp <=MAXHLD && (tmp+tmp1)<=MAXHLD)
                                  {
                                     for (lenth=0;lenth<tmp1;lenth++)
                                       {
                                         sio1_buf[3+2*lenth]=hld_reg[tmp+lenth]>>8;
                                         sio1_buf[4+2*lenth]=hld_reg[tmp+lenth]&0xff;
                                       }
                                       lenth=tmp1*2+3;
                                       sio1_buf[2]= tmp1 *2 ;
                                  }


                              else
                                   {
                                    gen_err_msg( sio1_buf,MODBUS_EXCPT_2 );
                                    lenth=3;
                                   }
                          }
                       else
                         {
                             gen_err_msg( sio1_buf,MODBUS_EXCPT_3 );
                             lenth=3;
                          }

                     break;

                     case RD_INP_REG:
                         tmp=sio1_buf[2];
                         tmp=tmp*256 + sio1_buf[3];
                         tmp1=sio1_buf[4];
                         tmp1= tmp1*256 + sio1_buf[5];

                         if (tmp1 <=MAX_LENTH )
                           {
                                if (tmp<=MAXDATA && (tmp+tmp1)<=MAXDATA)
                                  {
                                     for (lenth=0;lenth<tmp1;lenth++)
                                       {
                                         sio1_buf[3+2*lenth]=data[tmp+lenth]>>8;
                                         sio1_buf[4+2*lenth]=data[tmp+lenth]&0xff;
                                       }
                                       lenth=tmp1*2+3;
                                       sio1_buf[2]= tmp1 *2 ;
                                  }
                                else  if  ( (tmp >=SOE_BASE_ADR) && (tmp <=SOE_END_ADR) && (tmp+tmp1)<=SOE_END_ADR)
                                  {

                                        soe_buftmp = ( INT16U *) soe_read(0);
                                        for (lenth=0;lenth<tmp1;lenth++)
                                           {
                                            sio1_buf[3+2*lenth]=* soe_buftmp>>8;
                                            sio1_buf[4+2*lenth]=* soe_buftmp &0xff;
                                            soe_buftmp++;
                                            }
                                        lenth=tmp1*2+3;
                                        sio1_buf[2]= tmp1 *2 ;

                                  }

                               else
                                   {
                                    gen_err_msg( sio1_buf,MODBUS_EXCPT_2 );
                                    lenth=3;
                                   }
                          }
                       else
                         {
                             gen_err_msg( sio1_buf,MODBUS_EXCPT_3 );
                             lenth=3;
                          }
                     break;

                     case FORCE_SINGAL_COIL:
                         tmp=sio1_buf[2];
                         tmp=tmp*256 + sio1_buf[3];
                         tmp1=sio1_buf[4];
                         tmp1= tmp1*256 + sio1_buf[5];
                         if ( tmp<=MAXDO)
                           {
                             if ( tmp1 == 0xff00)
                                  {
                                   setdo ( tmp,1);
                                   lenth=6;
                                  }
                             else if ( tmp1 == 0x0000)
                                  {
                                   setdo ( tmp,0);
                                  // tmp1=setbit( do_out,tmp,0);
                                   lenth=6;
                                  }
                             else {
                                   gen_err_msg( sio1_buf,MODBUS_EXCPT_3 );
                                   lenth=3;
                                  }

                           }
                         else
                           {
                             gen_err_msg( sio1_buf,MODBUS_EXCPT_2 );
                             lenth=3;

                           }
                     break;

                     case PRESET_SINGAL_REG:
                         tmp=sio1_buf[2];
                         tmp=tmp*256 + sio1_buf[3];
                         tmp1=sio1_buf[4];
                         tmp1= tmp1*256 + sio1_buf[5];
                         if ( tmp <=MAXHLD)
                            {
                            write_hldreg(tmp,tmp1);
                            lenth=6;
                            }

                         else
                            {
                             gen_err_msg( sio1_buf,MODBUS_EXCPT_2 );
                             lenth=3;
                            }

                     break;

                     case FORCE_MUTI_COIL:
                         tmp=sio1_buf[2];
                         tmp=tmp*256 + sio1_buf[3];
                         tmp1=sio1_buf[4];
                         tmp1= tmp1*256 + sio1_buf[5];
                         if ( tmp1 % 8) tmp2= tmp1/8 +1;
                         else           tmp2= tmp1/8;
                         if ( (tmp1 <= (MAX_DIO_LENTH-16) )  && (sio1_buf[6] == tmp2) )
                            {
                               if ( tmp<=MAXDO && (tmp+tmp1)<=MAXDO)
                                 {
                                    for ( lenth=0;lenth< tmp1;lenth++)
                                    {
                                      setdo( lenth+tmp,getbit( &sio1_buf[7],lenth) );
                                    }
                                    lenth= 6;
                                 }
                              else
                                {
                                 gen_err_msg( sio1_buf,MODBUS_EXCPT_2 );
                                 lenth=3;
                                }
                            }

                         else
                            {

                             gen_err_msg( sio1_buf,MODBUS_EXCPT_3 );
                             lenth=3;

                            }
                     break;

                     case FORCE_MUTI_REG:
                         tmp=sio1_buf[2];
                         tmp=tmp*256 + sio1_buf[3];
                         tmp1=sio1_buf[4];
                         tmp1= tmp1*256 + sio1_buf[5];
                         tmp2= tmp1 *2;
                         if ( (tmp1 <=(MAX_LENTH-1))  && (sio1_buf[6] == tmp2 ) )
                            {
                               if ( tmp<=MAXHLD && (tmp+tmp1)<=MAXHLD)
                                 {
                                    for ( lenth=0;lenth< tmp1;lenth++)
                                    {
                                       tmp2= sio1_buf[7+2*lenth];
                                       tmp2= tmp2 *256+ sio1_buf[8+2*lenth];
                                       write_hldreg(tmp+lenth,tmp2);
                                    }
                                     lenth= 6;
                                 }
                              else
                                {
                                 gen_err_msg( sio1_buf,MODBUS_EXCPT_2 );
                                 lenth=3;
                                }
                            }

                         else
                            {

                             gen_err_msg( sio1_buf,MODBUS_EXCPT_3 );
                             lenth=3;

                            }
                     break;

                     case READ_FIFO:
                        gen_err_msg( sio1_buf,MODBUS_EXCPT_3 );
                        lenth=3;
                     break;



                     default:
                     gen_err_msg( sio1_buf,MODBUS_EXCPT_1  );
                      lenth =3;
                     break;
                  }


                  tmp=  crc ( sio1_buf,lenth );
                  *(sio1_buf+lenth )=(tmp>>8) &0xff; /* caculate lrc   */
                  *(sio1_buf+lenth +1)= tmp &0xff;
                  sio1_tx_lenth2=lenth+2;
                  sio1_tx_lenth=0;
                  com1_tr();
                  _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
                  _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
                  _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
                  _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
                  _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
                  _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
                  _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
                  _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
                  _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
                  _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
                  UCA0TXBUF= sio1_buf[0];

                   return 0;

            }
         else
             {


              switch (sio1_buf[1] )
                {
                case PRESET_SINGAL_REG:
                         tmp=sio1_buf[2];
                         tmp=tmp*256 + sio1_buf[3];
                         tmp1=sio1_buf[4];
                         tmp1= tmp1*256 + sio1_buf[5];
                         if ( tmp <MAXHLD)
                            {
                            write_hldreg_broadcast(tmp,tmp1);
                            }
                break;

                case FORCE_MUTI_REG:
                         tmp=sio1_buf[2];
                         tmp=tmp*256 + sio1_buf[3];
                         tmp1=sio1_buf[4];
                         tmp1= tmp1*256 + sio1_buf[5];
                         tmp2= tmp1 *2;
                         if ( (tmp1 < MAX_LENTH)  && (sio1_buf[6] == tmp2 ) )
                            {
                               if ( tmp<MAXHLD && (tmp+tmp1)<MAXHLD)
                                 {
                                    for ( lenth=0;lenth< tmp1;lenth++)
                                    {
                                       tmp2= sio1_buf[7+2*lenth];
                                       tmp2= tmp2 *256+ sio1_buf[8+2*lenth];
                                       write_hldreg_broadcast(tmp+lenth,tmp2);
                                    }
                                     lenth= 6;
                                 }
                              else
                                {
                                 gen_err_msg( sio1_buf,MODBUS_EXCPT_2 );
                                 lenth=3;
                                }
                            }

                         else
                            {

                             gen_err_msg( sio1_buf,MODBUS_EXCPT_3 );
                             lenth=3;
                            }


                break;

                default:
                break;
               }

               sio1_tx_lenth=0;
               sio1_rx_lenth=0;
               fr1_ready=0;
               com1_rcv();
               sio1_tx_tmr=0;


             }
      return 0;
}

