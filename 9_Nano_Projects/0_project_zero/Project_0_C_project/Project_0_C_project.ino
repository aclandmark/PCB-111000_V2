/*
EEPROM usage
0x1FF, 0x1FE and 0x1FD    OSCCAL
0x1FC and 0x1FB       PRN generator
*/


#include "Project_0_header.h"


 int main (void)                          //Example 1
  { 

  setup_HW;

while(1){

while ((switch_1_up) && (switch_2_up));
if(switch_1_up){switch_LED_1;while(switch_2_down);}
else{switch_LED_2;while(switch_1_down);}
}
  
    }
