
#include "0_ones_processor_header.h"
#include "0_ones_processor_header_2.h"

#define zero 0
#define one 1
#define two 2
#define three 3
#define four 4
#define five 5
#define six 6
#define seven 7





char reg_bkp[8];
char update_reg;
char bit_num, keypress;
char digits[8];

int main(void){
  char mode;

setup_HW;

String_to_PC_Basic("\r\n\r\nManipulating one of the PORT registers\r\n");

String_to_PC_Basic\
("\r\nStep 1  Set up CPU register\r\n\
Type bit names\r\n\
z, o, tw, th, fo, fi, si or se");

do{
if(select_bits())
{cpu_reg_1 |= (1 << bit_num);
Display_registers;} 
}while (keypress != 'x'); 

String_to_PC_Basic("\r\n\r\nStep 2  Copy to PORT register\r\n");
waitforkeypress_Basic();
port_reg = cpu_reg_1;
cpu_reg_1 = 0;
Display_registers;


String_to_PC_Basic("\r\nTo manipulate bits\r\n\t\
send 'r' to clear them\r\n\t\
's' to set them,\r\n\t\
'c' to change them and\r\n\t\
't' to test them");

while(1){

mode = waitforkeypress_Basic();

String_to_PC_Basic("\r\n\r\nStep 1: Set up CPU register");

select_bits_to_process();

switch(mode){
case 'r': clear_bits; break;
case 's': set_bits; break;
case 'c': break;
case 't': break;}

}
while(1);
}




void select_bits_to_process(void){
do{
if(select_bits())
{cpu_reg_1 |= (1 << bit_num);
Display_registers;}     
}while (keypress != 'x');}






char set_update(char bit_num)
{if(!(reg_bkp[bit_num])){update_reg = 0;reg_bkp[bit_num] = 1;} 
else update_reg = 1;return update_reg;}



char select_bits(void){
    update_reg = 0;
keypress = waitforkeypress_Basic();
switch(keypress){
  case 'z': bit_num = zero; set_update(0); break;
  case 'o': bit_num = one;  set_update(1); break;
  case 't': switch(waitforkeypress_Basic()){
    case 'w': bit_num = two; set_update(2); break;
    case 'h': bit_num = three; set_update(3); break;
              default: update_reg = 1; break;}break;
  case 'f': switch(waitforkeypress_Basic()){
    case 'o': bit_num = four; set_update(4); break;
    case 'i': bit_num = five; set_update(5); break;
    default: update_reg = 1; break;}break;
  case 's': switch(waitforkeypress_Basic()){
    case 'i': bit_num = six; set_update(6); break;
    case 'e': bit_num = seven; set_update(7);break;
   default: update_reg = 1; break;} break;
 case 'x': for(int m = 0; m <=7; m++)reg_bkp[m] = 0; 
 default: update_reg = 1; break;}
    if(!(update_reg))return 1; else return 0; }






/**********************************************************************************************************/
