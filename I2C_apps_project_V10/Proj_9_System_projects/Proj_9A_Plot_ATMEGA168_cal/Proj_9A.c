

#include "Proj_9A_header_file.h"







volatile char T1_OVF;
volatile long error_SUM;
char OSCCAL_WV, OSCCAL_DV, New_UC_value;
volatile int EA_counter, EA_buff_ptr;
volatile char cal_mode;

long buffer[1];												//Not actually used here



int main (void){
long  error; 


setup_HW;

OSCCAL_WV = OSCCAL;

String_to_PC("\r\nATMEGA 168 plot calibration results\r\n");

sei();	

cal_mode = 2;
String_to_PC("\r\n");

for(int m = 0x10; m <= 0xF0; m++){
Num_to_PC(10,m); String_to_PC("   \t");
initialise_and_start_timers;
OSCCAL = m;
error = compute_error(0,cal_mode,1);
OSCCAL = OSCCAL_WV;
close_timers;

Num_to_PC(10, error);
String_to_PC("   \t");
Num_to_PC(10,error*100/62500);String_to_PC("\t%");
String_to_PC("\r\n");}}




/**********************************************************************************************/
ISR(TIMER1_OVF_vect){T1_OVF++; }									//T1 should count to 62500 in 7.8125ms.




/************************************************************************************************/
ISR(TWI_vect){														//T2 (mini_OS) overflows: giving a calibrated tick rate
long TCNT1_BKP;
TWDR;																//Read TWDR the data register
send_byte_with_Nack(1);												//Master responds by remaining in mode P
clear_I2C_interrupt;

TCCR1B = 0;															//Halt T1
TCNT1_BKP = TCNT1;													//Copy the value of TCNT1
TCNT1 = 0;															//Clear TCNT1
TCCR1B = 1;															//Get T1 running again ASAP (Note T2 (mini_OS) has not stopped running)
if(EA_counter < cal_mode)T1_OVF = 0;								//Ignore first result for "emergency_cal()" or first 5 for "compute_error"
else
{switch(T1_OVF){
case 0: error_SUM = error_SUM + (TCNT1_BKP - 62500); break;
case 1: error_SUM = error_SUM + (TCNT1_BKP - 62500 + 65536); break;
case 2: error_SUM = error_SUM + (TCNT1_BKP - 62500 + 131072); break;}
T1_OVF = 0;}														//Sum error results fo the next 20 TWI interrupts
EA_counter++;														//counter increments up to 25.
waiting_for_I2C_master_with_ISR;}									//Be ready for next TWI interrupt	





/**********************************************************************************************/
void I2C_initiate_7_8125mS_ref(void){
char num_bytes=0;
char mode = 'U';
waiting_for_I2C_master;
send_byte_with_Ack(num_bytes);
send_byte_with_Nack(mode);
clear_I2C_interrupt;}	




/************************************************************************************************/		
long compute_error(char local_error_mode, char local_cal_mode, char sign)				
		{long error;
		char Num_1, Num_2;
		
		waiting_for_I2C_master_with_ISR;										//TWI generates interrupt every 7.8125mS 
		
		if (local_cal_mode == 1){Num_1 = 2; Num_2 = 1;}
		if (local_cal_mode == 2){Num_1 = 6; Num_2 = 4;}
		if (local_cal_mode == 5){Num_1 = 15; Num_2 = 10;}
		
		EA_counter = 0;															//Compute error for each value of OSCCAL 10 times
		error_SUM = 0;
		while(EA_counter < Num_1);EA_counter = 0;TWCR = 0;
		error = error_SUM;
		if ((!sign) && (error < 0)) error *= (-1);
		if (local_error_mode)
		{buffer[EA_buff_ptr] = error/Num_2; EA_buff_ptr++;}
		return error/Num_2;}

