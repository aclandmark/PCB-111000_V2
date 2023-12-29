

#include "Scientific_numbers_header.h"

#define message_1 "\r\nEnter_number and root (in integer form).\r\n"
#define message_2 "\r\n\r\nTime_out: Number too large or small. Try again!\r\n"

#define BL 30                                     //Buffer length


int main (void) 

{float result, result_2;
int root = 2;
float start_value = 2, temp_value = 1.0;
float Num = 100.0;
float inc = 1.0;

char Num_string[BL + 2];

setup_HW_Arduino;

switch(reset_status){
  case POR_reset:                 User_prompt_A;    Serial.write(message_1);break;
  case WDT_reset:                 Serial.write(message_1);break;
 case External_reset:            Serial.write(message_1);break;}

Num = Sc_Num_from_PC_A(Num_string, BL);           //User enters the scientific number
root = Sc_Num_from_PC_A(Num_string, BL); 

waitforkeypress_A();

while(1){
  for(int m = 0; m< root; m++)temp_value = temp_value * start_value;
  
if (temp_value <= Num){result = start_value; result_2 = temp_value;}//{Serial.print(iterative_value); Serial.write(" ");}
else break;
start_value += 1.0;
temp_value = 1.0;}

Serial.print(result,6); 
Serial.write(" ");Serial.print(result_2,6);

waitforkeypress_A();
if (result_2 > Num)result = 1.0;


for(int p = 1; p <= 6; p++)
{inc /= 10.0;
temp_value = 1.0;
start_value = result + inc;
while(1){
  for(int m = 0; m< root; m++)temp_value = temp_value * start_value;
if (temp_value <= Num){result = start_value; result_2 = temp_value;}
else break;
start_value += inc;
temp_value = 1.0;}

Serial.write("\r\n");Serial.print(result,6); 
Serial.write(" ");Serial.print(result_2,5);
waitforkeypress_A();}
SW_reset;
}
