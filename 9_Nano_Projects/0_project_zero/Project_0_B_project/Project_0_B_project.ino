/*
  EEPROM usage
  0x1FF, 0x1FE and 0x1FD    OSCCAL
  0x1FC and 0x1FB       PRN generator
*/


#include "Project_0_header.h"


int main (void)
{

  setup_HW;

  while (1) {

    while (switch_1_up);
    {
      switch_LED_1;
    }
    Timer_T0_10mS_delay_x_m(20);
  }
}
