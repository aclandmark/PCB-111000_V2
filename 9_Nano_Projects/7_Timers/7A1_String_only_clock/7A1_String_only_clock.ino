
/*Proj_7A_Demo_Clock_A
****************************************************************************************************************************/


/*IT INTRODUCES

Simple clock in which the display is kept up to date by manipulating the string sent to the display every 100 mS.

A watch crystal is not used to ensure accuracy which is therefore poor.
Pencil and paper recommended to understand string manipulation.


USER INSTRUCTIONS

Respond to the user prompt by pressing "R" and entering the time when requested or by pressing "r" and letting 
the clock start at time zero.*/

/*
EEPROM usage
0x1FF, 0x1FE and 0x1FD     OSCCAL
0x1FC and 0x1FB            PRN generator
0x1FA                      Reset source
0x1F9                      Crash counter
0x1F8                      rate (not used here)
0x1F7 to 0x1F0            digits[]

NOTE: Development of the I2C bus stoped once the bus appeared to function reliably.
No use was made of error states or the I2C diagnostic registers.
At some stage in the development of the mini-OS and user project resources the reliability of the 
I2C bus seesm to have been degraged and it can crash, maybe after 10 minutes or maybe a couple of hours.

Development of the I2C bus has not been carried out to understand the crash mechanism or fix it using the error codes.

Instead the watch dog timer with interrupt has been employed.
The watch dog interrupt service routine (ISR) saves the contents of the digits[] array (HoursH, HoursL..... etc.) to EEPROM
It also writes to an EEPROM register (0x1FA) indicating the source of the reset and resets the Atmega 328

But before finally driving the reset pin of the Atmega 328 low to reset it the ISR
  signals to the Atmega 328 by putting a low on PB4 (part of the programming bus)
  This ensures that the programmer generates a jump to 0x00000 
  rather than continuing to reprogram the Atmega 168
the ISR then times out resetting the Atmega 168.

Post reset the user project reads EEPROM location 0x1FA which identifies the source of the reset;
It then refreshes the digits[] array and continues the drive the clock from where it left off.

A special version of "Bootloader_main.c" known as "Bootloader_main_with_OS_reset.c" has been developed to enable 
the mini-OS to be reset in this way.  (Driving the  reset pin low normally takes control to the programmer). 

The waiting_for_I2C_master macro is also modified to ensure that a watch dog time out is generated after a suitable delay
(see the header file for this project).



*/


#include "Proj_7A1_header_file_1.h"


char digits[8];


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
else {reset_clock_1;}
String_to_PC_Basic("\r\n");Num_to_PC_Local((int)(eeprom_read_byte((uint8_t*)0x1F9)));String_to_PC_Basic("\r\n");}

if (reset_status == WDT_with_ISR_reset){
for(int m = 0; m <= 7; m++) digits[m] = eeprom_read_byte((uint8_t*)(0x1F7 - m)); }

I2C_Tx_8_byte_array(digits);  

if (reset_status != WDT_with_ISR_reset){String_to_PC_Basic("AK to start\r\n");
waitforkeypress_Basic();}

while(1){Timer_T0_10mS_delay_x_m(20);Inc_display();Inc_display();}}



/***********************************************************************************************************************/
void Inc_display(void){
 if (deci_SecsH < '9') deci_SecsH++; 
  else {deci_SecsH = '0'; if ((SecsL) < '9') SecsL++;   
              else {SecsL = '0'; if (SecsH < '5') SecsH++; 
                          else {SecsH = '0';  if ((MinsL) < '9') MinsL++; 
                                      else {MinsL = '0'; if (MinsH < '5') MinsH++; else {MinsH = '0';

 switch (HoursH){ case '0': case '1': if ((HoursL & 0x7F) < '9')HoursL++; else {HoursL='0'; HoursH++;} break;
 case '2': if((HoursL) < '3')HoursL++; else {HoursL='0'; HoursH='0';} break;}

 }  //  update deci_Secs, seconds units and tens, minutes units and tens and hours
 }  //  update deci_Secs, seconds units and tens and minutes units and tens
 }  //  update deci_Secs, seconds units and tens and minutes units
 }  //  update deci_Secs and seconds units and tens
 }  //  update deci_Secs and seconds units
I2C_Tx_8_byte_array(digits);}



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
I2C_Tx_any_segment_clear_all();
for(int m = 0; m < 5; m++){_delay_ms(10);wdr();}}




/***********************************************************************************************************************/
ISR(PCINT0_vect)
{ if (switch_2_up)return; 
  sei();
  while(1);}



/***********************************************************************************************************************/
ISR (WDT_vect){eeprom_write_byte((uint8_t*)0x1FA, 0x01); //String_to_PC_Basic("ABC");
for(int m = 0; m <= 7; m++)eeprom_write_byte((uint8_t*)(0x1F7 - m), digits[m]);

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

  
