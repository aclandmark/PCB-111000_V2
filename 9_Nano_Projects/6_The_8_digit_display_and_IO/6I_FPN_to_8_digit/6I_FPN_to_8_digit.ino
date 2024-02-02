
/*
Several subroutines have been developed to display floating point numbers on the 8 digit display
This one limits the number of characters to 8 so that the result always fits on the display

See Resources_nano_projects\Subroutines\FPN_DIY_IO\ for the subroutine  "FPN_to_String"
*/



//Problem with 1.0 and negative exponents


#include "6I_header_file.h"

#define message_1 "Enter FPN then press sw1 then AK twice\r\n//\tFPN and Product\r\n?\t"

#define message_2 "?\t"

#define message_3 "\r\nWDTout with interrupt occurred\r\n\
A wdr() statement is probably needed some where.\r\n"

#define BL  20                                      //Buffer length


float Num_1, Num_2;
char digits_12[BL + 2];                               //Array used to drive the display
char Num_as_string[12];
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
 
Num_1 = Scientific_number_from_KBD(digits, &sign, BL);
I2C_FPN_to_display(Num_1);
while(switch_1_up)wdr();
while(switch_1_down)wdr();
Serial.print(Num_1,4);
display_FPN_short_Local(Num_1, digits_12);
waitforkeypress_A();
Num_1 *= 11.0;
Serial.write("\t\t");Serial.print(Num_1,4);
display_FPN_short(Num_1, digits_12);
waitforkeypress_A();
newline_A();
SW_reset;}


/*****************************************************************************************************************/
void display_FPN_short_Local(float FPN, char * digits_2){    //Problem with 1.0 and negative exponents
char digits[8],sign, range;

for(int m = 0; m <=7; m++)digits[m] = 0;
if (FPN < 0) {sign = '-'; FPN *= -1.0;} else sign = '+';

if (FPN < 1.0e-9)range = 2;
if((FPN >= 1.0e-9 ) && (FPN < 1.0))range = 3;
if ((FPN >= 1.0) && (FPN < 10.0))range = 6;
if ((FPN >= 10.0) && (FPN < 1.0e10))range = 4;
if (FPN >= 1.0e10)range = 3;

if (sign == '-') {range -= 1;FPN *= -1.0;FPN_to_String(FPN, 1, range,'\0',digits_2); }

else FPN_to_String((FPN), 1, range,'\0',digits_2);

{int m = 0; while(digits_2[m]) {digits[7-m] = digits_2[m]; m += 1;}} 

I2C_Tx_8_byte_array(digits);}




/****************************************************************************************************************/
ISR (WDT_vect){eeprom_write_byte((uint8_t*)0x1FC, 0x01); while(1);}



/********************************************************************************************************************/
