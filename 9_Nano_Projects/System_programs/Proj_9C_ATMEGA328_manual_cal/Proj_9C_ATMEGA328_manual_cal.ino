
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



#include "Proj_9C_header_file.h"
int main (void){



long percentage_error;
int Results[41], error;
unsigned char osccal_MIN;
unsigned char cal_UC, OSCCAL_DV;
unsigned char  New_UC_value;  
char cal_mode=1, Test_res;
char Num_string[12];

setup_HW_Arduino_IO_Extra;

Serial.write("\r\nATMEGA 328 manual calibration (please wait 10 seconds)\r\n\
Cal factor user value   \t");


I2C_Tx(1, 'N', &cal_mode);                                              //Initiate Atmega 328 calibration mode
waiting_for_I2C_master;                                                 //Atmega 328 accepts request to calibrate             
OSCCAL_DV = receive_byte_with_Ack();                                    //Receive 328 OSCCAL_default Value
osccal_MIN = receive_byte_with_Ack();                                   //Working value + 20  

for(int m = 0; m <= 40; m++){                                           //Receive table giving 40 calibration error results
Results[m] = receive_byte_with_Ack();
Results[m] = (Results[m] << 8) + receive_byte_with_Ack();}  
cal_UC = receive_byte_with_Nack();                                      //Receive 328 OSCCAL_User Value
clear_I2C_interrupt;

Hex_to_PC_A(cal_UC,Num_string, '\r');                                   //Print User cal factor

Serial.write("Cal factor default value   \t");                          //Print OSCCAL_default_Value
Hex_to_PC_A(OSCCAL_DV,Num_string, '\r');

for(int m = 0; m <= 40; m++)                                            //Print table of values
{Hex_to_PC_A(osccal_MIN,Num_string, '\t');
Int_Num_to_PC_A(Results[m],Num_string, '\t');
percentage_error = Results[m];
Int_Num_to_PC_A(percentage_error*100/62500, Num_string, '%');
osccal_MIN++;newline_A();}

Serial.write("Enter new user cal value (Use capitol letters)\r\n\
or enter FF to delete the user cal");                                   //Request new OSCCAL_User_Value

New_UC_value = Hex_from_KBD();
Serial.write ("\r\nPress y if OK or AOK and repeat");

while(1){
if(waitforkeypress_A() == 'y')break; 
else {newline_A();Serial.write('?');New_UC_value = Hex_from_KBD();Serial.write(" y?");}}

waiting_for_I2C_master;                                               //Send OSCCAL_User_Value to AT328 device
send_byte_with_Nack(New_UC_value);                                   //mini-OS will test that User cal is suitable
clear_I2C_interrupt;

/**************************************/
waiting_for_I2C_master;
Test_res =  receive_byte_with_Ack();


if(Test_res == 'X'){                                                //min-OS rejects user-cal value just entered
error = receive_byte_with_Ack();
error = (error << 8) + receive_byte_with_Nack();clear_I2C_interrupt;
newline_A(); 
Int_Num_to_PC_A(error,Num_string, ' ');
Serial.write("  Error too great! Try again."); 
newline_A();}

if(Test_res == 'Y'){                                                //min-OS accepts user-cal value just entered
Serial.write("\r\nValues saved to EEPROM   \t");

New_UC_value = receive_byte_with_Ack();
Hex_to_PC_A(New_UC_value, Num_string, '\t');

New_UC_value = receive_byte_with_Nack();
clear_I2C_interrupt;
Hex_to_PC_A(New_UC_value, Num_string, '\r');}

Serial.write("AK to repeat\r\n");
waitforkeypress_A();SW_reset;}




/*********************************************************************************************************************************/
