/*
  EEPROM usage
  0x1FF, 0x1FE and 0x1FD    OSCCAL
  0x1FC and 0x1FB       PRN generator
*/


#include "Project_0_header.h"


int main (void)
{unsigned char next_LED;

next_LED = eeprom_read_byte((uint8_t*)(0x1FA))%3 + 1;
eeprom_write_byte((uint8_t*)(0x1FA), next_LED);



  setup_HW_Basic;

switch(next_LED){
case 1:LED_1_on;while(switch_1_down);break;
case 2:LED_2_on;while(switch_2_down);break;
case 3:LEDs_on; while(switch_3_down);break;
  
}
  
  //LED_1_on;

  
  SW_reset;
}
