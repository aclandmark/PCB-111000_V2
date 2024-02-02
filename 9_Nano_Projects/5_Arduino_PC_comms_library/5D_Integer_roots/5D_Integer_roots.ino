
/*
For numbers greater than or equal to 4 the square root can be found iteratively starting with a guess of 2.
We then refine the guess to include a first decimal place, then a second decimal place etc. 
For large numbers we initially divide them by 100 until they are in a suitable range then multiply the result by 10,
once for each division.  

This process can be refined for numbers below 4 and for any integer root.  For users waiting for a response at the PC
the delay is remarkably short.  
*/




#include "Integer_roots_header.h"

#define message_1 "\r\nThis program calculates roots iteratively.\r\n\
Enter scientific number then integer root?\r\n"
#define message_2 "\r\nEnter new data?\r\n"

#define BL 30                                     //Buffer length


int main (void) 

{float result, product_mem;
int root;
float start_value, product;
float Num, Num_bkp;
float inc;
int Tens_expt;
float Tens_multiplier;
float Ten_power_root;

char Num_string[BL + 2];

setup_HW_Arduino;

switch(reset_status){
  case POR_reset:                 User_prompt_A;    Serial.write(message_1);break;
  case WDT_reset:                 Serial.write(message_2);break;
 case External_reset:            Serial.write(message_1);break;}



Num = Sc_Num_from_PC_A(Num_string, BL);           //User enters the scientific number
Sc_Num_to_PC_A(Num, 1, 5, '\t');

root = Sc_Num_from_PC_A(Num_string, BL); 
Int_Num_to_PC_A(root, Num_string, '\t');

Num_bkp = Num;


/************************************Remove surplus ten's expontents*******************************************************************/
Ten_power_root = 1.0;
for(int p = 0; p < root; p++)Ten_power_root *= 10.0;
Tens_expt = 0;
Tens_multiplier = 1.0;

if (Num_bkp > Ten_power_root){while(Num > Ten_power_root){Num /= Ten_power_root; Tens_expt += root;} 
for(int m = 0; m < Tens_expt; m += root)Tens_multiplier *= 10.0;}

if (Num_bkp < 1.0)
{while(Num < 1.0){Num *= Ten_power_root; Tens_expt += root;} 
for(int m = 0; m < Tens_expt; m += root)Tens_multiplier /= 10.0;}


/****************************************Root to best integer value. Start iteration at 2***************************************/
inc = 1.0;
product = 1.0;
start_value = 2.0;
result = 1;
while(1){
  for(int m = 0; m< root; m++)product = product * start_value;
  
if (product <= Num){result = start_value; product_mem = product;}
else break;
start_value += inc;
product = 1.0;}

if (product_mem > Num)result = 1.0;                           //Initial value for numbers less than 4 


/*************************Improve iteration starting with one decimal place then 2, 3......up to 6 in all******************************/
for(int p = 1; p <= 6; p++)
{inc /= 10.0;
product = 1.0;
start_value = result + inc;
while(1){
  for(int m = 0; m< root; m++)product = product * start_value;
if (product <= Num){result = start_value; product_mem = product;}
else break;
start_value += inc;
product = 1.0;}}


result = result * Tens_multiplier;                                  //Factor in the surplus exponent
Sc_Num_to_PC_A(result, 2, 5, '\r');


SW_reset;}





/****************************************************************************************************************************************/
