



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
  String_to_PC_Local("\r\nExploring the operation oof the USART\r\n");
  Char_to_PC_Local('?');
  newline_Basic();
  while (1)
  { Char_to_PC_Local
    (waitforkeypress_Local());
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
  waitforkeypress_Local();
  SW_reset;
  return 1;
  }




********Example 3: Echo character string or prints file*******************************************************
int main (void)                          //Example 3
  { setup_HW;
  while (!(isCharavailable_Local(65)))    //Use local version
    Char_to_PC_Local('?');                //Use local version
  newline_Basic();
  Char_to_PC_Local(Char_from_PC_Local());
  while (1)
  { if (isCharavailable_Local(10))
      Char_to_PC_Local(Char_from_PC_Local());
    else break;
  }
  String_to_PC_Local("Done\r\n");
  SW_reset;
  return 1;}
 




********Example 4: Send strings to the PC********************************************************************
int main (void)                          //Example 4
  { setup_HW;
  wdt_enable(WDTO_120MS);
  while(switch_1_down)wdr();
  
  String_to_PC_Local("\r\nDefining and using text strings\r\n");
  const char *message_1 = "Hello world\r\n";
  const char *message_2 = "Sending text to a PC\r\n";
  const char message_3[] = "Writing C programs and\r\n";
  const char message_4[] = "Uploading them to a device\r\n";
  String_to_PC_Local(message_1);
  String_to_PC_Local(message_2);
  String_to_PC_Local(message_3);
  String_to_PC_Local(message_4);
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
  UDR0 = data;}

/********************************************************************************************************/
char waitforkeypress_Local (void){                    //Wait here indefinitely but prevent WDTime-out
while (!(UCSR0A & (1 << RXC0)))wdr(); 
return UDR0;}   



char isCharavailable_Local (int m){int n = 0;
while (!(UCSR0A & (1 << RXC0)))                      //Return 1 immediately that a character is received
{n++; wdr();                                        //No character yet: Increment counter                     
if (n>8000) {m--;n = 0;}if (m == 0)return 0;}        //Counter overflows before a character has been received: return zero
return 1;}


char Char_from_PC_Local(void)                         //Return character detected by "isCharavailable()"
{return UDR0;}


void String_to_PC_Local(const char s[]){                //Transmits a sequence (string) of characters and requires the address in program memory of the first one
int i = 0;                                              //"i" gives the relative address of the next character to be transmitted
while(i < 200){                                         //Transmits up to 200 characters using "Char_to_PC()" or untill the null (\0) character is detected
if(s[i] == '\0') break;
Char_to_PC_Local(s[i++]);}}                             //Transmit character and increment "i" so that it addresses (points to) the next one.




/********************************************************************************************************/
