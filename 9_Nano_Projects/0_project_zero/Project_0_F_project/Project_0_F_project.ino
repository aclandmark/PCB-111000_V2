/*
  EEPROM usage
  0x1FF, 0x1FE and 0x1FD    OSCCAL
  0x1FC and 0x1FB       PRN generator
*/


#include "Project_0_header.h"


int main (void)
{ int Max= 512;
    char switch_enabled = 1;
    
  setup_HW_Basic;
  
  while(1){
  
  


do {
   
      LED_1_off;
      LED_2_on;
      for (int m = 0; m < Max; m++)_delay_us(800);
   
      LED_1_on;
      LED_2_off;
      for (int m = 0; m < Max; m++)_delay_us(800);
    
   if ((switch_3_down) && (switch_enabled)){Max /= 2; switch_enabled = 0;} 
   
  if ((switch_1_down) && (switch_3_up)){switch_enabled = 1;}

   
  }while(Max >= 8); Max = 512; } }





  /************************************************************************************/
