
/*Proj_9I_Test_PCB_Assembly
**************************************************************/


/*IT INTRODUCES


The Project I2C subroutine "PCB_test()".
This disables the mini-OS multiplexer, enabling the digits and segments to be tested 
one at a time to aid PCB fault finding.
*/



#include "Proj_9G_header_file.h"



int main (void){

char test_num, test_digit;  

setup_HW;

String_to_PC_Basic("Enter number 0-9 followed by digit 0-7"); newline_Basic();

while(1){

test_num = waitforkeypress_Basic();
test_digit = waitforkeypress_Basic();

PCB_test();
waiting_for_I2C_master; 
send_byte_with_Ack(test_num);
send_byte_with_Nack(test_digit);
TWCR = (1 << TWINT);}}


/************************************************************************************************/
