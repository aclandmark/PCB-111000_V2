
/*
The start of an investigation into the operation of floating point numbers

Subroutine "Scientific_number_from_KBD_Local()" is specific to this project and is probably not worth much attention

Subroutines "unpack_FPN()" and "Assemble_FPN()" are stored in "Resources_nano_projects\Subroutines\FPN_DIY_IO.c"
They illustrate a method that could be used to process FPN's.  But the C library functions work much faster than those presented 
with these projects. Library functions will have been honed to make them as efficient as possible. These are just for fun.


This project takes a scientific number. Unpacks it giving a long number and a twos exponent
It then reassembles it and prints the FPN and original decimal number.
Note the small discrepancy due to rounding
See 
https://www.h-schmidt.net/FloatConverter/IEEE754.html
to chreck the results


*/



#include "9A_header.h"

#define Buff_Length  20

int main (void){


float FPN;
char sign;
int twos_expnt;
long RHS_of_binary_point;
char digits[Buff_Length + 2];
float dividend;


setup_HW_with_reset_analysis;

if(reset_status == 2){Serial.write("?");reset_status = 0;}
else 
{Serial.write("\r\nEnter scientific number \
& terminate with Return key.\r\nThen press AK to repeat.\r\n");
Serial.write("Exponent and long number used for arithmetic\tResulting \
floating point representation\t\tNumber restored\r\n?");}

  FPN = Scientific_number_from_KBD_Local(digits, &sign, Buff_Length);
  RHS_of_binary_point = unpack_FPN(FPN, &twos_expnt, &sign);

Serial.print(twos_expnt); Serial.write("  \t");
Print_RHS_of_binary_point(RHS_of_binary_point);

Serial.write("  \t ");Print_long_as_binary(*(long*)&FPN);
Serial.write("  \t");
FPN = Assemble_FPN(RHS_of_binary_point, twos_expnt, sign);

Sc_Num_to_PC_A(FPN, 1, 6, '\r');
display_FPN_short(FPN, digits);
waitforkeypress_A();
SW_reset;}



/**********************************************************************************************************************/
float Scientific_number_from_KBD_Local(char * digits, char *sign, int BL){

long Significand;
long  twos_denominator;
char   tens_expnt;
int twos_expnt;
long FPN_digits;
float FPN;
char sign_local;


Significand = Get_fpn_from_KBD
(digits, &twos_expnt, &tens_expnt, &twos_denominator, &sign_local, BL);              //Can be positive or negative
FPN_digits = Fraction_to_Binary_Signed(Significand, twos_denominator);               //0.1011000011.... for example
if (!(FPN_digits)) return 0;
else
{FPN = Assemble_FPN(FPN_digits, twos_expnt, sign_local);
FPN = Scientifc_num_to_FPN(FPN, tens_expnt);
*sign = sign_local;
Check_num_for_to_big_or_small(FPN);
return FPN;}}


/***********************************************************************************************************************/
void Print_long_as_binary(long L_num){

for(int m = 0; m <= 31; m++){if((m == 1) || (m== 9))Serial.write(' ');
if (L_num  &  ((unsigned long)0x80000000 >> m))Serial.write('1'); 
else Serial.write('0');}}



/***********************************************************************************************************************/
void Print_char_as_binary(unsigned char *num){
  for (int m = 0; m <=7; m++){
  if (*num & ((unsigned char) 0x80 >> m))Serial.write('1'); 
  else Serial.write('0');}Serial.write(' ');}




/*************************************************************************************************************************/
void Print_RHS_of_binary_point(long L_num){

for(int m = 1; m <= 24; m++){if(m==2)Serial.write(' ');
if (L_num  &  ((unsigned long)0x80000000 >> m))Serial.write('1'); 
else Serial.write('0');}}




/***********************************************************************************************************************************/
