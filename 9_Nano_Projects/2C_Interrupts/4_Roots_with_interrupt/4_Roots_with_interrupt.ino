

#include "Roots_with_interrrupt_header.h"
#include "Local_subroutines.c"

#define message_1 "\r\nThis program calculates roots iteratively.\r\n\
Enter scientific number then integer root?\r\n"
#define message_2 "\r\nEnter new data?\r\n"

volatile long PORT_1 = 1;
volatile char num_present;
long Num;
char Num_string[12];



int main (void)
{


  setup_HW;
  sei();
  T1_clock_tick(150);

  while (1) {
    Num_string_from_KBD_Local(Num_string);
    String_to_PC_Basic(Num_string);
    Char_to_PC_Basic('\t');
    //newline_Basic();
    
  }
}


void T1_clock_tick(int T1_period_in_ms)                               //Start the T1 clock
{ TCNT1 = 0;
  OCR1A = T1_period_in_ms * 125;
  TIMSK1 |= (1 <<  OCIE1A);
  TCCR1B = 0x03;
}



ISR(TIMER1_COMPA_vect)                                              //Example 1
{
  TCNT1 = 0;

  if (PORT_1 < 0x10000)I2C_Tx_2_integers(PORT_1, PORT_1);
  PORT_1 = PORT_1 << 1;
  if (PORT_1 == 0x10000)PORT_1 = 1;

  if (num_present) {
    num_present = 0; Num = Askii_to_binary_Local(Num_string);

    root_computation(Num, Num_string);String_to_PC_Basic(Num_string);newline_Basic();
  }


}



/***********************Subroutines*********************************/
void Num_string_from_KBD_Local(char * array_ptr)
{ char keypress;
  while ((keypress = waitforkeypress_Basic()) != '\r')
  { *array_ptr = keypress;
    //Char_to_PC_Basic(keypress);
    array_ptr += 1;
  }
  *array_ptr = '\0'; num_present = 1;
}



/********************************************************************************************************/
long Askii_to_binary_Local(char * array_ptr) {
  long num = 0;
{ int m = 0; while (*(array_ptr + m)) {
      num = num * 10 + (*(array_ptr + m++))  - '0';
    }
  }
  return num;
}






  /*************************************************************************************/
