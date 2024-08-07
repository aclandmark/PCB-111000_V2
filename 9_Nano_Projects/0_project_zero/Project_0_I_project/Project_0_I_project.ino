/*
  EEPROM usage
  0x1FF, 0x1FE and 0x1FD    OSCCAL
  0x1FC and 0x1FB       PRN generator
*/


#include "Project_0_header.h"


int main (void)
{ int counter, ON_time;

  setup_HW_Basic;
  
  while(1){
  
  counter = 0;
  ON_time = 16;

  if (switch_1_down){LED_2_on;}
else {LED_2_off;}


do {
    if (Led_1_is_on) {
      LED_1_off;
      for (int m = 0; m <= (512 - ON_time); m++)_delay_us(6);
    }
    else {
      LED_1_on;
      for (int m = 0; m <= ON_time; m++)_delay_us(6);
    }
    counter += 1;
    if ((counter >= 100)  && (switch_3_down)) {
      counter = 0;
      ON_time *= 2;
      }
  }while(ON_time != 512);  }}
