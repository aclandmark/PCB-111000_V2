



#include "Interrupt_basic_header.h"

volatile long PORT_1 = 1, PORT_2 = 0x8000;                            //For example 1, 2 and 3
volatile int m = 0, n = 0;                                            //Extras for example 3

volatile unsigned int PRN;                                            //For example 4
unsigned char PRN_counter = 0;                                        //For example 4
volatile unsigned char counter = 0;                                   //For example 4


 int main (void)   
  {
  setup_HW;

    sei();
   T1_clock_tick(150);
while(1);
   SW_reset;
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
 
if(PORT_1 < 0x10000)I2C_Tx_2_integers(PORT_1, PORT_1);
PORT_1 = PORT_1 << 1;
if (PORT_1 == 0x10000)PORT_1 = 1;}




/*
ISR(TIMER1_COMPA_vect)                                              //Example 1
{
 TCNT1 = 0; 
 
if(PORT_1 < 0x10000)I2C_Tx_2_integers(PORT_1, PORT_1);
PORT_1 = PORT_1 << 1;
if (PORT_1 == 0x10000)PORT_1 = 1;}




ISR(TIMER1_COMPA_vect)                                             //Example 2 
{
 TCNT1 = 0; 
     I2C_Tx_2_integers(PORT_1, PORT_2);
      PORT_1 = PORT_1 << 1;
      PORT_2 = PORT_2 >> 1;
   if (PORT_1 == 0x10000){PORT_1 = 1; PORT_2 = 0x8000;} 
  }
 

ISR(TIMER1_COMPA_vect)                                        //Example 3
  { 
 TCNT1 = 0; 
  I2C_Tx_2_integers(PORT_1 << m, PORT_2 >> m);
  
    if (!(n)) m += 1;
    if (m == 16)n = 1;
    if (n == 1)m -= 1;
    if (m == 0)n = 0;
  }
 
ISR(TIMER1_COMPA_vect)                                            //Example 4 
  { 
 TCNT1 = 0; 
   PRN_counter = counter;
  PRN = PRN_16bit_GEN (PRN, &PRN_counter);
  I2C_Tx_2_integers(PRN, (PRN << ((PRN % 2) + 1)));
    counter = PRN_counter;
    Int_to_PC_Basic(counter);
   }

*/





    
