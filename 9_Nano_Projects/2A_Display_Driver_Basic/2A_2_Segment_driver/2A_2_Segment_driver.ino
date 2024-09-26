
/*
 Almost identical to 6A
 */


/*
EEPROM usage
0x1FF, 0x1FE and 0x1FD     OSCCAL
0x1FC and 0x1FB            PRN generator
0x1FA                      Reset source
*/




#include "Segment_driver_header.h"


#define message_1 "\r\nFor each digit press 0 to 7 then a to g for segments. Press x to reset display.\r\n"
#define message_2 "\r\nAgain!"

int main (void){

char keypress = 0, digit_num=0;            

setup_HW;

if(watch_dog_reset != 1)String_to_PC_Basic(message_1);
else String_to_PC_Basic(message_2);

while(1){                                                               //Keep inputting data until x is received

while(!(isCharavailable_Basic(1)))wdr(); keypress = Char_from_PC_Basic();   //        
  
switch(keypress){                                                       //Test keypress
case '0': case '1': case '2': 
case '3': case '4': case '5': 
case '6': case '7': digit_num = keypress - '0'; break;                  //If numeric save it as "digit_num".

case 'a': case 'b': case 'c':                                           //If a letter illuminate the display 
case 'd': case 'e': case 'f': 
case 'g': I2C_Tx_any_segment(keypress, digit_num); break;

case 'x': {                                                             //Press x to reset the display
I2C_Tx_any_segment_clear_all(); SW_reset;} break;  

defaut: break;                                                            //if any other key (AOK) execute a SW_reset.

}                                                                        //Bottom of switch block return to top of "do-loop"
}
}                                                                        //Repeat program


//ISR (WDT_vect){eeprom_write_byte((uint8_t*)0x1FA, 0x01); while(1);}

/***************************************************************************************************************************/
