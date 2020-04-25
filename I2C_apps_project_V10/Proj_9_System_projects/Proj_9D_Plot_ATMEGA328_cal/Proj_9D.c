
/*Proj_9C_ATMEGA328_manual_cal
******************************************************************/


/*Like Proj_9A this project generates a print out of calibration error as a function of the calibration
factor OSCCAL and enables the user to choose a value for OSCCAL.


IT INTRODUCES 


Project Subroutine "I2C_Tx(1, 'N', &cal_mode)".
This causes the mini-OS to carry out manual calibration of the ATMEGA 328.

Note:  
Calibration subroutines are not visible to the user but are similar to those used for the ATMEGA 168 
however T2 running under crystal control provides the 7.8125 mS interrupt.

There is no auto_cal_project for the Atmega 328 because this runs automatically under the control 
of the mini-OS following a POR.
*/



#include "Proj_9D_header_file.h"



int main (void){
long percentage_error;
int  Error;	

setup_HW_basic;

String_to_PC("\r\nATMEGA 328 plot calibration results\r\n");

scan_328_cal_factors();

for(int m = 0x10; m <= 0xF0; m++){
Num_to_PC(10,m); String_to_PC("   \t");
waiting_for_I2C_master;
Error = receive_byte_with_Ack();
Error = (Error << 8) + receive_byte_with_Nack();
percentage_error = Error;
clear_I2C_interrupt;

Num_to_PC(10,Error); 
String_to_PC("   \t");
Num_to_PC(10,percentage_error*100/62500);String_to_PC("\t%");
String_to_PC("\r\n");}}


