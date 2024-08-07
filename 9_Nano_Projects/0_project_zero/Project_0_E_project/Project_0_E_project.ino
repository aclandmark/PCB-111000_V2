/*
  EEPROM usage
  0x1FF, 0x1FE and 0x1FD    OSCCAL
  0x1FC and 0x1FB       PRN generator
*/

/*
 Press sw2 then sw1 to increase rate
 Hold sw3 and pres asw1 to reduce it 
 */


#include "Project_0_header.h"


int main (void)  
{
  int on_time = 32;
  char switch_released;
  setup_HW_Basic;

  while (1) {
    if (switch_2_up)switch_released = 0;
    while (switch_1_up); {
      switch_LED_1;
    }
    Timer_T0_10mS_delay_x_m(on_time);

    if ((on_time >= 1) && (switch_2_down) && (switch_released == 0)) {
      on_time /= 2;
      switch_released = 1;
    }
    if (switch_3_down) on_time += 2;
  }

}
