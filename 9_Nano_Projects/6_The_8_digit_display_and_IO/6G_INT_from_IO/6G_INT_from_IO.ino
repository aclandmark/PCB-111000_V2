
/**************Proj_6G_Acquire_and process_Integer_Data_from_the _KBD********************/

/*
 * Prints colum of numbers each 1.5 times the previous until 99999999 or -9999999 is reached
 * then returns by dividing by 1.5
 * 
*/



#include "INT_IO_to_display_header.h"

#define message_1 "\r\nDATA FROM I/O\r\n\
Press sw1 to populate digits[0]\r\nsw3 to shift display left\r\n\
sw2 to enter the number\r\nsw1 to pause the program and restart the program.\r\n"

#define message_2 "Enter new number\r\n"

#define message_3 "\r\nWDTout with interrupt occurred\r\n\
A wdr() statement is probably needed some where.\r\n"


int main (void){
long Num_1;
char digits[8];
int counter = 0;

setup_HW_Arduino;
One_25ms_WDT_with_interrupt;

switch(reset_status){
  case POR_reset:             User_prompt_A;    Serial.write(message_1);break;
  case WDT_reset:             Serial.write(message_2);break;
   case External_reset:        Serial.write(message_1);break;
  case WDT_with_ISR_reset:    Serial.write(message_3);_delay_ms(25);cli();setup_watchdog_A;while(1);break;}


while((switch_1_down) || (switch_2_down) ||(switch_3_down));        //wait for switch release

Num_1 = number_from_IO();

do{
Serial.print(++counter); Serial.write('\t');
Serial.print(Num_1); Serial.write("\r\n");

I2C_Tx_long(Num_1);                                           //Sends number to the display
Timer_T0_10mS_delay_x_m(15);
while(switch_1_down)wdr();

Num_1 = (Num_1 / 2) *3;} 
while ((Num_1 < 99999999) && (Num_1 > -9999999));                   //Do some arithmetic

Serial.write("Press sw1 to continue\r\n");

while(switch_1_up)wdr();
Num_1 = (Num_1 / 3) *2; 

do{Num_1 = (Num_1 / 3) *2;                                           //Do the arithmetic in reverse
Serial.print(--counter); Serial.write('\t');
Serial.print(Num_1); 
Serial.write("\r\n");                                             
I2C_Tx_long(Num_1);
Timer_T0_10mS_delay_x_m(15);
while(switch_1_down)wdr();}while (counter-1);

Serial.write("sw1 !\r\n");
while(switch_1_up)wdr();
while(switch_1_down)wdr();
setup_watchdog_A;
SW_reset;}



/******************************************************************************************/
long number_from_IO(void){

set_up_PCI;
enable_PCI;

Init_display_for_pci_data_entry;                                    //Set digit[0] to zero and display it.
while(!(Data_Entry_complete))wdr();                                      //Line A. wait here for pci interrupts used to enter data
Data_Entry_complete = 0;
disable_PCI;
return I2C_displayToNum();}                                         //Acquire binary value of the display and return it.



/********************************************************************************************************************/
ISR(PCINT2_vect) {                                                //input number: store keypresses in array -digits
if(switch_1_down)enable_PCI_on_sw3;
while(switch_1_down){                                             //Use switch 1 to select keypress 0...9
    switch(digits[0]){                                            //Test digits[0]
      case '9': if(!(digits[1]))                                  //If its 9 scroll onto "-"
      digits[0] = '-';                                            //but only if this is the first entry (i.e. digits[1] is 0.
      else digits[0] = '0';break;                                 //otherwise scroll from zero to 9
      case '-': digits[0] = '0';break;                             //If digits[0] is "-" scroll onto '0'.
      default: digits[0]++; break;}                                //Otherwise increment it
    I2C_Tx_8_byte_array(digits);                                  //Send value to the display
    Timer_T0_10mS_delay_x_m(25);}                                 //return to Line A when sw1 is released

if(switch_3_down) {disable_PCI_on_sw3;                           //Use switch 3 to shift display left
    shift_digits_left; digits[0] = '0';                             //shift the display and initialise digit[0] (make it display '0')
  I2C_Tx_8_byte_array(digits);}}                                  //update the display and return to Line A



/********************************************************************************************************************/
ISR(PCINT0_vect){
if(switch_2_up)return;                                           //Ignore switch release
I2C_Tx_any_segment_clear_all();
Timer_T0_10mS_delay_x_m(25);                                      //Flash display
I2C_Tx_8_byte_array(digits);
Data_Entry_complete=1;}                                           //Return to Line A



/****************************************************************************************************************/
ISR (WDT_vect){eeprom_write_byte((uint8_t*)0x1FC, 0x01); while(1);}





/************************************************************************************************************************/
