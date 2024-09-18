



#include "Proj_2_C1_header_file.h"
#include "Local_subroutines.c"


extern char digit;
extern char mode;
 



int main (void){
char op=0;
digit=1;
mode = 'u';                         //mode: signed or unsigned
setup_HW;
I2C_Tx_display_char(digit,mode);update_screen();


while(1){
if(isCharavailable_Basic(1))op = Char_from_PC_Basic();
  
switch(op){
case 0: break;                        //Static display
case 'x': I2C_Tx_display_char(++digit,mode);update_screen(); break;
case 'y': I2C_Tx_display_char(--digit, mode);update_screen();break; 
case 'X': digit += 4;I2C_Tx_display_char(digit,mode);update_screen(); break;
case 'Y': digit -= 4;I2C_Tx_display_char(digit,mode);update_screen(); break;
case 'z':
case 'Z': if(mode == 'u')mode = 's'; else mode = 'u';
I2C_Tx_display_char(digit,mode);update_screen();break;
default: String_to_PC_Basic("\r\nStarting again.\r\n");SW_reset;break;}
Timer_T0_10mS_delay_x_m(15);
op = 0;}}







/**************************************************************************************************/
