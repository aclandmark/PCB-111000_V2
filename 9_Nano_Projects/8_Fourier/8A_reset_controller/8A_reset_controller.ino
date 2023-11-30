
#include "8A_header_file_1.h"

int main (void){

setup_HW;

setup_watchdog;
wdt_enable(WDTO_30MS);
while((switch_1_down) || (switch_2_down) || (switch_3_down)) wdr();
sei();
switch (reset_status){

case 1: User_prompt_Basic;
String_to_PC_Basic("\r\nProgram running following POR\t"); break;
case 2: String_to_PC_Basic("\r\nProgram triggered by a SW_reset (sw1 pressed)\t"); break;
case 3: String_to_PC_Basic("\r\nProgram triggered by pressing -r- at the p/r prompt (Ext reset)."); break;
case 4: String_to_PC_Basic("\r\nProgram triggered by a SW_reset (sw3 pressed).\t"); break;
case 5:String_to_PC_Basic("\r\nProgram triggered by a WDTout with interrupt (sw2 pressed)");break;}

while(1){if((switch_1_up) && (switch_3_up))wdr(); else 

if(switch_1_down)
while(switch_1_down);

if(switch_2_down){sixty_four_ms_WDT_with_interrupt;
while(switch_2_down);}

if(switch_3_down){eeprom_write_byte((uint8_t*)0x1FC, 0x0);
while(switch_3_down);}}    

return 1;}


ISR (WDT_vect){eeprom_write_byte((uint8_t*)0x1FC, 0x01); }




/*******************************************************************************************************************/
