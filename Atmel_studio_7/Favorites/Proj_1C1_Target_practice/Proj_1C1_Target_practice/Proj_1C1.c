/*
 * Proj_1C1_Target_practice.c
 *
 * Created: 04/05/2020 14:43:32
 * Author : Mark
 */ 

/*Proj_1C1_Pin_Change_Interupt
**************************************************************/


/*
A SLIGHT VARIATION ON PROGRAM 3 THE REACTION TIME TESTER.
Test your reactions by attempting to shoot the LEDS down using the middle pcb switch (switch_2).
*/



#include "Proj_1C1_header_file.h"



unsigned int PORT_1, mask;
char switch_control;

int main (void){
	setup_HW_basic;
	config_sw2_for_PCI;

	mask = 0xFFFF;
	switch_control = 0;
	sei();
	while(mask){
		PORT_1=1;

		for(int m = 1; m < 17; m++){
			if(m ==	1)switch_control = 0;
			I2C_Tx_2_integers(PORT_1 & mask, (~mask) ^ PORT_1);
			
			Timer_T0_10mS_delay_x_m(10);
			PORT_1 = (PORT_1 << 1);
		}}I2C_Tx_2_integers(0, 0xFFFF);
		Timer_T0_10mS_delay_x_m(100);
	SW_reset;}



	/**************Routine executed every time that switch is  operated**************************************/
	
	ISR(PCINT0_vect) {					//This ISR momentarily interrupts the main routine
		if(switch_2_up)return;				//It notes which LED has just been shot down and
		if (switch_control) return;			//Return if "switch_control" is not zero
		mask &= ~PORT_1;					//writes zero at its location in the "mask" register
	switch_control = 1;}


