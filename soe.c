#include    "cpu.h"
#include    "const.h"
#include    "var.h"
#include    "types.h"

void soe_write ( INT16U di_tmp,INT16U attrib_tmp )
     {
       _DINT();
       soe_write_p->di_num=di_tmp;
       soe_write_p->soe_attrib=attrib_tmp;
       soe_write_p->soe_timeL=hld_reg[ TIME_1MS_L ];
       soe_write_p->soe_timeH=hld_reg[ TIME_1MS_H ];

       if ( soe_write_p  == &soe_buf[MAXFIFO-1]) soe_write_p = soe_buf;
       else  soe_write_p++;
       _EINT();
     }

SOE  *soe_read ( INT16U rel_adr )
     {
       static SOE *soe_tmp= NULL, *soe1;
      // rel_adr = rel_adr % MAXFIFO;
       if (soe_tmp == NULL) soe_tmp =soe_buf;
       if  ( soe_tmp == soe_write_p )  return NULL;
       soe1= soe_tmp;
       soe_tmp ++;
       if ( soe_tmp > &soe_buf[MAXFIFO-1]) soe_tmp= soe_buf;
       return soe1;
     }

/*
SOE  *soe_read ( INT16U rel_adr )
     {
       SOE *soe_tmp;
       rel_adr = rel_adr % MAXFIFO;
       soe_tmp =soe_write_p;
       if( (soe_tmp + rel_adr ) >&soe_buf[MAXFIFO-1] ) soe_tmp=soe_write_p+ rel_adr-MAXFIFO;
       else soe_tmp=soe_write_p+ rel_adr;

       while ( soe_tmp->di_num == 0xffff)
        {
            if (soe_tmp >=&soe_buf[MAXFIFO-1]  ) soe_tmp= soe_buf;
            else  soe_tmp++;
            if (soe_tmp == soe_write_p) break;
        }

        return soe_tmp;

     }
*/
void soe_init( void)
    {

       for ( soe_write_p = soe_buf;soe_write_p<=&soe_buf[MAXFIFO-1];soe_write_p++)
       {
         soe_write_p->di_num    = 0xffff;
         soe_write_p->soe_attrib= 0xffff;
         soe_write_p->soe_timeL = 0xffff;
         soe_write_p->soe_timeH = 0xffff;
       }
       soe_write_p=soe_buf; soe_read_p =soe_buf;
    }




