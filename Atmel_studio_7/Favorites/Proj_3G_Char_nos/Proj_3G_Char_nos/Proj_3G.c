/*
 Proj_3G_Char_nos.c
 Simple project uses the display and switches
 to illustrate  the binary number system 
 for positive and negative integers 
 */



#include "Proj_3G_header_file.h"

char digit=1, op, mode;

int main (void){

op=0;
mode = 'u';													//mode: signed or unsigned
setup_HW;
I2C_Tx_display_char(digit,mode);
setup_and_enable_PCI;
sei();

while(1){
switch(op){
case 0: break;												//Static display
case 1: I2C_Tx_display_char(++digit,mode);break;			//Increment "digit" before calling "I2C_Tx....."
case 2: I2C_Tx_display_char(--digit, mode);break;}			//Decrement "digit" before calling "I2C_Tx....."

Timer_T0_10mS_delay_x_m(15);}}



/**************************************************************************************************/
ISR(PCINT2_vect) {											//sw1 and sw3 interrupt
if(switch_2_down)return;									//Ignore if sw2 is still down
if((switch_1_up) && (switch_3_up)){op = 0; return;}			//Both switches up

if(switch_1_down) {op = 1; return;}							//Digits increment
if(switch_3_down) {op = 2; return;}}						//Digits decrement


/**************************************************************************************************/
ISR(PCINT0_vect){											//sw2 interrupt service routine
if(switch_2_up)return;										//Ignore sw2 key release
if((switch_1_down) || (switch_3_down))return;				//Ignore if sw1 or 3 are still down
if (mode == 's')
{mode = 'u'; I2C_Tx_display_char(digit,mode);return;}		//toggle display from signed to unsigned
if (mode == 'u')
{mode = 's'; I2C_Tx_display_char(digit,mode);return;}}		//and visa-versa
