

#define BL 20 

#include "fp_nos_from_IO.h"


void scroll_display_zero(void);
void shift_display_left(void);
float acquire_fpn(void);

volatile char Data_Entry_complete, digit_entry;
char scroll_control;
char digits[12];

char float_as_string[BL];

int main (void){
float x1;

setup_HW;

if((!watch_dog_reset)){
String_to_PC_Basic("Press: sw_1 to populate digit_0, sw3 to shift the display left\r\n\
sw_2 to enter the number and sw1 to clear the display.\r\n\
Note: display flashes to indicate number has been entered.\r\n");}

watch_dog_reset = 0;

x1 = acquire_fpn();
sprintf(float_as_string, " %g  / 2  =  ",x1);
Num_String_to_PC(float_as_string);
sprintf(float_as_string, "%g (AK to repeat)\r\n?", x1/2.0);
Num_String_to_PC(float_as_string);
while(switch_2_down);
display_8_digit_num_string(float_as_string,  x1/2.0);
waitforkeypress_Basic();
SW_reset;}



/********************************************************************************/
float acquire_fpn(void)
{float x1;
setup_and_enable_PCI;
Init_display_for_pci_data_entry;
Data_Entry_complete = 0;
digit_entry = 0;
scroll_control = 0b00011110;
/*
bit 0: exponent disabled
bit 1: decimal point enabled
bit 2: negative sign enabled
bit 3: LHS of exponent
bit 4: Waiting for first character
*/

sei();
Float_string_from_IO();
sscanf(float_as_string, "%g", &x1);
display_8_digit_num_string(float_as_string,  x1);

disable_pci_on_sw1_and_sw3;
disable_pci_on_sw2;

return x1;}



/*************************************************************************/
void Float_string_from_IO(void){
	char keypress = 0, data = 0;
	int str_ptr = 0;
	
	do{
	while((!(Data_Entry_complete)) && (!(digit_entry)));
	if((digit_entry) && (!(Data_Entry_complete)))keypress = 1;
	if((Data_Entry_complete) && (digit_entry))keypress = 2;
	if((Data_Entry_complete) && (!(digit_entry)))keypress = 3;

	switch(keypress){
		case 1: data = digits[1]; digit_entry = 0; break;
		case 2: data = digits[0]; digit_entry = 0; break;
	case 3: data ='\r'; Data_Entry_complete = 0; break;}

float_as_string[str_ptr++] = data;
Timer_T0_10mS_delay_x_m(1);
}while(data != '\r');}



/*************************************************************************/
ISR(PCINT0_vect){
char disp_bkp[8];
if(switch_2_up)return;								//Ignore switch release
digit_entry = 1;
for(int m = 0; m<=7; m++){disp_bkp[m]=digits[m]; digits[m]=0;}I2C_Tx_8_byte_array(digits);
Timer_T0_10mS_delay_x_m(25);						//Flash display
for(int m = 0; m<=7; m++){digits[m]=disp_bkp[m];}I2C_Tx_8_byte_array(digits);
Data_Entry_complete=1;}	



/*************************************************************************/
ISR(PCINT2_vect){
if((switch_1_up) && (switch_3_up))return;
while(switch_1_down){scroll_display_zero();Timer_T0_10mS_delay_x_m(20);}
if(switch_3_down)shift_display_left();
Timer_T0_10mS_delay_x_m(20);
clear_PCI_on_sw1_and_sw3;}



/*************************************************************************/
void shift_display_left(void){
scroll_control &= (~(0x14));						//negative sign & first char disabled

switch(digits[0]){
case '-':	break;
case '.':	scroll_control &= (~(0x3)); break;		//exponent & dp disabled	
case 'e':	scroll_control &= (~(0xB));			//Set RHS and disable d.p. 
			scroll_control |= 0x04; break;			//Enable neg sign
default: 	if(scroll_control & 8)					//If LHS but not RHS
			scroll_control |= 1;break;} 			//enable exponent

shift_digits_left; digits[0] = '0'; 
I2C_Tx_8_byte_array(digits);
digit_entry = 1;}



/*************************************************************************/
void scroll_display_zero(void){ 					//display scrolls 0 to 9 then minus symbol d.p. E and back to 0
switch (digits[0]){
case '9':
switch(scroll_control){
case 0b11110: digits[0] = '-'; break;				//Waiting for first character: digits[0] = '\0'
case 0b01010: digits[0] = '.'; break;				//Waiting for second character: negative number digits[0] = '-'
case 0b01000: digits[0] = '0'; break;				//LHS waiting for first digit (0 to 9)
case 0b01011: digits[0] = '.'; break;				//digits[0] = 0 to 9: can receive d.p. e or additional digit
case 0b01001: digits[0] = 'e'; break;				//Real number: can only receive e or additional digits
case 0b00000: digits[0] = '0'; break;				//RHS: Can only receive digits
case 0b00100: digits[0] = '-'; break;} break;		//RHS: can receive a - or a digit
		

case '-':
switch(scroll_control){
case 0b11110: digits[0] = '.'; break;				//Waiting for first character: digits[0] = '\0'
case 0b00100: digits[0] = '0'; break;} break;		//RHS: can receive a - or a digit

case '.':
switch(scroll_control){
case 0b11110: digits[0] = '0'; break;				//Waiting for first character: digits[0] = '\0'
case 0b01010: digits[0] = '0'; break;				//Waiting for second character: negative number digits[0] = '-'
case 0b01011: digits[0] = 'e'; break;} break;		//digits[0] = 0 to 9: can receive d.p. e or additional digit
	

case 'e':
switch(scroll_control){
case 0b01011: digits[0] = '0'; break;				//digits[0] = 0 to 9: can receive d.p. e or additional digit
case 0b01001: digits[0] = '0'; break;} break;		//Real number: can only receive e or additional digits

default: digits[0] += 1; break;}

I2C_Tx_8_byte_array(digits);}
