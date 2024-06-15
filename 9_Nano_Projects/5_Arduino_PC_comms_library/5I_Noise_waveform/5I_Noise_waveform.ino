

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
0x1FF, 0x1FE and 0x1FD     	OSCCAL
0x1FC and 0x1FB            	PRN generator
0x1FA                      	Reset source
0x1F9                   	Crash counter	(Not used here)
0x1F8						Quad_global
0x1F7						waveform LB
0x1F6						waveform HB
0x1F5						p
0x1F4						m1
0x1F3						m2
*/







#include "Noise_waveform.h"

char Quad_global;

unsigned int PRN; 
    unsigned char PRN_counter;
    char mode;
    int m1, m2;
    int rate;
    unsigned int waveform;
    int p;


int main (void) 

{setup_HW_Arduino;

Thirty_two_ms_WDT_with_interrupt;

  set_up_pci_on_sw2;
  enable_pci_on_sw2;
  
  if(reset_status != WDT_with_ISR_reset){
  PRN_counter = 0;
  PRN = PRN_16bit_GEN (0, &PRN_counter);
  PRN = PRN_16bit_GEN (PRN, &PRN_counter);
  m1 = (PRN%7) + 1;
 Quad_global = 1;}

  else Quad_global = eeprom_read_byte((uint8_t*)0x1F8);

if(reset_status != WDT_with_ISR_reset){ 
waveform = 1 << 7;
for(int m = 0; m < m1; m++)waveform |= (1 << (6-m));}
else waveform = ((eeprom_read_byte((uint8_t*)0x1F6))<< 8) + (eeprom_read_byte((uint8_t*)0x1F7));

while(1){
Waveform_generator(Quad_global);
Quad_global = (Quad_global %4) + 1;}}



/**********************************************************************************************/
void Waveform_generator(char Quad){

 switch (Quad){

    case 1:
    
    if(reset_status != WDT_with_ISR_reset){p = m1;}
    else {
      p =  eeprom_read_byte((uint8_t*)0x1F5);
        m1 = eeprom_read_byte((uint8_t*)0x1F4);
        reset_status = 0;}
    
        if (m1 > 5)rate = 7*m1;
   else rate = 20*m1;
   
   while(p >= 0){
    I2C_Tx_2_integers(waveform, (1 << (7-m1)));
  if (p != m1){waveform &= (~(1 << (7-p)));}
  if(!(p))waveform |= (1 << 8+p);
  initialise_timer(rate);
  p -= 1;}
  break;
  
case 2:

  if(reset_status != WDT_with_ISR_reset)
  {PRN = PRN_16bit_GEN (PRN, &PRN_counter);
  m2 = (PRN%7) + 1;
  p=1;}
    
  else{
   p =  eeprom_read_byte((uint8_t*)0x1F5);
        m1 = eeprom_read_byte((uint8_t*)0x1F4);
        m2 = eeprom_read_byte((uint8_t*)0x1F3);
        reset_status = 0;}

 rate = 20*m1;
 
  while(p <= m2){
  I2C_Tx_2_integers(waveform, (1 << (7-m1)));
 initialise_timer(rate);
  waveform |= (1 << 8+p);
  p += 1;}
  waveform &=(~(1 << (7 - m1)));break;

 case 3:
 
  if(reset_status != WDT_with_ISR_reset){p = m2;}
  else{
  p =  eeprom_read_byte((uint8_t*)0x1F5);
        m1 = eeprom_read_byte((uint8_t*)0x1F4);
        m2 = eeprom_read_byte((uint8_t*)0x1F3);
        reset_status = 0;}
    if (m1 > 5)rate = 3*m1;
  else rate = 20*m2;
  while(p > 0){
  I2C_Tx_2_integers(waveform,  (1 << (8+m2)));
  initialise_timer(rate);
  if (p != m2)
  waveform &= (~(1 << (8+p)));
    p -= 1;} break;

  
  case 4:
  
  if(reset_status != WDT_with_ISR_reset){
  PRN = PRN_16bit_GEN (PRN, &PRN_counter);
  m1 = (PRN%7) + 1;
  p=0;}

  else{
 p =  eeprom_read_byte((uint8_t*)0x1F5);
        m1 = eeprom_read_byte((uint8_t*)0x1F4);
        m2 = eeprom_read_byte((uint8_t*)0x1F3);
        reset_status = 0;}

        rate = 20*m1;
    
      while(p <= m1){
  I2C_Tx_2_integers(waveform,  (1 << (8+m2)));//Serial.print((int) waveform);
  if (p==0)waveform = (1 << 7) | (1 << (8+m2));
  else waveform |= (1 << (7-p));
  initialise_timer(rate);
  p += 1;}
  waveform &= (~(1 << (8+m2)));break;}}

  
 
/**********************************************************************************************/
float initialise_timer(int rate){
   unsigned int count, Start_point;
  
count = (unsigned int)(20000)/rate; 
      Start_point = 0xFFFF - count;
    Timer_T1_sub(5, Start_point);}



/**********************************************************************************************/
ISR(PCINT0_vect)
{ if (switch_2_up)return; 
  sei();
  while(1);}



/**********************************************************************************************/
ISR (WDT_vect){eeprom_write_byte((uint8_t*)0x1FA, 0x01);
eeprom_write_byte((uint8_t*)0x1F8, Quad_global);
eeprom_write_byte((uint8_t*)0x1F7, waveform);
eeprom_write_byte((uint8_t*)0x1F6, waveform >> 8);
eeprom_write_byte((uint8_t*)0x1F5, p);
eeprom_write_byte((uint8_t*)0x1F4, m1);
eeprom_write_byte((uint8_t*)0x1F3, m2);
Reset_Atmega328;
Reset_I2C;}
  



/***************************************************************************************************************************/
