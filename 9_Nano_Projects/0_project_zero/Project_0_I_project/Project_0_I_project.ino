/*
  EEPROM usage
  0x1FF, 0x1FE and 0x1FD    OSCCAL
  0x1FC and 0x1FB       PRN generator
*/


#include "Project_0_header.h"


int main (void)
{ int counter, ON_time;

  setup_HW_Basic;
  counter = 0;
  ON_time = 160;

  LED_1_on;
  while (switch_3_up);

  while (1) {
    if (Led_1_is_on) {
      LED_1_off;
      for (int m = 0; m <= (320 - ON_time); m++)_delay_us(6);
    }
    else {
      LED_1_on;
      for (int m = 0; m <= ON_time; m++)_delay_us(6);
    }
    counter += 1;
    if ((counter >= 100)  && (switch_3_down)) {
      counter = 0;
      ON_time /= 2;
    }
    if (ON_time == 1) {
      ON_time = 160;
      while (switch_3_down);
    }
  }
}
