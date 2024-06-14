

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

/*
EEPROM usage
0x1FF, 0x1FE and 0x1FD     OSCCAL
0x1FC and 0x1FB            PRN generator
0x1FA                      Reset source
0x1F9                      Crash counter
0x1F8                      rate
*/




#include "Gravity.h"

 int rate;

 int main (void)  
  {
    unsigned int PRN; 
    int m;
    unsigned char PRN_counter;
   
  setup_HW_Arduino;
  
  Thirty_two_ms_WDT_with_interrupt;

  set_up_pci_on_sw2;
  enable_pci_on_sw2;
 
switch(reset_status){
  case POR_reset:                if(!(switch_1_up)){User_prompt_A; Serial.print((int)eeprom_read_byte((uint8_t*)0x1F9));}
                                  rate =  5;break;
  case WDT_reset:                break;
  case  WDT_with_ISR_reset:      eeprom_write_byte((uint8_t*)0x1F9, (int)(eeprom_read_byte((uint8_t*)0x1F9))+1); _delay_ms(5);
                                Serial.write('*');Serial.print((int)(eeprom_read_byte((uint8_t*)0x1F9)));Serial.write("\r\n");
                                 rate = (int)(eeprom_read_byte((uint8_t*)0x1F8));
                                 break;
case External_reset:            rate =  5; eeprom_write_byte((uint8_t*)0x1F9, 0);break;}
                                  

 
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
   
    PORT_1 = 1 << 16-num_steps;
    top = 65535;
  for (int m = 1; m <= num_steps; m++)
  {  
    I2C_Tx_2_integers(PORT_1, 0);
        
    switch (m){
    case 1: Time=1.0; break;
    case 2: Time = sqrt(2.0) - 1.0; break;
    default: Time = sqrt ((float)m ) - sqrt ((float)(m-1));break; }
   
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
    switch (m){
    case 1: Time=1.0; break;
    case 2: Time = sqrt(2.0) - 1.0; break;
    default: Time = sqrt ((float)m ) - sqrt ((float)(m-1));break;}

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
  Serial.write(' ');}



/**********************************************************************************************/
ISR(PCINT0_vect)
{ if (switch_2_up)return; 
  sei();
  while(1);}



/**********************************************************************************************/
ISR (WDT_vect){eeprom_write_byte((uint8_t*)0x1FA, 0x01);

eeprom_write_byte((uint8_t*)0x1F8, rate);
Reset_Atmega328;
Reset_I2C;while(1);}
  



/***************************************************************************************************************************/
