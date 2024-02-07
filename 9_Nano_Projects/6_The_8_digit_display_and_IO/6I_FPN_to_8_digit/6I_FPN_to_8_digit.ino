
/*
Several subroutines have been developed to display floating point numbers on the 8 digit display
This one limits the number of characters to 8 so that the result always fits on the display

See Resources_nano_projects\Subroutines\FPN_DIY_IO\ for the subroutine  "FPN_to_String"
*/



//Problem with 1.0 and negative exponents


#include "6I_header_file.h"

#define message_1 "Enter FPN then press sw1 then AK repetitively\r\n//\tFPN and Product\r\n?\t"

#define message_2 "?\t"

#define message_3 "\r\nWDTout with interrupt occurred\r\n\
A wdr() statement is probably needed some where.\r\n"

#define BL  20                                      //Buffer length


float Num_1;
float Num_2;
char Num_as_string[BL + 2];
char sign = '+';

int main (void){

setup_HW_Arduino;

One_25ms_WDT_with_interrupt;

switch(reset_status){
  case POR_reset:             User_prompt_A;    Serial.write(message_1);break;
  case WDT_reset:             Serial.write(message_2);break;
  case WDT_reset_with_flag:   Serial.write(message_1);break;
  case External_reset:        Serial.write(message_1);break;
  case WDT_with_ISR_reset:    Serial.write(message_3);_delay_ms(25);cli();setup_watchdog_A;while(1);break;}


Serial.write("Multiplier?");
Num_2 = Scientific_number_from_KBD(digits, &sign, BL);
Num_1 = Scientific_number_from_KBD(digits, &sign, BL);
I2C_FPN_to_display(Num_1);
while(switch_1_up)wdr();
while(switch_1_down)wdr();
Sc_Num_to_PC_A(Num_1, 1, 4, '\t');

display_FPN_short_Local(Num_1, Num_as_string);


while((waitforkeypress_A() != 'x'))

{Num_1 = Num_1 * Num_2;
newline_A();//Serial.write("\t\t");
Sc_Num_to_PC_A(Num_1, 1, 4, ' ');
display_FPN_short_Local(Num_1, Num_as_string);
//waitforkeypress_A();
}
SW_reset;}



/*****************************************************************************************************************/
void display_FPN_short_Local(float FPN, char * num_string){ 
char digits[8],sign, range;

for(int m = 0; m <=7; m++)digits[m] = 0;
if (FPN < 0) {sign = '-'; FPN *= -1.0;} else sign = '+';

if (FPN < 1.0e-9)range = 2;
if((FPN >= 1.0e-9 ) && (FPN < 1.0))range = 3;
if ((FPN >= 1.0) && (FPN < 10.0))range = 6;
if ((FPN >= 10.0) && (FPN < 1.0e10))range = 4;
if (FPN >= 1.0e10)range = 3;

if (sign == '-') {range -= 1;FPN *= -1.0;FPN_to_String(FPN, 1, range,'\0',num_string); }

else FPN_to_String((FPN), 1, range,'\0',num_string);
{int m = 0; while(num_string[m]) {digits[7-m] = num_string[m]; m += 1;}} 



/*********************Remove unwanted trailing zeros**************************************************/
{int p = 0;
while(p < 8){if (digits[p] == 'E') 
{p+=1;
if (digits[p] == '0'){for(int m = p; m < 7; m++)digits[m] = digits[m+1];digits[7]=0;}
p+=1;if (digits[p] == '0'){for(int m = p; m < 7; m++)digits[m] = digits[m+1];}
I2C_Tx_8_byte_array(digits);return;}

else p+=1;}

p=0;
while(p < 8){if (digits[p] == '.')break; else p+=1;}
if (p==8){I2C_Tx_8_byte_array(digits);return;} 

p=0;
while (digits[0] == '0'){for(int m = 0; m < 7; m++)digits[m] = digits[m+1];digits[7] = 0;} 
if(digits[0] == '.'){for(int m = 0; m < 7; m++)digits[7-m] = digits[6-m]; digits[0] = '0';}}

I2C_Tx_8_byte_array(digits);}


/****************************************************************************************************************/
ISR (WDT_vect){eeprom_write_byte((uint8_t*)0x1FC, 0x01); while(1);}



/********************************************************************************************************************/
