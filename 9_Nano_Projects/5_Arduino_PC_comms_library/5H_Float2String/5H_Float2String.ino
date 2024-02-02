
/*************Explores the operation of the Serial.print function********************

In the past difficuly could be had converting foating point numbers to strings unless space 
was available for a particular floating point library.
Arduino has done away with this problem.

For me a life line was provided by the following link
https://www.geeksforgeeks.org/convert-floating-point-number-string/

This project looks at what is going on underneath the various subroutins calls.  Floating point
implementation is introduced, this will be covered in more detail latrer on.

************************************************************************************************/



#include "Float2String_header.h"

#define Buff_Length  20
#define PIE 3.14159
#define message_1   "\r\nEnter scientific number & terminate with Return key (+ve numbers only and >=1.0!).\r\n\
Press SW2 to pause the display. SW1 to scroll through the results.\r\n"

#define message_2     "\n\rAnother number?\r\n"


int main (void){
float Num_1;                                                    //Number entered at keyboard
char ndp;                                                       //Number of decimal places
char counter;                                                   //Controls printing
char N_strg[Buff_Length + 2];                                    //Array for numbers as strings
char digits[8];                                                   //arrray to drive display


setup_HW_Arduino;

switch(reset_status){
  case POR_reset:                 User_prompt_A;    Serial.write(message_1);break;
  case WDT_reset:                 Serial.write(message_2);break;
  case External_reset:            Serial.write(message_1);break;}
 

Num_1 = FPN_KBD_to_display_A(N_strg, Buff_Length); 
Serial.print(Num_1,5);Serial.write("\t\t");
I2C_FPN_to_display(Num_1);
while(switch_1_down);

Serial.write ("\r\nNumber of decimal places?\r\n");

counter = 0;
while(1){
while(1){ndp = waitforkeypress_A();
if ((ndp >= '0') && (ndp <= '9') || (ndp == 'x'))break; 
else Serial.write("? ");}
if(ndp == 'x')break;
Serial.write(ndp);Serial.write("\t");
ndp -= '0';

Float_to_string(Num_1, N_strg, ndp);
Serial.write(N_strg);

{int m = 0;                                                                     //Copy number sent to PC to the display
for(int p = 0; p <= 7; p++)digits[p] = 0;
while(N_strg[m])m+=1;  m -= 1;
if (m >7)m = 7;
for(int p = 0; p <= m; p++)digits[p] = N_strg[m-p];
I2C_Tx_8_byte_array(digits);}


if(!(counter))Serial.write ("\tndp? or 'x' to escape");
newline_A();
counter += 1;}

SW_reset;}



/**************************************************************************************************************************************/
void Float_to_string(float Num_1, char * N_strg, char ndp){
long ipart;                                           //Integer part
float fpart;                                          //Float part
int array_ptr;                                         //controls data entry to N_strg
int zero_counter;
long decimal_part;

ipart = Float2Long(Num_1);                            //ipart = (long)Num_1;                ALTERNATIVE using C library function
fpart = Num_1 - Int2Float(ipart);                     //fpart = Num_1 - (float)ipart;       ALTERNATIVE using C library function

if(fpart== 0.0)
{Int_to_string(ipart, N_strg, 0);return;}             //Exit early for integer numbers

if(fpart < 0.1){zero_counter = 1;}                    //Count the number of leading zeros following the decimal point 
else zero_counter = 0;
for(int m = 0; m <= ndp; m++)
{if((fpart = fpart*10.0)<0.1){zero_counter += 1;}}     //Convert RHS of decimal point to an integer

decimal_part = Float2Long(fpart);                       //decimal_part = (long)fpart;     ALTERNATIVE using C library function
decimal_part = decimal_part + 5;
decimal_part/=10;                                       //Rounding
switch(decimal_part)
{case 1: case 10: case 100: case 1000: case 10000:      //Additional rounding for specal cases
if(zero_counter)zero_counter -= 1;
else {ipart += 1;decimal_part = 0;}break;
default: break;}

if (!(decimal_part))zero_counter = 0;                   //Remove spurious leading zeros

array_ptr = Int_to_string(ipart,N_strg,0);              //Determine entry point in string for decimal N_strg
N_strg[array_ptr] = '.'; N_strg[array_ptr + 1] = 0;
if (zero_counter)
{while(zero_counter>0){N_strg[array_ptr + 1] = '0'; 
zero_counter -= 1;array_ptr +=1;}}
N_strg[array_ptr + 1] = 0;

Int_to_string(decimal_part, N_strg, array_ptr+1);}



/***********************************************************************************************************************/
float Int2Float(long num){
int m;
long twos_expt;

for(m = 0; m <= 31; m++)                                        //Scan long number from bit 31 stoping at the most significant '1' (the MSB)
{if(num & ((unsigned long)0x80000000 >> m))break;}
m = 31 - m;
if (m < 23)num = num << (23 - m);                               //Shift the long number so that the MSB is at location 23
if (m > 23)num = num >> (m - 23); 
twos_expt = m - 1 + 127;                                       //The twos exponent is the number of significant bits plus 127
twos_expt = twos_expt << 23;                                    //shift the twos exponent so that it occupies bits 23 to 30.
num = num + twos_expt;                                          //Add the exponent into long number. Note bit 23 which is always '1' is overwritten
return (*(float*)(&num));}                                      //Tell the compiler that the long number is now to be treated as a floating point number



/***********************************************************************************************************************/
long Float2Long(float num){
long twos_expt;
long L_num;

twos_expt = (*(long*)&num);                                   //Process the floating point number and isolate the twps exponent (bits 23 to 30) 
twos_expt = twos_expt << 1;
twos_expt = (unsigned long)twos_expt >> 24;
twos_expt -= 127;                                             //Sbtract the 127 added during the reverse process carried out in subroutine "float Int2Float(long num)"

L_num = (*(long*)(&num));                                     //Isolate the integer part
L_num =  ((unsigned long)0x80000000 >> 8) | L_num;            //Restore bit 23 to a '1'
L_num = L_num << 8;                                           //Ensure that bit 31 is zero
L_num = (unsigned long)L_num >> (31 - twos_expt);             //Shift the number so that the MSB is in the correct location
return L_num;}



/***********************************************************************************************************************/
int Int_to_string (long number, char* s, int i)
{ char dig;
int start;

start = i;                                                //s is the address of array s[],  "i" is the location in the array where the string is to be stored            
  
 while(1){
 dig = number % 10 + '0'; s[start] = dig; i+= 1;
  if(!(number = number / 10)>0)break;
  for(int m = i; m>start; m--)s[m] = s[m-1];}
  s[i] = '\0';
  return i;}



/***********************************************************************************************************************/
void Long_to_PC(long num){
for(int m = 0; m <= 31; m++){if(num & ((unsigned long)0x80000000 >> m)) 
Serial.write('1'); else Serial.write('0');}}





/****************************************************************************************************************/
