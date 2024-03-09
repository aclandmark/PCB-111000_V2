

#include "Intensity_tests.h"

int main (void){

char digits[8];

//long num_from_KBD;


setup_HW_Arduino;

//aawhile(switch_1_down);

if (reset_status == 3){Serial.write("Integer Number? Terminate in CR");
Int_KBD_to_display_A(digits);
for (int m = 0; m <= 7; m++)eeprom_write_byte((uint8_t*)m, digits[m]);
Serial.write("\r\nHold sw2 down and toggle sw1 to change intensity\r\n");
}

if(reset_status == 2)
{
for (int m = 0; m <= 7; m++)digits[m] = eeprom_read_byte((uint8_t*)m);
I2C_Tx_8_byte_array(digits);}

//while(switch_1_up);
waitforkeypress_A();
SW_reset;

}
