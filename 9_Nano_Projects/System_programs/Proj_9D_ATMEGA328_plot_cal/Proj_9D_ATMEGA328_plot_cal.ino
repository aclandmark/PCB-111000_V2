
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
long Result, error;
char Num_string[12];

setup_HW_Arduino_IO_Extra;

Serial.write("\r\nATMEGA 328:  Calibration error plotted against OSCCAL\r\n");

scan_328_cal_factors();
for(int m = 0x10;m < 0xf0; m++){

waiting_for_I2C_master;
Result = receive_byte_with_Ack();
Result = (Result << 8) + receive_byte_with_Ack();
clear_I2C_interrupt;
Hex_to_PC_A(m,Num_string, '\t');
Int_Num_to_PC_A(m,Num_string, '\t');
Int_Num_to_PC_A(Result,Num_string, '\t');
percentage_error = Result;
Int_Num_to_PC_A(percentage_error*100/62500, Num_string, '\t');//'%'
newline_A();
_delay_ms(50);}

waiting_for_I2C_master;
Result = receive_byte_with_Ack();
Result = (Result << 8) + receive_byte_with_Nack();
clear_I2C_interrupt;
Hex_to_PC_A(0xf0,Num_string, '\t');
Int_Num_to_PC_A(0xf0,Num_string, '\t');
Int_Num_to_PC_A(Result,Num_string, '\t');
percentage_error = Result;
Int_Num_to_PC_A(percentage_error*100/62500, Num_string, '\t');//'%'
newline_A();

Serial.write("AK to repeat\r\n");
waitforkeypress_A();SW_reset;}




/*********************************************************************************************************************************/
