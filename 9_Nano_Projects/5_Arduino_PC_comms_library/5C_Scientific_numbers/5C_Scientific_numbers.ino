

#include "Scientific_numbers_header.h"

#define message_1 "\r\nThis program calculates roots iteratively.\r\n\
Enter scientific number then integer root?\r\n"
#define message_2 "\r\n\r\nEnter new data?\r\n"

#define BL 30                                     //Buffer length


int main (void) 

{float result, product_mem;
int root;
float start_value, product;
float Num, Num_bkp;
float inc;
int Tens_expt;
float Tens_multiplier;
float test;

char Num_string[BL + 2];

setup_HW_Arduino;

switch(reset_status){
  case POR_reset:                 User_prompt_A;    Serial.write(message_1);break;
  case WDT_reset:                 Serial.write(message_2);break;
 case External_reset:            Serial.write(message_1);break;}

//Serial.write("Enter scientific number?\t");

Num = Sc_Num_from_PC_A(Num_string, BL);           //User enters the scientific number
Sc_Num_to_PC_A(Num, 1, 5, '\t');

//Serial.write("Root (Integer)?\t");
root = Sc_Num_from_PC_A(Num_string, BL); 
Int_Num_to_PC_A(root, Num_string, '\r');


test = 1.0;
for(int p = 0; p < root; p++)test *= 10.0;
Tens_expt = 0;
Tens_multiplier = 1.0;
Num_bkp = Num;
if (Num_bkp > test){while(Num > test){Num /= test; Tens_expt += root;} 
for(int m = 0; m < Tens_expt; m += root)Tens_multiplier *= 10.0;}

newline_A();
Serial.print(Num,6); Serial.write('\t');Serial.print(Tens_expt);
newline_A();newline_A();


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

Serial.print(result,6); 
Serial.write(" ");Serial.print(product_mem,6);

if (product_mem > Num)result = 1.0;


for(int p = 1; p <= 6; p++)
{inc /= 10.0;
product = 1.0;
start_value = result + inc;
while(1){
  for(int m = 0; m< root; m++)product = product * start_value;
if (product <= Num){result = start_value; product_mem = product;}
else break;
start_value += inc;
product = 1.0;}

Serial.write("\r\n");Serial.print(result,6); 
Serial.write(" ");Serial.print(product_mem,5);
_delay_ms(50);}

result = result * Tens_multiplier;
Serial.write("\r\n");Serial.write("\r\n");
Sc_Num_to_PC_A(result, 2, 5, '\r');


SW_reset;
}
