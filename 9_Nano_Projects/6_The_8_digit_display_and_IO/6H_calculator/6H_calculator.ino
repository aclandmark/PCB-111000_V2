

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

//CHANGE DIGITS ARRAY SIZE TO 15

#include "Calculator_header.h"

#define message_1 "\r\nDATA FROM I/O\r\n\
Press sw1 to populate digits[0]\r\nsw3 to shift display left\r\n\
sw2 to enter the number\r\nsw1 to pause the program and restart the program.\r\n"

#define message_2 "Enter new number\r\n"

#define message_3 "\r\nWDTout with interrupt occurred\r\n\
A wdr() statement is probably needed some where.\r\n"


int main (void){
  char op;
  float x1, x2, result;
 
  setup_HW_Arduino;

  switch(reset_status){
  case POR_reset:             User_prompt_A;    Serial.write(message_1);break;
  case WDT_reset:             Serial.write(message_2);break;
   case External_reset:        Serial.write(message_1);break;
  case WDT_with_ISR_reset:    Serial.write(message_3);_delay_ms(25);cli();setup_watchdog_A;while(1);break;}

/*
if(!(watch_dog_reset))
{Serial.write("\r\nPress: sw_1 to populate digit_0, sw3 to shift the display left\r\n\
sw_2 to enter the number and sw1 to select a function.\r\n\
Note: display flashes to indicate number has been entered.\r\n\
Press sw_3 then sw_1 to reset (release slowly).\r\n");}

else {Serial.write("\r\nRestarted.\r\n\r\n"); watch_dog_reset = 0;}*/
  
 x1 = fpn_from_IO();
 Sc_Num_to_PC_A(x1,1,6 ,' ');
 I2C_FPN_to_display(x1);
while(switch_1_up);
  
while(1){
  if (switch_3_down)break;

op = 0;
cli();
I2C_Tx_any_segment_clear_all();
while(switch_1_down)
{op = op%8;
I2C_Tx_any_segment('d', 7- op);
op += 1;
Timer_T0_10mS_delay_x_m(40);
I2C_Tx_any_segment_clear_all();}

if(op <= 5){
x2 = fpn_from_IO();}

sei();
switch(op){
case 1:result = x1+x2; Serial.write (" + ");  break;
case 2:result = x1-x2; Serial.write (" - ");  break;
case 3:result = x1*x2; Serial.write (" x "); break;
case 4:result = x1/x2; Serial.write (" / ");  break;
case 5:result = pow(x1,x2); Serial.write (" ^ ");  break;
case 6:result = sin(x1 / 57.296); x2 = x1; Serial.write (" Sin ");  break; 
case 7:result = cos(x1 / 57.296); x2 = x1; Serial.write (" Cos "); break;                                               //
case 8:result = tan(x1 / 57.296); x2 = x1; Serial.write (" Tan "); break;}

Sc_Num_to_PC_A(x2,1,6 ,' '); 

Serial.write (" = "); Sc_Num_to_PC_A(result,1,6 ,' ');
Serial.write ("\r\n");
I2C_FPN_to_display(result);
x1 = result;

while(switch_1_up);}

cli();
while((switch_1_down) || (switch_3_down));
I2C_Tx_any_segment_clear_all();
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

Data_Entry_complete = 0;
digit_entry = 0;
non_zero_detect = 0;
  
while(1){                                               //Data entry loop
//sw_3_status = 0;
while (!(digit_entry));                                 //Wait here while each digit is entered

digit_entry = 0;
if (Data_Entry_complete)break;}                          //Leave loop when data entry is complete
                                                         //Increment string adddress after saving digit 
*(FPN_num_string) = '\r'; 
{int m = 0; while (digits[m]){shift_FPN_num_string_left; *(FPN_num_string)=digits[m++] ;}}
}     



/*************************************************************************/
ISR(PCINT0_vect){
  if(switch_2_up)return;                                 //Ignore switch release

if(switch_3_down)
{if(digits[1])shift_digits_right;non_zero_detect = 1;}                     //Over ride zero detect mechanism

if(!(non_zero_detect))return;                                //Detects entries of Zero, minus or d.p. only
  
  I2C_Tx_any_segment_clear_all();
  Timer_T0_10mS_delay_x_m(25);                           //Flash display
  I2C_Tx_8_byte_array(digits);
Data_Entry_complete=1;digit_entry = 1;
while(switch_2_down);}



/*************************************************************************/
ISR(PCINT2_vect){sei();
  if((switch_1_up) && (switch_3_up))return;

  sei();
  disable_PCI_on_sw1_and_sw3;

if(switch_1_down)non_zero_detect= 0;
 
  while(switch_1_down){scroll_display_zero();
  Timer_T0_10mS_delay_x_m(20);}

if((!(non_zero_detect))  && (digits[0] != '0')
&& (digits[0] != '-') && (digits[0] != '.')
 && (digits[0] != 'e'))non_zero_detect = 1;
  
  if(switch_3_down)shift_display_left();
  
  Timer_T0_10mS_delay_x_m(20);
clear_PCI_on_sw1_and_sw3;

while(switch_3_down);
enable_PCI;}



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







/*******************************************************************************/
