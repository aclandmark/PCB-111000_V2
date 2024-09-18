/* Numbers from the keyboard*/

#include "Receiver_Transmitter_header.h"

int main (void){                    //Example 1
  char num_string[12];
  long num;
setup_HW;
if(watch_dog_reset == 1)watch_dog_reset = 0; else
String_to_PC_Basic("Enter number at keyboard\r\n");
Num_string_from_KBD_Local(num_string);
Char_to_PC_Basic('\t');
num = Askii_to_binary_Local(num_string);
Int_to_PC_Basic(num/2);
newline_Basic();
SW_reset;
return 1;}


/*
int main (void){                    //Example 1
  char num_string[12];
  long num;
setup_HW;
if(watch_dog_reset == 1)watch_dog_reset = 0; else
String_to_PC_Basic("Enter number at keyboard\r\n");
Num_string_from_KBD_Local(num_string);
Char_to_PC_Basic('\t');
num = Askii_to_binary_Local(num_string);
Int_to_PC_Basic(num/2);
newline_Basic();
SW_reset;
return 1;}
*/


/*
int main (void){                      //Example 2
  char num_string[12];
  long num = 2;
  long old_num = 2;
setup_HW;
String_to_PC_Basic("Multiply number by 2 and enter it\r\n");
String_to_PC_Basic("2\r\n");
while(1){
  
Num_string_from_KBD_Local(num_string);
num = Askii_to_binary_Local(num_string);

if(num == old_num * 2)
{newline_Basic();old_num = num;}
else Char_to_PC_Basic('?');
}
SW_reset;
return 1;}

*/


/***********************Subroutines*********************************/
void Num_string_from_KBD_Local(char * array_ptr)
{ char keypress;
  while ((keypress = waitforkeypress_Basic()) != '\r')
  { *array_ptr = keypress;
    Char_to_PC_Basic(keypress);
    array_ptr += 1;
  }
  *array_ptr = '\0';}



/********************************************************************************************************/
long Askii_to_binary_Local(char * array_ptr) {
  long num = 0;
{ int m = 0; while (*(array_ptr + m)) {
      num = num * 10 + (*(array_ptr + m++))  - '0';
    }
  }
  return num;}



/*************************************************************************************************************/
/* void num_to_PC_Local (long number)
{ int i = 0;
  char s[12];
   do
  { s[i++] = number % 10 + '0';
  }
  while ((number = number / 10) > 0);
  s[i] = '\0';
  for (int m = i; m > 0; m--)Char_to_PC_Basic(s[m - 1]);
  Char_to_PC_Basic(' ');}*/

/********************************************************************************************************************/
