
/**************Proj_6E_Acquire_and process_FPN_Data_from_the KBD********************/

/*Enter +ve or -ve numbers at the keyboard.  
 * 
 * For +ve numbers 
 * PRINTS column of numbers each one the square root of the previous
 * until within 0.005 of unity then continues but calculates the squares
 * 
 * For -ve numbers prints powers of 3 or 0.33333
 * 
 * Introduces subroutine subroutine "float FPN_KBD_to_display(char *)"
 * This is a development of subroutine "long Int_KBD_to_display(char *)"
 * It allows for decimal points, E and the backspace key
 * The library function "atof" is used to generate the binary form rather than the mini-OS
 * 
*/

/*
EEPROM usage
0x1FF, 0x1FE and 0x1FD     OSCCAL
0x1FC and 0x1FB            PRN generator
0x1FA                      Reset source
*/


#include "FPN_KBD_to_display_header.h"
#define Buff_Length  20

#define message_1 "\r\nEnter scientific number & terminate with Return key.\r\n\
Press SW2 to pause the display.  SW1 to scroll through the results.\r\n"

#define message_2 "\r\nNew number?\r\n"

#define message_3 "\r\nWDTout with interrupt occurred\r\n\
A wdr() statement is probably needed some where.\r\n"


int main (void){
float Num_1;
float power = 0.5;
char digits[Buff_Length + 2];
char counter = 0;

setup_HW_Arduino;

One_25ms_WDT_with_interrupt;

switch(reset_status){
  case POR_reset:             User_prompt_A;    Serial.write(message_1);break;
  case WDT_reset:             Serial.write(message_2);break;
  case WDT_reset_with_flag:   Serial.write(message_2);break;
  case External_reset:        Serial.write(message_1);break;
  case WDT_with_ISR_reset:    Serial.write(message_3);_delay_ms(25);cli();setup_watchdog_A;while(1);break;}

Num_1 = FPN_KBD_to_display_A_Local(digits, Buff_Length);

if(Num_1 > 0.0)power = 0.5;
else power = 3.0;

while (1){
Int_Num_to_PC_A(counter,digits, '\t'); 
Sc_Num_to_PC_A(Num_1,1,6 ,'\r'); 

if (power == 0.5)
{if (!(counter%5)){I2C_FPN_to_display(Num_1);
while(switch_1_down)wdr();}

if ((Num_1 < (1 + 5.0e-3)) && (Num_1 > (1 - 5.0e-3)))break;}

if (power == 3.0)
{if ((Num_1 < -1.0E20) || (Num_1 > -1.0e-20)){counter = 15; Num_1 *= -1.0; break;}
  I2C_FPN_to_display(Num_1);
while(switch_1_down)wdr();}

Num_1 = pow(Num_1, power);  counter += 1;}                                //Do some arithmetic

power = 1.0/power;
do{
Int_Num_to_PC_A(counter,digits, '\t'); 
Sc_Num_to_PC_A(Num_1,1,6 ,'\r');                                            //Send number to PC

if (!(counter%5)){I2C_FPN_to_display(Num_1);
while(switch_1_down)wdr();}

Num_1 = pow(Num_1, power);  counter -= 1; }                               //Do some arithmetic
while(counter+1);

SW_reset;}



/***********************************************************************************************************************/
float FPN_KBD_to_display_A_Local(char digits[], int BL){      

float num_1 = 0;
 char sign = '+';

FPN_string_KBD_to_display_A_Local(digits, BL); 

if (digits[0]== '-'){for (int m = 0; m < BL - 1; m++)digits[m] =  digits[m + 1];
sign = '-';}
num_1 = atof(digits);

if (sign == '-') num_1 = num_1 * (-1);
return num_1;}



/********************************************************************************************************************************************/
void FPN_string_KBD_to_display_A_Local(char display_buffer[], int BL){    //Operation is similar to that of Int_KBD_to_display()
char keypress;

for(int n = 0; n < BL; n++) display_buffer[n] = 0;   

while(1){                                                            //Remain in loop until a valid character is received
keypress = waitforkeypress_A();
if ((!(decimal_digit_A(keypress)))
&& (keypress != '-')
&& (keypress != '.'))continue;                                      //Ignore keypress if it is not OK
display_buffer[0] = keypress;
break;}

I2C_Tx_8_byte_array(display_buffer);                                //Update display with the first key press

while(1){
if ((keypress = wait_for_return_key_A())  =='\r')break;               //Stay in loop until return key press is detected

if (!(decimal_digit_A(keypress)) && (keypress != '.')                 //Check for valid keypresses
&& (keypress != '-') && (keypress != 'E') 
&& (keypress != 'e') &&  (keypress != '\b'))continue;

switch (keypress){
case '\b':  for (int n = 0; n < BL - 1; n++)                             //Backspace keypress
display_buffer[n] = display_buffer[n + 1];
display_buffer[BL - 1] = 0;
I2C_Tx_8_byte_array(display_buffer); break;

default:
for(int n = BL - 1; n>=1; n--)                                            //Shift display for each new keypress except '.'
display_buffer[n] = display_buffer[n-1];
display_buffer[0] = keypress;                                         //Add new keypress to display           
I2C_Tx_8_byte_array(display_buffer); break;}}

I2C_Tx_any_segment_clear_all();                     //Flash display
_delay_ms(25);wdr();_delay_ms(25);wdr();
_delay_ms(25);wdr();_delay_ms(25);wdr();
I2C_Tx_8_byte_array(display_buffer);

reverse (display_buffer);}



/****************************************************************************************************************/
ISR (WDT_vect){eeprom_write_byte((uint8_t*)0x1FA, 0x01); while(1);}





/****************************************************************************************************************/
