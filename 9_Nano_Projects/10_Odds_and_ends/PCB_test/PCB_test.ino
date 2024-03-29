
/*Proj_9I_Test_PCB_Assembly
**************************************************************/


/*IT INTRODUCES


The Project I2C subroutine "PCB_test()".
This disables the mini-OS multiplexer, enabling the digits and segments to be tested 
one at a time to aid PCB fault finding.
*/



#include "PCB_test_header.h"



int main (void){

char test_num, test_digit;  

setup_HW;

if (!(watch_dog_reset))String_to_PC_Basic("Enter number 0-9 followed by digit 0-7\r\n");
else String_to_PC_Basic("Again\r\n");

while(1){

test_num = waitforkeypress_Basic();
if(test_num == 'x')break;
test_digit = waitforkeypress_Basic();

if(test_digit == 'x')break;

PCB_test(test_num, test_digit);
waiting_for_I2C_master; 
send_byte_with_Ack(test_num);
send_byte_with_Nack(test_digit);
TWCR = (1 << TWINT);} 
I2C_Tx_any_segment_clear_all();
SW_reset;}
