
/*Proj_2C_2_Hex_numbers
***********************************************************/

/*QUICK INTRODUCTION TO HEX NUMBERS


IT INTRODUCES


1.  Hex number format.

2.  Project subroutine  "Binary_to_PC_local()".*/



#include "Proj_2C_2_header_file.h"

char digits [12];

int main (void){
int Num;
int Inc;
unsigned char digit_array[4], radix = 16;

setup_HW;

  String_to_PC_Basic("Enter positive number and increment then AK or x\r\n");  
    Num = Int_from_PC_Basic(digits);
    Int_to_PC_local(10, Num);
    Inc = Int_from_PC_Basic(digits);
  
  Int_to_PC_local(10, Inc);
  newline_Basic();
    
do{ 
   Int_to_PC_local(10, Num);
  Char_to_PC_Basic('\t');
 Int_to_PC_local(radix, Num);Char_to_PC_Basic('\t');     //Send the number to the pc with a radix of 16.
  Binary_to_PC_local(Num);              //Send the number to the pc in binary form
  String_to_PC_Basic("\r\n");
  Num= Num + Inc;} while(waitforkeypress_Basic() != 'x'); //increment the number
  SW_reset;}
  
  

/**************************************************************************************************/
/*void NumericString_to_PC_local(unsigned char* s){
int n=0;
while (s[n] != '\0')n++;                //scan through the string until the '\0' is found
for(int m = n; m; m--)Char_to_PC_Basic(*(s + m-1));     //send characters to the pc finishing with s[0]
Timer_T0_10mS_delay_x_m(4);}              //rather than starting with it
*/

/**************************************************************************************************/
/*void Local_BtoA(long num,unsigned char digit_array[]){     //Local function only: check with paper and pencil
int m=0;
do  {digit_array[m++] = (num%10) + '0';}
while((num = num/10) > 0);
digit_array[m] = 0;}



/**************************************************************************************************/
void Binary_to_PC_local(int x){             //check with paper and pencil
for (int m = 0; m<16; m++){
  if(x & (1 << (15-m))) Char_to_PC_Basic('1');        //test each bit in turn starting with the most significant
  else Char_to_PC_Basic('0');   
  if(m%4==3)String_to_PC_Basic("  ");}}

/*******************************************************************************************************/
void Int_to_PC_local (char radix, int number)
{ int i = 0;
 char sign = '+';
  unsigned char s[12];
unsigned int num;
  
if ((number < 0) && (radix == 10)){sign = '-'; number *= -1;}
 //if ((number < 0) && (radix == 16)){number *= -1; sign = '-';}
  
   num = (unsigned)number;  
   do
  { s[i] = num % radix;// + '0';
if (s[i] < 10)s[i] += '0'; else s[i] += '7';
   i += 1; 
  }
  while ((num = num / radix) > 0);
  s[i] = '\0';
  
if ((sign == '-') && (radix == 10)){Char_to_PC_Basic('-');}
 //if ((sign == '-') && (radix == 16))number *= -1; 

//if (sign == '-')Char_to_PC_Basic('-');
  
  for (int m = i; m > 0; m--)Char_to_PC_Basic(s[m - 1]);
  Char_to_PC_Basic(' ');
}
