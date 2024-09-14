


/*******Example 8: Simple arithmetic Data to & from PC*********************************************************/
int main (void)                          //Example 8
{ long num = 0;
  char numLength;
  char Num_string[12];
  
  setup_HW;
  String_to_PC_Basic("?");
  Num_string_from_KBD_Basic(Num_string);

  { int m = 0; while (Num_string[m])
      Char_to_PC_Basic(Num_string[m++]);
  }

  num = Askii_to_binary(Num_string);

  num = num * 2;
  String_to_PC_Basic("\r\n");

  numLength = Binary_to_Askii(num, Num_string);

  for (int m = numLength; m > 0; m--)
    Char_to_PC_Basic(Num_string[m - 1]);

  I2C_Tx_long(num);
while(switch_1_up);
    newline_Basic();
  SW_reset;
  return 1;
}
