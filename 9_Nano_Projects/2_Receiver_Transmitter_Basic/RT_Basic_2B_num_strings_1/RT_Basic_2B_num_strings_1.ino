
/*Sending numbers to the PC*/


#include "Receiver_Transmitter_header.h"


int main (void)                          //Example 1 
  { int i = 0;
  int number = 12345;
  char s[12];
  setup_HW;
  //Set up watchdog timer to generate a reset
  wdt_enable(WDTO_120MS);
 while(switch_1_down)wdr();

  //Convert number to string
  while(1) {
    s[i++] = number % 10 + '0';
    number = number/10;
    if (!(number))break;
  }s[i] = '\0';

  //Print string out in reverse order
    for (int m = i; m > 0; m--)
    Char_to_PC_Basic(s[m - 1]);
  newline_Basic();
 //Generate a reset
 while(switch_1_up)wdr();
  while(switch_1_down);
  return 1;
  }



 /*int main (void)                          //Example 1 
  { int i = 0;
  int number = 12345;
  char s[12];
  setup_HW;
  //Set up watchdog timer to generate a reset
  wdt_enable(WDTO_120MS);
 while(switch_1_down)wdr();

  //Convert number to string
  while(1) {
    s[i++] = number % 10 + '0';
    number = number/10;
    if (!(number))break;
  }s[i] = '\0';

  //Print string out in reverse order
    for (int m = i; m > 0; m--)
    Char_to_PC_Basic(s[m - 1]);
  newline_Basic();
 //Generate a reset
 while(switch_1_up)wdr();
  while(switch_1_down);
  return 1;
  }*/



/*int main (void)                          //Example 2
  { long number = 1;
  long number_old = 1;
 
  setup_HW;
  wdt_enable(WDTO_120MS);
  while(switch_1_down)wdr();
  
  for(int m = 2; m < 11; m++){
  Int_to_PC_Basic(number);
 newline_Basic();

number = (number_old * 10) + m;  
number_old = number;}

while(switch_1_up)wdr();
  while(switch_1_down);
  return 1;}*/



/*int main (void)                          //Example 3
  { int i = 0; 
  long number = 123456789;
  long number_old = 123456789;
  char s[12];
  setup_HW;
   wdt_enable(WDTO_120MS);
  while(switch_1_down)wdr();
  
  for(int m = 2; m < 11; m++){
Int_to_PC_Basic(number);
 newline_Basic();

number = number_old / 10;  
number_old = number;}

while(switch_1_up)wdr();
  while(switch_1_down);
  return 1;}*/




/********************************************************************************************************/
