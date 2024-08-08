/*
  EEPROM usage
  0x1FF, 0x1FE and 0x1FD		OSCCAL
  0x1FC and 0x1FB				PRN generator
*/


#include "Project_0_header.h"


int main (void)
{

  setup_HW_Basic;

  Timer_T0_10mS_delay_x_m(10);
  if ((switch_1_down) ) {
    LED_1_on;
  }
  else if ((switch_2_down) ) {
    LED_2_on;
  }
  else if ((switch_3_down) ) {
    LEDs_on;
  }

  Timer_T0_10mS_delay_x_m(10);

  SW_reset;
}
