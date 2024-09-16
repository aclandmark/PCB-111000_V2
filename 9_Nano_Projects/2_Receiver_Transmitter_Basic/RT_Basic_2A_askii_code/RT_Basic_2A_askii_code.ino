



#include "Receiver_Transmitter_header.h"

  int main (void)                          //Example 5
  { char symbol = '!';
  setup_HW;
  wdt_enable(WDTO_120MS);
  while(switch_1_down)wdr();
  
  newline_Basic();
  while (symbol <= '~')
  { Int_to_PC_Basic(symbol);
    Char_to_PC_Local(symbol++);
    wdr();_delay_ms(50);
    if (!((symbol - '!') % 8))newline_Basic();
    else Char_to_PC_Local('\t');
  }
  while(switch_1_up)wdr();
  while(switch_1_down);
  }
  


/************************************************************************************************************

  Use this area for saving the examples when they have been got working and finished with
  Use the space above for the active program (the one being worked on)



********Example 1: Echoes keypresses*************************************************************************
  int main (void)                          //Example 1
  { setup_HW;
  User_prompt;
  String_to_PC_Basic("\r\nExploring the operation oof the USART\r\n");
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
  { Int_to_PC_Basic(symbol);
    Char_to_PC_Local(symbol++);
    wdr();_delay_ms(50);
    if (!((symbol - '!') % 8))newline_Basic();
    else Char_to_PC_Local('\t');
  }
  while(switch_1_up)wdr();
  while(switch_1_down);
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
