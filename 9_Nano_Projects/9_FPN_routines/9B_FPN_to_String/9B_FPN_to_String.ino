/*******************************************/

#include "9B_header.h"

#define Buff_Length  20

int main (void){

float FPN, FPN_bkp;
char sign;
char Digits_before_dp, Digits_after_dp;
char print_string[Buff_Length + 2];

setup_HW_with_reset_analysis;

if(reset_status == 2){Serial.write("?");reset_status = 0;}
else
{Serial.write("\r\nEnter scientific number \
& terminate with Return key.\r\n\
Then key in num digits before/after the decimal point.\r\nThen press AK to repeat\r\n?");}

//while(1){

FPN = Scientific_number_from_KBD(print_string, &sign,Buff_Length );
Sc_Num_to_PC_A(FPN, 1, 6, '\t');
Print_long_as_binary(*(long*)&FPN);
Serial.write(" \t ");

Digits_before_dp = waitforkeypress_A() - '0'; 
Digits_after_dp = waitforkeypress_A() - '0' ; 

FPN_to_String_Local(FPN, Digits_before_dp, Digits_after_dp, '\r', print_string);

Serial.write (print_string);//}
display_FPN_short(FPN, print_string);
waitforkeypress_A();
SW_reset;}



/*********************************************************************************************************************/
void FPN_to_String_Local(float FPN, char pre_dp, char post_dp, char next_char, char * print_string){

char tens_expnt, print_expnt;
int twos_expnt;
char sign;
long FPN_as_long, FPN_as_long_bkp;
long Denominator;
long int_part_max;
int Num_digits;
float round_denom;
float FPN_bkp;

if ((*(long*)(&FPN) == 0x80000000) || (*(long*)(&FPN) == 0))
{print_string[0] = '0'; print_string[1] = '.';print_string[2] = '0';
print_string[3] ='\r'; print_string[4] ='\n';print_string[5] = '\0';return;}          //+/- zero case

Num_digits = pre_dp + post_dp; 

if (*(long*)&FPN & (unsigned long) 0x80000000)
{(*(long*)&FPN &= 0x7FFFFFFF);sign = '-';} else sign = '+';       //determine sign and convert -ve numbers to +ve
tens_expnt = 0;
int_part_max = 1;
for(int m = 0; m < pre_dp; m++) int_part_max *= 10.0;             //for 3 digits before the dp the max integer part is set to 1000

FPN_bkp = FPN;

if(FPN_bkp >= (float)int_part_max){                               //For numbers above the max integer value
while  (FPN >= (float)int_part_max)
{FPN /= 10.0; tens_expnt += 1;}                                   //Divide FPN by 10 until its integer part is below the max allowed value
print_expnt = tens_expnt;}                                        //Increment the tens exponent accordinglly and save the result for printing

if(FPN_bkp < (float)int_part_max){                                //Repeat for numbers below the max integer valur
while  (FPN < (float)int_part_max)
{FPN *= 10.0; tens_expnt -= 1;}
print_expnt = tens_expnt+1;}

while (FPN >= 1.0){FPN /= 10.0; tens_expnt += 1;}                  //Convert to the form 0.123456789  (>= 0.1 to < 1)



/*****************************************Build the number 0.000000005 used for rounding*********************************/  
round_denom = 1.0;
for(int m = 0; m <= Num_digits; m++)round_denom *= 10.0; 
FPN = FPN + (5.0/round_denom);

if(FPN >= 1.0){FPN /= 10.0;tens_expnt += 1; 
print_expnt = tens_expnt-pre_dp;}                                   //Only runs if rounding increases FPN to 1


/*****************************************Obtain the number 12345678 in binary form***************************************/
if (sign == '-')  *(long*)& FPN |= (unsigned long) 0x80000000;
FPN_as_long = unpack_FPN(FPN, &twos_expnt, &sign);                  //for FPN = 0.1 2's exponent is -3 and for 0.999 it is 0
FPN_as_long = FPN_as_long >> 4 ;                                    //Occupies bits 0 to 27
Denominator = 0x8000000 << (-twos_expnt);                           //2^(27 - 2's exponent) 


/****************************************Convert 12345678 to string form***************************************************/
{int p = 0;
Denominator /= 10.0; 

if(sign == '-')                                                     //Start building string for -ve numbers and those with 0.
{print_string[0] = '-'; p += 1;}
if (!(pre_dp))print_string[p++] = '0'; 
for (int m = 0; m < Num_digits; m++){
FPN_as_long_bkp = FPN_as_long/Denominator;                          //Dividing FPN_as_long by the denominator isolates the most significant digit

if(m == pre_dp)print_string[p++] = '.';                             //Add a dp is encountered add it to the string
if(FPN_as_long_bkp){print_string[p] = (FPN_as_long_bkp + '0');      //Save digit as a char 
FPN_as_long = FPN_as_long%Denominator;}                             //Remainder of dividing FPN_as_long by the denominator
else print_string[p] = '0'; 
p += 1;
FPN_as_long *= 10;}                                                 //Prepare to isolate the next most significant digit

if(print_expnt) {print_string[p++] = 'E';                           //If there is an exponent add 'E' to the string
itoa(print_expnt, print_string+p, 10);}                             //Convert print_exponent to askii and add them to the string 
else print_string[p++] = '\0';

p = 0;                                                              //Locate the null char and replace it with next_char
while (print_string[p++]); p -= 1;
print_string[p++] = next_char;                                      //Reinstate the null char
print_string[p] = '\0';}}



/*********************************************************************************************************************************/
void Print_long_as_binary(long L_num){

for(int m = 0; m <=31; m++){if((m == 1) || (m== 9))Serial.write(' ');
if (L_num  &  ((unsigned long)0x80000000 >> m))Serial.write('1'); else Serial.write('0');}}



/**************************************************************************************************************************************/
