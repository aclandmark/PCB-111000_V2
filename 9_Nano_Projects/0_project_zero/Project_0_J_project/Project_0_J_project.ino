


#include "Project_0_header.h"


int main (void)
{ unsigned int PRN = 1;

  setup_HW_Basic;
  LED_1_on;

  while (1) {

    while (switch_1_up);

    if ((Led_1_is_on) && !(Led_2_is_on)) {
      LED_1_off;
      LED_2_on;
      }
    else if (!(Led_1_is_on) && (Led_2_is_on)) {
      LED_1_on;
    }
    else if ((Led_1_is_on) && (Led_2_is_on)) {
      LEDs_off;
    }
    else if (!(Led_1_is_on) && !(Led_2_is_on)) {

      PRN = (PRN_8bit_GEN() % 3);
      switch (PRN) {
        case 0: LED_1_on; break;
        case 1: LED_2_on; break;
        case 2: LEDs_on; break;
      }
    }
    Timer_T0_10mS_delay_x_m(20);
  }
}
