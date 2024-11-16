
#include "File_download_with_interrupt_header.h"

volatile long PORT_1 = 1;  


 int main (void)   
  {char keypress;
  setup_HW;

  sei();
   T1_clock_tick(100);

 while (!(isCharavailable_Basic(65)))    //Use local version
    Char_to_PC_Basic('?');                //Use local version
  newline_Basic();
  Char_to_PC_Basic(Char_from_PC_Basic());
  while (1)
  { if (isCharavailable_Basic(200))
      Char_to_PC_Basic(Char_from_PC_Basic());
    else break;
  }
  String_to_PC_Basic("\r\nDone: AK to repeat\r\n");
 waitforkeypress_Basic();

   SW_reset;
    }



void T1_clock_tick(int T1_period_in_ms)                               //Start the T1 clock
{ TCNT1 = 0;
OCR1A = T1_period_in_ms * 125;
  TIMSK1 |= (1 <<  OCIE1A);
  TCCR1B = 0x03;
}



ISR(TIMER1_COMPA_vect)                                              //Example 1
{
 TCNT1 = 0; 
 
if(PORT_1 < 0x10000)I2C_Tx_2_integers(PORT_1, PORT_1);
PORT_1 = PORT_1 << 1;
if (PORT_1 == 0x10000)PORT_1 = 1;}
