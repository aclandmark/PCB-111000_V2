

//PC3 on Atmega168 drives reset pin on Atmega328
#include "Noise_waveform.h"

unsigned int PRN; 
    unsigned char PRN_counter;
    char mode;
    int m1, m2, m3;
    int rate;
    unsigned int waveform;
    int p, p_1;

char Quad;


int main (void) 

{


  setup_HW_Arduino;

/*switch(reset_status){
  case POR_reset:                 User_prompt_A;    break;
  case WDT_reset:                 Serial.write("WDT reset\r\n");break;
  case WDT_with_ISR_reset:        Serial.write("WDT with interrupt reset\r\n");break;
  case External_reset:            break;}*/

  Thirty_two_ms_WDT_with_interrupt;
sei();
  set_up_pci_on_sw2;
  enable_pci_on_sw2;
  
rate = 0;
if(reset_status != WDT_with_ISR_reset){
PRN_counter = 0;
  PRN = PRN_16bit_GEN (0, &PRN_counter);

PRN = PRN_16bit_GEN (PRN, &PRN_counter);
  m1 = (PRN%7) + 1;

  Quad = 1;}
else Quad = eeprom_read_byte((uint8_t*)0x1FB);


if(reset_status != WDT_with_ISR_reset){ 
waveform = 1 << 7;
for(int m = 0; m < m1; m++)waveform |= (1 << (6-m));}
else waveform = ((eeprom_read_byte((uint8_t*)0x1F9))<< 8) + (eeprom_read_byte((uint8_t*)0x1FA));


while(1){
Waveform_generator(Quad);

Quad = (Quad %4) + 1;}}




void Waveform_generator(char Quad){

Serial.write('.');
  switch (Quad){

    case 1:
    
    if(reset_status != WDT_with_ISR_reset){p = m1;}
    else {//waveform = eeprom_read_byte((uint8_t*)0x1FA);
      p =  eeprom_read_byte((uint8_t*)0x1F8);
        m1 = eeprom_read_byte((uint8_t*)0x1F7);
        reset_status = 0;}
    //p = m1;
    rate = 20*m1;////////////////////
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
 p =  eeprom_read_byte((uint8_t*)0x1F8);
        m1 = eeprom_read_byte((uint8_t*)0x1F7);
        m2 = eeprom_read_byte((uint8_t*)0x1F6);
        reset_status = 0;}

 rate = 20*m1;///////////////////////////
while(p <= m2){
 I2C_Tx_2_integers(waveform, (1 << (7-m1)));
 initialise_timer(rate);
waveform |= (1 << 8+p);
p += 1;}
waveform &=(~(1 << (7 - m1)));break;

 case 3:
if(reset_status != WDT_with_ISR_reset){p = m2;}
else{
 p =  eeprom_read_byte((uint8_t*)0x1F8);
        m1 = eeprom_read_byte((uint8_t*)0x1F7);
        m2 = eeprom_read_byte((uint8_t*)0x1F6);
        reset_status = 0;}
 rate = 20*m2;//////////////////////////////////
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
 p =  eeprom_read_byte((uint8_t*)0x1F8);
        m1 = eeprom_read_byte((uint8_t*)0x1F7);
        m2 = eeprom_read_byte((uint8_t*)0x1F6);
        reset_status = 0;}
   rate = 20*m2;   //////////////////////////////////  
  while(p <= m1){
I2C_Tx_2_integers(waveform,  (1 << (8+m2)));//Serial.print((int) waveform);
if (p==0)waveform = (1 << 7) | (1 << (8+m2));
else waveform |= (1 << (7-p));
initialise_timer(rate);
p += 1;}
waveform &= (~(1 << (8+m2)));break;}
  }
  
 
/**********************************************************************************************/
float initialise_timer(int rate){
   unsigned int count, top, Start_point;
float Time;
top = 65535;
Time = 1.0;
  
count = (unsigned int)(Time * 20000)/rate;
      Start_point = top - count;
    Timer_T1_sub(5, Start_point);}




ISR(PCINT0_vect)
{ if (switch_2_up)return; 
  sei(); Serial.write('.');while(1);}


ISR (WDT_vect){eeprom_write_byte((uint8_t*)0x1FC, 0x01);
eeprom_write_byte((uint8_t*)0x1FB, Quad);
eeprom_write_byte((uint8_t*)0x1FA, waveform);
eeprom_write_byte((uint8_t*)0x1F9, waveform >> 8);
eeprom_write_byte((uint8_t*)0x1F8, p);
eeprom_write_byte((uint8_t*)0x1F7, m1);
eeprom_write_byte((uint8_t*)0x1F6, m2);
Reset_Atmega328;}
    
   /**************************************************************************************/







/***************************************************************************************************************************/
