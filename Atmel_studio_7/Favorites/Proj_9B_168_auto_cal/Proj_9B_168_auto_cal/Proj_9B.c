

/*Proj_9B_168_auto_cal
************************************************************************/

/*
Uses a watch crystal supplied 7_8125mS_ref provided by the I2C (Two wire interface) interrupt
Timer counter 1 being clocked at 8MHz counts up to 62500 between interrupts
OSCCAL the calibration register can be adjusted until the error is less than 1%.
*/



#include "Proj_9B_header_file.h"



int main (void){

char counter_1, counter_2;
char OSCCAL_WV, OSCCAL_mem = 0;
long  error_mag;
int limit;

setup_HW_basic;

initialise_and_start_timers;
sei();

cal_mode = 1;
counter_1 = 0xF1;
while(1){if (!(counter_1%6))Char_to_PC('.');
	counter_1 -= 1;
	OSCCAL = counter_1; 
	error_mag = compute_error(0,cal_mode,0);
	if(counter_1 > 0xE0)continue;										//Ignore first 16 results
	if(error_mag < 1000)break;}											//Acceptable error
	
OSCCAL_mem = OSCCAL;
counter_2 = 0;

cal_mode = 5;															//Sets the number of averages used to obtain the calibration error
limit = 1000;

for(int m = 1; m <= 9; m++)												//Iteratively improve error
	{Char_to_PC('.');
	limit -= 100;
	Minimise_error(limit, &counter_1, &counter_2, &error_mag, &OSCCAL_mem, cal_mode);}
	
error_mag = compute_error(0,cal_mode,0);
OSCCAL_WV = OSCCAL;

close_timers;

String_to_PC("Cal factor ");
Num_to_PC(10, OSCCAL);
String_to_PC("   Error ");
Num_to_PC(10, error_mag);
String_to_PC("\r\nSave? y or AOK\r\n");

if(waitforkeypress() == 'y'){
	eeprom_write_byte((uint8_t*)0x1FE, OSCCAL_WV);
	eeprom_write_byte((uint8_t*)0x1FF, OSCCAL_WV);
	String_to_PC("Values saved to EEPROM   ");							//Echo values back from the EEPROM
	Num_to_PC(10,eeprom_read_byte((uint8_t*)0x1FE));
	String_to_PC ("    ");
	Num_to_PC(10,eeprom_read_byte((uint8_t*)0x1FF));

	Char_to_PC('\t');
	cal_mode = 5;
	initialise_and_start_timers;
	error_mag = compute_error(0,cal_mode,0);
	close_timers;
	Num_to_PC(10, error_mag);
	}

else String_to_PC("Not saved\r\n");

String_to_PC("\r\nAK to repeat\r\n");
waitforkeypress();SW_reset;}






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
if(EA_counter < cal_mode)T1_OVF = 0;								//Ignore initial results
else
	{switch(T1_OVF){
		case 0: error_SUM = error_SUM + (TCNT1_BKP - 62500); break;
		case 1: error_SUM = error_SUM + (TCNT1_BKP - 62500 + 65536); break;
		case 2: error_SUM = error_SUM + (TCNT1_BKP - 62500 + 131072); break;}
	T1_OVF = 0;}													//Sum error results for up to 10 TWI interrupts
EA_counter++;														
waiting_for_I2C_master_with_ISR;}									//Be ready for next TWI interrupt



/**********************************************************************************************/
void I2C_initiate_7_8125mS_ref(void){
char num_bytes=0;
char mode = 'U';
waiting_for_I2C_master;
send_byte_with_Ack(num_bytes);
send_byte_with_Nack(mode);
clear_I2C_interrupt;}




/*************************************************************/
void Minimise_error(int limit, char *counter_1, char *counter_2, long *error_mag, char *OSCCAL_mem, char local_cal_mode )
{while(*counter_2 < 20)
	{ OSCCAL = *counter_1; *error_mag = compute_error(0,local_cal_mode,0);
	if(*error_mag < limit)break;
	*counter_1 -= 1;
	*counter_2 +=1;}
if (*counter_2 < 20)*OSCCAL_mem = OSCCAL;else OSCCAL = *OSCCAL_mem;}




/************************************************************************************************/
long compute_error(char local_error_mode, char local_cal_mode, char sign)
{long error;
char Num_1, Num_2;														//Set how many averages are taken when the calibration error is calculated
	
waiting_for_I2C_master_with_ISR;										//TWI generates interrupt every 7.8125mS
	
if (local_cal_mode == 1){Num_1 = 2; Num_2 = 1;}							//cal_mode sets the initial number of measurements to be ignored
if (local_cal_mode == 2){Num_1 = 6; Num_2 = 4;}
if (local_cal_mode == 5){Num_1 = 15; Num_2 = 10;}
	
EA_counter = 0;															//Error_Average_counter
error_SUM = 0;
while(EA_counter < Num_1);												//Wait here while calibration error is measured  Num_1 times
EA_counter = 0;TWCR = 0;
error = error_SUM;
if ((!sign) && (error < 0)) error *= (-1);
if (local_error_mode)													//This mode is not used in this project
	{buffer[EA_buff_ptr] = error/Num_2; EA_buff_ptr++;}
return error/Num_2;}													//Num_2 sets the number of measurements that are used to obtain the average result

