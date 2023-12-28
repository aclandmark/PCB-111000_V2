
/*
 From this point on some Arduino and standard C library functions will be introduced and used.
 For example 
 Serial.begin(115200):    This initialises the UART for 57600 Baud (assuming an 8MHz clock)
 Serial.write:            This sends text to the PC
 Serial print:            This sends numbers to the PC as text
 atol/atoi:               C functions that convert a text string to a long / integer number
 ltoa:                    C function that converts a long number to text (askii characters)

 Note all characters received by the UART are saved to a buffer.
 Serial.readBytesUntil as implemented here reads these characters and transfers them
 to a separate buffer "num_as_string" stopping only when a cartridge return (cr) is detected
 or a defined number keypresses have been made (i.e. 20 or 30).
 The keypresses contained in the buffer "num_as_string" are then converted to a number. 
 

 This project accepts an integer number and prints out all the integer numbers by which it can be divided
 giving an integer result.
  

Subroutines Int_Num_to_PC_A and Int_Num_from_PC_A() can be found in  UNO_proj_resources\PC_comms\Arduino_Rx_Tx_UNO_pcb.c
Int_Num_from_PC_A() implements the backspace key press
*/



/************************************************************************************************************/
#include "Integer_numbers_header.h"

#define message_1 "\r\nEnter integer number:  PCB returns all factors of that number.(Press sw1 to cancel.)\r\n"
#define message_2 "New number?\r\n"
#define message_3 "\r\nCalculation cancelled. New number?\r\n"


#define BL 30                     //Buffer length

int main (void)  
  { 
    char num_string[BL + 2];
    long  num;
    long m = 1;
 
 setup_HW_Arduino;
 set_up_pci_on_sw1_and_sw3;
 enable_pci_on_sw1_and_sw3
 
switch(reset_status){
  case POR_reset:                 User_prompt_A;    Serial.write(message_1);break;
  case WDT_reset:                 Serial.write(message_2);break;
  case WDT_reset_with_flag:       Serial.write(message_3);break;
  case External_reset:            Serial.write(message_1);break;}
 
num = Int_Num_from_PC_A_Local(num_string,BL);

newline_A();
Serial.print(num);
Serial.write("\t");

do{
if(!(num%m)){Int_Num_to_PC_A_Local(m, num_string, '\t');}
m += 1; if(!(m%10000))Serial.write(".");}while(m < num);

SW_reset;
return 1;  }





/******************************************************************************************/
long Int_Num_from_PC_A_Local(char * num_as_string, char bufferlen)    //Project version implements the backspace key
{char strln;
Serial.flush();   
strln = Serial.readBytesUntil('\r',num_as_string, bufferlen);
num_as_string[strln] = 0;
return atol(num_as_string);}



/******************************************************************************************/
void Int_Num_to_PC_A_Local(long Int_num, char * num_as_string, char next_char)
{ltoa(Int_num, num_as_string, 10);
Serial.print(num_as_string);
Serial.print(next_char);}


ISR(PCINT2_vect){
if(switch_3_down) return;
if((switch_1_up) && (switch_3_up))return;
 if(switch_1_down) eeprom_write_byte((uint8_t*)0x1FC, 0);
 SW_reset;}


/******************************************************************************************/
