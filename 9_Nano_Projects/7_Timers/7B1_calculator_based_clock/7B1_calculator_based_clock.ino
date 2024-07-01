
/*Proj_7B_Demo_Clock_B
*************************************************************************************************************************/



/*INTRODUCES

Simple clock in which the display is kept up to date by incrementing a counter every second
The counter is converted to Hours, Minutes, Seconds each of which is then converted to askii 
characters and sent to the display.

As for Project 7A accurracy is not good

USER INSTRUCTIONS

Respond to the user prompt by pressing "R" and entering the time when requested or by pressing "r" and letting 
the clock start at time zero.

Watch dog timer code added as with 7A1

*/



#include "Proj_7B_header_file_1.h"



int main (void){
char User_response;

setup_HW_with_reset_analysis;
sei();
 set_up_pci_on_sw2;
  enable_pci_on_sw2;

Sixty_four_ms_WDT_with_interrupt;

if(reset_status == External_reset){
eeprom_write_byte((uint8_t*)(0x1F9), 0);}


if (reset_status != WDT_with_ISR_reset){
String_to_PC_Basic("Press 'R' to enter time or 'r' to start at time zero  ");
User_prompt_Basic;

if(User_response == 'R')set_time();
else {reset_clock_1; deci_SecsH = '0'; deci_SecsL = '0'; deci_sec_counter = 0; }
String_to_PC_Basic("\r\n");Num_to_PC_Local((int)(eeprom_read_byte((uint8_t*)0x1F9)));String_to_PC_Basic("\r\n");}

if (reset_status == WDT_with_ISR_reset){
for(int m = 0; m <= 3; m++)deci_sec_counter += (long)eeprom_read_byte((uint8_t*)(0x1F7 - m)) << (8*m);
Format_time_for_display(); }

I2C_Tx_8_byte_array(digits);


if (reset_status != WDT_with_ISR_reset){String_to_PC_Basic("AK to start\r\n");
waitforkeypress_Basic();}


while(1){Timer_T0_10mS_delay_x_m(20); deci_sec_counter += 2;
if(deci_sec_counter == 864000)deci_sec_counter = 0;
Format_time_for_display();I2C_Tx_8_byte_array(digits);}}



/**********************************************************************************************************************/
void Format_time_for_display(void){
Hours =     deci_sec_counter/36000;
Minutes =   (deci_sec_counter%36000)/600;
Seconds =   ((deci_sec_counter%36000)%600)/10;
deci_Secs = ((deci_sec_counter%36000)%600)%10;

timer_utoa(Hours); HoursH = charH; HoursL = charL; 
timer_utoa(Minutes); MinsH = charH; MinsL = charL; 
timer_utoa(Seconds); SecsH = charH; SecsL = charL; 
timer_utoa(deci_Secs * 10); deci_SecsH = charH; deci_SecsL = charL; }



/***********************************************************************************************************************/
void set_time(void){

for(int m = 0; m <= 7; m++)digits[m] = 0;
String_to_PC_Basic("Enter start time Hours, Minutes and Seconds\
\r\n(24 hour clock with no spaces). Terminate with cr\r\n");

while(isCharavailable_Basic(50) == 0){String_to_PC_Basic("T?  ");}

digits[7] = waitforkeypress_Basic();I2C_Tx_8_byte_array(digits);
for (int m = 0; m<=4; m++){while(isCharavailable_Basic(5) == 0);
if(m == 4){digits[2] = waitforkeypress_Basic();deci_SecsH = '0'; deci_SecsL = '0';}
else digits[6 - m] = waitforkeypress_Basic(); 
I2C_Tx_8_byte_array(digits);}

waitforkeypress_Basic();

deci_sec_counter = 10*(long)((((long)((HoursH -'0') * 10) + HoursL-'0') * 3600) +
((((MinsH-'0') * 10) + MinsL-'0') * 60) +(SecsH-'0') * 10 + SecsL - '0');

I2C_Tx_any_segment_clear_all();
for(int m = 0; m < 5; m++){_delay_ms(10);wdr();}}



/**********************************************************************************************************************/
void timer_utoa(char n){                //converts an unsigned number to a string of digits
if (n>=10)
{charL = n%10;
charH = (n-charL)/10;
charL = charL + '0';
charH = charH + '0';}
else
{charH = '0'; charL = n + '0';}}



/***********************************************************************************************************************/
ISR(PCINT0_vect)
{ if (switch_2_up)return; 
  sei();
  while(1);}



/***********************************************************************************************************************/
ISR (WDT_vect){eeprom_write_byte((uint8_t*)0x1FA, 0x01); //String_to_PC_Basic("ABC");
for(int m = 0; m <= 3; m++){eeprom_write_byte((uint8_t*)(0x1F7 - m), deci_sec_counter >> (8*m));}

eeprom_write_byte((uint8_t*)0x1F9, (int)(eeprom_read_byte((uint8_t*)0x1F9))+1);

Reset_Atmega328;
Reset_I2C;while(1);} 


/***********************************************************************************************************************/
void Num_to_PC_Local (long number)
{ int i = 0;
  char s[12];
  
  do
  { s[i++] = number % 10 + '0';
  }
  while ((number = number / 10) > 0);
  s[i] = '\0';
  for (int m = i; m > 0; m--)Char_to_PC_Basic(s[m - 1]);
  Char_to_PC_Basic(' ');
}

  
