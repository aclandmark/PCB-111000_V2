/* Numbers from the keyboard try 67108864*/

#include "Receiver_Transmitter_header.h"

int main (void){   
  char num_string[12];
  long num;
  
setup_HW;
if(watch_dog_reset == 1)String_to_PC_Basic("\r\nAgain"); else
String_to_PC_Basic("Enter number at keyboard");
num = Int_KBD_to_display_A_Local(num_string);
waitforkeypress_Basic();
while(num){
num /= 2;
I2C_Tx_long(num);
waitforkeypress_Basic();}
SW_reset;
return 1;}




/************************************************************************/
long Int_KBD_to_display_A_Local(char display_buffer[]){                //Acquires an integer string from the keyboard and returns the binary equivalent
char keypress;
long Long_Num_from_mini_OS;

for(int n = 0; n<=8; n++) display_buffer[n] = 0;              

while(1){
if ((keypress = wait_for_return_key_Basic())=='\r')break;         //Detect return key press (i.e \r or\r\n)
if (!(decimal_digit_Basic(keypress)))continue;

if(display_buffer[0]){for(int n = 8; n>=1; n--)                                      //Shift display for each new keypress
display_buffer[n] = display_buffer[n-1];}
display_buffer[0] = keypress;                                  //Add new keypress           

I2C_Tx_8_byte_array(display_buffer);}                           //Update display includes "cr_keypress"                                                 

I2C_Tx_any_segment_clear_all();
wdr();_delay_ms(50);wdr();_delay_ms(50);
I2C_Tx_8_byte_array(display_buffer);

Long_Num_from_mini_OS = I2C_displayToNum();

return Long_Num_from_mini_OS;}





/***********************Subroutines*********************************/
