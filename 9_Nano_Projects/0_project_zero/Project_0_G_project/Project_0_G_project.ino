


#include "Project_0_header.h"


int main (void)
{ unsigned int PRN = 1;

  setup_HW_Basic;

while(1){
    LEDs_off;
PRN = (PRN_8bit_GEN() % 4);
      switch (PRN) {
        case 0: LED_1_on; break;
        case 1: LED_2_on; break;
        case 2: LEDs_on; break;
        case 4:  LEDs_off; break;
      }
Timer_T0_10mS_delay_x_m(20);
while (switch_1_down);

}}

