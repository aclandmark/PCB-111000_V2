

/*
Resolving crashes of the I2C bus when
the User project waits indefinitely for a signal from the master I2C that never arrives

The user project configures the watch dog so that it generates an interrupt when it times out.
The watch dog ISR:
  Saves any data required post reset to EEPROM
  Uses PC3 to drive the Atmega328 reset pin
  Uses PB4 to signal that we want to reset the mini-OS not generate a jump to the programmer  

The "waiting_for_I2C_master" macro is modified so that a watch dog reset is generated
if the signal from the I2C master is more than several mS late. 
*/

#include "Gravity.h"

 int rate;

 int main (void)                          //Example 1
  {
    unsigned int PRN; 
    int m;
    unsigned char PRN_counter;
    unsigned char test;
   
  setup_HW_Arduino;
  
  Thirty_two_ms_WDT_with_interrupt;

  set_up_pci_on_sw2;
  enable_pci_on_sw2;
 
switch(reset_status){
  case POR_reset:                User_prompt_A; Serial.print((int)eeprom_read_byte((uint8_t*)0x1FB));break;
  case WDT_reset:                break;
  case  WDT_with_ISR_reset:       test = eeprom_read_byte((uint8_t*)0x1FB);
                                  test += 1;
                                eeprom_write_byte((uint8_t*)0x1FB, test); _delay_ms(5);
                                 Serial.write('*');Serial.print((int)(eeprom_read_byte((uint8_t*)0x1FB)));Serial.write("\r\n");
                                 break;
case External_reset:            eeprom_write_byte((uint8_t*)0x1FB, 0);break;}
                                  

rate =  15;    //5;
PRN_counter = 0;
  PRN = PRN_16bit_GEN (0, &PRN_counter);
descending(16);
 while(1){
  PRN = PRN_16bit_GEN (PRN, &PRN_counter);
  m = PRN%17;
 if (switch_1_down)rate += 1; 
 ascending(m);
 descending(m);}
SW_reset;
    }
    
    
    void descending (int num_steps){
      unsigned int count, top, Start_point;
float Time;
    unsigned int PORT_1;
   //num_steps = +1;
    PORT_1 = 1 << 16-num_steps;
    top = 65535;
  for (int m = 1; m <= num_steps; m++)
  {  
    //Long_to_PC_Basic((PORT_1));String_to_PC_Basic("\t");
    I2C_Tx_2_integers(PORT_1, 0);
        
    switch (m){
    case 1: Time=1.0; break;
    case 2: Time = sqrt(2.0) - 1.0; break;
    default: Time = sqrt ((float)m ) - sqrt ((float)(m-1));break; }
    
   //Int_to_PC_Basic(int(Time * 10000.0));String_to_PC_Basic("\r\n");

    count = (unsigned int)(Time * 65000.0)/rate; //3
      Start_point = top - count;
    if (m != num_steps)Timer_T1_sub(4, Start_point);
   PORT_1 = PORT_1 << 1; }}


 void ascending (int num_steps){
      unsigned int count, top, Start_point;
float Time;
    unsigned int PORT_1;
    PORT_1 = 1 << 15;
    num_steps += 1;
    
    top = 65535;
  for (int m = num_steps; m; m--)
  { 
    //Long_to_PC_Basic((PORT_1));String_to_PC_Basic("\t");    
    switch (m){
    case 1: Time=1.0; break;
    case 2: Time = sqrt(2.0) - 1.0; break;
    default: Time = sqrt ((float)m ) - sqrt ((float)(m-1));break;}

    //Int_to_PC_Basic(int(Time * 10000.0));String_to_PC_Basic("\r\n");
      
    count = (unsigned int)(Time * 65000.0)/rate;   //3
      Start_point = top - count;
    Timer_T1_sub(4, Start_point);
    I2C_Tx_2_integers(0, PORT_1);
   PORT_1 = PORT_1 >> 1;  }}




void Long_to_PC_Basic (long number)
{ int i = 0;
  char s[12];
   do
  { s[i++] = number % 10 + '0';
  }
  while ((number = number / 10) > 0);
  s[i] = '\0';
  for (int m = i; m > 0; m--)Serial.write(s[m - 1]);
  Serial.write(' ');
}


   /**************************************************************************************/


/**********************************************************************************************/
ISR(PCINT0_vect)
{ if (switch_2_up)return; 
  sei();
  while(1);}



/**********************************************************************************************/
ISR (WDT_vect){eeprom_write_byte((uint8_t*)0x1FC, 0x01);

Reset_Atmega328;
Reset_I2C;while(1);}
  



/***************************************************************************************************************************/
