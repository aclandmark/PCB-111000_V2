
#include "Int_arithmetic.h"

/*******Example 8: Simple arithmetic Data to & from PC*********************************************************/
int main (void)                          //Example 8
{ long num_1, num_2;
 // char numLength;
 char Num_string[12];
  char op;
  
  setup_HW;
  String_to_PC_Basic("Enter positive integers and op");
  num_1 = Int_from_PC_Basic(Num_string);

while(1){
op = waitforkeypress_Basic();
num_2 = Int_from_PC_Basic(Num_string);
switch (op){
  case '+': num_1 = num_1 + num_2;break;
  case '-': num_1 = num_1 - num_2;break;
  case '*': num_1 = num_1 * num_2;break;
  case '/': num_1 = num_1 / num_2;break;
  }

  
  Char_to_PC_Basic('\t');

 Long_Hex_and_Int_to_PC_Basic(10, num_1);

  I2C_Tx_long(num_1);
//while(switch_1_up);
    newline_Basic();}
  SW_reset;
  return 1;
}
