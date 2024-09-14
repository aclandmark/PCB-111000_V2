

/*
In this project the "setup_328_HW_Arduino_IO" code segment has been replaced by "setup_328_HW_Basic_IO".

Arduino provides a comprehensive library for sending data to and from a PC.
However simple DIY (do it yourself) functions are used here to illustate
the steps involved and introduce the Atmega328 data sheet.
 
ASKII coding is introduced.  Every character on the PC keyboard is assigned a number from 32 to 127.
For example 63 is used to send a question mark '?' to the PC.
Decimal 125 is sent as 3 eight bit numbers 49, 50 and 53. 
48 is used to represent zero.
digits 1, 2 and 5 can therfore by extracted by subtracting 48 from 49, 50 and 53.  
5 is then added to 2 times 10 and 1 times 100 to obtain 125 in binary form. 
*/



#include "Receiver_Transmitter_header.h"

 
 


*****************Example 6: Sending numbers to the PC*********************************************************
 int main (void)                          //Example 6
  { int i = 0, number = 12345;
  char s[12];
  setup_HW;
  wdt_enable(WDTO_120MS);
  while(switch_1_down)wdr();
  
  do {
    s[i++] = number % 10 + '0';
  }
  while ((number = number / 10) > 0);
  s[i] = '\0';
  for (int m = i; m > 0; m--)
    Char_to_PC_Basic(s[m - 1]);
 
 newline_Basic();
 while(switch_1_up)wdr();
  while(switch_1_down);
  return 1;
  }



**************Example 7: Entering numbers at the keyboard*****************************************************
int main (void)                          //Example 7
  { long num = 0;
  char keypress;
  setup_HW;
  num = 0; Char_to_PC_Basic('?');
  while ((keypress = waitforkeypress_Basic()) != '\r')
  { num = num * 10 + keypress  - '0';
    I2C_Tx_long(num);
  }
  Num_to_PC_Local(num * 2);
    I2C_Tx_long(num * 2);
    while(switch_1_up);
    newline_Basic();
  SW_reset;
  return 1;
  }  
















************This area is for project subroutines*************************************************************/
void Num_string_from_KBD_Basic(char * array_ptr)
{ char keypress;
  while ((keypress = waitforkeypress_Basic()) != '\r')
  { *array_ptr = keypress;
    array_ptr += 1;
  }
  *array_ptr = '\0';
}



/********************************************************************************************************/
long Askii_to_binary(char * array_ptr) {
  long num = 0;
{ int m = 0; while (*(array_ptr + m)) {
      num = num * 10 + (*(array_ptr + m++))  - '0';
    }
  }
  return num;
}



/********************************************************************************************************/
int Binary_to_Askii (long number, char * array_ptr)
{ int i = 0;
  do {
    array_ptr[i++] = number % 10 + '0';
  }
  while ((number = number / 10) > 0);
  array_ptr[i] = '\0';
  return i;
}


/********************************************************************************************************/
void Char_to_PC_Local(char data)
{ while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = data;
}




/********************************************************************************************************/
void Num_to_PC_Local (long number)
{ int i = 0;
  char s[12];
  
  do
  { s[i++] = number % 10 + '0';
  }
  while ((number = number / 10) > 0);
  s[i] = '\0';
  for (int m = i; m > 0; m--)Char_to_PC_Basic(s[m - 1]);
  Char_to_PC_Basic(' ');
}



/********************************************************************************************************/
