
/*
Several subroutines have been developed to display floating point numbers on the 8 digit display
This one limits the number of characters to 8 so that the result always fits on the display

Subroutine "FPN_to_String" is introduced in Project "9B_FPN_to_String" 
*/



#include "6I_header_file.h"

#define message_1 "Enter integer multiplier then FPN.\r\n\
Press sw1 then -m- or -d- repetitively, -x- to escape\r\n"

#define message_2 "\r\nNew_numbers?\r\n"

#define message_3 "\r\nWDTout with interrupt occurred\r\n\
A wdr() statement is probably needed some where.\r\n"

#define BL  20                                      //Buffer length


float Num_1;
float Num_2;
char Num_as_string[BL + 2];
char sign = '+';



int main (void){

char keypress;

setup_HW_Arduino;

One_25ms_WDT_with_interrupt;

switch(reset_status){
  case POR_reset:             User_prompt_A;    Serial.write(message_1);break;
  case WDT_reset:             Serial.write(message_2);break;
  case WDT_reset_with_flag:   Serial.write(message_1);break;
  case External_reset:        Serial.write(message_1);break;
  case WDT_with_ISR_reset:    Serial.write(message_3);_delay_ms(25);cli();setup_watchdog_A;while(1);break;}



Num_2 = Scientific_number_from_KBD(Num_as_string, &sign, BL);
Num_1 = Scientific_number_from_KBD(Num_as_string, &sign, BL);
I2C_FPN_to_display(Num_1);
while(switch_1_up)wdr();
while(switch_1_down)wdr();
Sc_Num_to_PC_A(Num_1, 1, 4, '\t');
display_FPN_short_Local(Num_1, Num_as_string);

while((keypress = waitforkeypress_A()) != 'x')
{if(keypress == 'm')Num_1 = Num_1 * Num_2;
else if(keypress == 'd')Num_1 = Num_1 / Num_2;
else {Serial.write('?');continue;}
newline_A();
Sc_Num_to_PC_A(Num_1, 1, 4, ' ');
display_FPN_short_Local(Num_1, Num_as_string);}
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
while (!(digits[0])){for(int m = 0; m < 7; m++)                                       //short string: remove trailing null terms 
digits[m] = digits[m+1];digits[7]=0;}
   
while(p < 8){if (digits[p] == 'E') 
{p+=1;
if (digits[p] == '0'){for(int m = p; m < 7; m++)digits[m] = digits[m+1];digits[7]=0;   //Remove up to two zeros proceeding 'E'
p+=1;if (digits[p] == '0'){for(int m = p; m < 7; m++)digits[m] = digits[m+1];}
p-=1;if (digits[p] == '.')
{for(int m = 7; m > p; m--)digits[m] = digits[m-1];digits[p] = '0';}}                    //Display .0E rather than .E

I2C_Tx_8_byte_array(digits);return;}

else p+=1;}

p=0;
while(p < 8){if (digits[p] == '.')break; else p+=1;}                                    //No E and no decimal point. Display the full array
if (p==8){I2C_Tx_8_byte_array(digits);return;} 

p=0;                                                                                    //No E but decimal point
while (digits[0] == '0'){for(int m = 0; m < 7; m++)
digits[m] = digits[m+1];digits[7] = 0;}                                                 //Remove trailing zeros 
if(digits[0] == '.'){for(int m = 0; m < 7; m++)
digits[7-m] = digits[6-m]; digits[0] = '0';}}                                           //Display .0E rather than .E

I2C_Tx_8_byte_array(digits);}


/****************************************************************************************************************/
ISR (WDT_vect){eeprom_write_byte((uint8_t*)0x1FC, 0x01); while(1);}



/****************************************************************************************************************/
