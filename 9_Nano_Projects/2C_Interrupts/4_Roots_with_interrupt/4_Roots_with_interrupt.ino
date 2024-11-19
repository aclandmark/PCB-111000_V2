

#include "Roots_with_interrrupt_header.h"
#include "Local_subroutines.c"

void Num_string_from_KBD_Local(char *);

#define message_1 "\r\nThis program calculates roots.\r\n\
Enter integer number\r\n?"

volatile long PORT_1 = 1;
volatile char num_present = 0;
volatile char ready_to_compute = 0;
long Num;
char Num_string[12];



int main (void)
{  setup_HW;

if(watch_dog_reset != 1)String_to_PC_Basic(message_1);
  
  sei();
  T1_clock_tick(150);

  while (1) {
    Num_string_from_KBD_Local(Num_string);
    Num_string_to_PC_Basic(Num_string);
    Char_to_PC_Basic('\t');
   while(!(ready_to_compute));
   ready_to_compute = 0;
   num_present = 0; 
   Num = Askii_to_binary_Local(Num_string);

    root_computation(Num, Num_string);
    Num_string_to_PC_Basic(Num_string);
    newline_Basic();
    Char_to_PC_Basic('?');
   }}



/********************************************************************************************************/
void T1_clock_tick(int T1_period_in_ms)                               //Start the T1 clock
{ TCNT1 = 0;
  OCR1A = T1_period_in_ms * 125;
  TIMSK1 |= (1 <<  OCIE1A);
  TCCR1B = 0x03;}


/********************************************************************************************************/
ISR(TIMER1_COMPA_vect)                                              //Example 1
{ TCNT1 = 0;
  if (PORT_1 < 0x10000)I2C_Tx_2_integers(PORT_1, PORT_1);
  PORT_1 = PORT_1 << 1;
  if (PORT_1 == 0x10000)PORT_1 = 1;
  if (num_present) {ready_to_compute = 1;}}



/********************************************************************************************************/
long Askii_to_binary_Local(char * array_ptr) {
  long num = 0;
{ int m = 0; while (*(array_ptr + m)) {
      num = num * 10 + (*(array_ptr + m++))  - '0';}}
  return num;}






  /*************************************************************************************/
