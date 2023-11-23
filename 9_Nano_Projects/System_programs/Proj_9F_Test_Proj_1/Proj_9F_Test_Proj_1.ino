


#include "Receiver_Transmitter_header.h"

 int main (void)   
  { long number = 1;
  char s[12];
  setup_HW;
  wdt_enable(WDTO_120MS);
  while(switch_1_down)wdr();
  while((switch_1_up)  && (number <= 0x20000)){
  Num_to_PC_Local(number);
 
 newline_Basic();
 _delay_ms(25);
 number *= 2;
 wdr();}
 if(number == 0x40000){String_to_PC_Basic("Press sw1 to repeat\r\n");while(switch_1_up)wdr();}
  while(switch_1_down);
  return 1;
  } 


/********************************************************************************************************/
void Char_to_PC_Local(char data)
{ while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = data;}


/********************************************************************************************************/
void Num_to_PC_Local (long number)
{ int i = 0;
  char s[12];

for(int p = 0; p <12; p++)s[p] = 0;
  
  do
  { s[i++] = number % 10 + '0';
  }
  while ((number = number / 10) > 0);
  s[i] = '\0';
  for (int m = i; m > 0; m--)Char_to_PC_Basic(s[m - 1]);
  Char_to_PC_Local(' ');}



/********************************************************************************************************/
