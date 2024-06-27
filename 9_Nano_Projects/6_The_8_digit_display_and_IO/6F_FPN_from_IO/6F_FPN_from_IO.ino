

/*Proj_6F_fp_nos_from_IO
**************************************************************/

/*A look at getting floating point numbers from the user switches.

Repeat of 6E but uses IO data entry
Calculates power until FPN limit is reached

IT INTRODUCES


1.     Subroutine "scroll_display_zero()".
              This is used to acquire the next digit (i.e. 0-9, d.p. - or E)

2.     Subroutine "shift_display_left()".
              This is used to enter the latest digit and block any subsequent digits which may now be 
              illegal (i.e. only one decimal point is allowed). 
  
3.     Subroutine FPN_as_string()
              Acquires a string such as 1234.567e8 

4.     Subroutine fpn_from_IO()
              Converts the string to a real number and then multiplies it by the exponent
              to obtain a floating point number (FPN).
               
*/



#include "FPN_IO_to_display_header.h"

#define message_1 "\r\nFPN from IO\r\nPress: sw_1 to populate digit_0, sw3 to shift the display left\r\n\
sw_2 to enter the number and sw1 to do some arithmetic.\r\nNote: display flashes to indicate number has been entered.\r\n"

#define message_2 "\r\nNew number?\r\n"

#define message_3 "\r\nWDTout with interrupt occurred\r\n\
A wdr() statement is probably needed some where.\r\n"



int main (void){
  float x1;
  float power;
 
  setup_HW_Arduino;

while(switch_3_down)wdr();

One_25ms_WDT_with_interrupt;

switch(reset_status){
  case POR_reset:             User_prompt_A;    Serial.write(message_1);break;
  case WDT_reset:             Serial.write(message_2);break;
  case WDT_reset_with_flag:   Serial.write(message_2);break;
  case External_reset:        Serial.write(message_1);break;
  case WDT_with_ISR_reset:    Serial.write(message_3);_delay_ms(25);cli();setup_watchdog_A;while(1);break;}
  
 x1 = fpn_from_IO();
 if(x1 > 0.0)power = 1.2; else power = 3.0;

disable_PCI;
 
while(1){
Sc_Num_to_PC_A(x1,1,6 ,'\r');
I2C_FPN_to_display(x1);

while(switch_1_down)wdr();
if(switch_3_down)break;

x1 = pow(x1, power); }                                //Do some arithmetic

I2C_Tx_any_segment_clear_all;

while(switch_1_down)wdr();
SW_reset;}



/********************************************************************************/
float fpn_from_IO()
{
  char FPN_string[15];
  float num = 0.0;
  char sign = '+';
  
  for(int m = 0; m <= 14; m++)FPN_string[m] = 0;
  
  set_up_PCI;
  enable_PCI;
  
  Init_display_for_pci_data_entry;
  scroll_control = 0b00011110;
  /*
  bit 0: exponent disabled
  bit 1: decimal point enabled
  bit 2: negative sign enabled
  bit 3: LHS of exponent
  bit 4: Waiting for first character
  */
  
  sei();
FPN_as_string(FPN_string);

if (FPN_string[0]== '-')
{for (int m = 0; m <= 13; m++)
FPN_string[m] =  FPN_string[m + 1];
sign = '-';}
num = atof(FPN_string);

if (sign == '-') num = num * (-1);
disable_pci_on_sw2;
return num;}



/*************************************************************************/
int FPN_as_string(char * FPN_num_string){              //Returns the exponent

char keypress = 0;

Data_Entry_complete = 0;
digit_entry = 0;
  
while(1){                                               //Data entry loop
while (!(digit_entry))wdr();                                 //Wait here while each digit is entered
digit_entry = 0;
if (Data_Entry_complete)break;                          //Leave loop when data entry is complete
*(FPN_num_string++) = digits[1]; _delay_us(100);}         //Increment string adddress after saving digit was 1us

*(FPN_num_string++) = digits[0];                        //Save final digit
*FPN_num_string = '\r';   }                              //Terminate string with cr.



/*************************************************************************/
ISR(PCINT0_vect){
  if(switch_2_up)return;                                 //Ignore switch release
  I2C_Tx_any_segment_clear_all();
  Timer_T0_10mS_delay_x_m(25);                           //Flash display
  I2C_Tx_8_byte_array(digits);
Data_Entry_complete=1;digit_entry = 1;
sei();while(switch_2_down)wdr();}                        //Extra line



/*************************************************************************/
ISR(PCINT2_vect){
  if((switch_1_up) && (switch_3_up))return;

  if(switch_1_down)enable_PCI_on_sw3;
  while(switch_1_down){scroll_display_zero();
  Timer_T0_10mS_delay_x_m(20);}
  
  if(switch_3_down){disable_PCI_on_sw3;shift_display_left();}
  
  Timer_T0_10mS_delay_x_m(20);
clear_PCI_on_sw1_and_sw3;}



/*************************************************************************/
void shift_display_left(void){
  scroll_control &= (~(0x14));                        //negative sign & first char disabled

  switch(digits[0]){
    case '-': break;
    case '.': scroll_control &= (~(0x3)); break;      //exponent & dp disabled
    case 'e': scroll_control &= (~(0xB));             //Set RHS and disable d.p.
    scroll_control |= 0x04; break;                    //Enable neg sign
    default:  if(scroll_control & 8)                  //If LHS but not RHS
  scroll_control |= 1;break;}                         //enable exponent

  shift_digits_left; digits[0] = '0';
  I2C_Tx_8_byte_array(digits);
digit_entry = 1;}



/*************************************************************************/
void scroll_display_zero(void){                       //display scrolls 0 to 9 then minus symbol d.p. E and back to 0
  switch (digits[0]){
    case '9':
    switch(scroll_control){
      case 0b11110: digits[0] = '-'; break;           //Waiting for first character: digits[0] = '\0'
      case 0b01010: digits[0] = '.'; break;           //Waiting for second character: negative number digits[0] = '-'
      case 0b01000: digits[0] = '0'; break;           //LHS waiting for first digit (0 to 9)
      case 0b01011: digits[0] = '.'; break;           //digits[0] = 0 to 9: can receive d.p. e or additional digit
      case 0b01001: digits[0] = 'e'; break;           //Real number: can only receive e or additional digits
      case 0b00000: digits[0] = '0'; break;           //RHS: Can only receive digits
    case 0b00100: digits[0] = '-'; break;} break;     //RHS: can receive a - or a digit
    
    case '-':
    switch(scroll_control){
      case 0b11110: digits[0] = '.'; break;           //Waiting for first character: digits[0] = '\0'
    case 0b00100: digits[0] = '0'; break;} break;     //RHS: can receive a - or a digit

    case '.':
    switch(scroll_control){
      case 0b11110: digits[0] = '0'; break;           //Waiting for first character: digits[0] = '\0'
      case 0b01010: digits[0] = '0'; break;           //Waiting for second character: negative number digits[0] = '-'
    case 0b01011: digits[0] = 'e'; break;} break;     //digits[0] = 0 to 9: can receive d.p. e or additional digit
    
    case 'e':
    switch(scroll_control){
      case 0b01011: digits[0] = '0'; break;           //digits[0] = 0 to 9: can receive d.p. e or additional digit
    case 0b01001: digits[0] = '0'; break;} break;     //Real number: can only receive e or additional digits

  default: digits[0] += 1; break;}

I2C_Tx_8_byte_array(digits);}



/****************************************************************************************************************/
ISR (WDT_vect){eeprom_write_byte((uint8_t*)0x1FA, 0x01); while(1);}





/*******************************************************************************/
