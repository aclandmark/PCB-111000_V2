


long Int_KBD_to_display_A_Local(char display_buffer[]){                //Acquires an integer string from the keyboard and returns the binary equivalent
char keypress;
long Long_Num_from_mini_OS;

for(int n = 0; n<=8; n++) display_buffer[n] = 0;              

while(1){
keypress = waitforkeypress_A();
if ((!(decimal_digit_A(keypress)))
&& (keypress != '-'))continue;                                //Ignore keypress if it is not OK
display_buffer[0] = keypress;
break;}
 
I2C_Tx_8_byte_array(display_buffer);                    //See "Chip2chip_comms\Display_driver_header.h"
while(1){
if ((keypress = wait_for_return_key_A())  =='\r')break;         //Detect return key press (i.e \r or\r\n)
if ((decimal_digit_A(keypress)) || (keypress == '\b')\
 || (keypress == '-'))

{if (keypress == '\b'){                                 //Backspace key
for (int n = 0; n < 7; n++)
display_buffer[n] = display_buffer[n + 1];
display_buffer[7] = 0;}

else
{for(int n = 8; n>=1; n--)                                      //Shift display for each new keypress
display_buffer[n] = display_buffer[n-1];
display_buffer[0] = keypress;  }                              //Add new keypress           

I2C_Tx_8_byte_array(display_buffer);}}                                     //Update display includes "cr_keypress"                                                 

I2C_Tx_any_segment_clear_all();
wdr();_delay_ms(50);wdr();_delay_ms(50);
I2C_Tx_8_byte_array(display_buffer);

Long_Num_from_mini_OS = I2C_displayToNum();

return Long_Num_from_mini_OS;}
