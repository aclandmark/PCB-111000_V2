

/*
Proj_4D_int_num_ops.c
 

IT INTRODUCES

An alternative method to enter data using the switches.
Uses the mini-OS to calculate roots and powers of integer numbers.


USER INSTRUCTIONS

Press sw1 for roots or sw3 for powers
repeatedly press sw2 to scroll through the digits
when final digit is being entered press sw1 before releasing sw2 then release sw1
press sw2 for next number (only one digit is allowed)
release sw2 and root or power will be calculated
pulse sw2 to repeat calculation
hold sw1 or sw3 down while pulsing sw2 then press it again to reset the device.*/





#include "Proj_4D_header_file.h"


int main (void){
	
char  Op = 'P';

setup_HW_basic;
User_instructions;
config_sw2_for_PCI;

while(1){if(switch_3_down) {Op = 'P'; break;}
if(switch_1_down) {Op = 'R'; break;}}
while (switch_1_down); while (switch_3_down);
	
while(1){
	int_plus_short_from_IO();									//Line A
	I2C_Tx_Uarithmetic_OP(Op);									//OS does the arithmetic

	while(1){
		while(switch_2_up); 									//Wait for sw2 keypress
		if((switch_1_down) || (switch_3_down))break;			//if SW1 or 3 down clear display and reset
		I2C_Tx_Uarithmetic_OP('A'); 							//OS restores the display
		while(switch_2_down); 									//Wait for sw2 release
		inc_op;													//Change the arithmetic operation
		I2C_Tx_Uarithmetic_OP(Op);}								//OS does the arithmetic
	while (switch_1_down); while (switch_3_down);				//Prior to reset wait for all switches to be released
	clear_digits;I2C_Tx_8_byte_array(digits);SW_reset;}}		//clear display and reset
	



/****************************************************************************************************************/
ISR(PCINT0_vect){int_arithmetic_sw2_isr();}



/****************************************************************************************************************/
void int_plus_short_from_IO(void){						//Acquire two numbers
char copy_of_SREG;

initialise_OS_chip_for_arithmetic;
copy_of_SREG = SREG;									//Save interrupt state
data_counter = 0; Return_key = 0;						//Initialise data counter and return key
sei();													//Enable interrupts
while(1){												//Loop B. Wait for sw2_isr.
	if(data_counter == 2)break;							//Return to line A when 2 numbers have been entered
	if(Return_key){enter_number();						//If return key is set enter the number
		Return_key = 0;}}								//Reset return key and wait for next return key press
SREG = copy_of_SREG;} 									//Restore interrupts and return to line A



		/****************************************************************************************************************/
void enter_number(void){

I2C_Tx_2URNs_from_IO(digits);							//No change to display indicates return key press to the OS:
switch (data_counter){
	case 0: shift_digits_left; digits[0] = '\0';		//First number to be entered: shift digits left and blank digit 0
			shift_digits_left; digits[0] = '0';			//shift digits left and initialise digit 0 to '0'
			I2C_Tx_2URNs_from_IO(digits); break;		//Update the display
	case 1:
		if((digits[0]=='1') || (digits[0]=='0'))		//Second number to be entered:
		return; break;									//If it is zero or 1 do nothing. Return without incrementing data_counter
	
	case 2: disable_pci_on_sw2; break;}					//Data entry is complete. Disable pci on sw2.
data_counter++;}										//Increment data_counter



/****************************************************************************************************************/
void int_arithmetic_sw2_isr(void){
			
if (switch_2_up) return;								//Ignore key release
while(1){												//scrolling digit[0]
		do{												//Keypress on SW2 scrolls digits[0] through digits 0-9
			switch(digits[0]){							//Test digits[0]
				case '9': digits[0] = '0';break;		//If 9 set to 0
				default: digits[0]++; break;}			//Else increment
			I2C_Tx_2URNs_from_IO(digits);				//Update display
			Timer_T0_10mS_delay_x_m(25);
			}while(switch_2_down);						//Continue while sw2 is down

Timer_T0_10mS_delay_x_m(50);							//wait 500ms after sw2 has been released
if(switch_2_up)break;}									//if sw2_down again continue scrolling the digit
														//When sw2 is finally released  if:
if((switch_1_down) ||(data_counter == 1)) 				//Set return key. sw1 is down (data entry is complete) or
{Return_key=1; return;}									//data_counter == 1 as the second number can only be 1 digit long

if ((!(data_counter)) && digits[5])						//Set return key
{Return_key=1; return;}									//Acquiring number 1? This is automatically terminated after 6 digits

shift_digits_left;										//Ongoing acquisition? shift display left
digits[0] = '0';										//Initialise digit[0] to '0'
I2C_Tx_2URNs_from_IO(digits);}							//Update display and return to loop B without setting the return key




