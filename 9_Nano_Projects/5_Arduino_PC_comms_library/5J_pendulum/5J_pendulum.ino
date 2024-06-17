
#include "pendulum.h"
 int rate;

 int main (void)
  {
    char mode;
     setup_HW_Arduino;
     
     Thirty_two_ms_WDT_with_interrupt;

  set_up_pci_on_sw2;
  enable_pci_on_sw2;
  
rate = 20;

while(1){
for (int m = 8; m>1; m--){
 descending(m,1);
ascending(m,1); rate -= 2;}
for (int m = 1; m < 8; m++){
 descending(m,2);
ascending(m,2); rate += 2;}}
SW_reset;
    }
    

   /**********************************************************************************************/ 
    void descending (int num_steps, char mode)
   { unsigned int count, top, Start_point;
float Time;
    unsigned int PORT_1;
      PORT_1 = 1 << (8-num_steps);
    top = 65535;
  for (int m = 1; m <= num_steps; m++)
  { if((mode == 2) && (m == 1)) I2C_Tx_2_integers(PORT_1, PORT_1);
else I2C_Tx_2_integers(PORT_1, 0);
 
initialise_timer(m);
    PORT_1 = PORT_1 << 1; }
   
    for (int m = num_steps; m; m--){
    I2C_Tx_2_integers(PORT_1, 0);
    if (m > 1)initialise_timer(m);
    
   PORT_1 = PORT_1 << 1; }}



/**********************************************************************************************/
void ascending (int num_steps, char mode){
      unsigned int count, top, Start_point;
float Time;
    unsigned int PORT_1;
      PORT_1 = 1 << (15 - (8-num_steps));
    top = 65535;
  for (int m = 1; m <= num_steps; m++)
  {if (m == 1)I2C_Tx_2_integers(PORT_1, PORT_1);
  else 
  I2C_Tx_2_integers(0, PORT_1);
    
initialise_timer(m);
   
        PORT_1 = PORT_1 >> 1; }
   
    for (int m = num_steps; m; m--){
     
    if((mode == 1) && (m == 1))I2C_Tx_2_integers(PORT_1, PORT_1);
else I2C_Tx_2_integers(0, PORT_1);
    
    initialise_timer(m);
     
   PORT_1 = PORT_1 >> 1; }}



/**********************************************************************************************/
float initialise_timer(int m){
   unsigned int count, top, Start_point;
float Time;
top = 65535;
  
switch (m){
    case 1: Time=1.0; break;
    case 2: Time = sqrt(2.0) - 1.0; break;
    default: Time = sqrt ((float)m ) - sqrt ((float)(m-1));break; }

count = (unsigned int)(Time * 65000.0)/rate;
      Start_point = top - count;
    Timer_T1_sub(4, Start_point);}



/**********************************************************************************************/
ISR(PCINT0_vect)
{ if (switch_2_up)return; 
  sei();
  while(1);}



/**********************************************************************************************/
ISR (WDT_vect){eeprom_write_byte((uint8_t*)0x1FA, 0x01);
Reset_Atmega328;
Reset_I2C;while(1);}
  



/***************************************************************************************************************************/


   /**************************************************************************************/
