
/*Proj_3G_Char_nos
***********************************************************/

/*USES THE DISPLAY TO EXPLORE THE OPERATION OF BINARY NUMBERS



IT INTRODUCES



1.  Binary notation for unsigned integers from 0 to 255 and signed integers between -128 and +127.

2.  Project subroutine "I2C_Tx_display_char(digit, mode)" where digit is any binary number between 0 and 11111111 
and mode is either "u" for unsigned or "s" for signed.


OPERATION

The 4 LHS digits of the display are used to display the binary number and the 4 RHS digits for the decimal number.
Press sw1 to increment the binary number or sw3 to decrement it.
sw2 toggles the display between "signed" and "unsigned" numbers.

Switch location on PCB:  SW3 - SW2 - SW1

Note: 
This project has been compiled with Optimisation level 1 in an attempt to minimise occaisional unstable operation.*/




#include "Proj_2_B1_header_file.h"




char digit=1;
 char op, mode;

int main (void){

op=0;
mode = 'u';                         //mode: signed or unsigned
setup_HW;
I2C_Tx_display_char(digit,mode);update_screen();


while(1){
if(isCharavailable_Basic(1))op = Char_from_PC_Basic();
  
switch(op){
case 0: break;                        //Static display
case 'x': I2C_Tx_display_char(++digit,mode);update_screen(); break;
case 'y': I2C_Tx_display_char(--digit, mode);update_screen();break; 
case 'X': digit += 4;I2C_Tx_display_char(digit,mode);update_screen(); break;
case 'Y': digit -= 4;I2C_Tx_display_char(digit,mode);update_screen(); break;
case 'z':
case 'Z': if(mode == 'u')mode = 's'; else mode = 'u';
I2C_Tx_display_char(digit,mode);update_screen();break;
default: String_to_PC_Basic("\r\nStarting again.\r\n");SW_reset;break;}
Timer_T0_10mS_delay_x_m(15);
op = 0;}}


void Short_Int_to_PC_Basic (char radix, char number, char mode)
{ int i = 0;
   char s[12];
   char sign = '+';
   unsigned char num;
   
    num = (byte)number;
   if(radix == 10){  if ((mode == 's') && (number < 0)){num = -number; sign = '-';}}
   do
  { s[i] = num % radix;// + '0';
if (s[i] < 10)s[i] += '0'; else s[i] += '7';
   i += 1; 
  }
  while ((num = num / radix) > 0);
  s[i] = '\0';
  if (sign == '-')Char_to_PC_Basic('-');
  for (int m = i; m > 0; m--)Char_to_PC_Basic(s[m - 1]);
  Char_to_PC_Basic(' ');
}



void update_screen(void){

Short_Int_to_PC_Basic (10,digit, mode);
Char_to_PC_Basic ('\t');  
Short_Int_to_PC_Basic (16,digit, mode);
Char_to_PC_Basic ('\t');
Short_binary_to_PC_local(digit);

if(mode == 's'){
Char_to_PC_Basic ('\t');
Short_binary_to_PC_local((~digit) + 1);
Char_to_PC_Basic ('\t');
Short_Int_to_PC_Basic (16, (~digit)+1, + mode);
Char_to_PC_Basic ('\t');
Short_Int_to_PC_Basic (10, (~digit)+1, + mode);}

newline_Basic();}


void Short_binary_to_PC_local(int x){             //check with paper and pencil
for (int m = 0; m<8; m++){
  if(x & (1 << (7-m))) Char_to_PC_Basic('1');        //test each bit in turn starting with the most significant
  else Char_to_PC_Basic('0');   
  if(m%4==3)String_to_PC_Basic("  ");}}





/**************************************************************************************************/
