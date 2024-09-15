

#include "Receiver_Transmitter_header.h"


int main (void){
  char num_string[12];
setup_HW;
User_prompt;
Num_string_from_KBD_Basic(num_string);
newline_Basic();
String_to_PC_Basic(num_string);
SW_reset;
return 1;}



void Num_string_from_KBD_Basic(char * array_ptr)
{ char keypress;
  while ((keypress = waitforkeypress_Basic()) != '\r')
  { *array_ptr = keypress;
    Char_to_PC_Basic(keypress);
    array_ptr += 1;
  }
  *array_ptr = '\0';}



/********************************************************************************************************/
long Askii_to_binary(char * array_ptr) {
  long num = 0;
{ int m = 0; while (*(array_ptr + m)) {
      num = num * 10 + (*(array_ptr + m++))  - '0';
    }
  }
  return num;}


/*************************************************************************************************************/
 void num_to_PC (long number)                          //Example 6
  { 
  
  while(1) {
    s[i++] = number % 10 + '0';
    number = number/10;
    if (!(number))break;
  }s[i] = '\0';
  
    for (int m = i; m > 0; m--)
    Char_to_PC_Basic(s[m - 1]);
 
 newline_Basic();
  return 1;
  }
  
/*long Int_KBD_to_display_A_Local(char num_string[]){                //Acquires an integer string from the keyboard and returns the binary equivalent
char keypress;
long Long_Num_from_mini_OS;
int s=0;

while(1){
keypress = waitforkeypress_Basic();
if ((!(decimal_digit_Basic(keypress)))
&& (keypress != '-'))continue;                                //Ignore keypress if it is not OK
Char_to_PC_Basic(keypress);
num_string[s++] = keypress;
break;}
 
while(1){
if ((keypress = wait_for_return_key_Basic())  =='\r')break;         //Detect return key press (i.e \r or\r\n)
if ((decimal_digit_Basic(keypress)) || (keypress == '-'))
{num_string[s++] = keypress;
Char_to_PC_Basic(keypress);
}}  
newline_Basic();                                             
String_to_PC_Basic(num_string);
return 1;}*/



/********************************************************************************************************************/
