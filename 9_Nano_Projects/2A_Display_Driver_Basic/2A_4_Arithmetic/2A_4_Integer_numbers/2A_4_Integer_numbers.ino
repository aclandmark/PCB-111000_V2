

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
 if(switch_1_down) eeprom_write_byte((uint8_t*)0x1FA, 0);
 SW_reset;}


/******************************************************************************************/
