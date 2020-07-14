#include    "cpu.h"
#include    "types.h"


void spi_init(void)

{
  ME2 |= USPIE1;
  U1CTL  |= SYNC+CHAR+MM;
  U1TCTL |= CKPH+SSEL1+SSEL0+STC;
  U1BR0 = 0x02;                             // SMCLK/2 for baud rate
  U1BR1 = 0;                                // SMCLK/2 for baud rate
  U1MCTL = 0;                               // Clear modulation
  U1CTL &= ~SWRST;                          // Initialize USART state machine
}

