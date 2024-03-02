

#include "Mux_demo.h"


int main (void)
{ setup_HW;
char Mux_cntl = 0;

 String_to_PC_Basic("\r\nPulse sw1 to demonstrate multiplexer operation\r\n");
while(1){
I2C_Tx(1, 'L', &Mux_cntl);
while(switch_1_up);
while(switch_1_down);
Mux_cntl += 1;
Mux_cntl = Mux_cntl%8;}

  
}
