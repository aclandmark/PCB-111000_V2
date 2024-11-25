#include "Real_num_division.h"



//*************Example 9: Arithmetic: More on pointers********************************************************
int main (void)                          //Example 9
  { char keyboard_input;
  char Num_string[12];
  long Num, A = 55; long B = 7; long Div; long mod;
  int no_decimal_places;

    setup_HW;
  String_to_PC_Basic("?\r\n");
  
  for(int m = 0; m <=2; m++)
  {Num = Int_from_PC_Basic(Num_string);
  switch (m)
  { case 0: A = Num; Char_to_PC_Basic('?');break;
  case 1: B = Num; String_to_PC_Basic("?\r\n");break;
  case 2: no_decimal_places = Num; break;}}

  divide(A, B, &Div, &mod, no_decimal_places);
  Long_Hex_and_Int_to_PC_Basic(10,Div);
  String_to_PC_Basic(". ");
    while (no_decimal_places) {
  no_decimal_places =    divide(mod*10, B, &Div, &mod, no_decimal_places);
   Char_to_PC_Basic(Div + '0');}
  String_to_PC_Basic("\r\n");
   SW_reset;
  return 1; }



/***********************************************************************************************/
  long divide(long A, long B, long *Div, long *mod, int no_decimal_places)
  {*Div = A/B;
  *mod = A%B;
  no_decimal_places -= 1;
  return no_decimal_places; } 


  /****************************************************************************************************/
