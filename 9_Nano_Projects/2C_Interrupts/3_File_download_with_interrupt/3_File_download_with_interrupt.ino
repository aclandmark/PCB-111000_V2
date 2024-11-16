
#include "File_download_with_interrupt_header.h"

volatile long PORT_1 = 1;  
#define ctlz  0x1a

 int main (void)   
  {char keypress;
  setup_HW;

  sei();
  T1_clock_tick(100);

if(watch_dog_reset != 1)String_to_PC_Basic("Send file: Ctrl-Z when finished\r\n");
  
while (!(isCharavailable_Basic(65)))
    Char_to_PC_Basic('?');  
  newline_Basic();
Char_to_PC_Basic(Char_from_PC_Basic());

while ((keypress = waitforkeypress_Basic()) != ctlz)Char_to_PC_Basic(keypress);
 String_to_PC_Basic("\r\nDone: AK to repeat\r\n");
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
