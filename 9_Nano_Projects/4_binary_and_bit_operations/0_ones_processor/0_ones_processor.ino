
#include "0_ones_processor_header.h"
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

int main(void){
  unsigned int PORT_1 = 1;
char bit_num, bit_name;
 char digits[8];

setup_HW;
for(int m = 0; m <=7; m++)digits[m] = 0;
for(int m = 0; m <=7; m++)reg_bkp[m] = 0;
update_reg = 0;

//digits[0] = 1;
//digits[1] = 2;
//digits [2] = 3;
//I2C_Tx_BWops(digits);

//while(1);


while(1){

switch(waitforkeypress_Basic()){
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
    default: update_reg = 1; break;
}

if(!(update_reg))
{digits[0] |= (1 << bit_num);
I2C_Tx_BWops(digits);}
update_reg = 0;}

while(1);
}


char set_update(char bit_num)
{if(!(reg_bkp[bit_num])){update_reg = 0;reg_bkp[bit_num] = 1;} 
else update_reg = 1;return update_reg;}










/**********************************************************************************************************/
