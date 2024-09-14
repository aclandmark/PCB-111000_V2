

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

 int main (void)                          //Example 1
  { setup_HW;
  Char_to_PC_Basic('?');
  newline_Basic();
  while (1)
  { Char_to_PC_Local
    (waitforkeypress_Basic());
  }
  return 1;
  }

  


/************************************************************************************************************

  Use this area for saving the examples when they have been got working and finished with
  Use the space above for the active program (the one being worked on)



********Example 1: Echoes keypresses*************************************************************************
  int main (void)                          //Example 1
  { setup_HW;
  Char_to_PC_Basic('?');
  newline_Basic();
  while (1)
  { Char_to_PC_Local
    (waitforkeypress_Basic());
  }
  return 1;
  }




**********Example 2: Prints out ASKII characters**************************************************************
  int main (void)                          //Example 2
  { char symbol;
  setup_HW;
  newline_Basic();
  symbol = '!';
  while (symbol <= '~')
  { Char_to_PC_Local(symbol);
    _delay_ms(10);
    symbol++;
    wdr();
  }
  waitforkeypress_Basic();
  SW_reset;
  return 1;
  }




********Example 3: Echo character string or prints file*******************************************************
int main (void)                          //Example 3
  { setup_HW;
  while (!(isCharavailable_Basic(65)))
    Char_to_PC_Basic('?');
  newline_Basic();
  Char_to_PC_Basic(Char_from_PC_Basic());
  while (1)
  { if (isCharavailable_Basic(10))
      Char_to_PC_Basic(Char_from_PC_Basic());
    else break;
  }
  String_to_PC_Basic("Done\r\n");
  SW_reset;
  return 1;}
 




********Example 4: Send strings to the PC********************************************************************
int main (void)                          //Example 4
  { setup_HW;
  wdt_enable(WDTO_120MS);
  while(switch_1_down)wdr();
  
  String_to_PC_Basic("\r\nDefining and using text strings\r\n");
  const char *message_1 = "Hello world\r\n";
  const char *message_2 = "Sending text to a PC\r\n";
  const char message_3[] = "Writing C programs and\r\n";
  const char message_4[] = "Uploading them to a device\r\n";
  String_to_PC_Basic(message_1);
  String_to_PC_Basic(message_2);
  String_to_PC_Basic(message_3);
  String_to_PC_Basic(message_4);
  while(switch_1_up)wdr();
  while(switch_1_down);
  return 1;
  }  




************Example 5: Generate an ASKII table****************************************************************
int main (void)                          //Example 5
  { char symbol = '!';
  setup_HW;
  wdt_enable(WDTO_120MS);
  while(switch_1_down)wdr();
  
  newline_Basic();
  while (symbol <= '~')
  { Num_to_PC_Local(symbol);
    Char_to_PC_Local(symbol++);
    wdr();_delay_ms(50);
    if (!((symbol - '!') % 8))newline_Basic();
    else Char_to_PC_Local('\t');
  }
  while(switch_1_up)wdr();
  while(switch_1_down);
  }
  


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
