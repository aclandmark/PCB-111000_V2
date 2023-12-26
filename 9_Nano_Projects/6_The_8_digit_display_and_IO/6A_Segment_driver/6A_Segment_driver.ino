
/*
 Enables users to drive the display manually
 This indicates that there is not necessarily any connection between 
 the display and an actual number.


 Note:  The PCB111000 bootloader enables user programs to distinguish between
 SW_resets and other resets (i.e POR and first time run following programming).
 This facility is exercised here.
 */



#include "Segment_driver_header.h"



#define message_1 "\r\nPress key a to g (x for next digit).\r\n\
Press sw_2 to clear the display.\r\n"
#define message_2 "?  "
#define message_3 "\r\nWDTout with interrupt occurred\r\n\
A wdr() statement is probably needed some where.\r\n"


int main (void){

char letter = 0, digit_num;            

setup_HW_Arduino;
One_25ms_WDT_with_interrupt;

I2C_Tx_any_segment_clear_all();
while(switch_2_down)wdr();


set_up_PCI_on_sw2;
enable_pci_on_sw2;

switch(reset_status){
  case POR_reset:             User_prompt_A;    Serial.write(message_1);break;
  case WDT_reset:             Serial.write(message_2);break;
  case External_reset:        Serial.write(message_1);break;
  case WDT_with_ISR_reset:    Serial.write(message_3);setup_watchdog;while(1);break;}


while(1){digit_num=0;
while(digit_num < 8){

if(Serial.available())                                        //Wait up to 25mS for a key press.  In the absence of one
{switch(letter = Serial.read()){                              //the program reverts back to "while(digit_num < 8){"
case 'a': case 'A':                                           //otherwise it continues and updates the display
case 'b': case 'B':                                           //Program execution is not put on hold awaiting a keypress
case 'c': case 'C':
case 'd': case 'D':
case 'e': case 'E':
case 'f': case 'F':
case 'g': case 'G': 
I2C_Tx_any_segment(letter, digit_num); break;
case 'x': case 'X': digit_num += 1;break;
default: break;}}
_delay_ms(25);
wdr();
}}}


/***************************************************************************************************************/
ISR(PCINT0_vect)
{ if (switch_2_up)return;
   SW_reset;
}

ISR (WDT_vect){eeprom_write_byte((uint8_t*)0x1FC, 0x01); while(1);}
