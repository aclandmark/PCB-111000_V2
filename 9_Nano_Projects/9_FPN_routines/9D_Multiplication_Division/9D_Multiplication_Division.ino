/*******************************************
Division of floating point numbers

First off all unpack the FPNs generating the appropriate long numbers and exponents
Shift the numerator untill it is just less than the demoninator (probably 1 place at the most)
Subtract the exponents
Use subroutine "Fraction_to_Binary_Signed(A/B)" to generate a result of the 
form 0.1xxxxxxx where x will either be zero or 1.
Assemble the result and exponent into a new FPN

Multiplication of floating point numbers
Use the division subrouting twice, once to find the reciprocal of one of the numbers
and again to divide the other one by the reciprocal of the first.  
This method avoids the need to deal with overglows
 
 */

#include "9D_header.h"

#define Buff_Length  20

int main (void){

float FPN_1, FPN_2, Result;
char keypress;
char sign;
char num_as_string[Buff_Length + 2]; 


setup_HW_with_reset_analysis;
wdt_enable(WDTO_30MS);
while(switch_3_down)wdr();

if(reset_status == 2)
{reset_status = 0;
Serial.write("\r\nAccumulator reset. Enter new numbers.\r\n");}

else 

Serial.write("\r\nAccumulator:  Enter positive or negative scientific numbers \
& terminate with Return key.\r\nPress sw3 before sw1 to reset.\r\n");



FPN_1 = Scientific_number_from_KBD(num_as_string, &sign, Buff_Length);
FPN_to_String(FPN_1, 1, 6, '\t', num_as_string); 
Serial.write(num_as_string);  

while(1){Check_num_for_to_big_or_small(FPN_1);
Serial.write(" (x/?)\t");

while(1){keypress = waitforkeypress_A();
if ((keypress == 'x') || (keypress == '/'))break; else Serial.write('?');}
Serial.write(keypress);


FPN_2 = Scientific_number_from_KBD(num_as_string, &sign, Buff_Length);
FPN_to_String(FPN_2, 1, 6, ' ', num_as_string);
Serial.write(num_as_string);

Serial.write("= "); 
if (keypress == '/')Result = FPN_div_Local(FPN_1, FPN_2);
if (keypress == 'x')Result = FPN_mult_Local(FPN_1, FPN_2);

Serial.write('\t');
FPN_to_String(Result, 2, 4, '\r', num_as_string);
Serial.write(num_as_string);

I2C_FPN_to_display(Result);
while(switch_1_down)wdr();

FPN_1 = Result;
while(switch_3_down);}
SW_reset;}



/********************************************************************************************************************************/
float FPN_div_Local (float FPN_1, float FPN_2){

int twos_expnt_1, twos_expnt_2, twos_expnt_3;
long FPN_digits_1, FPN_digits_2, FPN_digits_3;
char sign_1, sign_2, sign_3 = '+';
float Result;
int FPN_shift;

sign_1 = '+';
sign_2 = '+';

FPN_digits_1 = unpack_FPN(FPN_1 , &twos_expnt_1, &sign_1);
FPN_digits_2 = unpack_FPN(FPN_2 , &twos_expnt_2, &sign_2);

while (FPN_digits_1 >= FPN_digits_2)
{FPN_digits_1 >>= 1; twos_expnt_1 += 1;}
FPN_digits_3 = Fraction_to_Binary_Signed(FPN_digits_1, FPN_digits_2);

if (sign_1 == sign_2)sign_3 = '+'; else sign_3 = '-';
twos_expnt_3 = twos_expnt_1 - twos_expnt_2;

if(twos_expnt_3 >= 127)
{Serial.write("Result too large for 32bit numbers\r\n");
SW_reset;}

Result = Assemble_FPN((unsigned long) 
FPN_digits_3, twos_expnt_3,  sign_3);
if((!(*(long*)&Result)) || (*(long*)&Result == 0x80000000)) 
{ Serial.write("Zero"); SW_reset;}

return Result;}



/********************************************************************************************************************************/
float FPN_mult_Local(float FPN_1, float FPN_2){
float Reciprocal;
float Result;
  Reciprocal = FPN_div_Local(1.0, FPN_2);
 Result = FPN_div_Local(FPN_1, Reciprocal);
return Result;}



/*********************************************************************************************************************************/
void Print_long_as_binary(long L_num){

for(int m = 0; m <=31; m++){if((m == 1) || (m== 9))Serial.write(' ');
if (L_num  &  ((unsigned long)0x80000000 >> m))Serial.write('1'); else Serial.write('0');}}



/**************************************************************************************************************************************/
