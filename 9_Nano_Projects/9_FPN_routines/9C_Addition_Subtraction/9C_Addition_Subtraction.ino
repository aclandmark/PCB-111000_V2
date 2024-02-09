/*******************************************
  Addition and subtraction.
  The two numbers are adjusted so that they have the same exponent
 
 ********************************************/

#include "9C_header.h"

#define Buff_Length  20

int main (void){

float FPN_1, FPN_2, Result;
char sign;
char num_as_string[Buff_Length + 2]; 


setup_HW_with_reset_analysis;

wdt_enable(WDTO_30MS);
while(switch_3_down)wdr();

if(reset_status == 2){Serial.write("\r\nAccumulator reset. Enter new numbers.\r\n");
reset_status = 0;}
else 
Serial.write("\r\nAccumulator:  Enter positive or negative scientific numbers \
& terminate with Return key.\r\nPress sw3 before sw1 to reset.\r\n");

FPN_1 = Scientific_number_from_KBD(num_as_string, &sign, Buff_Length);//while(1)wdr();
Sc_Num_to_PC_A(FPN_1, 1, 6, ' '); Serial.write(" + ");

while(1){
 FPN_2 = Scientific_number_from_KBD(num_as_string, &sign, Buff_Length);
 if (FPN_2 > 0)Serial.write('+');
Sc_Num_to_PC_A(FPN_2, 1, 6, ' '); Serial.write("= "); 

Result = FPN_add_Local(FPN_1, FPN_2);
FPN_to_String(Result, 1, 5, '\r', num_as_string);
Serial.write(num_as_string);

if ((*(long *)&Result)){I2C_FPN_to_display(Result); Serial.write("? ");}
else {I2C_Tx_long(0);while(switch_3_up)wdr(); while(switch_3_down);}
while(switch_1_down)wdr();
FPN_1 = Result;
while(switch_3_down);}

SW_reset;}



/********************************************************************************************************************************/
float FPN_add_Local (float FPN_1, float FPN_2){

int twos_expnt_1, twos_expnt_2, twos_expnt_3;
long FPN_digits_1, FPN_digits_2, FPN_digits_3;
char sign_1, sign_2, sign_3 = '+';
float Result;

sign_1 = '+';
sign_2 = '+';

FPN_digits_1 = unpack_FPN(FPN_1 , &twos_expnt_1, &sign_1);
FPN_digits_2 = unpack_FPN(FPN_2 , &twos_expnt_2, &sign_2);

if (sign_1 == '-')FPN_digits_1 = -FPN_digits_1;
if (sign_2 == '-')FPN_digits_2 = -FPN_digits_2;

twos_expnt_3 = twos_expnt_1;

if (twos_expnt_1 > twos_expnt_2){                                         //Divide FPN_digits_2 by 2 and increment twos_expnt_2 
  while(twos_expnt_1 > twos_expnt_2){twos_expnt_2 += 1;                   //untill twos_expnt_1 is equal to twos_expnt_2
  FPN_digits_2 = FPN_digits_2 >> 1;}twos_expnt_3 = twos_expnt_2;}         //Set twos_expnt_3 = twos_expnt_2 

if (twos_expnt_1 < twos_expnt_2){                                         //Repeat for twos_expnt_2 > twos_expnt_1
  while(twos_expnt_1 < twos_expnt_2){twos_expnt_1 += 1; 
  FPN_digits_1 = FPN_digits_1 >> 1;}twos_expnt_3 = twos_expnt_1;}

FPN_digits_3 = FPN_digits_1 + FPN_digits_2;                                 //Signs are included in this sum

if (sign_1 != sign_2)                                                       //Subtraction
{
    if (!(FPN_digits_3)){*(long*)(&Result) = 0x0; return Result;}           //zero result
  
 if (FPN_digits_3 & (unsigned long) 0x80000000)                              //negative result
 {FPN_digits_3 *= -1; sign_3 = '-';}                                         //Extract sign
while (!(FPN_digits_3 & (0x40000000)))                                       //Shift MSB to bit 30 and adjust exponent
{FPN_digits_3 = FPN_digits_3 << 1; twos_expnt_3 -= 1;}}

if(sign_1 == sign_2){                                                        //Both +ve or both -ve

if(twos_expnt_3 == 128){Serial.write("Infinity");SW_reset;}

switch(sign_1){
case '+': break;

case '-': 
FPN_digits_3 =    FPN_digits_1 * -1 + FPN_digits_2 * -1;                      //Both -ve: Repeat sum and set sign of result to -ve 
sign_3 = '-';break;}

if (FPN_digits_3 & (unsigned long) 0x80000000)                                //Overflow: shift right to vacate the sign bit (31)
{FPN_digits_3 = (unsigned long)FPN_digits_3  >> 1; twos_expnt_3 += 1;}}

Result = Assemble_FPN((unsigned long) FPN_digits_3, twos_expnt_3, sign_3);    //Assemble floating point number
if (sign_3 == '-')
*(long*)&Result = *(long*)&Result | (unsigned long)0x80000000;                  //re-instate the sign bit
return Result;}



/*********************************************************************************************************************************/
void Print_long_as_binary(long L_num){

for(int m = 0; m <=31; m++){if((m == 1) || (m== 9))Serial.write(' ');
if (L_num  &  ((unsigned long)0x80000000 >> m))Serial.write('1'); else Serial.write('0');}}



/**************************************************************************************************************************************/
