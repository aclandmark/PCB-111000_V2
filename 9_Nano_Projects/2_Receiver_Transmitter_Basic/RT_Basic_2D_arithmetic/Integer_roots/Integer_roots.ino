

#include "Integer_roots_header.h"


#define message_1 "\r\nThis program calculates roots iteratively.\r\n\
Enter scientific number then integer root?\r\n"
#define message_2 "\r\nEnter new data?\r\n"


int main (void) 

{long result, result_old;
int root = 2;
long start_value, product;
long Num;
char Num_string[12];

setup_HW;

Num = Int_from_PC_Basic(Num_string);

/****************************************Root to best integer value. Start iteration at 2***************************************/
product = 1.0;
start_value = 2.0;
result = 1;

if((product = start_value * start_value) > Num)result = 1; 
else{
while(1){
product = start_value * start_value; 
if (product <= Num){result = start_value;} 
else break;
start_value += 1;
product = 1.0;}}
Int_to_PC_Basic(result); Char_to_PC_Basic('.');



/*************************Improve iteration starting with one decimal place then 2, 3......up to 6 in all******************************/
for(int p = 1; p <= 6; p++)
{Num *= 100;
  result *= 10;
  result_old = result;
product = 1.0;
start_value = result + 1;
while(1){
 product = start_value * start_value;
if (product <= Num){result = start_value;}
else break;
start_value += 1;
product = 1.0;}

Char_to_PC_Basic(start_value-result_old -1 + '0');}

newline_Basic();

SW_reset;}





/****************************************************************************************************************************************/
